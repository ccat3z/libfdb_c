#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
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

#include "flow/MkCert.h"
#include "fmt/format.h"
#include "fdbrpc/simulator.h"
#include "flow/Arena.h"
#ifndef BOOST_SYSTEM_NO_LIB
#define BOOST_SYSTEM_NO_LIB
#endif
#ifndef BOOST_DATE_TIME_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#endif
#ifndef BOOST_REGEX_NO_LIB
#define BOOST_REGEX_NO_LIB
#endif
#include "fdbrpc/SimExternalConnection.h"
#include "flow/ActorCollection.h"
#include "flow/IRandom.h"
#include "flow/IThreadPool.h"
#include "flow/ProtocolVersion.h"
#include "flow/Util.h"
#include "flow/WriteOnlySet.h"
#include "flow/IAsyncFile.h"
#include "fdbrpc/AsyncFileCached.actor.h"
#include "fdbrpc/AsyncFileEncrypted.h"
#include "fdbrpc/SimulatorProcessInfo.h"
#include "fdbrpc/AsyncFileNonDurable.actor.h"
#include "fdbrpc/AsyncFileChaos.h"
#include "crc32/crc32c.h"
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
#include "flow/TaskQueue.h"
#include "flow/IUDPSocket.h"
#include "flow/IConnection.h"
#include "flow/actorcompiler.h" // This must be the last #include.

ISimulator* g_simulator = nullptr;
thread_local ISimulator::ProcessInfo* ISimulator::currentProcess = nullptr;

ISimulator::ISimulator()
  : desiredCoordinators(1), physicalDatacenters(1), processesPerMachine(0), listenersPerProcess(1), usableRegions(1),
    allowLogSetKills(true), tssMode(TSSMode::Disabled), configDBType(ConfigDBType::DISABLED), isStopped(false),
    lastConnectionFailure(0), connectionFailuresDisableDuration(0), speedUpSimulation(false),
    connectionFailureEnableTime(0), disableTLogRecoveryFinish(false), backupAgents(BackupAgentType::WaitForType),
    drAgents(BackupAgentType::WaitForType), allSwapsDisabled(false), blobGranulesEnabled(false) {}
ISimulator::~ISimulator() = default;

bool simulator_should_inject_fault(const char* context, const char* file, int line, int error_code) {
	if (!g_network->isSimulated() || !faultInjectionActivated)
		return false;

	auto p = g_simulator->getCurrentProcess();

	if (p->fault_injection_p2 && deterministicRandom()->random01() < p->fault_injection_p2 &&
	    !g_simulator->speedUpSimulation) {
		uint32_t h1 = line + (p->fault_injection_r >> 32);

		if (h1 < p->fault_injection_p1 * std::numeric_limits<uint32_t>::max()) {
			CODE_PROBE(true, "A fault was injected", probe::assert::simOnly, probe::context::sim2);
			CODE_PROBE(error_code == error_code_io_timeout,
			           "An io timeout was injected",
			           probe::assert::simOnly,
			           probe::context::sim2);
			CODE_PROBE(error_code == error_code_io_error,
			           "An io error was injected",
			           probe::assert::simOnly,
			           probe::context::sim2);
			CODE_PROBE(error_code == error_code_platform_error,
			           "A platform error was injected.",
			           probe::assert::simOnly,
			           probe::context::sim2);
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

bool simulator_should_inject_blob_fault(const char* context, const char* file, int line, int error_code) {
	if (!g_network->isSimulated() || !faultInjectionActivated)
		return false;

	auto p = g_simulator->getCurrentProcess();

	if (!g_simulator->speedUpSimulation && deterministicRandom()->random01() < p->blob_inject_failure_rate) {
		CODE_PROBE(true, "A blob fault was injected", probe::assert::simOnly, probe::context::sim2);
		CODE_PROBE(error_code == error_code_http_request_failed,
		           "A failed http request was injected",
		           probe::assert::simOnly,
		           probe::context::sim2);
		TraceEvent("BlobFaultInjected")
		    .detail("Context", context)
		    .detail("File", file)
		    .detail("Line", line)
		    .detail("ErrorCode", error_code);
		return true;
	}

	return false;
}

void ISimulator::disableFor(const std::string& desc, double time) {
	disabledMap[desc] = time;
}

double ISimulator::checkDisabled(const std::string& desc) const {
	auto iter = disabledMap.find(desc);
	if (iter != disabledMap.end()) {
		return iter->second;
	}
	return 0;
}

bool ISimulator::checkInjectedCorruption() {
	auto iter = corruptWorkerMap.find(currentProcess->address);
	if (iter != corruptWorkerMap.end())
		return iter->second;
	return false;
}

flowGlobalType ISimulator::global(int id) const {
	return getCurrentProcess()->global(id);
};

void ISimulator::setGlobal(size_t id, flowGlobalType v) {
	getCurrentProcess()->setGlobal(id, v);
};

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
			       processInfo->name.c_str(),
			       processInfo->startingClass.toString().c_str(),
			       (processInfo->isExcluded() ? "True" : "False"),
			       (processInfo->failed ? "True" : "False"),
			       (processInfo->rebooting ? "True" : "False"),
			       (processInfo->isCleared() ? "True" : "False"),
			       getRoles(processInfo->address).c_str(),
			       processInfo->dataFolder.c_str());
		}
	}

	return;
}

