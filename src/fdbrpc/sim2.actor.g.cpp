#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
/*
 * sim2.actor.cpp
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cinttypes>
#include <memory>
#include <string>
#include <utility>

#include "fmt/format.h"
#include "fdbrpc/simulator.h"
#define BOOST_SYSTEM_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#include "fdbrpc/SimExternalConnection.h"
#include "flow/ActorCollection.h"
#include "flow/IRandom.h"
#include "flow/IThreadPool.h"
#include "flow/ProtocolVersion.h"
#include "flow/Util.h"
#include "flow/WriteOnlySet.h"
#include "fdbrpc/IAsyncFile.h"
#include "fdbrpc/AsyncFileCached.actor.h"
#include "fdbrpc/AsyncFileEncrypted.h"
#include "fdbrpc/AsyncFileNonDurable.actor.h"
#include "fdbrpc/AsyncFileChaos.actor.h"
#include "flow/crc32c.h"
#include "fdbrpc/TraceFileIO.h"
#include "flow/flow.h"
#include "flow/genericactors.actor.h"
#include "flow/network.h"
#include "flow/TLSConfig.actor.h"
#include "fdbrpc/Net2FileSystem.h"
#include "fdbrpc/Replication.h"
#include "fdbrpc/ReplicationUtils.h"
#include "fdbrpc/AsyncFileWriteChecker.h"
#include "fdbrpc/genericactors.actor.h"
#include "flow/FaultInjection.h"
#include "flow/actorcompiler.h" // This must be the last #include.

bool simulator_should_inject_fault(const char* context, const char* file, int line, int error_code) {
	if (!g_network->isSimulated() || !faultInjectionActivated)
		return false;

	auto p = g_simulator.getCurrentProcess();

	if (p->fault_injection_p2 && deterministicRandom()->random01() < p->fault_injection_p2 &&
	    !g_simulator.speedUpSimulation) {
		uint32_t h1 = line + (p->fault_injection_r >> 32);

		if (h1 < p->fault_injection_p1 * std::numeric_limits<uint32_t>::max()) {
			TEST(true); // A fault was injected
			TEST(error_code == error_code_io_timeout); // An io timeout was injected
			TEST(error_code == error_code_io_error); // An io error was injected
			TEST(error_code == error_code_platform_error); // A platform error was injected.
			TraceEvent(SevWarn, "FaultInjected")
			    .detail("Context", context)
			    .detail("File", file)
			    .detail("Line", line)
			    .detail("ErrorCode", error_code);
			if (error_code == error_code_io_timeout) {
				g_network->setGlobal(INetwork::enASIOTimedOut, (flowGlobalType) true);
			}
			return true;
		}
	}

	return false;
}

void ISimulator::displayWorkers() const {
	std::map<std::string, std::vector<ISimulator::ProcessInfo*>> machineMap;

	// Create a map of machine Id
	for (auto processInfo : getAllProcesses()) {
		std::string dataHall = processInfo->locality.dataHallId().present()
		                           ? processInfo->locality.dataHallId().get().printable()
		                           : "[unset]";
		std::string machineId = processInfo->locality.machineId().present()
		                            ? processInfo->locality.machineId().get().printable()
		                            : "[unset]";
		machineMap[format("%-8s  %s", dataHall.c_str(), machineId.c_str())].push_back(processInfo);
	}

	printf("DataHall  MachineId\n");
	printf("                  Address   Name      Class        Excluded Failed Rebooting Cleared Role                  "
	       "                            DataFolder\n");
	for (auto& machineRecord : machineMap) {
		printf("\n%s\n", machineRecord.first.c_str());
		for (auto& processInfo : machineRecord.second) {
			printf("                  %9s %-10s%-13s%-8s %-6s %-9s %-8s %-48s %-40s\n",
			       processInfo->address.toString().c_str(),
			       processInfo->name,
			       processInfo->startingClass.toString().c_str(),
			       (processInfo->isExcluded() ? "True" : "False"),
			       (processInfo->failed ? "True" : "False"),
			       (processInfo->rebooting ? "True" : "False"),
			       (processInfo->isCleared() ? "True" : "False"),
			       getRoles(processInfo->address).c_str(),
			       processInfo->dataFolder);
		}
	}

	return;
}

int openCount = 0;

struct SimClogging {
	double getSendDelay(NetworkAddress from, NetworkAddress to, bool stableConnection = false) const {
		// stable connection here means it's a local connection between processes on the same machine
		// we expect it to have much lower latency
		return (stableConnection ? 0.1 : 1.0) * halfLatency();
	}

	double getRecvDelay(NetworkAddress from, NetworkAddress to, bool stableConnection = false) {
		auto pair = std::make_pair(from.ip, to.ip);

		double tnow = now();
		double t = tnow + (stableConnection ? 0.1 : 1.0) * halfLatency();
		if (!g_simulator.speedUpSimulation && !stableConnection)
			t += clogPairLatency[pair];

		if (!g_simulator.speedUpSimulation && !stableConnection && clogPairUntil.count(pair))
			t = std::max(t, clogPairUntil[pair]);

		auto p = std::make_pair(from, to);
		if (!g_simulator.speedUpSimulation && !stableConnection && clogProcessPairUntil.count(p))
			t = std::max(t, clogProcessPairUntil[p]);

		if (!g_simulator.speedUpSimulation && !stableConnection && clogRecvUntil.count(to.ip))
			t = std::max(t, clogRecvUntil[to.ip]);

		return t - tnow;
	}

	bool disconnected(const IPAddress& from, const IPAddress& to) {
		auto pair = std::make_pair(from, to);
		if (g_simulator.speedUpSimulation || disconnectPairUntil.find(pair) == disconnectPairUntil.end()) {
			return false;
		}

		double disconnectUntil = disconnectPairUntil[pair];
		return now() < disconnectUntil;
	}

	void clogPairFor(const IPAddress& from, const IPAddress& to, double t) {
		auto& u = clogPairUntil[std::make_pair(from, to)];
		u = std::max(u, now() + t);
	}

	void unclogPair(const IPAddress& from, const IPAddress& to) {
		auto pair = std::make_pair(from, to);
		clogPairUntil.erase(pair);
		clogPairLatency.erase(pair);
	}

	// Clog a pair of processes until a time. This is more fine-grained than
	// the IPAddress based one.
	void clogPairFor(const NetworkAddress& from, const NetworkAddress& to, double t) {
		auto& u = clogProcessPairUntil[std::make_pair(from, to)];
		u = std::max(u, now() + t);
	}

	void clogSendFor(const IPAddress& from, double t) {
		auto& u = clogSendUntil[from];
		u = std::max(u, now() + t);
	}
	void clogRecvFor(const IPAddress& from, double t) {
		auto& u = clogRecvUntil[from];
		u = std::max(u, now() + t);
	}
	double setPairLatencyIfNotSet(const IPAddress& from, const IPAddress& to, double t) {
		auto i = clogPairLatency.find(std::make_pair(from, to));
		if (i == clogPairLatency.end())
			i = clogPairLatency.insert(std::make_pair(std::make_pair(from, to), t)).first;
		return i->second;
	}

	void disconnectPairFor(const IPAddress& from, const IPAddress& to, double t) {
		auto& u = disconnectPairUntil[std::make_pair(from, to)];
		u = std::max(u, now() + t);
	}

	void reconnectPair(const IPAddress& from, const IPAddress& to) {
		disconnectPairUntil.erase(std::make_pair(from, to));
	}

private:
	std::map<IPAddress, double> clogSendUntil, clogRecvUntil;
	std::map<std::pair<IPAddress, IPAddress>, double> clogPairUntil;
	std::map<std::pair<IPAddress, IPAddress>, double> clogPairLatency;
	std::map<std::pair<NetworkAddress, NetworkAddress>, double> clogProcessPairUntil;
	std::map<std::pair<IPAddress, IPAddress>, double> disconnectPairUntil;

	double halfLatency() const {
		double a = deterministicRandom()->random01();
		const double pFast = 0.999;
		if (a <= pFast || g_simulator.speedUpSimulation) {
			a = a / pFast;
			return 0.5 * (FLOW_KNOBS->MIN_NETWORK_LATENCY * (1 - a) +
			              FLOW_KNOBS->FAST_NETWORK_LATENCY / pFast * a); // 0.5ms average
		} else {
			a = (a - pFast) / (1 - pFast); // uniform 0-1 again
			return 0.5 * (FLOW_KNOBS->MIN_NETWORK_LATENCY * (1 - a) +
			              FLOW_KNOBS->SLOW_NETWORK_LATENCY * a); // long tail up to X ms
		}
	}
};

SimClogging g_clogging;

struct Sim2Conn final : IConnection, ReferenceCounted<Sim2Conn> {
	Sim2Conn(ISimulator::ProcessInfo* process)
	  : opened(false), closedByCaller(false), stableConnection(false), process(process),
	    dbgid(deterministicRandom()->randomUniqueID()), stopReceive(Never()) {
		pipes = sender(this) && receiver(this);
	}

	// connect() is called on a pair of connections immediately after creation; logically it is part of the constructor
	// and no other method may be called previously!
	void connect(Reference<Sim2Conn> peer, NetworkAddress peerEndpoint) {
		this->peer = peer;
		this->peerProcess = peer->process;
		this->peerId = peer->dbgid;
		this->peerEndpoint = peerEndpoint;

		// Every one-way connection gets a random permanent latency and a random send buffer for the duration of the
		// connection
		auto latency =
		    g_clogging.setPairLatencyIfNotSet(peerProcess->address.ip,
		                                      process->address.ip,
		                                      FLOW_KNOBS->MAX_CLOGGING_LATENCY * deterministicRandom()->random01());
		sendBufSize = std::max<double>(deterministicRandom()->randomInt(0, 5000000), 25e6 * (latency + .002));
		// options like clogging or bitsflip are disabled for stable connections
		stableConnection = std::any_of(process->childs.begin(),
		                               process->childs.end(),
		                               [&](ISimulator::ProcessInfo* child) { return child && child == peerProcess; }) ||
		                   std::any_of(peerProcess->childs.begin(),
		                               peerProcess->childs.end(),
		                               [&](ISimulator::ProcessInfo* child) { return child && child == process; });

		if (g_clogging.disconnected(process->address.ip, peerProcess->address.ip)) {
			TraceEvent("SimulatedDisconnection")
			    .detail("Phase", "Connect")
			    .detail("Address", process->address)
			    .detail("Peer", peerProcess->address);
			throw connection_failed();
		}

		TraceEvent("Sim2Connection")
		    .detail("SendBufSize", sendBufSize)
		    .detail("Latency", latency)
		    .detail("StableConnection", stableConnection);
	}

	~Sim2Conn() { ASSERT_ABORT(!opened || closedByCaller); }

	void addref() override { ReferenceCounted<Sim2Conn>::addref(); }
	void delref() override { ReferenceCounted<Sim2Conn>::delref(); }
	void close() override {
		closedByCaller = true;
		closeInternal();
	}

	Future<Void> acceptHandshake() override { return delay(0.01 * deterministicRandom()->random01()); }
	Future<Void> connectHandshake() override { return delay(0.01 * deterministicRandom()->random01()); }

	Future<Void> onWritable() override { return whenWritable(this); }
	Future<Void> onReadable() override { return whenReadable(this); }

	bool isPeerGone() const { return !peer || peerProcess->failed; }

	bool isStableConnection() const override { return stableConnection; }

	void peerClosed() {
		leakedConnectionTracker = trackLeakedConnection(this);
		stopReceive = delay(1.0);
	}

	// Reads as many bytes as possible from the read buffer into [begin,end) and returns the number of bytes read (might
	// be 0) (or may throw an error if the connection dies)
	int read(uint8_t* begin, uint8_t* end) override {
		rollRandomClose();

		int64_t avail = receivedBytes.get() - readBytes.get(); // SOMEDAY: random?
		int toRead = std::min<int64_t>(end - begin, avail);
		ASSERT(toRead >= 0 && toRead <= recvBuf.size() && toRead <= end - begin);
		for (int i = 0; i < toRead; i++)
			begin[i] = recvBuf[i];
		recvBuf.erase(recvBuf.begin(), recvBuf.begin() + toRead);
		readBytes.set(readBytes.get() + toRead);
		return toRead;
	}

	// Writes as many bytes as possible from the given SendBuffer chain into the write buffer and returns the number of
	// bytes written (might be 0) (or may throw an error if the connection dies)
	int write(SendBuffer const* buffer, int limit) override {
		rollRandomClose();
		ASSERT(limit > 0);

		int toSend = 0;
		if (BUGGIFY && !stableConnection) {
			toSend = std::min(limit, buffer->bytes_written - buffer->bytes_sent);
		} else {
			for (auto p = buffer; p; p = p->next) {
				toSend += p->bytes_written - p->bytes_sent;
				if (toSend >= limit) {
					if (toSend > limit)
						toSend = limit;
					break;
				}
			}
		}
		ASSERT(toSend);
		if (BUGGIFY && !stableConnection)
			toSend = std::min(toSend, deterministicRandom()->randomInt(0, 1000));

		if (!peer)
			return toSend;
		toSend = std::min(toSend, peer->availableSendBufferForPeer());
		ASSERT(toSend >= 0);

		int leftToSend = toSend;
		for (auto p = buffer; p && leftToSend > 0; p = p->next) {
			int ts = std::min(leftToSend, p->bytes_written - p->bytes_sent);
			peer->recvBuf.insert(peer->recvBuf.end(), p->data() + p->bytes_sent, p->data() + p->bytes_sent + ts);
			leftToSend -= ts;
		}
		ASSERT(leftToSend == 0);
		peer->writtenBytes.set(peer->writtenBytes.get() + toSend);
		return toSend;
	}

	// Returns the network address and port of the other end of the connection.  In the case of an incoming connection,
	// this may not be an address we can connect to!
	NetworkAddress getPeerAddress() const override { return peerEndpoint; }
	UID getDebugID() const override { return dbgid; }

	boost::asio::ip::tcp::socket& getSocket() override { throw operation_failed(); }

	bool opened, closedByCaller, stableConnection;

private:
	ISimulator::ProcessInfo *process, *peerProcess;
	UID dbgid, peerId;
	NetworkAddress peerEndpoint;
	std::deque<uint8_t> recvBuf; // Includes bytes written but not yet received!
	AsyncVar<int64_t> readBytes, // bytes already pulled from recvBuf (location of the beginning of recvBuf)
	    receivedBytes, sentBytes,
	    writtenBytes; // location of the end of recvBuf ( == recvBuf.size() + readBytes.get() )
	Reference<Sim2Conn> peer;
	int sendBufSize;

	Future<Void> leakedConnectionTracker;

	Future<Void> pipes;
	Future<Void> stopReceive;

	int availableSendBufferForPeer() const {
		return sendBufSize - (writtenBytes.get() - receivedBytes.get());
	} // SOMEDAY: acknowledgedBytes instead of receivedBytes

	void closeInternal() {
		if (peer) {
			peer->peerClosed();
			stopReceive = delay(1.0);
		}
		leakedConnectionTracker.cancel();
		peer.clear();
	}

																#line 391 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via sender()
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class SenderActor>
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class SenderActorState {
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SenderActorState(Sim2Conn* const& self) 
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 404 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("sender", reinterpret_cast<unsigned long>(this));

	}
	~SenderActorState() 
	{
		fdb_probe_actor_destroy("sender", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 390 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			;
															#line 419 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~SenderActorState();
		static_cast<SenderActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 391 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->writtenBytes.onChange();
															#line 391 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<SenderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 451 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<SenderActor*>(this)->actor_wait_state = 1;
															#line 391 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SenderActor, 0, Void >*>(static_cast<SenderActor*>(this)));
															#line 456 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 392 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->peerProcess);
															#line 393 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(.002 * deterministicRandom()->random01());
															#line 393 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<SenderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 469 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SenderActor*>(this)->actor_wait_state = 2;
															#line 393 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SenderActor, 1, Void >*>(static_cast<SenderActor*>(this)));
															#line 474 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 392 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->peerProcess);
															#line 393 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(.002 * deterministicRandom()->random01());
															#line 393 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<SenderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 487 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SenderActor*>(this)->actor_wait_state = 2;
															#line 393 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SenderActor, 1, Void >*>(static_cast<SenderActor*>(this)));
															#line 492 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SenderActor*>(this)->actor_wait_state > 0) static_cast<SenderActor*>(this)->actor_wait_state = 0;
		static_cast<SenderActor*>(this)->ActorCallback< SenderActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SenderActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SenderActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SenderActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 394 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->sentBytes.set(self->writtenBytes.get());
															#line 564 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 394 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->sentBytes.set(self->writtenBytes.get());
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SenderActor*>(this)->actor_wait_state > 0) static_cast<SenderActor*>(this)->actor_wait_state = 0;
		static_cast<SenderActor*>(this)->ActorCallback< SenderActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SenderActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SenderActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SenderActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 643 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via sender()
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class SenderActor final : public Actor<Void>, public ActorCallback< SenderActor, 0, Void >, public ActorCallback< SenderActor, 1, Void >, public FastAllocated<SenderActor>, public SenderActorState<SenderActor> {
															#line 648 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<SenderActor>::operator new;
	using FastAllocated<SenderActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SenderActor, 0, Void >;
friend struct ActorCallback< SenderActor, 1, Void >;
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SenderActor(Sim2Conn* const& self) 
															#line 660 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   SenderActorState<SenderActor>(self)
	{
		fdb_probe_actor_enter("sender", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sender");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sender", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SenderActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SenderActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> sender( Sim2Conn* const& self ) {
															#line 389 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new SenderActor(self));
															#line 688 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
																#line 692 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via receiver()
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class ReceiverActor>
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class ReceiverActorState {
															#line 698 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ReceiverActorState(Sim2Conn* const& self) 
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 705 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("receiver", reinterpret_cast<unsigned long>(this));

	}
	~ReceiverActorState() 
	{
		fdb_probe_actor_destroy("receiver", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 398 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			;
															#line 720 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~ReceiverActorState();
		static_cast<ReceiverActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 399 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->sentBytes.get() != self->receivedBytes.get())
															#line 750 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 400 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator.onProcess(self->peerProcess);
															#line 400 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 756 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReceiverActor*>(this)->actor_wait_state = 1;
															#line 400 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 0, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 761 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 401 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		;
															#line 775 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 403 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->peerProcess);
															#line 406 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (g_clogging.disconnected(self->peerProcess->address.ip, self->process->address.ip))
															#line 861 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 407 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent("SimulatedDisconnection") .detail("Phase", "DataTransfer") .detail("Sender", self->peerProcess->address) .detail("Receiver", self->process->address);
															#line 411 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 1));
															#line 867 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 414 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		pos = deterministicRandom()->random01() < .5 ? self->sentBytes.get() : deterministicRandom()->randomInt64(self->receivedBytes.get(), self->sentBytes.get() + 1);
															#line 418 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(g_clogging.getSendDelay( self->peerProcess->address, self->process->address, self->isStableConnection()));
															#line 418 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 875 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 3;
															#line 418 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 2, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 880 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1(int loopDepth) 
	{
															#line 401 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!(self->sentBytes.get() == self->receivedBytes.get()))
															#line 896 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 402 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->sentBytes.onChange();
															#line 402 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 904 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 2;
															#line 402 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 1, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 909 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(self->process);
															#line 420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1008 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 4;
															#line 420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 3, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1013 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(self->process);
															#line 420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1024 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 4;
															#line 420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 3, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1029 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 421 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 422 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(g_clogging.getRecvDelay( self->peerProcess->address, self->process->address, self->isStableConnection()));
															#line 422 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1105 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6when1(__when_expr_4.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 5;
															#line 422 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 4, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1110 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 421 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 422 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(g_clogging.getRecvDelay( self->peerProcess->address, self->process->address, self->isStableConnection()));
															#line 422 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1123 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6when1(__when_expr_4.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 5;
															#line 422 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 4, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1128 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 424 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 425 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->stopReceive.isReady())
															#line 1202 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_5 = Future<Void>(Never());
															#line 426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1208 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_5.get(), loopDepth); };
			static_cast<ReceiverActor*>(this)->actor_wait_state = 6;
															#line 426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 5, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1213 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 424 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 425 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->stopReceive.isReady())
															#line 1229 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_5 = Future<Void>(Never());
															#line 426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1235 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_5.get(), loopDepth); };
			static_cast<ReceiverActor*>(this)->actor_wait_state = 6;
															#line 426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 5, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1240 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont8(int loopDepth) 
	{
															#line 428 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->receivedBytes.set(pos);
															#line 429 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_6 = Future<Void>(Void());
															#line 429 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1321 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont8when1(__when_expr_6.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 7;
															#line 429 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 6, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1326 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
															#line 430 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 1410 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 430 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 1419 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont8when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<ReceiverActor*>(this)->actor_wait_state > 0) static_cast<ReceiverActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiverActor*>(this)->ActorCallback< ReceiverActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ReceiverActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< ReceiverActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 414 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int64_t pos;
															#line 1491 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via receiver()
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class ReceiverActor final : public Actor<Void>, public ActorCallback< ReceiverActor, 0, Void >, public ActorCallback< ReceiverActor, 1, Void >, public ActorCallback< ReceiverActor, 2, Void >, public ActorCallback< ReceiverActor, 3, Void >, public ActorCallback< ReceiverActor, 4, Void >, public ActorCallback< ReceiverActor, 5, Void >, public ActorCallback< ReceiverActor, 6, Void >, public FastAllocated<ReceiverActor>, public ReceiverActorState<ReceiverActor> {
															#line 1496 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<ReceiverActor>::operator new;
	using FastAllocated<ReceiverActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReceiverActor, 0, Void >;
friend struct ActorCallback< ReceiverActor, 1, Void >;
friend struct ActorCallback< ReceiverActor, 2, Void >;
friend struct ActorCallback< ReceiverActor, 3, Void >;
friend struct ActorCallback< ReceiverActor, 4, Void >;
friend struct ActorCallback< ReceiverActor, 5, Void >;
friend struct ActorCallback< ReceiverActor, 6, Void >;
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ReceiverActor(Sim2Conn* const& self) 
															#line 1513 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   ReceiverActorState<ReceiverActor>(self)
	{
		fdb_probe_actor_enter("receiver", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("receiver");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("receiver", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReceiverActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReceiverActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReceiverActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ReceiverActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ReceiverActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ReceiverActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< ReceiverActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> receiver( Sim2Conn* const& self ) {
															#line 397 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new ReceiverActor(self));
															#line 1546 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
																#line 1550 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via whenReadable()
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class WhenReadableActor>
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class WhenReadableActorState {
															#line 1556 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	WhenReadableActorState(Sim2Conn* const& self) 
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 1563 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("whenReadable", reinterpret_cast<unsigned long>(this));

	}
	~WhenReadableActorState() 
	{
		fdb_probe_actor_destroy("whenReadable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 435 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				;
															#line 1579 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~WhenReadableActorState();
		static_cast<WhenReadableActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 444 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 445 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1611 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 436 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->readBytes.get() != self->receivedBytes.get())
															#line 1632 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 437 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 438 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (!static_cast<WhenReadableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WhenReadableActorState(); static_cast<WhenReadableActor*>(this)->destroy(); return 0; }
															#line 1638 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			new (&static_cast<WhenReadableActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WhenReadableActorState();
			static_cast<WhenReadableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 440 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->receivedBytes.onChange();
															#line 440 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<WhenReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1648 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WhenReadableActor*>(this)->actor_wait_state = 1;
															#line 440 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WhenReadableActor, 0, Void >*>(static_cast<WhenReadableActor*>(this)));
															#line 1653 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 441 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->rollRandomClose();
															#line 1662 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 441 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->rollRandomClose();
															#line 1671 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WhenReadableActor*>(this)->actor_wait_state > 0) static_cast<WhenReadableActor*>(this)->actor_wait_state = 0;
		static_cast<WhenReadableActor*>(this)->ActorCallback< WhenReadableActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WhenReadableActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("whenReadable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenReadable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WhenReadableActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("whenReadable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenReadable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WhenReadableActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("whenReadable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenReadable", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 1741 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via whenReadable()
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class WhenReadableActor final : public Actor<Void>, public ActorCallback< WhenReadableActor, 0, Void >, public FastAllocated<WhenReadableActor>, public WhenReadableActorState<WhenReadableActor> {
															#line 1746 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<WhenReadableActor>::operator new;
	using FastAllocated<WhenReadableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WhenReadableActor, 0, Void >;
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	WhenReadableActor(Sim2Conn* const& self) 
															#line 1757 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   WhenReadableActorState<WhenReadableActor>(self)
	{
		fdb_probe_actor_enter("whenReadable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("whenReadable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("whenReadable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WhenReadableActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> whenReadable( Sim2Conn* const& self ) {
															#line 433 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new WhenReadableActor(self));
															#line 1784 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
																#line 1788 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via whenWritable()
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class WhenWritableActor>
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class WhenWritableActorState {
															#line 1794 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	WhenWritableActorState(Sim2Conn* const& self) 
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 1801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("whenWritable", reinterpret_cast<unsigned long>(this));

	}
	~WhenWritableActorState() 
	{
		fdb_probe_actor_destroy("whenWritable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 450 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				;
															#line 1817 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~WhenWritableActorState();
		static_cast<WhenWritableActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 467 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 468 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1849 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 451 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!self->peer)
															#line 1870 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 452 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (!static_cast<WhenWritableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WhenWritableActorState(); static_cast<WhenWritableActor*>(this)->destroy(); return 0; }
															#line 1874 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			new (&static_cast<WhenWritableActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WhenWritableActorState();
			static_cast<WhenWritableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 453 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->peer->availableSendBufferForPeer() > 0)
															#line 1882 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 454 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator.getCurrentProcess() == self->process);
															#line 455 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (!static_cast<WhenWritableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WhenWritableActorState(); static_cast<WhenWritableActor*>(this)->destroy(); return 0; }
															#line 1888 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			new (&static_cast<WhenWritableActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WhenWritableActorState();
			static_cast<WhenWritableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 458 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->peer->receivedBytes.onChange();
															#line 458 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<WhenWritableActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1899 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WhenWritableActor*>(this)->actor_wait_state = 1;
															#line 458 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WhenWritableActor, 0, Void >*>(static_cast<WhenWritableActor*>(this)));
															#line 1904 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 464 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_simulator.onProcess(self->process);
															#line 464 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<WhenWritableActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1921 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WhenWritableActor*>(this)->actor_wait_state = 2;
															#line 464 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WhenWritableActor, 1, Void >*>(static_cast<WhenWritableActor*>(this)));
															#line 1926 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 461 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (e.code() != error_code_broken_promise)
															#line 1936 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 462 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				return a_body1Catch2(e, std::max(0, loopDepth - 1));
															#line 1940 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 459 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->peerProcess);
															#line 1956 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 459 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator.getCurrentProcess() == self->peerProcess);
															#line 1965 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WhenWritableActor*>(this)->actor_wait_state > 0) static_cast<WhenWritableActor*>(this)->actor_wait_state = 0;
		static_cast<WhenWritableActor*>(this)->ActorCallback< WhenWritableActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WhenWritableActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WhenWritableActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WhenWritableActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WhenWritableActor*>(this)->actor_wait_state > 0) static_cast<WhenWritableActor*>(this)->actor_wait_state = 0;
		static_cast<WhenWritableActor*>(this)->ActorCallback< WhenWritableActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WhenWritableActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WhenWritableActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WhenWritableActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 2123 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via whenWritable()
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class WhenWritableActor final : public Actor<Void>, public ActorCallback< WhenWritableActor, 0, Void >, public ActorCallback< WhenWritableActor, 1, Void >, public FastAllocated<WhenWritableActor>, public WhenWritableActorState<WhenWritableActor> {
															#line 2128 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<WhenWritableActor>::operator new;
	using FastAllocated<WhenWritableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WhenWritableActor, 0, Void >;
friend struct ActorCallback< WhenWritableActor, 1, Void >;
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	WhenWritableActor(Sim2Conn* const& self) 
															#line 2140 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   WhenWritableActorState<WhenWritableActor>(self)
	{
		fdb_probe_actor_enter("whenWritable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("whenWritable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("whenWritable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WhenWritableActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WhenWritableActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> whenWritable( Sim2Conn* const& self ) {
															#line 448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new WhenWritableActor(self));
															#line 2168 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 471 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

	void rollRandomClose() {
		// make sure connections between parenta and their childs are not closed
		if (!stableConnection &&
		    now() - g_simulator.lastConnectionFailure > g_simulator.connectionFailuresDisableDuration &&
		    deterministicRandom()->random01() < .00001) {
			g_simulator.lastConnectionFailure = now();
			double a = deterministicRandom()->random01(), b = deterministicRandom()->random01();
			TEST(true); // Simulated connection failure
			TraceEvent("ConnectionFailure", dbgid)
			    .detail("MyAddr", process->address)
			    .detail("PeerAddr", peerProcess->address)
			    .detail("PeerIsValid", peer.isValid())
			    .detail("SendClosed", a > .33)
			    .detail("RecvClosed", a < .66)
			    .detail("Explicit", b < .3);
			if (a < .66 && peer)
				peer->closeInternal();
			if (a > .33)
				closeInternal();
			// At the moment, we occasionally notice the connection failed immediately.  In principle, this could happen
			// but only after a delay.
			if (b < .3)
				throw connection_failed();
		}
	}

																#line 2199 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via trackLeakedConnection()
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class TrackLeakedConnectionActor>
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class TrackLeakedConnectionActorState {
															#line 2205 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	TrackLeakedConnectionActorState(Sim2Conn* const& self) 
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 2212 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("trackLeakedConnection", reinterpret_cast<unsigned long>(this));

	}
	~TrackLeakedConnectionActorState() 
	{
		fdb_probe_actor_destroy("trackLeakedConnection", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 499 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator.onProcess(self->process);
															#line 499 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2229 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 1;
															#line 499 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 0, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2234 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~TrackLeakedConnectionActorState();
		static_cast<TrackLeakedConnectionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 500 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->process->address.isPublic())
															#line 2257 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 501 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5 + FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME * 2.1 + FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 501 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2263 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 2;
															#line 501 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 1, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2268 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 504 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5);
															#line 504 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2277 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 3;
															#line 504 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 2, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2282 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 500 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->process->address.isPublic())
															#line 2292 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 501 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5 + FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME * 2.1 + FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 501 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2298 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 2;
															#line 501 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 1, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2303 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 504 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5);
															#line 504 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2312 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 3;
															#line 504 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 2, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2317 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state > 0) static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 0;
		static_cast<TrackLeakedConnectionActor*>(this)->ActorCallback< TrackLeakedConnectionActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TrackLeakedConnectionActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TrackLeakedConnectionActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TrackLeakedConnectionActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 506 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		TraceEvent(SevError, "LeakedConnection", self->dbgid) .error(connection_leaked()) .detail("MyAddr", self->process->address) .detail("PeerAddr", self->peerEndpoint) .detail("PeerId", self->peerId) .detail("Opened", self->opened);
															#line 512 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<TrackLeakedConnectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TrackLeakedConnectionActorState(); static_cast<TrackLeakedConnectionActor*>(this)->destroy(); return 0; }
															#line 2392 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<TrackLeakedConnectionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TrackLeakedConnectionActorState();
		static_cast<TrackLeakedConnectionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state > 0) static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 0;
		static_cast<TrackLeakedConnectionActor*>(this)->ActorCallback< TrackLeakedConnectionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TrackLeakedConnectionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TrackLeakedConnectionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TrackLeakedConnectionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state > 0) static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 0;
		static_cast<TrackLeakedConnectionActor*>(this)->ActorCallback< TrackLeakedConnectionActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TrackLeakedConnectionActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TrackLeakedConnectionActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TrackLeakedConnectionActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 2552 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via trackLeakedConnection()
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class TrackLeakedConnectionActor final : public Actor<Void>, public ActorCallback< TrackLeakedConnectionActor, 0, Void >, public ActorCallback< TrackLeakedConnectionActor, 1, Void >, public ActorCallback< TrackLeakedConnectionActor, 2, Void >, public FastAllocated<TrackLeakedConnectionActor>, public TrackLeakedConnectionActorState<TrackLeakedConnectionActor> {
															#line 2557 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<TrackLeakedConnectionActor>::operator new;
	using FastAllocated<TrackLeakedConnectionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TrackLeakedConnectionActor, 0, Void >;
friend struct ActorCallback< TrackLeakedConnectionActor, 1, Void >;
friend struct ActorCallback< TrackLeakedConnectionActor, 2, Void >;
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	TrackLeakedConnectionActor(Sim2Conn* const& self) 
															#line 2570 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   TrackLeakedConnectionActorState<TrackLeakedConnectionActor>(self)
	{
		fdb_probe_actor_enter("trackLeakedConnection", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("trackLeakedConnection");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("trackLeakedConnection", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TrackLeakedConnectionActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TrackLeakedConnectionActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TrackLeakedConnectionActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> trackLeakedConnection( Sim2Conn* const& self ) {
															#line 498 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new TrackLeakedConnectionActor(self));
															#line 2599 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 514 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
};

#include <fcntl.h>
#include <sys/stat.h>

int sf_open(const char* filename, int flags, int convFlags, int mode);

#if defined(_WIN32)
#include <io.h>
#define O_CLOEXEC 0

#elif defined(__unixish__)
#define _open ::open
#define _read ::read
#define _write ::write
#define _close ::close
#define _lseeki64 ::lseek
#define _commit ::fsync
#define _chsize ::ftruncate
#define O_BINARY 0

int sf_open(const char* filename, int flags, int convFlags, int mode) {
	return _open(filename, convFlags, mode);
}

#else
#error How do i open a file on a new platform?
#endif

class SimpleFile : public IAsyncFile, public ReferenceCounted<SimpleFile> {
public:
	static void init() {}

	static bool should_poll() { return false; }

																#line 2638 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via open()
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class OpenActor>
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class OpenActorState {
															#line 2644 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	OpenActorState(std::string const& filename,int const& flags,int const& mode,Reference<DiskParameters> const& diskParameters = makeReference<DiskParameters>(25000, 150000000),bool const& delayOnWrite = true) 
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : filename(filename),
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   flags(flags),
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   mode(mode),
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   diskParameters(diskParameters),
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   delayOnWrite(delayOnWrite),
															#line 555 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 556 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   currentTaskID(g_network->getCurrentTask())
															#line 2663 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("open", reinterpret_cast<unsigned long>(this));

	}
	~OpenActorState() 
	{
		fdb_probe_actor_destroy("open", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 558 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (++openCount >= 3000)
															#line 2678 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 559 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevError, "TooManyFiles").log();
															#line 560 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				ASSERT(false);
															#line 2684 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 563 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (openCount == 2000)
															#line 2688 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 564 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				disableConnectionFailures("TooManyFiles");
															#line 2692 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 569 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(basename(filename).size() < 250);
															#line 571 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 571 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2700 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 1;
															#line 571 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 0, Void >*>(static_cast<OpenActor*>(this)));
															#line 2705 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->MIN_OPEN_TIME + deterministicRandom()->random01() * (FLOW_KNOBS->MAX_OPEN_TIME - FLOW_KNOBS->MIN_OPEN_TIME));
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2731 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 2;
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 1, Void >*>(static_cast<OpenActor*>(this)));
															#line 2736 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->MIN_OPEN_TIME + deterministicRandom()->random01() * (FLOW_KNOBS->MAX_OPEN_TIME - FLOW_KNOBS->MIN_OPEN_TIME));
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2754 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 2;
															#line 573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 1, Void >*>(static_cast<OpenActor*>(this)));
															#line 2759 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OpenActor*>(this)->actor_wait_state > 0) static_cast<OpenActor*>(this)->actor_wait_state = 0;
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OpenActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OpenActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 600 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			err = e;
															#line 601 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 601 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2842 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 4;
															#line 601 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 3, Void >*>(static_cast<OpenActor*>(this)));
															#line 2847 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 576 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		std::string open_filename = filename;
															#line 577 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 2864 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 578 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT((flags & OPEN_CREATE) && (flags & OPEN_READWRITE) && !(flags & OPEN_EXCLUSIVE));
															#line 579 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			open_filename = filename + ".part";
															#line 2870 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 582 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		int h = sf_open(open_filename.c_str(), flags, flagConversion(flags), mode);
															#line 583 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (h == -1)
															#line 2876 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 584 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			bool notFound = errno == ENOENT;
															#line 585 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			Error e = notFound ? file_not_found() : io_error();
															#line 586 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(notFound ? SevWarn : SevWarnAlways, "FileOpenError") .error(e) .GetLastError() .detail("File", filename) .detail("Flags", flags);
															#line 591 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1cont1Catch1(e, loopDepth);
															#line 2886 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 594 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		platform::makeTemporary(open_filename.c_str());
															#line 595 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		SimpleFile* simpleFile = new SimpleFile(h, diskParameters, delayOnWrite, filename, open_filename, flags);
															#line 596 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		file = Reference<IAsyncFile>(simpleFile);
															#line 597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_2 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<OpenActor*>(this)->actor_wait_state = 3;
															#line 597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 2, Void >*>(static_cast<OpenActor*>(this)));
															#line 2903 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 576 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		std::string open_filename = filename;
															#line 577 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 2914 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 578 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT((flags & OPEN_CREATE) && (flags & OPEN_READWRITE) && !(flags & OPEN_EXCLUSIVE));
															#line 579 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			open_filename = filename + ".part";
															#line 2920 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 582 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		int h = sf_open(open_filename.c_str(), flags, flagConversion(flags), mode);
															#line 583 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (h == -1)
															#line 2926 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 584 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			bool notFound = errno == ENOENT;
															#line 585 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			Error e = notFound ? file_not_found() : io_error();
															#line 586 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(notFound ? SevWarn : SevWarnAlways, "FileOpenError") .error(e) .GetLastError() .detail("File", filename) .detail("Flags", flags);
															#line 591 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1cont1Catch1(e, loopDepth);
															#line 2936 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 594 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		platform::makeTemporary(open_filename.c_str());
															#line 595 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		SimpleFile* simpleFile = new SimpleFile(h, diskParameters, delayOnWrite, filename, open_filename, flags);
															#line 596 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		file = Reference<IAsyncFile>(simpleFile);
															#line 597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_2 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2948 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<OpenActor*>(this)->actor_wait_state = 3;
															#line 597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 2, Void >*>(static_cast<OpenActor*>(this)));
															#line 2953 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<OpenActor*>(this)->actor_wait_state > 0) static_cast<OpenActor*>(this)->actor_wait_state = 0;
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OpenActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OpenActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 598 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(file); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 3025 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(file)); // state_var_RVO
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 598 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(file); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 3037 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(file)); // state_var_RVO
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<OpenActor*>(this)->actor_wait_state > 0) static_cast<OpenActor*>(this)->actor_wait_state = 0;
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OpenActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OpenActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 602 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 3112 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 602 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 3120 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<OpenActor*>(this)->actor_wait_state > 0) static_cast<OpenActor*>(this)->actor_wait_state = 0;
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< OpenActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< OpenActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	std::string filename;
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int flags;
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int mode;
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<DiskParameters> diskParameters;
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	bool delayOnWrite;
															#line 555 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 556 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	TaskPriority currentTaskID;
															#line 596 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<IAsyncFile> file;
															#line 600 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Error err;
															#line 3205 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via open()
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class OpenActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< OpenActor, 0, Void >, public ActorCallback< OpenActor, 1, Void >, public ActorCallback< OpenActor, 2, Void >, public ActorCallback< OpenActor, 3, Void >, public FastAllocated<OpenActor>, public OpenActorState<OpenActor> {
															#line 3210 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<OpenActor>::operator new;
	using FastAllocated<OpenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OpenActor, 0, Void >;
friend struct ActorCallback< OpenActor, 1, Void >;
friend struct ActorCallback< OpenActor, 2, Void >;
friend struct ActorCallback< OpenActor, 3, Void >;
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	OpenActor(std::string const& filename,int const& flags,int const& mode,Reference<DiskParameters> const& diskParameters = makeReference<DiskParameters>(25000, 150000000),bool const& delayOnWrite = true) 
															#line 3224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Reference<IAsyncFile>>(),
		   OpenActorState<OpenActor>(filename, flags, mode, diskParameters, delayOnWrite)
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("open");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OpenActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OpenActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OpenActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< OpenActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( std::string const& filename, int const& flags, int const& mode, Reference<DiskParameters> const& diskParameters = makeReference<DiskParameters>(25000, 150000000), bool const& delayOnWrite = true ) {
															#line 549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IAsyncFile>>(new OpenActor(filename, flags, mode, diskParameters, delayOnWrite));
															#line 3254 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 605 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

	void addref() override { ReferenceCounted<SimpleFile>::addref(); }
	void delref() override { ReferenceCounted<SimpleFile>::delref(); }

	int64_t debugFD() const override { return (int64_t)h; }

	Future<int> read(void* data, int length, int64_t offset) override { return read_impl(this, data, length, offset); }

	Future<Void> write(void const* data, int length, int64_t offset) override {
		return write_impl(this, StringRef((const uint8_t*)data, length), offset);
	}

	Future<Void> truncate(int64_t size) override { return truncate_impl(this, size); }

	Future<Void> sync() override { return sync_impl(this); }

	Future<int64_t> size() const override { return size_impl(this); }

	std::string getFilename() const override { return actualFilename; }

	~SimpleFile() override {
		_close(h);
		--openCount;
	}

private:
	int h;

	// Performance parameters of simulated disk
	Reference<DiskParameters> diskParameters;

	std::string filename, actualFilename;
	int flags;
	UID dbgId;

	// If true, then writes/truncates will be preceded by a delay (like other operations).  If false, then they will not
	// This is to support AsyncFileNonDurable, which issues its own delays for writes and truncates
	bool delayOnWrite;

	SimpleFile(int h,
	           Reference<DiskParameters> diskParameters,
	           bool delayOnWrite,
	           const std::string& filename,
	           const std::string& actualFilename,
	           int flags)
	  : h(h), diskParameters(diskParameters), filename(filename), actualFilename(actualFilename), flags(flags),
	    dbgId(deterministicRandom()->randomUniqueID()), delayOnWrite(delayOnWrite) {}

	static int flagConversion(int flags) {
		int outFlags = O_BINARY | O_CLOEXEC;
		if (flags & OPEN_READWRITE)
			outFlags |= O_RDWR;
		if (flags & OPEN_CREATE)
			outFlags |= O_CREAT;
		if (flags & OPEN_READONLY)
			outFlags |= O_RDONLY;
		if (flags & OPEN_EXCLUSIVE)
			outFlags |= O_EXCL;
		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
			outFlags |= O_TRUNC;

		return outFlags;
	}

																#line 3322 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via read_impl()
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class Read_implActor>
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Read_implActorState {
															#line 3328 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Read_implActorState(SimpleFile* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   data(data),
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   length(length),
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   offset(offset)
															#line 3341 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("read_impl", reinterpret_cast<unsigned long>(this));

	}
	~Read_implActorState() 
	{
		fdb_probe_actor_destroy("read_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 670 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT((self->flags & IAsyncFile::OPEN_NO_AIO) != 0 || ((uintptr_t)data % 4096 == 0 && length % 4096 == 0 && offset % 4096 == 0));
															#line 672 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			opId = deterministicRandom()->randomUniqueID();
															#line 673 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 3360 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 674 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				fmt::print(randLog, "SFR1 {0} {1} {2} {3} {4}\n", self->dbgId.shortString(), self->filename, opId.shortString(), length, offset);
															#line 3364 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 682 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, length);
															#line 682 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<Read_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3370 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Read_implActor*>(this)->actor_wait_state = 1;
															#line 682 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_implActor, 0, Void >*>(static_cast<Read_implActor*>(this)));
															#line 3375 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 684 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (_lseeki64(self->h, offset, SEEK_SET) == -1)
															#line 3398 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 685 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 1);
															#line 686 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3404 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 689 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		unsigned int read_bytes = 0;
															#line 690 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if ((read_bytes = _read(self->h, data, (unsigned int)length)) == -1)
															#line 3410 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 691 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 2);
															#line 692 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3416 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 695 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3420 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 696 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			uint32_t a = crc32c_append(0, (const uint8_t*)data, read_bytes);
															#line 697 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFR2 %s %s %s %d %d\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str(), read_bytes, a);
															#line 3426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 706 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		debugFileCheck("SimpleFileRead", self->filename, data, offset, length);
															#line 708 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::read");
															#line 709 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::read");
															#line 711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Read_implActor*>(this)->SAV<int>::futures) { (void)(read_bytes); this->~Read_implActorState(); static_cast<Read_implActor*>(this)->destroy(); return 0; }
															#line 3436 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Read_implActor*>(this)->SAV< int >::value()) int(read_bytes);
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 684 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (_lseeki64(self->h, offset, SEEK_SET) == -1)
															#line 3448 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 685 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 1);
															#line 686 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3454 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 689 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		unsigned int read_bytes = 0;
															#line 690 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if ((read_bytes = _read(self->h, data, (unsigned int)length)) == -1)
															#line 3460 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 691 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 2);
															#line 692 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3466 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 695 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3470 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 696 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			uint32_t a = crc32c_append(0, (const uint8_t*)data, read_bytes);
															#line 697 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFR2 %s %s %s %d %d\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str(), read_bytes, a);
															#line 3476 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 706 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		debugFileCheck("SimpleFileRead", self->filename, data, offset, length);
															#line 708 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::read");
															#line 709 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::read");
															#line 711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Read_implActor*>(this)->SAV<int>::futures) { (void)(read_bytes); this->~Read_implActorState(); static_cast<Read_implActor*>(this)->destroy(); return 0; }
															#line 3486 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Read_implActor*>(this)->SAV< int >::value()) int(read_bytes);
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Read_implActor*>(this)->actor_wait_state > 0) static_cast<Read_implActor*>(this)->actor_wait_state = 0;
		static_cast<Read_implActor*>(this)->ActorCallback< Read_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	void* data;
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int length;
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int64_t offset;
															#line 672 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 3567 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via read_impl()
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Read_implActor final : public Actor<int>, public ActorCallback< Read_implActor, 0, Void >, public FastAllocated<Read_implActor>, public Read_implActorState<Read_implActor> {
															#line 3572 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Read_implActor>::operator new;
	using FastAllocated<Read_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_implActor, 0, Void >;
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Read_implActor(SimpleFile* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 3583 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<int>(),
		   Read_implActorState<Read_implActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<int> read_impl( SimpleFile* const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 669 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<int>(new Read_implActor(self, data, length, offset));
															#line 3610 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 713 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

																#line 3615 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via write_impl()
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class Write_implActor>
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Write_implActorState {
															#line 3621 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Write_implActorState(SimpleFile* const& self,StringRef const& data,int64_t const& offset) 
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   data(data),
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   offset(offset),
															#line 715 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 3634 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("write_impl", reinterpret_cast<unsigned long>(this));

	}
	~Write_implActorState() 
	{
		fdb_probe_actor_destroy("write_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 716 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 3649 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 717 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				uint32_t a = crc32c_append(0, data.begin(), data.size());
															#line 718 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				fmt::print(randLog, "SFW1 {0} {1} {2} {3} {4} {5}\n", self->dbgId.shortString(), self->filename, opId.shortString(), a, data.size(), offset);
															#line 3655 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 728 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (self->delayOnWrite)
															#line 3659 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 729 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, data.size());
															#line 729 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3665 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Write_implActor*>(this)->actor_wait_state = 1;
															#line 729 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 0, Void >*>(static_cast<Write_implActor*>(this)));
															#line 3670 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont1(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 731 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (_lseeki64(self->h, offset, SEEK_SET) == -1)
															#line 3698 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 732 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 3);
															#line 733 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3704 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 736 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		unsigned int write_bytes = 0;
															#line 737 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if ((write_bytes = _write(self->h, (void*)data.begin(), data.size())) == -1)
															#line 3710 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 738 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 4);
															#line 739 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3716 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 742 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (write_bytes != data.size())
															#line 3720 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 743 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 5);
															#line 744 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3726 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 747 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3730 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 748 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFW2 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 3734 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 755 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		debugFileCheck("SimpleFileWrite", self->filename, (void*)data.begin(), offset, data.size());
															#line 757 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::write");
															#line 758 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::write");
															#line 760 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 3744 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Write_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Write_implActor*>(this)->actor_wait_state > 0) static_cast<Write_implActor*>(this)->actor_wait_state = 0;
		static_cast<Write_implActor*>(this)->ActorCallback< Write_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Write_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	StringRef data;
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int64_t offset;
															#line 715 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 3835 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via write_impl()
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Write_implActor final : public Actor<Void>, public ActorCallback< Write_implActor, 0, Void >, public FastAllocated<Write_implActor>, public Write_implActorState<Write_implActor> {
															#line 3840 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Write_implActor>::operator new;
	using FastAllocated<Write_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Write_implActor, 0, Void >;
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Write_implActor(SimpleFile* const& self,StringRef const& data,int64_t const& offset) 
															#line 3851 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   Write_implActorState<Write_implActor>(self, data, offset)
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("write_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Write_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> write_impl( SimpleFile* const& self, StringRef const& data, int64_t const& offset ) {
															#line 714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new Write_implActor(self, data, offset));
															#line 3878 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 762 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

																#line 3883 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via truncate_impl()
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class Truncate_implActor>
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Truncate_implActorState {
															#line 3889 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Truncate_implActorState(SimpleFile* const& self,int64_t const& size) 
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   size(size),
															#line 764 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 3900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	~Truncate_implActorState() 
	{
		fdb_probe_actor_destroy("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 765 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 3915 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 766 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				fmt::print( randLog, "SFT1 {0} {1} {2} {3}\n", self->dbgId.shortString(), self->filename, opId.shortString(), size);
															#line 3919 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 770 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if ((self->flags & IAsyncFile::OPEN_NO_AIO) == 0 && size == 0)
															#line 3923 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 771 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				return a_body1Catch1(io_error(), loopDepth);
															#line 3927 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 774 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (self->delayOnWrite)
															#line 3931 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 775 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0);
															#line 775 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3937 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Truncate_implActor*>(this)->actor_wait_state = 1;
															#line 775 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 0, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 3942 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont1(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 777 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (_chsize(self->h, (long)size) == -1)
															#line 3970 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 778 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError") .detail("Location", 6) .detail("Filename", self->filename) .detail("Size", size) .detail("Fd", self->h) .GetLastError();
															#line 784 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3976 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 787 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3980 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 788 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFT2 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 3984 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 794 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::truncate");
															#line 795 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::truncate");
															#line 797 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Truncate_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActorState(); static_cast<Truncate_implActor*>(this)->destroy(); return 0; }
															#line 3992 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Truncate_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state > 0) static_cast<Truncate_implActor*>(this)->actor_wait_state = 0;
		static_cast<Truncate_implActor*>(this)->ActorCallback< Truncate_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Truncate_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	int64_t size;
															#line 764 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 4081 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via truncate_impl()
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Truncate_implActor final : public Actor<Void>, public ActorCallback< Truncate_implActor, 0, Void >, public FastAllocated<Truncate_implActor>, public Truncate_implActorState<Truncate_implActor> {
															#line 4086 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Truncate_implActor>::operator new;
	using FastAllocated<Truncate_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Truncate_implActor, 0, Void >;
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Truncate_implActor(SimpleFile* const& self,int64_t const& size) 
															#line 4097 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   Truncate_implActorState<Truncate_implActor>(self, size)
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("truncate_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Truncate_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> truncate_impl( SimpleFile* const& self, int64_t const& size ) {
															#line 763 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new Truncate_implActor(self, size));
															#line 4124 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 799 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

	// Simulated sync does not actually do anything besides wait a random amount of time
																#line 4130 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via sync_impl()
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class Sync_implActor>
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Sync_implActorState {
															#line 4136 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sync_implActorState(SimpleFile* const& self) 
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 802 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 4145 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("sync_impl", reinterpret_cast<unsigned long>(this));

	}
	~Sync_implActorState() 
	{
		fdb_probe_actor_destroy("sync_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 803 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 4160 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 804 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				fprintf(randLog, "SFC1 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4164 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 810 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (self->delayOnWrite)
															#line 4168 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 811 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0, true);
															#line 811 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (static_cast<Sync_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4174 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Sync_implActor*>(this)->actor_wait_state = 1;
															#line 811 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Sync_implActor, 0, Void >*>(static_cast<Sync_implActor*>(this)));
															#line 4179 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont1(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~Sync_implActorState();
		static_cast<Sync_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 813 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (self->flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 4207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 814 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			self->flags &= ~OPEN_ATOMIC_WRITE_AND_CREATE;
															#line 815 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			auto& machineCache = g_simulator.getCurrentProcess()->machine->openFiles;
															#line 816 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			std::string sourceFilename = self->filename + ".part";
															#line 818 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (machineCache.count(sourceFilename))
															#line 4217 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 819 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TraceEvent("SimpleFileRename") .detail("From", sourceFilename) .detail("To", self->filename) .detail("SourceCount", machineCache.count(sourceFilename)) .detail("FileCount", machineCache.count(self->filename));
															#line 824 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				renameFile(sourceFilename.c_str(), self->filename.c_str());
															#line 826 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				machineCache[self->filename] = machineCache[sourceFilename];
															#line 827 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				machineCache.erase(sourceFilename);
															#line 828 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				self->actualFilename = self->filename;
															#line 4229 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
		}
															#line 832 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4234 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 833 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFC2 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4238 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 839 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::sync");
															#line 840 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::sync");
															#line 842 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Sync_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Sync_implActorState(); static_cast<Sync_implActor*>(this)->destroy(); return 0; }
															#line 4246 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Sync_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Sync_implActorState();
		static_cast<Sync_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Sync_implActor*>(this)->actor_wait_state > 0) static_cast<Sync_implActor*>(this)->actor_wait_state = 0;
		static_cast<Sync_implActor*>(this)->ActorCallback< Sync_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Sync_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 802 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 4333 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via sync_impl()
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Sync_implActor final : public Actor<Void>, public ActorCallback< Sync_implActor, 0, Void >, public FastAllocated<Sync_implActor>, public Sync_implActorState<Sync_implActor> {
															#line 4338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Sync_implActor>::operator new;
	using FastAllocated<Sync_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Sync_implActor, 0, Void >;
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sync_implActor(SimpleFile* const& self) 
															#line 4349 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   Sync_implActorState<Sync_implActor>(self)
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sync_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Sync_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> sync_impl( SimpleFile* const& self ) {
															#line 801 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new Sync_implActor(self));
															#line 4376 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 844 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

																#line 4381 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via size_impl()
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class Size_implActor>
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Size_implActorState {
															#line 4387 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Size_implActorState(SimpleFile const* const& self) 
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 846 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 4396 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("size_impl", reinterpret_cast<unsigned long>(this));

	}
	~Size_implActorState() 
	{
		fdb_probe_actor_destroy("size_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 847 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 4411 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 848 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				fprintf(randLog, "SFS1 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4415 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 854 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0);
															#line 854 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<Size_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4421 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Size_implActor*>(this)->actor_wait_state = 1;
															#line 854 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Size_implActor, 0, Void >*>(static_cast<Size_implActor*>(this)));
															#line 4426 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 856 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		int64_t pos = _lseeki64(self->h, 0L, SEEK_END);
															#line 857 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (pos == -1)
															#line 4451 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 858 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 8);
															#line 859 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 4457 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 862 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4461 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 863 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fmt::print( randLog, "SFS2 {0} {1} {2} {3}\n", self->dbgId.shortString(), self->filename, opId.shortString(), pos);
															#line 4465 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 865 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::size");
															#line 867 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Size_implActor*>(this)->SAV<int64_t>::futures) { (void)(pos); this->~Size_implActorState(); static_cast<Size_implActor*>(this)->destroy(); return 0; }
															#line 4471 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Size_implActor*>(this)->SAV< int64_t >::value()) int64_t(pos);
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 856 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		int64_t pos = _lseeki64(self->h, 0L, SEEK_END);
															#line 857 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (pos == -1)
															#line 4485 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 858 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 8);
															#line 859 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 4491 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 862 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4495 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 863 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			fmt::print( randLog, "SFS2 {0} {1} {2} {3}\n", self->dbgId.shortString(), self->filename, opId.shortString(), pos);
															#line 4499 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 865 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::size");
															#line 867 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Size_implActor*>(this)->SAV<int64_t>::futures) { (void)(pos); this->~Size_implActorState(); static_cast<Size_implActor*>(this)->destroy(); return 0; }
															#line 4505 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Size_implActor*>(this)->SAV< int64_t >::value()) int64_t(pos);
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Size_implActor*>(this)->actor_wait_state > 0) static_cast<Size_implActor*>(this)->actor_wait_state = 0;
		static_cast<Size_implActor*>(this)->ActorCallback< Size_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Size_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Size_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Size_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SimpleFile const* self;
															#line 846 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 4580 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via size_impl()
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class Size_implActor final : public Actor<int64_t>, public ActorCallback< Size_implActor, 0, Void >, public FastAllocated<Size_implActor>, public Size_implActorState<Size_implActor> {
															#line 4585 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Size_implActor>::operator new;
	using FastAllocated<Size_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Size_implActor, 0, Void >;
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Size_implActor(SimpleFile const* const& self) 
															#line 4596 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<int64_t>(),
		   Size_implActorState<Size_implActor>(self)
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("size_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Size_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<int64_t> size_impl( SimpleFile const* const& self ) {
															#line 845 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<int64_t>(new Size_implActor(self));
															#line 4623 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 869 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
};

struct SimDiskSpace {
	int64_t totalSpace;
	int64_t baseFreeSpace; // The original free space of the disk + deltas from simulated external modifications
	double lastUpdate;
};

void doReboot(ISimulator::ProcessInfo* const& p, ISimulator::KillType const& kt);

struct Sim2Listener final : IListener, ReferenceCounted<Sim2Listener> {
	explicit Sim2Listener(ISimulator::ProcessInfo* process, const NetworkAddress& listenAddr)
	  : process(process), address(listenAddr) {}

	void incomingConnection(double seconds, Reference<IConnection> conn) { // Called by another process!
		incoming(Reference<Sim2Listener>::addRef(this), seconds, conn);
	}

	void addref() override { ReferenceCounted<Sim2Listener>::addref(); }
	void delref() override { ReferenceCounted<Sim2Listener>::delref(); }

	Future<Reference<IConnection>> accept() override { return popOne(nextConnection.getFuture()); }

	NetworkAddress getListenAddress() const override { return address; }

private:
	ISimulator::ProcessInfo* process;
	PromiseStream<Reference<IConnection>> nextConnection;

																#line 4656 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via incoming()
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class IncomingActor>
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class IncomingActorState {
															#line 4662 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	IncomingActorState(Reference<Sim2Listener> const& self,double const& seconds,Reference<IConnection> const& conn) 
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   seconds(seconds),
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   conn(conn)
															#line 4673 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("incoming", reinterpret_cast<unsigned long>(this));

	}
	~IncomingActorState() 
	{
		fdb_probe_actor_destroy("incoming", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 899 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator.onProcess(self->process);
															#line 899 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 4690 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			static_cast<IncomingActor*>(this)->actor_wait_state = 1;
															#line 899 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncomingActor, 0, Void >*>(static_cast<IncomingActor*>(this)));
															#line 4694 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		delete static_cast<IncomingActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(seconds);
															#line 900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 4718 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		static_cast<IncomingActor*>(this)->actor_wait_state = 2;
															#line 900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncomingActor, 1, Void >*>(static_cast<IncomingActor*>(this)));
															#line 4722 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(seconds);
															#line 900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 4733 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		static_cast<IncomingActor*>(this)->actor_wait_state = 2;
															#line 900 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncomingActor, 1, Void >*>(static_cast<IncomingActor*>(this)));
															#line 4737 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IncomingActor*>(this)->actor_wait_state > 0) static_cast<IncomingActor*>(this)->actor_wait_state = 0;
		static_cast<IncomingActor*>(this)->ActorCallback< IncomingActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncomingActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncomingActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncomingActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 901 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (((Sim2Conn*)conn.getPtr())->isPeerGone() && deterministicRandom()->random01() < 0.5)
															#line 4809 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 902 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			delete static_cast<IncomingActor*>(this);
															#line 4813 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			return 0;
		}
															#line 903 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		TraceEvent("Sim2IncomingConn", conn->getDebugID()) .detail("ListenAddress", self->getListenAddress()) .detail("PeerAddress", conn->getPeerAddress());
															#line 906 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->nextConnection.send(conn);
															#line 4820 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 901 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (((Sim2Conn*)conn.getPtr())->isPeerGone() && deterministicRandom()->random01() < 0.5)
															#line 4829 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 902 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			delete static_cast<IncomingActor*>(this);
															#line 4833 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			return 0;
		}
															#line 903 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		TraceEvent("Sim2IncomingConn", conn->getDebugID()) .detail("ListenAddress", self->getListenAddress()) .detail("PeerAddress", conn->getPeerAddress());
															#line 906 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->nextConnection.send(conn);
															#line 4840 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IncomingActor*>(this)->actor_wait_state > 0) static_cast<IncomingActor*>(this)->actor_wait_state = 0;
		static_cast<IncomingActor*>(this)->ActorCallback< IncomingActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncomingActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncomingActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncomingActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		delete static_cast<IncomingActor*>(this);
															#line 4912 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<Sim2Listener> self;
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	double seconds;
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<IConnection> conn;
															#line 4923 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via incoming()
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class IncomingActor final : public Actor<void>, public ActorCallback< IncomingActor, 0, Void >, public ActorCallback< IncomingActor, 1, Void >, public FastAllocated<IncomingActor>, public IncomingActorState<IncomingActor> {
															#line 4928 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<IncomingActor>::operator new;
	using FastAllocated<IncomingActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< IncomingActor, 0, Void >;
friend struct ActorCallback< IncomingActor, 1, Void >;
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	IncomingActor(Reference<Sim2Listener> const& self,double const& seconds,Reference<IConnection> const& conn) 
															#line 4940 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<void>(),
		   IncomingActorState<IncomingActor>(self, seconds, conn)
	{
		fdb_probe_actor_enter("incoming", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("incoming");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("incoming", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
static void incoming( Reference<Sim2Listener> const& self, double const& seconds, Reference<IConnection> const& conn ) {
															#line 898 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	new IncomingActor(self, seconds, conn);
															#line 4958 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
																#line 4962 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via popOne()
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class PopOneActor>
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class PopOneActorState {
															#line 4968 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	PopOneActorState(FutureStream<Reference<IConnection>> const& conns) 
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : conns(conns)
															#line 4975 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("popOne", reinterpret_cast<unsigned long>(this));

	}
	~PopOneActorState() 
	{
		fdb_probe_actor_destroy("popOne", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 909 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			FutureStream<Reference<IConnection>> __when_expr_0 = conns;
															#line 909 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<PopOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4992 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<PopOneActor*>(this)->actor_wait_state = 1;
															#line 909 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >*>(static_cast<PopOneActor*>(this)));
															#line 4997 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~PopOneActorState();
		static_cast<PopOneActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<IConnection> const& c,int loopDepth) 
	{
															#line 910 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		((Sim2Conn*)c.getPtr())->opened = true;
															#line 911 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<PopOneActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~PopOneActorState(); static_cast<PopOneActor*>(this)->destroy(); return 0; }
															#line 5022 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<PopOneActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(c);
		this->~PopOneActorState();
		static_cast<PopOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<IConnection> && c,int loopDepth) 
	{
															#line 910 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		((Sim2Conn*)c.getPtr())->opened = true;
															#line 911 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<PopOneActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~PopOneActorState(); static_cast<PopOneActor*>(this)->destroy(); return 0; }
															#line 5036 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<PopOneActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(c);
		this->~PopOneActorState();
		static_cast<PopOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IConnection> const& c,int loopDepth) 
	{
		loopDepth = a_body1cont1(c, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IConnection> && c,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(c), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PopOneActor*>(this)->actor_wait_state > 0) static_cast<PopOneActor*>(this)->actor_wait_state = 0;
		static_cast<PopOneActor*>(this)->ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("popOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("popOne", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("popOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("popOne", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("popOne", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("popOne", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	FutureStream<Reference<IConnection>> conns;
															#line 5109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via popOne()
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class PopOneActor final : public Actor<Reference<IConnection>>, public ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >, public FastAllocated<PopOneActor>, public PopOneActorState<PopOneActor> {
															#line 5114 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<PopOneActor>::operator new;
	using FastAllocated<PopOneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >;
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	PopOneActor(FutureStream<Reference<IConnection>> const& conns) 
															#line 5125 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   PopOneActorState<PopOneActor>(conns)
	{
		fdb_probe_actor_enter("popOne", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("popOne");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("popOne", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> popOne( FutureStream<Reference<IConnection>> const& conns ) {
															#line 908 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IConnection>>(new PopOneActor(conns));
															#line 5152 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 913 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

	NetworkAddress address;
};

#define g_sim2 ((Sim2&)g_simulator)

class Sim2 final : public ISimulator, public INetworkConnections {
public:
	// Implement INetwork interface
	// Everything actually network related is delegated to the Sim2Net class; Sim2 is only concerned with simulating
	// machines and time
	double now() const override { return time; }

	// timer() can be up to 0.1 seconds ahead of now()
	double timer() override {
		timerTime += deterministicRandom()->random01() * (time + 0.1 - timerTime) / 2.0;
		return timerTime;
	}

	double timer_monotonic() override { return timer(); }

	Future<class Void> delay(double seconds, TaskPriority taskID) override {
		ASSERT(taskID >= TaskPriority::Min && taskID <= TaskPriority::Max);
		return delay(seconds, taskID, currentProcess);
	}
	Future<class Void> orderedDelay(double seconds, TaskPriority taskID) override {
		ASSERT(taskID >= TaskPriority::Min && taskID <= TaskPriority::Max);
		return delay(seconds, taskID, currentProcess, true);
	}
	Future<class Void> delay(double seconds, TaskPriority taskID, ProcessInfo* machine, bool ordered = false) {
		ASSERT(seconds >= -0.0001);
		seconds = std::max(0.0, seconds);
		Future<Void> f;

		if (!ordered && !currentProcess->rebooting && machine == currentProcess &&
		    !currentProcess->shutdownSignal.isSet() && FLOW_KNOBS->MAX_BUGGIFIED_DELAY > 0 &&
		    deterministicRandom()->random01() < 0.25) { // FIXME: why doesn't this work when we are changing machines?
			seconds += FLOW_KNOBS->MAX_BUGGIFIED_DELAY * pow(deterministicRandom()->random01(), 1000.0);
		}

		mutex.enter();
		tasks.push(Task(time + seconds, taskID, taskCount++, machine, f));
		mutex.leave();

		return f;
	}
																#line 5202 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via checkShutdown()
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class CheckShutdownActor>
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class CheckShutdownActorState {
															#line 5208 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	CheckShutdownActorState(Sim2* const& self,TaskPriority const& taskID) 
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   taskID(taskID)
															#line 5217 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("checkShutdown", reinterpret_cast<unsigned long>(this));

	}
	~CheckShutdownActorState() 
	{
		fdb_probe_actor_destroy("checkShutdown", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 960 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(self->getCurrentProcess()->shutdownSignal.getFuture());
															#line 960 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<CheckShutdownActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5234 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckShutdownActor*>(this)->actor_wait_state = 1;
															#line 960 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckShutdownActor, 0, Void >*>(static_cast<CheckShutdownActor*>(this)));
															#line 5239 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~CheckShutdownActorState();
		static_cast<CheckShutdownActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 961 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->setCurrentTask(taskID);
															#line 962 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CheckShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckShutdownActorState(); static_cast<CheckShutdownActor*>(this)->destroy(); return 0; }
															#line 5264 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<CheckShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckShutdownActorState();
		static_cast<CheckShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 961 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->setCurrentTask(taskID);
															#line 962 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CheckShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckShutdownActorState(); static_cast<CheckShutdownActor*>(this)->destroy(); return 0; }
															#line 5278 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<CheckShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckShutdownActorState();
		static_cast<CheckShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckShutdownActor*>(this)->actor_wait_state > 0) static_cast<CheckShutdownActor*>(this)->actor_wait_state = 0;
		static_cast<CheckShutdownActor*>(this)->ActorCallback< CheckShutdownActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckShutdownActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckShutdownActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckShutdownActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2* self;
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	TaskPriority taskID;
															#line 5353 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via checkShutdown()
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class CheckShutdownActor final : public Actor<Void>, public ActorCallback< CheckShutdownActor, 0, Void >, public FastAllocated<CheckShutdownActor>, public CheckShutdownActorState<CheckShutdownActor> {
															#line 5358 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<CheckShutdownActor>::operator new;
	using FastAllocated<CheckShutdownActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckShutdownActor, 0, Void >;
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	CheckShutdownActor(Sim2* const& self,TaskPriority const& taskID) 
															#line 5369 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   CheckShutdownActorState<CheckShutdownActor>(self, taskID)
	{
		fdb_probe_actor_enter("checkShutdown", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkShutdown");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkShutdown", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckShutdownActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> checkShutdown( Sim2* const& self, TaskPriority const& taskID ) {
															#line 959 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new CheckShutdownActor(self, taskID));
															#line 5396 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 964 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Future<class Void> yield(TaskPriority taskID) override {
		if (taskID == TaskPriority::DefaultYield)
			taskID = currentTaskID;
		if (check_yield(taskID)) {
			// We want to check that yielders can handle actual time elapsing (it sometimes will outside simulation),
			// but don't want to prevent instantaneous shutdown of "rebooted" machines.
			return delay(getCurrentProcess()->rebooting ? 0 : .001, taskID) || checkShutdown(this, taskID);
		}
		setCurrentTask(taskID);
		return Void();
	}
	bool check_yield(TaskPriority taskID) override {
		if (yielded)
			return true;
		if (--yield_limit <= 0) {
			yield_limit = deterministicRandom()->randomInt(
			    1, 150); // If yield returns false *too* many times in a row, there could be a stack overflow, since we
			             // can't deterministically check stack size as the real network does
			return yielded = true;
		}
		return yielded = BUGGIFY_WITH_PROB(0.01);
	}
	TaskPriority getCurrentTask() const override { return currentTaskID; }
	void setCurrentTask(TaskPriority taskID) override { currentTaskID = taskID; }
	// Sets the taskID/priority of the current task, without yielding
	Future<Reference<IConnection>> connect(NetworkAddress toAddr,
	                                       boost::asio::ip::tcp::socket* existingSocket = nullptr) override {
		ASSERT(existingSocket == nullptr);
		if (!addressMap.count(toAddr)) {
			return waitForProcessAndConnect(toAddr, this);
		}
		auto peerp = getProcessByAddress(toAddr);
		auto myc = makeReference<Sim2Conn>(getCurrentProcess());
		auto peerc = makeReference<Sim2Conn>(peerp);

		myc->connect(peerc, toAddr);
		IPAddress localIp;
		if (getCurrentProcess()->address.ip.isV6()) {
			IPAddress::IPAddressStore store = getCurrentProcess()->address.ip.toV6();
			uint16_t* ipParts = (uint16_t*)store.data();
			ipParts[7] += deterministicRandom()->randomInt(0, 256);
			localIp = IPAddress(store);
		} else {
			localIp = IPAddress(getCurrentProcess()->address.ip.toV4() + deterministicRandom()->randomInt(0, 256));
		}
		peerc->connect(myc,
		               NetworkAddress(localIp, deterministicRandom()->randomInt(40000, 60000), false, toAddr.isTLS()));

		((Sim2Listener*)peerp->getListener(toAddr).getPtr())
		    ->incomingConnection(0.5 * deterministicRandom()->random01(), Reference<IConnection>(peerc));
		return onConnect(::delay(0.5 * deterministicRandom()->random01()), myc);
	}

	Future<Reference<IConnection>> connectExternal(NetworkAddress toAddr) override {
		return SimExternalConnection::connect(toAddr);
	}

	Future<Reference<IUDPSocket>> createUDPSocket(NetworkAddress toAddr) override;
	Future<Reference<IUDPSocket>> createUDPSocket(bool isV6 = false) override;

	// Add a <hostname, vector<NetworkAddress>> pair to mock DNS in simulation.
	void addMockTCPEndpoint(const std::string& host,
	                        const std::string& service,
	                        const std::vector<NetworkAddress>& addresses) override {
		mockDNS.add(host, service, addresses);
	}
	void removeMockTCPEndpoint(const std::string& host, const std::string& service) override {
		mockDNS.remove(host, service);
	}
	// Convert hostnameToAddresses from/to string. The format is:
	// hostname1,host1Address1,host1Address2;hostname2,host2Address1,host2Address2...
	void parseMockDNSFromString(const std::string& s) override { mockDNS = DNSCache::parseFromString(s); }
	std::string convertMockDNSToString() override { return mockDNS.toString(); }
	Future<std::vector<NetworkAddress>> resolveTCPEndpoint(const std::string& host,
	                                                       const std::string& service) override {
		// If a <hostname, vector<NetworkAddress>> pair was injected to mock DNS, use it.
		Optional<std::vector<NetworkAddress>> mock = mockDNS.find(host, service);
		if (mock.present()) {
			return mock.get();
		}
		return SimExternalConnection::resolveTCPEndpoint(host, service, &dnsCache);
	}
	Future<std::vector<NetworkAddress>> resolveTCPEndpointWithDNSCache(const std::string& host,
	                                                                   const std::string& service) override {
		// If a <hostname, vector<NetworkAddress>> pair was injected to mock DNS, use it.
		Optional<std::vector<NetworkAddress>> mock = mockDNS.find(host, service);
		if (mock.present()) {
			return mock.get();
		}
		if (FLOW_KNOBS->ENABLE_COORDINATOR_DNS_CACHE) {
			Optional<std::vector<NetworkAddress>> cache = dnsCache.find(host, service);
			if (cache.present()) {
				return cache.get();
			}
		}
		return SimExternalConnection::resolveTCPEndpoint(host, service, &dnsCache);
	}
	std::vector<NetworkAddress> resolveTCPEndpointBlocking(const std::string& host,
	                                                       const std::string& service) override {
		// If a <hostname, vector<NetworkAddress>> pair was injected to mock DNS, use it.
		Optional<std::vector<NetworkAddress>> mock = mockDNS.find(host, service);
		if (mock.present()) {
			return mock.get();
		}
		return SimExternalConnection::resolveTCPEndpointBlocking(host, service, &dnsCache);
	}
	std::vector<NetworkAddress> resolveTCPEndpointBlockingWithDNSCache(const std::string& host,
	                                                                   const std::string& service) override {
		// If a <hostname, vector<NetworkAddress>> pair was injected to mock DNS, use it.
		Optional<std::vector<NetworkAddress>> mock = mockDNS.find(host, service);
		if (mock.present()) {
			return mock.get();
		}
		if (FLOW_KNOBS->ENABLE_COORDINATOR_DNS_CACHE) {
			Optional<std::vector<NetworkAddress>> cache = dnsCache.find(host, service);
			if (cache.present()) {
				return cache.get();
			}
		}
		return SimExternalConnection::resolveTCPEndpointBlocking(host, service, &dnsCache);
	}
																#line 5521 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via onConnect()
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class OnConnectActor>
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class OnConnectActorState {
															#line 5527 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	OnConnectActorState(Future<Void> const& ready,Reference<Sim2Conn> const& conn) 
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : ready(ready),
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   conn(conn)
															#line 5536 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("onConnect", reinterpret_cast<unsigned long>(this));

	}
	~OnConnectActorState() 
	{
		fdb_probe_actor_destroy("onConnect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1086 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ready;
															#line 1086 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OnConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5553 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnConnectActor*>(this)->actor_wait_state = 1;
															#line 1086 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnConnectActor, 0, Void >*>(static_cast<OnConnectActor*>(this)));
															#line 5558 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~OnConnectActorState();
		static_cast<OnConnectActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1087 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (conn->isPeerGone())
															#line 5581 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1088 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			conn.clear();
															#line 1089 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 1 || (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 2 && deterministicRandom()->random01() > 0.5))
															#line 5587 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1091 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				return a_body1Catch1(connection_failed(), loopDepth);
															#line 5591 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 1093 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = Never();
															#line 1093 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OnConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5597 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnConnectActor*>(this)->actor_wait_state = 2;
															#line 1093 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnConnectActor, 1, Void >*>(static_cast<OnConnectActor*>(this)));
															#line 5602 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1087 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (conn->isPeerGone())
															#line 5616 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1088 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			conn.clear();
															#line 1089 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 1 || (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 2 && deterministicRandom()->random01() > 0.5))
															#line 5622 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1091 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				return a_body1Catch1(connection_failed(), loopDepth);
															#line 5626 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 1093 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = Never();
															#line 1093 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<OnConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5632 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnConnectActor*>(this)->actor_wait_state = 2;
															#line 1093 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnConnectActor, 1, Void >*>(static_cast<OnConnectActor*>(this)));
															#line 5637 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OnConnectActor*>(this)->actor_wait_state > 0) static_cast<OnConnectActor*>(this)->actor_wait_state = 0;
		static_cast<OnConnectActor*>(this)->ActorCallback< OnConnectActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnConnectActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnConnectActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnConnectActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 1095 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		conn->opened = true;
															#line 1096 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<OnConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~OnConnectActorState(); static_cast<OnConnectActor*>(this)->destroy(); return 0; }
															#line 5716 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<OnConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~OnConnectActorState();
		static_cast<OnConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<OnConnectActor*>(this)->actor_wait_state > 0) static_cast<OnConnectActor*>(this)->actor_wait_state = 0;
		static_cast<OnConnectActor*>(this)->ActorCallback< OnConnectActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnConnectActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnConnectActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnConnectActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Future<Void> ready;
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<Sim2Conn> conn;
															#line 5803 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via onConnect()
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class OnConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< OnConnectActor, 0, Void >, public ActorCallback< OnConnectActor, 1, Void >, public FastAllocated<OnConnectActor>, public OnConnectActorState<OnConnectActor> {
															#line 5808 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<OnConnectActor>::operator new;
	using FastAllocated<OnConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnConnectActor, 0, Void >;
friend struct ActorCallback< OnConnectActor, 1, Void >;
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	OnConnectActor(Future<Void> const& ready,Reference<Sim2Conn> const& conn) 
															#line 5820 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   OnConnectActorState<OnConnectActor>(ready, conn)
	{
		fdb_probe_actor_enter("onConnect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onConnect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onConnect", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnConnectActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnConnectActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> onConnect( Future<Void> const& ready, Reference<Sim2Conn> const& conn ) {
															#line 1085 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IConnection>>(new OnConnectActor(ready, conn));
															#line 5848 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 1098 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<IListener> listen(NetworkAddress localAddr) override {
		Reference<IListener> listener(getCurrentProcess()->getListener(localAddr));
		ASSERT(listener);
		return listener;
	}
																#line 5857 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via waitForProcessAndConnect()
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class WaitForProcessAndConnectActor>
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class WaitForProcessAndConnectActorState {
															#line 5863 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	WaitForProcessAndConnectActorState(NetworkAddress const& toAddr,INetworkConnections* const& self) 
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : toAddr(toAddr),
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   self(self)
															#line 5872 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this));

	}
	~WaitForProcessAndConnectActorState() 
	{
		fdb_probe_actor_destroy("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1106 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			;
															#line 5887 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~WaitForProcessAndConnectActorState();
		static_cast<WaitForProcessAndConnectActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 1107 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = ::delay(0.1 * deterministicRandom()->random01());
															#line 1107 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5919 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 1;
															#line 1107 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForProcessAndConnectActor, 0, Void >*>(static_cast<WaitForProcessAndConnectActor*>(this)));
															#line 5924 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1108 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (g_sim2.addressMap.count(toAddr))
															#line 5933 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_1 = self->connect(toAddr);
															#line 1109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5939 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 2;
															#line 1109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*>(static_cast<WaitForProcessAndConnectActor*>(this)));
															#line 5944 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1108 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (g_sim2.addressMap.count(toAddr))
															#line 5958 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_1 = self->connect(toAddr);
															#line 1109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5964 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 2;
															#line 1109 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*>(static_cast<WaitForProcessAndConnectActor*>(this)));
															#line 5969 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state > 0) static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForProcessAndConnectActor*>(this)->ActorCallback< WaitForProcessAndConnectActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForProcessAndConnectActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForProcessAndConnectActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForProcessAndConnectActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Reference<IConnection> const& c,int loopDepth) 
	{
															#line 1110 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<WaitForProcessAndConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~WaitForProcessAndConnectActorState(); static_cast<WaitForProcessAndConnectActor*>(this)->destroy(); return 0; }
															#line 6052 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<WaitForProcessAndConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(c);
		this->~WaitForProcessAndConnectActorState();
		static_cast<WaitForProcessAndConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Reference<IConnection> && c,int loopDepth) 
	{
															#line 1110 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<WaitForProcessAndConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~WaitForProcessAndConnectActorState(); static_cast<WaitForProcessAndConnectActor*>(this)->destroy(); return 0; }
															#line 6064 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<WaitForProcessAndConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(c);
		this->~WaitForProcessAndConnectActorState();
		static_cast<WaitForProcessAndConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Reference<IConnection> const& c,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(c, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Reference<IConnection> && c,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(c), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state > 0) static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForProcessAndConnectActor*>(this)->ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	NetworkAddress toAddr;
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	INetworkConnections* self;
															#line 6139 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via waitForProcessAndConnect()
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class WaitForProcessAndConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< WaitForProcessAndConnectActor, 0, Void >, public ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >, public FastAllocated<WaitForProcessAndConnectActor>, public WaitForProcessAndConnectActorState<WaitForProcessAndConnectActor> {
															#line 6144 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<WaitForProcessAndConnectActor>::operator new;
	using FastAllocated<WaitForProcessAndConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForProcessAndConnectActor, 0, Void >;
friend struct ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >;
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	WaitForProcessAndConnectActor(NetworkAddress const& toAddr,INetworkConnections* const& self) 
															#line 6156 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   WaitForProcessAndConnectActorState<WaitForProcessAndConnectActor>(toAddr, self)
	{
		fdb_probe_actor_enter("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForProcessAndConnect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForProcessAndConnect", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForProcessAndConnectActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> waitForProcessAndConnect( NetworkAddress const& toAddr, INetworkConnections* const& self ) {
															#line 1103 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IConnection>>(new WaitForProcessAndConnectActor(toAddr, self));
															#line 6184 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 1114 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	const TLSConfig& getTLSConfig() const override {
		static TLSConfig emptyConfig;
		return emptyConfig;
	}

	bool checkRunnable() override { return net2->checkRunnable(); }

#ifdef ENABLE_SAMPLING
	ActorLineageSet& getActorLineageSet() override { return actorLineageSet; }
#endif

	void stop() override { isStopped = true; }
	void addStopCallback(std::function<void()> fn) override { stopCallbacks.emplace_back(std::move(fn)); }
	bool isSimulated() const override { return true; }

	struct SimThreadArgs {
		THREAD_FUNC_RETURN (*func)(void*);
		void* arg;

		ISimulator::ProcessInfo* currentProcess;

		SimThreadArgs(THREAD_FUNC_RETURN (*func)(void*), void* arg) : func(func), arg(arg) {
			ASSERT(g_network->isSimulated());
			currentProcess = g_simulator.getCurrentProcess();
		}
	};

	// Starts a new thread, making sure to set any thread local state
	THREAD_FUNC simStartThread(void* arg) {
		SimThreadArgs* simArgs = (SimThreadArgs*)arg;
		ISimulator::currentProcess = simArgs->currentProcess;
		simArgs->func(simArgs->arg);

		delete simArgs;
		THREAD_RETURN;
	}

	THREAD_HANDLE startThread(THREAD_FUNC_RETURN (*func)(void*), void* arg, int stackSize, const char* name) override {
		SimThreadArgs* simArgs = new SimThreadArgs(func, arg);
		return ::startThread(simStartThread, simArgs, stackSize, name);
	}

	void getDiskBytes(std::string const& directory, int64_t& free, int64_t& total) override {
		ProcessInfo* proc = getCurrentProcess();
		SimDiskSpace& diskSpace = diskSpaceMap[proc->address.ip];

		int64_t totalFileSize = 0;
		int numFiles = 0;

		// Get the size of all files we've created on the server and subtract them from the free space
		for (auto file = proc->machine->openFiles.begin(); file != proc->machine->openFiles.end(); ++file) {
			if (file->second.get().isReady()) {
				totalFileSize += ((AsyncFileNonDurable*)file->second.get().get().getPtr())->approximateSize;
			}
			numFiles++;
		}

		if (diskSpace.totalSpace == 0) {
			diskSpace.totalSpace = 5e9 + deterministicRandom()->random01() * 100e9; // Total space between 5GB and 105GB
			diskSpace.baseFreeSpace = std::min<int64_t>(
			    diskSpace.totalSpace,
			    std::max(5e9, (deterministicRandom()->random01() * (1 - .075) + .075) * diskSpace.totalSpace) +
			        totalFileSize); // Minimum 5GB or 7.5% total disk space, whichever is higher

			TraceEvent("Sim2DiskSpaceInitialization")
			    .detail("TotalSpace", diskSpace.totalSpace)
			    .detail("BaseFreeSpace", diskSpace.baseFreeSpace)
			    .detail("TotalFileSize", totalFileSize)
			    .detail("NumFiles", numFiles);
		} else {
			int64_t maxDelta = std::min(5.0, (now() - diskSpace.lastUpdate)) *
			                   (BUGGIFY ? 10e6 : 1e6); // External processes modifying the disk
			int64_t delta = -maxDelta + deterministicRandom()->random01() * maxDelta * 2;
			diskSpace.baseFreeSpace = std::min<int64_t>(
			    diskSpace.totalSpace, std::max<int64_t>(diskSpace.baseFreeSpace + delta, totalFileSize));
		}

		diskSpace.lastUpdate = now();

		total = diskSpace.totalSpace;
		free = std::max<int64_t>(0, diskSpace.baseFreeSpace - totalFileSize);

		if (free == 0)
			TraceEvent(SevWarnAlways, "Sim2NoFreeSpace")
			    .detail("TotalSpace", diskSpace.totalSpace)
			    .detail("BaseFreeSpace", diskSpace.baseFreeSpace)
			    .detail("TotalFileSize", totalFileSize)
			    .detail("NumFiles", numFiles);
	}
	bool isAddressOnThisHost(NetworkAddress const& addr) const override {
		return addr.ip == getCurrentProcess()->address.ip;
	}

																#line 6281 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via deleteFileImpl()
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class DeleteFileImplActor>
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class DeleteFileImplActorState {
															#line 6287 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	DeleteFileImplActorState(Sim2* const& self,std::string const& filename,bool const& mustBeDurable) 
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   filename(filename),
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   mustBeDurable(mustBeDurable)
															#line 6298 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("deleteFileImpl", reinterpret_cast<unsigned long>(this));

	}
	~DeleteFileImplActorState() 
	{
		fdb_probe_actor_destroy("deleteFileImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1211 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (g_simulator.getCurrentProcess()->machine->openFiles.count(filename))
															#line 6313 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1212 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				g_simulator.getCurrentProcess()->machine->openFiles.erase(filename);
															#line 1213 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				g_simulator.getCurrentProcess()->machine->deletingOrClosingFiles.insert(filename);
															#line 6319 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 1215 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (mustBeDurable || deterministicRandom()->random01() < 0.5)
															#line 6323 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1216 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				currentProcess = g_simulator.getCurrentProcess();
															#line 1217 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				currentTaskID = g_network->getCurrentTask();
															#line 1218 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "Sim2DeleteFileImpl") .detail("CurrentProcess", currentProcess->toString()) .detail("Filename", filename) .detail("Durable", mustBeDurable);
															#line 1222 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 1222 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6335 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 1;
															#line 1222 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 0, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6340 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 1239 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "Sim2DeleteFileImplNonDurable") .detail("Filename", filename) .detail("Durable", mustBeDurable);
															#line 1242 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TEST(true);
															#line 1243 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (!static_cast<DeleteFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteFileImplActorState(); static_cast<DeleteFileImplActor*>(this)->destroy(); return 0; }
															#line 6351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				new (&static_cast<DeleteFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~DeleteFileImplActorState();
				static_cast<DeleteFileImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~DeleteFileImplActorState();
		static_cast<DeleteFileImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		try {
															#line 1224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6381 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont3Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 2;
															#line 1224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 1, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6386 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont3Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont3Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		try {
															#line 1224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6404 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont3Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 2;
															#line 1224 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 1, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6409 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont3Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont3Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state > 0) static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteFileImplActor*>(this)->ActorCallback< DeleteFileImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteFileImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1234 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			err = e;
															#line 1235 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_4 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 1235 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6492 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont3Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 5;
															#line 1235 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 4, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6497 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 1225 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!currentProcess->rebooting)
															#line 6512 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1226 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			auto f = IAsyncFileSystem::filesystem(self->net2)->deleteFile(filename, false);
															#line 1227 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(f.isReady());
															#line 1228 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1228 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6522 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont3Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 3;
															#line 1228 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 2, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6527 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 1225 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!currentProcess->rebooting)
															#line 6541 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1226 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			auto f = IAsyncFileSystem::filesystem(self->net2)->deleteFile(filename, false);
															#line 1227 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(f.isReady());
															#line 1228 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1228 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6551 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont3Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 3;
															#line 1228 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 2, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6556 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state > 0) static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteFileImplActor*>(this)->ActorCallback< DeleteFileImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteFileImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont3Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 1231 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 1231 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6635 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont3Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 4;
															#line 1231 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 3, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6640 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 1229 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		TEST(true);
															#line 6649 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 1229 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		TEST(true);
															#line 6658 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state > 0) static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteFileImplActor*>(this)->ActorCallback< DeleteFileImplActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DeleteFileImplActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 1232 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<DeleteFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteFileImplActorState(); static_cast<DeleteFileImplActor*>(this)->destroy(); return 0; }
															#line 6730 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<DeleteFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteFileImplActorState();
		static_cast<DeleteFileImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 1232 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<DeleteFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteFileImplActorState(); static_cast<DeleteFileImplActor*>(this)->destroy(); return 0; }
															#line 6742 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<DeleteFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteFileImplActorState();
		static_cast<DeleteFileImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state > 0) static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteFileImplActor*>(this)->ActorCallback< DeleteFileImplActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DeleteFileImplActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont3Catch1(error, 0);
		} catch (...) {
			a_body1cont3Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont3Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 1236 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 6817 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont3Catch1cont1(Void && _,int loopDepth) 
	{
															#line 1236 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 6825 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont3Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state > 0) static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteFileImplActor*>(this)->ActorCallback< DeleteFileImplActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont3Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< DeleteFileImplActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont3Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< DeleteFileImplActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Sim2* self;
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	std::string filename;
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	bool mustBeDurable;
															#line 1216 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 1217 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	TaskPriority currentTaskID;
															#line 1234 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Error err;
															#line 6904 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via deleteFileImpl()
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class DeleteFileImplActor final : public Actor<Void>, public ActorCallback< DeleteFileImplActor, 0, Void >, public ActorCallback< DeleteFileImplActor, 1, Void >, public ActorCallback< DeleteFileImplActor, 2, Void >, public ActorCallback< DeleteFileImplActor, 3, Void >, public ActorCallback< DeleteFileImplActor, 4, Void >, public FastAllocated<DeleteFileImplActor>, public DeleteFileImplActorState<DeleteFileImplActor> {
															#line 6909 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<DeleteFileImplActor>::operator new;
	using FastAllocated<DeleteFileImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteFileImplActor, 0, Void >;
friend struct ActorCallback< DeleteFileImplActor, 1, Void >;
friend struct ActorCallback< DeleteFileImplActor, 2, Void >;
friend struct ActorCallback< DeleteFileImplActor, 3, Void >;
friend struct ActorCallback< DeleteFileImplActor, 4, Void >;
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	DeleteFileImplActor(Sim2* const& self,std::string const& filename,bool const& mustBeDurable) 
															#line 6924 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   DeleteFileImplActorState<DeleteFileImplActor>(self, filename, mustBeDurable)
	{
		fdb_probe_actor_enter("deleteFileImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteFileImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteFileImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteFileImplActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteFileImplActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DeleteFileImplActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DeleteFileImplActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< DeleteFileImplActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> deleteFileImpl( Sim2* const& self, std::string const& filename, bool const& mustBeDurable ) {
															#line 1207 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new DeleteFileImplActor(self, filename, mustBeDurable));
															#line 6955 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 1246 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

	static void runLoop(Sim2* self) {
		ISimulator::ProcessInfo* callingMachine = self->currentProcess;
		while (!self->isStopped) {
			self->mutex.enter();
			if (self->tasks.size() == 0) {
				self->mutex.leave();
				ASSERT(false);
			}
			// if (!randLog/* && now() >= 32.0*/)
			//	randLog = fopen("randLog.txt", "wt");
			Task t = std::move(self->tasks.top()); // Unfortunately still a copy under gcc where .top() returns const&
			self->currentTaskID = t.taskID;
			self->tasks.pop();
			self->mutex.leave();

			self->execTask(t);
			self->yielded = false;
		}
		self->currentProcess = callingMachine;
		for (auto& fn : self->stopCallbacks) {
			fn();
		}
	}

	// Implement ISimulator interface
	void run() override { runLoop(this); }
	ProcessInfo* newProcess(const char* name,
	                        IPAddress ip,
	                        uint16_t port,
	                        bool sslEnabled,
	                        uint16_t listenPerProcess,
	                        LocalityData locality,
	                        ProcessClass startingClass,
	                        const char* dataFolder,
	                        const char* coordinationFolder,
	                        ProtocolVersion protocol) override {
		ASSERT(locality.machineId().present());
		MachineInfo& machine = machines[locality.machineId().get()];
		if (!machine.machineId.present())
			machine.machineId = locality.machineId();
		if (port == 0 && std::string(name) == "remote flow process") {
			port = machine.getRandomPort();
		}
		for (int i = 0; i < machine.processes.size(); i++) {
			if (machine.processes[i]->locality.machineId() !=
			    locality.machineId()) { // SOMEDAY: compute ip from locality to avoid this check
				TraceEvent("Sim2Mismatch")
				    .detail("IP", format("%s", ip.toString().c_str()))
				    .detail("MachineId", locality.machineId())
				    .detail("NewName", name)
				    .detail("ExistingMachineId", machine.processes[i]->locality.machineId())
				    .detail("ExistingName", machine.processes[i]->name);
				ASSERT(false);
			}
			ASSERT(machine.processes[i]->address.port != port);
		}

		// This is for async operations on non-durable files.
		// These files must live on after process kills for sim purposes.
		if (machine.machineProcess == 0) {
			NetworkAddress machineAddress(ip, 0, false, false);
			machine.machineProcess =
			    new ProcessInfo("Machine", locality, startingClass, { machineAddress }, this, "", "");
			machine.machineProcess->machine = &machine;
		}

		NetworkAddressList addresses;
		addresses.address = NetworkAddress(ip, port, true, sslEnabled);
		if (listenPerProcess == 2) { // listenPerProcess is only 1 or 2
			addresses.secondaryAddress = NetworkAddress(ip, port + 1, true, false);
		}

		ProcessInfo* m =
		    new ProcessInfo(name, locality, startingClass, addresses, this, dataFolder, coordinationFolder);
		for (int processPort = port; processPort < port + listenPerProcess; ++processPort) {
			NetworkAddress address(ip, processPort, true, sslEnabled && processPort == port);
			m->listenerMap[address] = Reference<IListener>(new Sim2Listener(m, address));
			addressMap[address] = m;
		}
		m->machine = &machine;
		machine.processes.push_back(m);
		currentlyRebootingProcesses.erase(addresses.address);
		m->excluded = g_simulator.isExcluded(NetworkAddress(ip, port, true, false));
		m->cleared = g_simulator.isCleared(addresses.address);
		m->protocolVersion = protocol;

		m->setGlobal(enTDMetrics, (flowGlobalType)&m->tdmetrics);
		if (FLOW_KNOBS->ENABLE_CHAOS_FEATURES) {
			m->setGlobal(enChaosMetrics, (flowGlobalType)&m->chaosMetrics);
		}
		m->setGlobal(enNetworkConnections, (flowGlobalType)m->network);
		m->setGlobal(enASIOTimedOut, (flowGlobalType) false);

		TraceEvent("NewMachine")
		    .detail("Name", name)
		    .detail("Address", m->address)
		    .detail("MachineId", m->locality.machineId())
		    .detail("Excluded", m->excluded)
		    .detail("Cleared", m->cleared);

		if (std::string(name) == "remote flow process") {
			protectedAddresses.insert(m->address);
			TraceEvent(SevDebug, "NewFlowProcessProtected").detail("Address", m->address);
		}

		// FIXME: Sometimes, connections to/from this process will explicitly close

		return m;
	}
	bool isAvailable() const override {
		std::vector<ProcessInfo*> processesLeft, processesDead;
		for (auto processInfo : getAllProcesses()) {
			if (processInfo->isAvailableClass()) {
				if (processInfo->isExcluded() || processInfo->isCleared() || !processInfo->isAvailable()) {
					processesDead.push_back(processInfo);
				} else {
					processesLeft.push_back(processInfo);
				}
			}
		}
		return canKillProcesses(processesLeft, processesDead, KillInstantly, nullptr);
	}

	bool datacenterDead(Optional<Standalone<StringRef>> dcId) const override {
		if (!dcId.present()) {
			return false;
		}

		LocalityGroup primaryProcessesLeft, primaryProcessesDead;
		std::vector<LocalityData> primaryLocalitiesDead, primaryLocalitiesLeft;

		for (auto processInfo : getAllProcesses()) {
			if (processInfo->isAvailableClass() && processInfo->locality.dcId() == dcId) {
				if (processInfo->isExcluded() || processInfo->isCleared() || !processInfo->isAvailable()) {
					primaryProcessesDead.add(processInfo->locality);
					primaryLocalitiesDead.push_back(processInfo->locality);
				} else {
					primaryProcessesLeft.add(processInfo->locality);
					primaryLocalitiesLeft.push_back(processInfo->locality);
				}
			}
		}

		std::vector<LocalityData> badCombo;
		bool primaryTLogsDead =
		    tLogWriteAntiQuorum
		        ? !validateAllCombinations(
		              badCombo, primaryProcessesDead, tLogPolicy, primaryLocalitiesLeft, tLogWriteAntiQuorum, false)
		        : primaryProcessesDead.validate(tLogPolicy);
		if (usableRegions > 1 && remoteTLogPolicy && !primaryTLogsDead) {
			primaryTLogsDead = primaryProcessesDead.validate(remoteTLogPolicy);
		}

		return primaryTLogsDead || primaryProcessesDead.validate(storagePolicy);
	}

	// The following function will determine if the specified configuration of available and dead processes can allow
	// the cluster to survive
	bool canKillProcesses(std::vector<ProcessInfo*> const& availableProcesses,
	                      std::vector<ProcessInfo*> const& deadProcesses,
	                      KillType kt,
	                      KillType* newKillType) const override {
		bool canSurvive = true;
		int nQuorum = ((desiredCoordinators + 1) / 2) * 2 - 1;

		KillType newKt = kt;
		if ((kt == KillInstantly) || (kt == InjectFaults) || (kt == FailDisk) || (kt == RebootAndDelete) ||
		    (kt == RebootProcessAndDelete)) {
			LocalityGroup primaryProcessesLeft, primaryProcessesDead;
			LocalityGroup primarySatelliteProcessesLeft, primarySatelliteProcessesDead;
			LocalityGroup remoteProcessesLeft, remoteProcessesDead;
			LocalityGroup remoteSatelliteProcessesLeft, remoteSatelliteProcessesDead;

			std::vector<LocalityData> primaryLocalitiesDead, primaryLocalitiesLeft;
			std::vector<LocalityData> primarySatelliteLocalitiesDead, primarySatelliteLocalitiesLeft;
			std::vector<LocalityData> remoteLocalitiesDead, remoteLocalitiesLeft;
			std::vector<LocalityData> remoteSatelliteLocalitiesDead, remoteSatelliteLocalitiesLeft;

			std::vector<LocalityData> badCombo;
			std::set<Optional<Standalone<StringRef>>> uniqueMachines;

			if (!primaryDcId.present()) {
				for (auto processInfo : availableProcesses) {
					primaryProcessesLeft.add(processInfo->locality);
					primaryLocalitiesLeft.push_back(processInfo->locality);
					uniqueMachines.insert(processInfo->locality.zoneId());
				}
				for (auto processInfo : deadProcesses) {
					primaryProcessesDead.add(processInfo->locality);
					primaryLocalitiesDead.push_back(processInfo->locality);
				}
			} else {
				for (auto processInfo : availableProcesses) {
					uniqueMachines.insert(processInfo->locality.zoneId());
					if (processInfo->locality.dcId() == primaryDcId) {
						primaryProcessesLeft.add(processInfo->locality);
						primaryLocalitiesLeft.push_back(processInfo->locality);
					} else if (processInfo->locality.dcId() == remoteDcId) {
						remoteProcessesLeft.add(processInfo->locality);
						remoteLocalitiesLeft.push_back(processInfo->locality);
					} else if (std::find(primarySatelliteDcIds.begin(),
					                     primarySatelliteDcIds.end(),
					                     processInfo->locality.dcId()) != primarySatelliteDcIds.end()) {
						primarySatelliteProcessesLeft.add(processInfo->locality);
						primarySatelliteLocalitiesLeft.push_back(processInfo->locality);
					} else if (std::find(remoteSatelliteDcIds.begin(),
					                     remoteSatelliteDcIds.end(),
					                     processInfo->locality.dcId()) != remoteSatelliteDcIds.end()) {
						remoteSatelliteProcessesLeft.add(processInfo->locality);
						remoteSatelliteLocalitiesLeft.push_back(processInfo->locality);
					}
				}
				for (auto processInfo : deadProcesses) {
					if (processInfo->locality.dcId() == primaryDcId) {
						primaryProcessesDead.add(processInfo->locality);
						primaryLocalitiesDead.push_back(processInfo->locality);
					} else if (processInfo->locality.dcId() == remoteDcId) {
						remoteProcessesDead.add(processInfo->locality);
						remoteLocalitiesDead.push_back(processInfo->locality);
					} else if (std::find(primarySatelliteDcIds.begin(),
					                     primarySatelliteDcIds.end(),
					                     processInfo->locality.dcId()) != primarySatelliteDcIds.end()) {
						primarySatelliteProcessesDead.add(processInfo->locality);
						primarySatelliteLocalitiesDead.push_back(processInfo->locality);
					} else if (std::find(remoteSatelliteDcIds.begin(),
					                     remoteSatelliteDcIds.end(),
					                     processInfo->locality.dcId()) != remoteSatelliteDcIds.end()) {
						remoteSatelliteProcessesDead.add(processInfo->locality);
						remoteSatelliteLocalitiesDead.push_back(processInfo->locality);
					}
				}
			}

			bool tooManyDead = false;
			bool notEnoughLeft = false;
			bool primaryTLogsDead =
			    tLogWriteAntiQuorum
			        ? !validateAllCombinations(
			              badCombo, primaryProcessesDead, tLogPolicy, primaryLocalitiesLeft, tLogWriteAntiQuorum, false)
			        : primaryProcessesDead.validate(tLogPolicy);
			if (usableRegions > 1 && remoteTLogPolicy && !primaryTLogsDead) {
				primaryTLogsDead = primaryProcessesDead.validate(remoteTLogPolicy);
			}

			if (!primaryDcId.present()) {
				tooManyDead = primaryTLogsDead || primaryProcessesDead.validate(storagePolicy);
				notEnoughLeft =
				    !primaryProcessesLeft.validate(tLogPolicy) || !primaryProcessesLeft.validate(storagePolicy);
			} else {
				bool remoteTLogsDead = tLogWriteAntiQuorum ? !validateAllCombinations(badCombo,
				                                                                      remoteProcessesDead,
				                                                                      tLogPolicy,
				                                                                      remoteLocalitiesLeft,
				                                                                      tLogWriteAntiQuorum,
				                                                                      false)
				                                           : remoteProcessesDead.validate(tLogPolicy);
				if (usableRegions > 1 && remoteTLogPolicy && !remoteTLogsDead) {
					remoteTLogsDead = remoteProcessesDead.validate(remoteTLogPolicy);
				}

				if (!hasSatelliteReplication) {
					if (usableRegions > 1) {
						tooManyDead = primaryTLogsDead || remoteTLogsDead ||
						              (primaryProcessesDead.validate(storagePolicy) &&
						               remoteProcessesDead.validate(storagePolicy));
						notEnoughLeft = !primaryProcessesLeft.validate(tLogPolicy) ||
						                !primaryProcessesLeft.validate(remoteTLogPolicy) ||
						                !primaryProcessesLeft.validate(storagePolicy) ||
						                !remoteProcessesLeft.validate(tLogPolicy) ||
						                !remoteProcessesLeft.validate(remoteTLogPolicy) ||
						                !remoteProcessesLeft.validate(storagePolicy);
					} else {
						tooManyDead = primaryTLogsDead || remoteTLogsDead ||
						              primaryProcessesDead.validate(storagePolicy) ||
						              remoteProcessesDead.validate(storagePolicy);
						notEnoughLeft = !primaryProcessesLeft.validate(tLogPolicy) ||
						                !primaryProcessesLeft.validate(storagePolicy) ||
						                !remoteProcessesLeft.validate(tLogPolicy) ||
						                !remoteProcessesLeft.validate(storagePolicy);
					}
				} else {
					bool primarySatelliteTLogsDead =
					    satelliteTLogWriteAntiQuorumFallback
					        ? !validateAllCombinations(badCombo,
					                                   primarySatelliteProcessesDead,
					                                   satelliteTLogPolicyFallback,
					                                   primarySatelliteLocalitiesLeft,
					                                   satelliteTLogWriteAntiQuorumFallback,
					                                   false)
					        : primarySatelliteProcessesDead.validate(satelliteTLogPolicyFallback);
					bool remoteSatelliteTLogsDead =
					    satelliteTLogWriteAntiQuorumFallback
					        ? !validateAllCombinations(badCombo,
					                                   remoteSatelliteProcessesDead,
					                                   satelliteTLogPolicyFallback,
					                                   remoteSatelliteLocalitiesLeft,
					                                   satelliteTLogWriteAntiQuorumFallback,
					                                   false)
					        : remoteSatelliteProcessesDead.validate(satelliteTLogPolicyFallback);

					if (usableRegions > 1) {
						notEnoughLeft = !primaryProcessesLeft.validate(tLogPolicy) ||
						                !primaryProcessesLeft.validate(remoteTLogPolicy) ||
						                !primaryProcessesLeft.validate(storagePolicy) ||
						                !primarySatelliteProcessesLeft.validate(satelliteTLogPolicy) ||
						                !remoteProcessesLeft.validate(tLogPolicy) ||
						                !remoteProcessesLeft.validate(remoteTLogPolicy) ||
						                !remoteProcessesLeft.validate(storagePolicy) ||
						                !remoteSatelliteProcessesLeft.validate(satelliteTLogPolicy);
					} else {
						notEnoughLeft = !primaryProcessesLeft.validate(tLogPolicy) ||
						                !primaryProcessesLeft.validate(storagePolicy) ||
						                !primarySatelliteProcessesLeft.validate(satelliteTLogPolicy) ||
						                !remoteProcessesLeft.validate(tLogPolicy) ||
						                !remoteProcessesLeft.validate(storagePolicy) ||
						                !remoteSatelliteProcessesLeft.validate(satelliteTLogPolicy);
					}

					if (usableRegions > 1 && allowLogSetKills) {
						tooManyDead = (primaryTLogsDead && primarySatelliteTLogsDead) ||
						              (remoteTLogsDead && remoteSatelliteTLogsDead) ||
						              (primaryTLogsDead && remoteTLogsDead) ||
						              (primaryProcessesDead.validate(storagePolicy) &&
						               remoteProcessesDead.validate(storagePolicy));
					} else {
						tooManyDead = primaryTLogsDead || remoteTLogsDead ||
						              primaryProcessesDead.validate(storagePolicy) ||
						              remoteProcessesDead.validate(storagePolicy);
					}
				}
			}

			// Reboot if dead machines do fulfill policies
			if (tooManyDead) {
				newKt = Reboot;
				canSurvive = false;
				TraceEvent("KillChanged")
				    .detail("KillType", kt)
				    .detail("NewKillType", newKt)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("Reason", "Too many dead processes that cannot satisfy tLogPolicy.");
			}
			// Reboot and Delete if remaining machines do NOT fulfill policies
			else if ((kt < RebootAndDelete) && notEnoughLeft) {
				newKt = RebootAndDelete;
				canSurvive = false;
				TraceEvent("KillChanged")
				    .detail("KillType", kt)
				    .detail("NewKillType", newKt)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("Reason", "Not enough tLog left to satisfy tLogPolicy.");
			} else if ((kt < RebootAndDelete) && (nQuorum > uniqueMachines.size())) {
				newKt = RebootAndDelete;
				canSurvive = false;
				TraceEvent("KillChanged")
				    .detail("KillType", kt)
				    .detail("NewKillType", newKt)
				    .detail("StoragePolicy", storagePolicy->info())
				    .detail("Quorum", nQuorum)
				    .detail("Machines", uniqueMachines.size())
				    .detail("Reason", "Not enough unique machines to perform auto configuration of coordinators.");
			} else {
				TraceEvent("CanSurviveKills")
				    .detail("KillType", kt)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("StoragePolicy", storagePolicy->info())
				    .detail("Quorum", nQuorum)
				    .detail("Machines", uniqueMachines.size());
			}
		}
		if (newKillType)
			*newKillType = newKt;
		return canSurvive;
	}

	void destroyProcess(ISimulator::ProcessInfo* p) override {
		TraceEvent("ProcessDestroyed")
		    .detail("Name", p->name)
		    .detail("Address", p->address)
		    .detail("MachineId", p->locality.machineId());
		currentlyRebootingProcesses.insert(std::pair<NetworkAddress, ProcessInfo*>(p->address, p));
		std::vector<ProcessInfo*>& processes = machines[p->locality.machineId().get()].processes;
		machines[p->locality.machineId().get()].removeRemotePort(p->address.port);
		if (p != processes.back()) {
			auto it = std::find(processes.begin(), processes.end(), p);
			std::swap(*it, processes.back());
		}
		processes.pop_back();
		killProcess_internal(p, KillInstantly);
	}
	void killProcess_internal(ProcessInfo* machine, KillType kt) {
		TEST(true); // Simulated machine was killed with any kill type
		TEST(kt == KillInstantly); // Simulated machine was killed instantly
		TEST(kt == InjectFaults); // Simulated machine was killed with faults
		TEST(kt == FailDisk); // Simulated machine was killed with a failed disk

		if (kt == KillInstantly) {
			TraceEvent(SevWarn, "FailMachine")
			    .detail("Name", machine->name)
			    .detail("Address", machine->address)
			    .detail("ZoneId", machine->locality.zoneId())
			    .detail("Process", machine->toString())
			    .detail("Rebooting", machine->rebooting)
			    .detail("Protected", protectedAddresses.count(machine->address))
			    .backtrace();
			// This will remove all the "tracked" messages that came from the machine being killed
			if (std::string(machine->name) != "remote flow process")
				latestEventCache.clear();
			machine->failed = true;
		} else if (kt == InjectFaults) {
			TraceEvent(SevWarn, "FaultMachine")
			    .detail("Name", machine->name)
			    .detail("Address", machine->address)
			    .detail("ZoneId", machine->locality.zoneId())
			    .detail("Process", machine->toString())
			    .detail("Rebooting", machine->rebooting)
			    .detail("Protected", protectedAddresses.count(machine->address))
			    .backtrace();
			should_inject_fault = simulator_should_inject_fault;
			machine->fault_injection_r = deterministicRandom()->randomUniqueID().first();
			machine->fault_injection_p1 = 0.1;
			machine->fault_injection_p2 = deterministicRandom()->random01();
		} else if (kt == FailDisk) {
			TraceEvent(SevWarn, "FailDiskMachine")
			    .detail("Name", machine->name)
			    .detail("Address", machine->address)
			    .detail("ZoneId", machine->locality.zoneId())
			    .detail("Process", machine->toString())
			    .detail("Rebooting", machine->rebooting)
			    .detail("Protected", protectedAddresses.count(machine->address))
			    .backtrace();
			machine->failedDisk = true;
		} else {
			ASSERT(false);
		}
		ASSERT(!protectedAddresses.count(machine->address) || machine->rebooting ||
		       std::string(machine->name) == "remote flow process");
	}
	void rebootProcess(ProcessInfo* process, KillType kt) override {
		if (kt == RebootProcessAndDelete && protectedAddresses.count(process->address)) {
			TraceEvent("RebootChanged")
			    .detail("ZoneId", process->locality.describeZone())
			    .detail("KillType", RebootProcess)
			    .detail("OrigKillType", kt)
			    .detail("Reason", "Protected process");
			kt = RebootProcess;
		}
		doReboot(process, kt);
	}
	void rebootProcess(Optional<Standalone<StringRef>> zoneId, bool allProcesses) override {
		if (allProcesses) {
			auto processes = getAllProcesses();
			for (int i = 0; i < processes.size(); i++)
				if (processes[i]->locality.zoneId() == zoneId && !processes[i]->rebooting)
					doReboot(processes[i], RebootProcess);
		} else {
			auto processes = getAllProcesses();
			for (int i = 0; i < processes.size(); i++) {
				if (processes[i]->locality.zoneId() != zoneId || processes[i]->rebooting) {
					swapAndPop(&processes, i--);
				}
			}
			if (processes.size())
				doReboot(deterministicRandom()->randomChoice(processes), RebootProcess);
		}
	}
	void killProcess(ProcessInfo* machine, KillType kt) override {
		TraceEvent("AttemptingKillProcess").detail("ProcessInfo", machine->toString());
		if (kt < RebootAndDelete) {
			killProcess_internal(machine, kt);
		}
	}
	void killInterface(NetworkAddress address, KillType kt) override {
		if (kt < RebootAndDelete) {
			std::vector<ProcessInfo*>& processes = machines[addressMap[address]->locality.machineId()].processes;
			for (int i = 0; i < processes.size(); i++)
				killProcess_internal(processes[i], kt);
		}
	}
	bool killZone(Optional<Standalone<StringRef>> zoneId, KillType kt, bool forceKill, KillType* ktFinal) override {
		auto processes = getAllProcesses();
		std::set<Optional<Standalone<StringRef>>> zoneMachines;
		for (auto& process : processes) {
			if (process->locality.zoneId() == zoneId) {
				zoneMachines.insert(process->locality.machineId());
			}
		}
		bool result = false;
		for (auto& machineId : zoneMachines) {
			if (killMachine(machineId, kt, forceKill, ktFinal)) {
				result = true;
			}
		}
		return result;
	}
	bool killMachine(Optional<Standalone<StringRef>> machineId,
	                 KillType kt,
	                 bool forceKill,
	                 KillType* ktFinal) override {
		auto ktOrig = kt;

		TEST(true); // Trying to killing a machine
		TEST(kt == KillInstantly); // Trying to kill instantly
		TEST(kt == InjectFaults); // Trying to kill by injecting faults

		if (speedUpSimulation && !forceKill) {
			TraceEvent(SevWarn, "AbortedKill")
			    .detail("MachineId", machineId)
			    .detail("Reason", "Unforced kill within speedy simulation.")
			    .backtrace();
			if (ktFinal)
				*ktFinal = None;
			return false;
		}

		int processesOnMachine = 0;

		KillType originalKt = kt;
		// Reboot if any of the processes are protected and count the number of processes not rebooting
		for (auto& process : machines[machineId].processes) {
			if (protectedAddresses.count(process->address))
				kt = Reboot;
			if (!process->rebooting)
				processesOnMachine++;
		}

		// Do nothing, if no processes to kill
		if (processesOnMachine == 0) {
			TraceEvent(SevWarn, "AbortedKill")
			    .detail("MachineId", machineId)
			    .detail("Reason", "The target had no processes running.")
			    .detail("Processes", processesOnMachine)
			    .detail("ProcessesPerMachine", processesPerMachine)
			    .backtrace();
			if (ktFinal)
				*ktFinal = None;
			return false;
		}

		// Check if machine can be removed, if requested
		if (!forceKill && ((kt == KillInstantly) || (kt == InjectFaults) || (kt == FailDisk) ||
		                   (kt == RebootAndDelete) || (kt == RebootProcessAndDelete))) {
			std::vector<ProcessInfo*> processesLeft, processesDead;
			int protectedWorker = 0, unavailable = 0, excluded = 0, cleared = 0;

			for (auto processInfo : getAllProcesses()) {
				if (processInfo->isAvailableClass()) {
					if (processInfo->isExcluded()) {
						processesDead.push_back(processInfo);
						excluded++;
					} else if (processInfo->isCleared()) {
						processesDead.push_back(processInfo);
						cleared++;
					} else if (!processInfo->isAvailable()) {
						processesDead.push_back(processInfo);
						unavailable++;
					} else if (protectedAddresses.count(processInfo->address)) {
						processesLeft.push_back(processInfo);
						protectedWorker++;
					} else if (processInfo->locality.machineId() != machineId) {
						processesLeft.push_back(processInfo);
					} else {
						processesDead.push_back(processInfo);
					}
				}
			}
			if (!canKillProcesses(processesLeft, processesDead, kt, &kt)) {
				TraceEvent("ChangedKillMachine")
				    .detail("MachineId", machineId)
				    .detail("KillType", kt)
				    .detail("OrigKillType", ktOrig)
				    .detail("ProcessesLeft", processesLeft.size())
				    .detail("ProcessesDead", processesDead.size())
				    .detail("TotalProcesses", machines.size())
				    .detail("ProcessesPerMachine", processesPerMachine)
				    .detail("Protected", protectedWorker)
				    .detail("Unavailable", unavailable)
				    .detail("Excluded", excluded)
				    .detail("Cleared", cleared)
				    .detail("ProtectedTotal", protectedAddresses.size())
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("StoragePolicy", storagePolicy->info());
			} else if ((kt == KillInstantly) || (kt == InjectFaults) || (kt == FailDisk)) {
				TraceEvent("DeadMachine")
				    .detail("MachineId", machineId)
				    .detail("KillType", kt)
				    .detail("ProcessesLeft", processesLeft.size())
				    .detail("ProcessesDead", processesDead.size())
				    .detail("TotalProcesses", machines.size())
				    .detail("ProcessesPerMachine", processesPerMachine)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("StoragePolicy", storagePolicy->info());
				for (auto process : processesLeft) {
					TraceEvent("DeadMachineSurvivors")
					    .detail("MachineId", machineId)
					    .detail("KillType", kt)
					    .detail("ProcessesLeft", processesLeft.size())
					    .detail("ProcessesDead", processesDead.size())
					    .detail("SurvivingProcess", process->toString());
				}
				for (auto process : processesDead) {
					TraceEvent("DeadMachineVictims")
					    .detail("MachineId", machineId)
					    .detail("KillType", kt)
					    .detail("ProcessesLeft", processesLeft.size())
					    .detail("ProcessesDead", processesDead.size())
					    .detail("VictimProcess", process->toString());
				}
			} else {
				TraceEvent("ClearMachine")
				    .detail("MachineId", machineId)
				    .detail("KillType", kt)
				    .detail("ProcessesLeft", processesLeft.size())
				    .detail("ProcessesDead", processesDead.size())
				    .detail("TotalProcesses", machines.size())
				    .detail("ProcessesPerMachine", processesPerMachine)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("StoragePolicy", storagePolicy->info());
				for (auto process : processesLeft) {
					TraceEvent("ClearMachineSurvivors")
					    .detail("MachineId", machineId)
					    .detail("KillType", kt)
					    .detail("ProcessesLeft", processesLeft.size())
					    .detail("ProcessesDead", processesDead.size())
					    .detail("SurvivingProcess", process->toString());
				}
				for (auto process : processesDead) {
					TraceEvent("ClearMachineVictims")
					    .detail("MachineId", machineId)
					    .detail("KillType", kt)
					    .detail("ProcessesLeft", processesLeft.size())
					    .detail("ProcessesDead", processesDead.size())
					    .detail("VictimProcess", process->toString());
				}
			}
		}

		TEST(originalKt != kt); // Kill type was changed from requested to reboot.

		// Check if any processes on machine are rebooting
		if (processesOnMachine != processesPerMachine && kt >= RebootAndDelete) {
			TEST(true); // Attempted reboot, but the target did not have all of its processes running
			TraceEvent(SevWarn, "AbortedKill")
			    .detail("KillType", kt)
			    .detail("MachineId", machineId)
			    .detail("Reason", "Machine processes does not match number of processes per machine")
			    .detail("Processes", processesOnMachine)
			    .detail("ProcessesPerMachine", processesPerMachine)
			    .backtrace();
			if (ktFinal)
				*ktFinal = None;
			return false;
		}

		// Check if any processes on machine are rebooting
		if (processesOnMachine != processesPerMachine) {
			TEST(true); // Attempted reboot and kill, but the target did not have all of its processes running
			TraceEvent(SevWarn, "AbortedKill")
			    .detail("KillType", kt)
			    .detail("MachineId", machineId)
			    .detail("Reason", "Machine processes does not match number of processes per machine")
			    .detail("Processes", processesOnMachine)
			    .detail("ProcessesPerMachine", processesPerMachine)
			    .backtrace();
			if (ktFinal)
				*ktFinal = None;
			return false;
		}

		TraceEvent("KillMachine")
		    .detail("MachineId", machineId)
		    .detail("Kt", kt)
		    .detail("KtOrig", ktOrig)
		    .detail("KillableMachines", processesOnMachine)
		    .detail("ProcessPerMachine", processesPerMachine)
		    .detail("KillChanged", kt != ktOrig);
		if (kt < RebootAndDelete) {
			if ((kt == InjectFaults || kt == FailDisk) && machines[machineId].machineProcess != nullptr)
				killProcess_internal(machines[machineId].machineProcess, kt);
			for (auto& process : machines[machineId].processes) {
				TraceEvent("KillMachineProcess")
				    .detail("KillType", kt)
				    .detail("Process", process->toString())
				    .detail("StartingClass", process->startingClass.toString())
				    .detail("Failed", process->failed)
				    .detail("Excluded", process->excluded)
				    .detail("Cleared", process->cleared)
				    .detail("Rebooting", process->rebooting);
				if (process->startingClass != ProcessClass::TesterClass)
					killProcess_internal(process, kt);
			}
		} else if (kt == Reboot || kt == RebootAndDelete) {
			for (auto& process : machines[machineId].processes) {
				TraceEvent("KillMachineProcess")
				    .detail("KillType", kt)
				    .detail("Process", process->toString())
				    .detail("StartingClass", process->startingClass.toString())
				    .detail("Failed", process->failed)
				    .detail("Excluded", process->excluded)
				    .detail("Cleared", process->cleared)
				    .detail("Rebooting", process->rebooting);
				if (process->startingClass != ProcessClass::TesterClass)
					doReboot(process, kt);
			}
		}

		TEST(kt == RebootAndDelete); // Resulted in a reboot and delete
		TEST(kt == Reboot); // Resulted in a reboot
		TEST(kt == KillInstantly); // Resulted in an instant kill
		TEST(kt == InjectFaults); // Resulted in a kill by injecting faults

		if (ktFinal)
			*ktFinal = kt;
		return true;
	}

	bool killDataCenter(Optional<Standalone<StringRef>> dcId, KillType kt, bool forceKill, KillType* ktFinal) override {
		auto ktOrig = kt;
		auto processes = getAllProcesses();
		std::map<Optional<Standalone<StringRef>>, int> datacenterMachines;
		int dcProcesses = 0;

		// Switch to a reboot, if anything protected on machine
		for (auto& procRecord : processes) {
			auto processDcId = procRecord->locality.dcId();
			auto processMachineId = procRecord->locality.machineId();
			ASSERT(processMachineId.present());
			if (processDcId.present() && (processDcId == dcId)) {
				if ((kt != Reboot) && (protectedAddresses.count(procRecord->address))) {
					kt = Reboot;
					TraceEvent(SevWarn, "DcKillChanged")
					    .detail("DataCenter", dcId)
					    .detail("KillType", kt)
					    .detail("OrigKillType", ktOrig)
					    .detail("Reason", "Datacenter has protected process")
					    .detail("ProcessAddress", procRecord->address)
					    .detail("Failed", procRecord->failed)
					    .detail("Rebooting", procRecord->rebooting)
					    .detail("Excluded", procRecord->excluded)
					    .detail("Cleared", procRecord->cleared)
					    .detail("Process", procRecord->toString());
				}
				datacenterMachines[processMachineId.get()]++;
				dcProcesses++;
			}
		}

		// Check if machine can be removed, if requested
		if (!forceKill && ((kt == KillInstantly) || (kt == InjectFaults) || (kt == FailDisk) ||
		                   (kt == RebootAndDelete) || (kt == RebootProcessAndDelete))) {
			std::vector<ProcessInfo*> processesLeft, processesDead;
			for (auto processInfo : getAllProcesses()) {
				if (processInfo->isAvailableClass()) {
					if (processInfo->isExcluded() || processInfo->isCleared() || !processInfo->isAvailable()) {
						processesDead.push_back(processInfo);
					} else if (protectedAddresses.count(processInfo->address) ||
					           datacenterMachines.find(processInfo->locality.machineId()) == datacenterMachines.end()) {
						processesLeft.push_back(processInfo);
					} else {
						processesDead.push_back(processInfo);
					}
				}
			}

			if (!canKillProcesses(processesLeft, processesDead, kt, &kt)) {
				TraceEvent(SevWarn, "DcKillChanged")
				    .detail("DataCenter", dcId)
				    .detail("KillType", kt)
				    .detail("OrigKillType", ktOrig);
			} else {
				TraceEvent("DeadDataCenter")
				    .detail("DataCenter", dcId)
				    .detail("KillType", kt)
				    .detail("DcZones", datacenterMachines.size())
				    .detail("DcProcesses", dcProcesses)
				    .detail("ProcessesDead", processesDead.size())
				    .detail("ProcessesLeft", processesLeft.size())
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("StoragePolicy", storagePolicy->info());
				for (auto process : processesLeft) {
					TraceEvent("DeadDcSurvivors")
					    .detail("MachineId", process->locality.machineId())
					    .detail("KillType", kt)
					    .detail("ProcessesLeft", processesLeft.size())
					    .detail("ProcessesDead", processesDead.size())
					    .detail("SurvivingProcess", process->toString());
				}
				for (auto process : processesDead) {
					TraceEvent("DeadDcVictims")
					    .detail("MachineId", process->locality.machineId())
					    .detail("KillType", kt)
					    .detail("ProcessesLeft", processesLeft.size())
					    .detail("ProcessesDead", processesDead.size())
					    .detail("VictimProcess", process->toString());
				}
			}
		}

		KillType ktResult, ktMin = kt;
		for (auto& datacenterMachine : datacenterMachines) {
			if (deterministicRandom()->random01() < 0.99 || forceKill) {
				killMachine(datacenterMachine.first, kt, true, &ktResult);
				if (ktResult != kt) {
					TraceEvent(SevWarn, "KillDCFail")
					    .detail("Zone", datacenterMachine.first)
					    .detail("KillType", kt)
					    .detail("KillTypeResult", ktResult)
					    .detail("KillTypeOrig", ktOrig);
					ASSERT(ktResult == None);
				}
				ktMin = std::min<KillType>(ktResult, ktMin);
			}
		}

		TraceEvent("KillDataCenter")
		    .detail("DcZones", datacenterMachines.size())
		    .detail("DcProcesses", dcProcesses)
		    .detail("DCID", dcId)
		    .detail("KillType", kt)
		    .detail("KillTypeOrig", ktOrig)
		    .detail("KillTypeMin", ktMin)
		    .detail("KilledDC", kt == ktMin);

		TEST(kt != ktMin); // DataCenter kill was rejected by killMachine
		TEST((kt == ktMin) && (kt == RebootAndDelete)); // Datacenter kill Resulted in a reboot and delete
		TEST((kt == ktMin) && (kt == Reboot)); // Datacenter kill Resulted in a reboot
		TEST((kt == ktMin) && (kt == KillInstantly)); // Datacenter kill Resulted in an instant kill
		TEST((kt == ktMin) && (kt == InjectFaults)); // Datacenter kill Resulted in a kill by injecting faults
		TEST((kt == ktMin) && (kt != ktOrig)); // Datacenter Kill request was downgraded
		TEST((kt == ktMin) && (kt == ktOrig)); // Datacenter kill - Requested kill was done

		if (ktFinal)
			*ktFinal = ktMin;

		return (kt == ktMin);
	}
	void clogInterface(const IPAddress& ip, double seconds, ClogMode mode = ClogDefault) override {
		if (mode == ClogDefault) {
			double a = deterministicRandom()->random01();
			if (a < 0.3)
				mode = ClogSend;
			else if (a < 0.6)
				mode = ClogReceive;
			else
				mode = ClogAll;
		}
		TraceEvent("ClogInterface")
		    .detail("IP", ip.toString())
		    .detail("Delay", seconds)
		    .detail("Queue",
		            mode == ClogSend      ? "Send"
		            : mode == ClogReceive ? "Receive"
		                                  : "All");

		if (mode == ClogSend || mode == ClogAll)
			g_clogging.clogSendFor(ip, seconds);
		if (mode == ClogReceive || mode == ClogAll)
			g_clogging.clogRecvFor(ip, seconds);
	}
	void clogPair(const IPAddress& from, const IPAddress& to, double seconds) override {
		TraceEvent("CloggingPair").detail("From", from).detail("To", to).detail("Seconds", seconds);
		g_clogging.clogPairFor(from, to, seconds);
	}

	void unclogPair(const IPAddress& from, const IPAddress& to) override {
		TraceEvent("UncloggingPair").detail("From", from).detail("To", to);
		g_clogging.unclogPair(from, to);
	}

	void disconnectPair(const IPAddress& from, const IPAddress& to, double seconds) override {
		TraceEvent("DisconnectPair").detail("From", from).detail("To", to).detail("Seconds", seconds);
		g_clogging.disconnectPairFor(from, to, seconds);
	}

	void reconnectPair(const IPAddress& from, const IPAddress& to) override {
		TraceEvent("ReconnectPair").detail("From", from).detail("To", to);
		g_clogging.reconnectPair(from, to);
	}

	std::vector<ProcessInfo*> getAllProcesses() const override {
		std::vector<ProcessInfo*> processes;
		for (auto& c : machines) {
			processes.insert(processes.end(), c.second.processes.begin(), c.second.processes.end());
		}
		for (auto& c : currentlyRebootingProcesses) {
			processes.push_back(c.second);
		}
		return processes;
	}
	ProcessInfo* getProcessByAddress(NetworkAddress const& address) override {
		NetworkAddress normalizedAddress(address.ip, address.port, true, address.isTLS());
		ASSERT(addressMap.count(normalizedAddress));
		// NOTE: addressMap[normalizedAddress]->address may not equal to normalizedAddress
		return addressMap[normalizedAddress];
	}

	MachineInfo* getMachineByNetworkAddress(NetworkAddress const& address) override {
		return &machines[addressMap[address]->locality.machineId()];
	}

	MachineInfo* getMachineById(Optional<Standalone<StringRef>> const& machineId) override {
		return &machines[machineId];
	}

	void destroyMachine(Optional<Standalone<StringRef>> const& machineId) override {
		auto& machine = machines[machineId];
		for (auto process : machine.processes) {
			ASSERT(process->failed);
		}
		if (machine.machineProcess) {
			killProcess_internal(machine.machineProcess, KillInstantly);
		}
		machines.erase(machineId);
	}

	Sim2(bool printSimTime)
	  : time(0.0), timerTime(0.0), currentTaskID(TaskPriority::Zero), taskCount(0), yielded(false), yield_limit(0),
	    printSimTime(printSimTime) {
		// Not letting currentProcess be nullptr eliminates some annoying special cases
		currentProcess =
		    new ProcessInfo("NoMachine",
		                    LocalityData(Optional<Standalone<StringRef>>(), StringRef(), StringRef(), StringRef()),
		                    ProcessClass(),
		                    { NetworkAddress() },
		                    this,
		                    "",
		                    "");
		g_network = net2 = newNet2(TLSConfig(), false, true);
		g_network->addStopCallback(Net2FileSystem::stop);
		Net2FileSystem::newFileSystem();
		check_yield(TaskPriority::Zero);
	}

	// Implementation
	struct Task {
		TaskPriority taskID;
		double time;
		uint64_t stable;
		ProcessInfo* machine;
		Promise<Void> action;
		Task(double time, TaskPriority taskID, uint64_t stable, ProcessInfo* machine, Promise<Void>&& action)
		  : taskID(taskID), time(time), stable(stable), machine(machine), action(std::move(action)) {}
		Task(double time, TaskPriority taskID, uint64_t stable, ProcessInfo* machine, Future<Void>& future)
		  : taskID(taskID), time(time), stable(stable), machine(machine) {
			future = action.getFuture();
		}
		Task(Task&& rhs) noexcept
		  : taskID(rhs.taskID), time(rhs.time), stable(rhs.stable), machine(rhs.machine),
		    action(std::move(rhs.action)) {}
		void operator=(Task const& rhs) {
			taskID = rhs.taskID;
			time = rhs.time;
			stable = rhs.stable;
			machine = rhs.machine;
			action = rhs.action;
		}
		Task(Task const& rhs)
		  : taskID(rhs.taskID), time(rhs.time), stable(rhs.stable), machine(rhs.machine), action(rhs.action) {}
		void operator=(Task&& rhs) noexcept {
			time = rhs.time;
			taskID = rhs.taskID;
			stable = rhs.stable;
			machine = rhs.machine;
			action = std::move(rhs.action);
		}

		bool operator<(Task const& rhs) const {
			// Ordering is reversed for priority_queue
			if (time != rhs.time)
				return time > rhs.time;
			return stable > rhs.stable;
		}
	};

	void execTask(struct Task& t) {
		if (t.machine->failed) {
			t.action.send(Never());
		} else {
			mutex.enter();
			if (printSimTime && (int)this->time < (int)t.time) {
				printf("Time: %d\n", (int)t.time);
			}
			this->time = t.time;
			this->timerTime = std::max(this->timerTime, this->time);
			mutex.leave();

			this->currentProcess = t.machine;
			try {
				t.action.send(Void());
				ASSERT(this->currentProcess == t.machine);
			} catch (Error& e) {
				TraceEvent(SevError, "UnhandledSimulationEventError").errorUnsuppressed(e);
				killProcess(t.machine, KillInstantly);
			}

			if (randLog)
				fmt::print(randLog,
				           "T {0} {1} {2} {3}\n",
				           this->time,
				           int(deterministicRandom()->peek() % 10000),
				           t.machine ? t.machine->name : "none",
				           t.stable);
		}
	}

	void onMainThread(Promise<Void>&& signal, TaskPriority taskID) override {
		// This is presumably coming from either a "fake" thread pool thread, i.e. it is actually on this thread
		// or a thread created with g_network->startThread
		ASSERT(getCurrentProcess());

		mutex.enter();
		ASSERT(taskID >= TaskPriority::Min && taskID <= TaskPriority::Max);
		tasks.push(Task(time, taskID, taskCount++, getCurrentProcess(), std::move(signal)));
		mutex.leave();
	}
	bool isOnMainThread() const override { return net2->isOnMainThread(); }
	Future<Void> onProcess(ISimulator::ProcessInfo* process, TaskPriority taskID) override {
		return delay(0, taskID, process);
	}
	Future<Void> onMachine(ISimulator::ProcessInfo* process, TaskPriority taskID) override {
		if (process->machine == 0)
			return Void();
		return delay(0, taskID, process->machine->machineProcess);
	}

	ProtocolVersion protocolVersion() const override { return getCurrentProcess()->protocolVersion; }

	// time is guarded by ISimulator::mutex. It is not necessary to guard reads on the main thread because
	// time should only be modified from the main thread.
	double time;
	double timerTime;
	TaskPriority currentTaskID;

	// taskCount is guarded by ISimulator::mutex
	uint64_t taskCount;

	std::map<Optional<Standalone<StringRef>>, MachineInfo> machines;
	std::map<NetworkAddress, ProcessInfo*> addressMap;
	std::map<ProcessInfo*, Promise<Void>> filesDeadMap;

	// tasks is guarded by ISimulator::mutex
	std::priority_queue<Task, std::vector<Task>> tasks;

	std::vector<std::function<void()>> stopCallbacks;

	// Sim2Net network;
	INetwork* net2;

	// Map from machine IP -> machine disk space info
	std::map<IPAddress, SimDiskSpace> diskSpaceMap;

	// Whether or not yield has returned true during the current iteration of the run loop
	bool yielded;
	int yield_limit; // how many more times yield may return false before next returning true
	bool printSimTime;

