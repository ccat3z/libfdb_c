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
#include "flow/Platform.h"
#include "flow/Trace.h"
#include <algorithm>
#include <memory>
#define BOOST_SYSTEM_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string/join.hpp>
#include "flow/network.h"
#include "flow/IThreadPool.h"

#include "flow/ActorCollection.h"
#include "flow/ThreadSafeQueue.h"
#include "flow/ThreadHelper.actor.h"
#include "flow/TDMetric.actor.h"
#include "flow/AsioReactor.h"
#include "flow/Profiler.h"
#include "flow/ProtocolVersion.h"
#include "flow/SendBufferIterator.h"
#include "flow/TLSConfig.actor.h"
#include "flow/genericactors.actor.h"
#include "flow/Util.h"

#ifdef ADDRESS_SANITIZER
#include <sanitizer/lsan_interface.h>
#endif

// See the comment in TLSConfig.actor.h for the explanation of why this module breaking include was done.
#include "fdbrpc/IAsyncFile.h"

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
	static StringRef typeName() { return LiteralStringRef("SlowTask"); }
	typedef SlowTask type;
	struct clocksDescriptor {
		static StringRef name() { return LiteralStringRef("clocks"); }
		static StringRef typeName() { return LiteralStringRef("int64_t"); }
		static StringRef comment() { return LiteralStringRef(" clocks"); }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	struct durationDescriptor {
		static StringRef name() { return LiteralStringRef("duration"); }
		static StringRef typeName() { return LiteralStringRef("int64_t"); }
		static StringRef comment() { return LiteralStringRef(" ns"); }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	struct priorityDescriptor {
		static StringRef name() { return LiteralStringRef("priority"); }
		static StringRef typeName() { return LiteralStringRef("int64_t"); }
		static StringRef comment() { return LiteralStringRef(" priority level"); }
		typedef int64_t type;
		static inline type get(SlowTask& from);
	};
	struct numYieldsDescriptor {
		static StringRef name() { return LiteralStringRef("numYields"); }
		static StringRef typeName() { return LiteralStringRef("int64_t"); }
		static StringRef comment() { return LiteralStringRef(" count"); }
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
#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"


namespace N2 { // No indent, it's the whole file

class Net2;
class Peer;
class Connection;

// Outlives main
Net2* g_net2 = nullptr;

class Task {
public:
	virtual void operator()() = 0;
};

struct OrderedTask {
	int64_t priority;
	TaskPriority taskID;
	Task* task;
	OrderedTask(int64_t priority, TaskPriority taskID, Task* task) : priority(priority), taskID(taskID), task(task) {}
	bool operator<(OrderedTask const& rhs) const { return priority < rhs.priority; }
};

template <class T>
class ReadyQueue : public std::priority_queue<T, std::vector<T>> {
public:
	typedef typename std::priority_queue<T, std::vector<T>>::size_type size_type;
	ReadyQueue(size_type capacity = 0) { reserve(capacity); };
	void reserve(size_type capacity) { this->c.reserve(capacity); }
};

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

	ProtocolVersion protocolVersion() const override { return currentProtocolVersion; }

	std::vector<flowGlobalType> globals;

	const TLSConfig& getTLSConfig() const override { return tlsConfig; }

	bool checkRunnable() override;

#ifdef ENABLE_SAMPLING
	ActorLineageSet& getActorLineageSet() override;
#endif

	bool useThreadPool;

	// private:

	ASIOReactor reactor;
#ifndef TLS_DISABLED
	AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>> sslContextVar;
	Reference<IThreadPool> sslHandshakerPool;
	int sslHandshakerThreadsStarted;
	int sslPoolHandshakesInProgress;
#endif
	TLSConfig tlsConfig;
	Future<Void> backgroundCertRefresh;
	ETLSInitState tlsInitializedState;

	INetworkConnections* network; // initially this, but can be changed

	int64_t tscBegin, tscEnd;
	double taskBegin;
	TaskPriority currentTaskID;
	uint64_t tasksIssued;
	TDMetricCollection tdmetrics;
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

	ReadyQueue<OrderedTask> ready;
	ThreadSafeQueue<OrderedTask> threadReady;

	struct DelayedTask : OrderedTask {
		double at;
		DelayedTask(double at, int64_t priority, TaskPriority taskID, Task* task)
		  : OrderedTask(priority, taskID, task), at(at) {}
		bool operator<(DelayedTask const& rhs) const { return at > rhs.at; } // Ordering is reversed for priority_queue
	};
	std::priority_queue<DelayedTask, std::vector<DelayedTask>> timers;

	void checkForSlowTask(int64_t tscBegin, int64_t tscEnd, double duration, TaskPriority priority);
	bool check_yield(TaskPriority taskId, int64_t tscNow);
	void processThreadReady();
	void trackAtPriority(TaskPriority priority, double now);
	void stopImmediately() {
#ifdef ADDRESS_SANITIZER
		// Do leak check before intentionally leaking a bunch of memory
		__lsan_do_leak_check();
#endif
		stopped = true;
		decltype(ready) _1;
		ready.swap(_1);
		decltype(timers) _2;
		timers.swap(_2);
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
	Int64MetricHandle countCantSleep;
	Int64MetricHandle countWontSleep;
	Int64MetricHandle countTimers;
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
	const char* errContext;
	UID errID;

public:
	BindPromise(const char* errContext, UID errID) : errContext(errContext), errID(errID) {}
	BindPromise(BindPromise const& r) : p(r.p), errContext(r.errContext), errID(r.errID) {}
	BindPromise(BindPromise&& r) noexcept : p(std::move(r.p)), errContext(r.errContext), errID(r.errID) {}

	Future<Void> getFuture() { return p.getFuture(); }

	void operator()(const boost::system::error_code& error, size_t bytesWritten = 0) {
		try {
			if (error) {
				// Log the error...
				{
					TraceEvent evt(SevWarn, errContext, errID);
					evt.suppressFor(1.0).detail("ErrorCode", error.value()).detail("Message", error.message());
#ifndef TLS_DISABLED
					// There is no function in OpenSSL to use to check if an error code is from OpenSSL,
					// but all OpenSSL errors have a non-zero "library" code set in bits 24-32, and linux
					// error codes should never go that high.
					if (error.value() >= (1 << 24L)) {
						evt.detail("WhichMeans", TLSPolicy::ErrorString(error));
					}
#endif
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
																#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectActor>
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActorState {
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActorState(boost::asio::io_service* const& ios,NetworkAddress const& addr) 
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : ios(ios),
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   addr(addr),
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   self(new Connection(*ios))
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->peer_address = addr;
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			try {
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto to = tcpEndpoint(addr);
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				BindPromise p("N2_ConnectError", self->id);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Future<Void> onConnected = p.getFuture();
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->socket.async_connect(to, std::move(p));
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = onConnected;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<ConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ConnectActor*>(this)->actor_wait_state = 1;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor, 0, Void >*>(static_cast<ConnectActor*>(this)));
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(self)); // state_var_RVO
		this->~ConnectActorState();
		static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	boost::asio::io_service* ios;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	NetworkAddress addr;
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<Connection> self;
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< ConnectActor, 0, Void >, public FastAllocated<ConnectActor>, public ConnectActorState<ConnectActor> {
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectActor>::operator new;
	using FastAllocated<ConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor, 0, Void >;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor(boost::asio::io_service* const& ios,NetworkAddress const& addr) 
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> connect( boost::asio::io_service* const& ios, NetworkAddress const& addr ) {
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new ConnectActor(ios, addr));
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

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
			    .detail("ErrorCode", error.value())
			    .detail("Message", error.message());
	}

	void onReadError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_ReadError", id)
		    .suppressFor(1.0)
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
	void onWriteError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_WriteError", id)
		    .suppressFor(1.0)
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
																#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectActor1>
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor1State {
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor1State(boost::asio::io_service* const& io_service,Optional<NetworkAddress> const& toAddress,bool const& isV6) 
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : io_service(io_service),
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   toAddress(toAddress),
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   isV6(isV6),
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   self(new UDPSocket(*io_service, toAddress, isV6))
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			ASSERT(!toAddress.present() || toAddress.get().ip.isV6() == isV6);
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (!toAddress.present())
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (!static_cast<ConnectActor1*>(this)->SAV<Reference<IUDPSocket>>::futures) { (void)(self); this->~ConnectActor1State(); static_cast<ConnectActor1*>(this)->destroy(); return 0; }
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				new (&static_cast<ConnectActor1*>(this)->SAV< Reference<IUDPSocket> >::value()) Reference<IUDPSocket>(std::move(self)); // state_var_RVO
				this->~ConnectActor1State();
				static_cast<ConnectActor1*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			try {
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (toAddress.present())
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto to = udpEndpoint(toAddress.get());
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					BindPromise p("N2_UDPConnectError", self->id);
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					Future<Void> onConnected = p.getFuture();
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					self->socket.async_connect(to, std::move(p));
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					StrictFuture<Void> __when_expr_0 = onConnected;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					if (static_cast<ConnectActor1*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ConnectActor1*>(this)->actor_wait_state = 1;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor1, 0, Void >*>(static_cast<ConnectActor1*>(this)));
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor1*>(this)->SAV<Reference<IUDPSocket>>::futures) { (void)(self); this->~ConnectActor1State(); static_cast<ConnectActor1*>(this)->destroy(); return 0; }
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	boost::asio::io_service* io_service;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Optional<NetworkAddress> toAddress;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	bool isV6;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<UDPSocket> self;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor1 final : public Actor<Reference<IUDPSocket>>, public ActorCallback< ConnectActor1, 0, Void >, public FastAllocated<ConnectActor1>, public ConnectActor1State<ConnectActor1> {
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectActor1>::operator new;
	using FastAllocated<ConnectActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IUDPSocket>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor1, 0, Void >;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor1(boost::asio::io_service* const& io_service,Optional<NetworkAddress> const& toAddress,bool const& isV6) 
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IUDPSocket>> connect( boost::asio::io_service* const& io_service, Optional<NetworkAddress> const& toAddress, bool const& isV6 ) {
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IUDPSocket>>(new ConnectActor1(io_service, toAddress, isV6));
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

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
																#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doAccept()
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoAcceptActor>
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActorState {
															#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActorState(Listener* const& self) 
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   conn(new Connection(self->io_service)),
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   peer_endpoint()
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				BindPromise p("N2_AcceptError", UID());
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto f = p.getFuture();
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->acceptor.async_accept(conn->getSocket(), peer_endpoint, std::move(p));
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = f;
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<DoAcceptActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoAcceptActor*>(this)->actor_wait_state = 1;
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoAcceptActor, 0, Void >*>(static_cast<DoAcceptActor*>(this)));
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			conn->close();
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port()));
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActorState(); static_cast<DoAcceptActor*>(this)->destroy(); return 0; }
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<DoAcceptActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~DoAcceptActorState();
		static_cast<DoAcceptActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port()));
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActorState(); static_cast<DoAcceptActor*>(this)->destroy(); return 0; }
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Listener* self;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<Connection> conn;
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::acceptor::endpoint_type peer_endpoint;
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doAccept()
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActor final : public Actor<Reference<IConnection>>, public ActorCallback< DoAcceptActor, 0, Void >, public FastAllocated<DoAcceptActor>, public DoAcceptActorState<DoAcceptActor> {
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoAcceptActor>::operator new;
	using FastAllocated<DoAcceptActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoAcceptActor, 0, Void >;
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActor(Listener* const& self) 
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> doAccept( Listener* const& self ) {
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new DoAcceptActor(self));
															#line 1496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
};

#ifndef TLS_DISABLED
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
				           h.type == ssl_socket::handshake_type::client ? "N2_ConnectHandshakeError"
				                                                        : "N2_AcceptHandshakeError")
				    .detail("ErrorCode", h.err.value())
				    .detail("ErrorMsg", h.err.message().c_str())
				    .detail("BackgroundThread", true);
				h.done.sendError(connection_failed());
			} else {
				h.done.send(Void());
			}
		} catch (...) {
			TraceEvent(SevWarn,
			           h.type == ssl_socket::handshake_type::client ? "N2_ConnectHandshakeUnknownError"
			                                                        : "N2_AcceptHandshakeUnknownError")
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
	    sslContext(context) {}

	explicit SSLConnection(Reference<ReferencedObject<boost::asio::ssl::context>> context, tcp::socket* existingSocket)
	  : id(nondeterministicRandom()->randomUniqueID()), socket(std::move(*existingSocket)),
	    ssl_sock(socket, context->mutate()), sslContext(context) {}

	// This is not part of the IConnection interface, because it is wrapped by INetwork::connect()
																#line 1566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectActor2>
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor2State {
															#line 1572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor2State(boost::asio::io_service* const& ios,Reference<ReferencedObject<boost::asio::ssl::context>> const& context,NetworkAddress const& addr,tcp::socket* const& existingSocket = nullptr) 
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : ios(ios),
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   context(context),
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   addr(addr),
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   existingSocket(existingSocket)
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			std::pair<IPAddress, uint16_t> peerIP = std::make_pair(addr.ip, addr.port);
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 1604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (now() < iter->second.second)
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					if (iter->second.first >= FLOW_KNOBS->TLS_CLIENT_CONNECTION_THROTTLE_ATTEMPTS)
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					{
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						TraceEvent("TLSOutgoingConnectionThrottlingWarning").suppressFor(1.0).detail("PeerIP", addr);
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						if (static_cast<ConnectActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
						static_cast<ConnectActor2*>(this)->actor_wait_state = 1;
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor2, 0, Void >*>(static_cast<ConnectActor2*>(this)));
															#line 1625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						loopDepth = 0;
					}
					else
					{
						loopDepth = a_body1cont3(loopDepth);
					}
				}
				else
				{
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler.erase(peerIP);
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (existingSocket != nullptr)
															#line 1666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			Reference<SSLConnection> self(new SSLConnection(context, existingSocket));
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->peer_address = addr;
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->init();
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (!static_cast<ConnectActor2*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActor2State(); static_cast<ConnectActor2*>(this)->destroy(); return 0; }
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			new (&static_cast<ConnectActor2*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(self);
			this->~ConnectActor2State();
			static_cast<ConnectActor2*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self = Reference<SSLConnection>(new SSLConnection(*ios, context));
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->peer_address = addr;
															#line 1686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			auto to = tcpEndpoint(self->peer_address);
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			BindPromise p("N2_ConnectError", self->id);
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			Future<Void> onConnected = p.getFuture();
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->socket.async_connect(to, std::move(p));
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = onConnected;
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectActor2*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ConnectActor2*>(this)->actor_wait_state = 2;
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectActor2, 1, Void >*>(static_cast<ConnectActor2*>(this)));
															#line 1705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 1732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 1740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

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
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor2*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActor2State(); static_cast<ConnectActor2*>(this)->destroy(); return 0; }
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ConnectActor2*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(self)); // state_var_RVO
		this->~ConnectActor2State();
		static_cast<ConnectActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->init();
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ConnectActor2*>(this)->SAV<Reference<IConnection>>::futures) { (void)(self); this->~ConnectActor2State(); static_cast<ConnectActor2*>(this)->destroy(); return 0; }
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	boost::asio::io_service* ios;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<ReferencedObject<boost::asio::ssl::context>> context;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	NetworkAddress addr;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::socket* existingSocket;
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectActor2 final : public Actor<Reference<IConnection>>, public ActorCallback< ConnectActor2, 0, Void >, public ActorCallback< ConnectActor2, 1, Void >, public FastAllocated<ConnectActor2>, public ConnectActor2State<ConnectActor2> {
															#line 1930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ConnectActor2>::operator new;
	using FastAllocated<ConnectActor2>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor2, 0, Void >;
friend struct ActorCallback< ConnectActor2, 1, Void >;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectActor2(boost::asio::io_service* const& ios,Reference<ReferencedObject<boost::asio::ssl::context>> const& context,NetworkAddress const& addr,tcp::socket* const& existingSocket = nullptr) 
															#line 1942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> connect( boost::asio::io_service* const& ios, Reference<ReferencedObject<boost::asio::ssl::context>> const& context, NetworkAddress const& addr, tcp::socket* const& existingSocket = nullptr ) {
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new ConnectActor2(ios, context, addr, existingSocket));
															#line 1970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	// This is not part of the IConnection interface, because it is wrapped by IListener::accept()
	void accept(NetworkAddress peerAddr) {
		this->peer_address = peerAddr;
		init();
	}

																#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doAcceptHandshake()
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoAcceptHandshakeActor>
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptHandshakeActorState {
															#line 1987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptHandshakeActorState(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   connected(connected),
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   holder()
															#line 1998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Future<Void> onHandshook;
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (N2::g_net2->sslPoolHandshakesInProgress < N2::g_net2->sslHandshakerThreadsStarted)
															#line 2016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					holder = Hold(&N2::g_net2->sslPoolHandshakesInProgress);
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto handshake = new SSLHandshakerThread::Handshake(self->ssl_sock, boost::asio::ssl::stream_base::server);
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = handshake->done.getFuture();
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					N2::g_net2->sslHandshakerPool->post(handshake);
															#line 2026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					BindPromise p("N2_AcceptHandshakeError", UID());
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = p.getFuture();
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					self->ssl_sock.async_handshake(boost::asio::ssl::stream_base::server, std::move(p));
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = onHandshook;
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 2042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 1;
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoAcceptHandshakeActor, 0, Void >*>(static_cast<DoAcceptHandshakeActor*>(this)));
															#line 2046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			connected.sendError(connection_failed());
															#line 2083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 2100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 2;
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoAcceptHandshakeActor, 1, Void >*>(static_cast<DoAcceptHandshakeActor*>(this)));
															#line 2104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 2115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoAcceptHandshakeActor*>(this)->actor_wait_state = 2;
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoAcceptHandshakeActor, 1, Void >*>(static_cast<DoAcceptHandshakeActor*>(this)));
															#line 2119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		delete static_cast<DoAcceptHandshakeActor*>(this);
															#line 2285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Promise<Void> connected;
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Hold<int> holder;
															#line 2296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doAcceptHandshake()
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptHandshakeActor final : public Actor<void>, public ActorCallback< DoAcceptHandshakeActor, 0, Void >, public ActorCallback< DoAcceptHandshakeActor, 1, Void >, public FastAllocated<DoAcceptHandshakeActor>, public DoAcceptHandshakeActorState<DoAcceptHandshakeActor> {
															#line 2301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoAcceptHandshakeActor>::operator new;
	using FastAllocated<DoAcceptHandshakeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoAcceptHandshakeActor, 0, Void >;
friend struct ActorCallback< DoAcceptHandshakeActor, 1, Void >;
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptHandshakeActor(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 2313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static void doAcceptHandshake( Reference<SSLConnection> const& self, Promise<Void> const& connected ) {
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	new DoAcceptHandshakeActor(self, connected);
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

																#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via acceptHandshakeWrapper()
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class AcceptHandshakeWrapperActor>
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class AcceptHandshakeWrapperActorState {
															#line 2342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AcceptHandshakeWrapperActorState(Reference<SSLConnection> const& self) 
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   peerIP()
															#line 2351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			peerIP = std::make_pair(self->getPeerAddress().ip, static_cast<uint16_t>(0));
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (now() < iter->second.second)
															#line 2374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					if (iter->second.first >= FLOW_KNOBS->TLS_SERVER_CONNECTION_THROTTLE_ATTEMPTS)
															#line 2378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
					{
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						TraceEvent("TLSIncomingConnectionThrottlingWarning") .suppressFor(1.0) .detail("PeerIP", peerIP.first.toString());
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
						static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 1;
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
						__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 0, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
						loopDepth = 0;
					}
					else
					{
						loopDepth = a_body1cont3(loopDepth);
					}
				}
				else
				{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler.erase(peerIP);
															#line 2403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = g_network->networkInfo.handshakeLock->take();
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 2;
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 1, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->closeSocket();
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 2462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->closeSocket();
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1Catch1(connection_failed(), loopDepth);
															#line 2472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

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
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		releaser = FlowLock::Releaser(*g_network->networkInfo.handshakeLock);
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> connected;
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doAcceptHandshake(self, connected);
															#line 2547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = connected.getFuture();
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont7Catch1(actor_cancelled(), loopDepth);
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont7Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont7when1(__when_expr_2.get(), loopDepth); };
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 2557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont7Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont7when2(__when_expr_3.get(), loopDepth); };
			static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 3;
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		releaser = FlowLock::Releaser(*g_network->networkInfo.handshakeLock);
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		Promise<Void> connected;
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		doAcceptHandshake(self, connected);
															#line 2583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = connected.getFuture();
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont7Catch1(actor_cancelled(), loopDepth);
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont7Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont7when1(__when_expr_2.get(), loopDepth); };
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 2593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont7Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont7when2(__when_expr_3.get(), loopDepth); };
			static_cast<AcceptHandshakeWrapperActor*>(this)->actor_wait_state = 3;
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 2, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AcceptHandshakeWrapperActor, 3, Void >*>(static_cast<AcceptHandshakeWrapperActor*>(this)));
															#line 2600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() != error_code_actor_cancelled)
															#line 2679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 2685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					iter->second.first++;
															#line 2689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler[peerIP] = std::make_pair(0, now() + FLOW_KNOBS->TLS_SERVER_CONNECTION_THROTTLE_TIMEOUT);
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
			}
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 2702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<AcceptHandshakeWrapperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AcceptHandshakeWrapperActorState(); static_cast<AcceptHandshakeWrapperActor*>(this)->destroy(); return 0; }
															#line 2716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<AcceptHandshakeWrapperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AcceptHandshakeWrapperActorState();
		static_cast<AcceptHandshakeWrapperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<AcceptHandshakeWrapperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AcceptHandshakeWrapperActorState(); static_cast<AcceptHandshakeWrapperActor*>(this)->destroy(); return 0; }
															#line 2728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<AcceptHandshakeWrapperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AcceptHandshakeWrapperActorState();
		static_cast<AcceptHandshakeWrapperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7when2(Void const& _,int loopDepth) 
	{
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont7Catch1(connection_failed(), loopDepth);
															#line 2740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont7when2(Void && _,int loopDepth) 
	{
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont7Catch1(connection_failed(), loopDepth);
															#line 2748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

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
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::pair<IPAddress, uint16_t> peerIP;
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	FlowLock::Releaser releaser;
															#line 2855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via acceptHandshakeWrapper()
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class AcceptHandshakeWrapperActor final : public Actor<Void>, public ActorCallback< AcceptHandshakeWrapperActor, 0, Void >, public ActorCallback< AcceptHandshakeWrapperActor, 1, Void >, public ActorCallback< AcceptHandshakeWrapperActor, 2, Void >, public ActorCallback< AcceptHandshakeWrapperActor, 3, Void >, public FastAllocated<AcceptHandshakeWrapperActor>, public AcceptHandshakeWrapperActorState<AcceptHandshakeWrapperActor> {
															#line 2860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AcceptHandshakeWrapperActor(Reference<SSLConnection> const& self) 
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Void> acceptHandshakeWrapper( Reference<SSLConnection> const& self ) {
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new AcceptHandshakeWrapperActor(self));
															#line 2903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

	Future<Void> acceptHandshake() override { return acceptHandshakeWrapper(Reference<SSLConnection>::addRef(this)); }

																#line 2910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doConnectHandshake()
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoConnectHandshakeActor>
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoConnectHandshakeActorState {
															#line 2916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoConnectHandshakeActorState(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   connected(connected),
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   holder()
															#line 2927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				Future<Void> onHandshook;
															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (N2::g_net2->sslPoolHandshakesInProgress < N2::g_net2->sslHandshakerThreadsStarted)
															#line 2945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					holder = Hold(&N2::g_net2->sslPoolHandshakesInProgress);
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					auto handshake = new SSLHandshakerThread::Handshake(self->ssl_sock, boost::asio::ssl::stream_base::client);
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = handshake->done.getFuture();
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					N2::g_net2->sslHandshakerPool->post(handshake);
															#line 2955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					BindPromise p("N2_ConnectHandshakeError", self->id);
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					onHandshook = p.getFuture();
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					self->ssl_sock.async_handshake(boost::asio::ssl::stream_base::client, std::move(p));
															#line 2965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = onHandshook;
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 2971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 1;
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoConnectHandshakeActor, 0, Void >*>(static_cast<DoConnectHandshakeActor*>(this)));
															#line 2975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 2;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoConnectHandshakeActor, 1, Void >*>(static_cast<DoConnectHandshakeActor*>(this)));
															#line 3033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::Handshake);
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 3044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		static_cast<DoConnectHandshakeActor*>(this)->actor_wait_state = 2;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoConnectHandshakeActor, 1, Void >*>(static_cast<DoConnectHandshakeActor*>(this)));
															#line 3048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		connected.send(Void());
															#line 3129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		delete static_cast<DoConnectHandshakeActor*>(this);
															#line 3214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> self;
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Promise<Void> connected;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Hold<int> holder;
															#line 3225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doConnectHandshake()
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoConnectHandshakeActor final : public Actor<void>, public ActorCallback< DoConnectHandshakeActor, 0, Void >, public ActorCallback< DoConnectHandshakeActor, 1, Void >, public FastAllocated<DoConnectHandshakeActor>, public DoConnectHandshakeActorState<DoConnectHandshakeActor> {
															#line 3230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoConnectHandshakeActor>::operator new;
	using FastAllocated<DoConnectHandshakeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoConnectHandshakeActor, 0, Void >;
friend struct ActorCallback< DoConnectHandshakeActor, 1, Void >;
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoConnectHandshakeActor(Reference<SSLConnection> const& self,Promise<Void> const& connected) 
															#line 3242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
static void doConnectHandshake( Reference<SSLConnection> const& self, Promise<Void> const& connected ) {
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	new DoConnectHandshakeActor(self, connected);
															#line 3260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

																#line 3265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via connectHandshakeWrapper()
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ConnectHandshakeWrapperActor>
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectHandshakeWrapperActorState {
															#line 3271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ConnectHandshakeWrapperActorState(Reference<SSLConnection> const& self) 
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self)
															#line 3278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 1;
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 0, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 3300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = connected.getFuture();
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 2;
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 3344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_1 = connected.getFuture();
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<ConnectHandshakeWrapperActor*>(this)->actor_wait_state = 2;
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 1, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectHandshakeWrapperActor, 2, Void >*>(static_cast<ConnectHandshakeWrapperActor*>(this)));
															#line 3380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				std::pair<IPAddress, uint16_t> peerIP = std::make_pair(self->peer_address.ip, self->peer_address.port);
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto iter(g_network->networkInfo.serverTLSConnectionThrottler.find(peerIP));
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (iter != g_network->networkInfo.serverTLSConnectionThrottler.end())
															#line 3467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				{
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					iter->second.first++;
															#line 3471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
				else
				{
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
					g_network->networkInfo.serverTLSConnectionThrottler[peerIP] = std::make_pair(0, now() + FLOW_KNOBS->TLS_CLIENT_CONNECTION_THROTTLE_TIMEOUT);
															#line 3477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				}
			}
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			self->closeSocket();
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 3484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		return a_body1cont1Catch1(connection_failed(), loopDepth);
															#line 3530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"

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
															#line 3635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via connectHandshakeWrapper()
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ConnectHandshakeWrapperActor final : public Actor<Void>, public ActorCallback< ConnectHandshakeWrapperActor, 0, Void >, public ActorCallback< ConnectHandshakeWrapperActor, 1, Void >, public ActorCallback< ConnectHandshakeWrapperActor, 2, Void >, public FastAllocated<ConnectHandshakeWrapperActor>, public ConnectHandshakeWrapperActorState<ConnectHandshakeWrapperActor> {
															#line 3640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 3681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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

	UID getDebugID() const override { return id; }

	tcp::socket& getSocket() override { return socket; }

	ssl_socket& getSSLSocket() { return ssl_sock; }

private:
	UID id;
	tcp::socket socket;
	ssl_socket ssl_sock;
	NetworkAddress peer_address;
	Reference<ReferencedObject<boost::asio::ssl::context>> sslContext;

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
		    .detail("ErrorCode", error.value())
		    .detail("Message", error.message());
		closeSocket();
	}
	void onWriteError(const boost::system::error_code& error) {
		TraceEvent(SevWarn, "N2_WriteError", id)
		    .suppressFor(1.0)
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
		platform::setCloseOnExec(acceptor.native_handle());
	}

	void addref() override { ReferenceCounted<SSLListener>::addref(); }
	void delref() override { ReferenceCounted<SSLListener>::delref(); }

	// Returns one incoming connection when it is available
	Future<Reference<IConnection>> accept() override { return doAccept(this); }

	NetworkAddress getListenAddress() const override { return listenAddress; }

private:
																#line 3838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via doAccept()
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class DoAcceptActor1>
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActor1State {
															#line 3844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActor1State(SSLListener* const& self) 
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   conn(new SSLConnection(self->io_service, self->contextVar->get())),
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   peer_endpoint()
															#line 3855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				BindPromise p("N2_AcceptError", UID());
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				auto f = p.getFuture();
															#line 1198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->acceptor.async_accept(conn->getSocket(), peer_endpoint, std::move(p));
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = f;
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<DoAcceptActor1*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoAcceptActor1*>(this)->actor_wait_state = 1;
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoAcceptActor1, 0, Void >*>(static_cast<DoAcceptActor1*>(this)));
															#line 3884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			conn->close();
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 3916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port(), false, true));
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor1*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActor1State(); static_cast<DoAcceptActor1*>(this)->destroy(); return 0; }
															#line 3934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<DoAcceptActor1*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(std::move(conn)); // state_var_RVO
		this->~DoAcceptActor1State();
		static_cast<DoAcceptActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		auto peer_address = peer_endpoint.address().is_v6() ? IPAddress(peer_endpoint.address().to_v6().to_bytes()) : IPAddress(peer_endpoint.address().to_v4().to_ulong());
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		conn->accept(NetworkAddress(peer_address, peer_endpoint.port(), false, true));
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<DoAcceptActor1*>(this)->SAV<Reference<IConnection>>::futures) { (void)(conn); this->~DoAcceptActor1State(); static_cast<DoAcceptActor1*>(this)->destroy(); return 0; }
															#line 3950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	SSLListener* self;
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Reference<SSLConnection> conn;
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::acceptor::endpoint_type peer_endpoint;
															#line 4027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via doAccept()
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class DoAcceptActor1 final : public Actor<Reference<IConnection>>, public ActorCallback< DoAcceptActor1, 0, Void >, public FastAllocated<DoAcceptActor1>, public DoAcceptActor1State<DoAcceptActor1> {
															#line 4032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<DoAcceptActor1>::operator new;
	using FastAllocated<DoAcceptActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoAcceptActor1, 0, Void >;
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	DoAcceptActor1(SSLListener* const& self) 
															#line 4043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> doAccept( SSLListener* const& self ) {
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Reference<IConnection>>(new DoAcceptActor1(self));
															#line 4070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
};
#endif

