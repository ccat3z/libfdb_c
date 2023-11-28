#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
/*
 * genericactors.actor.h
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

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FDBRPC_GENERICACTORS_ACTOR_G_H)
#define FDBRPC_GENERICACTORS_ACTOR_G_H
#include "fdbrpc/genericactors.actor.g.h"
#elif !defined(RPCGENERICACTORS_ACTOR_H)
#define RPCGENERICACTORS_ACTOR_H

#include "flow/genericactors.actor.h"
#include "fdbrpc/fdbrpc.h"
#include "fdbclient/WellKnownEndpoints.h"
#include "flow/Hostname.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 37 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryBrokenPromise()
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class RetryBrokenPromiseActor>
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActorState {
															#line 44 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActorState(RequestStream<Req> const& to,Req const& request) 
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : to(to),
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   request(request)
															#line 53 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("retryBrokenPromise", reinterpret_cast<unsigned long>(this));

	}
	~RetryBrokenPromiseActorState() 
	{
		fdb_probe_actor_destroy("retryBrokenPromise", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 41 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 68 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~RetryBrokenPromiseActorState();
		static_cast<RetryBrokenPromiseActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 43 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<REPLY_TYPE(Req)> __when_expr_0 = to.getReply(request);
															#line 43 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 101 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state = 1;
															#line 43 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor, 0, REPLY_TYPE(Req) >*>(static_cast<RetryBrokenPromiseActor*>(this)));
															#line 106 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 46 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() != error_code_broken_promise)
															#line 128 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 47 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 132 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
															#line 48 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 49 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delayJittered(FLOW_KNOBS->PREVENT_FAST_SPIN_DELAY);
															#line 49 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 140 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state = 2;
															#line 49 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor, 1, Void >*>(static_cast<RetryBrokenPromiseActor*>(this)));
															#line 145 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(REPLY_TYPE(Req) const& reply,int loopDepth) 
	{
															#line 44 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActorState(); static_cast<RetryBrokenPromiseActor*>(this)->destroy(); return 0; }
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<RetryBrokenPromiseActor*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply);
		this->~RetryBrokenPromiseActorState();
		static_cast<RetryBrokenPromiseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(REPLY_TYPE(Req) && reply,int loopDepth) 
	{
															#line 44 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActorState(); static_cast<RetryBrokenPromiseActor*>(this)->destroy(); return 0; }
															#line 172 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<RetryBrokenPromiseActor*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply);
		this->~RetryBrokenPromiseActorState();
		static_cast<RetryBrokenPromiseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(REPLY_TYPE(Req) const& reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(reply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(REPLY_TYPE(Req) && reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state > 0) static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state = 0;
		static_cast<RetryBrokenPromiseActor*>(this)->ActorCallback< RetryBrokenPromiseActor, 0, REPLY_TYPE(Req) >::remove();

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor, 0, REPLY_TYPE(Req) >*,REPLY_TYPE(Req) const& value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor, 0, REPLY_TYPE(Req) >*,REPLY_TYPE(Req) && value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RetryBrokenPromiseActor, 0, REPLY_TYPE(Req) >*,Error err) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 50 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		TEST(true);
															#line 247 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 50 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		TEST(true);
															#line 256 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state > 0) static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state = 0;
		static_cast<RetryBrokenPromiseActor*>(this)->ActorCallback< RetryBrokenPromiseActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RetryBrokenPromiseActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RequestStream<Req> to;
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Req request;
															#line 328 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryBrokenPromise()
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActor final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryBrokenPromiseActor<Req>, 0, REPLY_TYPE(Req) >, public ActorCallback< RetryBrokenPromiseActor<Req>, 1, Void >, public FastAllocated<RetryBrokenPromiseActor<Req>>, public RetryBrokenPromiseActorState<Req, RetryBrokenPromiseActor<Req>> {
															#line 335 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<RetryBrokenPromiseActor<Req>>::operator new;
	using FastAllocated<RetryBrokenPromiseActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Req)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RetryBrokenPromiseActor<Req>, 0, REPLY_TYPE(Req) >;
friend struct ActorCallback< RetryBrokenPromiseActor<Req>, 1, Void >;
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActor(RequestStream<Req> const& to,Req const& request) 
															#line 347 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<REPLY_TYPE(Req)>(),
		   RetryBrokenPromiseActorState<Req, RetryBrokenPromiseActor<Req>>(to, request)
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("retryBrokenPromise");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor<Req>, 0, REPLY_TYPE(Req) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor<Req>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryBrokenPromise( RequestStream<Req> const& to, Req const& request ) {
															#line 35 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryBrokenPromiseActor<Req>(to, request));
															#line 378 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 54 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 383 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryBrokenPromise()
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class RetryBrokenPromiseActor1>
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActor1State {
															#line 390 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActor1State(RequestStream<Req> const& to,Req const& request,TaskPriority const& taskID) 
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : to(to),
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   request(request),
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   taskID(taskID)
															#line 401 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("retryBrokenPromise", reinterpret_cast<unsigned long>(this));

	}
	~RetryBrokenPromiseActor1State() 
	{
		fdb_probe_actor_destroy("retryBrokenPromise", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 61 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 416 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~RetryBrokenPromiseActor1State();
		static_cast<RetryBrokenPromiseActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 63 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<REPLY_TYPE(Req)> __when_expr_0 = to.getReply(request, taskID);
															#line 63 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 449 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state = 1;
															#line 63 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor1, 0, REPLY_TYPE(Req) >*>(static_cast<RetryBrokenPromiseActor1*>(this)));
															#line 454 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 66 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() != error_code_broken_promise)
															#line 476 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 67 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 480 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
															#line 68 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 69 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delayJittered(FLOW_KNOBS->PREVENT_FAST_SPIN_DELAY, taskID);
															#line 69 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 488 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state = 2;
															#line 69 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor1, 1, Void >*>(static_cast<RetryBrokenPromiseActor1*>(this)));
															#line 493 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(REPLY_TYPE(Req) const& reply,int loopDepth) 
	{
															#line 64 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor1*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActor1State(); static_cast<RetryBrokenPromiseActor1*>(this)->destroy(); return 0; }
															#line 508 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<RetryBrokenPromiseActor1*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply);
		this->~RetryBrokenPromiseActor1State();
		static_cast<RetryBrokenPromiseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(REPLY_TYPE(Req) && reply,int loopDepth) 
	{
															#line 64 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor1*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActor1State(); static_cast<RetryBrokenPromiseActor1*>(this)->destroy(); return 0; }
															#line 520 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<RetryBrokenPromiseActor1*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply);
		this->~RetryBrokenPromiseActor1State();
		static_cast<RetryBrokenPromiseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(REPLY_TYPE(Req) const& reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(reply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(REPLY_TYPE(Req) && reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state > 0) static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state = 0;
		static_cast<RetryBrokenPromiseActor1*>(this)->ActorCallback< RetryBrokenPromiseActor1, 0, REPLY_TYPE(Req) >::remove();

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor1, 0, REPLY_TYPE(Req) >*,REPLY_TYPE(Req) const& value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor1, 0, REPLY_TYPE(Req) >*,REPLY_TYPE(Req) && value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RetryBrokenPromiseActor1, 0, REPLY_TYPE(Req) >*,Error err) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 70 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		TEST(true);
															#line 595 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 70 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		TEST(true);
															#line 604 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state > 0) static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state = 0;
		static_cast<RetryBrokenPromiseActor1*>(this)->ActorCallback< RetryBrokenPromiseActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RetryBrokenPromiseActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RetryBrokenPromiseActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RequestStream<Req> to;
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Req request;
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TaskPriority taskID;
															#line 678 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryBrokenPromise()
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActor1 final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryBrokenPromiseActor1<Req>, 0, REPLY_TYPE(Req) >, public ActorCallback< RetryBrokenPromiseActor1<Req>, 1, Void >, public FastAllocated<RetryBrokenPromiseActor1<Req>>, public RetryBrokenPromiseActor1State<Req, RetryBrokenPromiseActor1<Req>> {
															#line 685 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<RetryBrokenPromiseActor1<Req>>::operator new;
	using FastAllocated<RetryBrokenPromiseActor1<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Req)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RetryBrokenPromiseActor1<Req>, 0, REPLY_TYPE(Req) >;
friend struct ActorCallback< RetryBrokenPromiseActor1<Req>, 1, Void >;
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActor1(RequestStream<Req> const& to,Req const& request,TaskPriority const& taskID) 
															#line 697 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<REPLY_TYPE(Req)>(),
		   RetryBrokenPromiseActor1State<Req, RetryBrokenPromiseActor1<Req>>(to, request, taskID)
	{
		fdb_probe_actor_enter("retryBrokenPromise", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("retryBrokenPromise");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("retryBrokenPromise", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor1<Req>, 0, REPLY_TYPE(Req) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor1<Req>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryBrokenPromise( RequestStream<Req> const& to, Req const& request, TaskPriority const& taskID ) {
															#line 55 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryBrokenPromiseActor1<Req>(to, request, taskID));
															#line 728 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 74 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 733 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tryInitializeRequestStream()
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class TryInitializeRequestStreamActor>
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TryInitializeRequestStreamActorState {
															#line 740 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TryInitializeRequestStreamActorState(RequestStream<Req>* const& stream,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : stream(stream),
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   token(token)
															#line 751 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this));

	}
	~TryInitializeRequestStreamActorState() 
	{
		fdb_probe_actor_destroy("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 77 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hostname.resolve();
															#line 77 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<TryInitializeRequestStreamActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 768 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryInitializeRequestStreamActor*>(this)->actor_wait_state = 1;
															#line 77 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryInitializeRequestStreamActor, 0, Optional<NetworkAddress> >*>(static_cast<TryInitializeRequestStreamActor*>(this)));
															#line 773 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~TryInitializeRequestStreamActorState();
		static_cast<TryInitializeRequestStreamActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 796 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 79 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 800 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TryInitializeRequestStreamActorState();
			static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 81 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		ASSERT(stream != nullptr);
															#line 82 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		*stream = RequestStream<Req>(Endpoint::wellKnown({ address.get() }, token));
															#line 83 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 812 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TryInitializeRequestStreamActorState();
		static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 824 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 79 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 828 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TryInitializeRequestStreamActorState();
			static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 81 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		ASSERT(stream != nullptr);
															#line 82 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		*stream = RequestStream<Req>(Endpoint::wellKnown({ address.get() }, token));
															#line 83 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 840 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TryInitializeRequestStreamActorState();
		static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
		loopDepth = a_body1cont1(address, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> && address,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(address), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryInitializeRequestStreamActor*>(this)->actor_wait_state > 0) static_cast<TryInitializeRequestStreamActor*>(this)->actor_wait_state = 0;
		static_cast<TryInitializeRequestStreamActor*>(this)->ActorCallback< TryInitializeRequestStreamActor, 0, Optional<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< TryInitializeRequestStreamActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryInitializeRequestStreamActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryInitializeRequestStreamActor, 0, Optional<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RequestStream<Req>* stream;
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 917 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via tryInitializeRequestStream()
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TryInitializeRequestStreamActor final : public Actor<Void>, public ActorCallback< TryInitializeRequestStreamActor<Req>, 0, Optional<NetworkAddress> >, public FastAllocated<TryInitializeRequestStreamActor<Req>>, public TryInitializeRequestStreamActorState<Req, TryInitializeRequestStreamActor<Req>> {
															#line 924 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TryInitializeRequestStreamActor<Req>>::operator new;
	using FastAllocated<TryInitializeRequestStreamActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryInitializeRequestStreamActor<Req>, 0, Optional<NetworkAddress> >;
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TryInitializeRequestStreamActor(RequestStream<Req>* const& stream,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 935 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TryInitializeRequestStreamActorState<Req, TryInitializeRequestStreamActor<Req>>(stream, hostname, token)
	{
		fdb_probe_actor_enter("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryInitializeRequestStream");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryInitializeRequestStream", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryInitializeRequestStreamActor<Req>, 0, Optional<NetworkAddress> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> tryInitializeRequestStream( RequestStream<Req>* const& stream, Hostname const& hostname, WellKnownEndpoints const& token ) {
															#line 75 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<Void>(new TryInitializeRequestStreamActor<Req>(stream, hostname, token));
															#line 965 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 85 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 970 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class TryGetReplyFromHostnameActor>
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActorState {
															#line 977 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActorState(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   token(token)
															#line 988 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	~TryGetReplyFromHostnameActorState() 
	{
		fdb_probe_actor_destroy("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 91 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hostname.resolve();
															#line 91 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1005 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 1;
															#line 91 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor, 0, Optional<NetworkAddress> >*>(static_cast<TryGetReplyFromHostnameActor*>(this)));
															#line 1010 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~TryGetReplyFromHostnameActorState();
		static_cast<TryGetReplyFromHostnameActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
															#line 92 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1033 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 93 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActorState(); static_cast<TryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1037 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActorState();
			static_cast<TryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 95 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request);
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1049 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor*>(this)));
															#line 1054 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 92 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1063 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 93 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActorState(); static_cast<TryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1067 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActorState();
			static_cast<TryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 95 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request);
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1079 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor*>(this)));
															#line 1084 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
		loopDepth = a_body1cont1(address, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> && address,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(address), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state > 0) static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetReplyFromHostnameActor*>(this)->ActorCallback< TryGetReplyFromHostnameActor, 0, Optional<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetReplyFromHostnameActor, 0, Optional<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 97 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 1156 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 98 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 99 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 1162 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 101 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				INetworkConnections::net()->removeCachedDNS(hostname.host, hostname.service);
															#line 1166 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
		}
															#line 104 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryGetReplyFromHostnameActor*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(reply); this->~TryGetReplyFromHostnameActorState(); static_cast<TryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1171 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryGetReplyFromHostnameActor*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(std::move(reply)); // state_var_RVO
		this->~TryGetReplyFromHostnameActorState();
		static_cast<TryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 1183 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(ErrorOr<REPLY_TYPE(Req)> && __reply,int loopDepth) 
	{
		reply = std::move(__reply);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state > 0) static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetReplyFromHostnameActor*>(this)->ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> const& value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> && value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Req request;
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 96 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 1254 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActor final : public Actor<ErrorOr<REPLY_TYPE(Req)>>, public ActorCallback< TryGetReplyFromHostnameActor<Req>, 0, Optional<NetworkAddress> >, public ActorCallback< TryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public FastAllocated<TryGetReplyFromHostnameActor<Req>>, public TryGetReplyFromHostnameActorState<Req, TryGetReplyFromHostnameActor<Req>> {
															#line 1261 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TryGetReplyFromHostnameActor<Req>>::operator new;
	using FastAllocated<TryGetReplyFromHostnameActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<REPLY_TYPE(Req)>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetReplyFromHostnameActor<Req>, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< TryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >;
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActor(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 1273 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<ErrorOr<REPLY_TYPE(Req)>>(),
		   TryGetReplyFromHostnameActorState<Req, TryGetReplyFromHostnameActor<Req>>(request, hostname, token)
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetReplyFromHostname");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetReplyFromHostnameActor<Req>, 0, Optional<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<REPLY_TYPE(Req)>> tryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token ) {
															#line 86 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<ErrorOr<REPLY_TYPE(Req)>>(new TryGetReplyFromHostnameActor<Req>(request, hostname, token));
															#line 1304 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 106 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 1309 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class TryGetReplyFromHostnameActor1>
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActor1State {
															#line 1316 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActor1State(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   token(token),
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   taskID(taskID)
															#line 1329 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	~TryGetReplyFromHostnameActor1State() 
	{
		fdb_probe_actor_destroy("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 115 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hostname.resolve();
															#line 115 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1346 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 1;
															#line 115 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor1, 0, Optional<NetworkAddress> >*>(static_cast<TryGetReplyFromHostnameActor1*>(this)));
															#line 1351 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~TryGetReplyFromHostnameActor1State();
		static_cast<TryGetReplyFromHostnameActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
															#line 116 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1374 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 117 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActor1State(); static_cast<TryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 1378 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActor1State();
			static_cast<TryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 119 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request, taskID);
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1390 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor1*>(this)));
															#line 1395 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 116 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1404 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 117 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActor1State(); static_cast<TryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 1408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActor1State();
			static_cast<TryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 119 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request, taskID);
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1420 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor1*>(this)));
															#line 1425 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
		loopDepth = a_body1cont1(address, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> && address,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(address), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state > 0) static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 0;
		static_cast<TryGetReplyFromHostnameActor1*>(this)->ActorCallback< TryGetReplyFromHostnameActor1, 0, Optional<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor1, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor1, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetReplyFromHostnameActor1, 0, Optional<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 121 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 1497 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 122 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 123 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 1503 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 125 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				INetworkConnections::net()->removeCachedDNS(hostname.host, hostname.service);
															#line 1507 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
		}
															#line 128 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(reply); this->~TryGetReplyFromHostnameActor1State(); static_cast<TryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 1512 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(std::move(reply)); // state_var_RVO
		this->~TryGetReplyFromHostnameActor1State();
		static_cast<TryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 1524 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(ErrorOr<REPLY_TYPE(Req)> && __reply,int loopDepth) 
	{
		reply = std::move(__reply);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state > 0) static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 0;
		static_cast<TryGetReplyFromHostnameActor1*>(this)->ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> const& value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> && value) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Req request;
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TaskPriority taskID;
															#line 120 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 1597 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActor1 final : public Actor<ErrorOr<REPLY_TYPE(Req)>>, public ActorCallback< TryGetReplyFromHostnameActor1<Req>, 0, Optional<NetworkAddress> >, public ActorCallback< TryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public FastAllocated<TryGetReplyFromHostnameActor1<Req>>, public TryGetReplyFromHostnameActor1State<Req, TryGetReplyFromHostnameActor1<Req>> {
															#line 1604 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TryGetReplyFromHostnameActor1<Req>>::operator new;
	using FastAllocated<TryGetReplyFromHostnameActor1<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<REPLY_TYPE(Req)>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetReplyFromHostnameActor1<Req>, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< TryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >;
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActor1(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 1616 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<ErrorOr<REPLY_TYPE(Req)>>(),
		   TryGetReplyFromHostnameActor1State<Req, TryGetReplyFromHostnameActor1<Req>>(request, hostname, token, taskID)
	{
		fdb_probe_actor_enter("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetReplyFromHostname");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetReplyFromHostnameActor1<Req>, 0, Optional<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<REPLY_TYPE(Req)>> tryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token, TaskPriority const& taskID ) {
															#line 107 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<ErrorOr<REPLY_TYPE(Req)>>(new TryGetReplyFromHostnameActor1<Req>(request, hostname, token, taskID));
															#line 1647 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 130 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 1652 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class RetryGetReplyFromHostnameActor>
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActorState {
															#line 1659 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActorState(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   token(token),
															#line 136 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   reconnetInterval(FLOW_KNOBS->HOSTNAME_RECONNECT_INIT_INTERVAL),
															#line 137 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   to()
															#line 1674 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	~RetryGetReplyFromHostnameActorState() 
	{
		fdb_probe_actor_destroy("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 138 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 1689 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~RetryGetReplyFromHostnameActorState();
		static_cast<RetryGetReplyFromHostnameActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 139 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<NetworkAddress> __when_expr_0 = hostname.resolveWithRetry();
															#line 139 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1721 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 1;
															#line 139 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 0, NetworkAddress >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1726 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress const& address,int loopDepth) 
	{
															#line 140 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 1735 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 141 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 1739 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request);
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1745 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1750 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress && address,int loopDepth) 
	{
															#line 140 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 1759 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 141 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 1763 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request);
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1769 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1774 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(NetworkAddress const& address,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(address, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(NetworkAddress && address,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(address), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state > 0) static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 0;
		static_cast<RetryGetReplyFromHostnameActor*>(this)->ActorCallback< RetryGetReplyFromHostnameActor, 0, NetworkAddress >::remove();

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor, 0, NetworkAddress >*,NetworkAddress const& value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor, 0, NetworkAddress >*,NetworkAddress && value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RetryGetReplyFromHostnameActor, 0, NetworkAddress >*,Error err) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 144 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 1846 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 145 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 146 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 1852 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 148 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_2 = delay(reconnetInterval);
															#line 148 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1858 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
				static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 3;
															#line 148 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 2, Void >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1863 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 152 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(reply.getError(), std::max(0, loopDepth - 1));
															#line 1870 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
		}
		else
		{
															#line 155 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<RetryGetReplyFromHostnameActor*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply.get()); this->~RetryGetReplyFromHostnameActorState(); static_cast<RetryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1877 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<RetryGetReplyFromHostnameActor*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply.get());
			this->~RetryGetReplyFromHostnameActorState();
			static_cast<RetryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 1890 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<REPLY_TYPE(Req)> && __reply,int loopDepth) 
	{
		reply = std::move(__reply);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state > 0) static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 0;
		static_cast<RetryGetReplyFromHostnameActor*>(this)->ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >::remove();

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> const& value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> && value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*,Error err) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 149 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reconnetInterval = std::min(2 * reconnetInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 150 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		INetworkConnections::net()->removeCachedDNS(hostname.host, hostname.service);
															#line 1971 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 149 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reconnetInterval = std::min(2 * reconnetInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 150 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		INetworkConnections::net()->removeCachedDNS(hostname.host, hostname.service);
															#line 1982 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state > 0) static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 0;
		static_cast<RetryGetReplyFromHostnameActor*>(this)->ActorCallback< RetryGetReplyFromHostnameActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RetryGetReplyFromHostnameActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Req request;
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 136 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	double reconnetInterval;
															#line 137 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::unique_ptr<RequestStream<Req>> to;
															#line 143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 2062 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActor final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryGetReplyFromHostnameActor<Req>, 0, NetworkAddress >, public ActorCallback< RetryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public ActorCallback< RetryGetReplyFromHostnameActor<Req>, 2, Void >, public FastAllocated<RetryGetReplyFromHostnameActor<Req>>, public RetryGetReplyFromHostnameActorState<Req, RetryGetReplyFromHostnameActor<Req>> {
															#line 2069 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<RetryGetReplyFromHostnameActor<Req>>::operator new;
	using FastAllocated<RetryGetReplyFromHostnameActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Req)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RetryGetReplyFromHostnameActor<Req>, 0, NetworkAddress >;
friend struct ActorCallback< RetryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >;
friend struct ActorCallback< RetryGetReplyFromHostnameActor<Req>, 2, Void >;
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActor(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 2082 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<REPLY_TYPE(Req)>(),
		   RetryGetReplyFromHostnameActorState<Req, RetryGetReplyFromHostnameActor<Req>>(request, hostname, token)
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("retryGetReplyFromHostname");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RetryGetReplyFromHostnameActor<Req>, 0, NetworkAddress >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RetryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RetryGetReplyFromHostnameActor<Req>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token ) {
															#line 131 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryGetReplyFromHostnameActor<Req>(request, hostname, token));
															#line 2114 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 159 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 2119 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req, class RetryGetReplyFromHostnameActor1>
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActor1State {
															#line 2126 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActor1State(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   token(token),
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   taskID(taskID),
															#line 168 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   reconnetInterval(FLOW_KNOBS->HOSTNAME_RECONNECT_INIT_INTERVAL),
															#line 169 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   to()
															#line 2143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	~RetryGetReplyFromHostnameActor1State() 
	{
		fdb_probe_actor_destroy("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 170 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 2158 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~RetryGetReplyFromHostnameActor1State();
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 171 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<NetworkAddress> __when_expr_0 = hostname.resolveWithRetry();
															#line 171 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2190 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 1;
															#line 171 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 0, NetworkAddress >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2195 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress const& address,int loopDepth) 
	{
															#line 172 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 2204 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 173 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 2208 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request, taskID);
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2214 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2219 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress && address,int loopDepth) 
	{
															#line 172 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 2228 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 173 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 2232 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request, taskID);
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2238 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2243 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(NetworkAddress const& address,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(address, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(NetworkAddress && address,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(address), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state > 0) static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 0;
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->ActorCallback< RetryGetReplyFromHostnameActor1, 0, NetworkAddress >::remove();

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor1, 0, NetworkAddress >*,NetworkAddress const& value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor1, 0, NetworkAddress >*,NetworkAddress && value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RetryGetReplyFromHostnameActor1, 0, NetworkAddress >*,Error err) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 176 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 2315 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 177 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 178 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 2321 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 180 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_2 = delay(reconnetInterval);
															#line 180 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
				static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 3;
															#line 180 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 2, Void >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2332 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 184 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(reply.getError(), std::max(0, loopDepth - 1));
															#line 2339 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
		}
		else
		{
															#line 187 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (!static_cast<RetryGetReplyFromHostnameActor1*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply.get()); this->~RetryGetReplyFromHostnameActor1State(); static_cast<RetryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 2346 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<RetryGetReplyFromHostnameActor1*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply.get());
			this->~RetryGetReplyFromHostnameActor1State();
			static_cast<RetryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 2359 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<REPLY_TYPE(Req)> && __reply,int loopDepth) 
	{
		reply = std::move(__reply);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state > 0) static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 0;
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >::remove();

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> const& value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*,ErrorOr<REPLY_TYPE(Req)> && value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*,Error err) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 181 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reconnetInterval = std::min(2 * reconnetInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 182 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		INetworkConnections::net()->removeCachedDNS(hostname.host, hostname.service);
															#line 2440 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 181 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		reconnetInterval = std::min(2 * reconnetInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 182 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		INetworkConnections::net()->removeCachedDNS(hostname.host, hostname.service);
															#line 2451 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state > 0) static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 0;
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->ActorCallback< RetryGetReplyFromHostnameActor1, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor1, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RetryGetReplyFromHostnameActor1, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RetryGetReplyFromHostnameActor1, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Req request;
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TaskPriority taskID;
															#line 168 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	double reconnetInterval;
															#line 169 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::unique_ptr<RequestStream<Req>> to;
															#line 175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 2533 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActor1 final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 0, NetworkAddress >, public ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 2, Void >, public FastAllocated<RetryGetReplyFromHostnameActor1<Req>>, public RetryGetReplyFromHostnameActor1State<Req, RetryGetReplyFromHostnameActor1<Req>> {
															#line 2540 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<RetryGetReplyFromHostnameActor1<Req>>::operator new;
	using FastAllocated<RetryGetReplyFromHostnameActor1<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Req)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 0, NetworkAddress >;
friend struct ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >;
friend struct ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 2, Void >;
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActor1(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 2553 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<REPLY_TYPE(Req)>(),
		   RetryGetReplyFromHostnameActor1State<Req, RetryGetReplyFromHostnameActor1<Req>>(request, hostname, token, taskID)
	{
		fdb_probe_actor_enter("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("retryGetReplyFromHostname");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("retryGetReplyFromHostname", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 0, NetworkAddress >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token, TaskPriority const& taskID ) {
															#line 160 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryGetReplyFromHostnameActor1<Req>(request, hostname, token, taskID));
															#line 2585 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 191 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 2590 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via timeoutWarning()
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class TimeoutWarningActor>
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TimeoutWarningActorState {
															#line 2597 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TimeoutWarningActorState(Future<T> const& what,double const& time,PromiseStream<Void> const& output) 
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : what(what),
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   time(time),
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 194 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   end(delay(time))
															#line 2610 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("timeoutWarning", reinterpret_cast<unsigned long>(this));

	}
	~TimeoutWarningActorState() 
	{
		fdb_probe_actor_destroy("timeoutWarning", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 195 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 2625 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~TimeoutWarningActorState();
		static_cast<TimeoutWarningActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 196 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<T> __when_expr_0 = what;
															#line 195 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (static_cast<TimeoutWarningActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2657 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 199 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = end;
															#line 2661 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<TimeoutWarningActor*>(this)->actor_wait_state = 1;
															#line 196 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeoutWarningActor, 0, T >*>(static_cast<TimeoutWarningActor*>(this)));
															#line 199 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutWarningActor, 1, Void >*>(static_cast<TimeoutWarningActor*>(this)));
															#line 2668 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& t,int loopDepth) 
	{
															#line 197 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<TimeoutWarningActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutWarningActorState(); static_cast<TimeoutWarningActor*>(this)->destroy(); return 0; }
															#line 2683 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TimeoutWarningActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutWarningActorState();
		static_cast<TimeoutWarningActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(T && t,int loopDepth) 
	{
															#line 197 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<TimeoutWarningActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutWarningActorState(); static_cast<TimeoutWarningActor*>(this)->destroy(); return 0; }
															#line 2695 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TimeoutWarningActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutWarningActorState();
		static_cast<TimeoutWarningActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 200 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(Void());
															#line 201 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		end = delay(time);
															#line 2709 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 200 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(Void());
															#line 201 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		end = delay(time);
															#line 2720 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeoutWarningActor*>(this)->actor_wait_state > 0) static_cast<TimeoutWarningActor*>(this)->actor_wait_state = 0;
		static_cast<TimeoutWarningActor*>(this)->ActorCallback< TimeoutWarningActor, 0, T >::remove();
		static_cast<TimeoutWarningActor*>(this)->ActorCallback< TimeoutWarningActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeoutWarningActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutWarningActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeoutWarningActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutWarningActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeoutWarningActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeoutWarningActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> what;
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	double time;
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	PromiseStream<Void> output;
															#line 194 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<Void> end;
															#line 2830 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via timeoutWarning()
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class TimeoutWarningActor final : public Actor<T>, public ActorCallback< TimeoutWarningActor<T>, 0, T >, public ActorCallback< TimeoutWarningActor<T>, 1, Void >, public FastAllocated<TimeoutWarningActor<T>>, public TimeoutWarningActorState<T, TimeoutWarningActor<T>> {
															#line 2837 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TimeoutWarningActor<T>>::operator new;
	using FastAllocated<TimeoutWarningActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeoutWarningActor<T>, 0, T >;
friend struct ActorCallback< TimeoutWarningActor<T>, 1, Void >;
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	TimeoutWarningActor(Future<T> const& what,double const& time,PromiseStream<Void> const& output) 
															#line 2849 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<T>(),
		   TimeoutWarningActorState<T, TimeoutWarningActor<T>>(what, time, output)
	{
		fdb_probe_actor_enter("timeoutWarning", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeoutWarning");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeoutWarning", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeoutWarningActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<T> timeoutWarning( Future<T> const& what, double const& time, PromiseStream<Void> const& output ) {
															#line 192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<T>(new TimeoutWarningActor<T>(what, time, output));
															#line 2879 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 205 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 2884 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardPromise()
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class ForwardPromiseActor>
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ForwardPromiseActorState {
															#line 2891 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ForwardPromiseActorState(Promise<T> const& output,Future<T> const& input) 
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : output(output),
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   input(input)
															#line 2900 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardPromise", reinterpret_cast<unsigned long>(this));

	}
	~ForwardPromiseActorState() 
	{
		fdb_probe_actor_destroy("forwardPromise", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 209 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 209 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 2918 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				static_cast<ForwardPromiseActor*>(this)->actor_wait_state = 1;
															#line 209 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardPromiseActor, 0, T >*>(static_cast<ForwardPromiseActor*>(this)));
															#line 2922 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		delete static_cast<ForwardPromiseActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& err,int loopDepth=0) 
	{
		try {
															#line 212 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output.sendError(err);
															#line 2957 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& value,int loopDepth) 
	{
															#line 210 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 2972 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 210 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 2981 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont2(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ForwardPromiseActor*>(this)->actor_wait_state > 0) static_cast<ForwardPromiseActor*>(this)->actor_wait_state = 0;
		static_cast<ForwardPromiseActor*>(this)->ActorCallback< ForwardPromiseActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardPromiseActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardPromiseActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardPromiseActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
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
	int a_body1cont5(int loopDepth) 
	{
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		delete static_cast<ForwardPromiseActor*>(this);
															#line 3066 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Promise<T> output;
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 3075 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardPromise()
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor final : public Actor<void>, public ActorCallback< ForwardPromiseActor<T>, 0, T >, public FastAllocated<ForwardPromiseActor<T>>, public ForwardPromiseActorState<T, ForwardPromiseActor<T>> {
															#line 3082 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardPromiseActor<T>>::operator new;
	using FastAllocated<ForwardPromiseActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardPromiseActor<T>, 0, T >;
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor(Promise<T> const& output,Future<T> const& input) 
															#line 3093 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<void>(),
		   ForwardPromiseActorState<T, ForwardPromiseActor<T>>(output, input)
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardPromise");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
void forwardPromise( Promise<T> const& output, Future<T> const& input ) {
															#line 206 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	new ForwardPromiseActor<T>(output, input);
															#line 3114 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 215 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 3119 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardPromise()
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class ForwardPromiseActor1>
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor1State {
															#line 3126 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor1State(ReplyPromise<T> const& output,Future<T> const& input) 
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : output(output),
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   input(input)
															#line 3135 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardPromise", reinterpret_cast<unsigned long>(this));

	}
	~ForwardPromiseActor1State() 
	{
		fdb_probe_actor_destroy("forwardPromise", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 219 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 219 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 3153 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				static_cast<ForwardPromiseActor1*>(this)->actor_wait_state = 1;
															#line 219 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardPromiseActor1, 0, T >*>(static_cast<ForwardPromiseActor1*>(this)));
															#line 3157 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		delete static_cast<ForwardPromiseActor1*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& err,int loopDepth=0) 
	{
		try {
															#line 222 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output.sendError(err);
															#line 3192 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& value,int loopDepth) 
	{
															#line 220 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3207 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 220 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont2(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ForwardPromiseActor1*>(this)->actor_wait_state > 0) static_cast<ForwardPromiseActor1*>(this)->actor_wait_state = 0;
		static_cast<ForwardPromiseActor1*>(this)->ActorCallback< ForwardPromiseActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardPromiseActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardPromiseActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardPromiseActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
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
	int a_body1cont5(int loopDepth) 
	{
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		delete static_cast<ForwardPromiseActor1*>(this);
															#line 3301 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReplyPromise<T> output;
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 3310 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardPromise()
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor1 final : public Actor<void>, public ActorCallback< ForwardPromiseActor1<T>, 0, T >, public FastAllocated<ForwardPromiseActor1<T>>, public ForwardPromiseActor1State<T, ForwardPromiseActor1<T>> {
															#line 3317 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardPromiseActor1<T>>::operator new;
	using FastAllocated<ForwardPromiseActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardPromiseActor1<T>, 0, T >;
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor1(ReplyPromise<T> const& output,Future<T> const& input) 
															#line 3328 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<void>(),
		   ForwardPromiseActor1State<T, ForwardPromiseActor1<T>>(output, input)
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardPromise");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
void forwardPromise( ReplyPromise<T> const& output, Future<T> const& input ) {
															#line 216 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	new ForwardPromiseActor1<T>(output, input);
															#line 3349 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 225 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 3354 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardPromise()
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class ForwardPromiseActor2>
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor2State {
															#line 3361 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor2State(PromiseStream<T> const& output,Future<T> const& input) 
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : output(output),
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   input(input)
															#line 3370 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardPromise", reinterpret_cast<unsigned long>(this));

	}
	~ForwardPromiseActor2State() 
	{
		fdb_probe_actor_destroy("forwardPromise", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 229 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 229 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 3388 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				static_cast<ForwardPromiseActor2*>(this)->actor_wait_state = 1;
															#line 229 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardPromiseActor2, 0, T >*>(static_cast<ForwardPromiseActor2*>(this)));
															#line 3392 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		delete static_cast<ForwardPromiseActor2*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 232 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output.sendError(e);
															#line 3427 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& value,int loopDepth) 
	{
															#line 230 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3442 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 230 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3451 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont2(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ForwardPromiseActor2*>(this)->actor_wait_state > 0) static_cast<ForwardPromiseActor2*>(this)->actor_wait_state = 0;
		static_cast<ForwardPromiseActor2*>(this)->ActorCallback< ForwardPromiseActor2, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardPromiseActor2, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardPromiseActor2, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardPromiseActor2, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
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
	int a_body1cont5(int loopDepth) 
	{
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		delete static_cast<ForwardPromiseActor2*>(this);
															#line 3536 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	PromiseStream<T> output;
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 3545 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardPromise()
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor2 final : public Actor<void>, public ActorCallback< ForwardPromiseActor2<T>, 0, T >, public FastAllocated<ForwardPromiseActor2<T>>, public ForwardPromiseActor2State<T, ForwardPromiseActor2<T>> {
															#line 3552 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardPromiseActor2<T>>::operator new;
	using FastAllocated<ForwardPromiseActor2<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardPromiseActor2<T>, 0, T >;
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor2(PromiseStream<T> const& output,Future<T> const& input) 
															#line 3563 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<void>(),
		   ForwardPromiseActor2State<T, ForwardPromiseActor2<T>>(output, input)
	{
		fdb_probe_actor_enter("forwardPromise", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardPromise");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardPromise", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
void forwardPromise( PromiseStream<T> const& output, Future<T> const& input ) {
															#line 226 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	new ForwardPromiseActor2<T>(output, input);
															#line 3584 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 235 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 3589 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via broadcast()
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class BroadcastActor>
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class BroadcastActorState {
															#line 3596 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	BroadcastActorState(Future<T> const& input,std::vector<Promise<T>> const& output) 
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   output(output)
															#line 3605 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("broadcast", reinterpret_cast<unsigned long>(this));

	}
	~BroadcastActorState() 
	{
		fdb_probe_actor_destroy("broadcast", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 238 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 238 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<BroadcastActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3622 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<BroadcastActor*>(this)->actor_wait_state = 1;
															#line 238 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BroadcastActor, 0, T >*>(static_cast<BroadcastActor*>(this)));
															#line 3627 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~BroadcastActorState();
		static_cast<BroadcastActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& value,int loopDepth) 
	{
															#line 239 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 240 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3652 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 241 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActorState(); static_cast<BroadcastActor*>(this)->destroy(); return 0; }
															#line 3656 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<BroadcastActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~BroadcastActorState();
		static_cast<BroadcastActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && value,int loopDepth) 
	{
															#line 239 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 240 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3670 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 241 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActorState(); static_cast<BroadcastActor*>(this)->destroy(); return 0; }
															#line 3674 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<BroadcastActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~BroadcastActorState();
		static_cast<BroadcastActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont1(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<BroadcastActor*>(this)->actor_wait_state > 0) static_cast<BroadcastActor*>(this)->actor_wait_state = 0;
		static_cast<BroadcastActor*>(this)->ActorCallback< BroadcastActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< BroadcastActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< BroadcastActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< BroadcastActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::vector<Promise<T>> output;
															#line 3749 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via broadcast()
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class BroadcastActor final : public Actor<Void>, public ActorCallback< BroadcastActor<T>, 0, T >, public FastAllocated<BroadcastActor<T>>, public BroadcastActorState<T, BroadcastActor<T>> {
															#line 3756 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<BroadcastActor<T>>::operator new;
	using FastAllocated<BroadcastActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BroadcastActor<T>, 0, T >;
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	BroadcastActor(Future<T> const& input,std::vector<Promise<T>> const& output) 
															#line 3767 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<Void>(),
		   BroadcastActorState<T, BroadcastActor<T>>(input, output)
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("broadcast");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< BroadcastActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> broadcast( Future<T> const& input, std::vector<Promise<T>> const& output ) {
															#line 236 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<Void>(new BroadcastActor<T>(input, output));
															#line 3797 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 243 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 3802 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via broadcast()
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class BroadcastActor1>
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class BroadcastActor1State {
															#line 3809 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	BroadcastActor1State(Future<T> const& input,std::vector<ReplyPromise<T>> const& output) 
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   output(output)
															#line 3818 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("broadcast", reinterpret_cast<unsigned long>(this));

	}
	~BroadcastActor1State() 
	{
		fdb_probe_actor_destroy("broadcast", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 246 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 246 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<BroadcastActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3835 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<BroadcastActor1*>(this)->actor_wait_state = 1;
															#line 246 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BroadcastActor1, 0, T >*>(static_cast<BroadcastActor1*>(this)));
															#line 3840 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~BroadcastActor1State();
		static_cast<BroadcastActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& value,int loopDepth) 
	{
															#line 247 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 248 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3865 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 249 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActor1State(); static_cast<BroadcastActor1*>(this)->destroy(); return 0; }
															#line 3869 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<BroadcastActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~BroadcastActor1State();
		static_cast<BroadcastActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && value,int loopDepth) 
	{
															#line 247 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 248 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3883 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
															#line 249 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActor1State(); static_cast<BroadcastActor1*>(this)->destroy(); return 0; }
															#line 3887 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<BroadcastActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~BroadcastActor1State();
		static_cast<BroadcastActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont1(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<BroadcastActor1*>(this)->actor_wait_state > 0) static_cast<BroadcastActor1*>(this)->actor_wait_state = 0;
		static_cast<BroadcastActor1*>(this)->ActorCallback< BroadcastActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< BroadcastActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< BroadcastActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< BroadcastActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::vector<ReplyPromise<T>> output;
															#line 3962 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via broadcast()
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class BroadcastActor1 final : public Actor<Void>, public ActorCallback< BroadcastActor1<T>, 0, T >, public FastAllocated<BroadcastActor1<T>>, public BroadcastActor1State<T, BroadcastActor1<T>> {
															#line 3969 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<BroadcastActor1<T>>::operator new;
	using FastAllocated<BroadcastActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BroadcastActor1<T>, 0, T >;
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	BroadcastActor1(Future<T> const& input,std::vector<ReplyPromise<T>> const& output) 
															#line 3980 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<Void>(),
		   BroadcastActor1State<T, BroadcastActor1<T>>(input, output)
	{
		fdb_probe_actor_enter("broadcast", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("broadcast");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("broadcast", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< BroadcastActor1<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> broadcast( Future<T> const& input, std::vector<ReplyPromise<T>> const& output ) {
															#line 244 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<Void>(new BroadcastActor1<T>(input, output));
															#line 4010 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 251 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 4015 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via incrementalBroadcast()
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class IncrementalBroadcastActor>
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActorState {
															#line 4022 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActorState(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   batchSize(batchSize)
															#line 4033 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("incrementalBroadcast", reinterpret_cast<unsigned long>(this));

	}
	~IncrementalBroadcastActorState() 
	{
		fdb_probe_actor_destroy("incrementalBroadcast", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 254 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 254 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4050 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state = 1;
															#line 254 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor, 0, T >*>(static_cast<IncrementalBroadcastActor*>(this)));
															#line 4055 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~IncrementalBroadcastActorState();
		static_cast<IncrementalBroadcastActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 255 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		i = 0;
															#line 256 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		;
															#line 4080 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& __value,int loopDepth) 
	{
															#line 254 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		value = __value;
															#line 4089 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && __value,int loopDepth) 
	{
		value = std::move(__value);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastActor*>(this)->ActorCallback< IncrementalBroadcastActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 262 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<IncrementalBroadcastActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalBroadcastActorState(); static_cast<IncrementalBroadcastActor*>(this)->destroy(); return 0; }
															#line 4156 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<IncrementalBroadcastActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncrementalBroadcastActorState();
		static_cast<IncrementalBroadcastActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 256 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 4175 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 257 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output[i].send(value);
															#line 258 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 4183 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 259 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0);
															#line 259 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4189 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state = 2;
															#line 259 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor, 1, Void >*>(static_cast<IncrementalBroadcastActor*>(this)));
															#line 4194 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 256 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		i++;
															#line 4221 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastActor*>(this)->ActorCallback< IncrementalBroadcastActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::vector<Promise<T>> output;
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	int batchSize;
															#line 254 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	T value;
															#line 255 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	int i;
															#line 4311 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via incrementalBroadcast()
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActor final : public Actor<Void>, public ActorCallback< IncrementalBroadcastActor<T>, 0, T >, public ActorCallback< IncrementalBroadcastActor<T>, 1, Void >, public FastAllocated<IncrementalBroadcastActor<T>>, public IncrementalBroadcastActorState<T, IncrementalBroadcastActor<T>> {
															#line 4318 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<IncrementalBroadcastActor<T>>::operator new;
	using FastAllocated<IncrementalBroadcastActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncrementalBroadcastActor<T>, 0, T >;
friend struct ActorCallback< IncrementalBroadcastActor<T>, 1, Void >;
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActor(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 4330 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<Void>(),
		   IncrementalBroadcastActorState<T, IncrementalBroadcastActor<T>>(input, output, batchSize)
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("incrementalBroadcast");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IncrementalBroadcastActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IncrementalBroadcastActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> incrementalBroadcast( Future<T> const& input, std::vector<Promise<T>> const& output, int const& batchSize ) {
															#line 252 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<Void>(new IncrementalBroadcastActor<T>(input, output, batchSize));
															#line 4361 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 264 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 4366 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via incrementalBroadcast()
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class IncrementalBroadcastActor1>
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActor1State {
															#line 4373 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActor1State(Future<T> const& input,std::vector<ReplyPromise<T>> const& output,int const& batchSize) 
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   batchSize(batchSize)
															#line 4384 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("incrementalBroadcast", reinterpret_cast<unsigned long>(this));

	}
	~IncrementalBroadcastActor1State() 
	{
		fdb_probe_actor_destroy("incrementalBroadcast", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 267 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 267 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4401 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state = 1;
															#line 267 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor1, 0, T >*>(static_cast<IncrementalBroadcastActor1*>(this)));
															#line 4406 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~IncrementalBroadcastActor1State();
		static_cast<IncrementalBroadcastActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 268 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		i = 0;
															#line 269 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		;
															#line 4431 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& __value,int loopDepth) 
	{
															#line 267 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		value = __value;
															#line 4440 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && __value,int loopDepth) 
	{
		value = std::move(__value);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastActor1*>(this)->ActorCallback< IncrementalBroadcastActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 275 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<IncrementalBroadcastActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalBroadcastActor1State(); static_cast<IncrementalBroadcastActor1*>(this)->destroy(); return 0; }
															#line 4507 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<IncrementalBroadcastActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncrementalBroadcastActor1State();
		static_cast<IncrementalBroadcastActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 269 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 4526 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 270 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output[i].send(value);
															#line 271 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 4534 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 272 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0);
															#line 272 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4540 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state = 2;
															#line 272 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor1, 1, Void >*>(static_cast<IncrementalBroadcastActor1*>(this)));
															#line 4545 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 269 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		i++;
															#line 4572 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastActor1*>(this)->ActorCallback< IncrementalBroadcastActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::vector<ReplyPromise<T>> output;
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	int batchSize;
															#line 267 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	T value;
															#line 268 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	int i;
															#line 4662 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via incrementalBroadcast()
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActor1 final : public Actor<Void>, public ActorCallback< IncrementalBroadcastActor1<T>, 0, T >, public ActorCallback< IncrementalBroadcastActor1<T>, 1, Void >, public FastAllocated<IncrementalBroadcastActor1<T>>, public IncrementalBroadcastActor1State<T, IncrementalBroadcastActor1<T>> {
															#line 4669 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<IncrementalBroadcastActor1<T>>::operator new;
	using FastAllocated<IncrementalBroadcastActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncrementalBroadcastActor1<T>, 0, T >;
friend struct ActorCallback< IncrementalBroadcastActor1<T>, 1, Void >;
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActor1(Future<T> const& input,std::vector<ReplyPromise<T>> const& output,int const& batchSize) 
															#line 4681 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<Void>(),
		   IncrementalBroadcastActor1State<T, IncrementalBroadcastActor1<T>>(input, output, batchSize)
	{
		fdb_probe_actor_enter("incrementalBroadcast", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("incrementalBroadcast");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("incrementalBroadcast", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IncrementalBroadcastActor1<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IncrementalBroadcastActor1<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> incrementalBroadcast( Future<T> const& input, std::vector<ReplyPromise<T>> const& output, int const& batchSize ) {
															#line 265 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<Void>(new IncrementalBroadcastActor1<T>(input, output, batchSize));
															#line 4712 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 277 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 4717 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via incrementalBroadcastWithError()
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class IncrementalBroadcastWithErrorActor>
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastWithErrorActorState {
															#line 4724 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastWithErrorActorState(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   batchSize(batchSize),
															#line 280 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   i(0)
															#line 4737 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this));

	}
	~IncrementalBroadcastWithErrorActorState() 
	{
		fdb_probe_actor_destroy("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 282 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 282 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4755 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 1;
															#line 282 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastWithErrorActor, 0, T >*>(static_cast<IncrementalBroadcastWithErrorActor*>(this)));
															#line 4760 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~IncrementalBroadcastWithErrorActorState();
		static_cast<IncrementalBroadcastWithErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 301 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<IncrementalBroadcastWithErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalBroadcastWithErrorActorState(); static_cast<IncrementalBroadcastWithErrorActor*>(this)->destroy(); return 0; }
															#line 4789 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<IncrementalBroadcastWithErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncrementalBroadcastWithErrorActorState();
		static_cast<IncrementalBroadcastWithErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& _e,int loopDepth=0) 
	{
		try {
															#line 290 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (_e.code() == error_code_operation_cancelled)
															#line 4802 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 291 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(_e, loopDepth);
															#line 4806 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
															#line 293 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			e = _e;
															#line 294 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 4812 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = a_body1Catch2loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
															#line 283 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		;
															#line 4827 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& __value,int loopDepth) 
	{
															#line 282 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		value = __value;
															#line 4836 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && __value,int loopDepth) 
	{
		value = std::move(__value);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastWithErrorActor*>(this)->ActorCallback< IncrementalBroadcastWithErrorActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastWithErrorActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastWithErrorActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastWithErrorActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1(int loopDepth) 
	{
															#line 283 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 4916 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 284 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output[i].send(value);
															#line 285 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 4924 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 286 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0);
															#line 286 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4930 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 2;
															#line 286 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastWithErrorActor, 1, Void >*>(static_cast<IncrementalBroadcastWithErrorActor*>(this)));
															#line 4935 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2break1(int loopDepth) 
	{
		try {
			return a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(int loopDepth) 
	{
															#line 283 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		i++;
															#line 4962 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastWithErrorActor*>(this)->ActorCallback< IncrementalBroadcastWithErrorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastWithErrorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastWithErrorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastWithErrorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
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
	int a_body1Catch2cont1(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1Catch2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2loopBody1(int loopDepth) 
	{
															#line 294 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 5072 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1Catch2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 295 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		output[i].sendError(e);
															#line 296 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 5080 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		{
															#line 297 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_2 = delay(0);
															#line 297 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5086 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1Catch2loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 3;
															#line 297 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastWithErrorActor, 2, Void >*>(static_cast<IncrementalBroadcastWithErrorActor*>(this)));
															#line 5091 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1Catch2loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2break1(int loopDepth) 
	{
		try {
			return a_body1Catch2cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2loopBody1cont1(int loopDepth) 
	{
															#line 294 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		i++;
															#line 5118 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1Catch2loopHead1(0);

		return loopDepth;
	}
	int a_body1Catch2loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state > 0) static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 0;
		static_cast<IncrementalBroadcastWithErrorActor*>(this)->ActorCallback< IncrementalBroadcastWithErrorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastWithErrorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< IncrementalBroadcastWithErrorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< IncrementalBroadcastWithErrorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	std::vector<Promise<T>> output;
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	int batchSize;
															#line 280 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	int i;
															#line 282 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	T value;
															#line 293 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Error e;
															#line 5210 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via incrementalBroadcastWithError()
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastWithErrorActor final : public Actor<Void>, public ActorCallback< IncrementalBroadcastWithErrorActor<T>, 0, T >, public ActorCallback< IncrementalBroadcastWithErrorActor<T>, 1, Void >, public ActorCallback< IncrementalBroadcastWithErrorActor<T>, 2, Void >, public FastAllocated<IncrementalBroadcastWithErrorActor<T>>, public IncrementalBroadcastWithErrorActorState<T, IncrementalBroadcastWithErrorActor<T>> {
															#line 5217 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<IncrementalBroadcastWithErrorActor<T>>::operator new;
	using FastAllocated<IncrementalBroadcastWithErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncrementalBroadcastWithErrorActor<T>, 0, T >;
friend struct ActorCallback< IncrementalBroadcastWithErrorActor<T>, 1, Void >;
friend struct ActorCallback< IncrementalBroadcastWithErrorActor<T>, 2, Void >;
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastWithErrorActor(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 5230 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<Void>(),
		   IncrementalBroadcastWithErrorActorState<T, IncrementalBroadcastWithErrorActor<T>>(input, output, batchSize)
	{
		fdb_probe_actor_enter("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("incrementalBroadcastWithError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("incrementalBroadcastWithError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IncrementalBroadcastWithErrorActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IncrementalBroadcastWithErrorActor<T>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< IncrementalBroadcastWithErrorActor<T>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> incrementalBroadcastWithError( Future<T> const& input, std::vector<Promise<T>> const& output, int const& batchSize ) {
															#line 278 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<Void>(new IncrementalBroadcastWithErrorActor<T>(input, output, batchSize));
															#line 5262 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 303 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

// Needed for the call to endpointNotFound()
#include "fdbrpc/FailureMonitor.h"

struct PeerHolder {
	Reference<Peer> peer;
	explicit PeerHolder(Reference<Peer> peer) : peer(peer) {
		if (peer) {
			peer->outstandingReplies++;
		}
	}
	~PeerHolder() {
		if (peer) {
			peer->outstandingReplies--;
		}
	}
};

// Implements getReplyStream, this a void actor with the same lifetime as the input ReplyPromiseStream.
// Because this actor holds a reference to the stream, normally it would be impossible to know when there are no other
// references. To get around this, there is a SAV inside the stream that has one less promise reference than it should
// (caused by getErrorFutureAndDelPromiseRef()). When that SAV gets a broken promise because no one besides this void
// actor is referencing it, this void actor will get a broken_promise dropping the final reference to the full
// ReplyPromiseStream
															#line 5290 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via endStreamOnDisconnect()
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X, class EndStreamOnDisconnectActor>
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class EndStreamOnDisconnectActorState {
															#line 5297 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	EndStreamOnDisconnectActorState(Future<Void> const& signal,ReplyPromiseStream<X> const& stream,Endpoint const& endpoint,Reference<Peer> const& peer = Reference<Peer>()) 
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : signal(signal),
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   stream(stream),
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint),
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   peer(peer),
															#line 332 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   holder(PeerHolder(peer))
															#line 5312 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this));

	}
	~EndStreamOnDisconnectActorState() 
	{
		fdb_probe_actor_destroy("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 333 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			stream.setRequestStreamEndpoint(endpoint);
															#line 5327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			try {
															#line 336 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = signal;
															#line 335 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 339 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_1 = peer.isValid() ? peer->disconnect.getFuture() : Never();
															#line 5335 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
															#line 342 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_2 = stream.getErrorFutureAndDelPromiseRef();
															#line 5339 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1when3(__when_expr_2.get(), loopDepth); };
				static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state = 1;
															#line 336 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 0, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 339 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 1, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 342 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 2, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 5348 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		delete static_cast<EndStreamOnDisconnectActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 345 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 5383 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 347 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (!stream.connected())
															#line 5387 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				{
															#line 348 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
					StrictFuture<Void> __when_expr_3 = signal || stream.onConnected();
															#line 348 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
					if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_3.get(), loopDepth); };
															#line 5393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
					static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state = 2;
															#line 348 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
					__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 3, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 5397 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1Catch2cont2(loopDepth);
				}
			}
			else
			{
				loopDepth = a_body1Catch2cont1(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 337 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5428 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 337 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5437 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 340 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5446 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 340 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5455 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state > 0) static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state = 0;
		static_cast<EndStreamOnDisconnectActor*>(this)->ActorCallback< EndStreamOnDisconnectActor, 0, Void >::remove();
		static_cast<EndStreamOnDisconnectActor*>(this)->ActorCallback< EndStreamOnDisconnectActor, 1, Void >::remove();
		static_cast<EndStreamOnDisconnectActor*>(this)->ActorCallback< EndStreamOnDisconnectActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< EndStreamOnDisconnectActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< EndStreamOnDisconnectActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose1();
		try {
			a_body1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose1();
		try {
			a_body1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< EndStreamOnDisconnectActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 2);

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
	int a_body1Catch2cont1(int loopDepth) 
	{
															#line 352 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		stream.notifyFailed();
															#line 5632 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont2(int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont2(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont2(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state > 0) static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state = 0;
		static_cast<EndStreamOnDisconnectActor*>(this)->ActorCallback< EndStreamOnDisconnectActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose2();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< EndStreamOnDisconnectActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose2();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< EndStreamOnDisconnectActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		delete static_cast<EndStreamOnDisconnectActor*>(this);
															#line 5722 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<Void> signal;
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReplyPromiseStream<X> stream;
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Reference<Peer> peer;
															#line 332 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	PeerHolder holder;
															#line 5737 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via endStreamOnDisconnect()
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X>
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class EndStreamOnDisconnectActor final : public Actor<void>, public ActorCallback< EndStreamOnDisconnectActor<X>, 0, Void >, public ActorCallback< EndStreamOnDisconnectActor<X>, 1, Void >, public ActorCallback< EndStreamOnDisconnectActor<X>, 2, Void >, public ActorCallback< EndStreamOnDisconnectActor<X>, 3, Void >, public FastAllocated<EndStreamOnDisconnectActor<X>>, public EndStreamOnDisconnectActorState<X, EndStreamOnDisconnectActor<X>> {
															#line 5744 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<EndStreamOnDisconnectActor<X>>::operator new;
	using FastAllocated<EndStreamOnDisconnectActor<X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< EndStreamOnDisconnectActor<X>, 0, Void >;
friend struct ActorCallback< EndStreamOnDisconnectActor<X>, 1, Void >;
friend struct ActorCallback< EndStreamOnDisconnectActor<X>, 2, Void >;
friend struct ActorCallback< EndStreamOnDisconnectActor<X>, 3, Void >;
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	EndStreamOnDisconnectActor(Future<Void> const& signal,ReplyPromiseStream<X> const& stream,Endpoint const& endpoint,Reference<Peer> const& peer = Reference<Peer>()) 
															#line 5758 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<void>(),
		   EndStreamOnDisconnectActorState<X, EndStreamOnDisconnectActor<X>>(signal, stream, endpoint, peer)
	{
		fdb_probe_actor_enter("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("endStreamOnDisconnect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("endStreamOnDisconnect", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X>
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
void endStreamOnDisconnect( Future<Void> const& signal, ReplyPromiseStream<X> const& stream, Endpoint const& endpoint, Reference<Peer> const& peer = Reference<Peer>() ) {
															#line 327 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	new EndStreamOnDisconnectActor<X>(signal, stream, endpoint, peer);
															#line 5779 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 355 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

// Implements tryGetReply, getReplyUnlessFailedFor
															#line 5785 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via waitValueOrSignal()
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X, class WaitValueOrSignalActor>
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class WaitValueOrSignalActorState {
															#line 5792 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WaitValueOrSignalActorState(Future<X> const& value,Future<Void> const& signal,Endpoint const& endpoint,ReplyPromise<X> const& holdme = ReplyPromise<X>(),Reference<Peer> const& peer = Reference<Peer>()) 
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : value(value),
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   signal(signal),
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint),
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   holdme(holdme),
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   peer(peer),
															#line 363 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   holder(PeerHolder(peer))
															#line 5809 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("waitValueOrSignal", reinterpret_cast<unsigned long>(this));

	}
	~WaitValueOrSignalActorState() 
	{
		fdb_probe_actor_destroy("waitValueOrSignal", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 364 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			;
															#line 5824 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 367 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<X> __when_expr_0 = value;
															#line 366 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (static_cast<WaitValueOrSignalActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5857 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 370 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = signal;
															#line 5861 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<WaitValueOrSignalActor*>(this)->actor_wait_state = 1;
															#line 367 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitValueOrSignalActor, 0, X >*>(static_cast<WaitValueOrSignalActor*>(this)));
															#line 370 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitValueOrSignalActor, 1, Void >*>(static_cast<WaitValueOrSignalActor*>(this)));
															#line 5868 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 375 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (signal.isError())
															#line 5890 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 376 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				TraceEvent(SevError, "WaitValueOrSignalError").error(signal.getError());
															#line 377 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(internal_error())); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5896 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(internal_error()));
				this->~WaitValueOrSignalActorState();
				static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 380 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 5904 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 381 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 5908 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
															#line 385 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() != error_code_broken_promise || signal.isError())
															#line 5912 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 386 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(e)); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5916 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(e));
				this->~WaitValueOrSignalActorState();
				static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 387 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			IFailureMonitor::failureMonitor().endpointNotFound(endpoint);
															#line 388 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			value = Never();
															#line 5926 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(X const& x,int loopDepth) 
	{
															#line 368 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(x); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5941 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(x);
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(X && x,int loopDepth) 
	{
															#line 368 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(x); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5953 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(x);
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 371 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(request_maybe_delivered())); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5965 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(request_maybe_delivered()));
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 371 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(request_maybe_delivered())); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5977 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(request_maybe_delivered()));
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitValueOrSignalActor*>(this)->actor_wait_state > 0) static_cast<WaitValueOrSignalActor*>(this)->actor_wait_state = 0;
		static_cast<WaitValueOrSignalActor*>(this)->ActorCallback< WaitValueOrSignalActor, 0, X >::remove();
		static_cast<WaitValueOrSignalActor*>(this)->ActorCallback< WaitValueOrSignalActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitValueOrSignalActor, 0, X >*,X const& value) 
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitValueOrSignalActor, 0, X >*,X && value) 
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitValueOrSignalActor, 0, X >*,Error err) 
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitValueOrSignalActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitValueOrSignalActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitValueOrSignalActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<X> value;
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<Void> signal;
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReplyPromise<X> holdme;
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Reference<Peer> peer;
															#line 363 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	PeerHolder holder;
															#line 6094 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via waitValueOrSignal()
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X>
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class WaitValueOrSignalActor final : public Actor<ErrorOr<X>>, public ActorCallback< WaitValueOrSignalActor<X>, 0, X >, public ActorCallback< WaitValueOrSignalActor<X>, 1, Void >, public FastAllocated<WaitValueOrSignalActor<X>>, public WaitValueOrSignalActorState<X, WaitValueOrSignalActor<X>> {
															#line 6101 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<WaitValueOrSignalActor<X>>::operator new;
	using FastAllocated<WaitValueOrSignalActor<X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<X>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitValueOrSignalActor<X>, 0, X >;
friend struct ActorCallback< WaitValueOrSignalActor<X>, 1, Void >;
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	WaitValueOrSignalActor(Future<X> const& value,Future<Void> const& signal,Endpoint const& endpoint,ReplyPromise<X> const& holdme = ReplyPromise<X>(),Reference<Peer> const& peer = Reference<Peer>()) 
															#line 6113 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<ErrorOr<X>>(),
		   WaitValueOrSignalActorState<X, WaitValueOrSignalActor<X>>(value, signal, endpoint, holdme, peer)
	{
		fdb_probe_actor_enter("waitValueOrSignal", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitValueOrSignal");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitValueOrSignal", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitValueOrSignalActor<X>, 0, X >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X>
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<X>> waitValueOrSignal( Future<X> const& value, Future<Void> const& signal, Endpoint const& endpoint, ReplyPromise<X> const& holdme = ReplyPromise<X>(), Reference<Peer> const& peer = Reference<Peer>() ) {
															#line 357 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<ErrorOr<X>>(new WaitValueOrSignalActor<X>(value, signal, endpoint, holdme, peer));
															#line 6143 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 392 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 6148 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via sendCanceler()
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T, class SendCancelerActor>
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class SendCancelerActorState {
															#line 6155 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	SendCancelerActorState(ReplyPromise<T> const& reply,ReliablePacket* const& send,Endpoint const& endpoint) 
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : reply(reply),
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   send(send),
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint)
															#line 6166 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("sendCanceler", reinterpret_cast<unsigned long>(this));

	}
	~SendCancelerActorState() 
	{
		fdb_probe_actor_destroy("sendCanceler", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 396 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = reply.getFuture();
															#line 396 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (static_cast<SendCancelerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6184 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<SendCancelerActor*>(this)->actor_wait_state = 1;
															#line 396 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendCancelerActor, 0, T >*>(static_cast<SendCancelerActor*>(this)));
															#line 6189 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~SendCancelerActorState();
		static_cast<SendCancelerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 400 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			FlowTransport::transport().cancelReliable(send);
															#line 401 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 6221 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 402 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				IFailureMonitor::failureMonitor().endpointNotFound(endpoint);
															#line 6225 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
															#line 404 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 6229 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& t,int loopDepth) 
	{
															#line 397 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		FlowTransport::transport().cancelReliable(send);
															#line 398 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<SendCancelerActor*>(this)->SAV<T>::futures) { (void)(t); this->~SendCancelerActorState(); static_cast<SendCancelerActor*>(this)->destroy(); return 0; }
															#line 6245 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<SendCancelerActor*>(this)->SAV< T >::value()) T(t);
		this->~SendCancelerActorState();
		static_cast<SendCancelerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 397 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		FlowTransport::transport().cancelReliable(send);
															#line 398 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<SendCancelerActor*>(this)->SAV<T>::futures) { (void)(t); this->~SendCancelerActorState(); static_cast<SendCancelerActor*>(this)->destroy(); return 0; }
															#line 6259 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<SendCancelerActor*>(this)->SAV< T >::value()) T(t);
		this->~SendCancelerActorState();
		static_cast<SendCancelerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
		loopDepth = a_body1cont2(t, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(t), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SendCancelerActor*>(this)->actor_wait_state > 0) static_cast<SendCancelerActor*>(this)->actor_wait_state = 0;
		static_cast<SendCancelerActor*>(this)->ActorCallback< SendCancelerActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendCancelerActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReplyPromise<T> reply;
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReliablePacket* send;
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 6336 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via sendCanceler()
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class SendCancelerActor final : public Actor<T>, public ActorCallback< SendCancelerActor<T>, 0, T >, public FastAllocated<SendCancelerActor<T>>, public SendCancelerActorState<T, SendCancelerActor<T>> {
															#line 6343 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<SendCancelerActor<T>>::operator new;
	using FastAllocated<SendCancelerActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendCancelerActor<T>, 0, T >;
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	SendCancelerActor(ReplyPromise<T> const& reply,ReliablePacket* const& send,Endpoint const& endpoint) 
															#line 6354 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<T>(),
		   SendCancelerActorState<T, SendCancelerActor<T>>(reply, send, endpoint)
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendCanceler");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendCancelerActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class T>
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<T> sendCanceler( ReplyPromise<T> const& reply, ReliablePacket* const& send, Endpoint const& endpoint ) {
															#line 393 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<T>(new SendCancelerActor<T>(reply, send, endpoint));
															#line 6384 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 407 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

															#line 6389 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via reportEndpointFailure()
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X, class ReportEndpointFailureActor>
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ReportEndpointFailureActorState {
															#line 6396 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReportEndpointFailureActorState(Future<X> const& value,Endpoint const& endpoint) 
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		 : value(value),
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint)
															#line 6405 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("reportEndpointFailure", reinterpret_cast<unsigned long>(this));

	}
	~ReportEndpointFailureActorState() 
	{
		fdb_probe_actor_destroy("reportEndpointFailure", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 411 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				StrictFuture<X> __when_expr_0 = value;
															#line 411 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				if (static_cast<ReportEndpointFailureActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6423 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReportEndpointFailureActor*>(this)->actor_wait_state = 1;
															#line 411 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReportEndpointFailureActor, 0, X >*>(static_cast<ReportEndpointFailureActor*>(this)));
															#line 6428 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
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
		this->~ReportEndpointFailureActorState();
		static_cast<ReportEndpointFailureActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 414 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 6458 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			{
															#line 415 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
				IFailureMonitor::failureMonitor().endpointNotFound(endpoint);
															#line 6462 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
			}
															#line 417 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 6466 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(X const& x,int loopDepth) 
	{
															#line 412 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<ReportEndpointFailureActor*>(this)->SAV<X>::futures) { (void)(x); this->~ReportEndpointFailureActorState(); static_cast<ReportEndpointFailureActor*>(this)->destroy(); return 0; }
															#line 6480 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<ReportEndpointFailureActor*>(this)->SAV< X >::value()) X(x);
		this->~ReportEndpointFailureActorState();
		static_cast<ReportEndpointFailureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(X && x,int loopDepth) 
	{
															#line 412 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
		if (!static_cast<ReportEndpointFailureActor*>(this)->SAV<X>::futures) { (void)(x); this->~ReportEndpointFailureActorState(); static_cast<ReportEndpointFailureActor*>(this)->destroy(); return 0; }
															#line 6492 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<ReportEndpointFailureActor*>(this)->SAV< X >::value()) X(x);
		this->~ReportEndpointFailureActorState();
		static_cast<ReportEndpointFailureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(X const& x,int loopDepth) 
	{
		loopDepth = a_body1cont2(x, loopDepth);

		return loopDepth;
	}
	int a_body1when1(X && x,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(x), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReportEndpointFailureActor*>(this)->actor_wait_state > 0) static_cast<ReportEndpointFailureActor*>(this)->actor_wait_state = 0;
		static_cast<ReportEndpointFailureActor*>(this)->ActorCallback< ReportEndpointFailureActor, 0, X >::remove();

	}
	void a_callback_fire(ActorCallback< ReportEndpointFailureActor, 0, X >*,X const& value) 
	{
		fdb_probe_actor_enter("reportEndpointFailure", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reportEndpointFailure", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReportEndpointFailureActor, 0, X >*,X && value) 
	{
		fdb_probe_actor_enter("reportEndpointFailure", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reportEndpointFailure", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReportEndpointFailureActor, 0, X >*,Error err) 
	{
		fdb_probe_actor_enter("reportEndpointFailure", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reportEndpointFailure", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Future<X> value;
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 6567 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via reportEndpointFailure()
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X>
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
class ReportEndpointFailureActor final : public Actor<X>, public ActorCallback< ReportEndpointFailureActor<X>, 0, X >, public FastAllocated<ReportEndpointFailureActor<X>>, public ReportEndpointFailureActorState<X, ReportEndpointFailureActor<X>> {
															#line 6574 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ReportEndpointFailureActor<X>>::operator new;
	using FastAllocated<ReportEndpointFailureActor<X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<X>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReportEndpointFailureActor<X>, 0, X >;
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	ReportEndpointFailureActor(Future<X> const& value,Endpoint const& endpoint) 
															#line 6585 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
		 : Actor<X>(),
		   ReportEndpointFailureActorState<X, ReportEndpointFailureActor<X>>(value, endpoint)
	{
		fdb_probe_actor_enter("reportEndpointFailure", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("reportEndpointFailure");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("reportEndpointFailure", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReportEndpointFailureActor<X>, 0, X >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
template <class X>
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<X> reportEndpointFailure( Future<X> const& value, Endpoint const& endpoint ) {
															#line 408 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"
	return Future<X>(new ReportEndpointFailureActor<X>(value, endpoint));
															#line 6615 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.g.h"
}

#line 420 "/usr/src/libfdb_c/fdbrpc/genericactors.actor.h"

#include "flow/unactorcompiler.h"

#endif
