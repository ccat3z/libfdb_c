#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
/*
 * Stats.actor.cpp
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

#include "fdbrpc/Stats.h"
#include "flow/IRandom.h"
#include "flow/Knobs.h"
#include "flow/OTELMetrics.h"
#include "flow/TDMetric.actor.h"
#include "flow/Trace.h"
#include "flow/flow.h"
#include "flow/network.h"
#include <string>
#include "flow/actorcompiler.h" // has to be last include

Counter::Counter(std::string const& name, CounterCollection& collection)
  : name(name), interval_start(0), last_event(0), interval_sq_time(0), roughness_interval_start(0), interval_delta(0),
    interval_start_value(0) {
	metric.init(collection.getName() + "." + (char)toupper(name.at(0)) + name.substr(1), collection.getId());
	collection.addCounter(this);
}

void Counter::operator+=(Value delta) {
	if (!delta)
		return; //< Otherwise last_event will be reset
	interval_delta += delta;
	auto t = now();
	auto elapsed = t - last_event;
	interval_sq_time += elapsed * elapsed;
	last_event = t;

	metric += delta;
}

double Counter::getRate() const {
	double elapsed = now() - interval_start;
	return elapsed > 0 ? interval_delta / elapsed : 0;
}

double Counter::getRoughness() const {
	double elapsed = last_event - roughness_interval_start;
	if (elapsed == 0) {
		return -1;
	}

	// If we have time interval samples t in T, and let:
	// n = size(T) = interval_delta
	// m = mean(T) = elapsed / interval_delta
	// v = sum(t^2) for t in T = interval_sq_time
	//
	// The formula below is: (v/(m*n)) / m - 1
	// This is equivalent to (v/n - m^2) / m^2 = Variance(T)/m^2
	// Variance(T)/m^2 is equal to Variance(t/m) for t in T
	double delay = interval_sq_time / elapsed;
	return delay * interval_delta / elapsed - 1;
}

void Counter::resetInterval() {
	interval_start_value += interval_delta;
	interval_delta = 0;
	interval_sq_time = 0;
	interval_start = now();
	if (last_event == 0) {
		last_event = interval_start;
	}
	roughness_interval_start = last_event;
}

void Counter::clear() {
	resetInterval();
	interval_start_value = 0;

	metric = 0;
}

void CounterCollection::logToTraceEvent(TraceEvent& te) {
	NetworkAddress addr = g_network->getLocalAddress();
	for (ICounter* c : counters) {
		MetricCollection* metrics = MetricCollection::getMetricCollection();
		if (metrics != nullptr) {
			std::string ip_str = addr.ip.toString();
			std::string port_str = std::to_string(addr.port);
			uint64_t val = c->getValue();
			switch (c->model) {
			case MetricsDataModel::OTLP: {
				if (metrics->sumMap.find(c->id) != metrics->sumMap.end()) {
					metrics->sumMap[c->id].points.emplace_back(static_cast<int64_t>(val));
				} else {
					metrics->sumMap[c->id] = OTEL::OTELSum(name + "." + c->getName(), val);
				}
				metrics->sumMap[c->id].points.back().addAttribute("ip", ip_str);
				metrics->sumMap[c->id].points.back().addAttribute("port", port_str);
				metrics->sumMap[c->id].points.back().startTime = logTime;
			}
			case MetricsDataModel::STATSD: {
				std::vector<std::pair<std::string, std::string>> statsd_attributes{ { "ip", ip_str },
					                                                                { "port", port_str } };
				metrics->statsd_message.push_back(createStatsdMessage(
				    c->getName(), StatsDMetric::COUNTER, std::to_string(val) /*, statsd_attributes*/));
			}
			case MetricsDataModel::NONE:
			default: {
			}
			}
		}
		te.detail(c->getName().c_str(), c);
		c->resetInterval();
	}
}

