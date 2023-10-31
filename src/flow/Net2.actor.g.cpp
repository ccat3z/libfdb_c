#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
/*
 * Net2.actor.cpp
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

#include "boost/asio/buffer.hpp"
#include "boost/asio/ip/address.hpp"
#include "boost/system/system_error.hpp"
#include "flow/Arena.h"
#include "flow/Platform.h"
#include "flow/Trace.h"
#include <algorithm>
#include <memory>
#ifndef BOOST_SYSTEM_NO_LIB
#define BOOST_SYSTEM_NO_LIB
#endif
#ifndef BOOST_DATE_TIME_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#endif
#ifndef BOOST_REGEX_NO_LIB
#define BOOST_REGEX_NO_LIB
#endif
#include <boost/asio.hpp>
#include "boost/asio/ssl.hpp"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string/join.hpp>
#include "flow/network.h"
#include "flow/IThreadPool.h"

#include "flow/IAsyncFile.h"
#include "flow/ActorCollection.h"
#include "flow/TaskQueue.h"
#include "flow/ThreadHelper.actor.h"
#include "flow/ChaosMetrics.h"
#include "flow/TDMetric.actor.h"
#include "flow/AsioReactor.h"
#include "flow/Profiler.h"
#include "flow/ProtocolVersion.h"
#include "flow/SendBufferIterator.h"
#include "flow/TLSConfig.actor.h"
#include "flow/WatchFile.actor.h"
#include "flow/genericactors.actor.h"
#include "flow/Util.h"
#include "flow/UnitTest.h"
#include "flow/ScopeExit.h"
#include "flow/IUDPSocket.h"
#include "flow/IConnection.h"

#ifdef ADDRESS_SANITIZER
#include <sanitizer/lsan_interface.h>
#endif

#ifdef WIN32
#include <mmsystem.h>
#endif
#include "flow/actorcompiler.h" // This must be the last #include.

// Defined to track the stack limit
extern "C" intptr_t g_stackYieldLimit;
intptr_t g_stackYieldLimit = 0;

using namespace boost::asio::ip;

#if defined(__linux__) || defined(__FreeBSD__)
#include <execinfo.h>

std::atomic<int64_t> net2RunLoopIterations(0);
std::atomic<int64_t> net2RunLoopSleeps(0);

volatile size_t net2backtraces_max = 10000;
volatile void** volatile net2backtraces = nullptr;
volatile size_t net2backtraces_offset = 0;
volatile bool net2backtraces_overflow = false;
volatile int net2backtraces_count = 0;

volatile void** other_backtraces = nullptr;
sigset_t sigprof_set;

void initProfiling() {
	net2backtraces = new volatile void*[net2backtraces_max];
	other_backtraces = new volatile void*[net2backtraces_max];

	// According to folk wisdom, calling this once before setting up the signal handler makes
	// it async signal safe in practice :-/
	backtrace(const_cast<void**>(other_backtraces), net2backtraces_max);

	sigemptyset(&sigprof_set);
	sigaddset(&sigprof_set, SIGPROF);
}
#endif

template<> struct Descriptor<struct SlowTask> {
	static StringRef typeName() { return "SlowTask"_sr; }
	typedef SlowTask type;
	struct clocksDescriptor {
		static StringRef name() { return "clocks"_sr; }
		static StringRef typeName() { return "int64_t"_sr; }
		static StringRef comment() { return " clocks"_sr; }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	struct durationDescriptor {
		static StringRef name() { return "duration"_sr; }
		static StringRef typeName() { return "int64_t"_sr; }
		static StringRef comment() { return " ns"_sr; }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	struct priorityDescriptor {
		static StringRef name() { return "priority"_sr; }
		static StringRef typeName() { return "int64_t"_sr; }
		static StringRef comment() { return " priority level"_sr; }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	struct numYieldsDescriptor {
		static StringRef name() { return "numYields"_sr; }
		static StringRef typeName() { return "int64_t"_sr; }
		static StringRef comment() { return " count"_sr; }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	typedef std::tuple<clocksDescriptor,durationDescriptor,priorityDescriptor,numYieldsDescriptor> fields;
	typedef make_index_sequence_impl<0, index_sequence<>, std::tuple_size<fields>::value>::type field_indexes;
};
struct SlowTask {
	int64_t clocks; // clocks
	int64_t duration; // ns
	int64_t priority; // priority level
	int64_t numYields; // count
};
int64_t Descriptor<SlowTask>::clocksDescriptor::get(SlowTask& from) { return from.clocks; }
int64_t Descriptor<SlowTask>::durationDescriptor::get(SlowTask& from) { return from.duration; }
int64_t Descriptor<SlowTask>::priorityDescriptor::get(SlowTask& from) { return from.priority; }
int64_t Descriptor<SlowTask>::numYieldsDescriptor::get(SlowTask& from) { return from.numYields; }
#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"


namespace N2 { // No indent, it's the whole file

class Net2;
class Peer;
class Connection;

// Outlives main
Net2* g_net2 = nullptr;

thread_local INetwork* thread_network = 0;

class Net2 final : public INetwork, public INetworkConnections {

private:
	void updateStarvationTracker(struct NetworkMetrics::PriorityStats& binStats,
	                             TaskPriority priority,
	                             TaskPriority lastPriority,
	                             double now);

public:
	Net2(const TLSConfig& tlsConfig, bool useThreadPool, bool useMetrics);
	void initTLS(ETLSInitState targetState) override;
	void run() override;
	void initMetrics() override;

	// INetworkConnections interface
	Future<Reference<IConnection>> connect(NetworkAddress toAddr, tcp::socket* existingSocket = nullptr) override;
	Future<Reference<IConnection>> connectExternal(NetworkAddress toAddr) override;
	Future<Reference<IUDPSocket>> createUDPSocket(NetworkAddress toAddr) override;
	Future<Reference<IUDPSocket>> createUDPSocket(bool isV6) override;
	// The mock DNS methods should only be used in simulation.
	void addMockTCPEndpoint(const std::string& host,
	                        const std::string& service,
	                        const std::vector<NetworkAddress>& addresses) override {
		throw operation_failed();
	}
	// The mock DNS methods should only be used in simulation.
	void removeMockTCPEndpoint(const std::string& host, const std::string& service) override {
		throw operation_failed();
	}
	void parseMockDNSFromString(const std::string& s) override { throw operation_failed(); }
	std::string convertMockDNSToString() override { throw operation_failed(); }

	Future<std::vector<NetworkAddress>> resolveTCPEndpoint(const std::string& host,
	                                                       const std::string& service) override;
	Future<std::vector<NetworkAddress>> resolveTCPEndpointWithDNSCache(const std::string& host,
	                                                                   const std::string& service) override;
	std::vector<NetworkAddress> resolveTCPEndpointBlocking(const std::string& host,
	                                                       const std::string& service) override;
	std::vector<NetworkAddress> resolveTCPEndpointBlockingWithDNSCache(const std::string& host,
	                                                                   const std::string& service) override;
	Reference<IListener> listen(NetworkAddress localAddr) override;

	// INetwork interface
	double now() const override { return currentTime; };
	double timer() override { return ::timer(); };
	double timer_monotonic() override { return ::timer_monotonic(); };
	Future<Void> delay(double seconds, TaskPriority taskId) override;
	Future<Void> orderedDelay(double seconds, TaskPriority taskId) override;
	Future<class Void> yield(TaskPriority taskID) override;
	bool check_yield(TaskPriority taskId) override;
	TaskPriority getCurrentTask() const override { return currentTaskID; }
	void setCurrentTask(TaskPriority taskID) override {
		currentTaskID = taskID;
		priorityMetric = (int64_t)taskID;
	}
	void onMainThread(Promise<Void>&& signal, TaskPriority taskID) override;
	bool isOnMainThread() const override { return thread_network == this; }
	void stop() override {
		if (thread_network == this)
			stopImmediately();
		else
			onMainThreadVoid([this] { this->stopImmediately(); });
	}
	void addStopCallback(std::function<void()> fn) override {
		if (thread_network == this)
			stopCallbacks.emplace_back(std::move(fn));
		else
			onMainThreadVoid([this, fn] { this->stopCallbacks.emplace_back(std::move(fn)); });
	}

	bool isSimulated() const override { return false; }
	THREAD_HANDLE startThread(THREAD_FUNC_RETURN (*func)(void*), void* arg, int stackSize, const char* name) override;

	void getDiskBytes(std::string const& directory, int64_t& free, int64_t& total) override;
	bool isAddressOnThisHost(NetworkAddress const& addr) const override;
	void updateNow() { currentTime = timer_monotonic(); }

	flowGlobalType global(int id) const override { return (globals.size() > id) ? globals[id] : nullptr; }
	void setGlobal(size_t id, flowGlobalType v) override {
		ASSERT(id < globals.size());
		globals[id] = v;
	}

	ProtocolVersion protocolVersion() const override { return currentProtocolVersion(); }

	std::vector<flowGlobalType> globals;

	const TLSConfig& getTLSConfig() const override { return tlsConfig; }

	bool checkRunnable() override;

#ifdef ENABLE_SAMPLING
	ActorLineageSet& getActorLineageSet() override;
#endif

	bool useThreadPool;

	// private:

	ASIOReactor reactor;
	AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>> sslContextVar;
	Reference<IThreadPool> sslHandshakerPool;
	int sslHandshakerThreadsStarted;
	int sslPoolHandshakesInProgress;
	TLSConfig tlsConfig;
	Reference<TLSPolicy> activeTlsPolicy;
	Future<Void> backgroundCertRefresh;
	ETLSInitState tlsInitializedState;

	INetworkConnections* network; // initially this, but can be changed

	int64_t tscBegin, tscEnd;
	double taskBegin;
	TaskPriority currentTaskID;
	TDMetricCollection tdmetrics;
	MetricCollection metrics;
	ChaosMetrics chaosMetrics;
	// we read now() from a different thread. On Intel, reading a double is atomic anyways, but on other platforms it's
	// not. For portability this should be atomic
	std::atomic<double> currentTime;
	// May be accessed off the network thread, e.g. by onMainThread
	std::atomic<bool> stopped;
	mutable std::map<IPAddress, bool> addressOnHostCache;

#ifdef ENABLE_SAMPLING
	ActorLineageSet actorLineageSet;
#endif

	std::atomic<bool> started;

	uint64_t numYields;

	NetworkMetrics::PriorityStats* lastPriorityStats;

	struct PromiseTask final : public FastAllocated<PromiseTask> {
		Promise<Void> promise;
		PromiseTask() {}
		explicit PromiseTask(Promise<Void>&& promise) noexcept : promise(std::move(promise)) {}

		void operator()() {
			promise.send(Void());
			delete this;
		}
	};

	TaskQueue<PromiseTask> taskQueue;

	void checkForSlowTask(int64_t tscBegin, int64_t tscEnd, double duration, TaskPriority priority);
	bool check_yield(TaskPriority taskId, int64_t tscNow);
	void trackAtPriority(TaskPriority priority, double now);
	void stopImmediately() {
#ifdef ADDRESS_SANITIZER
		// Do leak check before intentionally leaking a bunch of memory
		__lsan_do_leak_check();
#endif
		stopped = true;
		taskQueue.clear();
	}

	Future<Void> timeOffsetLogger;
	Future<Void> logTimeOffset();

	Int64MetricHandle bytesReceived;
	Int64MetricHandle udpBytesReceived;
	Int64MetricHandle countWriteProbes;
	Int64MetricHandle countReadProbes;
	Int64MetricHandle countReads;
	Int64MetricHandle countUDPReads;
	Int64MetricHandle countWouldBlock;
	Int64MetricHandle countWrites;
	Int64MetricHandle countUDPWrites;
	Int64MetricHandle countRunLoop;
	Int64MetricHandle countTasks;
	Int64MetricHandle countYields;
	Int64MetricHandle countYieldBigStack;
	Int64MetricHandle countYieldCalls;
	Int64MetricHandle countYieldCallsTrue;
	Int64MetricHandle countASIOEvents;
	Int64MetricHandle countRunLoopProfilingSignals;
	Int64MetricHandle countTLSPolicyFailures;
	Int64MetricHandle priorityMetric;
	DoubleMetricHandle countLaunchTime;
	DoubleMetricHandle countReactTime;
	BoolMetricHandle awakeMetric;

	EventMetricHandle<SlowTask> slowTaskMetric;

	std::vector<std::string> blobCredentialFiles;
	std::vector<std::function<void()>> stopCallbacks;
};

static boost::asio::ip::address tcpAddress(IPAddress const& n) {
	if (n.isV6()) {
		return boost::asio::ip::address_v6(n.toV6());
	} else {
		return boost::asio::ip::address_v4(n.toV4());
	}
}

static IPAddress toIPAddress(boost::asio::ip::address const& addr) {
	if (addr.is_v4()) {
		return IPAddress(addr.to_v4().to_uint());
	} else {
		return IPAddress(addr.to_v6().to_bytes());
	}
}

static tcp::endpoint tcpEndpoint(NetworkAddress const& n) {
	return tcp::endpoint(tcpAddress(n.ip), n.port);
}

static udp::endpoint udpEndpoint(NetworkAddress const& n) {
	return udp::endpoint(tcpAddress(n.ip), n.port);
}

class BindPromise {
	Promise<Void> p;
	std::variant<const char*, AuditedEvent> errContext;
	UID errID;

public:
	BindPromise(const char* errContext, UID errID) : errContext(errContext), errID(errID) {}
	BindPromise(AuditedEvent auditedEvent, UID errID) : errContext(auditedEvent), errID(errID) {}
	BindPromise(BindPromise const& r) : p(r.p), errContext(r.errContext), errID(r.errID) {}
	BindPromise(BindPromise&& r) noexcept : p(std::move(r.p)), errContext(r.errContext), errID(r.errID) {}

	Future<Void> getFuture() const { return p.getFuture(); }

	void operator()(const boost::system::error_code& error, size_t bytesWritten = 0) {
		try {
			if (error) {
				// Log the error...
				{
					std::optional<TraceEvent> traceEvent;
					if (std::holds_alternative<AuditedEvent>(errContext))
						traceEvent.emplace(SevWarn, std::get<AuditedEvent>(errContext), errID);
					else
						traceEvent.emplace(SevWarn, std::get<const char*>(errContext), errID);
					TraceEvent& evt = *traceEvent;
					evt.suppressFor(1.0).detail("ErrorCode", error.value()).detail("Message", error.message());
					// There is no function in OpenSSL to use to check if an error code is from OpenSSL,
					// but all OpenSSL errors have a non-zero "library" code set in bits 24-32, and linux
					// error codes should never go that high.
					if (error.value() >= (1 << 24L)) {
						evt.detail("WhichMeans", TLSPolicy::ErrorString(error));
					}
				}

				p.sendError(connection_failed());
			} else
				p.send(Void());
		} catch (Error& e) {
			p.sendError(e);
		} catch (...) {
			p.sendError(unknown_error());
		}
	}
};

class Connection final : public IConnection, ReferenceCounted<Connection> {
public:
	void addref() override { ReferenceCounted<Connection>::addref(); }
	void delref() override { ReferenceCounted<Connection>::delref(); }

	void close() override { closeSocket(); }

	explicit Connection(boost::asio::io_service& io_service)
	  : id(nondeterministicRandom()->randomUniqueID()), socket(io_service) {}

	// This is not part of the IConnection interface, because it is wrapped by INetwork::connect()
																#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectActor>
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActorState {
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActorState(boost::asio::io_service* const& ios,NetworkAddress const& addr) 
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : ios(ios),
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   addr(addr),
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   self(new Connection(*ios))
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("connect", reinterpret_cast<unsigned long>(this));

	}
	~ConnectActorState() 
	{
		fdb_probe_actor_destroy("connect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->peer_address = addr;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			try {
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto to = tcpEndpoint(addr);
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				BindPromise p("N2_ConnectError", self->id);
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Future<Void> onConnected = p.getFuture();
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->socket.async_connect(to, std::move(p));
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = onConnected;
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<ConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ConnectActor*>(this)->actor_wait_state = 1;
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor, 0, Void >*>(static_cast<ConnectActor*>(this)));
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				loopDepth = 0;
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
		this->~ConnectActorState();
		static_cast<ConnectActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(self)); // state_var_RVO
		this->~ConnectActorState();
		static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(self)); // state_var_RVO
		this->~ConnectActorState();
		static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConnectActor*>(this)->actor_wait_state > 0) static_cast<ConnectActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectActor*>(this)->ActorCallback< ConnectActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	boost::asio::io_service* ios;
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	NetworkAddress addr;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<Connection> self;
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< ConnectActor, 0, Void >, public FastAllocated<ConnectActor>, public ConnectActorState<ConnectActor> {
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectActor>::operator new;
	using FastAllocated<ConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor, 0, Void >;
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor(boost::asio::io_service* const& ios,NetworkAddress const& addr) 
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   ConnectActorState<ConnectActor>(ios, addr)
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> connect( boost::asio::io_service* const& ios, NetworkAddress const& addr ) {
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new ConnectActor(ios, addr));
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	// This is not part of the IConnection interface, because it is wrapped by IListener::accept()
	void accept(NetworkAddress peerAddr) {
		this->peer_address = peerAddr;
		init();
	}

	Future<Void> acceptHandshake() override { return Void(); }

	Future<Void> connectHandshake() override { return Void(); }

	// returns when write() can write at least one byte
	Future<Void> onWritable() override {
		++g_net2->countWriteProbes;
		BindPromise p("N2_WriteProbeError", id);
		auto f = p.getFuture();
		socket.async_write_some(boost::asio::null_buffers(), std::move(p));
		return f;
	}

	// returns when read() can read at least one byte
	Future<Void> onReadable() override {
		++g_net2->countReadProbes;
		BindPromise p("N2_ReadProbeError", id);
		auto f = p.getFuture();
		socket.async_read_some(boost::asio::null_buffers(), std::move(p));
		return f;
	}

	// Reads as many bytes as possible from the read buffer into [begin,end) and returns the number of bytes read (might
	// be 0)
	int read(uint8_t* begin, uint8_t* end) override {
		boost::system::error_code err;
		++g_net2->countReads;
		size_t toRead = end - begin;
		size_t size = socket.read_some(boost::asio::mutable_buffers_1(begin, toRead), err);
		g_net2->bytesReceived += size;
		//TraceEvent("ConnRead", this->id).detail("Bytes", size);
		if (err) {
			if (err == boost::asio::error::would_block) {
				++g_net2->countWouldBlock;
				return 0;
			}
			onReadError(err);
			throw connection_failed();
		}
		ASSERT(size); // If the socket is closed, we expect an 'eof' error, not a zero return value

		return size;
	}

	// Writes as many bytes as possible from the given SendBuffer chain into the write buffer and returns the number of
	// bytes written (might be 0)
	int write(SendBuffer const* data, int limit) override {
		boost::system::error_code err;
		++g_net2->countWrites;

		size_t sent = socket.write_some(
		    boost::iterator_range<SendBufferIterator>(SendBufferIterator(data, limit), SendBufferIterator()), err);

		if (err) {
			// Since there was an error, sent's value can't be used to infer that the buffer has data and the limit is
			// positive so check explicitly.
			ASSERT(limit > 0);
			bool notEmpty = false;
			for (auto p = data; p; p = p->next)
				if (p->bytes_written - p->bytes_sent > 0) {
					notEmpty = true;
					break;
				}
			ASSERT(notEmpty);

			if (err == boost::asio::error::would_block) {
				++g_net2->countWouldBlock;
				return 0;
			}
			onWriteError(err);
			throw connection_failed();
		}

		ASSERT(sent); // Make sure data was sent, and also this check will fail if the buffer chain was empty or the
		              // limit was not > 0.
		return sent;
	}

	NetworkAddress getPeerAddress() const override { return peer_address; }

	bool hasTrustedPeer() const override { return true; }

	UID getDebugID() const override { return id; }

	tcp::socket& getSocket() override { return socket; }

private:
	UID id;
	tcp::socket socket;
	NetworkAddress peer_address;

	void init() {
		// Socket settings that have to be set after connect or accept succeeds
		socket.non_blocking(true);
		if (FLOW_KNOBS->FLOW_TCP_NODELAY & 1) {
			socket.set_option(boost::asio::ip::tcp::no_delay(true));
		}
		if (FLOW_KNOBS->FLOW_TCP_QUICKACK & 1) {
#ifdef __linux__
			socket.set_option(boost::asio::detail::socket_option::boolean<IPPROTO_TCP, TCP_QUICKACK>(true));
#else
			TraceEvent(SevWarn, "N2_InitWarn").detail("Message", "TCP_QUICKACK not supported");
#endif
		}
		platform::setCloseOnExec(socket.native_handle());
	}

	void closeSocket() {
		boost::system::error_code error;
		socket.close(error);
		if (error)
			TraceEvent(SevWarn, "N2_CloseError", id)
			    .suppressFor(1.0)
			    .detail("PeerAddr", peer_address)
			    .detail("ErrorCode", error.value())
			    .detail("Message", error.message());
	}

	void onReadError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_ReadError", id)
		    .suppressFor(1.0)
		    .detail("PeerAddr", peer_address)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
	void onWriteError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_WriteError", id)
		    .suppressFor(1.0)
		    .detail("PeerAddr", peer_address)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
};

class ReadPromise {
	Promise<int> p;
	const char* errContext;
	UID errID;
	std::shared_ptr<udp::endpoint> endpoint = nullptr;

public:
	ReadPromise(const char* errContext, UID errID) : errContext(errContext), errID(errID) {}
	ReadPromise(ReadPromise const& other) = default;
	ReadPromise(ReadPromise&& other) : p(std::move(other.p)), errContext(other.errContext), errID(other.errID) {}

	std::shared_ptr<udp::endpoint>& getEndpoint() { return endpoint; }

	Future<int> getFuture() { return p.getFuture(); }
	void operator()(const boost::system::error_code& error, size_t bytesWritten) {
		try {
			if (error) {
				TraceEvent evt(SevWarn, errContext, errID);
				evt.suppressFor(1.0).detail("ErrorCode", error.value()).detail("Message", error.message());
				p.sendError(connection_failed());
			} else {
				p.send(int(bytesWritten));
			}
		} catch (Error& e) {
			p.sendError(e);
		} catch (...) {
			p.sendError(unknown_error());
		}
	}
};

class UDPSocket : public IUDPSocket, ReferenceCounted<UDPSocket> {
	UID id;
	Optional<NetworkAddress> toAddress;
	udp::socket socket;
	bool isPublic = false;

public:
																#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectActor1>
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor1State {
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor1State(boost::asio::io_service* const& io_service,Optional<NetworkAddress> const& toAddress,bool const& isV6) 
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : io_service(io_service),
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   toAddress(toAddress),
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   isV6(isV6),
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   self(new UDPSocket(*io_service, toAddress, isV6))
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("connect", reinterpret_cast<unsigned long>(this));

	}
	~ConnectActor1State() 
	{
		fdb_probe_actor_destroy("connect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(!toAddress.present() || toAddress.get().ip.isV6() == isV6);
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (!toAddress.present())
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (!static_cast<ConnectActor1*>(this)->SAV<Reference<IUDPSocket>>::futures) { (void)(self); this->~ConnectActor1State(); static_cast<ConnectActor1*>(this)->destroy(); return 0; }
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				new (&static_cast<ConnectActor1*>(this)->SAV< Reference<IUDPSocket> >::value()) Reference<IUDPSocket>(std::move(self)); // state_var_RVO
				this->~ConnectActor1State();
				static_cast<ConnectActor1*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			try {
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (toAddress.present())
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto to = udpEndpoint(toAddress.get());
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					BindPromise p("N2_UDPConnectError", self->id);
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					Future<Void> onConnected = p.getFuture();
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					self->socket.async_connect(to, std::move(p));
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					StrictFuture<Void> __when_expr_0 = onConnected;
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					if (static_cast<ConnectActor1*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ConnectActor1*>(this)->actor_wait_state = 1;
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor1, 0, Void >*>(static_cast<ConnectActor1*>(this)));
															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1cont3(loopDepth);
				}
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
		this->~ConnectActor1State();
		static_cast<ConnectActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor1*>(this)->SAV<Reference<IUDPSocket>>::futures) { (void)(self); this->~ConnectActor1State(); static_cast<ConnectActor1*>(this)->destroy(); return 0; }
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor1*>(this)->SAV< Reference<IUDPSocket> >::value()) Reference<IUDPSocket>(std::move(self)); // state_var_RVO
		this->~ConnectActor1State();
		static_cast<ConnectActor1*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ConnectActor1*>(this)->actor_wait_state > 0) static_cast<ConnectActor1*>(this)->actor_wait_state = 0;
		static_cast<ConnectActor1*>(this)->ActorCallback< ConnectActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	boost::asio::io_service* io_service;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Optional<NetworkAddress> toAddress;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	bool isV6;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<UDPSocket> self;
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor1 final : public Actor<Reference<IUDPSocket>>, public ActorCallback< ConnectActor1, 0, Void >, public FastAllocated<ConnectActor1>, public ConnectActor1State<ConnectActor1> {
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectActor1>::operator new;
	using FastAllocated<ConnectActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IUDPSocket>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor1, 0, Void >;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor1(boost::asio::io_service* const& io_service,Optional<NetworkAddress> const& toAddress,bool const& isV6) 
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Reference<IUDPSocket>>(),
		   ConnectActor1State<ConnectActor1>(io_service, toAddress, isV6)
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectActor1, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IUDPSocket>> connect( boost::asio::io_service* const& io_service, Optional<NetworkAddress> const& toAddress, bool const& isV6 ) {
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IUDPSocket>>(new ConnectActor1(io_service, toAddress, isV6));
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	void close() override { closeSocket(); }

	Future<int> receive(uint8_t* begin, uint8_t* end) override {
		++g_net2->countUDPReads;
		ReadPromise p("N2_UDPReadError", id);
		auto res = p.getFuture();
		socket.async_receive(boost::asio::mutable_buffer(begin, end - begin), std::move(p));
		return fmap(
		    [](int bytes) {
			    g_net2->udpBytesReceived += bytes;
			    return bytes;
		    },
		    res);
	}

	Future<int> receiveFrom(uint8_t* begin, uint8_t* end, NetworkAddress* sender) override {
		++g_net2->countUDPReads;
		ReadPromise p("N2_UDPReadFromError", id);
		p.getEndpoint() = std::make_shared<udp::endpoint>();
		auto endpoint = p.getEndpoint().get();
		auto res = p.getFuture();
		socket.async_receive_from(boost::asio::mutable_buffer(begin, end - begin), *endpoint, std::move(p));
		return fmap(
		    [endpoint, sender](int bytes) {
			    if (sender) {
				    sender->port = endpoint->port();
				    sender->ip = toIPAddress(endpoint->address());
			    }
			    g_net2->udpBytesReceived += bytes;
			    return bytes;
		    },
		    res);
	}

	Future<int> send(uint8_t const* begin, uint8_t const* end) override {
		++g_net2->countUDPWrites;
		ReadPromise p("N2_UDPWriteError", id);
		auto res = p.getFuture();
		socket.async_send(boost::asio::const_buffer(begin, end - begin), std::move(p));
		return res;
	}

	Future<int> sendTo(uint8_t const* begin, uint8_t const* end, NetworkAddress const& peer) override {
		++g_net2->countUDPWrites;
		ReadPromise p("N2_UDPWriteError", id);
		auto res = p.getFuture();
		udp::endpoint toEndpoint = udpEndpoint(peer);
		socket.async_send_to(boost::asio::const_buffer(begin, end - begin), toEndpoint, std::move(p));
		return res;
	}

	void bind(NetworkAddress const& addr) override {
		boost::system::error_code ec;
		socket.bind(udpEndpoint(addr), ec);
		if (ec) {
			Error x;
			if (ec.value() == EADDRINUSE)
				x = address_in_use();
			else if (ec.value() == EADDRNOTAVAIL)
				x = invalid_local_address();
			else
				x = bind_failed();
			TraceEvent(SevWarnAlways, "Net2UDPBindError").error(x);
			throw x;
		}
		isPublic = true;
	}

	UID getDebugID() const override { return id; }

	void addref() override { ReferenceCounted<UDPSocket>::addref(); }
	void delref() override { ReferenceCounted<UDPSocket>::delref(); }

	NetworkAddress localAddress() const override {
		auto endpoint = socket.local_endpoint();
		return NetworkAddress(toIPAddress(endpoint.address()), endpoint.port(), isPublic, false);
	}

	boost::asio::ip::udp::socket::native_handle_type native_handle() override { return socket.native_handle(); }

private:
	UDPSocket(boost::asio::io_service& io_service, Optional<NetworkAddress> toAddress, bool isV6)
	  : id(nondeterministicRandom()->randomUniqueID()), socket(io_service, isV6 ? udp::v6() : udp::v4()) {}

	void closeSocket() {
		boost::system::error_code error;
		socket.close(error);
		if (error)
			TraceEvent(SevWarn, "N2_CloseError", id)
			    .suppressFor(1.0)
			    .detail("ErrorCode", error.value())
			    .detail("Message", error.message());
	}

	void onReadError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_UDPReadError", id)
		    .suppressFor(1.0)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
	void onWriteError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_UDPWriteError", id)
		    .suppressFor(1.0)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}

	void init() {
		socket.non_blocking(true);
		platform::setCloseOnExec(socket.native_handle());
	}
};

class Listener final : public IListener, ReferenceCounted<Listener> {
	boost::asio::io_context& io_service;
	NetworkAddress listenAddress;
	tcp::acceptor acceptor;

public:
	Listener(boost::asio::io_context& io_service, NetworkAddress listenAddress)
	  : io_service(io_service), listenAddress(listenAddress), acceptor(io_service, tcpEndpoint(listenAddress)) {
		// when port 0 is passed in, a random port will be opened
		// set listenAddress as the address with the actual port opened instead of port 0
		if (listenAddress.port == 0) {
			this->listenAddress =
			    NetworkAddress::parse(acceptor.local_endpoint().address().to_string().append(":").append(
			        std::to_string(acceptor.local_endpoint().port())));
		}
		platform::setCloseOnExec(acceptor.native_handle());
	}

	void addref() override { ReferenceCounted<Listener>::addref(); }
	void delref() override { ReferenceCounted<Listener>::delref(); }

	// Returns one incoming connection when it is available
	Future<Reference<IConnection>> accept() override { return doAccept(this); }

	NetworkAddress getListenAddress() const override { return listenAddress; }

private:
																#line 1258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doAccept()
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoAcceptActor>
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActorState {
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActorState(Listener* const& self) 
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   conn(new Connection(self->io_service)),
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   peer_endpoint()
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("doAccept", reinterpret_cast<unsigned long>(this));

	}
	~DoAcceptActorState() 
	{
		fdb_probe_actor_destroy("doAccept", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				BindPromise p("N2_AcceptError", UID());
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto f = p.getFuture();
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->acceptor.async_accept(conn->getSocket(), peer_endpoint, std::move(p));
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = f;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<DoAcceptActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoAcceptActor*>(this)->actor_wait_state = 1;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoAcceptActor, 0, Void >*>(static_cast<DoAcceptActor*>(this)));
															#line 1304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				loopDepth = 0;
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
		this->~DoAcceptActorState();
		static_cast<DoAcceptActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			conn->close();
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port()));
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActorState(); static_cast<DoAcceptActor*>(this)->destroy(); return 0; }
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<DoAcceptActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~DoAcceptActorState();
		static_cast<DoAcceptActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port()));
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActorState(); static_cast<DoAcceptActor*>(this)->destroy(); return 0; }
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<DoAcceptActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~DoAcceptActorState();
		static_cast<DoAcceptActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoAcceptActor*>(this)->actor_wait_state > 0) static_cast<DoAcceptActor*>(this)->actor_wait_state = 0;
		static_cast<DoAcceptActor*>(this)->ActorCallback< DoAcceptActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoAcceptActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoAcceptActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoAcceptActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Listener* self;
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<Connection> conn;
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::acceptor::endpoint_type peer_endpoint;
															#line 1447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doAccept()
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActor final : public Actor<Reference<IConnection>>, public ActorCallback< DoAcceptActor, 0, Void >, public FastAllocated<DoAcceptActor>, public DoAcceptActorState<DoAcceptActor> {
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoAcceptActor>::operator new;
	using FastAllocated<DoAcceptActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoAcceptActor, 0, Void >;
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActor(Listener* const& self) 
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   DoAcceptActorState<DoAcceptActor>(self)
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doAccept");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoAcceptActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> doAccept( Listener* const& self ) {
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new DoAcceptActor(self));
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
};

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> ssl_socket;

struct SSLHandshakerThread final : IThreadPoolReceiver {
	SSLHandshakerThread() {}
	void init() override {}

	struct Handshake final : TypedAction<SSLHandshakerThread, Handshake> {
		Handshake(ssl_socket& socket, ssl_socket::handshake_type type) : socket(socket), type(type) {}
		double getTimeEstimate() const override { return 0.001; }

		ThreadReturnPromise<Void> done;
		ssl_socket& socket;
		ssl_socket::handshake_type type;
		boost::system::error_code err;
	};

	void action(Handshake& h) {
		try {
			h.socket.next_layer().non_blocking(false, h.err);
			if (!h.err.failed()) {
				h.socket.handshake(h.type, h.err);
			}
			if (!h.err.failed()) {
				h.socket.next_layer().non_blocking(true, h.err);
			}
			if (h.err.failed()) {
				TraceEvent(SevWarn,
				           h.type == ssl_socket::handshake_type::client ? "N2_ConnectHandshakeError"_audit
				                                                        : "N2_AcceptHandshakeError"_audit)
				    .detail("ErrorCode", h.err.value())
				    .detail("ErrorMsg", h.err.message().c_str())
				    .detail("BackgroundThread", true);
				h.done.sendError(connection_failed());
			} else {
				h.done.send(Void());
			}
		} catch (...) {
			TraceEvent(SevWarn,
			           h.type == ssl_socket::handshake_type::client ? "N2_ConnectHandshakeUnknownError"_audit
			                                                        : "N2_AcceptHandshakeUnknownError"_audit)
			    .detail("BackgroundThread", true);
			h.done.sendError(connection_failed());
		}
	}
};

class SSLConnection final : public IConnection, ReferenceCounted<SSLConnection> {
public:
	void addref() override { ReferenceCounted<SSLConnection>::addref(); }
	void delref() override { ReferenceCounted<SSLConnection>::delref(); }

	void close() override { closeSocket(); }

	explicit SSLConnection(boost::asio::io_service& io_service,
	                       Reference<ReferencedObject<boost::asio::ssl::context>> context)
	  : id(nondeterministicRandom()->randomUniqueID()), socket(io_service), ssl_sock(socket, context->mutate()),
	    sslContext(context), has_trusted_peer(false) {}

	explicit SSLConnection(Reference<ReferencedObject<boost::asio::ssl::context>> context, tcp::socket* existingSocket)
	  : id(nondeterministicRandom()->randomUniqueID()), socket(std::move(*existingSocket)),
	    ssl_sock(socket, context->mutate()), sslContext(context) {}

	// This is not part of the IConnection interface, because it is wrapped by INetwork::connect()
																#line 1559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectActor2>
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor2State {
															#line 1565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor2State(boost::asio::io_service* const& ios,Reference<ReferencedObject<boost::asio::ssl::context>> const& context,NetworkAddress const& addr,tcp::socket* const& existingSocket = nullptr) 
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : ios(ios),
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   context(context),
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   addr(addr),
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   existingSocket(existingSocket)
															#line 1578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("connect", reinterpret_cast<unsigned long>(this));

	}
	~ConnectActor2State() 
	{
		fdb_probe_actor_destroy("connect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			std::pair<IPAddress, uint16_t> peerIP = std::make_pair(addr.ip, addr.port);
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (now() < iter->second.second)
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					if (iter->second.first >= FLOW_KNOBS->TLS_CLIENT_CONNECTION_THROTTLE_ATTEMPTS)
															#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					{
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						TraceEvent("TLSOutgoingConnectionThrottlingWarning").suppressFor(1.0).detail("PeerIP", addr);
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						if (static_cast<ConnectActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
						static_cast<ConnectActor2*>(this)->actor_wait_state = 1;
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor2, 0, Void >*>(static_cast<ConnectActor2*>(this)));
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						loopDepth = 0;
					}
					else
					{
						loopDepth = a_body1cont3(loopDepth);
					}
				}
				else
				{
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler.erase(peerIP);
															#line 1630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					loopDepth = a_body1cont2(loopDepth);
				}
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
		this->~ConnectActor2State();
		static_cast<ConnectActor2*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (existingSocket != nullptr)
															#line 1659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
															#line 865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			Reference<SSLConnection> self(new SSLConnection(context, existingSocket));
															#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->peer_address = addr;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->init();
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (!static_cast<ConnectActor2*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActor2State(); static_cast<ConnectActor2*>(this)->destroy(); return 0; }
															#line 1669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			new (&static_cast<ConnectActor2*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(self);
			this->~ConnectActor2State();
			static_cast<ConnectActor2*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self = Reference<SSLConnection>(new SSLConnection(*ios, context));
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->peer_address = addr;
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			auto to = tcpEndpoint(self->peer_address);
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			BindPromise p("N2_ConnectError", self->id);
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			Future<Void> onConnected = p.getFuture();
															#line 877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->socket.async_connect(to, std::move(p));
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = onConnected;
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectActor2*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ConnectActor2*>(this)->actor_wait_state = 2;
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectActor2, 1, Void >*>(static_cast<ConnectActor2*>(this)));
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 1733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

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
		if (static_cast<ConnectActor2*>(this)->actor_wait_state > 0) static_cast<ConnectActor2*>(this)->actor_wait_state = 0;
		static_cast<ConnectActor2*>(this)->ActorCallback< ConnectActor2, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectActor2, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectActor2, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectActor2, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 1807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor2*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActor2State(); static_cast<ConnectActor2*>(this)->destroy(); return 0; }
															#line 1823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor2*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(self)); // state_var_RVO
		this->~ConnectActor2State();
		static_cast<ConnectActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor2*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActor2State(); static_cast<ConnectActor2*>(this)->destroy(); return 0; }
															#line 1837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor2*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(self)); // state_var_RVO
		this->~ConnectActor2State();
		static_cast<ConnectActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectActor2*>(this)->actor_wait_state > 0) static_cast<ConnectActor2*>(this)->actor_wait_state = 0;
		static_cast<ConnectActor2*>(this)->ActorCallback< ConnectActor2, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectActor2, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectActor2, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectActor2, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	boost::asio::io_service* ios;
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<ReferencedObject<boost::asio::ssl::context>> context;
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	NetworkAddress addr;
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::socket* existingSocket;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 1918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor2 final : public Actor<Reference<IConnection>>, public ActorCallback< ConnectActor2, 0, Void >, public ActorCallback< ConnectActor2, 1, Void >, public FastAllocated<ConnectActor2>, public ConnectActor2State<ConnectActor2> {
															#line 1923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectActor2>::operator new;
	using FastAllocated<ConnectActor2>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor2, 0, Void >;
friend struct ActorCallback< ConnectActor2, 1, Void >;
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor2(boost::asio::io_service* const& ios,Reference<ReferencedObject<boost::asio::ssl::context>> const& context,NetworkAddress const& addr,tcp::socket* const& existingSocket = nullptr) 
															#line 1935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   ConnectActor2State<ConnectActor2>(ios, context, addr, existingSocket)
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectActor2, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectActor2, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> connect( boost::asio::io_service* const& ios, Reference<ReferencedObject<boost::asio::ssl::context>> const& context, NetworkAddress const& addr, tcp::socket* const& existingSocket = nullptr ) {
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new ConnectActor2(ios, context, addr, existingSocket));
															#line 1963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	// This is not part of the IConnection interface, because it is wrapped by IListener::accept()
	void accept(NetworkAddress peerAddr) {
		this->peer_address = peerAddr;
		init();
	}

																#line 1974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doAcceptHandshake()
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoAcceptHandshakeActor>
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptHandshakeActorState {
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptHandshakeActorState(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   connected(connected),
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   holder()
															#line 1991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("doAcceptHandshake", reinterpret_cast<unsigned long>(this));

	}
	~DoAcceptHandshakeActorState() 
	{
		fdb_probe_actor_destroy("doAcceptHandshake", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Future<Void> onHandshook;
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				ConfigureSSLStream(N2::g_net2->activeTlsPolicy, self->ssl_sock, [conn = self.getPtr()](bool verifyOk) { conn->has_trusted_peer = verifyOk; });
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (N2::g_net2->sslPoolHandshakesInProgress < N2::g_net2->sslHandshakerThreadsStarted)
															#line 2011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					holder = Hold(&N2::g_net2->sslPoolHandshakesInProgress);
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto handshake = new SSLHandshakerThread::Handshake(self->ssl_sock, boost::asio::ssl::stream_base::server);
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = handshake->done.getFuture();
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					N2::g_net2->sslHandshakerPool->post(handshake);
															#line 2021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					BindPromise p("N2_AcceptHandshakeError"_audit, UID());
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = p.getFuture();
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					self->ssl_sock.async_handshake(boost::asio::ssl::stream_base::server, std::move(p));
															#line 2031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = onHandshook;
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 2037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 1;
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoAcceptHandshakeActor, 0, Void >*>(static_cast<DoAcceptHandshakeActor*>(this)));
															#line 2041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				loopDepth = 0;
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
		delete static_cast<DoAcceptHandshakeActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			connected.sendError(connection_failed());
															#line 2078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 2095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 2;
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoAcceptHandshakeActor, 1, Void >*>(static_cast<DoAcceptHandshakeActor*>(this)));
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 2110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 2;
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoAcceptHandshakeActor, 1, Void >*>(static_cast<DoAcceptHandshakeActor*>(this)));
															#line 2114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state > 0) static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 0;
		static_cast<DoAcceptHandshakeActor*>(this)->ActorCallback< DoAcceptHandshakeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoAcceptHandshakeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoAcceptHandshakeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoAcceptHandshakeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 2186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 2195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state > 0) static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 0;
		static_cast<DoAcceptHandshakeActor*>(this)->ActorCallback< DoAcceptHandshakeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoAcceptHandshakeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoAcceptHandshakeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoAcceptHandshakeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
	{
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		delete static_cast<DoAcceptHandshakeActor*>(this);
															#line 2280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Promise<Void> connected;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Hold<int> holder;
															#line 2291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doAcceptHandshake()
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptHandshakeActor final : public Actor<void>, public ActorCallback< DoAcceptHandshakeActor, 0, Void >, public ActorCallback< DoAcceptHandshakeActor, 1, Void >, public FastAllocated<DoAcceptHandshakeActor>, public DoAcceptHandshakeActorState<DoAcceptHandshakeActor> {
															#line 2296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoAcceptHandshakeActor>::operator new;
	using FastAllocated<DoAcceptHandshakeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoAcceptHandshakeActor, 0, Void >;
friend struct ActorCallback< DoAcceptHandshakeActor, 1, Void >;
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptHandshakeActor(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 2308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<void>(),
		   DoAcceptHandshakeActorState<DoAcceptHandshakeActor>(self, connected)
	{
		fdb_probe_actor_enter("doAcceptHandshake", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doAcceptHandshake");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doAcceptHandshake", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static void doAcceptHandshake( Reference<SSLConnection> const& self, Promise<Void> const& connected ) {
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	new DoAcceptHandshakeActor(self, connected);
															#line 2326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

																#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via acceptHandshakeWrapper()
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class AcceptHandshakeWrapperActor>
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class AcceptHandshakeWrapperActorState {
															#line 2337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AcceptHandshakeWrapperActorState(Reference<SSLConnection> const& self) 
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   peerIP()
															#line 2346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this));

	}
	~AcceptHandshakeWrapperActorState() 
	{
		fdb_probe_actor_destroy("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			peerIP = std::make_pair(self->getPeerAddress().ip, static_cast<uint16_t>(0));
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 2365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (now() < iter->second.second)
															#line 2369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					if (iter->second.first >= FLOW_KNOBS->TLS_SERVER_CONNECTION_THROTTLE_ATTEMPTS)
															#line 2373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					{
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						TraceEvent("TLSIncomingConnectionThrottlingWarning") .suppressFor(1.0) .detail("PeerIP", peerIP.first.toString());
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
						static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 1;
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 0, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						loopDepth = 0;
					}
					else
					{
						loopDepth = a_body1cont3(loopDepth);
					}
				}
				else
				{
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler.erase(peerIP);
															#line 2398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					loopDepth = a_body1cont2(loopDepth);
				}
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
		this->~AcceptHandshakeWrapperActorState();
		static_cast<AcceptHandshakeWrapperActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_network->networkInfo.handshakeLock->take();
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 2;
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 1, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->closeSocket();
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 2457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->closeSocket();
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 2467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

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
		if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state > 0) static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 0;
		static_cast<AcceptHandshakeWrapperActor*>(this)->ActorCallback< AcceptHandshakeWrapperActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AcceptHandshakeWrapperActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		releaser = FlowLock::Releaser(*g_network->networkInfo.handshakeLock);
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> connected;
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doAcceptHandshake(self, connected);
															#line 2542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = connected.getFuture();
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont7Catch1(actor_cancelled(), loopDepth);
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont7Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont7when1(__when_expr_2.get(), loopDepth); };
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 2552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont7Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont7when2(__when_expr_3.get(), loopDepth); };
			static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 3;
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont7Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont7Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		releaser = FlowLock::Releaser(*g_network->networkInfo.handshakeLock);
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> connected;
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doAcceptHandshake(self, connected);
															#line 2578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = connected.getFuture();
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont7Catch1(actor_cancelled(), loopDepth);
															#line 2584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont7Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont7when1(__when_expr_2.get(), loopDepth); };
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 2588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont7Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont7when2(__when_expr_3.get(), loopDepth); };
			static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 3;
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont7Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont7Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state > 0) static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 0;
		static_cast<AcceptHandshakeWrapperActor*>(this)->ActorCallback< AcceptHandshakeWrapperActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AcceptHandshakeWrapperActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() != error_code_actor_cancelled)
															#line 2674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 2680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					iter->second.first++;
															#line 2684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler[peerIP] = std::make_pair(0, now() + FLOW_KNOBS->TLS_SERVER_CONNECTION_THROTTLE_TIMEOUT);
															#line 2690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
			}
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<AcceptHandshakeWrapperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AcceptHandshakeWrapperActorState(); static_cast<AcceptHandshakeWrapperActor*>(this)->destroy(); return 0; }
															#line 2711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<AcceptHandshakeWrapperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AcceptHandshakeWrapperActorState();
		static_cast<AcceptHandshakeWrapperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<AcceptHandshakeWrapperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AcceptHandshakeWrapperActorState(); static_cast<AcceptHandshakeWrapperActor*>(this)->destroy(); return 0; }
															#line 2723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<AcceptHandshakeWrapperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AcceptHandshakeWrapperActorState();
		static_cast<AcceptHandshakeWrapperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7when2(Void const& _,int loopDepth) 
	{
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont7Catch1(connection_failed(), loopDepth);
															#line 2735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont7when2(Void && _,int loopDepth) 
	{
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont7Catch1(connection_failed(), loopDepth);
															#line 2743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state > 0) static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 0;
		static_cast<AcceptHandshakeWrapperActor*>(this)->ActorCallback< AcceptHandshakeWrapperActor, 2, Void >::remove();
		static_cast<AcceptHandshakeWrapperActor*>(this)->ActorCallback< AcceptHandshakeWrapperActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont7Catch1(error, 0);
		} catch (...) {
			a_body1cont7Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont7Catch1(error, 0);
		} catch (...) {
			a_body1cont7Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont7Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont7Catch1(error, 0);
		} catch (...) {
			a_body1cont7Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont7when2(value, 0);
		}
		catch (Error& error) {
			a_body1cont7Catch1(error, 0);
		} catch (...) {
			a_body1cont7Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont7when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont7Catch1(error, 0);
		} catch (...) {
			a_body1cont7Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont7Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont7Catch1(error, 0);
		} catch (...) {
			a_body1cont7Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::pair<IPAddress, uint16_t> peerIP;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowLock::Releaser releaser;
															#line 2850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via acceptHandshakeWrapper()
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class AcceptHandshakeWrapperActor final : public Actor<Void>, public ActorCallback< AcceptHandshakeWrapperActor, 0, Void >, public ActorCallback< AcceptHandshakeWrapperActor, 1, Void >, public ActorCallback< AcceptHandshakeWrapperActor, 2, Void >, public ActorCallback< AcceptHandshakeWrapperActor, 3, Void >, public FastAllocated<AcceptHandshakeWrapperActor>, public AcceptHandshakeWrapperActorState<AcceptHandshakeWrapperActor> {
															#line 2855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<AcceptHandshakeWrapperActor>::operator new;
	using FastAllocated<AcceptHandshakeWrapperActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AcceptHandshakeWrapperActor, 0, Void >;
friend struct ActorCallback< AcceptHandshakeWrapperActor, 1, Void >;
friend struct ActorCallback< AcceptHandshakeWrapperActor, 2, Void >;
friend struct ActorCallback< AcceptHandshakeWrapperActor, 3, Void >;
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AcceptHandshakeWrapperActor(Reference<SSLConnection> const& self) 
															#line 2869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   AcceptHandshakeWrapperActorState<AcceptHandshakeWrapperActor>(self)
	{
		fdb_probe_actor_enter("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("acceptHandshakeWrapper");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("acceptHandshakeWrapper", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AcceptHandshakeWrapperActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AcceptHandshakeWrapperActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Void> acceptHandshakeWrapper( Reference<SSLConnection> const& self ) {
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new AcceptHandshakeWrapperActor(self));
															#line 2898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	Future<Void> acceptHandshake() override { return acceptHandshakeWrapper(Reference<SSLConnection>::addRef(this)); }

																#line 2905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doConnectHandshake()
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoConnectHandshakeActor>
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoConnectHandshakeActorState {
															#line 2911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoConnectHandshakeActorState(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   connected(connected),
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   holder()
															#line 2922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("doConnectHandshake", reinterpret_cast<unsigned long>(this));

	}
	~DoConnectHandshakeActorState() 
	{
		fdb_probe_actor_destroy("doConnectHandshake", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Future<Void> onHandshook;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				ConfigureSSLStream(N2::g_net2->activeTlsPolicy, self->ssl_sock, [conn = self.getPtr()](bool verifyOk) { conn->has_trusted_peer = verifyOk; });
															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (N2::g_net2->sslPoolHandshakesInProgress < N2::g_net2->sslHandshakerThreadsStarted)
															#line 2942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					holder = Hold(&N2::g_net2->sslPoolHandshakesInProgress);
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto handshake = new SSLHandshakerThread::Handshake(self->ssl_sock, boost::asio::ssl::stream_base::client);
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = handshake->done.getFuture();
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					N2::g_net2->sslHandshakerPool->post(handshake);
															#line 2952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					BindPromise p("N2_ConnectHandshakeError"_audit, self->id);
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = p.getFuture();
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					self->ssl_sock.async_handshake(boost::asio::ssl::stream_base::client, std::move(p));
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = onHandshook;
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 2968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 1;
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoConnectHandshakeActor, 0, Void >*>(static_cast<DoConnectHandshakeActor*>(this)));
															#line 2972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				loopDepth = 0;
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
		delete static_cast<DoConnectHandshakeActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			connected.sendError(connection_failed());
															#line 3009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 3026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 2;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoConnectHandshakeActor, 1, Void >*>(static_cast<DoConnectHandshakeActor*>(this)));
															#line 3030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 3041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 2;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoConnectHandshakeActor, 1, Void >*>(static_cast<DoConnectHandshakeActor*>(this)));
															#line 3045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state > 0) static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 0;
		static_cast<DoConnectHandshakeActor*>(this)->ActorCallback< DoConnectHandshakeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoConnectHandshakeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoConnectHandshakeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoConnectHandshakeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 3117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 3126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state > 0) static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 0;
		static_cast<DoConnectHandshakeActor*>(this)->ActorCallback< DoConnectHandshakeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoConnectHandshakeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoConnectHandshakeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoConnectHandshakeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
	{
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		delete static_cast<DoConnectHandshakeActor*>(this);
															#line 3211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Promise<Void> connected;
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Hold<int> holder;
															#line 3222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doConnectHandshake()
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoConnectHandshakeActor final : public Actor<void>, public ActorCallback< DoConnectHandshakeActor, 0, Void >, public ActorCallback< DoConnectHandshakeActor, 1, Void >, public FastAllocated<DoConnectHandshakeActor>, public DoConnectHandshakeActorState<DoConnectHandshakeActor> {
															#line 3227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoConnectHandshakeActor>::operator new;
	using FastAllocated<DoConnectHandshakeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoConnectHandshakeActor, 0, Void >;
friend struct ActorCallback< DoConnectHandshakeActor, 1, Void >;
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoConnectHandshakeActor(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 3239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<void>(),
		   DoConnectHandshakeActorState<DoConnectHandshakeActor>(self, connected)
	{
		fdb_probe_actor_enter("doConnectHandshake", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doConnectHandshake");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doConnectHandshake", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static void doConnectHandshake( Reference<SSLConnection> const& self, Promise<Void> const& connected ) {
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	new DoConnectHandshakeActor(self, connected);
															#line 3257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

																#line 3262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connectHandshakeWrapper()
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectHandshakeWrapperActor>
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectHandshakeWrapperActorState {
															#line 3268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectHandshakeWrapperActorState(Reference<SSLConnection> const& self) 
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self)
															#line 3275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this));

	}
	~ConnectHandshakeWrapperActorState() 
	{
		fdb_probe_actor_destroy("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_network->networkInfo.handshakeLock->take();
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 1;
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 0, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 3297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		this->~ConnectHandshakeWrapperActorState();
		static_cast<ConnectHandshakeWrapperActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		releaser = FlowLock::Releaser(*g_network->networkInfo.handshakeLock);
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> connected;
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doConnectHandshake(self, connected);
															#line 3324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = connected.getFuture();
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 2;
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 3341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		releaser = FlowLock::Releaser(*g_network->networkInfo.handshakeLock);
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> connected;
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doConnectHandshake(self, connected);
															#line 3360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = connected.getFuture();
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 2;
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 3377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state > 0) static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectHandshakeWrapperActor*>(this)->ActorCallback< ConnectHandshakeWrapperActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectHandshakeWrapperActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectHandshakeWrapperActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectHandshakeWrapperActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() != error_code_actor_cancelled)
															#line 3456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				std::pair<IPAddress, uint16_t> peerIP = std::make_pair(self->peer_address.ip, self->peer_address.port);
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 3464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					iter->second.first++;
															#line 3468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler[peerIP] = std::make_pair(0, now() + FLOW_KNOBS->TLS_CLIENT_CONNECTION_THROTTLE_TIMEOUT);
															#line 3474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
			}
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 3481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectHandshakeWrapperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectHandshakeWrapperActorState(); static_cast<ConnectHandshakeWrapperActor*>(this)->destroy(); return 0; }
															#line 3495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectHandshakeWrapperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectHandshakeWrapperActorState();
		static_cast<ConnectHandshakeWrapperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectHandshakeWrapperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectHandshakeWrapperActorState(); static_cast<ConnectHandshakeWrapperActor*>(this)->destroy(); return 0; }
															#line 3507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectHandshakeWrapperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectHandshakeWrapperActorState();
		static_cast<ConnectHandshakeWrapperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont1Catch1(connection_failed(), loopDepth);
															#line 3519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont1Catch1(connection_failed(), loopDepth);
															#line 3527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state > 0) static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectHandshakeWrapperActor*>(this)->ActorCallback< ConnectHandshakeWrapperActor, 1, Void >::remove();
		static_cast<ConnectHandshakeWrapperActor*>(this)->ActorCallback< ConnectHandshakeWrapperActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowLock::Releaser releaser;
															#line 3632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connectHandshakeWrapper()
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectHandshakeWrapperActor final : public Actor<Void>, public ActorCallback< ConnectHandshakeWrapperActor, 0, Void >, public ActorCallback< ConnectHandshakeWrapperActor, 1, Void >, public ActorCallback< ConnectHandshakeWrapperActor, 2, Void >, public FastAllocated<ConnectHandshakeWrapperActor>, public ConnectHandshakeWrapperActorState<ConnectHandshakeWrapperActor> {
															#line 3637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectHandshakeWrapperActor>::operator new;
	using FastAllocated<ConnectHandshakeWrapperActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectHandshakeWrapperActor, 0, Void >;
friend struct ActorCallback< ConnectHandshakeWrapperActor, 1, Void >;
friend struct ActorCallback< ConnectHandshakeWrapperActor, 2, Void >;
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectHandshakeWrapperActor(Reference<SSLConnection> const& self) 
															#line 3650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectHandshakeWrapperActorState<ConnectHandshakeWrapperActor>(self)
	{
		fdb_probe_actor_enter("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectHandshakeWrapper");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectHandshakeWrapper", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectHandshakeWrapperActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Void> connectHandshakeWrapper( Reference<SSLConnection> const& self ) {
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new ConnectHandshakeWrapperActor(self));
															#line 3678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	Future<Void> connectHandshake() override { return connectHandshakeWrapper(Reference<SSLConnection>::addRef(this)); }

	// returns when write() can write at least one byte
	Future<Void> onWritable() override {
		++g_net2->countWriteProbes;
		BindPromise p("N2_WriteProbeError", id);
		auto f = p.getFuture();
		socket.async_write_some(boost::asio::null_buffers(), std::move(p));
		return f;
	}

	// returns when read() can read at least one byte
	Future<Void> onReadable() override {
		++g_net2->countReadProbes;
		BindPromise p("N2_ReadProbeError", id);
		auto f = p.getFuture();
		socket.async_read_some(boost::asio::null_buffers(), std::move(p));
		return f;
	}

	// Reads as many bytes as possible from the read buffer into [begin,end) and returns the number of bytes read (might
	// be 0)
	int read(uint8_t* begin, uint8_t* end) override {
		boost::system::error_code err;
		++g_net2->countReads;
		size_t toRead = end - begin;
		size_t size = ssl_sock.read_some(boost::asio::mutable_buffers_1(begin, toRead), err);
		g_net2->bytesReceived += size;
		//TraceEvent("ConnRead", this->id).detail("Bytes", size);
		if (err) {
			if (err == boost::asio::error::would_block) {
				++g_net2->countWouldBlock;
				return 0;
			}
			onReadError(err);
			throw connection_failed();
		}
		ASSERT(size); // If the socket is closed, we expect an 'eof' error, not a zero return value

		return size;
	}

	// Writes as many bytes as possible from the given SendBuffer chain into the write buffer and returns the number of
	// bytes written (might be 0)
	int write(SendBuffer const* data, int limit) override {
#ifdef __APPLE__
		// For some reason, writing ssl_sock with more than 2016 bytes when socket is writeable sometimes results in a
		// broken pipe error.
		limit = std::min(limit, 2016);
#endif
		boost::system::error_code err;
		++g_net2->countWrites;

		size_t sent = ssl_sock.write_some(
		    boost::iterator_range<SendBufferIterator>(SendBufferIterator(data, limit), SendBufferIterator()), err);

		if (err) {
			// Since there was an error, sent's value can't be used to infer that the buffer has data and the limit is
			// positive so check explicitly.
			ASSERT(limit > 0);
			bool notEmpty = false;
			for (auto p = data; p; p = p->next)
				if (p->bytes_written - p->bytes_sent > 0) {
					notEmpty = true;
					break;
				}
			ASSERT(notEmpty);

			if (err == boost::asio::error::would_block) {
				++g_net2->countWouldBlock;
				return 0;
			}
			onWriteError(err);
			throw connection_failed();
		}

		ASSERT(sent); // Make sure data was sent, and also this check will fail if the buffer chain was empty or the
		              // limit was not > 0.
		return sent;
	}

	NetworkAddress getPeerAddress() const override { return peer_address; }

	bool hasTrustedPeer() const override { return has_trusted_peer; }

	UID getDebugID() const override { return id; }

	tcp::socket& getSocket() override { return socket; }

	ssl_socket& getSSLSocket() { return ssl_sock; }

private:
	UID id;
	tcp::socket socket;
	ssl_socket ssl_sock;
	NetworkAddress peer_address;
	Reference<ReferencedObject<boost::asio::ssl::context>> sslContext;
	bool has_trusted_peer;

	void init() {
		// Socket settings that have to be set after connect or accept succeeds
		socket.non_blocking(true);
		socket.set_option(boost::asio::ip::tcp::no_delay(true));
		platform::setCloseOnExec(socket.native_handle());
	}

	void closeSocket() {
		boost::system::error_code cancelError;
		socket.cancel(cancelError);
		boost::system::error_code closeError;
		socket.close(closeError);
		boost::system::error_code shutdownError;
		ssl_sock.shutdown(shutdownError);
	}

	void onReadError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_ReadError", id)
		    .suppressFor(1.0)
		    .detail("PeerAddr", peer_address)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
	void onWriteError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_WriteError", id)
		    .suppressFor(1.0)
		    .detail("PeerAddr", peer_address)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
};

class SSLListener final : public IListener, ReferenceCounted<SSLListener> {
	boost::asio::io_context& io_service;
	NetworkAddress listenAddress;
	tcp::acceptor acceptor;
	AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* contextVar;

public:
	SSLListener(boost::asio::io_context& io_service,
	            AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* contextVar,
	            NetworkAddress listenAddress)
	  : io_service(io_service), listenAddress(listenAddress), acceptor(io_service, tcpEndpoint(listenAddress)),
	    contextVar(contextVar) {
		// when port 0 is passed in, a random port will be opened
		// set listenAddress as the address with the actual port opened instead of port 0
		if (listenAddress.port == 0) {
			this->listenAddress = NetworkAddress::parse(acceptor.local_endpoint()
			                                                .address()
			                                                .to_string()
			                                                .append(":")
			                                                .append(std::to_string(acceptor.local_endpoint().port()))
			                                                .append(listenAddress.isTLS() ? ":tls" : ""));
		}
		platform::setCloseOnExec(acceptor.native_handle());
	}

	void addref() override { ReferenceCounted<SSLListener>::addref(); }
	void delref() override { ReferenceCounted<SSLListener>::delref(); }

	// Returns one incoming connection when it is available
	Future<Reference<IConnection>> accept() override { return doAccept(this); }

	NetworkAddress getListenAddress() const override { return listenAddress; }

private:
																#line 3850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doAccept()
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoAcceptActor1>
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActor1State {
															#line 3856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActor1State(SSLListener* const& self) 
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   conn(new SSLConnection(self->io_service, self->contextVar->get())),
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   peer_endpoint()
															#line 3867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("doAccept", reinterpret_cast<unsigned long>(this));

	}
	~DoAcceptActor1State() 
	{
		fdb_probe_actor_destroy("doAccept", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				BindPromise p("N2_AcceptError", UID());
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto f = p.getFuture();
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->acceptor.async_accept(conn->getSocket(), peer_endpoint, std::move(p));
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = f;
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<DoAcceptActor1*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoAcceptActor1*>(this)->actor_wait_state = 1;
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoAcceptActor1, 0, Void >*>(static_cast<DoAcceptActor1*>(this)));
															#line 3896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				loopDepth = 0;
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
		this->~DoAcceptActor1State();
		static_cast<DoAcceptActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			conn->close();
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 3928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port(), false, true));
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor1*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActor1State(); static_cast<DoAcceptActor1*>(this)->destroy(); return 0; }
															#line 3946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<DoAcceptActor1*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~DoAcceptActor1State();
		static_cast<DoAcceptActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port(), false, true));
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor1*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActor1State(); static_cast<DoAcceptActor1*>(this)->destroy(); return 0; }
															#line 3962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<DoAcceptActor1*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~DoAcceptActor1State();
		static_cast<DoAcceptActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoAcceptActor1*>(this)->actor_wait_state > 0) static_cast<DoAcceptActor1*>(this)->actor_wait_state = 0;
		static_cast<DoAcceptActor1*>(this)->ActorCallback< DoAcceptActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoAcceptActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoAcceptActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoAcceptActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	SSLListener* self;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> conn;
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::acceptor::endpoint_type peer_endpoint;
															#line 4039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doAccept()
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActor1 final : public Actor<Reference<IConnection>>, public ActorCallback< DoAcceptActor1, 0, Void >, public FastAllocated<DoAcceptActor1>, public DoAcceptActor1State<DoAcceptActor1> {
															#line 4044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoAcceptActor1>::operator new;
	using FastAllocated<DoAcceptActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoAcceptActor1, 0, Void >;
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActor1(SSLListener* const& self) 
															#line 4055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   DoAcceptActor1State<DoAcceptActor1>(self)
	{
		fdb_probe_actor_enter("doAccept", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doAccept");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doAccept", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoAcceptActor1, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> doAccept( SSLListener* const& self ) {
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new DoAcceptActor1(self));
															#line 4082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
};

// 5MB for loading files into memory

Net2::Net2(const TLSConfig& tlsConfig, bool useThreadPool, bool useMetrics)
  : globals(enumGlobal::COUNT), useThreadPool(useThreadPool), reactor(this),
    sslContextVar({ ReferencedObject<boost::asio::ssl::context>::from(
        boost::asio::ssl::context(boost::asio::ssl::context::tls)) }),
    sslHandshakerThreadsStarted(0), sslPoolHandshakesInProgress(0), tlsConfig(tlsConfig),
    tlsInitializedState(ETLSInitState::NONE), network(this), tscBegin(0), tscEnd(0), taskBegin(0),
    currentTaskID(TaskPriority::DefaultYield), stopped(false), started(false), numYields(0),
    lastPriorityStats(nullptr) {
	// Until run() is called, yield() will always yield
	TraceEvent("Net2Starting").log();

	// Set the global members
	if (useMetrics) {
		setGlobal(INetwork::enTDMetrics, (flowGlobalType)&tdmetrics);
	}
	if (FLOW_KNOBS->ENABLE_CHAOS_FEATURES) {
		setGlobal(INetwork::enChaosMetrics, (flowGlobalType)&chaosMetrics);
	}
	setGlobal(INetwork::enMetrics, (flowGlobalType)&metrics);
	setGlobal(INetwork::enNetworkConnections, (flowGlobalType)network);
	setGlobal(INetwork::enASIOService, (flowGlobalType)&reactor.ios);
	setGlobal(INetwork::enBlobCredentialFiles, &blobCredentialFiles);

#ifdef __linux__
	setGlobal(INetwork::enEventFD, (flowGlobalType)N2::ASIOReactor::newEventFD(reactor));
#endif

	updateNow();
}

															#line 4120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via reloadCertificatesOnChange()
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ReloadCertificatesOnChangeActor>
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ReloadCertificatesOnChangeActorState {
															#line 4126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ReloadCertificatesOnChangeActorState(TLSConfig const& config,std::function<void()> const& onPolicyFailure,AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* const& contextVar,Reference<TLSPolicy>* const& policy) 
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : config(config),
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   onPolicyFailure(onPolicyFailure),
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   contextVar(contextVar),
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   policy(policy)
															#line 4139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this));

	}
	~ReloadCertificatesOnChangeActorState() 
	{
		fdb_probe_actor_destroy("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (FLOW_KNOBS->TLS_CERT_REFRESH_DELAY_SECONDS <= 0)
															#line 4154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (!static_cast<ReloadCertificatesOnChangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReloadCertificatesOnChangeActorState(); static_cast<ReloadCertificatesOnChangeActor*>(this)->destroy(); return 0; }
															#line 4158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				new (&static_cast<ReloadCertificatesOnChangeActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ReloadCertificatesOnChangeActorState();
				static_cast<ReloadCertificatesOnChangeActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			;
															#line 4166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		this->~ReloadCertificatesOnChangeActorState();
		static_cast<ReloadCertificatesOnChangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		mismatches = 0;
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		fileChanged = AsyncTrigger();
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes = std::vector<Future<Void>>();
															#line 1278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		const int& intervalSeconds = FLOW_KNOBS->TLS_CERT_REFRESH_DELAY_SECONDS;
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes.push_back(watchFileForChanges( config.getCertificatePathSync(), &fileChanged, &intervalSeconds, "TLSCertificateRefreshStatError"));
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes.push_back( watchFileForChanges(config.getKeyPathSync(), &fileChanged, &intervalSeconds, "TLSKeyRefreshStatError"));
															#line 1283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes.push_back( watchFileForChanges(config.getCAPathSync(), &fileChanged, &intervalSeconds, "TLSCARefreshStatError"));
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		;
															#line 4203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

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
															#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (IAsyncFileSystem::filesystem() != nullptr)
															#line 4219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(1.0);
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 1;
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state > 0) static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 0;
		static_cast<ReloadCertificatesOnChangeActor*>(this)->ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = fileChanged.onTrigger();
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 2;
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent("TLSCertificateRefreshBegin").log();
															#line 4352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<LoadedTLSConfig> __when_expr_2 = config.loadAsync();
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1loopBody1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 3;
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1loopBody1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1loopBody1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent("TLSCertificateRefreshBegin").log();
															#line 4378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<LoadedTLSConfig> __when_expr_2 = config.loadAsync();
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1loopBody1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 3;
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1loopBody1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1loopBody1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state > 0) static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 0;
		static_cast<ReloadCertificatesOnChangeActor*>(this)->ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1loopBody1cont2(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 4474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 4478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
															#line 1302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			mismatches++;
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			TraceEvent(SevWarn, "TLSCertificateRefreshMismatch").error(e).detail("mismatches", mismatches);
															#line 4484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			loopDepth = a_body1cont1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(LoadedTLSConfig const& loaded,int loopDepth) 
	{
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		boost::asio::ssl::context context(boost::asio::ssl::context::tls);
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		ConfigureSSLContext(loaded, context);
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		*policy = makeReference<TLSPolicy>(loaded, onPolicyFailure);
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent(SevInfo, "TLSCertificateRefreshSucceeded").log();
															#line 1295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		mismatches = 0;
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		contextVar->set(ReferencedObject<boost::asio::ssl::context>::from(std::move(context)));
															#line 4509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(LoadedTLSConfig && loaded,int loopDepth) 
	{
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		boost::asio::ssl::context context(boost::asio::ssl::context::tls);
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		ConfigureSSLContext(loaded, context);
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		*policy = makeReference<TLSPolicy>(loaded, onPolicyFailure);
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent(SevInfo, "TLSCertificateRefreshSucceeded").log();
															#line 1295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		mismatches = 0;
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		contextVar->set(ReferencedObject<boost::asio::ssl::context>::from(std::move(context)));
															#line 4528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(LoadedTLSConfig const& loaded,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(loaded, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(LoadedTLSConfig && loaded,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(std::move(loaded), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state > 0) static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 0;
		static_cast<ReloadCertificatesOnChangeActor*>(this)->ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >::remove();

	}
	void a_callback_fire(ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*,LoadedTLSConfig const& value) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*,LoadedTLSConfig && value) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*,Error err) 
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1loopBody1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	TLSConfig config;
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::function<void()> onPolicyFailure;
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* contextVar;
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<TLSPolicy>* policy;
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	int mismatches;
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AsyncTrigger fileChanged;
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::vector<Future<Void>> lifetimes;
															#line 4623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via reloadCertificatesOnChange()
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ReloadCertificatesOnChangeActor final : public Actor<Void>, public ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >, public ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >, public ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >, public FastAllocated<ReloadCertificatesOnChangeActor>, public ReloadCertificatesOnChangeActorState<ReloadCertificatesOnChangeActor> {
															#line 4628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ReloadCertificatesOnChangeActor>::operator new;
	using FastAllocated<ReloadCertificatesOnChangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >;
friend struct ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >;
friend struct ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >;
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ReloadCertificatesOnChangeActor(TLSConfig const& config,std::function<void()> const& onPolicyFailure,AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* const& contextVar,Reference<TLSPolicy>* const& policy) 
															#line 4641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   ReloadCertificatesOnChangeActorState<ReloadCertificatesOnChangeActor>(config, onPolicyFailure, contextVar, policy)
	{
		fdb_probe_actor_enter("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("reloadCertificatesOnChange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("reloadCertificatesOnChange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Void> reloadCertificatesOnChange( TLSConfig const& config, std::function<void()> const& onPolicyFailure, AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* const& contextVar, Reference<TLSPolicy>* const& policy ) {
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new ReloadCertificatesOnChangeActor(config, onPolicyFailure, contextVar, policy));
															#line 4670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

void Net2::initTLS(ETLSInitState targetState) {
	if (tlsInitializedState >= targetState) {
		return;
	}
	// Any target state must be higher than NONE so if the current state is NONE
	// then initialize the TLS config
	if (tlsInitializedState == ETLSInitState::NONE) {
		auto onPolicyFailure = [this]() { this->countTLSPolicyFailures++; };
		try {
			boost::asio::ssl::context newContext(boost::asio::ssl::context::tls);
			const LoadedTLSConfig& loaded = tlsConfig.loadSync();
			TraceEvent("Net2TLSConfig")
			    .detail("CAPath", tlsConfig.getCAPathSync())
			    .detail("CertificatePath", tlsConfig.getCertificatePathSync())
			    .detail("KeyPath", tlsConfig.getKeyPathSync())
			    .detail("HasPassword", !loaded.getPassword().empty())
			    .detail("VerifyPeers", boost::algorithm::join(loaded.getVerifyPeers(), "|"));
			auto loadedTlsConfig = tlsConfig.loadSync();
			ConfigureSSLContext(loadedTlsConfig, newContext);
			activeTlsPolicy = makeReference<TLSPolicy>(loadedTlsConfig, onPolicyFailure);
			sslContextVar.set(ReferencedObject<boost::asio::ssl::context>::from(std::move(newContext)));
		} catch (Error& e) {
			TraceEvent("Net2TLSInitError").error(e);
		}
		backgroundCertRefresh =
		    reloadCertificatesOnChange(tlsConfig, onPolicyFailure, &sslContextVar, &activeTlsPolicy);
	}

	// If a TLS connection is actually going to be used then start background threads if configured
	if (targetState > ETLSInitState::CONFIG) {
		int threadsToStart;
		switch (targetState) {
		case ETLSInitState::CONNECT:
			threadsToStart = FLOW_KNOBS->TLS_CLIENT_HANDSHAKE_THREADS;
			break;
		case ETLSInitState::LISTEN:
			threadsToStart = FLOW_KNOBS->TLS_SERVER_HANDSHAKE_THREADS;
			break;
		default:
			threadsToStart = 0;
		};
		threadsToStart -= sslHandshakerThreadsStarted;

		if (threadsToStart > 0) {
			if (sslHandshakerThreadsStarted == 0) {
#if defined(__linux__)
				if (mallopt(M_ARENA_MAX, FLOW_KNOBS->TLS_MALLOC_ARENA_MAX) != 1) {
					TraceEvent(SevWarn, "TLSMallocSetMaxArenasFailure")
					    .detail("MaxArenas", FLOW_KNOBS->TLS_MALLOC_ARENA_MAX);
				};
#endif
				sslHandshakerPool = createGenericThreadPool(FLOW_KNOBS->TLS_HANDSHAKE_THREAD_STACKSIZE);
			}

			for (int i = 0; i < threadsToStart; ++i) {
				++sslHandshakerThreadsStarted;
				sslHandshakerPool->addThread(new SSLHandshakerThread(), "fdb-ssl-connect");
			}
		}
	}

	tlsInitializedState = targetState;
}

															#line 4739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via logTimeOffset()
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class Net2_LogTimeOffsetActor>
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class Net2_LogTimeOffsetActorState {
															#line 4745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Net2_LogTimeOffsetActorState() 
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	{
															#line 4751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		fdb_probe_actor_create("logTimeOffset", reinterpret_cast<unsigned long>(this));

	}
	~Net2_LogTimeOffsetActorState() 
	{
		fdb_probe_actor_destroy("logTimeOffset", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			;
															#line 4765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		this->~Net2_LogTimeOffsetActorState();
		static_cast<Net2_LogTimeOffsetActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		double processTime = timer_monotonic();
															#line 1375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		double systemTime = timer();
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent("ProcessTimeOffset") .detailf("ProcessTime", "%lf", processTime) .detailf("SystemTime", "%lf", systemTime) .detailf("OffsetFromSystemTime", "%lf", processTime - systemTime);
															#line 1380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = ::delay(FLOW_KNOBS->TIME_OFFSET_LOGGING_INTERVAL);
															#line 1380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<Net2_LogTimeOffsetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Net2_LogTimeOffsetActor*>(this)->actor_wait_state = 1;
															#line 1380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Net2_LogTimeOffsetActor, 0, Void >*>(static_cast<Net2_LogTimeOffsetActor*>(this)));
															#line 4808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<Net2_LogTimeOffsetActor*>(this)->actor_wait_state > 0) static_cast<Net2_LogTimeOffsetActor*>(this)->actor_wait_state = 0;
		static_cast<Net2_LogTimeOffsetActor*>(this)->ActorCallback< Net2_LogTimeOffsetActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Net2_LogTimeOffsetActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("logTimeOffset", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("logTimeOffset", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Net2_LogTimeOffsetActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("logTimeOffset", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("logTimeOffset", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Net2_LogTimeOffsetActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("logTimeOffset", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("logTimeOffset", reinterpret_cast<unsigned long>(this), 0);

	}
};
// This generated class is to be used only via logTimeOffset()
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class Net2_LogTimeOffsetActor final : public Actor<Void>, public ActorCallback< Net2_LogTimeOffsetActor, 0, Void >, public FastAllocated<Net2_LogTimeOffsetActor>, public Net2_LogTimeOffsetActorState<Net2_LogTimeOffsetActor> {
															#line 4892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<Net2_LogTimeOffsetActor>::operator new;
	using FastAllocated<Net2_LogTimeOffsetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Net2_LogTimeOffsetActor, 0, Void >;
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Net2_LogTimeOffsetActor() 
															#line 4903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   Net2_LogTimeOffsetActorState<Net2_LogTimeOffsetActor>()
	{
		fdb_probe_actor_enter("logTimeOffset", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("logTimeOffset");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("logTimeOffset", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Net2_LogTimeOffsetActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] Future<Void> Net2::logTimeOffset(  ) {
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new Net2_LogTimeOffsetActor());
															#line 4930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

void Net2::initMetrics() {
	bytesReceived.init("Net2.BytesReceived"_sr);
	countWriteProbes.init("Net2.CountWriteProbes"_sr);
	countReadProbes.init("Net2.CountReadProbes"_sr);
	countReads.init("Net2.CountReads"_sr);
	countWouldBlock.init("Net2.CountWouldBlock"_sr);
	countWrites.init("Net2.CountWrites"_sr);
	countRunLoop.init("Net2.CountRunLoop"_sr);
	countTasks.init("Net2.CountTasks"_sr);
	countYields.init("Net2.CountYields"_sr);
	countYieldBigStack.init("Net2.CountYieldBigStack"_sr);
	countYieldCalls.init("Net2.CountYieldCalls"_sr);
	countASIOEvents.init("Net2.CountASIOEvents"_sr);
	countYieldCallsTrue.init("Net2.CountYieldCallsTrue"_sr);
	countRunLoopProfilingSignals.init("Net2.CountRunLoopProfilingSignals"_sr);
	countTLSPolicyFailures.init("Net2.CountTLSPolicyFailures"_sr);
	priorityMetric.init("Net2.Priority"_sr);
	awakeMetric.init("Net2.Awake"_sr);
	slowTaskMetric.init("Net2.SlowTask"_sr);
	countLaunchTime.init("Net2.CountLaunchTime"_sr);
	countReactTime.init("Net2.CountReactTime"_sr);
	taskQueue.initMetrics();
}

bool Net2::checkRunnable() {
	return !started.exchange(true);
}

#ifdef ENABLE_SAMPLING
ActorLineageSet& Net2::getActorLineageSet() {
	return actorLineageSet;
}
#endif

void Net2::run() {
	TraceEvent::setNetworkThread();
	TraceEvent("Net2Running").log();

	thread_network = this;

	unsigned int tasksSinceReact = 0;

#ifdef WIN32
	if (timeBeginPeriod(1) != TIMERR_NOERROR)
		TraceEvent(SevError, "TimeBeginPeriodError").log();
#endif

	timeOffsetLogger = logTimeOffset();
	const char* flow_profiler_enabled = getenv("FLOW_PROFILER_ENABLED");
	if (flow_profiler_enabled != nullptr && *flow_profiler_enabled != '\0') {
		// The empty string check is to allow running `FLOW_PROFILER_ENABLED= ./fdbserver` to force disabling flow
		// profiling at startup.
		startProfiling(this);
	}

	// Get the address to the launch function
	typedef void (*runCycleFuncPtr)();
	runCycleFuncPtr runFunc = reinterpret_cast<runCycleFuncPtr>(
	    reinterpret_cast<flowGlobalType>(g_network->global(INetwork::enRunCycleFunc)));

	started.store(true);
	double nnow = timer_monotonic();

	while (!stopped) {
		FDB_TRACE_PROBE(run_loop_begin);
		++countRunLoop;

		if (runFunc) {
			tscBegin = timestampCounter();
			taskBegin = nnow;
			trackAtPriority(TaskPriority::RunCycleFunction, taskBegin);
			runFunc();
			double taskEnd = timer_monotonic();
			trackAtPriority(TaskPriority::RunLoop, taskEnd);
			countLaunchTime += taskEnd - taskBegin;
			checkForSlowTask(tscBegin, timestampCounter(), taskEnd - taskBegin, TaskPriority::RunCycleFunction);
		}

		double sleepTime = 0;
		if (taskQueue.canSleep()) {
			sleepTime = 1e99;
			double sleepStart = timer_monotonic();
			sleepTime = taskQueue.getSleepTime(sleepStart);
			if (sleepTime > 0) {
#if defined(__linux__)
				// notify the run loop monitoring thread that we have gone idle
				net2RunLoopSleeps.fetch_add(1);
#endif

				trackAtPriority(TaskPriority::Zero, sleepStart);
				awakeMetric = false;
				priorityMetric = 0;
				reactor.sleep(sleepTime);
				awakeMetric = true;
			}
		}

		tscBegin = timestampCounter();
		taskBegin = timer_monotonic();
		trackAtPriority(TaskPriority::ASIOReactor, taskBegin);
		reactor.react();
		tasksSinceReact = 0;

		updateNow();
		double now = this->currentTime;
		trackAtPriority(TaskPriority::RunLoop, now);

		countReactTime += now - taskBegin;
		checkForSlowTask(tscBegin, timestampCounter(), now - taskBegin, TaskPriority::ASIOReactor);

		if ((now - nnow) > FLOW_KNOBS->SLOW_LOOP_CUTOFF &&
		    nondeterministicRandom()->random01() < (now - nnow) * FLOW_KNOBS->SLOW_LOOP_SAMPLING_RATE)
			TraceEvent("SomewhatSlowRunLoopTop").detail("Elapsed", now - nnow);

		taskQueue.processReadyTimers(now);

		taskQueue.processThreadReady();

		tscBegin = timestampCounter();
		tscEnd = tscBegin + FLOW_KNOBS->TSC_YIELD_TIME;
		taskBegin = timer_monotonic();
		numYields = 0;
		TaskPriority minTaskID = TaskPriority::Max;
		[[maybe_unused]] int queueSize = taskQueue.getNumReadyTasks();

		FDB_TRACE_PROBE(run_loop_tasks_start, queueSize);
		while (taskQueue.hasReadyTask()) {
			++countTasks;
			currentTaskID = taskQueue.getReadyTaskID();
			priorityMetric = static_cast<int64_t>(currentTaskID);
			PromiseTask* task = taskQueue.getReadyTask();
			taskQueue.popReadyTask();

			try {
				++tasksSinceReact;
				(*task)();
			} catch (Error& e) {
				TraceEvent(SevError, "TaskError").error(e);
			} catch (...) {
				TraceEvent(SevError, "TaskError").error(unknown_error());
			}

			if (currentTaskID < minTaskID) {
				trackAtPriority(currentTaskID, taskBegin);
				minTaskID = currentTaskID;
			}

			// attempt to empty out the IO backlog
			if (tasksSinceReact >= FLOW_KNOBS->TASKS_PER_REACTOR_CHECK) {
				if (runFunc) {
					runFunc();
				}
				reactor.react();
				tasksSinceReact = 0;
			}

			double tscNow = timestampCounter();
			double newTaskBegin = timer_monotonic();
			if (check_yield(TaskPriority::Max, tscNow)) {
				checkForSlowTask(tscBegin, tscNow, newTaskBegin - taskBegin, currentTaskID);
				taskBegin = newTaskBegin;
				FDB_TRACE_PROBE(run_loop_yield);
				++countYields;
				break;
			}

			taskBegin = newTaskBegin;
			tscBegin = tscNow;
		}

		trackAtPriority(TaskPriority::RunLoop, taskBegin);

		queueSize = taskQueue.getNumReadyTasks();
		FDB_TRACE_PROBE(run_loop_done, queueSize);

#if defined(__linux__)
		if (FLOW_KNOBS->RUN_LOOP_PROFILING_INTERVAL > 0) {
			sigset_t orig_set;
			pthread_sigmask(SIG_BLOCK, &sigprof_set, &orig_set);

			size_t other_offset = net2backtraces_offset;
			bool was_overflow = net2backtraces_overflow;
			int signal_count = net2backtraces_count;

			countRunLoopProfilingSignals += signal_count;

			if (other_offset) {
				volatile void** _traces = net2backtraces;
				net2backtraces = other_backtraces;
				other_backtraces = _traces;

				net2backtraces_offset = 0;
			}

			net2backtraces_overflow = false;
			net2backtraces_count = 0;

			pthread_sigmask(SIG_SETMASK, &orig_set, nullptr);

			if (was_overflow) {
				TraceEvent("Net2RunLoopProfilerOverflow")
				    .detail("SignalsReceived", signal_count)
				    .detail("BackTraceHarvested", other_offset != 0);
			}
			if (other_offset) {
				size_t iter_offset = 0;
				while (iter_offset < other_offset) {
					ProfilingSample* ps = (ProfilingSample*)(other_backtraces + iter_offset);
					TraceEvent(SevWarn, "Net2RunLoopTrace")
					    .detailf("TraceTime", "%.6f", ps->timestamp)
					    .detail("Trace", platform::format_backtrace(ps->frames, ps->length));
					iter_offset += ps->length + 2;
				}
			}

			// notify the run loop monitoring thread that we are making progress
			net2RunLoopIterations.fetch_add(1);
		}
#endif
		nnow = timer_monotonic();

		if ((nnow - now) > FLOW_KNOBS->SLOW_LOOP_CUTOFF &&
		    nondeterministicRandom()->random01() < (nnow - now) * FLOW_KNOBS->SLOW_LOOP_SAMPLING_RATE)
			TraceEvent("SomewhatSlowRunLoopBottom")
			    .detail("Elapsed", nnow - now); // This includes the time spent running tasks
	}

	for (auto& fn : stopCallbacks) {
		fn();
	}

#ifdef WIN32
	timeEndPeriod(1);
#endif
} // Net2::run

// Updates the PriorityStats found in NetworkMetrics
void Net2::updateStarvationTracker(struct NetworkMetrics::PriorityStats& binStats,
                                   TaskPriority priority,
                                   TaskPriority lastPriority,
                                   double now) {

	// Busy -> idle at binStats.priority
	if (binStats.priority > priority && binStats.priority <= lastPriority) {
		binStats.active = false;
		binStats.duration += now - binStats.windowedTimer;
		binStats.maxDuration = std::max(binStats.maxDuration, now - binStats.timer);
	}

	// Idle -> busy at binStats.priority
	else if (binStats.priority <= priority && binStats.priority > lastPriority) {
		binStats.active = true;
		binStats.timer = now;
		binStats.windowedTimer = now;
	}
}

// Update both vectors of starvation trackers (one that updates every 5s and the other every 1s)
void Net2::trackAtPriority(TaskPriority priority, double now) {
	if (lastPriorityStats == nullptr || priority != lastPriorityStats->priority) {
		// Start tracking current priority
		auto activeStatsItr = networkInfo.metrics.activeTrackers.try_emplace(priority, priority);
		activeStatsItr.first->second.active = true;
		activeStatsItr.first->second.windowedTimer = now;

		if (lastPriorityStats != nullptr) {
			// Stop tracking previous priority
			lastPriorityStats->active = false;
			lastPriorityStats->duration += now - lastPriorityStats->windowedTimer;
		}

		// Update starvation trackers
		TaskPriority lastPriority = (lastPriorityStats == nullptr) ? TaskPriority::Zero : lastPriorityStats->priority;
		for (auto& binStats : networkInfo.metrics.starvationTrackers) {
			if (binStats.priority > lastPriority && binStats.priority > priority) {
				break;
			}
			updateStarvationTracker(binStats, priority, lastPriority, now);
		}

		// Update starvation trackers for network busyness
		updateStarvationTracker(networkInfo.metrics.starvationTrackerNetworkBusyness, priority, lastPriority, now);

		lastPriorityStats = &activeStatsItr.first->second;
	}
}

void Net2::checkForSlowTask(int64_t tscBegin, int64_t tscEnd, double duration, TaskPriority priority) {
	int64_t elapsed = tscEnd - tscBegin;
	if (elapsed > FLOW_KNOBS->TSC_YIELD_TIME && tscBegin > 0) {
		int i = std::min<double>(NetworkMetrics::SLOW_EVENT_BINS - 1, log(elapsed / 1e6) / log(2.));
		++networkInfo.metrics.countSlowEvents[i];
		int64_t warnThreshold = g_network->isSimulated() ? 10e9 : 500e6;

		// printf("SlowTask: %d, %d yields\n", (int)(elapsed/1e6), numYields);

		slowTaskMetric->clocks = elapsed;
		slowTaskMetric->duration = (int64_t)(duration * 1e9);
		slowTaskMetric->priority = static_cast<int64_t>(priority);
		slowTaskMetric->numYields = numYields;
		slowTaskMetric->log();

		double sampleRate = std::min(1.0, (elapsed > warnThreshold) ? 1.0 : elapsed / 10e9);
		double slowTaskProfilingLogInterval =
		    std::max(FLOW_KNOBS->RUN_LOOP_PROFILING_INTERVAL, FLOW_KNOBS->SLOWTASK_PROFILING_LOG_INTERVAL);
		if (slowTaskProfilingLogInterval > 0 && duration > slowTaskProfilingLogInterval) {
			sampleRate = 1; // Always include slow task events that could show up in our slow task profiling.
		}

		if (!DEBUG_DETERMINISM && (nondeterministicRandom()->random01() < sampleRate))
			TraceEvent(elapsed > warnThreshold ? SevWarnAlways : SevInfo, "SlowTask")
			    .detail("TaskID", priority)
			    .detail("MClocks", elapsed / 1e6)
			    .detail("Duration", duration)
			    .detail("SampleRate", sampleRate)
			    .detail("NumYields", numYields);
	}
}

bool Net2::check_yield(TaskPriority taskID, int64_t tscNow) {
	// SOMEDAY: Yield if there are lots of higher priority tasks queued?
	if ((g_stackYieldLimit) && ((intptr_t)&taskID < g_stackYieldLimit)) {
		++countYieldBigStack;
		return true;
	}

	taskQueue.processThreadReady();

	if (taskID == TaskPriority::DefaultYield)
		taskID = currentTaskID;
	if (taskQueue.hasReadyTask() && taskQueue.getReadyTaskPriority() > int64_t(taskID) << 32) {
		return true;
	}

	if (tscNow < tscBegin) {
		return true;
	}

	if (tscNow > tscEnd) {
		++numYields;
		return true;
	}

	return false;
}

bool Net2::check_yield(TaskPriority taskID) {
	if (numYields > 0) {
		++numYields;
		return true;
	}

	return check_yield(taskID, timestampCounter());
}

Future<class Void> Net2::yield(TaskPriority taskID) {
	++countYieldCalls;
	if (taskID == TaskPriority::DefaultYield)
		taskID = currentTaskID;
	if (check_yield(taskID)) {
		++countYieldCallsTrue;
		return delay(0, taskID);
	}
	g_network->setCurrentTask(taskID);
	return Void();
}

Future<Void> Net2::delay(double seconds, TaskPriority taskId) {
	if (seconds >= 4e12) // Intervals that overflow an int64_t in microseconds (more than 100,000 years) are treated
	                     // as infinite
		return Never();

	PromiseTask* t = new PromiseTask;
	if (seconds <= 0.) {
		taskQueue.addReady(taskId, t);
	} else {
		double at = now() + seconds;
		taskQueue.addTimer(at, taskId, t);
	}
	return t->promise.getFuture();
}

Future<Void> Net2::orderedDelay(double seconds, TaskPriority taskId) {
	// The regular delay already provides the required ordering property
	return delay(seconds, taskId);
}

void Net2::onMainThread(Promise<Void>&& signal, TaskPriority taskID) {
	if (stopped)
		return;
	PromiseTask* p = new PromiseTask(std::move(signal));
	if (taskQueue.addReadyThreadSafe(isOnMainThread(), taskID, p)) {
		reactor.wake();
	}
}

THREAD_HANDLE Net2::startThread(THREAD_FUNC_RETURN (*func)(void*), void* arg, int stackSize, const char* name) {
	return ::startThread(func, arg, stackSize, name);
}

Future<Reference<IConnection>> Net2::connect(NetworkAddress toAddr, tcp::socket* existingSocket) {
	if (toAddr.isTLS()) {
		initTLS(ETLSInitState::CONNECT);
		return SSLConnection::connect(&this->reactor.ios, this->sslContextVar.get(), toAddr, existingSocket);
	}

	return Connection::connect(&this->reactor.ios, toAddr);
}

Future<Reference<IConnection>> Net2::connectExternal(NetworkAddress toAddr) {
	return connect(toAddr);
}

Future<Reference<IUDPSocket>> Net2::createUDPSocket(NetworkAddress toAddr) {
	return UDPSocket::connect(&reactor.ios, toAddr, toAddr.ip.isV6());
}

Future<Reference<IUDPSocket>> Net2::createUDPSocket(bool isV6) {
	return UDPSocket::connect(&reactor.ios, Optional<NetworkAddress>(), isV6);
}

															#line 5356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via resolveTCPEndpoint_impl()
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ResolveTCPEndpoint_implActor>
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ResolveTCPEndpoint_implActorState {
															#line 5362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ResolveTCPEndpoint_implActorState(Net2* const& self,std::string const& host,std::string const& service) 
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   host(host),
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   service(service),
															#line 1808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   tcpResolver(self->reactor.ios)
															#line 5375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this));

	}
	~ResolveTCPEndpoint_implActorState() 
	{
		fdb_probe_actor_destroy("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			Promise<std::vector<NetworkAddress>> promise;
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			result = promise.getFuture();
															#line 1812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			tcpResolver.async_resolve( host, service, [promise](const boost::system::error_code& ec, tcp::resolver::iterator iter) { if (ec) { promise.sendError(lookup_failed()); return; } std::vector<NetworkAddress> addrs; tcp::resolver::iterator end; while (iter != end) { auto endpoint = iter->endpoint(); auto addr = endpoint.address(); if (addr.is_v6()) { if (!addr.is_loopback()) { addrs.emplace_back(IPAddress(addr.to_v6().to_bytes()), endpoint.port()); } } else { addrs.emplace_back(addr.to_v4().to_ulong(), endpoint.port()); } ++iter; } if (addrs.empty()) { promise.sendError(lookup_failed()); } else { promise.send(addrs); } });
															#line 5394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			try {
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = ready(result);
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<ResolveTCPEndpoint_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ResolveTCPEndpoint_implActor*>(this)->actor_wait_state = 1;
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >*>(static_cast<ResolveTCPEndpoint_implActor*>(this)));
															#line 5405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				loopDepth = 0;
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
		this->~ResolveTCPEndpoint_implActorState();
		static_cast<ResolveTCPEndpoint_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		tcpResolver.cancel();
															#line 1852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		std::vector<NetworkAddress> ret = result.get();
															#line 1853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->dnsCache.add(host, service, ret);
															#line 1855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ResolveTCPEndpoint_implActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(ret); this->~ResolveTCPEndpoint_implActorState(); static_cast<ResolveTCPEndpoint_implActor*>(this)->destroy(); return 0; }
															#line 5440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ResolveTCPEndpoint_implActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(ret);
		this->~ResolveTCPEndpoint_implActorState();
		static_cast<ResolveTCPEndpoint_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() == error_code_lookup_failed)
															#line 5453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->dnsCache.remove(host, service);
															#line 5457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 5461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ResolveTCPEndpoint_implActor*>(this)->actor_wait_state > 0) static_cast<ResolveTCPEndpoint_implActor*>(this)->actor_wait_state = 0;
		static_cast<ResolveTCPEndpoint_implActor*>(this)->ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Net2* self;
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::string host;
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::string service;
															#line 1808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::resolver tcpResolver;
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Future<std::vector<NetworkAddress>> result;
															#line 5569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via resolveTCPEndpoint_impl()
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ResolveTCPEndpoint_implActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >, public FastAllocated<ResolveTCPEndpoint_implActor>, public ResolveTCPEndpoint_implActorState<ResolveTCPEndpoint_implActor> {
															#line 5574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ResolveTCPEndpoint_implActor>::operator new;
	using FastAllocated<ResolveTCPEndpoint_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >;
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ResolveTCPEndpoint_implActor(Net2* const& self,std::string const& host,std::string const& service) 
															#line 5585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<std::vector<NetworkAddress>>(),
		   ResolveTCPEndpoint_implActorState<ResolveTCPEndpoint_implActor>(self, host, service)
	{
		fdb_probe_actor_enter("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resolveTCPEndpoint_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("resolveTCPEndpoint_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<std::vector<NetworkAddress>> resolveTCPEndpoint_impl( Net2* const& self, std::string const& host, std::string const& service ) {
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new ResolveTCPEndpoint_implActor(self, host, service));
															#line 5612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

Future<std::vector<NetworkAddress>> Net2::resolveTCPEndpoint(const std::string& host, const std::string& service) {
	return resolveTCPEndpoint_impl(this, host, service);
}

Future<std::vector<NetworkAddress>> Net2::resolveTCPEndpointWithDNSCache(const std::string& host,
                                                                         const std::string& service) {
	if (FLOW_KNOBS->ENABLE_COORDINATOR_DNS_CACHE) {
		Optional<std::vector<NetworkAddress>> cache = dnsCache.find(host, service);
		if (cache.present()) {
			return cache.get();
		}
	}
	return resolveTCPEndpoint_impl(this, host, service);
}

std::vector<NetworkAddress> Net2::resolveTCPEndpointBlocking(const std::string& host, const std::string& service) {
	tcp::resolver tcpResolver(reactor.ios);
	try {
		auto iter = tcpResolver.resolve(host, service);
		decltype(iter) end;
		std::vector<NetworkAddress> addrs;
		while (iter != end) {
			auto endpoint = iter->endpoint();
			auto addr = endpoint.address();
			if (addr.is_v6()) {
				addrs.emplace_back(IPAddress(addr.to_v6().to_bytes()), endpoint.port());
			} else {
				addrs.emplace_back(addr.to_v4().to_ulong(), endpoint.port());
			}
			++iter;
		}
		if (addrs.empty()) {
			throw lookup_failed();
		}
		return addrs;
	} catch (...) {
		dnsCache.remove(host, service);
		throw lookup_failed();
	}
}

std::vector<NetworkAddress> Net2::resolveTCPEndpointBlockingWithDNSCache(const std::string& host,
                                                                         const std::string& service) {
	if (FLOW_KNOBS->ENABLE_COORDINATOR_DNS_CACHE) {
		Optional<std::vector<NetworkAddress>> cache = dnsCache.find(host, service);
		if (cache.present()) {
			return cache.get();
		}
	}
	return resolveTCPEndpointBlocking(host, service);
}

bool Net2::isAddressOnThisHost(NetworkAddress const& addr) const {
	auto it = addressOnHostCache.find(addr.ip);
	if (it != addressOnHostCache.end())
		return it->second;

	if (addressOnHostCache.size() > 50000)
		addressOnHostCache.clear(); // Bound cache memory; should not really happen

	try {
		boost::asio::io_service ioService;
		boost::asio::ip::udp::socket socket(ioService);
		boost::asio::ip::udp::endpoint endpoint(tcpAddress(addr.ip), 1);
		socket.connect(endpoint);
		bool local = addr.ip.isV6() ? socket.local_endpoint().address().to_v6().to_bytes() == addr.ip.toV6()
		                            : socket.local_endpoint().address().to_v4().to_ulong() == addr.ip.toV4();
		socket.close();
		if (local)
			TraceEvent(SevInfo, "AddressIsOnHost").detail("Address", addr);
		return addressOnHostCache[addr.ip] = local;
	} catch (boost::system::system_error e) {
		TraceEvent(SevWarnAlways, "IsAddressOnHostError")
		    .detail("Address", addr)
		    .detail("ErrDesc", e.what())
		    .detail("ErrCode", e.code().value());
		return addressOnHostCache[addr.ip] = false;
	}
}

Reference<IListener> Net2::listen(NetworkAddress localAddr) {
	try {
		if (localAddr.isTLS()) {
			initTLS(ETLSInitState::LISTEN);
			return Reference<IListener>(new SSLListener(reactor.ios, &this->sslContextVar, localAddr));
		}
		return Reference<IListener>(new Listener(reactor.ios, localAddr));
	} catch (boost::system::system_error const& e) {
		Error x;
		if (e.code().value() == EADDRINUSE)
			x = address_in_use();
		else if (e.code().value() == EADDRNOTAVAIL)
			x = invalid_local_address();
		else
			x = bind_failed();
		TraceEvent("Net2ListenError").error(x).detail("Message", e.what());
		throw x;
	} catch (std::exception const& e) {
		Error x = unknown_error();
		TraceEvent("Net2ListenError").error(x).detail("Message", e.what());
		throw x;
	} catch (Error& e) {
		TraceEvent("Net2ListenError").error(e);
		throw e;
	} catch (...) {
		Error x = unknown_error();
		TraceEvent("Net2ListenError").error(x);
		throw x;
	}
}

void Net2::getDiskBytes(std::string const& directory, int64_t& free, int64_t& total) {
	return ::getDiskBytes(directory, free, total);
}

#ifdef __linux__
#include <sys/prctl.h>
#include <pthread.h>
#include <sched.h>
#endif

ASIOReactor::ASIOReactor(Net2* net) : do_not_stop(ios), network(net), firstTimer(ios) {
#ifdef __linux__
	// Reactor flags are used only for experimentation, and are platform-specific
	if (FLOW_KNOBS->REACTOR_FLAGS & 1) {
		prctl(PR_SET_TIMERSLACK, 1, 0, 0, 0);
		printf("Set timerslack to 1ns\n");
	}

	if (FLOW_KNOBS->REACTOR_FLAGS & 2) {
		int ret;
		pthread_t this_thread = pthread_self();
		struct sched_param params;
		params.sched_priority = sched_get_priority_max(SCHED_FIFO);
		ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
		if (ret != 0)
			printf("Error setting priority (%d %d)\n", ret, errno);
		else
			printf("Set scheduler mode to SCHED_FIFO\n");
	}
#endif
}

void ASIOReactor::sleep(double sleepTime) {
	if (sleepTime > FLOW_KNOBS->BUSY_WAIT_THRESHOLD) {
		if (FLOW_KNOBS->REACTOR_FLAGS & 4) {
#ifdef __linux
			timespec tv;
			tv.tv_sec = 0;
			tv.tv_nsec = 20000;
			nanosleep(&tv, nullptr);
#endif
		} else {
			sleepTime -= FLOW_KNOBS->BUSY_WAIT_THRESHOLD;
			if (sleepTime < 4e12) {
				this->firstTimer.expires_from_now(boost::posix_time::microseconds(int64_t(sleepTime * 1e6)));
				this->firstTimer.async_wait(&nullWaitHandler);
			}
			setProfilingEnabled(0); // The following line generates false positives for slow task profiling
			ios.run_one();
			setProfilingEnabled(1);
			this->firstTimer.cancel();
		}
		++network->countASIOEvents;
	} else if (sleepTime > 0) {
		if (!(FLOW_KNOBS->REACTOR_FLAGS & 8))
			threadYield();
	}
}

void ASIOReactor::react() {
	while (ios.poll_one())
		++network->countASIOEvents; // Make this a task?
}

void ASIOReactor::wake() {
	ios.post(nullCompletionHandler);
}

} // namespace N2

SendBufferIterator::SendBufferIterator(SendBuffer const* p, int limit) : p(p), limit(limit) {
	ASSERT(limit > 0);
}

void SendBufferIterator::operator++() {
	limit -= p->bytes_written - p->bytes_sent;
	if (limit > 0)
		p = p->next;
	else
		p = nullptr;
}

boost::asio::const_buffer SendBufferIterator::operator*() const {
	return boost::asio::const_buffer(p->data() + p->bytes_sent, std::min(limit, p->bytes_written - p->bytes_sent));
}

INetwork* newNet2(const TLSConfig& tlsConfig, bool useThreadPool, bool useMetrics) {
	try {
		N2::g_net2 = new N2::Net2(tlsConfig, useThreadPool, useMetrics);
	} catch (boost::system::system_error e) {
		TraceEvent("Net2InitError").detail("Message", e.what());
		throw unknown_error();
	} catch (std::exception const& e) {
		TraceEvent("Net2InitError").detail("Message", e.what());
		throw unknown_error();
	}

	return N2::g_net2;
}

struct TestGVR {
	Standalone<StringRef> key;
	int64_t version;
	Optional<std::pair<UID, UID>> debugID;
	Promise<Optional<Standalone<StringRef>>> reply;

	TestGVR() {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, key, version, debugID, reply);
	}
};

template <class F>
THREAD_HANDLE startThreadF(F&& func) {
	struct Thing {
		F f;
		Thing(F&& f) : f(std::move(f)) {}
		THREAD_FUNC start(void* p) {
			Thing* self = (Thing*)p;
			self->f();
			delete self;
			THREAD_RETURN;
		}
	};
	Thing* t = new Thing(std::move(func));
	return g_network->startThread(Thing::start, t);
}

															#line 5858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase2099()
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class FlowTestCase2099Actor>
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class FlowTestCase2099ActorState {
															#line 5865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowTestCase2099ActorState(UnitTestParameters const& params) 
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : params(params)
															#line 5872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase2099", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase2099ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase2099", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ThreadSafeQueue<int> tq;
															#line 2101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(!tq.pop().present());
															#line 2102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.canSleep());
															#line 2104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.push(1) == true);
															#line 2105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(!tq.canSleep());
															#line 2106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(!tq.canSleep());
															#line 2107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.push(2) == false);
															#line 2108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.push(3) == false);
															#line 2110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.pop().get() == 1);
															#line 2111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.pop().get() == 2);
															#line 2112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.push(4) == false);
															#line 2113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.pop().get() == 3);
															#line 2114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.pop().get() == 4);
															#line 2115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(!tq.pop().present());
															#line 2116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(tq.canSleep());
															#line 2117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (!static_cast<FlowTestCase2099Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2099ActorState(); static_cast<FlowTestCase2099Actor*>(this)->destroy(); return 0; }
															#line 5917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			new (&static_cast<FlowTestCase2099Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase2099ActorState();
			static_cast<FlowTestCase2099Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase2099ActorState();
		static_cast<FlowTestCase2099Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	UnitTestParameters params;
															#line 5941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase2099()
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class FlowTestCase2099Actor final : public Actor<Void>, public FastAllocated<FlowTestCase2099Actor>, public FlowTestCase2099ActorState<FlowTestCase2099Actor> {
															#line 5946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase2099Actor>::operator new;
	using FastAllocated<FlowTestCase2099Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowTestCase2099Actor(UnitTestParameters const& params) 
															#line 5956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase2099ActorState<FlowTestCase2099Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase2099", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase2099");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase2099", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static Future<Void> flowTestCase2099( UnitTestParameters const& params ) {
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new FlowTestCase2099Actor(params));
															#line 5983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase2099, "flow/Net2/ThreadSafeQueue/Interface")

#line 2119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

// A helper struct used by queueing tests which use multiple threads.
struct QueueTestThreadState {
	QueueTestThreadState(int threadId, int toProduce) : threadId(threadId), toProduce(toProduce) {}
	int threadId;
	THREAD_HANDLE handle;
	int toProduce;
	int produced = 0;
	Promise<Void> doneProducing;
	int consumed = 0;

	static int valueToThreadId(int value) { return value >> 20; }
	int elementValue(int index) { return index + (threadId << 20); }
	int nextProduced() { return elementValue(produced++); }
	int nextConsumed() { return elementValue(consumed++); }
	void checkDone() {
		ASSERT_EQ(produced, toProduce);
		ASSERT_EQ(consumed, produced);
	}
};

															#line 6009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase2140()
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class FlowTestCase2140Actor>
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class FlowTestCase2140ActorState {
															#line 6016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowTestCase2140ActorState(UnitTestParameters const& params) 
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : params(params)
															#line 6023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase2140", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase2140ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase2140", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			noUnseed = true;
															#line 2145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ThreadSafeQueue<int> queue;
															#line 2146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			perThread = { QueueTestThreadState(0, 1000000), QueueTestThreadState(1, 100000), QueueTestThreadState(2, 1000000) };
															#line 2149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			doneProducing = std::vector<Future<Void>>();
															#line 2151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			int total = 0;
															#line 2152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			for(int t = 0;t < perThread.size();++t) {
															#line 2153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto& s = perThread[t];
															#line 2154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				doneProducing.push_back(s.doneProducing.getFuture());
															#line 2155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				total += s.toProduce;
															#line 2156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				s.handle = startThreadF([&queue, &s]() { printf("Thread%d\n", s.threadId); int nextYield = 0; while (s.produced < s.toProduce) { queue.push(s.nextProduced()); if (nextYield-- == 0) { std::this_thread::yield(); nextYield = nondeterministicRandom()->randomInt(0, 100); } } printf("T%dDone\n", s.threadId); s.doneProducing.send(Void()); });
															#line 6056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
															#line 2170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			int consumed = 0;
															#line 2171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			for(;consumed < total;) {
															#line 2172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Optional<int> element = queue.pop();
															#line 2173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (element.present())
															#line 6066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 2174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					int v = element.get();
															#line 2175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto& s = perThread[QueueTestThreadState::valueToThreadId(v)];
															#line 2176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					++consumed;
															#line 2177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					ASSERT(v == s.nextConsumed());
															#line 6076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 2179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					std::this_thread::yield();
															#line 6082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
															#line 2181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if ((consumed & 3) == 0)
															#line 6086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 2182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					queue.canSleep();
															#line 6090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
			}
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(doneProducing);
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state = 1;
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2140Actor, 0, Void >*>(static_cast<FlowTestCase2140Actor*>(this)));
															#line 6102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		this->~FlowTestCase2140ActorState();
		static_cast<FlowTestCase2140Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> signal;
															#line 2189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doneConsuming = signal.getFuture();
															#line 2190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		g_network->onMainThread(std::move(signal), TaskPriority::DefaultOnMainThread);
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = doneConsuming;
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state = 2;
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2140Actor, 1, Void >*>(static_cast<FlowTestCase2140Actor*>(this)));
															#line 6138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> signal;
															#line 2189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doneConsuming = signal.getFuture();
															#line 2190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		g_network->onMainThread(std::move(signal), TaskPriority::DefaultOnMainThread);
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = doneConsuming;
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state = 2;
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2140Actor, 1, Void >*>(static_cast<FlowTestCase2140Actor*>(this)));
															#line 6160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		if (static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase2140Actor*>(this)->ActorCallback< FlowTestCase2140Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase2140Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase2140Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase2140Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 2193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		for(int t = 0;t < perThread.size();++t) {
															#line 2194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			waitThread(perThread[t].handle);
															#line 2195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			perThread[t].checkDone();
															#line 6236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
															#line 2197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<FlowTestCase2140Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2140ActorState(); static_cast<FlowTestCase2140Actor*>(this)->destroy(); return 0; }
															#line 6240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<FlowTestCase2140Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase2140ActorState();
		static_cast<FlowTestCase2140Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 2193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		for(int t = 0;t < perThread.size();++t) {
															#line 2194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			waitThread(perThread[t].handle);
															#line 2195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			perThread[t].checkDone();
															#line 6256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
															#line 2197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<FlowTestCase2140Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2140ActorState(); static_cast<FlowTestCase2140Actor*>(this)->destroy(); return 0; }
															#line 6260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<FlowTestCase2140Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase2140ActorState();
		static_cast<FlowTestCase2140Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase2140Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase2140Actor*>(this)->ActorCallback< FlowTestCase2140Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase2140Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase2140Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase2140Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	UnitTestParameters params;
															#line 2146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::vector<QueueTestThreadState> perThread;
															#line 2149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::vector<Future<Void>> doneProducing;
															#line 2189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Future<Void> doneConsuming;
															#line 6339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase2140()
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class FlowTestCase2140Actor final : public Actor<Void>, public ActorCallback< FlowTestCase2140Actor, 0, Void >, public ActorCallback< FlowTestCase2140Actor, 1, Void >, public FastAllocated<FlowTestCase2140Actor>, public FlowTestCase2140ActorState<FlowTestCase2140Actor> {
															#line 6344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase2140Actor>::operator new;
	using FastAllocated<FlowTestCase2140Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase2140Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase2140Actor, 1, Void >;
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowTestCase2140Actor(UnitTestParameters const& params) 
															#line 6356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase2140ActorState<FlowTestCase2140Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase2140", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase2140");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase2140", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase2140Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase2140Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static Future<Void> flowTestCase2140( UnitTestParameters const& params ) {
															#line 2140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new FlowTestCase2140Actor(params));
															#line 6385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase2140, "flow/Net2/ThreadSafeQueue/Threaded")

#line 2199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

															#line 6391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase2200()
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class FlowTestCase2200Actor>
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class FlowTestCase2200ActorState {
															#line 6398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowTestCase2200ActorState(UnitTestParameters const& params) 
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : params(params)
															#line 6405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase2200", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase2200ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase2200", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			noUnseed = true;
															#line 2204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			perThread = { QueueTestThreadState(0, 1000000), QueueTestThreadState(1, 100000), QueueTestThreadState(2, 1000000) };
															#line 2207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			doneProducing = std::vector<Future<Void>>();
															#line 2208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			for(int t = 0;t < perThread.size();++t) {
															#line 2209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto& s = perThread[t];
															#line 2210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				doneProducing.push_back(s.doneProducing.getFuture());
															#line 2211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				s.handle = startThreadF([&s]() { int nextYield = 0; while (s.produced < s.toProduce) { if (nextYield-- == 0) { std::this_thread::yield(); nextYield = nondeterministicRandom()->randomInt(0, 100); } int v = s.nextProduced(); onMainThreadVoid([&s, v]() { ASSERT_EQ(v, s.nextConsumed()); }); } s.doneProducing.send(Void()); });
															#line 6432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(doneProducing);
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state = 1;
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2200Actor, 0, Void >*>(static_cast<FlowTestCase2200Actor*>(this)));
															#line 6443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		this->~FlowTestCase2200ActorState();
		static_cast<FlowTestCase2200Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> signal;
															#line 2228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doneConsuming = signal.getFuture();
															#line 2229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		g_network->onMainThread(std::move(signal), TaskPriority::DefaultOnMainThread);
															#line 2230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = doneConsuming;
															#line 2230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state = 2;
															#line 2230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2200Actor, 1, Void >*>(static_cast<FlowTestCase2200Actor*>(this)));
															#line 6479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> signal;
															#line 2228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doneConsuming = signal.getFuture();
															#line 2229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		g_network->onMainThread(std::move(signal), TaskPriority::DefaultOnMainThread);
															#line 2230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = doneConsuming;
															#line 2230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state = 2;
															#line 2230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2200Actor, 1, Void >*>(static_cast<FlowTestCase2200Actor*>(this)));
															#line 6501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		if (static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase2200Actor*>(this)->ActorCallback< FlowTestCase2200Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase2200Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase2200Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase2200Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		for(int t = 0;t < perThread.size();++t) {
															#line 2233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			waitThread(perThread[t].handle);
															#line 2234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			perThread[t].checkDone();
															#line 6577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
															#line 2236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<FlowTestCase2200Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2200ActorState(); static_cast<FlowTestCase2200Actor*>(this)->destroy(); return 0; }
															#line 6581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<FlowTestCase2200Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase2200ActorState();
		static_cast<FlowTestCase2200Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		for(int t = 0;t < perThread.size();++t) {
															#line 2233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			waitThread(perThread[t].handle);
															#line 2234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			perThread[t].checkDone();
															#line 6597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
															#line 2236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<FlowTestCase2200Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2200ActorState(); static_cast<FlowTestCase2200Actor*>(this)->destroy(); return 0; }
															#line 6601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<FlowTestCase2200Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase2200ActorState();
		static_cast<FlowTestCase2200Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase2200Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase2200Actor*>(this)->ActorCallback< FlowTestCase2200Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase2200Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase2200Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase2200Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	UnitTestParameters params;
															#line 2204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::vector<QueueTestThreadState> perThread;
															#line 2207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::vector<Future<Void>> doneProducing;
															#line 2228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Future<Void> doneConsuming;
															#line 6680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase2200()
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class FlowTestCase2200Actor final : public Actor<Void>, public ActorCallback< FlowTestCase2200Actor, 0, Void >, public ActorCallback< FlowTestCase2200Actor, 1, Void >, public FastAllocated<FlowTestCase2200Actor>, public FlowTestCase2200ActorState<FlowTestCase2200Actor> {
															#line 6685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase2200Actor>::operator new;
	using FastAllocated<FlowTestCase2200Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase2200Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase2200Actor, 1, Void >;
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowTestCase2200Actor(UnitTestParameters const& params) 
															#line 6697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase2200ActorState<FlowTestCase2200Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase2200", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase2200");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase2200", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase2200Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase2200Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static Future<Void> flowTestCase2200( UnitTestParameters const& params ) {
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new FlowTestCase2200Actor(params));
															#line 6726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase2200, "noSim/flow/Net2/onMainThreadFIFO")

#line 2238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

void net2_test(){
	/*
	g_network = newNet2();  // for promise serialization below

	Endpoint destination;

	printf("  Used: %lld\n", FastAllocator<4096>::getTotalMemory());

	char junk[100];

	double before = timer();

	std::vector<TestGVR> reqs;
	reqs.reserve( 10000 );

	int totalBytes = 0;
	for(int j=0; j<1000; j++) {
	    UnsentPacketQueue unsent;
	    ReliablePacketList reliable;

	    reqs.resize(10000);
	    for(int i=0; i<10000; i++) {
	        TestGVR &req = reqs[i];
	        req.key = "Foobar"_sr;

	        SerializeSource<TestGVR> what(req);

	        SendBuffer* pb = unsent.getWriteBuffer();
	        ReliablePacket* rp = new ReliablePacket;  // 0

	        PacketWriter wr(pb,rp,AssumeVersion(g_network->protocolVersion()));
	        //BinaryWriter wr;
	        SplitBuffer packetLen;
	        uint32_t len = 0;
	        wr.writeAhead(sizeof(len), &packetLen);
	        wr << destination.token;
	        //req.reply.getEndpoint();
	        what.serializePacketWriter(wr);
	        //wr.serializeBytes(junk, 43);

	        unsent.setWriteBuffer(wr.finish());
	        len = wr.size() - sizeof(len);
	        packetLen.write(&len, sizeof(len));

	        //totalBytes += wr.getLength();
	        totalBytes += wr.size();

	        if (rp) reliable.insert(rp);
	    }
	    reqs.clear();
	    unsent.discardAll();
	    reliable.discardAll();
	}

	printf("SimSend x 1Kx10K: %0.2f sec\n", timer()-before);
	printf("  Bytes: %d\n", totalBytes);
	printf("  Used: %lld\n", FastAllocator<4096>::getTotalMemory());
	*/
};
