#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
/*
 * FailureMonitor.actor.cpp
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

#include "fdbrpc/FailureMonitor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 26 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
namespace {
// This generated class is to be used only via waitForStateEqual()
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
template <class WaitForStateEqualActor>
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
class WaitForStateEqualActorState {
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
public:
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	WaitForStateEqualActorState(IFailureMonitor* const& monitor,Endpoint const& endpoint,FailureStatus const& status) 
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		 : monitor(monitor),
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		   endpoint(endpoint),
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		   status(status)
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
	{
		fdb_probe_actor_create("waitForStateEqual", reinterpret_cast<unsigned long>(this));

	}
	~WaitForStateEqualActorState() 
	{
		fdb_probe_actor_destroy("waitForStateEqual", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
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
		this->~WaitForStateEqualActorState();
		static_cast<WaitForStateEqualActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 26 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		Future<Void> change = monitor->onStateChanged(endpoint);
															#line 27 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (monitor->getState(endpoint) == status)
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		{
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			if (!static_cast<WaitForStateEqualActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForStateEqualActorState(); static_cast<WaitForStateEqualActor*>(this)->destroy(); return 0; }
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
			new (&static_cast<WaitForStateEqualActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForStateEqualActorState();
			static_cast<WaitForStateEqualActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 29 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		StrictFuture<Void> __when_expr_0 = change;
															#line 29 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (static_cast<WaitForStateEqualActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WaitForStateEqualActor*>(this)->actor_wait_state = 1;
															#line 29 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForStateEqualActor, 0, Void >*>(static_cast<WaitForStateEqualActor*>(this)));
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
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
		if (static_cast<WaitForStateEqualActor*>(this)->actor_wait_state > 0) static_cast<WaitForStateEqualActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForStateEqualActor*>(this)->ActorCallback< WaitForStateEqualActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForStateEqualActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForStateEqual", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForStateEqual", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForStateEqualActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForStateEqual", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForStateEqual", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForStateEqualActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForStateEqual", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForStateEqual", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	IFailureMonitor* monitor;
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	Endpoint endpoint;
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	FailureStatus status;
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
};
// This generated class is to be used only via waitForStateEqual()
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
class WaitForStateEqualActor final : public Actor<Void>, public ActorCallback< WaitForStateEqualActor, 0, Void >, public FastAllocated<WaitForStateEqualActor>, public WaitForStateEqualActorState<WaitForStateEqualActor> {
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
public:
	using FastAllocated<WaitForStateEqualActor>::operator new;
	using FastAllocated<WaitForStateEqualActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForStateEqualActor, 0, Void >;
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	WaitForStateEqualActor(IFailureMonitor* const& monitor,Endpoint const& endpoint,FailureStatus const& status) 
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		 : Actor<Void>(),
		   WaitForStateEqualActorState<WaitForStateEqualActor>(monitor, endpoint, status)
	{
		fdb_probe_actor_enter("waitForStateEqual", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForStateEqual");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForStateEqual", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForStateEqualActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
[[nodiscard]] Future<Void> waitForStateEqual( IFailureMonitor* const& monitor, Endpoint const& endpoint, FailureStatus const& status ) {
															#line 24 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	return Future<Void>(new WaitForStateEqualActor(monitor, endpoint, status));
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
}

#line 32 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"

															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
namespace {
// This generated class is to be used only via waitForContinuousFailure()
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
template <class WaitForContinuousFailureActor>
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
class WaitForContinuousFailureActorState {
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
public:
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	WaitForContinuousFailureActorState(IFailureMonitor* const& monitor,Endpoint const& endpoint,double const& sustainedFailureDuration,double const& slope) 
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		 : monitor(monitor),
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		   endpoint(endpoint),
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		   sustainedFailureDuration(sustainedFailureDuration),
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		   slope(slope),
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		   startT(now())
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
	{
		fdb_probe_actor_create("waitForContinuousFailure", reinterpret_cast<unsigned long>(this));

	}
	~WaitForContinuousFailureActorState() 
	{
		fdb_probe_actor_destroy("waitForContinuousFailure", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			;
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
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
		this->~WaitForContinuousFailureActorState();
		static_cast<WaitForContinuousFailureActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		StrictFuture<Void> __when_expr_0 = monitor->onFailed(endpoint);
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state = 1;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForContinuousFailureActor, 0, Void >*>(static_cast<WaitForContinuousFailureActor*>(this)));
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (monitor->permanentlyFailed(endpoint))
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		{
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			if (!static_cast<WaitForContinuousFailureActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForContinuousFailureActorState(); static_cast<WaitForContinuousFailureActor*>(this)->destroy(); return 0; }
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
			new (&static_cast<WaitForContinuousFailureActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForContinuousFailureActorState();
			static_cast<WaitForContinuousFailureActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		double waitDelay = (sustainedFailureDuration + slope * (now() - startT)) / (1 - slope);
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (waitDelay < std::min(FLOW_KNOBS->CLIENT_REQUEST_INTERVAL, FLOW_KNOBS->SERVER_REQUEST_INTERVAL))
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		{
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			waitDelay = 0;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		StrictFuture<Void> __when_expr_1 = monitor->onStateEqual(endpoint, FailureStatus(false));
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(waitDelay);
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
		static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state = 2;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForContinuousFailureActor, 1, Void >*>(static_cast<WaitForContinuousFailureActor*>(this)));
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WaitForContinuousFailureActor, 2, Void >*>(static_cast<WaitForContinuousFailureActor*>(this)));
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (monitor->permanentlyFailed(endpoint))
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		{
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			if (!static_cast<WaitForContinuousFailureActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForContinuousFailureActorState(); static_cast<WaitForContinuousFailureActor*>(this)->destroy(); return 0; }
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
			new (&static_cast<WaitForContinuousFailureActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForContinuousFailureActorState();
			static_cast<WaitForContinuousFailureActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		double waitDelay = (sustainedFailureDuration + slope * (now() - startT)) / (1 - slope);
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (waitDelay < std::min(FLOW_KNOBS->CLIENT_REQUEST_INTERVAL, FLOW_KNOBS->SERVER_REQUEST_INTERVAL))
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		{
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
			waitDelay = 0;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		StrictFuture<Void> __when_expr_1 = monitor->onStateEqual(endpoint, FailureStatus(false));
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(waitDelay);
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
		static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state = 2;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForContinuousFailureActor, 1, Void >*>(static_cast<WaitForContinuousFailureActor*>(this)));
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WaitForContinuousFailureActor, 2, Void >*>(static_cast<WaitForContinuousFailureActor*>(this)));
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
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
		if (static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state > 0) static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForContinuousFailureActor*>(this)->ActorCallback< WaitForContinuousFailureActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForContinuousFailureActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForContinuousFailureActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForContinuousFailureActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void const& _,int loopDepth) 
	{
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (!static_cast<WaitForContinuousFailureActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForContinuousFailureActorState(); static_cast<WaitForContinuousFailureActor*>(this)->destroy(); return 0; }
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		new (&static_cast<WaitForContinuousFailureActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitForContinuousFailureActorState();
		static_cast<WaitForContinuousFailureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void && _,int loopDepth) 
	{
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
		if (!static_cast<WaitForContinuousFailureActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForContinuousFailureActorState(); static_cast<WaitForContinuousFailureActor*>(this)->destroy(); return 0; }
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		new (&static_cast<WaitForContinuousFailureActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitForContinuousFailureActorState();
		static_cast<WaitForContinuousFailureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state > 0) static_cast<WaitForContinuousFailureActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForContinuousFailureActor*>(this)->ActorCallback< WaitForContinuousFailureActor, 1, Void >::remove();
		static_cast<WaitForContinuousFailureActor*>(this)->ActorCallback< WaitForContinuousFailureActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForContinuousFailureActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitForContinuousFailureActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitForContinuousFailureActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitForContinuousFailureActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WaitForContinuousFailureActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WaitForContinuousFailureActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	IFailureMonitor* monitor;
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	Endpoint endpoint;
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	double sustainedFailureDuration;
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	double slope;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	double startT;
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
};
// This generated class is to be used only via waitForContinuousFailure()
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
class WaitForContinuousFailureActor final : public Actor<Void>, public ActorCallback< WaitForContinuousFailureActor, 0, Void >, public ActorCallback< WaitForContinuousFailureActor, 1, Void >, public ActorCallback< WaitForContinuousFailureActor, 2, Void >, public FastAllocated<WaitForContinuousFailureActor>, public WaitForContinuousFailureActorState<WaitForContinuousFailureActor> {
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
public:
	using FastAllocated<WaitForContinuousFailureActor>::operator new;
	using FastAllocated<WaitForContinuousFailureActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForContinuousFailureActor, 0, Void >;
friend struct ActorCallback< WaitForContinuousFailureActor, 1, Void >;
friend struct ActorCallback< WaitForContinuousFailureActor, 2, Void >;
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	WaitForContinuousFailureActor(IFailureMonitor* const& monitor,Endpoint const& endpoint,double const& sustainedFailureDuration,double const& slope) 
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
		 : Actor<Void>(),
		   WaitForContinuousFailureActorState<WaitForContinuousFailureActor>(monitor, endpoint, sustainedFailureDuration, slope)
	{
		fdb_probe_actor_enter("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForContinuousFailure");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForContinuousFailure", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForContinuousFailureActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitForContinuousFailureActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
[[nodiscard]] Future<Void> waitForContinuousFailure( IFailureMonitor* const& monitor, Endpoint const& endpoint, double const& sustainedFailureDuration, double const& slope ) {
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"
	return Future<Void>(new WaitForContinuousFailureActor(monitor, endpoint, sustainedFailureDuration, slope));
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.g.cpp"
}

#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FailureMonitor.actor.cpp"

Future<Void> IFailureMonitor::onStateEqual(Endpoint const& endpoint, FailureStatus status) {
	if (status == getState(endpoint))
		return Void();
	return waitForStateEqual(this, endpoint, status);
}

Future<Void> IFailureMonitor::onFailedFor(Endpoint const& endpoint, double sustainedFailureDuration, double slope) {
	ASSERT(slope < 1.0);
	return waitForContinuousFailure(this, endpoint, sustainedFailureDuration, slope);
}

SimpleFailureMonitor::SimpleFailureMonitor() {
	// Mark ourselves as available in FailureMonitor
	const auto& localAddresses = FlowTransport::transport().getLocalAddresses();
	addressStatus[localAddresses.address] = FailureStatus(false);
	if (localAddresses.secondaryAddress.present()) {
		addressStatus[localAddresses.secondaryAddress.get()] = FailureStatus(false);
	}
}

void SimpleFailureMonitor::setStatus(NetworkAddress const& address, FailureStatus const& status) {

	// if (status.failed)
	//	printf("On machine '%s': Machine '%s' is failed\n", g_network->getLocalAddress().toString().c_str(),
	//         address.toString().c_str()); printf("%s.setState(%s, %s) %p\n", g_network->getLocalAddress().toString(),
	//         address.toString(), status.failed ? "FAILED" : "OK", this); addressStatus.set( address, status );

	// onStateChanged() will be waiting on endpointKnownFailed only where it is false, so if the address status
	// for an endpoint that is waited on changes, the waiter sees its failure status change
	auto it = addressStatus.find(address);

	if (it == addressStatus.end()) {
		if (status != FailureStatus()) {
			TraceEvent("NotifyAddressHealthy").suppressFor(1.0).detail("Address", address);
			addressStatus[address] = status;
			endpointKnownFailed.triggerRange(Endpoint({ address }, UID()), Endpoint({ address }, UID(-1, -1)));
		}
	} else {
		bool triggerEndpoint = status != it->second;
		if (status != FailureStatus())
			it->second = status;
		else
			addressStatus.erase(it);
		if (triggerEndpoint) {
			if (status.failed) {
				TraceEvent("NotifyAddressFailed").suppressFor(1.0).detail("Address", address);
			} else {
				TraceEvent("NotifyAddressHealthyPresent").suppressFor(1.0).detail("Address", address);
			}
			endpointKnownFailed.triggerRange(Endpoint({ address }, UID()), Endpoint({ address }, UID(-1, -1)));
		}
	}
}

void SimpleFailureMonitor::endpointNotFound(Endpoint const& endpoint) {
	// SOMEDAY: Expiration (this "leaks" memory)
	if (endpoint.token.first() == -1) {
		TraceEvent("WellKnownEndpointNotFound")
		    .suppressFor(1.0)
		    .detail("Address", endpoint.getPrimaryAddress())
		    .detail("TokenFirst", endpoint.token.first())
		    .detail("TokenSecond", endpoint.token.second());
		return;
	}
	TraceEvent("EndpointNotFound")
	    .suppressFor(1.0)
	    .detail("Address", endpoint.getPrimaryAddress())
	    .detail("Token", endpoint.token);
	if (endpoint.getPrimaryAddress().isPublic()) {
		if (failedEndpoints.size() > 100000) {
			TraceEvent(SevWarnAlways, "TooManyFailedEndpoints").suppressFor(1.0);
			failedEndpoints.clear();
		}
		failedEndpoints.emplace(endpoint, FailedReason::NOT_FOUND);
	}
	endpointKnownFailed.trigger(endpoint);
}

void SimpleFailureMonitor::unauthorizedEndpoint(Endpoint const& endpoint) {
	TraceEvent(g_network->isSimulated() ? SevWarnAlways : SevError, "TriedAccessPrivateEndpoint")
	    .suppressFor(1.0)
	    .detail("Address", endpoint.getPrimaryAddress())
	    .detail("Token", endpoint.token);
	failedEndpoints.emplace(endpoint, FailedReason::UNAUTHORIZED);
	endpointKnownFailed.trigger(endpoint);
}

void SimpleFailureMonitor::notifyDisconnect(NetworkAddress const& address) {
	//TraceEvent("NotifyDisconnect").detail("Address", address);
	endpointKnownFailed.triggerRange(Endpoint({ address }, UID()), Endpoint({ address }, UID(-1, -1)));
	disconnectTriggers.trigger(address);
}

Future<Void> SimpleFailureMonitor::onDisconnectOrFailure(Endpoint const& endpoint) {
	// If the endpoint or address is already failed, return right away
	auto i = addressStatus.find(endpoint.getPrimaryAddress());
	if (i == addressStatus.end() || i->second.isFailed() || failedEndpoints.count(endpoint)) {
		TraceEvent event("AlreadyDisconnected");
		if (endpoint.token.first() == 0xffffffffffffffff) {
			// well known endpoint
			event.suppressFor(5.0);
		} else {
			event.suppressFor(0.1);
		}
		event.detail("Addr", endpoint.getPrimaryAddress())
		    .detail("Reason", i == addressStatus.end() || i->second.isFailed() ? "Disconnected" : "EndpointFailed")
		    .detail("Tok", endpoint.token)
		    .log();
		return Void();
	}

	// Return when the endpoint is triggered, which means that either the endpoint has become known failed, or the
	//   address has changed state (and since it was previously not failed, it must now be failed), or
	//   notifyDisconnect() has been called.
	return endpointKnownFailed.onChange(endpoint);
}

Future<Void> SimpleFailureMonitor::onDisconnect(NetworkAddress const& address) {
	return disconnectTriggers.onChange(address);
}

Future<Void> SimpleFailureMonitor::onStateChanged(Endpoint const& endpoint) {
	// Wait on endpointKnownFailed if it is false, to pick up both endpointNotFound errors (which set it to true)
	//   and changes to addressStatus (which trigger a range).  Don't wait on endpointKnownFailed if it is true, because
	//   failure status for that endpoint can never change (and we could be spuriously triggered by setStatus)
	// Also returns spuriously when notifyDisconnect is called (which doesn't actually change the state), but callers
	//   check the state so it's OK
	if (failedEndpoints.count(endpoint))
		return Never();
	else
		return endpointKnownFailed.onChange(endpoint);
}

FailureStatus SimpleFailureMonitor::getState(Endpoint const& endpoint) const {
	if (failedEndpoints.count(endpoint))
		return FailureStatus(true);
	else {
		auto a = addressStatus.find(endpoint.getPrimaryAddress());
		if (a == addressStatus.end())
			return FailureStatus();
		else
			return a->second;
		// printf("%s.getState(%s) = %s %p\n", g_network->getLocalAddress().toString(), endpoint.address.toString(),
		//        a.failed ? "FAILED" : "OK", this);
	}
}

FailureStatus SimpleFailureMonitor::getState(NetworkAddress const& address) const {
	auto a = addressStatus.find(address);
	if (a == addressStatus.end())
		return FailureStatus();
	else
		return a->second;
}

bool SimpleFailureMonitor::onlyEndpointFailed(Endpoint const& endpoint) const {
	if (!failedEndpoints.count(endpoint))
		return false;
	auto a = addressStatus.find(endpoint.getPrimaryAddress());
	if (a == addressStatus.end())
		return true;
	else
		return !a->second.failed;
}

bool SimpleFailureMonitor::permanentlyFailed(Endpoint const& endpoint) const {
	return failedEndpoints.count(endpoint);
}

bool SimpleFailureMonitor::knownUnauthorized(Endpoint const& endpoint) const {
	auto iter = failedEndpoints.find(endpoint);
	return iter != failedEndpoints.end() && iter->second == FailedReason::UNAUTHORIZED;
}

void SimpleFailureMonitor::reset() {
	addressStatus = std::unordered_map<NetworkAddress, FailureStatus>();
	failedEndpoints = std::unordered_map<Endpoint, FailedReason>();
	endpointKnownFailed.resetNoWaiting();
}