private:
	DNSCache mockDNS;

#ifdef ENABLE_SAMPLING
	ActorLineageSet actorLineageSet;
#endif
};

class UDPSimSocket : public IUDPSocket, ReferenceCounted<UDPSimSocket> {
	using Packet = std::shared_ptr<std::vector<uint8_t>>;
	UID id;
	ISimulator::ProcessInfo* process;
	Optional<NetworkAddress> peerAddress;
	Optional<ISimulator::ProcessInfo*> peerProcess;
	Optional<Reference<UDPSimSocket>> peerSocket;
	ActorCollection actors;
	Promise<Void> closed;
	std::deque<std::pair<NetworkAddress, Packet>> recvBuffer;
	AsyncVar<int64_t> writtenPackets;
	NetworkAddress _localAddress;
	bool randomDropPacket() {
		auto res = deterministicRandom()->random01() < .000001;
		TEST(res); // UDP packet drop
		return res;
	}

	bool isClosed() const { return closed.getFuture().isReady(); }
	Future<Void> onClosed() const { return closed.getFuture(); }

																#line 8045 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via cleanupPeerSocket()
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class CleanupPeerSocketActor>
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class CleanupPeerSocketActorState {
															#line 8051 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	CleanupPeerSocketActorState(UDPSimSocket* const& self) 
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 8058 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("cleanupPeerSocket", reinterpret_cast<unsigned long>(this));

	}
	~CleanupPeerSocketActorState() 
	{
		fdb_probe_actor_destroy("cleanupPeerSocket", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2333 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->peerSocket.get()->onClosed();
															#line 2333 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<CleanupPeerSocketActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8075 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanupPeerSocketActor*>(this)->actor_wait_state = 1;
															#line 2333 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanupPeerSocketActor, 0, Void >*>(static_cast<CleanupPeerSocketActor*>(this)));
															#line 8080 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~CleanupPeerSocketActorState();
		static_cast<CleanupPeerSocketActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2334 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->peerSocket.reset();
															#line 2335 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CleanupPeerSocketActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupPeerSocketActorState(); static_cast<CleanupPeerSocketActor*>(this)->destroy(); return 0; }
															#line 8105 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<CleanupPeerSocketActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CleanupPeerSocketActorState();
		static_cast<CleanupPeerSocketActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2334 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->peerSocket.reset();
															#line 2335 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CleanupPeerSocketActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupPeerSocketActorState(); static_cast<CleanupPeerSocketActor*>(this)->destroy(); return 0; }
															#line 8119 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<CleanupPeerSocketActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CleanupPeerSocketActorState();
		static_cast<CleanupPeerSocketActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CleanupPeerSocketActor*>(this)->actor_wait_state > 0) static_cast<CleanupPeerSocketActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupPeerSocketActor*>(this)->ActorCallback< CleanupPeerSocketActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupPeerSocketActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CleanupPeerSocketActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CleanupPeerSocketActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UDPSimSocket* self;
															#line 8192 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via cleanupPeerSocket()
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class CleanupPeerSocketActor final : public Actor<Void>, public ActorCallback< CleanupPeerSocketActor, 0, Void >, public FastAllocated<CleanupPeerSocketActor>, public CleanupPeerSocketActorState<CleanupPeerSocketActor> {
															#line 8197 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<CleanupPeerSocketActor>::operator new;
	using FastAllocated<CleanupPeerSocketActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CleanupPeerSocketActor, 0, Void >;
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	CleanupPeerSocketActor(UDPSimSocket* const& self) 
															#line 8208 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   CleanupPeerSocketActorState<CleanupPeerSocketActor>(self)
	{
		fdb_probe_actor_enter("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cleanupPeerSocket");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cleanupPeerSocket", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CleanupPeerSocketActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> cleanupPeerSocket( UDPSimSocket* const& self ) {
															#line 2332 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new CleanupPeerSocketActor(self));
															#line 8235 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 2337 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

																#line 8240 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via send()
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class SendActor>
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class SendActorState {
															#line 8246 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SendActorState(UDPSimSocket* const& self,Reference<UDPSimSocket> const& peerSocket,uint8_t const* const& begin,uint8_t const* const& end) 
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   peerSocket(peerSocket),
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   begin(begin),
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   end(end),
															#line 2342 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   packet(std::make_shared<std::vector<uint8_t>>())
															#line 8261 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("send", reinterpret_cast<unsigned long>(this));

	}
	~SendActorState() 
	{
		fdb_probe_actor_destroy("send", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2343 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			packet->resize(end - begin);
															#line 2344 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			std::copy(begin, end, packet->begin());
															#line 2345 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(.002 * deterministicRandom()->random01());
															#line 2345 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<SendActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8282 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendActor*>(this)->actor_wait_state = 1;
															#line 2345 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendActor, 0, Void >*>(static_cast<SendActor*>(this)));
															#line 8287 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~SendActorState();
		static_cast<SendActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2346 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		peerSocket->recvBuffer.emplace_back(self->_localAddress, std::move(packet));
															#line 2347 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		peerSocket->writtenPackets.set(peerSocket->writtenPackets.get() + 1);
															#line 2348 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<SendActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendActorState(); static_cast<SendActor*>(this)->destroy(); return 0; }
															#line 8314 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<SendActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendActorState();
		static_cast<SendActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2346 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		peerSocket->recvBuffer.emplace_back(self->_localAddress, std::move(packet));
															#line 2347 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		peerSocket->writtenPackets.set(peerSocket->writtenPackets.get() + 1);
															#line 2348 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<SendActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendActorState(); static_cast<SendActor*>(this)->destroy(); return 0; }
															#line 8330 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<SendActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendActorState();
		static_cast<SendActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SendActor*>(this)->actor_wait_state > 0) static_cast<SendActor*>(this)->actor_wait_state = 0;
		static_cast<SendActor*>(this)->ActorCallback< SendActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("send", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("send", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("send", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("send", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("send", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("send", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UDPSimSocket* self;
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Reference<UDPSimSocket> peerSocket;
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	uint8_t const* begin;
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	uint8_t const* end;
															#line 2342 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	Packet packet;
															#line 8411 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via send()
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class SendActor final : public Actor<Void>, public ActorCallback< SendActor, 0, Void >, public FastAllocated<SendActor>, public SendActorState<SendActor> {
															#line 8416 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<SendActor>::operator new;
	using FastAllocated<SendActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendActor, 0, Void >;
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	SendActor(UDPSimSocket* const& self,Reference<UDPSimSocket> const& peerSocket,uint8_t const* const& begin,uint8_t const* const& end) 
															#line 8427 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   SendActorState<SendActor>(self, peerSocket, begin, end)
	{
		fdb_probe_actor_enter("send", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("send");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("send", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> send( UDPSimSocket* const& self, Reference<UDPSimSocket> const& peerSocket, uint8_t const* const& begin, uint8_t const* const& end ) {
															#line 2338 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new SendActor(self, peerSocket, begin, end));
															#line 8454 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 2350 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

																#line 8459 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via receiveFrom()
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class ReceiveFromActor>
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class ReceiveFromActorState {
															#line 8465 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ReceiveFromActorState(UDPSimSocket* const& self,uint8_t* const& begin,uint8_t* const& end,NetworkAddress* const& sender) 
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   begin(begin),
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   end(end),
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   sender(sender),
															#line 2352 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   currentTaskID(g_sim2.getCurrentTask())
															#line 8480 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("receiveFrom", reinterpret_cast<unsigned long>(this));

	}
	~ReceiveFromActorState() 
	{
		fdb_probe_actor_destroy("receiveFrom", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2353 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->writtenPackets.onChange();
															#line 2353 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiveFromActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8497 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReceiveFromActor*>(this)->actor_wait_state = 1;
															#line 2353 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReceiveFromActor, 0, Void >*>(static_cast<ReceiveFromActor*>(this)));
															#line 8502 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~ReceiveFromActorState();
		static_cast<ReceiveFromActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2354 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_sim2.onProcess(self->process, currentTaskID);
															#line 2354 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiveFromActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8527 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReceiveFromActor*>(this)->actor_wait_state = 2;
															#line 2354 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReceiveFromActor, 1, Void >*>(static_cast<ReceiveFromActor*>(this)));
															#line 8532 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2354 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_sim2.onProcess(self->process, currentTaskID);
															#line 2354 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiveFromActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8543 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReceiveFromActor*>(this)->actor_wait_state = 2;
															#line 2354 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReceiveFromActor, 1, Void >*>(static_cast<ReceiveFromActor*>(this)));
															#line 8548 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReceiveFromActor*>(this)->actor_wait_state > 0) static_cast<ReceiveFromActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiveFromActor*>(this)->ActorCallback< ReceiveFromActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiveFromActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReceiveFromActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReceiveFromActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 2355 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		auto packet = self->recvBuffer.front().second;
															#line 2356 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		int sz = packet->size();
															#line 2357 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(sz <= end - begin);
															#line 2358 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (sender)
															#line 8626 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 2359 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			*sender = self->recvBuffer.front().first;
															#line 8630 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 2361 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		std::copy(packet->begin(), packet->end(), begin);
															#line 2362 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->recvBuffer.pop_front();
															#line 2363 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<ReceiveFromActor*>(this)->SAV<int>::futures) { (void)(sz); this->~ReceiveFromActorState(); static_cast<ReceiveFromActor*>(this)->destroy(); return 0; }
															#line 8638 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<ReceiveFromActor*>(this)->SAV< int >::value()) int(sz);
		this->~ReceiveFromActorState();
		static_cast<ReceiveFromActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 2355 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		auto packet = self->recvBuffer.front().second;
															#line 2356 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		int sz = packet->size();
															#line 2357 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		ASSERT(sz <= end - begin);
															#line 2358 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (sender)
															#line 8656 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		{
															#line 2359 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			*sender = self->recvBuffer.front().first;
															#line 8660 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
															#line 2361 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		std::copy(packet->begin(), packet->end(), begin);
															#line 2362 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		self->recvBuffer.pop_front();
															#line 2363 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<ReceiveFromActor*>(this)->SAV<int>::futures) { (void)(sz); this->~ReceiveFromActorState(); static_cast<ReceiveFromActor*>(this)->destroy(); return 0; }
															#line 8668 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<ReceiveFromActor*>(this)->SAV< int >::value()) int(sz);
		this->~ReceiveFromActorState();
		static_cast<ReceiveFromActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReceiveFromActor*>(this)->actor_wait_state > 0) static_cast<ReceiveFromActor*>(this)->actor_wait_state = 0;
		static_cast<ReceiveFromActor*>(this)->ActorCallback< ReceiveFromActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReceiveFromActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReceiveFromActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReceiveFromActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	UDPSimSocket* self;
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	uint8_t* begin;
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	uint8_t* end;
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	NetworkAddress* sender;
															#line 2352 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	TaskPriority currentTaskID;
															#line 8749 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via receiveFrom()
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class ReceiveFromActor final : public Actor<int>, public ActorCallback< ReceiveFromActor, 0, Void >, public ActorCallback< ReceiveFromActor, 1, Void >, public FastAllocated<ReceiveFromActor>, public ReceiveFromActorState<ReceiveFromActor> {
															#line 8754 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<ReceiveFromActor>::operator new;
	using FastAllocated<ReceiveFromActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReceiveFromActor, 0, Void >;
friend struct ActorCallback< ReceiveFromActor, 1, Void >;
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ReceiveFromActor(UDPSimSocket* const& self,uint8_t* const& begin,uint8_t* const& end,NetworkAddress* const& sender) 
															#line 8766 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<int>(),
		   ReceiveFromActorState<ReceiveFromActor>(self, begin, end, sender)
	{
		fdb_probe_actor_enter("receiveFrom", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("receiveFrom");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("receiveFrom", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReceiveFromActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReceiveFromActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<int> receiveFrom( UDPSimSocket* const& self, uint8_t* const& begin, uint8_t* const& end, NetworkAddress* const& sender ) {
															#line 2351 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<int>(new ReceiveFromActor(self, begin, end, sender));
															#line 8794 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 2365 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

public:
	UDPSimSocket(NetworkAddress const& localAddress, Optional<NetworkAddress> const& peerAddress)
	  : id(deterministicRandom()->randomUniqueID()), process(g_simulator.getCurrentProcess()), peerAddress(peerAddress),
	    actors(false), _localAddress(localAddress) {
		g_sim2.addressMap.emplace(_localAddress, process);
		ASSERT(process->boundUDPSockets.find(localAddress) == process->boundUDPSockets.end());
		process->boundUDPSockets.emplace(localAddress, this);
	}
	~UDPSimSocket() override {
		if (!closed.getFuture().isReady()) {
			close();
			closed.send(Void());
		}
		actors.clear(true);
	}
	void close() override {
		process->boundUDPSockets.erase(_localAddress);
		g_sim2.addressMap.erase(_localAddress);
	}
	UID getDebugID() const override { return id; }
	void addref() override { ReferenceCounted<UDPSimSocket>::addref(); }
	void delref() override { ReferenceCounted<UDPSimSocket>::delref(); }

	Future<int> send(uint8_t const* begin, uint8_t const* end) override {
		int sz = int(end - begin);
		auto res = fmap([sz](Void) { return sz; }, delay(0.0));
		ASSERT(sz <= IUDPSocket::MAX_PACKET_SIZE);
		ASSERT(peerAddress.present());
		if (!peerProcess.present()) {
			auto iter = g_sim2.addressMap.find(peerAddress.get());
			if (iter == g_sim2.addressMap.end()) {
				return res;
			}
			peerProcess = iter->second;
		}
		if (!peerSocket.present() || peerSocket.get()->isClosed()) {
			peerSocket.reset();
			auto iter = peerProcess.get()->boundUDPSockets.find(peerAddress.get());
			if (iter == peerProcess.get()->boundUDPSockets.end()) {
				return fmap([sz](Void) { return sz; }, delay(0.0));
			}
			peerSocket = iter->second.castTo<UDPSimSocket>();
			// the notation of leaking connections doesn't make much sense in the context of UDP
			// so we simply handle those in the simulator
			actors.add(cleanupPeerSocket(this));
		}
		if (randomDropPacket()) {
			return res;
		}
		actors.add(send(this, peerSocket.get(), begin, end));
		return res;
	}
	Future<int> sendTo(uint8_t const* begin, uint8_t const* end, NetworkAddress const& peer) override {
		int sz = int(end - begin);
		auto res = fmap([sz](Void) { return sz; }, delay(0.0));
		ASSERT(sz <= MAX_PACKET_SIZE);
		ISimulator::ProcessInfo* peerProcess = nullptr;
		Reference<UDPSimSocket> peerSocket;
		{
			auto iter = g_sim2.addressMap.find(peer);
			if (iter == g_sim2.addressMap.end()) {
				return res;
			}
			peerProcess = iter->second;
		}
		{
			auto iter = peerProcess->boundUDPSockets.find(peer);
			if (iter == peerProcess->boundUDPSockets.end()) {
				return res;
			}
			peerSocket = iter->second.castTo<UDPSimSocket>();
		}
		actors.add(send(this, peerSocket, begin, end));
		return res;
	}
	Future<int> receive(uint8_t* begin, uint8_t* end) override { return receiveFrom(begin, end, nullptr); }
	Future<int> receiveFrom(uint8_t* begin, uint8_t* end, NetworkAddress* sender) override {
		if (!recvBuffer.empty()) {
			auto buf = recvBuffer.front().second;
			if (sender) {
				*sender = recvBuffer.front().first;
			}
			int sz = buf->size();
			ASSERT(sz <= end - begin);
			std::copy(buf->begin(), buf->end(), begin);
			auto res = fmap([sz](Void) { return sz; }, delay(0.0));
			recvBuffer.pop_front();
			return res;
		}
		return receiveFrom(this, begin, end, sender);
	}
	void bind(NetworkAddress const& addr) override {
		g_sim2.addressMap.erase(_localAddress);
		process->boundUDPSockets.erase(_localAddress);
		process->boundUDPSockets.emplace(addr, Reference<UDPSimSocket>::addRef(this));
		_localAddress = addr;
		g_sim2.addressMap.emplace(_localAddress, process);
	}

	NetworkAddress localAddress() const override { return _localAddress; }

	boost::asio::ip::udp::socket::native_handle_type native_handle() override { return 0; }
};

Future<Reference<IUDPSocket>> Sim2::createUDPSocket(NetworkAddress toAddr) {
	NetworkAddress localAddress;
	auto process = g_simulator.getCurrentProcess();
	if (process->address.ip.isV6()) {
		IPAddress::IPAddressStore store = process->address.ip.toV6();
		uint16_t* ipParts = (uint16_t*)store.data();
		ipParts[7] += deterministicRandom()->randomInt(0, 256);
		localAddress.ip = IPAddress(store);
	} else {
		localAddress.ip = IPAddress(process->address.ip.toV4() + deterministicRandom()->randomInt(0, 256));
	}
	localAddress.port = deterministicRandom()->randomInt(40000, 60000);
	while (process->boundUDPSockets.find(localAddress) != process->boundUDPSockets.end()) {
		localAddress.port = deterministicRandom()->randomInt(40000, 60000);
	}
	return Reference<IUDPSocket>(new UDPSimSocket(localAddress, toAddr));
}

Future<Reference<IUDPSocket>> Sim2::createUDPSocket(bool isV6) {
	NetworkAddress localAddress;
	auto process = g_simulator.getCurrentProcess();
	if (process->address.ip.isV6() == isV6) {
		localAddress = process->address;
	} else {
		ASSERT(process->addresses.secondaryAddress.present() &&
		       process->addresses.secondaryAddress.get().isV6() == isV6);
		localAddress = process->addresses.secondaryAddress.get();
	}
	if (localAddress.ip.isV6()) {
		IPAddress::IPAddressStore store = localAddress.ip.toV6();
		uint16_t* ipParts = (uint16_t*)store.data();
		ipParts[7] += deterministicRandom()->randomInt(0, 256);
		localAddress.ip = IPAddress(store);
	} else {
		localAddress.ip = IPAddress(localAddress.ip.toV4() + deterministicRandom()->randomInt(0, 256));
	}
	localAddress.port = deterministicRandom()->randomInt(40000, 60000);
	return Reference<IUDPSocket>(new UDPSimSocket(localAddress, Optional<NetworkAddress>{}));
}

void startNewSimulator(bool printSimTime) {
	ASSERT(!g_network);
	g_network = g_pSimulator = new Sim2(printSimTime);
	g_simulator.connectionFailuresDisableDuration =
	    deterministicRandom()->coinflip() ? 0 : DISABLE_CONNECTION_FAILURE_FOREVER;
}

															#line 8950 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
namespace {
// This generated class is to be used only via doReboot()
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class DoRebootActor>
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class DoRebootActorState {
															#line 8957 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	DoRebootActorState(ISimulator::ProcessInfo* const& p,ISimulator::KillType const& kt) 
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : p(p),
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   kt(kt)
															#line 8966 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("doReboot", reinterpret_cast<unsigned long>(this));

	}
	~DoRebootActorState() 
	{
		fdb_probe_actor_destroy("doReboot", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2518 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent("RebootingProcessAttempt") .detail("ZoneId", p->locality.zoneId()) .detail("KillType", kt) .detail("Process", p->toString()) .detail("StartingClass", p->startingClass.toString()) .detail("Failed", p->failed) .detail("Excluded", p->excluded) .detail("Cleared", p->cleared) .detail("Rebooting", p->rebooting) .detail("TaskPriorityDefaultDelay", TaskPriority::DefaultDelay);
															#line 2529 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_sim2.delay(0, TaskPriority::DefaultDelay, p);
															#line 2529 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 8985 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			static_cast<DoRebootActor*>(this)->actor_wait_state = 1;
															#line 2529 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRebootActor, 0, Void >*>(static_cast<DoRebootActor*>(this)));
															#line 8989 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		delete static_cast<DoRebootActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 2532 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(kt == ISimulator::RebootProcess || kt == ISimulator::Reboot || kt == ISimulator::RebootAndDelete || kt == ISimulator::RebootProcessAndDelete);
															#line 2535 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST(kt == ISimulator::RebootProcess);
															#line 2536 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST(kt == ISimulator::Reboot);
															#line 2537 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST(kt == ISimulator::RebootAndDelete);
															#line 2538 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST( kt == ISimulator::RebootProcessAndDelete);
															#line 2542 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (p->rebooting || !p->isReliable())
															#line 9022 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2543 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "DoRebootFailed") .detail("Rebooting", p->rebooting) .detail("Reliable", p->isReliable());
															#line 2546 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				delete static_cast<DoRebootActor*>(this);
															#line 9028 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				return 0;
			}
			else
			{
															#line 2547 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (std::string(p->name) == "remote flow process")
															#line 9035 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				{
															#line 2548 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
					TraceEvent(SevDebug, "DoRebootFailed").detail("Name", p->name).detail("Address", p->address);
															#line 2549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
					delete static_cast<DoRebootActor*>(this);
															#line 9041 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
					return 0;
				}
				else
				{
															#line 2550 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
					if (p->getChilds().size())
															#line 9048 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
					{
															#line 2551 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
						TraceEvent(SevDebug, "DoRebootFailedOnParentProcess").detail("Address", p->address);
															#line 2552 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
						delete static_cast<DoRebootActor*>(this);
															#line 9054 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
						return 0;
					}
				}
			}
															#line 2555 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent("RebootingProcess") .detail("KillType", kt) .detail("Address", p->address) .detail("ZoneId", p->locality.zoneId()) .detail("DataHall", p->locality.dataHallId()) .detail("Locality", p->locality.toString()) .detail("Failed", p->failed) .detail("Excluded", p->excluded) .detail("Cleared", p->cleared) .backtrace();
															#line 2565 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			p->rebooting = true;
															#line 2566 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if ((kt == ISimulator::RebootAndDelete) || (kt == ISimulator::RebootProcessAndDelete))
															#line 9065 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2567 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				p->cleared = true;
															#line 2568 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				g_simulator.clearAddress(p->address);
															#line 9071 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 2570 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			p->shutdownSignal.send(kt);
															#line 9075 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1cont10(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 2532 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			ASSERT(kt == ISimulator::RebootProcess || kt == ISimulator::Reboot || kt == ISimulator::RebootAndDelete || kt == ISimulator::RebootProcessAndDelete);
															#line 2535 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST(kt == ISimulator::RebootProcess);
															#line 2536 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST(kt == ISimulator::Reboot);
															#line 2537 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST(kt == ISimulator::RebootAndDelete);
															#line 2538 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TEST( kt == ISimulator::RebootProcessAndDelete);
															#line 2542 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (p->rebooting || !p->isReliable())
															#line 9101 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2543 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "DoRebootFailed") .detail("Rebooting", p->rebooting) .detail("Reliable", p->isReliable());
															#line 2546 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				delete static_cast<DoRebootActor*>(this);
															#line 9107 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				return 0;
			}
			else
			{
															#line 2547 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				if (std::string(p->name) == "remote flow process")
															#line 9114 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
				{
															#line 2548 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
					TraceEvent(SevDebug, "DoRebootFailed").detail("Name", p->name).detail("Address", p->address);
															#line 2549 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
					delete static_cast<DoRebootActor*>(this);
															#line 9120 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
					return 0;
				}
				else
				{
															#line 2550 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
					if (p->getChilds().size())
															#line 9127 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
					{
															#line 2551 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
						TraceEvent(SevDebug, "DoRebootFailedOnParentProcess").detail("Address", p->address);
															#line 2552 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
						delete static_cast<DoRebootActor*>(this);
															#line 9133 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
						return 0;
					}
				}
			}
															#line 2555 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent("RebootingProcess") .detail("KillType", kt) .detail("Address", p->address) .detail("ZoneId", p->locality.zoneId()) .detail("DataHall", p->locality.dataHallId()) .detail("Locality", p->locality.toString()) .detail("Failed", p->failed) .detail("Excluded", p->excluded) .detail("Cleared", p->cleared) .backtrace();
															#line 2565 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			p->rebooting = true;
															#line 2566 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if ((kt == ISimulator::RebootAndDelete) || (kt == ISimulator::RebootProcessAndDelete))
															#line 9144 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2567 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				p->cleared = true;
															#line 2568 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
				g_simulator.clearAddress(p->address);
															#line 9150 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			}
															#line 2570 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			p->shutdownSignal.send(kt);
															#line 9154 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1cont10(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoRebootActor*>(this)->actor_wait_state > 0) static_cast<DoRebootActor*>(this)->actor_wait_state = 0;
		static_cast<DoRebootActor*>(this)->ActorCallback< DoRebootActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRebootActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doReboot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doReboot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoRebootActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doReboot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doReboot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoRebootActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doReboot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doReboot", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 2572 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevError, "RebootError").error(e);
															#line 2573 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			p->shutdownSignal.sendError(e);
															#line 2574 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 9243 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont11(int loopDepth) 
	{
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		delete static_cast<DoRebootActor*>(this);
															#line 9270 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ISimulator::ProcessInfo* p;
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	ISimulator::KillType kt;
															#line 9279 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via doReboot()
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class DoRebootActor final : public Actor<void>, public ActorCallback< DoRebootActor, 0, Void >, public FastAllocated<DoRebootActor>, public DoRebootActorState<DoRebootActor> {
															#line 9284 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<DoRebootActor>::operator new;
	using FastAllocated<DoRebootActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoRebootActor, 0, Void >;
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	DoRebootActor(ISimulator::ProcessInfo* const& p,ISimulator::KillType const& kt) 
															#line 9295 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<void>(),
		   DoRebootActorState<DoRebootActor>(p, kt)
	{
		fdb_probe_actor_enter("doReboot", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doReboot");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doReboot", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
void doReboot( ISimulator::ProcessInfo* const& p, ISimulator::KillType const& kt ) {
															#line 2517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	new DoRebootActor(p, kt);
															#line 9314 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 2577 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

// Simulates delays for performing operations on disk
Future<Void> waitUntilDiskReady(Reference<DiskParameters> diskParameters, int64_t size, bool sync) {
	if (g_simulator.getCurrentProcess()->failedDisk) {
		return Never();
	}
	if (g_simulator.connectionFailuresDisableDuration > 1e4)
		return delay(0.0001);

	if (diskParameters->nextOperation < now())
		diskParameters->nextOperation = now();
	diskParameters->nextOperation += (1.0 / diskParameters->iops) + (1.0 * size / diskParameters->bandwidth);

	double randomLatency;
	if (sync) {
		randomLatency = .005 + deterministicRandom()->random01() * (BUGGIFY ? 1.0 : .010);
	} else
		randomLatency = 10 * deterministicRandom()->random01() / diskParameters->iops;

	return delayUntil(diskParameters->nextOperation + randomLatency);
}

void enableConnectionFailures(std::string const& context) {
	if (g_network->isSimulated()) {
		g_simulator.connectionFailuresDisableDuration = 0;
		g_simulator.speedUpSimulation = false;
		TraceEvent(SevWarnAlways, ("EnableConnectionFailures_" + context).c_str());
	}
}

void disableConnectionFailures(std::string const& context) {
	if (g_network->isSimulated()) {
		g_simulator.connectionFailuresDisableDuration = DISABLE_CONNECTION_FAILURE_FOREVER;
		g_simulator.speedUpSimulation = true;
		TraceEvent(SevWarnAlways, ("DisableConnectionFailures_" + context).c_str());
	}
}

#if defined(_WIN32)

/* Opening with FILE_SHARE_DELETE lets simulation actually work on windows - previously renames were always failing.
   FIXME: Use an actual platform abstraction for this stuff!  Is there any reason we can't use underlying net2 for
   example? */

#include <Windows.h>

int sf_open(const char* filename, int flags, int convFlags, int mode) {
	HANDLE wh = CreateFile(filename,
	                       GENERIC_READ | ((flags & IAsyncFile::OPEN_READWRITE) ? GENERIC_WRITE : 0),
	                       FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
	                       nullptr,
	                       (flags & IAsyncFile::OPEN_EXCLUSIVE) ? CREATE_NEW
	                       : (flags & IAsyncFile::OPEN_CREATE)  ? OPEN_ALWAYS
	                                                            : OPEN_EXISTING,
	                       FILE_ATTRIBUTE_NORMAL,
	                       nullptr);
	int h = -1;
	if (wh != INVALID_HANDLE_VALUE)
		h = _open_osfhandle((intptr_t)wh, convFlags);
	else
		errno = GetLastError() == ERROR_FILE_NOT_FOUND ? ENOENT : EFAULT;
	return h;
}

#endif

// Opens a file for asynchronous I/O
Future<Reference<class IAsyncFile>> Sim2FileSystem::open(const std::string& filename, int64_t flags, int64_t mode) {
	ASSERT((flags & IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE) || !(flags & IAsyncFile::OPEN_CREATE) ||
	       StringRef(filename).endsWith(
	           LiteralStringRef(".fdb-lock"))); // We don't use "ordinary" non-atomic file creation right now except for
	                                            // folder locking, and we don't have code to simulate its unsafeness.

	if ((flags & IAsyncFile::OPEN_EXCLUSIVE))
		ASSERT(flags & IAsyncFile::OPEN_CREATE);

	if (flags & IAsyncFile::OPEN_UNCACHED) {
		auto& machineCache = g_simulator.getCurrentProcess()->machine->openFiles;
		std::string actualFilename = filename;
		if (flags & IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE) {
			actualFilename = filename + ".part";
			auto partFile = machineCache.find(actualFilename);
			if (partFile != machineCache.end()) {
				Future<Reference<IAsyncFile>> f = AsyncFileDetachable::open(partFile->second.get());
				if (FLOW_KNOBS->PAGE_WRITE_CHECKSUM_HISTORY > 0)
					f = map(f, [=](Reference<IAsyncFile> r) {
						return Reference<IAsyncFile>(new AsyncFileWriteChecker(r));
					});
				return f;
			}
		}

		Future<Reference<IAsyncFile>> f;
		auto itr = machineCache.find(actualFilename);
		if (itr == machineCache.end()) {
			// Simulated disk parameters are shared by the AsyncFileNonDurable and the underlying SimpleFile.
			// This way, they can both keep up with the time to start the next operation
			auto diskParameters =
			    makeReference<DiskParameters>(FLOW_KNOBS->SIM_DISK_IOPS, FLOW_KNOBS->SIM_DISK_BANDWIDTH);
			f = AsyncFileNonDurable::open(filename,
			                              actualFilename,
			                              SimpleFile::open(filename, flags, mode, diskParameters, false),
			                              diskParameters,
			                              (flags & IAsyncFile::OPEN_NO_AIO) == 0);

			machineCache[actualFilename] = UnsafeWeakFutureReference<IAsyncFile>(f);
		} else {
			f = itr->second.get();
		}

		f = AsyncFileDetachable::open(f);
		if (FLOW_KNOBS->PAGE_WRITE_CHECKSUM_HISTORY > 0)
			f = map(f, [=](Reference<IAsyncFile> r) { return Reference<IAsyncFile>(new AsyncFileWriteChecker(r)); });
		if (FLOW_KNOBS->ENABLE_CHAOS_FEATURES)
			f = map(f, [=](Reference<IAsyncFile> r) { return Reference<IAsyncFile>(new AsyncFileChaos(r)); });
#if ENCRYPTION_ENABLED
		if (flags & IAsyncFile::OPEN_ENCRYPTED)
			f = map(f, [flags](Reference<IAsyncFile> r) {
				auto mode = flags & IAsyncFile::OPEN_READWRITE ? AsyncFileEncrypted::Mode::APPEND_ONLY
				                                               : AsyncFileEncrypted::Mode::READ_ONLY;
				return Reference<IAsyncFile>(new AsyncFileEncrypted(r, mode));
			});
#endif // ENCRYPTION_ENABLED
		return f;
	} else
		return AsyncFileCached::open(filename, flags, mode);
}

// Deletes the given file.  If mustBeDurable, returns only when the file is guaranteed to be deleted even after a power
// failure.
Future<Void> Sim2FileSystem::deleteFile(const std::string& filename, bool mustBeDurable) {
	return Sim2::deleteFileImpl(&g_sim2, filename, mustBeDurable);
}

															#line 9452 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
namespace {
// This generated class is to be used only via renameFileImpl()
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
template <class RenameFileImplActor>
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class RenameFileImplActorState {
															#line 9459 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	RenameFileImplActorState(std::string const& from,std::string const& to) 
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		 : from(from),
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		   to(to)
															#line 9468 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("renameFileImpl", reinterpret_cast<unsigned long>(this));

	}
	~RenameFileImplActorState() 
	{
		fdb_probe_actor_destroy("renameFileImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2712 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0.5 * deterministicRandom()->random01());
															#line 2712 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			if (static_cast<RenameFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9485 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RenameFileImplActor*>(this)->actor_wait_state = 1;
															#line 2712 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RenameFileImplActor, 0, Void >*>(static_cast<RenameFileImplActor*>(this)));
															#line 9490 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~RenameFileImplActorState();
		static_cast<RenameFileImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2713 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		::renameFile(from, to);
															#line 2714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0.5 * deterministicRandom()->random01());
															#line 2714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<RenameFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9517 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RenameFileImplActor*>(this)->actor_wait_state = 2;
															#line 2714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameFileImplActor, 1, Void >*>(static_cast<RenameFileImplActor*>(this)));
															#line 9522 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2713 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		::renameFile(from, to);
															#line 2714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0.5 * deterministicRandom()->random01());
															#line 2714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (static_cast<RenameFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9535 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RenameFileImplActor*>(this)->actor_wait_state = 2;
															#line 2714 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameFileImplActor, 1, Void >*>(static_cast<RenameFileImplActor*>(this)));
															#line 9540 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RenameFileImplActor*>(this)->actor_wait_state > 0) static_cast<RenameFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<RenameFileImplActor*>(this)->ActorCallback< RenameFileImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameFileImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RenameFileImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RenameFileImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 2715 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<RenameFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameFileImplActorState(); static_cast<RenameFileImplActor*>(this)->destroy(); return 0; }
															#line 9612 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<RenameFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameFileImplActorState();
		static_cast<RenameFileImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 2715 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
		if (!static_cast<RenameFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameFileImplActorState(); static_cast<RenameFileImplActor*>(this)->destroy(); return 0; }
															#line 9624 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<RenameFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameFileImplActorState();
		static_cast<RenameFileImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RenameFileImplActor*>(this)->actor_wait_state > 0) static_cast<RenameFileImplActor*>(this)->actor_wait_state = 0;
		static_cast<RenameFileImplActor*>(this)->ActorCallback< RenameFileImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameFileImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RenameFileImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RenameFileImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	std::string from;
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	std::string to;
															#line 9699 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via renameFileImpl()
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
class RenameFileImplActor final : public Actor<Void>, public ActorCallback< RenameFileImplActor, 0, Void >, public ActorCallback< RenameFileImplActor, 1, Void >, public FastAllocated<RenameFileImplActor>, public RenameFileImplActorState<RenameFileImplActor> {
															#line 9704 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<RenameFileImplActor>::operator new;
	using FastAllocated<RenameFileImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RenameFileImplActor, 0, Void >;
friend struct ActorCallback< RenameFileImplActor, 1, Void >;
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	RenameFileImplActor(std::string const& from,std::string const& to) 
															#line 9716 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   RenameFileImplActorState<RenameFileImplActor>(from, to)
	{
		fdb_probe_actor_enter("renameFileImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("renameFileImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("renameFileImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RenameFileImplActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RenameFileImplActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
[[nodiscard]] Future<Void> renameFileImpl( std::string const& from, std::string const& to ) {
															#line 2711 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new RenameFileImplActor(from, to));
															#line 9745 "/usr/src/libfdb_c/fdbrpc/sim2.actor.g.cpp"
}

#line 2717 "/usr/src/libfdb_c/fdbrpc/sim2.actor.cpp"

Future<Void> Sim2FileSystem::renameFile(std::string const& from, std::string const& to) {
	return renameFileImpl(from, to);
}

Future<std::time_t> Sim2FileSystem::lastWriteTime(const std::string& filename) {
	// TODO: update this map upon file writes.
	static std::map<std::string, double> fileWrites;
	if (BUGGIFY && deterministicRandom()->random01() < 0.01) {
		fileWrites[filename] = now();
	}
	return fileWrites[filename];
}

#ifdef ENABLE_SAMPLING
ActorLineageSet& Sim2FileSystem::getActorLineageSet() {
	return actorLineageSet;
}
#endif

void Sim2FileSystem::newFileSystem() {
	g_network->setGlobal(INetwork::enFileSystem, (flowGlobalType) new Sim2FileSystem());
}