class CounterCollectionImpl {
public:
																#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
// This generated class is to be used only via traceCounters()
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
template <class TraceCountersActor>
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
class TraceCountersActorState {
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
public:
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	TraceCountersActorState(CounterCollection* const& counters,std::string const& traceEventName,UID const& traceEventID,double const& interval,std::string const& trackLatestName,std::function<void(TraceEvent&)> const& decorator) 
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		 : counters(counters),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		   traceEventName(traceEventName),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		   traceEventID(traceEventID),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		   interval(interval),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		   trackLatestName(trackLatestName),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		   decorator(decorator)
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
	{
		fdb_probe_actor_create("traceCounters", reinterpret_cast<unsigned long>(this));

	}
	~TraceCountersActorState() 
	{
		fdb_probe_actor_destroy("traceCounters", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			if (static_cast<TraceCountersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TraceCountersActor*>(this)->actor_wait_state = 1;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TraceCountersActor, 0, Void >*>(static_cast<TraceCountersActor*>(this)));
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
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
		this->~TraceCountersActorState();
		static_cast<TraceCountersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		for( ICounter* c : counters->counters ) {
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			c->resetInterval();
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		traceEventHolder = Reference<EventCacheHolder>();
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		if (!trackLatestName.empty())
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			traceEventHolder = makeReference<EventCacheHolder>(trackLatestName);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		}
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		last_interval = now();
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		;
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		for( ICounter* c : counters->counters ) {
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			c->resetInterval();
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		traceEventHolder = Reference<EventCacheHolder>();
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		if (!trackLatestName.empty())
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			traceEventHolder = makeReference<EventCacheHolder>(trackLatestName);
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		}
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		last_interval = now();
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

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
		if (static_cast<TraceCountersActor*>(this)->actor_wait_state > 0) static_cast<TraceCountersActor*>(this)->actor_wait_state = 0;
		static_cast<TraceCountersActor*>(this)->ActorCallback< TraceCountersActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TraceCountersActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TraceCountersActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TraceCountersActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		TraceEvent te(traceEventName.c_str(), traceEventID);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		te.detail("Elapsed", now() - last_interval);
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		counters->logToTraceEvent(te);
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		decorator(te);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		if (!trackLatestName.empty())
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
			te.trackLatest(traceEventHolder->trackingKey);
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		}
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		last_interval = now();
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(interval, TaskPriority::FlushTrace);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		if (static_cast<TraceCountersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TraceCountersActor*>(this)->actor_wait_state = 2;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TraceCountersActor, 1, Void >*>(static_cast<TraceCountersActor*>(this)));
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

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
		if (static_cast<TraceCountersActor*>(this)->actor_wait_state > 0) static_cast<TraceCountersActor*>(this)->actor_wait_state = 0;
		static_cast<TraceCountersActor*>(this)->ActorCallback< TraceCountersActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TraceCountersActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TraceCountersActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TraceCountersActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	CounterCollection* counters;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	std::string traceEventName;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	UID traceEventID;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	double interval;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	std::string trackLatestName;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	std::function<void(TraceEvent&)> decorator;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	Reference<EventCacheHolder> traceEventHolder;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	double last_interval;
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
};
// This generated class is to be used only via traceCounters()
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
class TraceCountersActor final : public Actor<Void>, public ActorCallback< TraceCountersActor, 0, Void >, public ActorCallback< TraceCountersActor, 1, Void >, public FastAllocated<TraceCountersActor>, public TraceCountersActorState<TraceCountersActor> {
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
public:
	using FastAllocated<TraceCountersActor>::operator new;
	using FastAllocated<TraceCountersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TraceCountersActor, 0, Void >;
friend struct ActorCallback< TraceCountersActor, 1, Void >;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	TraceCountersActor(CounterCollection* const& counters,std::string const& traceEventName,UID const& traceEventID,double const& interval,std::string const& trackLatestName,std::function<void(TraceEvent&)> const& decorator) 
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
		 : Actor<Void>(),
		   TraceCountersActorState<TraceCountersActor>(counters, traceEventName, traceEventID, interval, trackLatestName, decorator)
	{
		fdb_probe_actor_enter("traceCounters", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("traceCounters");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("traceCounters", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TraceCountersActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TraceCountersActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
[[nodiscard]] static Future<Void> traceCounters( CounterCollection* const& counters, std::string const& traceEventName, UID const& traceEventID, double const& interval, std::string const& trackLatestName, std::function<void(TraceEvent&)> const& decorator ) {
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
	return Future<Void>(new TraceCountersActor(counters, traceEventName, traceEventID, interval, trackLatestName, decorator));
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.g.cpp"
}

#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/Stats.actor.cpp"
};

Future<Void> CounterCollection::traceCounters(std::string const& traceEventName,
                                              UID traceEventID,
                                              double interval,
                                              std::string const& trackLatestName,
                                              std::function<void(TraceEvent&)> const& decorator) {
	return CounterCollectionImpl::traceCounters(
	    this, traceEventName, traceEventID, interval, trackLatestName, decorator);
}

void LatencyBands::insertBand(double value) {
	bands.emplace(std::make_pair(value, std::make_unique<Counter>(format("Band%f", value), *cc)));
}

LatencyBands::LatencyBands(std::string const& name,
                           UID id,
                           double loggingInterval,
                           std::function<void(TraceEvent&)> const& decorator)
  : name(name), id(id), loggingInterval(loggingInterval), decorator(decorator) {}

void LatencyBands::addThreshold(double value) {
	if (value > 0 && bands.count(value) == 0) {
		if (bands.size() == 0) {
			ASSERT(!cc && !filteredCount);
			cc = std::make_unique<CounterCollection>(name, id.toString());
			logger = cc->traceCounters(name, id, loggingInterval, id.toString() + "/" + name, decorator);
			filteredCount = std::make_unique<Counter>("Filtered", *cc);
			insertBand(std::numeric_limits<double>::infinity());
		}

		insertBand(value);
	}
}

void LatencyBands::addMeasurement(double measurement, int count, Filtered filtered) {
	if (filtered && filteredCount) {
		(*filteredCount) += count;
	} else if (bands.size() > 0) {
		auto itr = bands.upper_bound(measurement);
		ASSERT(itr != bands.end());
		(*itr->second) += count;
	}
}

void LatencyBands::clearBands() {
	logger = Void();
	bands.clear();
	filteredCount.reset();
	cc.reset();
}

LatencyBands::~LatencyBands() {
	clearBands();
}

LatencySample::LatencySample(std::string name, UID id, double loggingInterval, double accuracy)
  : name(name), IMetric(knobToMetricModel(FLOW_KNOBS->METRICS_DATA_MODEL)), id(id), sampleEmit(now()), sketch(accuracy),
    latencySampleEventHolder(makeReference<EventCacheHolder>(id.toString() + "/" + name)) {
	logger = recurring([this]() { logSample(); }, loggingInterval);
	p50id = deterministicRandom()->randomUniqueID();
	p90id = deterministicRandom()->randomUniqueID();
	p95id = deterministicRandom()->randomUniqueID();
	p99id = deterministicRandom()->randomUniqueID();
	p999id = deterministicRandom()->randomUniqueID();
}

void LatencySample::addMeasurement(double measurement) {
	sketch.addSample(measurement);
}

void LatencySample::logSample() {
	double p25 = sketch.percentile(0.25);
	double p50 = sketch.mean();
	double p90 = sketch.percentile(0.9);
	double p95 = sketch.percentile(0.95);
	double p99 = sketch.percentile(0.99);
	double p99_9 = sketch.percentile(0.999);
	TraceEvent(name.c_str(), id)
	    .detail("Count", sketch.getPopulationSize())
	    .detail("Elapsed", now() - sampleEmit)
	    .detail("Min", sketch.min())
	    .detail("Max", sketch.max())
	    .detail("Mean", sketch.mean())
	    .detail("Median", p50)
	    .detail("P25", p25)
	    .detail("P90", p90)
	    .detail("P95", p95)
	    .detail("P99", p99)
	    .detail("P99.9", p99_9)
	    .trackLatest(latencySampleEventHolder->trackingKey);
	MetricCollection* metrics = MetricCollection::getMetricCollection();
	if (metrics != nullptr) {
		NetworkAddress addr = g_network->getLocalAddress();
		std::string ip_str = addr.ip.toString();
		std::string port_str = std::to_string(addr.port);
		switch (model) {
		case MetricsDataModel::OTLP: {
			// We only want to emit the entire DDSketch if the knob is set
			if (FLOW_KNOBS->METRICS_EMIT_DDSKETCH) {
				if (metrics->histMap.find(IMetric::id) != metrics->histMap.end()) {
					metrics->histMap[IMetric::id].points.emplace_back(
					    sketch.getErrorGuarantee(), sketch.getSamples(), sketch.min(), sketch.max(), sketch.getSum());
				} else {
					metrics->histMap[IMetric::id] = OTEL::OTELHistogram(name,
					                                                    sketch.getErrorGuarantee(),
					                                                    sketch.getSamples(),
					                                                    sketch.min(),
					                                                    sketch.max(),
					                                                    sketch.getSum());
				}
				metrics->histMap[IMetric::id].points.back().addAttribute("ip", ip_str);
				metrics->histMap[IMetric::id].points.back().addAttribute("port", port_str);
				metrics->histMap[IMetric::id].points.back().startTime = sampleEmit;
			}
			createOtelGauge(p50id, name + "p50", p50);
			createOtelGauge(p90id, name + "p90", p90);
			createOtelGauge(p95id, name + "p95", p95);
			createOtelGauge(p99id, name + "p99", p99);
			createOtelGauge(p999id, name + "p99_9", p99_9);
		}
		case MetricsDataModel::STATSD: {
			std::vector<std::pair<std::string, std::string>> statsd_attributes{ { "ip", ip_str },
				                                                                { "port", port_str } };
			auto median_gauge =
			    createStatsdMessage(name + "p50", StatsDMetric::GAUGE, std::to_string(p50) /*, statsd_attributes*/);
			auto p90_gauge =
			    createStatsdMessage(name + "p90", StatsDMetric::GAUGE, std::to_string(p90) /*, statsd_attributes*/);
			auto p95_gauge =
			    createStatsdMessage(name + "p95", StatsDMetric::GAUGE, std::to_string(p95) /*, statsd_attributes*/);
			auto p99_gauge =
			    createStatsdMessage(name + "p99", StatsDMetric::GAUGE, std::to_string(p99) /*, statsd_attributes*/);
			auto p999_gauge =
			    createStatsdMessage(name + "p99.9", StatsDMetric::GAUGE, std::to_string(p99_9) /*, statsd_attributes*/);
		}
		case MetricsDataModel::NONE:
		default: {
		}
		}
	}
	sketch.clear();
	sampleEmit = now();
}