struct PromiseTask final : public Task, public FastAllocated<PromiseTask> {
	Promise<Void> promise;
	PromiseTask() {}
	explicit PromiseTask(Promise<Void>&& promise) noexcept : promise(std::move(promise)) {}

	void operator()() override {
		promise.send(Void());
		delete this;
	}
};

// 5MB for loading files into memory

Net2::Net2(const TLSConfig& tlsConfig, bool useThreadPool, bool useMetrics)
  : globals(enumGlobal::COUNT), useThreadPool(useThreadPool), reactor(this),
#ifndef TLS_DISABLED
    sslContextVar({ ReferencedObject<boost::asio::ssl::context>::from(
        boost::asio::ssl::context(boost::asio::ssl::context::tls)) }),
    sslHandshakerThreadsStarted(0), sslPoolHandshakesInProgress(0),
#endif
    tlsConfig(tlsConfig), tlsInitializedState(ETLSInitState::NONE), network(this), tscBegin(0), tscEnd(0), taskBegin(0),
    currentTaskID(TaskPriority::DefaultYield), tasksIssued(0), stopped(false), started(false), numYields(0),
    lastPriorityStats(nullptr), ready(FLOW_KNOBS->READY_QUEUE_RESERVED_SIZE) {
	// Until run() is called, yield() will always yield
	TraceEvent("Net2Starting").log();

	// Set the global members
	if (useMetrics) {
		setGlobal(INetwork::enTDMetrics, (flowGlobalType)&tdmetrics);
	}
	if (FLOW_KNOBS->ENABLE_CHAOS_FEATURES) {
		setGlobal(INetwork::enChaosMetrics, (flowGlobalType)&chaosMetrics);
	}
	setGlobal(INetwork::enNetworkConnections, (flowGlobalType)network);
	setGlobal(INetwork::enASIOService, (flowGlobalType)&reactor.ios);
	setGlobal(INetwork::enBlobCredentialFiles, &blobCredentialFiles);

#ifdef __linux__
	setGlobal(INetwork::enEventFD, (flowGlobalType)N2::ASIOReactor::newEventFD(reactor));
#endif

	updateNow();
}