WipedString ISimulator::makeToken(int64_t tenantId, uint64_t ttlSecondsFromNow) {
	ASSERT_GT(authKeys.size(), 0);
	auto tokenSpec = authz::jwt::TokenRef{};
	auto [keyName, key] = *authKeys.begin();
	tokenSpec.algorithm = key.algorithm() == PKeyAlgorithm::EC ? authz::Algorithm::ES256 : authz::Algorithm::RS256;
	tokenSpec.keyId = keyName;
	tokenSpec.issuer = "sim2_issuer"_sr;
	tokenSpec.subject = "sim2_testing"_sr;
	auto const now = static_cast<uint64_t>(g_network->timer());
	tokenSpec.notBeforeUnixTime = now - 1;
	tokenSpec.issuedAtUnixTime = now;
	tokenSpec.expiresAtUnixTime = now + ttlSecondsFromNow;
	auto const tokenId = deterministicRandom()->randomAlphaNumeric(10);
	tokenSpec.tokenId = StringRef(tokenId);
	tokenSpec.tenants = VectorRef<int64_t>(&tenantId, 1);
	Arena arena;
	return WipedString(authz::jwt::signToken(arena, tokenSpec, key));
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
		if (!g_simulator->speedUpSimulation && !stableConnection)
			t += clogPairLatency[pair];

		if (!g_simulator->speedUpSimulation && !stableConnection && clogPairUntil.count(pair))
			t = std::max(t, clogPairUntil[pair]);

		auto p = std::make_pair(from, to);
		if (!g_simulator->speedUpSimulation && !stableConnection && clogProcessPairUntil.count(p))
			t = std::max(t, clogProcessPairUntil[p]);

		if (!g_simulator->speedUpSimulation && !stableConnection && clogRecvUntil.count(to.ip))
			t = std::max(t, clogRecvUntil[to.ip]);

		return t - tnow;
	}

	bool disconnected(const IPAddress& from, const IPAddress& to) {
		auto pair = std::make_pair(from, to);
		if (g_simulator->speedUpSimulation || disconnectPairUntil.find(pair) == disconnectPairUntil.end()) {
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
		if (a <= pFast || g_simulator->speedUpSimulation) {
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
	  : opened(false), closedByCaller(false), stableConnection(false), trustedPeer(true), process(process),
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

	bool hasTrustedPeer() const override { return trustedPeer; }

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

	bool opened, closedByCaller, stableConnection, trustedPeer;

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

																#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via sender()
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class SenderActor>
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class SenderActorState {
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SenderActorState(Sim2Conn* const& self) 
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			;
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->writtenBytes.onChange();
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<SenderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<SenderActor*>(this)->actor_wait_state = 1;
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SenderActor, 0, Void >*>(static_cast<SenderActor*>(this)));
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->peerProcess);
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(.002 * deterministicRandom()->random01());
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<SenderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SenderActor*>(this)->actor_wait_state = 2;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SenderActor, 1, Void >*>(static_cast<SenderActor*>(this)));
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->peerProcess);
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(.002 * deterministicRandom()->random01());
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<SenderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SenderActor*>(this)->actor_wait_state = 2;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SenderActor, 1, Void >*>(static_cast<SenderActor*>(this)));
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->sentBytes.set(self->writtenBytes.get());
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->sentBytes.set(self->writtenBytes.get());
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via sender()
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class SenderActor final : public Actor<Void>, public ActorCallback< SenderActor, 0, Void >, public ActorCallback< SenderActor, 1, Void >, public FastAllocated<SenderActor>, public SenderActorState<SenderActor> {
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<SenderActor>::operator new;
	using FastAllocated<SenderActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SenderActor, 0, Void >;
friend struct ActorCallback< SenderActor, 1, Void >;
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SenderActor(Sim2Conn* const& self) 
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> sender( Sim2Conn* const& self ) {
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new SenderActor(self));
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
																#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via receiver()
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class ReceiverActor>
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class ReceiverActorState {
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ReceiverActorState(Sim2Conn* const& self) 
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			;
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->sentBytes.get() != self->receivedBytes.get())
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onProcess(self->peerProcess);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReceiverActor*>(this)->actor_wait_state = 1;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 0, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		;
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->peerProcess);
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (g_clogging.disconnected(self->peerProcess->address.ip, self->process->address.ip))
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent("SimulatedDisconnection") .detail("Phase", "DataTransfer") .detail("Sender", self->peerProcess->address) .detail("Receiver", self->process->address);
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 1));
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		pos = deterministicRandom()->random01() < .5 ? self->sentBytes.get() : deterministicRandom()->randomInt64(self->receivedBytes.get(), self->sentBytes.get() + 1);
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(g_clogging.getSendDelay( self->peerProcess->address, self->process->address, self->isStableConnection()));
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 3;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 2, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!(self->sentBytes.get() == self->receivedBytes.get()))
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->sentBytes.onChange();
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 2;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 1, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(self->process);
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 4;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 3, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(self->process);
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 4;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 3, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(g_clogging.getRecvDelay( self->peerProcess->address, self->process->address, self->isStableConnection()));
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6when1(__when_expr_4.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 5;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 4, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(g_clogging.getRecvDelay( self->peerProcess->address, self->process->address, self->isStableConnection()));
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6when1(__when_expr_4.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 5;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 4, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->stopReceive.isReady())
															#line 1305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_5 = Future<Void>(Never());
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_5.get(), loopDepth); };
			static_cast<ReceiverActor*>(this)->actor_wait_state = 6;
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 5, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->stopReceive.isReady())
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_5 = Future<Void>(Never());
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_5.get(), loopDepth); };
			static_cast<ReceiverActor*>(this)->actor_wait_state = 6;
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 5, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->receivedBytes.set(pos);
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_6 = Future<Void>(Void());
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont8when1(__when_expr_6.get(), loopDepth); };
		static_cast<ReceiverActor*>(this)->actor_wait_state = 7;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ReceiverActor, 6, Void >*>(static_cast<ReceiverActor*>(this)));
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int64_t pos;
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via receiver()
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class ReceiverActor final : public Actor<Void>, public ActorCallback< ReceiverActor, 0, Void >, public ActorCallback< ReceiverActor, 1, Void >, public ActorCallback< ReceiverActor, 2, Void >, public ActorCallback< ReceiverActor, 3, Void >, public ActorCallback< ReceiverActor, 4, Void >, public ActorCallback< ReceiverActor, 5, Void >, public ActorCallback< ReceiverActor, 6, Void >, public FastAllocated<ReceiverActor>, public ReceiverActorState<ReceiverActor> {
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ReceiverActor(Sim2Conn* const& self) 
															#line 1616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> receiver( Sim2Conn* const& self ) {
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new ReceiverActor(self));
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
																#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via whenReadable()
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class WhenReadableActor>
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class WhenReadableActorState {
															#line 1659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	WhenReadableActorState(Sim2Conn* const& self) 
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 1666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				;
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->readBytes.get() != self->receivedBytes.get())
															#line 1735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (!static_cast<WhenReadableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WhenReadableActorState(); static_cast<WhenReadableActor*>(this)->destroy(); return 0; }
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			new (&static_cast<WhenReadableActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WhenReadableActorState();
			static_cast<WhenReadableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->receivedBytes.onChange();
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<WhenReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WhenReadableActor*>(this)->actor_wait_state = 1;
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WhenReadableActor, 0, Void >*>(static_cast<WhenReadableActor*>(this)));
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->rollRandomClose();
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->rollRandomClose();
															#line 1774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via whenReadable()
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class WhenReadableActor final : public Actor<Void>, public ActorCallback< WhenReadableActor, 0, Void >, public FastAllocated<WhenReadableActor>, public WhenReadableActorState<WhenReadableActor> {
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<WhenReadableActor>::operator new;
	using FastAllocated<WhenReadableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WhenReadableActor, 0, Void >;
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	WhenReadableActor(Sim2Conn* const& self) 
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> whenReadable( Sim2Conn* const& self ) {
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new WhenReadableActor(self));
															#line 1887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
																#line 1891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via whenWritable()
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class WhenWritableActor>
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class WhenWritableActorState {
															#line 1897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	WhenWritableActorState(Sim2Conn* const& self) 
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				;
															#line 1920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!self->peer)
															#line 1973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (!static_cast<WhenWritableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WhenWritableActorState(); static_cast<WhenWritableActor*>(this)->destroy(); return 0; }
															#line 1977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			new (&static_cast<WhenWritableActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WhenWritableActorState();
			static_cast<WhenWritableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->peer->availableSendBufferForPeer() > 0)
															#line 1985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(g_simulator->getCurrentProcess() == self->process);
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (!static_cast<WhenWritableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WhenWritableActorState(); static_cast<WhenWritableActor*>(this)->destroy(); return 0; }
															#line 1991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			new (&static_cast<WhenWritableActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WhenWritableActorState();
			static_cast<WhenWritableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->peer->receivedBytes.onChange();
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<WhenWritableActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WhenWritableActor*>(this)->actor_wait_state = 1;
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WhenWritableActor, 0, Void >*>(static_cast<WhenWritableActor*>(this)));
															#line 2007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_simulator->onProcess(self->process);
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<WhenWritableActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WhenWritableActor*>(this)->actor_wait_state = 2;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WhenWritableActor, 1, Void >*>(static_cast<WhenWritableActor*>(this)));
															#line 2029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (e.code() != error_code_broken_promise)
															#line 2039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				return a_body1Catch2(e, std::max(0, loopDepth - 1));
															#line 2043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->peerProcess);
															#line 2059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(g_simulator->getCurrentProcess() == self->peerProcess);
															#line 2068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 2226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via whenWritable()
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class WhenWritableActor final : public Actor<Void>, public ActorCallback< WhenWritableActor, 0, Void >, public ActorCallback< WhenWritableActor, 1, Void >, public FastAllocated<WhenWritableActor>, public WhenWritableActorState<WhenWritableActor> {
															#line 2231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<WhenWritableActor>::operator new;
	using FastAllocated<WhenWritableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WhenWritableActor, 0, Void >;
friend struct ActorCallback< WhenWritableActor, 1, Void >;
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	WhenWritableActor(Sim2Conn* const& self) 
															#line 2243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> whenWritable( Sim2Conn* const& self ) {
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new WhenWritableActor(self));
															#line 2271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

	void rollRandomClose() {
		// make sure connections between parenta and their childs are not closed
		if (!stableConnection &&
		    now() - g_simulator->lastConnectionFailure > g_simulator->connectionFailuresDisableDuration &&
		    deterministicRandom()->random01() < .00001) {
			g_simulator->lastConnectionFailure = now();
			double a = deterministicRandom()->random01(), b = deterministicRandom()->random01();
			CODE_PROBE(true, "Simulated connection failure", probe::context::sim2, probe::assert::simOnly);
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

																#line 2302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via trackLeakedConnection()
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class TrackLeakedConnectionActor>
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class TrackLeakedConnectionActorState {
															#line 2308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	TrackLeakedConnectionActorState(Sim2Conn* const& self) 
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 2315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (g_simulator->httpServerIps.count(self->process->address.ip))
															#line 2330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (!static_cast<TrackLeakedConnectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TrackLeakedConnectionActorState(); static_cast<TrackLeakedConnectionActor*>(this)->destroy(); return 0; }
															#line 2334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				new (&static_cast<TrackLeakedConnectionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~TrackLeakedConnectionActorState();
				static_cast<TrackLeakedConnectionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onProcess(self->process);
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 1;
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 0, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->process->address.isPublic())
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5 + FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME * 2.1 + FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 2;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 1, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5);
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 3;
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 2, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->process->address.isPublic())
															#line 2407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5 + FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME * 2.1 + FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 2;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 1, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * 1.5);
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<TrackLeakedConnectionActor*>(this)->actor_wait_state = 3;
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TrackLeakedConnectionActor, 2, Void >*>(static_cast<TrackLeakedConnectionActor*>(this)));
															#line 2432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
	int a_body1cont3(int loopDepth) 
	{
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		TraceEvent(SevError, "LeakedConnection", self->dbgid) .error(connection_leaked()) .detail("MyAddr", self->process->address) .detail("IsPublic", self->process->address.isPublic()) .detail("PeerAddr", self->peerEndpoint) .detail("PeerId", self->peerId) .detail("Opened", self->opened);
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<TrackLeakedConnectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TrackLeakedConnectionActorState(); static_cast<TrackLeakedConnectionActor*>(this)->destroy(); return 0; }
															#line 2507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<TrackLeakedConnectionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TrackLeakedConnectionActorState();
		static_cast<TrackLeakedConnectionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

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
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

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
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2Conn* self;
															#line 2667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via trackLeakedConnection()
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class TrackLeakedConnectionActor final : public Actor<Void>, public ActorCallback< TrackLeakedConnectionActor, 0, Void >, public ActorCallback< TrackLeakedConnectionActor, 1, Void >, public ActorCallback< TrackLeakedConnectionActor, 2, Void >, public FastAllocated<TrackLeakedConnectionActor>, public TrackLeakedConnectionActorState<TrackLeakedConnectionActor> {
															#line 2672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	TrackLeakedConnectionActor(Sim2Conn* const& self) 
															#line 2685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> trackLeakedConnection( Sim2Conn* const& self ) {
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new TrackLeakedConnectionActor(self));
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
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

																#line 2753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via open()
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class OpenActor>
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class OpenActorState {
															#line 2759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	OpenActorState(std::string const& filename,int const& flags,int const& mode,Reference<DiskParameters> const& diskParameters = makeReference<DiskParameters>(25000, 150000000),bool const& delayOnWrite = true) 
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : filename(filename),
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   flags(flags),
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   mode(mode),
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   diskParameters(diskParameters),
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   delayOnWrite(delayOnWrite),
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   currentTaskID(g_network->getCurrentTask())
															#line 2778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (++openCount >= 6000)
															#line 2793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevError, "TooManyFiles").log();
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				ASSERT(false);
															#line 2799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (openCount == 4000)
															#line 2803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				disableConnectionFailures("TooManyFiles");
															#line 2807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(basename(filename).size() < 250);
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 1;
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 0, Void >*>(static_cast<OpenActor*>(this)));
															#line 2820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->MIN_OPEN_TIME + deterministicRandom()->random01() * (FLOW_KNOBS->MAX_OPEN_TIME - FLOW_KNOBS->MIN_OPEN_TIME));
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 2;
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 1, Void >*>(static_cast<OpenActor*>(this)));
															#line 2851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->MIN_OPEN_TIME + deterministicRandom()->random01() * (FLOW_KNOBS->MAX_OPEN_TIME - FLOW_KNOBS->MIN_OPEN_TIME));
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 2;
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 1, Void >*>(static_cast<OpenActor*>(this)));
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			err = e;
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 4;
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 3, Void >*>(static_cast<OpenActor*>(this)));
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		std::string open_filename = filename;
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 2979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT((flags & OPEN_CREATE) && (flags & OPEN_READWRITE) && !(flags & OPEN_EXCLUSIVE));
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			open_filename = filename + ".part";
															#line 2985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		int h = sf_open(open_filename.c_str(), flags, flagConversion(flags), mode);
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (h == -1)
															#line 2991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			bool notFound = errno == ENOENT;
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			Error e = notFound ? file_not_found() : io_error();
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(notFound ? SevWarn : SevWarnAlways, "FileOpenError") .error(e) .GetLastError() .detail("File", filename) .detail("Flags", flags);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1cont1Catch1(e, loopDepth);
															#line 3001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		platform::makeTemporary(open_filename.c_str());
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		SimpleFile* simpleFile = new SimpleFile(h, diskParameters, delayOnWrite, filename, open_filename, flags);
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		file = Reference<IAsyncFile>(simpleFile);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_2 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<OpenActor*>(this)->actor_wait_state = 3;
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 2, Void >*>(static_cast<OpenActor*>(this)));
															#line 3018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		std::string open_filename = filename;
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 3029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT((flags & OPEN_CREATE) && (flags & OPEN_READWRITE) && !(flags & OPEN_EXCLUSIVE));
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			open_filename = filename + ".part";
															#line 3035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		int h = sf_open(open_filename.c_str(), flags, flagConversion(flags), mode);
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (h == -1)
															#line 3041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			bool notFound = errno == ENOENT;
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			Error e = notFound ? file_not_found() : io_error();
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(notFound ? SevWarn : SevWarnAlways, "FileOpenError") .error(e) .GetLastError() .detail("File", filename) .detail("Flags", flags);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1cont1Catch1(e, loopDepth);
															#line 3051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		platform::makeTemporary(open_filename.c_str());
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		SimpleFile* simpleFile = new SimpleFile(h, diskParameters, delayOnWrite, filename, open_filename, flags);
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		file = Reference<IAsyncFile>(simpleFile);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_2 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<OpenActor*>(this)->actor_wait_state = 3;
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 2, Void >*>(static_cast<OpenActor*>(this)));
															#line 3068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(file); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 3140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(file)); // state_var_RVO
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(file); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 3152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 3227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 3235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"

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
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::string filename;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int flags;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int mode;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<DiskParameters> diskParameters;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	bool delayOnWrite;
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	TaskPriority currentTaskID;
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<IAsyncFile> file;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Error err;
															#line 3320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via open()
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class OpenActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< OpenActor, 0, Void >, public ActorCallback< OpenActor, 1, Void >, public ActorCallback< OpenActor, 2, Void >, public ActorCallback< OpenActor, 3, Void >, public FastAllocated<OpenActor>, public OpenActorState<OpenActor> {
															#line 3325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	OpenActor(std::string const& filename,int const& flags,int const& mode,Reference<DiskParameters> const& diskParameters = makeReference<DiskParameters>(25000, 150000000),bool const& delayOnWrite = true) 
															#line 3339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( std::string const& filename, int const& flags, int const& mode, Reference<DiskParameters> const& diskParameters = makeReference<DiskParameters>(25000, 150000000), bool const& delayOnWrite = true ) {
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IAsyncFile>>(new OpenActor(filename, flags, mode, diskParameters, delayOnWrite));
															#line 3369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

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

																#line 3437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via read_impl()
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class Read_implActor>
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Read_implActorState {
															#line 3443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Read_implActorState(SimpleFile* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   data(data),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   length(length),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   offset(offset)
															#line 3456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT((self->flags & IAsyncFile::OPEN_NO_AIO) != 0 || ((uintptr_t)data % 4096 == 0 && length % 4096 == 0 && offset % 4096 == 0));
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			opId = deterministicRandom()->randomUniqueID();
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 3475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				fmt::print(randLog, "SFR1 {0} {1} {2} {3} {4}\n", self->dbgId.shortString(), self->filename, opId.shortString(), length, offset);
															#line 3479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, length);
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<Read_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Read_implActor*>(this)->actor_wait_state = 1;
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_implActor, 0, Void >*>(static_cast<Read_implActor*>(this)));
															#line 3490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (_lseeki64(self->h, offset, SEEK_SET) == -1)
															#line 3513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 1);
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		unsigned int read_bytes = 0;
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if ((read_bytes = _read(self->h, data, (unsigned int)length)) == -1)
															#line 3525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 2);
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			uint32_t a = crc32c_append(0, (const uint8_t*)data, read_bytes);
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFR2 %s %s %s %d %d\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str(), read_bytes, a);
															#line 3541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		debugFileCheck("SimpleFileRead", self->filename, data, offset, length);
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::read");
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::read");
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Read_implActor*>(this)->SAV<int>::futures) { (void)(read_bytes); this->~Read_implActorState(); static_cast<Read_implActor*>(this)->destroy(); return 0; }
															#line 3551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Read_implActor*>(this)->SAV< int >::value()) int(read_bytes);
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (_lseeki64(self->h, offset, SEEK_SET) == -1)
															#line 3563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 1);
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		unsigned int read_bytes = 0;
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if ((read_bytes = _read(self->h, data, (unsigned int)length)) == -1)
															#line 3575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 2);
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			uint32_t a = crc32c_append(0, (const uint8_t*)data, read_bytes);
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFR2 %s %s %s %d %d\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str(), read_bytes, a);
															#line 3591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		debugFileCheck("SimpleFileRead", self->filename, data, offset, length);
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::read");
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::read");
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Read_implActor*>(this)->SAV<int>::futures) { (void)(read_bytes); this->~Read_implActorState(); static_cast<Read_implActor*>(this)->destroy(); return 0; }
															#line 3601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	void* data;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int length;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int64_t offset;
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 3682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via read_impl()
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Read_implActor final : public Actor<int>, public ActorCallback< Read_implActor, 0, Void >, public FastAllocated<Read_implActor>, public Read_implActorState<Read_implActor> {
															#line 3687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Read_implActor>::operator new;
	using FastAllocated<Read_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_implActor, 0, Void >;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Read_implActor(SimpleFile* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 3698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<int> read_impl( SimpleFile* const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<int>(new Read_implActor(self, data, length, offset));
															#line 3725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

																#line 3730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via write_impl()
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class Write_implActor>
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Write_implActorState {
															#line 3736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Write_implActorState(SimpleFile* const& self,StringRef const& data,int64_t const& offset) 
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   data(data),
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   offset(offset),
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 3749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 3764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				uint32_t a = crc32c_append(0, data.begin(), data.size());
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				fmt::print(randLog, "SFW1 {0} {1} {2} {3} {4} {5}\n", self->dbgId.shortString(), self->filename, opId.shortString(), a, data.size(), offset);
															#line 3770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (self->delayOnWrite)
															#line 3774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, data.size());
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Write_implActor*>(this)->actor_wait_state = 1;
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 0, Void >*>(static_cast<Write_implActor*>(this)));
															#line 3785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (_lseeki64(self->h, offset, SEEK_SET) == -1)
															#line 3813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 3);
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		unsigned int write_bytes = 0;
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if ((write_bytes = _write(self->h, (void*)data.begin(), data.size())) == -1)
															#line 3825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 4);
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (write_bytes != data.size())
															#line 3835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 5);
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 3841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 3845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFW2 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 3849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		debugFileCheck("SimpleFileWrite", self->filename, (void*)data.begin(), offset, data.size());
															#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::write");
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::write");
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 3859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	StringRef data;
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int64_t offset;
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 3950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via write_impl()
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Write_implActor final : public Actor<Void>, public ActorCallback< Write_implActor, 0, Void >, public FastAllocated<Write_implActor>, public Write_implActorState<Write_implActor> {
															#line 3955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Write_implActor>::operator new;
	using FastAllocated<Write_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Write_implActor, 0, Void >;
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Write_implActor(SimpleFile* const& self,StringRef const& data,int64_t const& offset) 
															#line 3966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> write_impl( SimpleFile* const& self, StringRef const& data, int64_t const& offset ) {
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new Write_implActor(self, data, offset));
															#line 3993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

																#line 3998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via truncate_impl()
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class Truncate_implActor>
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Truncate_implActorState {
															#line 4004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Truncate_implActorState(SimpleFile* const& self,int64_t const& size) 
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   size(size),
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 4015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 4030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				fmt::print( randLog, "SFT1 {0} {1} {2} {3}\n", self->dbgId.shortString(), self->filename, opId.shortString(), size);
															#line 4034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if ((self->flags & IAsyncFile::OPEN_NO_AIO) == 0 && size == 0)
															#line 4038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				return a_body1Catch1(io_error(), loopDepth);
															#line 4042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (self->delayOnWrite)
															#line 4046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0);
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Truncate_implActor*>(this)->actor_wait_state = 1;
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 0, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 4057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (_chsize(self->h, (long)size) == -1)
															#line 4085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError") .detail("Location", 6) .detail("Filename", self->filename) .detail("Size", size) .detail("Fd", self->h) .GetLastError();
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 4091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFT2 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::truncate");
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::truncate");
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Truncate_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActorState(); static_cast<Truncate_implActor*>(this)->destroy(); return 0; }
															#line 4107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int64_t size;
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 4196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via truncate_impl()
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Truncate_implActor final : public Actor<Void>, public ActorCallback< Truncate_implActor, 0, Void >, public FastAllocated<Truncate_implActor>, public Truncate_implActorState<Truncate_implActor> {
															#line 4201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Truncate_implActor>::operator new;
	using FastAllocated<Truncate_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Truncate_implActor, 0, Void >;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Truncate_implActor(SimpleFile* const& self,int64_t const& size) 
															#line 4212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> truncate_impl( SimpleFile* const& self, int64_t const& size ) {
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new Truncate_implActor(self, size));
															#line 4239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

	// Simulated sync does not actually do anything besides wait a random amount of time
																#line 4245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via sync_impl()
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class Sync_implActor>
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Sync_implActorState {
															#line 4251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sync_implActorState(SimpleFile* const& self) 
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 4260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 4275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				fprintf(randLog, "SFC1 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (self->delayOnWrite)
															#line 4283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0, true);
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (static_cast<Sync_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Sync_implActor*>(this)->actor_wait_state = 1;
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Sync_implActor, 0, Void >*>(static_cast<Sync_implActor*>(this)));
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (self->flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 4322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			self->flags &= ~OPEN_ATOMIC_WRITE_AND_CREATE;
															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			auto& machineCache = g_simulator->getCurrentProcess()->machine->openFiles;
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			std::string sourceFilename = self->filename + ".part";
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (machineCache.count(sourceFilename))
															#line 4332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				using block_value_type = typename decltype(g_simulator->corruptedBlocks)::key_type::second_type;
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				TraceEvent("SimpleFileRename") .detail("From", sourceFilename) .detail("To", self->filename) .detail("SourceCount", machineCache.count(sourceFilename)) .detail("FileCount", machineCache.count(self->filename));
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				auto maxBlockValue = std::numeric_limits<block_value_type>::max();
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				g_simulator->corruptedBlocks.erase( g_simulator->corruptedBlocks.lower_bound(std::make_pair(sourceFilename, 0u)), g_simulator->corruptedBlocks.upper_bound(std::make_pair(self->filename, maxBlockValue)));
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				auto begin = g_simulator->corruptedBlocks.lower_bound(std::make_pair(sourceFilename, 0u)), end = g_simulator->corruptedBlocks.upper_bound(std::make_pair(sourceFilename, maxBlockValue));
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				for(auto iter = begin;iter != end;++iter) {
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					g_simulator->corruptedBlocks.emplace(self->filename, iter->second);
															#line 4348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				}
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				g_simulator->corruptedBlocks.erase(begin, end);
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				renameFile(sourceFilename.c_str(), self->filename.c_str());
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				machineCache[self->filename] = machineCache[sourceFilename];
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				machineCache.erase(sourceFilename);
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				self->actualFilename = self->filename;
															#line 4360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
		}
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fprintf(randLog, "SFC2 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_timeout, "SimpleFile::sync");
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::sync");
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Sync_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Sync_implActorState(); static_cast<Sync_implActor*>(this)->destroy(); return 0; }
															#line 4377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SimpleFile* self;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 4464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via sync_impl()
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Sync_implActor final : public Actor<Void>, public ActorCallback< Sync_implActor, 0, Void >, public FastAllocated<Sync_implActor>, public Sync_implActorState<Sync_implActor> {
															#line 4469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Sync_implActor>::operator new;
	using FastAllocated<Sync_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Sync_implActor, 0, Void >;
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sync_implActor(SimpleFile* const& self) 
															#line 4480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> sync_impl( SimpleFile* const& self ) {
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new Sync_implActor(self));
															#line 4507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

																#line 4512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via size_impl()
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class Size_implActor>
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Size_implActorState {
															#line 4518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Size_implActorState(SimpleFile const* const& self) 
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   opId(deterministicRandom()->randomUniqueID())
															#line 4527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (randLog)
															#line 4542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				fprintf(randLog, "SFS1 %s %s %s\n", self->dbgId.shortString().c_str(), self->filename.c_str(), opId.shortString().c_str());
															#line 4546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0);
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<Size_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Size_implActor*>(this)->actor_wait_state = 1;
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Size_implActor, 0, Void >*>(static_cast<Size_implActor*>(this)));
															#line 4557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		int64_t pos = _lseeki64(self->h, 0L, SEEK_END);
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (pos == -1)
															#line 4582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 8);
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 4588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fmt::print( randLog, "SFS2 {0} {1} {2} {3}\n", self->dbgId.shortString(), self->filename, opId.shortString(), pos);
															#line 4596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::size");
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Size_implActor*>(this)->SAV<int64_t>::futures) { (void)(pos); this->~Size_implActorState(); static_cast<Size_implActor*>(this)->destroy(); return 0; }
															#line 4602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<Size_implActor*>(this)->SAV< int64_t >::value()) int64_t(pos);
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		int64_t pos = _lseeki64(self->h, 0L, SEEK_END);
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (pos == -1)
															#line 4616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevWarn, "SimpleFileIOError").detail("Location", 8);
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(io_error(), loopDepth);
															#line 4622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (randLog)
															#line 4626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			fmt::print( randLog, "SFS2 {0} {1} {2} {3}\n", self->dbgId.shortString(), self->filename, opId.shortString(), pos);
															#line 4630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		INJECT_FAULT(io_error, "SimpleFile::size");
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<Size_implActor*>(this)->SAV<int64_t>::futures) { (void)(pos); this->~Size_implActorState(); static_cast<Size_implActor*>(this)->destroy(); return 0; }
															#line 4636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SimpleFile const* self;
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UID opId;
															#line 4711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via size_impl()
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class Size_implActor final : public Actor<int64_t>, public ActorCallback< Size_implActor, 0, Void >, public FastAllocated<Size_implActor>, public Size_implActorState<Size_implActor> {
															#line 4716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<Size_implActor>::operator new;
	using FastAllocated<Size_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Size_implActor, 0, Void >;
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Size_implActor(SimpleFile const* const& self) 
															#line 4727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<int64_t> size_impl( SimpleFile const* const& self ) {
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<int64_t>(new Size_implActor(self));
															#line 4754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
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

																#line 4787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via incoming()
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class IncomingActor>
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class IncomingActorState {
															#line 4793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	IncomingActorState(Reference<Sim2Listener> const& self,double const& seconds,Reference<IConnection> const& conn) 
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   seconds(seconds),
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   conn(conn)
															#line 4804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onProcess(self->process);
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 4821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			static_cast<IncomingActor*>(this)->actor_wait_state = 1;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncomingActor, 0, Void >*>(static_cast<IncomingActor*>(this)));
															#line 4825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(seconds);
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 4849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		static_cast<IncomingActor*>(this)->actor_wait_state = 2;
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncomingActor, 1, Void >*>(static_cast<IncomingActor*>(this)));
															#line 4853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(seconds);
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 4864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		static_cast<IncomingActor*>(this)->actor_wait_state = 2;
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncomingActor, 1, Void >*>(static_cast<IncomingActor*>(this)));
															#line 4868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (((Sim2Conn*)conn.getPtr())->isPeerGone() && deterministicRandom()->random01() < 0.5)
															#line 4940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			delete static_cast<IncomingActor*>(this);
															#line 4944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			return 0;
		}
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		TraceEvent("Sim2IncomingConn", conn->getDebugID()) .detail("ListenAddress", self->getListenAddress()) .detail("PeerAddress", conn->getPeerAddress());
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->nextConnection.send(conn);
															#line 4951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (((Sim2Conn*)conn.getPtr())->isPeerGone() && deterministicRandom()->random01() < 0.5)
															#line 4960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			delete static_cast<IncomingActor*>(this);
															#line 4964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			return 0;
		}
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		TraceEvent("Sim2IncomingConn", conn->getDebugID()) .detail("ListenAddress", self->getListenAddress()) .detail("PeerAddress", conn->getPeerAddress());
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->nextConnection.send(conn);
															#line 4971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		delete static_cast<IncomingActor*>(this);
															#line 5043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<Sim2Listener> self;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	double seconds;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<IConnection> conn;
															#line 5054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via incoming()
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class IncomingActor final : public Actor<void>, public ActorCallback< IncomingActor, 0, Void >, public ActorCallback< IncomingActor, 1, Void >, public FastAllocated<IncomingActor>, public IncomingActorState<IncomingActor> {
															#line 5059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<IncomingActor>::operator new;
	using FastAllocated<IncomingActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< IncomingActor, 0, Void >;
friend struct ActorCallback< IncomingActor, 1, Void >;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	IncomingActor(Reference<Sim2Listener> const& self,double const& seconds,Reference<IConnection> const& conn) 
															#line 5071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
static void incoming( Reference<Sim2Listener> const& self, double const& seconds, Reference<IConnection> const& conn ) {
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	new IncomingActor(self, seconds, conn);
															#line 5089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
																#line 5093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via popOne()
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class PopOneActor>
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class PopOneActorState {
															#line 5099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	PopOneActorState(FutureStream<Reference<IConnection>> const& conns) 
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : conns(conns)
															#line 5106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			FutureStream<Reference<IConnection>> __when_expr_0 = conns;
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<PopOneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<PopOneActor*>(this)->actor_wait_state = 1;
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >*>(static_cast<PopOneActor*>(this)));
															#line 5128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		((Sim2Conn*)c.getPtr())->opened = true;
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<PopOneActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~PopOneActorState(); static_cast<PopOneActor*>(this)->destroy(); return 0; }
															#line 5153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<PopOneActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(c);
		this->~PopOneActorState();
		static_cast<PopOneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<IConnection> && c,int loopDepth) 
	{
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		((Sim2Conn*)c.getPtr())->opened = true;
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<PopOneActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~PopOneActorState(); static_cast<PopOneActor*>(this)->destroy(); return 0; }
															#line 5167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	FutureStream<Reference<IConnection>> conns;
															#line 5240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via popOne()
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class PopOneActor final : public Actor<Reference<IConnection>>, public ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >, public FastAllocated<PopOneActor>, public PopOneActorState<PopOneActor> {
															#line 5245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<PopOneActor>::operator new;
	using FastAllocated<PopOneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< PopOneActor, 0, Reference<IConnection> >;
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	PopOneActor(FutureStream<Reference<IConnection>> const& conns) 
															#line 5256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> popOne( FutureStream<Reference<IConnection>> const& conns ) {
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IConnection>>(new PopOneActor(conns));
															#line 5283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

	NetworkAddress address;
};

#define g_sim2 ((Sim2&)(*g_simulator))

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

		if (seconds >= 4e12) // Intervals that overflow an int64_t in microseconds (more than 100,000 years) are treated
		                     // as infinite
			return Never();

		PromiseTask* t = new PromiseTask(machine);
		if (seconds <= TIME_EPS) {
			taskQueue.addReady(taskID, t);
		} else {
			if (!ordered && !currentProcess->rebooting && machine == currentProcess &&
			    !currentProcess->shutdownSignal.isSet() && FLOW_KNOBS->MAX_BUGGIFIED_DELAY > 0 &&
			    deterministicRandom()->random01() < 0.25) {
				// FIXME: why doesn't this work when we are changing machines?
				seconds += FLOW_KNOBS->MAX_BUGGIFIED_DELAY * pow(deterministicRandom()->random01(), 1000.0);
			}
			double at = now() + seconds;
			taskQueue.addTimer(at, taskID, t);
		}
		return t->promise.getFuture();
	}
																#line 5338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via checkShutdown()
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class CheckShutdownActor>
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class CheckShutdownActorState {
															#line 5344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	CheckShutdownActorState(Sim2* const& self,TaskPriority const& taskID) 
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   taskID(taskID)
															#line 5353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(self->getCurrentProcess()->shutdownSignal.getFuture());
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<CheckShutdownActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckShutdownActor*>(this)->actor_wait_state = 1;
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckShutdownActor, 0, Void >*>(static_cast<CheckShutdownActor*>(this)));
															#line 5375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->setCurrentTask(taskID);
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CheckShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckShutdownActorState(); static_cast<CheckShutdownActor*>(this)->destroy(); return 0; }
															#line 5400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<CheckShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckShutdownActorState();
		static_cast<CheckShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->setCurrentTask(taskID);
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CheckShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckShutdownActorState(); static_cast<CheckShutdownActor*>(this)->destroy(); return 0; }
															#line 5414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2* self;
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	TaskPriority taskID;
															#line 5489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via checkShutdown()
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class CheckShutdownActor final : public Actor<Void>, public ActorCallback< CheckShutdownActor, 0, Void >, public FastAllocated<CheckShutdownActor>, public CheckShutdownActorState<CheckShutdownActor> {
															#line 5494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<CheckShutdownActor>::operator new;
	using FastAllocated<CheckShutdownActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckShutdownActor, 0, Void >;
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	CheckShutdownActor(Sim2* const& self,TaskPriority const& taskID) 
															#line 5505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> checkShutdown( Sim2* const& self, TaskPriority const& taskID ) {
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new CheckShutdownActor(self, taskID));
															#line 5532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
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
		// If sim http connection, do connect instead of external connect
		if (httpServerIps.count(toAddr.ip)) {
			return connect(toAddr);
		}
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
																#line 5661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via onConnect()
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class OnConnectActor>
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class OnConnectActorState {
															#line 5667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	OnConnectActorState(Future<Void> const& ready,Reference<Sim2Conn> const& conn) 
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : ready(ready),
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   conn(conn)
															#line 5676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ready;
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OnConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnConnectActor*>(this)->actor_wait_state = 1;
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnConnectActor, 0, Void >*>(static_cast<OnConnectActor*>(this)));
															#line 5698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (conn->isPeerGone())
															#line 5721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			conn.clear();
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 1 || (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 2 && deterministicRandom()->random01() > 0.5))
															#line 5727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				return a_body1Catch1(connection_failed(), loopDepth);
															#line 5731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = Never();
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OnConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnConnectActor*>(this)->actor_wait_state = 2;
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnConnectActor, 1, Void >*>(static_cast<OnConnectActor*>(this)));
															#line 5742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (conn->isPeerGone())
															#line 5756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			conn.clear();
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 1 || (FLOW_KNOBS->SIM_CONNECT_ERROR_MODE == 2 && deterministicRandom()->random01() > 0.5))
															#line 5762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				return a_body1Catch1(connection_failed(), loopDepth);
															#line 5766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = Never();
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<OnConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnConnectActor*>(this)->actor_wait_state = 2;
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnConnectActor, 1, Void >*>(static_cast<OnConnectActor*>(this)));
															#line 5777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		conn->opened = true;
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<OnConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~OnConnectActorState(); static_cast<OnConnectActor*>(this)->destroy(); return 0; }
															#line 5856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Future<Void> ready;
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<Sim2Conn> conn;
															#line 5943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via onConnect()
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class OnConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< OnConnectActor, 0, Void >, public ActorCallback< OnConnectActor, 1, Void >, public FastAllocated<OnConnectActor>, public OnConnectActorState<OnConnectActor> {
															#line 5948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<OnConnectActor>::operator new;
	using FastAllocated<OnConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnConnectActor, 0, Void >;
friend struct ActorCallback< OnConnectActor, 1, Void >;
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	OnConnectActor(Future<Void> const& ready,Reference<Sim2Conn> const& conn) 
															#line 5960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> onConnect( Future<Void> const& ready, Reference<Sim2Conn> const& conn ) {
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IConnection>>(new OnConnectActor(ready, conn));
															#line 5988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 1230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<IListener> listen(NetworkAddress localAddr) override {
		Reference<IListener> listener(getCurrentProcess()->getListener(localAddr));
		ASSERT(listener);
		return listener;
	}
																#line 5997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via waitForProcessAndConnect()
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class WaitForProcessAndConnectActor>
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class WaitForProcessAndConnectActorState {
															#line 6003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	WaitForProcessAndConnectActorState(NetworkAddress const& toAddr,INetworkConnections* const& self) 
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : toAddr(toAddr),
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   self(self)
															#line 6012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			;
															#line 6027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = ::delay(0.1 * deterministicRandom()->random01());
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 1;
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForProcessAndConnectActor, 0, Void >*>(static_cast<WaitForProcessAndConnectActor*>(this)));
															#line 6064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (g_sim2.addressMap.count(toAddr))
															#line 6073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_1 = self->connect(toAddr);
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 2;
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*>(static_cast<WaitForProcessAndConnectActor*>(this)));
															#line 6084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (g_sim2.addressMap.count(toAddr))
															#line 6098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_1 = self->connect(toAddr);
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WaitForProcessAndConnectActor*>(this)->actor_wait_state = 2;
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >*>(static_cast<WaitForProcessAndConnectActor*>(this)));
															#line 6109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<WaitForProcessAndConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~WaitForProcessAndConnectActorState(); static_cast<WaitForProcessAndConnectActor*>(this)->destroy(); return 0; }
															#line 6192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<WaitForProcessAndConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(c);
		this->~WaitForProcessAndConnectActorState();
		static_cast<WaitForProcessAndConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Reference<IConnection> && c,int loopDepth) 
	{
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<WaitForProcessAndConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(c); this->~WaitForProcessAndConnectActorState(); static_cast<WaitForProcessAndConnectActor*>(this)->destroy(); return 0; }
															#line 6204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	NetworkAddress toAddr;
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	INetworkConnections* self;
															#line 6279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via waitForProcessAndConnect()
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class WaitForProcessAndConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< WaitForProcessAndConnectActor, 0, Void >, public ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >, public FastAllocated<WaitForProcessAndConnectActor>, public WaitForProcessAndConnectActorState<WaitForProcessAndConnectActor> {
															#line 6284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<WaitForProcessAndConnectActor>::operator new;
	using FastAllocated<WaitForProcessAndConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForProcessAndConnectActor, 0, Void >;
friend struct ActorCallback< WaitForProcessAndConnectActor, 1, Reference<IConnection> >;
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	WaitForProcessAndConnectActor(NetworkAddress const& toAddr,INetworkConnections* const& self) 
															#line 6296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> waitForProcessAndConnect( NetworkAddress const& toAddr, INetworkConnections* const& self ) {
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Reference<IConnection>>(new WaitForProcessAndConnectActor(toAddr, self));
															#line 6324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
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
			currentProcess = g_simulator->getCurrentProcess();
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

																#line 6421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via deleteFileImpl()
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class DeleteFileImplActor>
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class DeleteFileImplActorState {
															#line 6427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	DeleteFileImplActorState(Sim2* const& self,std::string const& filename,bool const& mustBeDurable) 
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   filename(filename),
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   mustBeDurable(mustBeDurable)
															#line 6438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (g_simulator->getCurrentProcess()->machine->openFiles.count(filename))
															#line 6453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				g_simulator->getCurrentProcess()->machine->openFiles.erase(filename);
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				g_simulator->getCurrentProcess()->machine->deletingOrClosingFiles.insert(filename);
															#line 6459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (mustBeDurable || deterministicRandom()->random01() < 0.5)
															#line 6463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				currentProcess = g_simulator->getCurrentProcess();
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				currentTaskID = g_network->getCurrentTask();
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "Sim2DeleteFileImpl") .detail("CurrentProcess", currentProcess->toString()) .detail("Filename", filename) .detail("Durable", mustBeDurable);
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 1;
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 0, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 1371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "Sim2DeleteFileImplNonDurable") .detail("Filename", filename) .detail("Durable", mustBeDurable);
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				CODE_PROBE(true, "Simulated non-durable delete", probe::context::sim2, probe::assert::simOnly);
															#line 1375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (!static_cast<DeleteFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteFileImplActorState(); static_cast<DeleteFileImplActor*>(this)->destroy(); return 0; }
															#line 6491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont3Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 2;
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 1, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont3Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 2;
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 1, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			err = e;
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_4 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont3Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 5;
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 4, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!currentProcess->rebooting)
															#line 6652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			auto f = IAsyncFileSystem::filesystem(self->net2)->deleteFile(filename, false);
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(f.isReady());
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont3Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 3;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 2, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!currentProcess->rebooting)
															#line 6681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			auto f = IAsyncFileSystem::filesystem(self->net2)->deleteFile(filename, false);
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(f.isReady());
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ::delay(0.05 * deterministicRandom()->random01());
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont3Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
			static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 3;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 2, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<DeleteFileImplActor*>(this)->actor_wait_state < 0) return a_body1cont3Catch1(actor_cancelled(), loopDepth);
															#line 6775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont3Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<DeleteFileImplActor*>(this)->actor_wait_state = 4;
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DeleteFileImplActor, 3, Void >*>(static_cast<DeleteFileImplActor*>(this)));
															#line 6780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		CODE_PROBE(true, "Simulated durable delete", probe::context::sim2, probe::assert::simOnly);
															#line 6789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		CODE_PROBE(true, "Simulated durable delete", probe::context::sim2, probe::assert::simOnly);
															#line 6798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<DeleteFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteFileImplActorState(); static_cast<DeleteFileImplActor*>(this)->destroy(); return 0; }
															#line 6870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<DeleteFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteFileImplActorState();
		static_cast<DeleteFileImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<DeleteFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteFileImplActorState(); static_cast<DeleteFileImplActor*>(this)->destroy(); return 0; }
															#line 6882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 6957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont3Catch1cont1(Void && _,int loopDepth) 
	{
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 6965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"

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
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2* self;
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::string filename;
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	bool mustBeDurable;
															#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	TaskPriority currentTaskID;
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Error err;
															#line 7044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via deleteFileImpl()
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class DeleteFileImplActor final : public Actor<Void>, public ActorCallback< DeleteFileImplActor, 0, Void >, public ActorCallback< DeleteFileImplActor, 1, Void >, public ActorCallback< DeleteFileImplActor, 2, Void >, public ActorCallback< DeleteFileImplActor, 3, Void >, public ActorCallback< DeleteFileImplActor, 4, Void >, public FastAllocated<DeleteFileImplActor>, public DeleteFileImplActorState<DeleteFileImplActor> {
															#line 7049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	DeleteFileImplActor(Sim2* const& self,std::string const& filename,bool const& mustBeDurable) 
															#line 7064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> deleteFileImpl( Sim2* const& self, std::string const& filename, bool const& mustBeDurable ) {
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new DeleteFileImplActor(self, filename, mustBeDurable));
															#line 7095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

	static void runLoop(Sim2* self) {
		ISimulator::ProcessInfo* callingMachine = self->currentProcess;
		int lastPrintTime = 0;
		while (!self->isStopped) {
			if (self->taskQueue.canSleep()) {
				double sleepTime = self->taskQueue.getSleepTime(self->time);
				self->time +=
				    sleepTime + FLOW_KNOBS->MAX_RUNLOOP_SLEEP_DELAY * pow(deterministicRandom()->random01(), 1000.0);
				if (self->printSimTime && (int)self->time > lastPrintTime) {
					printf("Time: %d\n", (int)self->time);
					lastPrintTime = (int)self->time;
				}
				self->timerTime = std::max(self->timerTime, self->time);
			}
			// if (!randLog/* && now() >= 32.0*/)
			//	randLog = fopen("randLog.txt", "wt");

			self->taskQueue.processReadyTimers(self->time);
			self->taskQueue.processThreadReady();

			while (self->taskQueue.hasReadyTask()) {
				self->currentTaskID = self->taskQueue.getReadyTaskID();
				PromiseTask* task = self->taskQueue.getReadyTask();
				self->taskQueue.popReadyTask();
				self->execTask(*task);
				delete task;
				self->yielded = false;
			}
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
	                        ProtocolVersion protocol,
	                        bool drProcess) override {
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
		m->excluded = g_simulator->isExcluded(NetworkAddress(ip, port, true, false));
		m->cleared = g_simulator->isCleared(addresses.address);
		m->protocolVersion = protocol;
		m->drProcess = drProcess;

		m->setGlobal(enTDMetrics, (flowGlobalType)&m->tdmetrics);
		if (FLOW_KNOBS->ENABLE_CHAOS_FEATURES) {
			m->setGlobal(enChaosMetrics, (flowGlobalType)&m->chaosMetrics);
		}
		m->setGlobal(enNetworkConnections, (flowGlobalType)m->network);
		m->setGlobal(enASIOTimedOut, (flowGlobalType) false);
		m->setGlobal(INetwork::enMetrics, (flowGlobalType)&m->metrics);

		TraceEvent("NewMachine")
		    .detail("Name", name)
		    .detail("Address", m->address)
		    .detail("MachineId", m->locality.machineId())
		    .detail("Excluded", m->excluded)
		    .detail("Cleared", m->cleared)
		    .detail("DrProcess", m->drProcess);

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
		return canKillProcesses(processesLeft, processesDead, KillType::KillInstantly, nullptr);
	}

	std::vector<AddressExclusion> getAllAddressesInDCToExclude(Optional<Standalone<StringRef>> dcId) const override {
		std::vector<AddressExclusion> addresses;
		if (!dcId.present()) {
			return addresses;
		}
		for (const auto& processInfo : getAllProcesses()) {
			if (processInfo->locality.dcId() == dcId) {
				addresses.emplace_back(processInfo->address.ip, processInfo->address.port);
			}
		}
		return addresses;
	}

	bool datacenterDead(Optional<Standalone<StringRef>> dcId) const override {
		if (!dcId.present()) {
			return false;
		}

		LocalityGroup primaryProcessesLeft, primaryProcessesDead;
		std::vector<LocalityData> primaryLocalitiesDead, primaryLocalitiesLeft;

		for (auto processInfo : getAllProcesses()) {
			if (!processInfo->isSpawnedKVProcess() && processInfo->isAvailableClass() &&
			    processInfo->locality.dcId() == dcId) {
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

	// The following function will determine if a machine can be remove in case when it has a blob worker
	bool canKillMachineWithBlobWorkers(Optional<Standalone<StringRef>> machineId, KillType kt, KillType* ktFinal) {
		// Allow if no blob workers, or it's a reboot(without removing the machine)
		// FIXME: this should be ||
		if (!blobGranulesEnabled && kt >= KillType::RebootAndDelete) {
			return true;
		}

		// Allow if the machine doesn't support blob worker
		MachineInfo& currentMachine = machines[machineId];
		bool hasBlobWorker = false;
		for (auto processInfo : currentMachine.processes) {
			if (processInfo->startingClass == ProcessClass::BlobWorkerClass) {
				hasBlobWorker = true;
				break;
			}
		}
		if (!hasBlobWorker)
			return true;

		// Count # remaining support blob workers in current dc
		auto currentDcId = currentMachine.machineProcess->locality.dcId();
		int nLeft = 0;
		for (auto processInfo : getAllProcesses()) {
			if (currentDcId != processInfo->locality.dcId() || // skip other dc
			    processInfo->startingClass != ProcessClass::BlobWorkerClass || // skip non blob workers
			    processInfo->failed || // if process was killed but has not yet been removed from the process list
			    processInfo->locality.machineId() == machineId) { // skip current machine
				continue;
			}
			nLeft++; // alive blob workers after killing machineId
		}

		// Ensure there is at least 1 remaining blob workers after removing current machine
		if (nLeft <= 1) {
			*ktFinal = KillType::RebootAndDelete; // reboot and delete data, but keep this machine
			return false;
		}
		return true;
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
		if ((kt == KillType::KillInstantly) || (kt == KillType::InjectFaults) || (kt == KillType::FailDisk) ||
		    (kt == KillType::RebootAndDelete) || (kt == KillType::RebootProcessAndDelete)) {
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

			if (!primaryDcId.present() || usableRegions == 1) {
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
					}
					if (processInfo->locality.dcId() == remoteDcId) {
						remoteProcessesLeft.add(processInfo->locality);
						remoteLocalitiesLeft.push_back(processInfo->locality);
					}
					if (std::find(primarySatelliteDcIds.begin(),
					              primarySatelliteDcIds.end(),
					              processInfo->locality.dcId()) != primarySatelliteDcIds.end()) {
						primarySatelliteProcessesLeft.add(processInfo->locality);
						primarySatelliteLocalitiesLeft.push_back(processInfo->locality);
					}
					if (std::find(remoteSatelliteDcIds.begin(),
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
					}
					if (processInfo->locality.dcId() == remoteDcId) {
						remoteProcessesDead.add(processInfo->locality);
						remoteLocalitiesDead.push_back(processInfo->locality);
					}
					if (std::find(primarySatelliteDcIds.begin(),
					              primarySatelliteDcIds.end(),
					              processInfo->locality.dcId()) != primarySatelliteDcIds.end()) {
						primarySatelliteProcessesDead.add(processInfo->locality);
						primarySatelliteLocalitiesDead.push_back(processInfo->locality);
					}
					if (std::find(remoteSatelliteDcIds.begin(),
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
			if (tooManyDead || (usableRegions > 1 && notEnoughLeft)) {
				newKt = KillType::Reboot;
				canSurvive = false;
				TraceEvent("KillChanged")
				    .detail("KillType", kt)
				    .detail("NewKillType", newKt)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("Reason", "Too many dead processes that cannot satisfy tLogPolicy.");
			}
			// Reboot and Delete if remaining machines do NOT fulfill policies
			else if ((kt < KillType::RebootAndDelete) && notEnoughLeft) {
				newKt = KillType::RebootAndDelete;
				canSurvive = false;
				TraceEvent("KillChanged")
				    .detail("KillType", kt)
				    .detail("NewKillType", newKt)
				    .detail("TLogPolicy", tLogPolicy->info())
				    .detail("Reason", "Not enough tLog left to satisfy tLogPolicy.");
			} else if ((kt < KillType::RebootAndDelete) && (nQuorum > uniqueMachines.size())) {
				newKt = KillType::RebootAndDelete;
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
		killProcess_internal(p, KillType::KillInstantly);
	}
	void killProcess_internal(ProcessInfo* machine, KillType kt) {
		CODE_PROBE(
		    true, "Simulated machine was killed with any kill type", probe::context::sim2, probe::assert::simOnly);
		CODE_PROBE(kt == KillType::KillInstantly,
		           "Simulated machine was killed instantly",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE(kt == KillType::InjectFaults,
		           "Simulated machine was killed with faults",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE(kt == KillType::FailDisk,
		           "Simulated machine was killed with a failed disk",
		           probe::context::sim2,
		           probe::assert::simOnly,
		           probe::decoration::rare);

		if (kt == KillType::KillInstantly) {
			TraceEvent(SevWarn, "FailMachine")
			    .detail("Name", machine->name)
			    .detail("Address", machine->address)
			    .detail("ZoneId", machine->locality.zoneId())
			    .detail("Process", machine->toString())
			    .detail("Rebooting", machine->rebooting)
			    .detail("Protected", protectedAddresses.count(machine->address))
			    .backtrace();
			// This will remove all the "tracked" messages that came from the machine being killed
			if (!machine->isSpawnedKVProcess())
				latestEventCache.clear();
			machine->failed = true;
		} else if (kt == KillType::InjectFaults) {
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
		} else if (kt == KillType::FailDisk) {
			TraceEvent(SevWarn, "KillType::FailDiskMachine")
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
		ASSERT(!protectedAddresses.count(machine->address) || machine->rebooting || machine->isSpawnedKVProcess());
	}
	void rebootProcess(ProcessInfo* process, KillType kt) override {
		if (kt == KillType::RebootProcessAndDelete && protectedAddresses.count(process->address)) {
			TraceEvent("RebootChanged")
			    .detail("ZoneId", process->locality.describeZone())
			    .detail("KillType", KillType::RebootProcess)
			    .detail("OrigKillType", kt)
			    .detail("Reason", "Protected process");
			kt = KillType::RebootProcess;
		}
		doReboot(process, kt);
	}
	void rebootProcess(Optional<Standalone<StringRef>> zoneId, bool allProcesses) override {
		if (allProcesses) {
			auto processes = getAllProcesses();
			for (int i = 0; i < processes.size(); i++)
				if (processes[i]->locality.zoneId() == zoneId && !processes[i]->rebooting)
					doReboot(processes[i], KillType::RebootProcess);
		} else {
			auto processes = getAllProcesses();
			for (int i = 0; i < processes.size(); i++) {
				if (processes[i]->locality.zoneId() != zoneId || processes[i]->rebooting) {
					swapAndPop(&processes, i--);
				}
			}
			if (processes.size())
				doReboot(deterministicRandom()->randomChoice(processes), KillType::RebootProcess);
		}
	}
	void killProcess(ProcessInfo* machine, KillType kt) override {
		TraceEvent("AttemptingKillProcess").detail("ProcessInfo", machine->toString());
		// Refuse to kill a protected process.
		if (kt < KillType::RebootAndDelete && protectedAddresses.count(machine->address) == 0) {
			killProcess_internal(machine, kt);
		}
	}
	void killInterface(NetworkAddress address, KillType kt) override {
		if (kt < KillType::RebootAndDelete) {
			std::vector<ProcessInfo*>& processes = machines[addressMap[address]->locality.machineId()].processes;
			for (auto& process : processes) {
				// Refuse to kill a protected process.
				if (protectedAddresses.count(process->address) == 0) {
					killProcess_internal(process, kt);
				}
			}
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
	bool killDataHall(Optional<Standalone<StringRef>> dataHallId,
	                  KillType kt,
	                  bool forceKill,
	                  KillType* ktFinal) override {
		auto processes = getAllProcesses();
		std::set<Optional<Standalone<StringRef>>> dataHallMachines;
		for (auto& process : processes) {
			if (process->locality.dataHallId() == dataHallId) {
				dataHallMachines.insert(process->locality.machineId());
			}
		}
		bool result = false;
		for (auto& machineId : dataHallMachines) {
			if (killMachine(machineId, kt, forceKill, ktFinal)) {
				result = true;
			}
		}
		return result;
	}
	bool killAll(KillType kt, bool forceKill, KillType* ktFinal) override {
		bool result = false;
		for (auto& machine : machines) {
			if (killMachine(machine.second.machineId, kt, forceKill, ktFinal)) {
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

		CODE_PROBE(true, "Trying to killing a machine", probe::context::sim2, probe::assert::simOnly);
		CODE_PROBE(
		    kt == KillType::KillInstantly, "Trying to kill instantly", probe::context::sim2, probe::assert::simOnly);
		CODE_PROBE(kt == KillType::InjectFaults,
		           "Trying to kill by injecting faults",
		           probe::context::sim2,
		           probe::assert::simOnly);

		if (speedUpSimulation && !forceKill) {
			TraceEvent(SevWarn, "AbortedKill")
			    .detail("MachineId", machineId)
			    .detail("Reason", "Unforced kill within speedy simulation.")
			    .backtrace();
			if (ktFinal)
				*ktFinal = KillType::None;
			return false;
		}

		int processesOnMachine = 0;
		bool isMainCluster = true; // false for machines running DR processes

		KillType originalKt = kt;
		// Reboot if any of the processes are protected and count the number of processes not rebooting
		for (auto& process : machines[machineId].processes) {
			if (protectedAddresses.count(process->address) && kt != KillType::RebootProcessAndSwitch) {
				kt = KillType::Reboot;
			}

			if (!process->rebooting)
				processesOnMachine++;
			if (process->drProcess) {
				isMainCluster = false;
			}
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
				*ktFinal = KillType::None;
			return false;
		}

		// Check if machine can be removed, if requested
		if (!forceKill &&
		    ((kt == KillType::KillInstantly) || (kt == KillType::InjectFaults) || (kt == KillType::FailDisk) ||
		     (kt == KillType::RebootAndDelete) || (kt == KillType::RebootProcessAndDelete))) {

			if (!canKillMachineWithBlobWorkers(machineId, kt, &kt)) {
				TraceEvent("CanKillMachineWithBlobWorkers")
				    .detail("MachineId", machineId)
				    .detail("KillType", kt)
				    .detail("OrigKillType", ktOrig);
			}

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
			} else if ((kt == KillType::KillInstantly) || (kt == KillType::InjectFaults) ||
			           (kt == KillType::FailDisk)) {
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

		CODE_PROBE(originalKt != kt,
		           "Kill type was changed from requested to reboot.",
		           probe::context::sim2,
		           probe::assert::simOnly);

		if (isMainCluster && originalKt == KillType::RebootProcessAndSwitch) {
			// When killing processes with the RebootProcessAndSwitch kill
			// type, processes in the original cluster should be rebooted in
			// order to kill any zombie processes.
			kt = KillType::Reboot;
		} else if (processesOnMachine != processesPerMachine && kt != KillType::RebootProcessAndSwitch) {
			// Check if any processes on machine are rebooting
			CODE_PROBE(true,
			           "Attempted reboot, but the target did not have all of its processes running",
			           probe::context::sim2,
			           probe::assert::simOnly);
			TraceEvent(SevWarn, "AbortedKill")
			    .detail("KillType", kt)
			    .detail("MachineId", machineId)
			    .detail("Reason", "Machine processes does not match number of processes per machine")
			    .detail("Processes", processesOnMachine)
			    .detail("ProcessesPerMachine", processesPerMachine)
			    .backtrace();
			if (ktFinal)
				*ktFinal = KillType::None;
			return false;
		}

		TraceEvent("KillMachine")
		    .detail("MachineId", machineId)
		    .detail("Kt", kt)
		    .detail("KtOrig", ktOrig)
		    .detail("KillableMachines", processesOnMachine)
		    .detail("ProcessPerMachine", processesPerMachine)
		    .detail("KillChanged", kt != ktOrig);
		if (kt < KillType::RebootAndDelete) {
			if ((kt == KillType::InjectFaults || kt == KillType::FailDisk) &&
			    machines[machineId].machineProcess != nullptr)
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
		} else if (kt == KillType::Reboot || kt == KillType::RebootAndDelete ||
		           kt == KillType::RebootProcessAndSwitch) {
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

		CODE_PROBE(kt == KillType::RebootAndDelete,
		           "Resulted in a reboot and delete",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE(kt == KillType::Reboot, "Resulted in a reboot", probe::context::sim2, probe::assert::simOnly);
		CODE_PROBE(
		    kt == KillType::KillInstantly, "Resulted in an instant kill", probe::context::sim2, probe::assert::simOnly);
		CODE_PROBE(kt == KillType::InjectFaults,
		           "Resulted in a kill by injecting faults",
		           probe::context::sim2,
		           probe::assert::simOnly);

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
				if ((kt != KillType::Reboot) && (protectedAddresses.count(procRecord->address))) {
					kt = KillType::Reboot;
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
		if (!forceKill &&
		    ((kt == KillType::KillInstantly) || (kt == KillType::InjectFaults) || (kt == KillType::FailDisk) ||
		     (kt == KillType::RebootAndDelete) || (kt == KillType::RebootProcessAndDelete))) {
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
					ASSERT(ktResult == KillType::None);
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

		CODE_PROBE(kt != ktMin,
		           "DataCenter kill was rejected by killMachine",
		           probe::context::sim2,
		           probe::assert::simOnly,
		           probe::decoration::rare);
		CODE_PROBE((kt == ktMin) && (kt == KillType::RebootAndDelete),
		           "Datacenter kill Resulted in a reboot and delete",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE((kt == ktMin) && (kt == KillType::Reboot),
		           "Datacenter kill Resulted in a reboot",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE((kt == ktMin) && (kt == KillType::KillInstantly),
		           "Datacenter kill Resulted in an instant kill",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE((kt == ktMin) && (kt == KillType::InjectFaults),
		           "Datacenter kill Resulted in a kill by injecting faults",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE((kt == ktMin) && (kt != ktOrig),
		           "Datacenter Kill request was downgraded",
		           probe::context::sim2,
		           probe::assert::simOnly);
		CODE_PROBE((kt == ktMin) && (kt == ktOrig),
		           "Datacenter kill - Requested kill was done",
		           probe::context::sim2,
		           probe::assert::simOnly);

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

	void processInjectBlobFault(ProcessInfo* machine, double failureRate) override {
		CODE_PROBE(true, "Simulated process beginning blob fault", probe::context::sim2, probe::assert::simOnly);
		should_inject_blob_fault = simulator_should_inject_blob_fault;
		ASSERT(machine->blob_inject_failure_rate == 0.0);
		machine->blob_inject_failure_rate = failureRate;
	}

	void processStopInjectBlobFault(ProcessInfo* machine) override {
		CODE_PROBE(true, "Simulated process stopping blob fault", probe::context::sim2, probe::assert::simOnly);
		machine->blob_inject_failure_rate = 0.0;
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
			killProcess_internal(machine.machineProcess, KillType::KillInstantly);
		}
		machines.erase(machineId);
	}

	// Assumes the simulator is already onProcess for proc
	void startRequestHandlerOnProcess(ProcessInfo* process,
	                                  Reference<HTTP::SimServerContext> serverContext,
	                                  Reference<HTTP::SimRegisteredHandlerContext> handlerContext) {
		try {
			NetworkAddress addr = serverContext->newAddress();
			process->listenerMap[addr] = Reference<IListener>(new Sim2Listener(process, addr));
			addressMap[addr] = process;
			handlerContext->addAddress(addr);
			serverContext->registerNewServer(addr, handlerContext->requestHandler->clone());
		} catch (Error& e) {
			// this should never happen, but would cause weird behavior if it did like unintentionally switching
			// processes, so just fail
			TraceEvent(SevError, "UnexpectedErrorRegisteringHTTPServer").errorUnsuppressed(e);
			ASSERT(false);
		}
	}

	// add a simulated http server process. New http servers called by registerHTTPServer will run on this process
	void addSimHTTPProcess(Reference<HTTP::SimServerContext> context) override {
		ProcessInfo* p = getCurrentProcess();

		if (!g_simulator->httpProtected) {
			// always protect one http server process so that if others are killed permanently, one will always be
			// rebooted
			fmt::print("SimHTTPServer protecting {0}\n", p->address.toString());
			TraceEvent(SevDebug, "HTTPProcessProtected").detail("Address", p->address);
			g_simulator->httpProtected = true;
			protectedAddresses.insert(p->address);
		}
		// make sure this process isn't already added
		for (int i = 0; i < httpServerProcesses.size(); i++) {
			ASSERT(p != httpServerProcesses[i].first);
		}
		httpServerProcesses.push_back({ p, context });
		httpServerIps.insert(p->address.ip);

		for (auto& it : httpHandlers) {
			startRequestHandlerOnProcess(p, context, it.second);
		}
	}

	void removeSimHTTPProcess() override {
		ProcessInfo* p = getCurrentProcess();

		bool found = false;
		for (int i = 0; i < httpServerProcesses.size(); i++) {
			if (p == httpServerProcesses[i].first) {
				swapAndPop(&httpServerProcesses, i);
				found = true;
				break;
			}
		}
		ASSERT(found);

		// FIXME: potentially instead delay removing from DNS for a bit so we still briefly try to talk to dead server
		for (auto& it : httpHandlers) {
			it.second->removeIp(p->address.ip);
		}
	}

																#line 8254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via registerSimHTTPServerActor()
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class RegisterSimHTTPServerActorActor>
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class RegisterSimHTTPServerActorActorState {
															#line 8260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	RegisterSimHTTPServerActorActorState(Sim2* const& self,std::string const& hostname,std::string const& service,Reference<HTTP::IRequestHandler> const& requestHandler) 
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   hostname(hostname),
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   service(service),
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   requestHandler(requestHandler)
															#line 8273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
	{
		fdb_probe_actor_create("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this));

	}
	~RegisterSimHTTPServerActorActorState() 
	{
		fdb_probe_actor_destroy("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			std::string id = hostname + ":" + service;
															#line 2541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(!self->httpHandlers.count(id));
															#line 2543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			handlerContext = makeReference<HTTP::SimRegisteredHandlerContext>(hostname, service, requestHandler);
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			self->httpHandlers.insert({ id, handlerContext });
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			callingProcess = self->getCurrentProcess();
															#line 2549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			i = 0;
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			procsCopy = self->httpServerProcesses;
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			;
															#line 8302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
		this->~RegisterSimHTTPServerActorActorState();
		static_cast<RegisterSimHTTPServerActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->onProcess(callingProcess, TaskPriority::DefaultYield);
															#line 2559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state = 2;
															#line 2559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >*>(static_cast<RegisterSimHTTPServerActorActor*>(this)));
															#line 8332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!(i < procsCopy.size()))
															#line 8348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 2554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		serverProcess = procsCopy[i].first;
															#line 2555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->onProcess(serverProcess, TaskPriority::DefaultYield);
															#line 2555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state = 1;
															#line 2555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >*>(static_cast<RegisterSimHTTPServerActorActor*>(this)));
															#line 8363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 2556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->startRequestHandlerOnProcess(serverProcess, procsCopy[i].second, handlerContext);
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		i++;
															#line 8387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 2556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->startRequestHandlerOnProcess(serverProcess, procsCopy[i].second, handlerContext);
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		i++;
															#line 8398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
		if (static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state > 0) static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state = 0;
		static_cast<RegisterSimHTTPServerActorActor*>(this)->ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 2561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<RegisterSimHTTPServerActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RegisterSimHTTPServerActorActorState(); static_cast<RegisterSimHTTPServerActorActor*>(this)->destroy(); return 0; }
															#line 8470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<RegisterSimHTTPServerActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RegisterSimHTTPServerActorActorState();
		static_cast<RegisterSimHTTPServerActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 2561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<RegisterSimHTTPServerActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RegisterSimHTTPServerActorActorState(); static_cast<RegisterSimHTTPServerActorActor*>(this)->destroy(); return 0; }
															#line 8482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<RegisterSimHTTPServerActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RegisterSimHTTPServerActorActorState();
		static_cast<RegisterSimHTTPServerActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state > 0) static_cast<RegisterSimHTTPServerActorActor*>(this)->actor_wait_state = 0;
		static_cast<RegisterSimHTTPServerActorActor*>(this)->ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Sim2* self;
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::string hostname;
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::string service;
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<HTTP::IRequestHandler> requestHandler;
															#line 2543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<HTTP::SimRegisteredHandlerContext> handlerContext;
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ProcessInfo* callingProcess;
															#line 2549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	int i;
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::vector<std::pair<ProcessInfo*, Reference<HTTP::SimServerContext>>> procsCopy;
															#line 2554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ProcessInfo* serverProcess;
															#line 8571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via registerSimHTTPServerActor()
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class RegisterSimHTTPServerActorActor final : public Actor<Void>, public ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >, public ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >, public FastAllocated<RegisterSimHTTPServerActorActor>, public RegisterSimHTTPServerActorActorState<RegisterSimHTTPServerActorActor> {
															#line 8576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<RegisterSimHTTPServerActorActor>::operator new;
	using FastAllocated<RegisterSimHTTPServerActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >;
friend struct ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >;
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	RegisterSimHTTPServerActorActor(Sim2* const& self,std::string const& hostname,std::string const& service,Reference<HTTP::IRequestHandler> const& requestHandler) 
															#line 8588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		 : Actor<Void>(),
		   RegisterSimHTTPServerActorActorState<RegisterSimHTTPServerActorActor>(self, hostname, service, requestHandler)
	{
		fdb_probe_actor_enter("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("registerSimHTTPServerActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("registerSimHTTPServerActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RegisterSimHTTPServerActorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RegisterSimHTTPServerActorActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> registerSimHTTPServerActor( Sim2* const& self, std::string const& hostname, std::string const& service, Reference<HTTP::IRequestHandler> const& requestHandler ) {
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new RegisterSimHTTPServerActorActor(self, hostname, service, requestHandler));
															#line 8616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 2563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

	// starts a numAddresses http servers with the dns alias hostname:service with the provided server callback
	Future<Void> registerSimHTTPServer(std::string hostname,
	                                   std::string service,
	                                   Reference<HTTP::IRequestHandler> requestHandler) override {
		return registerSimHTTPServerActor(this, hostname, service, requestHandler);
	}

	Sim2(bool printSimTime)
	  : time(0.0), timerTime(0.0), currentTaskID(TaskPriority::Zero), yielded(false), yield_limit(0),
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
		// create a key pair for AuthZ testing
		auto key = mkcert::makeEcP256();
		authKeys.insert(std::make_pair(Standalone<StringRef>("DefaultKey"_sr), key));
		g_network = net2 = newNet2(TLSConfig(), false, true);
		g_network->addStopCallback(Net2FileSystem::stop);
		Net2FileSystem::newFileSystem();
		check_yield(TaskPriority::Zero);
	}

	// Implementation
	struct PromiseTask final : public FastAllocated<PromiseTask> {
		Promise<Void> promise;
		ProcessInfo* machine;
		explicit PromiseTask(ProcessInfo* machine) : machine(machine) {}
		PromiseTask(ProcessInfo* machine, Promise<Void>&& promise) : machine(machine), promise(std::move(promise)) {}
	};

	void execTask(struct PromiseTask& t) {
		if (t.machine->failed) {
			t.promise.send(Never());
		} else {
			this->currentProcess = t.machine;
			try {
				t.promise.send(Void());
				ASSERT(this->currentProcess == t.machine);
			} catch (Error& e) {
				TraceEvent(SevError, "UnhandledSimulationEventError").errorUnsuppressed(e);
				killProcess(t.machine, KillType::KillInstantly);
			}

			if (randLog)
				fmt::print(randLog,
				           "T {0} {1} {2}\n",
				           this->time,
				           int(deterministicRandom()->peek() % 10000),
				           t.machine ? t.machine->name : "none");
		}
	}

	void onMainThread(Promise<Void>&& signal, TaskPriority taskID) override {
		// This is presumably coming from either a "fake" thread pool thread, i.e. it is actually on this thread
		// or a thread created with g_network->startThread
		ASSERT(getCurrentProcess());
		ASSERT(taskID >= TaskPriority::Min && taskID <= TaskPriority::Max);

		PromiseTask* p = new PromiseTask(getCurrentProcess(), std::move(signal));
		taskQueue.addReadyThreadSafe(isOnMainThread(), taskID, p);
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

	double time;
	double timerTime;
	TaskPriority currentTaskID;

	std::map<Optional<Standalone<StringRef>>, MachineInfo> machines;
	std::map<NetworkAddress, ProcessInfo*> addressMap;
	std::map<ProcessInfo*, Promise<Void>> filesDeadMap;

	TaskQueue<PromiseTask> taskQueue;

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
	static bool randomDropPacket() {
		auto res = deterministicRandom()->random01() < .000005;
		CODE_PROBE(res, "UDP packet drop", probe::context::sim2, probe::assert::simOnly);
		return res;
	}

	bool isClosed() const { return closed.getFuture().isReady(); }
	Future<Void> onClosed() const { return closed.getFuture(); }

																#line 8752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via cleanupPeerSocket()
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class CleanupPeerSocketActor>
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class CleanupPeerSocketActorState {
															#line 8758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	CleanupPeerSocketActorState(UDPSimSocket* const& self) 
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self)
															#line 8765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->peerSocket.get()->onClosed();
															#line 2696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<CleanupPeerSocketActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanupPeerSocketActor*>(this)->actor_wait_state = 1;
															#line 2696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanupPeerSocketActor, 0, Void >*>(static_cast<CleanupPeerSocketActor*>(this)));
															#line 8787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->peerSocket.reset();
															#line 2698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CleanupPeerSocketActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupPeerSocketActorState(); static_cast<CleanupPeerSocketActor*>(this)->destroy(); return 0; }
															#line 8812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<CleanupPeerSocketActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CleanupPeerSocketActorState();
		static_cast<CleanupPeerSocketActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->peerSocket.reset();
															#line 2698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<CleanupPeerSocketActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupPeerSocketActorState(); static_cast<CleanupPeerSocketActor*>(this)->destroy(); return 0; }
															#line 8826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UDPSimSocket* self;
															#line 8899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via cleanupPeerSocket()
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class CleanupPeerSocketActor final : public Actor<Void>, public ActorCallback< CleanupPeerSocketActor, 0, Void >, public FastAllocated<CleanupPeerSocketActor>, public CleanupPeerSocketActorState<CleanupPeerSocketActor> {
															#line 8904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<CleanupPeerSocketActor>::operator new;
	using FastAllocated<CleanupPeerSocketActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CleanupPeerSocketActor, 0, Void >;
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	CleanupPeerSocketActor(UDPSimSocket* const& self) 
															#line 8915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> cleanupPeerSocket( UDPSimSocket* const& self ) {
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new CleanupPeerSocketActor(self));
															#line 8942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 2700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

																#line 8947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via send()
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class SendActor>
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class SendActorState {
															#line 8953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SendActorState(UDPSimSocket* const& self,Reference<UDPSimSocket> const& peerSocket,uint8_t const* const& begin,uint8_t const* const& end) 
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   peerSocket(peerSocket),
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   begin(begin),
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   end(end),
															#line 2705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   packet(std::make_shared<std::vector<uint8_t>>())
															#line 8968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			packet->resize(end - begin);
															#line 2707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			std::copy(begin, end, packet->begin());
															#line 2708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(.002 * deterministicRandom()->random01());
															#line 2708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<SendActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendActor*>(this)->actor_wait_state = 1;
															#line 2708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendActor, 0, Void >*>(static_cast<SendActor*>(this)));
															#line 8994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		peerSocket->recvBuffer.emplace_back(self->_localAddress, std::move(packet));
															#line 2710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		peerSocket->writtenPackets.set(peerSocket->writtenPackets.get() + 1);
															#line 2711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<SendActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendActorState(); static_cast<SendActor*>(this)->destroy(); return 0; }
															#line 9021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<SendActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendActorState();
		static_cast<SendActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		peerSocket->recvBuffer.emplace_back(self->_localAddress, std::move(packet));
															#line 2710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		peerSocket->writtenPackets.set(peerSocket->writtenPackets.get() + 1);
															#line 2711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<SendActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendActorState(); static_cast<SendActor*>(this)->destroy(); return 0; }
															#line 9037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UDPSimSocket* self;
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Reference<UDPSimSocket> peerSocket;
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	uint8_t const* begin;
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	uint8_t const* end;
															#line 2705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	Packet packet;
															#line 9118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via send()
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class SendActor final : public Actor<Void>, public ActorCallback< SendActor, 0, Void >, public FastAllocated<SendActor>, public SendActorState<SendActor> {
															#line 9123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<SendActor>::operator new;
	using FastAllocated<SendActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendActor, 0, Void >;
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	SendActor(UDPSimSocket* const& self,Reference<UDPSimSocket> const& peerSocket,uint8_t const* const& begin,uint8_t const* const& end) 
															#line 9134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<Void> send( UDPSimSocket* const& self, Reference<UDPSimSocket> const& peerSocket, uint8_t const* const& begin, uint8_t const* const& end ) {
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new SendActor(self, peerSocket, begin, end));
															#line 9161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 2713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

																#line 9166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
// This generated class is to be used only via receiveFrom()
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class ReceiveFromActor>
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class ReceiveFromActorState {
															#line 9172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ReceiveFromActorState(UDPSimSocket* const& self,uint8_t* const& begin,uint8_t* const& end,NetworkAddress* const& sender) 
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : self(self),
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   begin(begin),
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   end(end),
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   sender(sender),
															#line 2715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   currentTaskID(g_sim2.getCurrentTask())
															#line 9187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = self->writtenPackets.onChange();
															#line 2716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<ReceiveFromActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReceiveFromActor*>(this)->actor_wait_state = 1;
															#line 2716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReceiveFromActor, 0, Void >*>(static_cast<ReceiveFromActor*>(this)));
															#line 9209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_sim2.onProcess(self->process, currentTaskID);
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiveFromActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReceiveFromActor*>(this)->actor_wait_state = 2;
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReceiveFromActor, 1, Void >*>(static_cast<ReceiveFromActor*>(this)));
															#line 9239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_sim2.onProcess(self->process, currentTaskID);
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<ReceiveFromActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReceiveFromActor*>(this)->actor_wait_state = 2;
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReceiveFromActor, 1, Void >*>(static_cast<ReceiveFromActor*>(this)));
															#line 9255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		auto packet = self->recvBuffer.front().second;
															#line 2719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		int sz = packet->size();
															#line 2720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(sz <= end - begin);
															#line 2721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (sender)
															#line 9333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 2722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			*sender = self->recvBuffer.front().first;
															#line 9337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 2724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		std::copy(packet->begin(), packet->end(), begin);
															#line 2725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->recvBuffer.pop_front();
															#line 2726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<ReceiveFromActor*>(this)->SAV<int>::futures) { (void)(sz); this->~ReceiveFromActorState(); static_cast<ReceiveFromActor*>(this)->destroy(); return 0; }
															#line 9345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<ReceiveFromActor*>(this)->SAV< int >::value()) int(sz);
		this->~ReceiveFromActorState();
		static_cast<ReceiveFromActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 2718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		auto packet = self->recvBuffer.front().second;
															#line 2719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		int sz = packet->size();
															#line 2720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		ASSERT(sz <= end - begin);
															#line 2721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (sender)
															#line 9363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		{
															#line 2722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			*sender = self->recvBuffer.front().first;
															#line 9367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 2724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		std::copy(packet->begin(), packet->end(), begin);
															#line 2725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		self->recvBuffer.pop_front();
															#line 2726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<ReceiveFromActor*>(this)->SAV<int>::futures) { (void)(sz); this->~ReceiveFromActorState(); static_cast<ReceiveFromActor*>(this)->destroy(); return 0; }
															#line 9375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	UDPSimSocket* self;
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	uint8_t* begin;
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	uint8_t* end;
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	NetworkAddress* sender;
															#line 2715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	TaskPriority currentTaskID;
															#line 9456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via receiveFrom()
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class ReceiveFromActor final : public Actor<int>, public ActorCallback< ReceiveFromActor, 0, Void >, public ActorCallback< ReceiveFromActor, 1, Void >, public FastAllocated<ReceiveFromActor>, public ReceiveFromActorState<ReceiveFromActor> {
															#line 9461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<ReceiveFromActor>::operator new;
	using FastAllocated<ReceiveFromActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReceiveFromActor, 0, Void >;
friend struct ActorCallback< ReceiveFromActor, 1, Void >;
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ReceiveFromActor(UDPSimSocket* const& self,uint8_t* const& begin,uint8_t* const& end,NetworkAddress* const& sender) 
															#line 9473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] static Future<int> receiveFrom( UDPSimSocket* const& self, uint8_t* const& begin, uint8_t* const& end, NetworkAddress* const& sender ) {
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<int>(new ReceiveFromActor(self, begin, end, sender));
															#line 9501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 2728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

public:
	UDPSimSocket(NetworkAddress const& localAddress, Optional<NetworkAddress> const& peerAddress)
	  : id(deterministicRandom()->randomUniqueID()), process(g_simulator->getCurrentProcess()),
	    peerAddress(peerAddress), actors(false), _localAddress(localAddress) {
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
	auto process = g_simulator->getCurrentProcess();
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
	auto process = g_simulator->getCurrentProcess();
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
	g_network = g_simulator = new Sim2(printSimTime);
	g_simulator->connectionFailuresDisableDuration =
	    deterministicRandom()->coinflip() ? 0 : DISABLE_CONNECTION_FAILURE_FOREVER;
}

															#line 9657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
namespace {
// This generated class is to be used only via doReboot()
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class DoRebootActor>
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class DoRebootActorState {
															#line 9664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	DoRebootActorState(ISimulator::ProcessInfo* const& p,ISimulator::KillType const& kt) 
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : p(p),
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   kt(kt)
															#line 9673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent("RebootingProcessAttempt") .detail("ZoneId", p->locality.zoneId()) .detail("KillType", kt) .detail("Process", p->toString()) .detail("StartingClass", p->startingClass.toString()) .detail("Failed", p->failed) .detail("Excluded", p->excluded) .detail("Cleared", p->cleared) .detail("Rebooting", p->rebooting) .detail("TaskPriorityDefaultDelay", TaskPriority::DefaultDelay);
															#line 2892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_sim2.delay(0, TaskPriority::DefaultDelay, p);
															#line 2892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 9692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			static_cast<DoRebootActor*>(this)->actor_wait_state = 1;
															#line 2892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRebootActor, 0, Void >*>(static_cast<DoRebootActor*>(this)));
															#line 9696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(kt == ISimulator::KillType::RebootProcess || kt == ISimulator::KillType::Reboot || kt == ISimulator::KillType::RebootAndDelete || kt == ISimulator::KillType::RebootProcessAndDelete || kt == ISimulator::KillType::RebootProcessAndSwitch);
															#line 2899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootProcess, "Simulated process rebooted", probe::assert::simOnly, probe::context::sim2);
															#line 2903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::Reboot, "Simulated machine rebooted", probe::assert::simOnly, probe::context::sim2);
															#line 2907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootAndDelete, "Simulated machine rebooted with data and coordination state deletion", probe::assert::simOnly, probe::context::sim2);
															#line 2911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootProcessAndDelete, "Simulated process rebooted with data and coordination state deletion", probe::assert::simOnly, probe::context::sim2);
															#line 2915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootProcessAndSwitch, "Simulated process rebooted with different cluster file", probe::assert::simOnly, probe::context::sim2);
															#line 2920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (p->rebooting || !p->isReliable())
															#line 9731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "DoRebootFailed") .detail("Rebooting", p->rebooting) .detail("Reliable", p->isReliable());
															#line 2924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				delete static_cast<DoRebootActor*>(this);
															#line 9737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				return 0;
			}
			else
			{
															#line 2925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (p->isSpawnedKVProcess())
															#line 9744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				{
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					TraceEvent(SevDebug, "DoRebootFailed").detail("Name", p->name).detail("Address", p->address);
															#line 2927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					delete static_cast<DoRebootActor*>(this);
															#line 9750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
					return 0;
				}
				else
				{
															#line 2928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					if (p->getChilds().size())
															#line 9757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
					{
															#line 2929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
						TraceEvent(SevDebug, "DoRebootFailedOnParentProcess").detail("Address", p->address);
															#line 2930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
						delete static_cast<DoRebootActor*>(this);
															#line 9763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
						return 0;
					}
				}
			}
															#line 2933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent("RebootingProcess") .detail("KillType", kt) .detail("Address", p->address) .detail("ZoneId", p->locality.zoneId()) .detail("DataHall", p->locality.dataHallId()) .detail("Locality", p->locality.toString()) .detail("Failed", p->failed) .detail("Excluded", p->excluded) .detail("Cleared", p->cleared) .backtrace();
															#line 2943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			p->rebooting = true;
															#line 2944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if ((kt == ISimulator::KillType::RebootAndDelete) || (kt == ISimulator::KillType::RebootProcessAndDelete))
															#line 9774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				p->cleared = true;
															#line 2946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				g_simulator->clearAddress(p->address);
															#line 9780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
			else
			{
															#line 2947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (kt == ISimulator::KillType::RebootProcessAndSwitch)
															#line 9786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				{
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					g_simulator->switchCluster(p->address);
															#line 9790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				}
			}
															#line 2950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			p->shutdownSignal.send(kt);
															#line 9795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1cont12(loopDepth);
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
															#line 2895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			ASSERT(kt == ISimulator::KillType::RebootProcess || kt == ISimulator::KillType::Reboot || kt == ISimulator::KillType::RebootAndDelete || kt == ISimulator::KillType::RebootProcessAndDelete || kt == ISimulator::KillType::RebootProcessAndSwitch);
															#line 2899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootProcess, "Simulated process rebooted", probe::assert::simOnly, probe::context::sim2);
															#line 2903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::Reboot, "Simulated machine rebooted", probe::assert::simOnly, probe::context::sim2);
															#line 2907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootAndDelete, "Simulated machine rebooted with data and coordination state deletion", probe::assert::simOnly, probe::context::sim2);
															#line 2911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootProcessAndDelete, "Simulated process rebooted with data and coordination state deletion", probe::assert::simOnly, probe::context::sim2);
															#line 2915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			CODE_PROBE(kt == ISimulator::KillType::RebootProcessAndSwitch, "Simulated process rebooted with different cluster file", probe::assert::simOnly, probe::context::sim2);
															#line 2920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (p->rebooting || !p->isReliable())
															#line 9823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				TraceEvent(SevDebug, "DoRebootFailed") .detail("Rebooting", p->rebooting) .detail("Reliable", p->isReliable());
															#line 2924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				delete static_cast<DoRebootActor*>(this);
															#line 9829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				return 0;
			}
			else
			{
															#line 2925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (p->isSpawnedKVProcess())
															#line 9836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				{
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					TraceEvent(SevDebug, "DoRebootFailed").detail("Name", p->name).detail("Address", p->address);
															#line 2927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					delete static_cast<DoRebootActor*>(this);
															#line 9842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
					return 0;
				}
				else
				{
															#line 2928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					if (p->getChilds().size())
															#line 9849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
					{
															#line 2929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
						TraceEvent(SevDebug, "DoRebootFailedOnParentProcess").detail("Address", p->address);
															#line 2930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
						delete static_cast<DoRebootActor*>(this);
															#line 9855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
						return 0;
					}
				}
			}
															#line 2933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent("RebootingProcess") .detail("KillType", kt) .detail("Address", p->address) .detail("ZoneId", p->locality.zoneId()) .detail("DataHall", p->locality.dataHallId()) .detail("Locality", p->locality.toString()) .detail("Failed", p->failed) .detail("Excluded", p->excluded) .detail("Cleared", p->cleared) .backtrace();
															#line 2943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			p->rebooting = true;
															#line 2944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if ((kt == ISimulator::KillType::RebootAndDelete) || (kt == ISimulator::KillType::RebootProcessAndDelete))
															#line 9866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			{
															#line 2945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				p->cleared = true;
															#line 2946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				g_simulator->clearAddress(p->address);
															#line 9872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			}
			else
			{
															#line 2947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
				if (kt == ISimulator::KillType::RebootProcessAndSwitch)
															#line 9878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				{
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
					g_simulator->switchCluster(p->address);
															#line 9882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
				}
			}
															#line 2950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			p->shutdownSignal.send(kt);
															#line 9887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			loopDepth = a_body1cont12(loopDepth);
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
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 2952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			TraceEvent(SevError, "RebootError").error(e);
															#line 2953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			p->shutdownSignal.sendError(e);
															#line 2954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 9976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont12(int loopDepth) 
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
	int a_body1cont13(int loopDepth) 
	{
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		delete static_cast<DoRebootActor*>(this);
															#line 10003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ISimulator::ProcessInfo* p;
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	ISimulator::KillType kt;
															#line 10012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via doReboot()
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class DoRebootActor final : public Actor<void>, public ActorCallback< DoRebootActor, 0, Void >, public FastAllocated<DoRebootActor>, public DoRebootActorState<DoRebootActor> {
															#line 10017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<DoRebootActor>::operator new;
	using FastAllocated<DoRebootActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoRebootActor, 0, Void >;
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	DoRebootActor(ISimulator::ProcessInfo* const& p,ISimulator::KillType const& kt) 
															#line 10028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
void doReboot( ISimulator::ProcessInfo* const& p, ISimulator::KillType const& kt ) {
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	new DoRebootActor(p, kt);
															#line 10047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 2957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

// Simulates delays for performing operations on disk
Future<Void> waitUntilDiskReady(Reference<DiskParameters> diskParameters, int64_t size, bool sync) {
	if (g_simulator->getCurrentProcess()->failedDisk) {
		return Never();
	}
	if (g_simulator->connectionFailuresDisableDuration > 1e4)
		return delay(0.0001);

	if (diskParameters->nextOperation < now())
		diskParameters->nextOperation = now();
	diskParameters->nextOperation += (1.0 / diskParameters->iops) + (size / diskParameters->bandwidth);

	double randomLatency;
	if (sync) {
		randomLatency = .005 + deterministicRandom()->random01() * (BUGGIFY ? 1.0 : .010);
	} else
		randomLatency = 10 * deterministicRandom()->random01() / diskParameters->iops;

	return delayUntil(diskParameters->nextOperation + randomLatency);
}

void enableConnectionFailures(std::string const& context) {
	if (g_network->isSimulated()) {
		g_simulator->connectionFailuresDisableDuration = 0;
		g_simulator->speedUpSimulation = false;
		g_simulator->connectionFailureEnableTime = now();
		TraceEvent(SevWarnAlways, ("EnableConnectionFailures_" + context).c_str());
	}
}

void disableConnectionFailures(std::string const& context) {
	if (g_network->isSimulated()) {
		g_simulator->connectionFailuresDisableDuration = DISABLE_CONNECTION_FAILURE_FOREVER;
		g_simulator->speedUpSimulation = true;
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
	           ".fdb-lock"_sr)); // We don't use "ordinary" non-atomic file creation right now except for
	                             // folder locking, and we don't have code to simulate its unsafeness.

	if ((flags & IAsyncFile::OPEN_EXCLUSIVE))
		ASSERT(flags & IAsyncFile::OPEN_CREATE);

	if (flags & IAsyncFile::OPEN_UNCACHED) {
		auto& machineCache = g_simulator->getCurrentProcess()->machine->openFiles;
		std::string actualFilename = filename;
		if (flags & IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE) {
			actualFilename = filename + ".part";
			auto partFile = machineCache.find(actualFilename);
			if (partFile != machineCache.end()) {
				Future<Reference<IAsyncFile>> f = AsyncFileDetachable::open(partFile->second.get());
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

			f = SimpleFile::open(filename, flags, mode, diskParameters, false);
			if (FLOW_KNOBS->PAGE_WRITE_CHECKSUM_HISTORY > 0) {
				f = map(f,
				        [=](Reference<IAsyncFile> r) { return Reference<IAsyncFile>(new AsyncFileWriteChecker(r)); });
			}

			f = AsyncFileNonDurable::open(
			    filename, actualFilename, f, diskParameters, (flags & IAsyncFile::OPEN_NO_AIO) == 0);

			machineCache[actualFilename] = UnsafeWeakFutureReference<IAsyncFile>(f);
		} else {
			f = itr->second.get();
		}

		f = AsyncFileDetachable::open(f);
		if (FLOW_KNOBS->ENABLE_CHAOS_FEATURES)
			f = map(f, [=](Reference<IAsyncFile> r) { return Reference<IAsyncFile>(new AsyncFileChaos(r)); });
		if (flags & IAsyncFile::OPEN_ENCRYPTED)
			f = map(f, [flags](Reference<IAsyncFile> r) {
				auto mode = flags & IAsyncFile::OPEN_READWRITE ? AsyncFileEncrypted::Mode::APPEND_ONLY
				                                               : AsyncFileEncrypted::Mode::READ_ONLY;
				return Reference<IAsyncFile>(new AsyncFileEncrypted(r, mode));
			});
		return f;
	} else
		return AsyncFileCached::open(filename, flags, mode);
}

// Deletes the given file.  If mustBeDurable, returns only when the file is guaranteed to be deleted even after a power
// failure.
Future<Void> Sim2FileSystem::deleteFile(const std::string& filename, bool mustBeDurable) {
	return Sim2::deleteFileImpl(&g_sim2, filename, mustBeDurable);
}

															#line 10182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
namespace {
// This generated class is to be used only via renameFileImpl()
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
template <class RenameFileImplActor>
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class RenameFileImplActorState {
															#line 10189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	RenameFileImplActorState(std::string const& from,std::string const& to) 
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		 : from(from),
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		   to(to)
															#line 10198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0.5 * deterministicRandom()->random01());
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			if (static_cast<RenameFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RenameFileImplActor*>(this)->actor_wait_state = 1;
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RenameFileImplActor, 0, Void >*>(static_cast<RenameFileImplActor*>(this)));
															#line 10220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 3092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		TraceEvent("RenamingFile").detail("From", from).detail("To", to).log();
															#line 3094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		using block_value_type = typename decltype(g_simulator->corruptedBlocks)::key_type::second_type;
															#line 3095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		auto maxBlockValue = std::numeric_limits<block_value_type>::max();
															#line 3096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		g_simulator->corruptedBlocks.erase(g_simulator->corruptedBlocks.lower_bound(std::make_pair(to, 0u)), g_simulator->corruptedBlocks.upper_bound(std::make_pair(to, maxBlockValue)));
															#line 3099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		auto begin = g_simulator->corruptedBlocks.lower_bound(std::make_pair(from, 0u)), end = g_simulator->corruptedBlocks.upper_bound(std::make_pair(from, maxBlockValue));
															#line 3101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		for(auto iter = begin;iter != end;++iter) {
															#line 3102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			g_simulator->corruptedBlocks.emplace(to, iter->second);
															#line 10255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 3104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		g_simulator->corruptedBlocks.erase(begin, end);
															#line 3106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		::renameFile(from, to);
															#line 3107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0.5 * deterministicRandom()->random01());
															#line 3107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<RenameFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RenameFileImplActor*>(this)->actor_wait_state = 2;
															#line 3107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameFileImplActor, 1, Void >*>(static_cast<RenameFileImplActor*>(this)));
															#line 10270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 3092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		TraceEvent("RenamingFile").detail("From", from).detail("To", to).log();
															#line 3094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		using block_value_type = typename decltype(g_simulator->corruptedBlocks)::key_type::second_type;
															#line 3095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		auto maxBlockValue = std::numeric_limits<block_value_type>::max();
															#line 3096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		g_simulator->corruptedBlocks.erase(g_simulator->corruptedBlocks.lower_bound(std::make_pair(to, 0u)), g_simulator->corruptedBlocks.upper_bound(std::make_pair(to, maxBlockValue)));
															#line 3099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		auto begin = g_simulator->corruptedBlocks.lower_bound(std::make_pair(from, 0u)), end = g_simulator->corruptedBlocks.upper_bound(std::make_pair(from, maxBlockValue));
															#line 3101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		for(auto iter = begin;iter != end;++iter) {
															#line 3102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
			g_simulator->corruptedBlocks.emplace(to, iter->second);
															#line 10291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		}
															#line 3104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		g_simulator->corruptedBlocks.erase(begin, end);
															#line 3106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		::renameFile(from, to);
															#line 3107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0.5 * deterministicRandom()->random01());
															#line 3107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (static_cast<RenameFileImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RenameFileImplActor*>(this)->actor_wait_state = 2;
															#line 3107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameFileImplActor, 1, Void >*>(static_cast<RenameFileImplActor*>(this)));
															#line 10306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 3108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<RenameFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameFileImplActorState(); static_cast<RenameFileImplActor*>(this)->destroy(); return 0; }
															#line 10378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
		new (&static_cast<RenameFileImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameFileImplActorState();
		static_cast<RenameFileImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 3108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
		if (!static_cast<RenameFileImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameFileImplActorState(); static_cast<RenameFileImplActor*>(this)->destroy(); return 0; }
															#line 10390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::string from;
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	std::string to;
															#line 10465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
};
// This generated class is to be used only via renameFileImpl()
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
class RenameFileImplActor final : public Actor<Void>, public ActorCallback< RenameFileImplActor, 0, Void >, public ActorCallback< RenameFileImplActor, 1, Void >, public FastAllocated<RenameFileImplActor>, public RenameFileImplActorState<RenameFileImplActor> {
															#line 10470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
public:
	using FastAllocated<RenameFileImplActor>::operator new;
	using FastAllocated<RenameFileImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RenameFileImplActor, 0, Void >;
friend struct ActorCallback< RenameFileImplActor, 1, Void >;
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	RenameFileImplActor(std::string const& from,std::string const& to) 
															#line 10482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
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
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
[[nodiscard]] Future<Void> renameFileImpl( std::string const& from, std::string const& to ) {
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"
	return Future<Void>(new RenameFileImplActor(from, to));
															#line 10511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.g.cpp"
}

#line 3110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/sim2.actor.cpp"

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
