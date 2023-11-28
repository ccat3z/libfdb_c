#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
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
#include "flow/actorcompiler.h" // has to be last include

Counter::Counter(std::string const& name, CounterCollection& collection)
  : name(name), interval_start(0), last_event(0), interval_sq_time(0), roughness_interval_start(0), interval_delta(0),
    interval_start_value(0) {
	metric.init(collection.name + "." + (char)toupper(name.at(0)) + name.substr(1), collection.id);
	collection.counters.push_back(this);
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

void CounterCollection::logToTraceEvent(TraceEvent& te) const {
	for (ICounter* c : counters) {
		te.detail(c->getName().c_str(), c);
		c->resetInterval();
	}
}

															#line 93 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
namespace {
// This generated class is to be used only via traceCounters()
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
template <class TraceCountersActor>
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
class TraceCountersActorState {
															#line 100 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
public:
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	TraceCountersActorState(std::string const& traceEventName,UID const& traceEventID,double const& interval,CounterCollection* const& counters,std::string const& trackLatestName,std::function<void(TraceEvent&)> const& decorator) 
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		 : traceEventName(traceEventName),
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		   traceEventID(traceEventID),
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		   interval(interval),
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		   counters(counters),
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		   trackLatestName(trackLatestName),
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		   decorator(decorator)
															#line 117 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
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
															#line 97 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 97 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			if (static_cast<TraceCountersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 134 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TraceCountersActor*>(this)->actor_wait_state = 1;
															#line 97 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TraceCountersActor, 0, Void >*>(static_cast<TraceCountersActor*>(this)));
															#line 139 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
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
															#line 99 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		for( ICounter* c : counters->counters ) {
															#line 100 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			c->resetInterval();
															#line 164 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		}
															#line 102 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		traceEventHolder = Reference<EventCacheHolder>();
															#line 103 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		if (!trackLatestName.empty())
															#line 170 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		{
															#line 104 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			traceEventHolder = makeReference<EventCacheHolder>(trackLatestName);
															#line 174 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		}
															#line 107 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		last_interval = now();
															#line 109 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		;
															#line 180 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 99 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		for( ICounter* c : counters->counters ) {
															#line 100 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			c->resetInterval();
															#line 191 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		}
															#line 102 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		traceEventHolder = Reference<EventCacheHolder>();
															#line 103 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		if (!trackLatestName.empty())
															#line 197 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		{
															#line 104 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			traceEventHolder = makeReference<EventCacheHolder>(trackLatestName);
															#line 201 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		}
															#line 107 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		last_interval = now();
															#line 109 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		;
															#line 207 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
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
															#line 110 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		TraceEvent te(traceEventName.c_str(), traceEventID);
															#line 111 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		te.detail("Elapsed", now() - last_interval);
															#line 113 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		counters->logToTraceEvent(te);
															#line 114 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		decorator(te);
															#line 116 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		if (!trackLatestName.empty())
															#line 294 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		{
															#line 117 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
			te.trackLatest(traceEventHolder->trackingKey);
															#line 298 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		}
															#line 120 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		last_interval = now();
															#line 121 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(interval, TaskPriority::FlushTrace);
															#line 121 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		if (static_cast<TraceCountersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 306 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TraceCountersActor*>(this)->actor_wait_state = 2;
															#line 121 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TraceCountersActor, 1, Void >*>(static_cast<TraceCountersActor*>(this)));
															#line 311 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
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
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	std::string traceEventName;
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	UID traceEventID;
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	double interval;
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	CounterCollection* counters;
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	std::string trackLatestName;
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	std::function<void(TraceEvent&)> decorator;
															#line 102 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	Reference<EventCacheHolder> traceEventHolder;
															#line 107 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	double last_interval;
															#line 407 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
};
// This generated class is to be used only via traceCounters()
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
class TraceCountersActor final : public Actor<Void>, public ActorCallback< TraceCountersActor, 0, Void >, public ActorCallback< TraceCountersActor, 1, Void >, public FastAllocated<TraceCountersActor>, public TraceCountersActorState<TraceCountersActor> {
															#line 412 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
public:
	using FastAllocated<TraceCountersActor>::operator new;
	using FastAllocated<TraceCountersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TraceCountersActor, 0, Void >;
friend struct ActorCallback< TraceCountersActor, 1, Void >;
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	TraceCountersActor(std::string const& traceEventName,UID const& traceEventID,double const& interval,CounterCollection* const& counters,std::string const& trackLatestName,std::function<void(TraceEvent&)> const& decorator) 
															#line 424 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
		 : Actor<Void>(),
		   TraceCountersActorState<TraceCountersActor>(traceEventName, traceEventID, interval, counters, trackLatestName, decorator)
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
}
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
[[nodiscard]] Future<Void> traceCounters( std::string const& traceEventName, UID const& traceEventID, double const& interval, CounterCollection* const& counters, std::string const& trackLatestName, std::function<void(TraceEvent&)> const& decorator ) {
															#line 91 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
	return Future<Void>(new TraceCountersActor(traceEventName, traceEventID, interval, counters, trackLatestName, decorator));
															#line 453 "/usr/src/libfdb_c/fdbrpc/Stats.actor.g.cpp"
}

#line 124 "/usr/src/libfdb_c/fdbrpc/Stats.actor.cpp"