#ifndef TLS_DISABLED
															#line 4122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via watchFileForChanges()
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class WatchFileForChangesActor>
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class WatchFileForChangesActorState {
															#line 4128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	WatchFileForChangesActorState(std::string const& filename,AsyncTrigger* const& fileChanged) 
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : filename(filename),
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   fileChanged(fileChanged)
															#line 4137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
	{
		fdb_probe_actor_create("watchFileForChanges", reinterpret_cast<unsigned long>(this));

	}
	~WatchFileForChangesActorState() 
	{
		fdb_probe_actor_destroy("watchFileForChanges", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (filename == "")
															#line 4152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				this->~WatchFileForChangesActorState();
															#line 4156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				static_cast<WatchFileForChangesActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 1263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			firstRun = true;
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			statError = false;
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			lastModTime = 0;
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			;
															#line 4168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
		this->~WatchFileForChangesActorState();
		static_cast<WatchFileForChangesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
		try {
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<std::time_t> __when_expr_0 = IAsyncFileSystem::filesystem()->lastWriteTime(filename);
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 1;
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchFileForChangesActor, 0, std::time_t >*>(static_cast<WatchFileForChangesActor*>(this)));
															#line 4206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->TLS_CERT_REFRESH_DELAY_SECONDS);
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 2;
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchFileForChangesActor, 1, Void >*>(static_cast<WatchFileForChangesActor*>(this)));
															#line 4228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() == error_code_io_error)
															#line 4238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				TraceEvent(SevWarnAlways, "TLSCertificateRefreshStatError").detail("File", filename);
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				statError = true;
															#line 4244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
			else
			{
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 4250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::time_t const& modtime,int loopDepth) 
	{
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (firstRun)
															#line 4266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
															#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			lastModTime = modtime;
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			firstRun = false;
															#line 4272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (lastModTime != modtime || statError)
															#line 4276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			lastModTime = modtime;
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			statError = false;
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			fileChanged->trigger();
															#line 4284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::time_t && modtime,int loopDepth) 
	{
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (firstRun)
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
															#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			lastModTime = modtime;
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			firstRun = false;
															#line 4300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (lastModTime != modtime || statError)
															#line 4304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			lastModTime = modtime;
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			statError = false;
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			fileChanged->trigger();
															#line 4312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::time_t const& modtime,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(modtime, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::time_t && modtime,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(modtime), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state > 0) static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 0;
		static_cast<WatchFileForChangesActor*>(this)->ActorCallback< WatchFileForChangesActor, 0, std::time_t >::remove();

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 0, std::time_t >*,std::time_t const& value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 0, std::time_t >*,std::time_t && value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchFileForChangesActor, 0, std::time_t >*,Error err) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
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
		if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state > 0) static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 0;
		static_cast<WatchFileForChangesActor*>(this)->ActorCallback< WatchFileForChangesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WatchFileForChangesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::string filename;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AsyncTrigger* fileChanged;
															#line 1263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	bool firstRun;
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	bool statError;
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::time_t lastModTime;
															#line 4479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via watchFileForChanges()
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class WatchFileForChangesActor final : public Actor<Void>, public ActorCallback< WatchFileForChangesActor, 0, std::time_t >, public ActorCallback< WatchFileForChangesActor, 1, Void >, public FastAllocated<WatchFileForChangesActor>, public WatchFileForChangesActorState<WatchFileForChangesActor> {
															#line 4484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<WatchFileForChangesActor>::operator new;
	using FastAllocated<WatchFileForChangesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchFileForChangesActor, 0, std::time_t >;
friend struct ActorCallback< WatchFileForChangesActor, 1, Void >;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	WatchFileForChangesActor(std::string const& filename,AsyncTrigger* const& fileChanged) 
															#line 4496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   WatchFileForChangesActorState<WatchFileForChangesActor>(filename, fileChanged)
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("watchFileForChanges");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchFileForChangesActor, 0, std::time_t >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WatchFileForChangesActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Void> watchFileForChanges( std::string const& filename, AsyncTrigger* const& fileChanged ) {
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new WatchFileForChangesActor(filename, fileChanged));
															#line 4524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

															#line 4529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via reloadCertificatesOnChange()
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ReloadCertificatesOnChangeActor>
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ReloadCertificatesOnChangeActorState {
															#line 4535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ReloadCertificatesOnChangeActorState(TLSConfig const& config,std::function<void()> const& onPolicyFailure,AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* const& contextVar) 
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : config(config),
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   onPolicyFailure(onPolicyFailure),
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   contextVar(contextVar)
															#line 4546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (FLOW_KNOBS->TLS_CERT_REFRESH_DELAY_SECONDS <= 0)
															#line 4561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (!static_cast<ReloadCertificatesOnChangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReloadCertificatesOnChangeActorState(); static_cast<ReloadCertificatesOnChangeActor*>(this)->destroy(); return 0; }
															#line 4565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				new (&static_cast<ReloadCertificatesOnChangeActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ReloadCertificatesOnChangeActorState();
				static_cast<ReloadCertificatesOnChangeActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			;
															#line 4573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		mismatches = 0;
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		fileChanged = AsyncTrigger();
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes = std::vector<Future<Void>>();
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes.push_back(watchFileForChanges(config.getCertificatePathSync(), &fileChanged));
															#line 1312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes.push_back(watchFileForChanges(config.getKeyPathSync(), &fileChanged));
															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		lifetimes.push_back(watchFileForChanges(config.getCAPathSync(), &fileChanged));
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		;
															#line 4608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (IAsyncFileSystem::filesystem() != nullptr)
															#line 4624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(1.0);
															#line 1306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 1;
															#line 1306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_1 = fileChanged.onTrigger();
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 2;
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent("TLSCertificateRefreshBegin").log();
															#line 4757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<LoadedTLSConfig> __when_expr_2 = config.loadAsync();
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1loopBody1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 3;
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent("TLSCertificateRefreshBegin").log();
															#line 4783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		try {
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			StrictFuture<LoadedTLSConfig> __when_expr_2 = config.loadAsync();
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1loopBody1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ReloadCertificatesOnChangeActor*>(this)->actor_wait_state = 3;
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >*>(static_cast<ReloadCertificatesOnChangeActor*>(this)));
															#line 4794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 4879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 4883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
															#line 1330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			mismatches++;
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			TraceEvent(SevWarn, "TLSCertificateRefreshMismatch").error(e).detail("mismatches", mismatches);
															#line 4889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		boost::asio::ssl::context context(boost::asio::ssl::context::tls);
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		ConfigureSSLContext(loaded, &context, onPolicyFailure);
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent(SevInfo, "TLSCertificateRefreshSucceeded").log();
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		mismatches = 0;
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		contextVar->set(ReferencedObject<boost::asio::ssl::context>::from(std::move(context)));
															#line 4912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(LoadedTLSConfig && loaded,int loopDepth) 
	{
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		boost::asio::ssl::context context(boost::asio::ssl::context::tls);
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		ConfigureSSLContext(loaded, &context, onPolicyFailure);
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent(SevInfo, "TLSCertificateRefreshSucceeded").log();
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		mismatches = 0;
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		contextVar->set(ReferencedObject<boost::asio::ssl::context>::from(std::move(context)));
															#line 4929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	TLSConfig config;
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::function<void()> onPolicyFailure;
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* contextVar;
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	int mismatches;
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	AsyncTrigger fileChanged;
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::vector<Future<Void>> lifetimes;
															#line 5022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via reloadCertificatesOnChange()
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ReloadCertificatesOnChangeActor final : public Actor<Void>, public ActorCallback< ReloadCertificatesOnChangeActor, 0, Void >, public ActorCallback< ReloadCertificatesOnChangeActor, 1, Void >, public ActorCallback< ReloadCertificatesOnChangeActor, 2, LoadedTLSConfig >, public FastAllocated<ReloadCertificatesOnChangeActor>, public ReloadCertificatesOnChangeActorState<ReloadCertificatesOnChangeActor> {
															#line 5027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ReloadCertificatesOnChangeActor(TLSConfig const& config,std::function<void()> const& onPolicyFailure,AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* const& contextVar) 
															#line 5040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		 : Actor<Void>(),
		   ReloadCertificatesOnChangeActorState<ReloadCertificatesOnChangeActor>(config, onPolicyFailure, contextVar)
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
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<Void> reloadCertificatesOnChange( TLSConfig const& config, std::function<void()> const& onPolicyFailure, AsyncVar<Reference<ReferencedObject<boost::asio::ssl::context>>>* const& contextVar ) {
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new ReloadCertificatesOnChangeActor(config, onPolicyFailure, contextVar));
															#line 5069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
#endif

void Net2::initTLS(ETLSInitState targetState) {
	if (tlsInitializedState >= targetState) {
		return;
	}
#ifndef TLS_DISABLED
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
			ConfigureSSLContext(tlsConfig.loadSync(), &newContext, onPolicyFailure);
			sslContextVar.set(ReferencedObject<boost::asio::ssl::context>::from(std::move(newContext)));
		} catch (Error& e) {
			TraceEvent("Net2TLSInitError").error(e);
		}
		backgroundCertRefresh = reloadCertificatesOnChange(tlsConfig, onPolicyFailure, &sslContextVar);
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
#endif

	tlsInitializedState = targetState;
}

															#line 5138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via logTimeOffset()
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class Net2_LogTimeOffsetActor>
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class Net2_LogTimeOffsetActorState {
															#line 5144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Net2_LogTimeOffsetActorState() 
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	{
															#line 5150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		fdb_probe_actor_create("logTimeOffset", reinterpret_cast<unsigned long>(this));

	}
	~Net2_LogTimeOffsetActorState() 
	{
		fdb_probe_actor_destroy("logTimeOffset", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			;
															#line 5164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		double processTime = timer_monotonic();
															#line 1403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		double systemTime = timer();
															#line 1404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		TraceEvent("ProcessTimeOffset") .detailf("ProcessTime", "%lf", processTime) .detailf("SystemTime", "%lf", systemTime) .detailf("OffsetFromSystemTime", "%lf", processTime - systemTime);
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		StrictFuture<Void> __when_expr_0 = ::delay(FLOW_KNOBS->TIME_OFFSET_LOGGING_INTERVAL);
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (static_cast<Net2_LogTimeOffsetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Net2_LogTimeOffsetActor*>(this)->actor_wait_state = 1;
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Net2_LogTimeOffsetActor, 0, Void >*>(static_cast<Net2_LogTimeOffsetActor*>(this)));
															#line 5207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class Net2_LogTimeOffsetActor final : public Actor<Void>, public ActorCallback< Net2_LogTimeOffsetActor, 0, Void >, public FastAllocated<Net2_LogTimeOffsetActor>, public Net2_LogTimeOffsetActorState<Net2_LogTimeOffsetActor> {
															#line 5291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<Net2_LogTimeOffsetActor>::operator new;
	using FastAllocated<Net2_LogTimeOffsetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Net2_LogTimeOffsetActor, 0, Void >;
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Net2_LogTimeOffsetActor() 
															#line 5302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] Future<Void> Net2::logTimeOffset(  ) {
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<Void>(new Net2_LogTimeOffsetActor());
															#line 5329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

void Net2::initMetrics() {
	bytesReceived.init(LiteralStringRef("Net2.BytesReceived"));
	countWriteProbes.init(LiteralStringRef("Net2.CountWriteProbes"));
	countReadProbes.init(LiteralStringRef("Net2.CountReadProbes"));
	countReads.init(LiteralStringRef("Net2.CountReads"));
	countWouldBlock.init(LiteralStringRef("Net2.CountWouldBlock"));
	countWrites.init(LiteralStringRef("Net2.CountWrites"));
	countRunLoop.init(LiteralStringRef("Net2.CountRunLoop"));
	countCantSleep.init(LiteralStringRef("Net2.CountCantSleep"));
	countWontSleep.init(LiteralStringRef("Net2.CountWontSleep"));
	countTimers.init(LiteralStringRef("Net2.CountTimers"));
	countTasks.init(LiteralStringRef("Net2.CountTasks"));
	countYields.init(LiteralStringRef("Net2.CountYields"));
	countYieldBigStack.init(LiteralStringRef("Net2.CountYieldBigStack"));
	countYieldCalls.init(LiteralStringRef("Net2.CountYieldCalls"));
	countASIOEvents.init(LiteralStringRef("Net2.CountASIOEvents"));
	countYieldCallsTrue.init(LiteralStringRef("Net2.CountYieldCallsTrue"));
	countRunLoopProfilingSignals.init(LiteralStringRef("Net2.CountRunLoopProfilingSignals"));
	countTLSPolicyFailures.init(LiteralStringRef("Net2.CountTLSPolicyFailures"));
	priorityMetric.init(LiteralStringRef("Net2.Priority"));
	awakeMetric.init(LiteralStringRef("Net2.Awake"));
	slowTaskMetric.init(LiteralStringRef("Net2.SlowTask"));
	countLaunchTime.init(LiteralStringRef("Net2.CountLaunchTime"));
	countReactTime.init(LiteralStringRef("Net2.CountReactTime"));
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
		bool b = ready.empty();
		if (b) {
			b = threadReady.canSleep();
			if (!b)
				++countCantSleep;
		} else
			++countWontSleep;
		if (b) {
			sleepTime = 1e99;
			double sleepStart = timer_monotonic();
			if (!timers.empty()) {
				sleepTime = timers.top().at - sleepStart; // + 500e-6?
			}
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

		int numTimers = 0;
		while (!timers.empty() && timers.top().at < now) {
			++numTimers;
			++countTimers;
			ready.push(timers.top());
			timers.pop();
		}
		// FIXME: Is this double counting?
		countTimers += numTimers;
		FDB_TRACE_PROBE(run_loop_ready_timers, numTimers);

		processThreadReady();

		tscBegin = timestampCounter();
		tscEnd = tscBegin + FLOW_KNOBS->TSC_YIELD_TIME;
		taskBegin = timer_monotonic();
		numYields = 0;
		TaskPriority minTaskID = TaskPriority::Max;
		[[maybe_unused]] int queueSize = ready.size();

		FDB_TRACE_PROBE(run_loop_tasks_start, queueSize);
		while (!ready.empty()) {
			++countTasks;
			currentTaskID = ready.top().taskID;
			priorityMetric = static_cast<int64_t>(currentTaskID);
			Task* task = ready.top().task;
			ready.pop();

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

		queueSize = ready.size();
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

void Net2::processThreadReady() {
	[[maybe_unused]] int numReady = 0;
	while (true) {
		Optional<OrderedTask> t = threadReady.pop();
		if (!t.present())
			break;
		t.get().priority -= ++tasksIssued;
		ASSERT(t.get().task != 0);
		ready.push(t.get());
		++numReady;
	}
	FDB_TRACE_PROBE(run_loop_thread_ready, numReady);
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

	processThreadReady();

	if (taskID == TaskPriority::DefaultYield)
		taskID = currentTaskID;
	if (!ready.empty() && ready.top().priority > int64_t(taskID) << 32) {
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
	if (seconds <= 0.) {
		PromiseTask* t = new PromiseTask;
		this->ready.push(OrderedTask((int64_t(taskId) << 32) - (++tasksIssued), taskId, t));
		return t->promise.getFuture();
	}
	if (seconds >=
	    4e12) // Intervals that overflow an int64_t in microseconds (more than 100,000 years) are treated as infinite
		return Never();

	double at = now() + seconds;
	PromiseTask* t = new PromiseTask;
	this->timers.push(DelayedTask(at, (int64_t(taskId) << 32) - (++tasksIssued), taskId, t));
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
	int64_t priority = int64_t(taskID) << 32;

	if (thread_network == this) {
		processThreadReady();
		this->ready.push(OrderedTask(priority - (++tasksIssued), taskID, p));
	} else {
		if (threadReady.push(OrderedTask(priority, taskID, p)))
			reactor.wake();
	}
}

THREAD_HANDLE Net2::startThread(THREAD_FUNC_RETURN (*func)(void*), void* arg, int stackSize, const char* name) {
	return ::startThread(func, arg, stackSize, name);
}

Future<Reference<IConnection>> Net2::connect(NetworkAddress toAddr, tcp::socket* existingSocket) {
#ifndef TLS_DISABLED
	if (toAddr.isTLS()) {
		initTLS(ETLSInitState::CONNECT);
		return SSLConnection::connect(&this->reactor.ios, this->sslContextVar.get(), toAddr, existingSocket);
	}
#endif

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

															#line 5798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
// This generated class is to be used only via resolveTCPEndpoint_impl()
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
template <class ResolveTCPEndpoint_implActor>
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ResolveTCPEndpoint_implActorState {
															#line 5804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ResolveTCPEndpoint_implActorState(Net2* const& self,std::string const& host,std::string const& service) 
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		 : self(self),
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   host(host),
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   service(service),
															#line 1879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		   tcpResolver(self->reactor.ios)
															#line 5817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			Promise<std::vector<NetworkAddress>> promise;
															#line 1881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			result = promise.getFuture();
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			tcpResolver.async_resolve( host, service, [promise](const boost::system::error_code& ec, tcp::resolver::iterator iter) { if (ec) { promise.sendError(lookup_failed()); return; } std::vector<NetworkAddress> addrs; tcp::resolver::iterator end; while (iter != end) { auto endpoint = iter->endpoint(); auto addr = endpoint.address(); if (addr.is_v6()) { addrs.emplace_back(IPAddress(addr.to_v6().to_bytes()), endpoint.port()); } else { addrs.emplace_back(addr.to_v4().to_ulong(), endpoint.port()); } ++iter; } if (addrs.empty()) { promise.sendError(lookup_failed()); } else { promise.send(addrs); } });
															#line 5836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			try {
															#line 1912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				StrictFuture<Void> __when_expr_0 = ready(result);
															#line 1912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				if (static_cast<ResolveTCPEndpoint_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ResolveTCPEndpoint_implActor*>(this)->actor_wait_state = 1;
															#line 1912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >*>(static_cast<ResolveTCPEndpoint_implActor*>(this)));
															#line 5847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		tcpResolver.cancel();
															#line 1920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		std::vector<NetworkAddress> ret = result.get();
															#line 1921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		self->dnsCache.add(host, service, ret);
															#line 1923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
		if (!static_cast<ResolveTCPEndpoint_implActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(ret); this->~ResolveTCPEndpoint_implActorState(); static_cast<ResolveTCPEndpoint_implActor*>(this)->destroy(); return 0; }
															#line 5882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
		new (&static_cast<ResolveTCPEndpoint_implActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(ret);
		this->~ResolveTCPEndpoint_implActorState();
		static_cast<ResolveTCPEndpoint_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			if (e.code() == error_code_lookup_failed)
															#line 5895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			{
															#line 1915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
				self->dnsCache.remove(host, service);
															#line 5899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
			}
															#line 1917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 5903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Net2* self;
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::string host;
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	std::string service;
															#line 1879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	tcp::resolver tcpResolver;
															#line 1881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	Future<std::vector<NetworkAddress>> result;
															#line 6011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
};
// This generated class is to be used only via resolveTCPEndpoint_impl()
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
class ResolveTCPEndpoint_implActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >, public FastAllocated<ResolveTCPEndpoint_implActor>, public ResolveTCPEndpoint_implActorState<ResolveTCPEndpoint_implActor> {
															#line 6016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
public:
	using FastAllocated<ResolveTCPEndpoint_implActor>::operator new;
	using FastAllocated<ResolveTCPEndpoint_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveTCPEndpoint_implActor, 0, Void >;
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	ResolveTCPEndpoint_implActor(Net2* const& self,std::string const& host,std::string const& service) 
															#line 6027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
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
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
[[nodiscard]] static Future<std::vector<NetworkAddress>> resolveTCPEndpoint_impl( Net2* const& self, std::string const& host, std::string const& service ) {
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new ResolveTCPEndpoint_implActor(self, host, service));
															#line 6054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.g.cpp"
}

#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Net2.actor.cpp"

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
#ifndef TLS_DISABLED
		if (localAddr.isTLS()) {
			initTLS(ETLSInitState::LISTEN);
			return Reference<IListener>(new SSLListener(reactor.ios, &this->sslContextVar, localAddr));
		}
#endif
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
void startThreadF(F&& func) {
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
	startThread(Thing::start, t);
}

void net2_test(){
	/*printf("ThreadSafeQueue test\n");
	printf("  Interface: ");
	ThreadSafeQueue<int> tq;
	ASSERT( tq.canSleep() == true );

	ASSERT( tq.push( 1 ) == true ) ;
	ASSERT( tq.push( 2 ) == false );
	ASSERT( tq.push( 3 ) == false );

	ASSERT( tq.pop().get() == 1 );
	ASSERT( tq.pop().get() == 2 );
	ASSERT( tq.push( 4 ) == false );
	ASSERT( tq.pop().get() == 3 );
	ASSERT( tq.pop().get() == 4 );
	ASSERT( !tq.pop().present() );
	printf("OK\n");

	printf("Threaded: ");
	Event finished, finished2;
	int thread1Iterations = 1000000, thread2Iterations = 100000;

	if (thread1Iterations)
	    startThreadF([&](){
	        printf("Thread1\n");
	        for(int i=0; i<thread1Iterations; i++)
	            tq.push(i);
	        printf("T1Done\n");
	        finished.set();
	    });
	if (thread2Iterations)
	    startThreadF([&](){
	        printf("Thread2\n");
	        for(int i=0; i<thread2Iterations; i++)
	            tq.push(i + (1<<20));
	        printf("T2Done\n");
	        finished2.set();
	    });
	int c = 0, mx[2]={0, 1<<20}, p = 0;
	while (c < thread1Iterations + thread2Iterations)
	{
	    Optional<int> i = tq.pop();
	    if (i.present()) {
	        int v = i.get();
	        ++c;
	        if (mx[v>>20] != v)
	            printf("Wrong value dequeued!\n");
	        ASSERT( mx[v>>20] == v );
	        mx[v>>20] = v + 1;
	    } else {
	        ++p;
	        _mm_pause();
	    }
	    if ((c&3)==0) tq.canSleep();
	}
	printf("%d %d %x %x %s\n", c, p, mx[0], mx[1], mx[0]==thread1Iterations && mx[1]==(1<<20)+thread2Iterations ? "OK" :
	"FAIL");

	finished.block();
	finished2.block();


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
	        req.key = LiteralStringRef("Foobar");

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
