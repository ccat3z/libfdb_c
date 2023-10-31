#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
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
#include "fdbrpc/WellKnownEndpoints.h"
#include "flow/Hostname.h"
#include "flow/actorcompiler.h" // This must be the last #include.

// To avoid diretly access INetworkConnection::net()->removeCachedDNS(), which will require heavy include budget, put
// the call to FlowTransport.actor.cpp as a external function.
extern void removeCachedDNS(const std::string& host, const std::string& service);

															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryBrokenPromise()
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, bool P, class RetryBrokenPromiseActor>
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActorState {
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActorState(RequestStream<Req, P> const& to,Req const& request) 
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : to(to),
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   request(request)
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<REPLY_TYPE(Req)> __when_expr_0 = to.getReply(request);
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state = 1;
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor, 0, REPLY_TYPE(Req) >*>(static_cast<RetryBrokenPromiseActor*>(this)));
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() != error_code_broken_promise)
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delayJittered(FLOW_KNOBS->PREVENT_FAST_SPIN_DELAY);
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor*>(this)->actor_wait_state = 2;
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor, 1, Void >*>(static_cast<RetryBrokenPromiseActor*>(this)));
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActorState(); static_cast<RetryBrokenPromiseActor*>(this)->destroy(); return 0; }
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<RetryBrokenPromiseActor*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply);
		this->~RetryBrokenPromiseActorState();
		static_cast<RetryBrokenPromiseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(REPLY_TYPE(Req) && reply,int loopDepth) 
	{
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActorState(); static_cast<RetryBrokenPromiseActor*>(this)->destroy(); return 0; }
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		CODE_PROBE(true, "retryBrokenPromise");
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		CODE_PROBE(true, "retryBrokenPromise");
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RequestStream<Req, P> to;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Req request;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryBrokenPromise()
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, bool P>
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActor final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryBrokenPromiseActor<Req, P>, 0, REPLY_TYPE(Req) >, public ActorCallback< RetryBrokenPromiseActor<Req, P>, 1, Void >, public FastAllocated<RetryBrokenPromiseActor<Req, P>>, public RetryBrokenPromiseActorState<Req, P, RetryBrokenPromiseActor<Req, P>> {
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<RetryBrokenPromiseActor<Req, P>>::operator new;
	using FastAllocated<RetryBrokenPromiseActor<Req, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Req)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RetryBrokenPromiseActor<Req, P>, 0, REPLY_TYPE(Req) >;
friend struct ActorCallback< RetryBrokenPromiseActor<Req, P>, 1, Void >;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActor(RequestStream<Req, P> const& to,Req const& request) 
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		 : Actor<REPLY_TYPE(Req)>(),
		   RetryBrokenPromiseActorState<Req, P, RetryBrokenPromiseActor<Req, P>>(to, request)
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
		case 1: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor<Req, P>, 0, REPLY_TYPE(Req) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor<Req, P>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, bool P>
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryBrokenPromise( RequestStream<Req, P> const& to, Req const& request ) {
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryBrokenPromiseActor<Req, P>(to, request));
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryBrokenPromise()
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, bool P, class RetryBrokenPromiseActor1>
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActor1State {
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActor1State(RequestStream<Req, P> const& to,Req const& request,TaskPriority const& taskID) 
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : to(to),
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   request(request),
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   taskID(taskID)
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<REPLY_TYPE(Req)> __when_expr_0 = to.getReply(request, taskID);
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state = 1;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor1, 0, REPLY_TYPE(Req) >*>(static_cast<RetryBrokenPromiseActor1*>(this)));
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() != error_code_broken_promise)
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delayJittered(FLOW_KNOBS->PREVENT_FAST_SPIN_DELAY, taskID);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RetryBrokenPromiseActor1*>(this)->actor_wait_state = 2;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryBrokenPromiseActor1, 1, Void >*>(static_cast<RetryBrokenPromiseActor1*>(this)));
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor1*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActor1State(); static_cast<RetryBrokenPromiseActor1*>(this)->destroy(); return 0; }
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<RetryBrokenPromiseActor1*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply);
		this->~RetryBrokenPromiseActor1State();
		static_cast<RetryBrokenPromiseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(REPLY_TYPE(Req) && reply,int loopDepth) 
	{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<RetryBrokenPromiseActor1*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply); this->~RetryBrokenPromiseActor1State(); static_cast<RetryBrokenPromiseActor1*>(this)->destroy(); return 0; }
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		CODE_PROBE(true, "retryBrokenPromise with taskID");
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		CODE_PROBE(true, "retryBrokenPromise with taskID");
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RequestStream<Req, P> to;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Req request;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TaskPriority taskID;
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryBrokenPromise()
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, bool P>
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryBrokenPromiseActor1 final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryBrokenPromiseActor1<Req, P>, 0, REPLY_TYPE(Req) >, public ActorCallback< RetryBrokenPromiseActor1<Req, P>, 1, Void >, public FastAllocated<RetryBrokenPromiseActor1<Req, P>>, public RetryBrokenPromiseActor1State<Req, P, RetryBrokenPromiseActor1<Req, P>> {
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<RetryBrokenPromiseActor1<Req, P>>::operator new;
	using FastAllocated<RetryBrokenPromiseActor1<Req, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Req)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RetryBrokenPromiseActor1<Req, P>, 0, REPLY_TYPE(Req) >;
friend struct ActorCallback< RetryBrokenPromiseActor1<Req, P>, 1, Void >;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryBrokenPromiseActor1(RequestStream<Req, P> const& to,Req const& request,TaskPriority const& taskID) 
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		 : Actor<REPLY_TYPE(Req)>(),
		   RetryBrokenPromiseActor1State<Req, P, RetryBrokenPromiseActor1<Req, P>>(to, request, taskID)
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
		case 1: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor1<Req, P>, 0, REPLY_TYPE(Req) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RetryBrokenPromiseActor1<Req, P>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, bool P>
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryBrokenPromise( RequestStream<Req, P> const& to, Req const& request, TaskPriority const& taskID ) {
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryBrokenPromiseActor1<Req, P>(to, request, taskID));
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tryInitializeRequestStream()
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, class TryInitializeRequestStreamActor>
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TryInitializeRequestStreamActorState {
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TryInitializeRequestStreamActorState(RequestStream<Req>* const& stream,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : stream(stream),
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   token(token)
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hostname.resolve();
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<TryInitializeRequestStreamActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryInitializeRequestStreamActor*>(this)->actor_wait_state = 1;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryInitializeRequestStreamActor, 0, Optional<NetworkAddress> >*>(static_cast<TryInitializeRequestStreamActor*>(this)));
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TryInitializeRequestStreamActorState();
			static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		ASSERT(stream != nullptr);
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		*stream = RequestStream<Req>(Endpoint::wellKnown({ address.get() }, token));
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TryInitializeRequestStreamActorState();
		static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryInitializeRequestStreamActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TryInitializeRequestStreamActorState();
			static_cast<TryInitializeRequestStreamActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		ASSERT(stream != nullptr);
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		*stream = RequestStream<Req>(Endpoint::wellKnown({ address.get() }, token));
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryInitializeRequestStreamActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TryInitializeRequestStreamActorState(); static_cast<TryInitializeRequestStreamActor*>(this)->destroy(); return 0; }
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RequestStream<Req>* stream;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via tryInitializeRequestStream()
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TryInitializeRequestStreamActor final : public Actor<Void>, public ActorCallback< TryInitializeRequestStreamActor<Req>, 0, Optional<NetworkAddress> >, public FastAllocated<TryInitializeRequestStreamActor<Req>>, public TryInitializeRequestStreamActorState<Req, TryInitializeRequestStreamActor<Req>> {
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TryInitializeRequestStreamActor<Req>>::operator new;
	using FastAllocated<TryInitializeRequestStreamActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryInitializeRequestStreamActor<Req>, 0, Optional<NetworkAddress> >;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TryInitializeRequestStreamActor(RequestStream<Req>* const& stream,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> tryInitializeRequestStream( RequestStream<Req>* const& stream, Hostname const& hostname, WellKnownEndpoints const& token ) {
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<Void>(new TryInitializeRequestStreamActor<Req>(stream, hostname, token));
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, class TryGetReplyFromHostnameActor>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActorState {
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActorState(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   token(token)
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hostname.resolve();
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 1;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor, 0, Optional<NetworkAddress> >*>(static_cast<TryGetReplyFromHostnameActor*>(this)));
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActorState(); static_cast<TryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActorState();
			static_cast<TryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor*>(this)));
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActorState(); static_cast<TryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActorState();
			static_cast<TryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor*>(this)));
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				removeCachedDNS(hostname.host, hostname.service);
															#line 1170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
		}
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryGetReplyFromHostnameActor*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(reply); this->~TryGetReplyFromHostnameActorState(); static_cast<TryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryGetReplyFromHostnameActor*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(std::move(reply)); // state_var_RVO
		this->~TryGetReplyFromHostnameActorState();
		static_cast<TryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Req request;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 1258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActor final : public Actor<ErrorOr<REPLY_TYPE(Req)>>, public ActorCallback< TryGetReplyFromHostnameActor<Req>, 0, Optional<NetworkAddress> >, public ActorCallback< TryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public FastAllocated<TryGetReplyFromHostnameActor<Req>>, public TryGetReplyFromHostnameActorState<Req, TryGetReplyFromHostnameActor<Req>> {
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TryGetReplyFromHostnameActor<Req>>::operator new;
	using FastAllocated<TryGetReplyFromHostnameActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<REPLY_TYPE(Req)>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetReplyFromHostnameActor<Req>, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< TryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActor(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<REPLY_TYPE(Req)>> tryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token ) {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<ErrorOr<REPLY_TYPE(Req)>>(new TryGetReplyFromHostnameActor<Req>(request, hostname, token));
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, class TryGetReplyFromHostnameActor1>
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActor1State {
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActor1State(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   token(token),
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   taskID(taskID)
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hostname.resolve();
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 1;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor1, 0, Optional<NetworkAddress> >*>(static_cast<TryGetReplyFromHostnameActor1*>(this)));
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActor1State(); static_cast<TryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 1382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActor1State();
			static_cast<TryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request, taskID);
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor1*>(this)));
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!address.present())
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(ErrorOr<REPLY_TYPE(Req)>(lookup_failed())); this->~TryGetReplyFromHostnameActor1State(); static_cast<TryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(ErrorOr<REPLY_TYPE(Req)>(lookup_failed()));
			this->~TryGetReplyFromHostnameActor1State();
			static_cast<TryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		RequestStream<Req> to(Endpoint::wellKnown({ address.get() }, token));
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to.tryGetReply(request, taskID);
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<TryGetReplyFromHostnameActor1*>(this)));
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				removeCachedDNS(hostname.host, hostname.service);
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
		}
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV<ErrorOr<REPLY_TYPE(Req)>>::futures) { (void)(reply); this->~TryGetReplyFromHostnameActor1State(); static_cast<TryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TryGetReplyFromHostnameActor1*>(this)->SAV< ErrorOr<REPLY_TYPE(Req)> >::value()) ErrorOr<REPLY_TYPE(Req)>(std::move(reply)); // state_var_RVO
		this->~TryGetReplyFromHostnameActor1State();
		static_cast<TryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Req request;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TaskPriority taskID;
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via tryGetReplyFromHostname()
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TryGetReplyFromHostnameActor1 final : public Actor<ErrorOr<REPLY_TYPE(Req)>>, public ActorCallback< TryGetReplyFromHostnameActor1<Req>, 0, Optional<NetworkAddress> >, public ActorCallback< TryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public FastAllocated<TryGetReplyFromHostnameActor1<Req>>, public TryGetReplyFromHostnameActor1State<Req, TryGetReplyFromHostnameActor1<Req>> {
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TryGetReplyFromHostnameActor1<Req>>::operator new;
	using FastAllocated<TryGetReplyFromHostnameActor1<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<REPLY_TYPE(Req)>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetReplyFromHostnameActor1<Req>, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< TryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TryGetReplyFromHostnameActor1(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 1620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<REPLY_TYPE(Req)>> tryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token, TaskPriority const& taskID ) {
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<ErrorOr<REPLY_TYPE(Req)>>(new TryGetReplyFromHostnameActor1<Req>(request, hostname, token, taskID));
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 1656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, class RetryGetReplyFromHostnameActor>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActorState {
															#line 1663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActorState(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   token(token),
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   reconnectInterval(FLOW_KNOBS->HOSTNAME_RECONNECT_INIT_INTERVAL),
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   to()
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<NetworkAddress> __when_expr_0 = hostname.resolveWithRetry();
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 1;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 0, NetworkAddress >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress const& address,int loopDepth) 
	{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 1739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 1743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress && address,int loopDepth) 
	{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 1763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 1767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 2;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_2 = delay(reconnectInterval);
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
				static_cast<RetryGetReplyFromHostnameActor*>(this)->actor_wait_state = 3;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor, 2, Void >*>(static_cast<RetryGetReplyFromHostnameActor*>(this)));
															#line 1867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(reply.getError(), std::max(0, loopDepth - 1));
															#line 1874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
		}
		else
		{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<RetryGetReplyFromHostnameActor*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply.get()); this->~RetryGetReplyFromHostnameActorState(); static_cast<RetryGetReplyFromHostnameActor*>(this)->destroy(); return 0; }
															#line 1881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<RetryGetReplyFromHostnameActor*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply.get());
			this->~RetryGetReplyFromHostnameActorState();
			static_cast<RetryGetReplyFromHostnameActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 1894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reconnectInterval = std::min(2 * reconnectInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		removeCachedDNS(hostname.host, hostname.service);
															#line 1975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reconnectInterval = std::min(2 * reconnectInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		removeCachedDNS(hostname.host, hostname.service);
															#line 1986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Req request;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	double reconnectInterval;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::unique_ptr<RequestStream<Req>> to;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 2066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActor final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryGetReplyFromHostnameActor<Req>, 0, NetworkAddress >, public ActorCallback< RetryGetReplyFromHostnameActor<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public ActorCallback< RetryGetReplyFromHostnameActor<Req>, 2, Void >, public FastAllocated<RetryGetReplyFromHostnameActor<Req>>, public RetryGetReplyFromHostnameActorState<Req, RetryGetReplyFromHostnameActor<Req>> {
															#line 2073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActor(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token) 
															#line 2086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token ) {
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryGetReplyFromHostnameActor<Req>(request, hostname, token));
															#line 2118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 2123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req, class RetryGetReplyFromHostnameActor1>
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActor1State {
															#line 2130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActor1State(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : request(request),
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   hostname(hostname),
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   token(token),
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   taskID(taskID),
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   reconnectInitInterval(FLOW_KNOBS->HOSTNAME_RECONNECT_INIT_INTERVAL),
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   to()
															#line 2147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 2162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<NetworkAddress> __when_expr_0 = hostname.resolveWithRetry();
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 1;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 0, NetworkAddress >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress const& address,int loopDepth) 
	{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 2208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 2212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request, taskID);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(NetworkAddress && address,int loopDepth) 
	{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (to == nullptr || to->getEndpoint().getPrimaryAddress() != address)
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			to = std::make_unique<RequestStream<Req>>(Endpoint::wellKnown({ address }, token));
															#line 2236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Req)>> __when_expr_1 = to->tryGetReply(request, taskID);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 2;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 1, ErrorOr<REPLY_TYPE(Req)> >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (reply.isError())
															#line 2319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			resetReply(request);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (reply.getError().code() == error_code_request_maybe_delivered)
															#line 2325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_2 = delay(reconnectInitInterval);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
				static_cast<RetryGetReplyFromHostnameActor1*>(this)->actor_wait_state = 3;
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RetryGetReplyFromHostnameActor1, 2, Void >*>(static_cast<RetryGetReplyFromHostnameActor1*>(this)));
															#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(reply.getError(), std::max(0, loopDepth - 1));
															#line 2343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
		}
		else
		{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!static_cast<RetryGetReplyFromHostnameActor1*>(this)->SAV<REPLY_TYPE(Req)>::futures) { (void)(reply.get()); this->~RetryGetReplyFromHostnameActor1State(); static_cast<RetryGetReplyFromHostnameActor1*>(this)->destroy(); return 0; }
															#line 2350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			new (&static_cast<RetryGetReplyFromHostnameActor1*>(this)->SAV< REPLY_TYPE(Req) >::value()) REPLY_TYPE(Req)(reply.get());
			this->~RetryGetReplyFromHostnameActor1State();
			static_cast<RetryGetReplyFromHostnameActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(ErrorOr<REPLY_TYPE(Req)> const& __reply,int loopDepth) 
	{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reply = __reply;
															#line 2363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reconnectInitInterval = std::min(2 * reconnectInitInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		removeCachedDNS(hostname.host, hostname.service);
															#line 2444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		reconnectInitInterval = std::min(2 * reconnectInitInterval, FLOW_KNOBS->HOSTNAME_RECONNECT_MAX_INTERVAL);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		removeCachedDNS(hostname.host, hostname.service);
															#line 2455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Req request;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Hostname hostname;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WellKnownEndpoints token;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TaskPriority taskID;
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	double reconnectInitInterval;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::unique_ptr<RequestStream<Req>> to;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ErrorOr<REPLY_TYPE(Req)> reply;
															#line 2537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via retryGetReplyFromHostname()
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class RetryGetReplyFromHostnameActor1 final : public Actor<REPLY_TYPE(Req)>, public ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 0, NetworkAddress >, public ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 1, ErrorOr<REPLY_TYPE(Req)> >, public ActorCallback< RetryGetReplyFromHostnameActor1<Req>, 2, Void >, public FastAllocated<RetryGetReplyFromHostnameActor1<Req>>, public RetryGetReplyFromHostnameActor1State<Req, RetryGetReplyFromHostnameActor1<Req>> {
															#line 2544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	RetryGetReplyFromHostnameActor1(Req const& request,Hostname const& hostname,WellKnownEndpoints const& token,TaskPriority const& taskID) 
															#line 2557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class Req>
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Req)> retryGetReplyFromHostname( Req const& request, Hostname const& hostname, WellKnownEndpoints const& token, TaskPriority const& taskID ) {
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<REPLY_TYPE(Req)>(new RetryGetReplyFromHostnameActor1<Req>(request, hostname, token, taskID));
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 2594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via timeoutWarning()
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class TimeoutWarningActor>
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TimeoutWarningActorState {
															#line 2601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TimeoutWarningActorState(Future<T> const& what,double const& time,PromiseStream<Void> const& output) 
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : what(what),
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   time(time),
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   end(delay(time))
															#line 2614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 2629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<T> __when_expr_0 = what;
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<TimeoutWarningActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = end;
															#line 2665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<TimeoutWarningActor*>(this)->actor_wait_state = 1;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeoutWarningActor, 0, T >*>(static_cast<TimeoutWarningActor*>(this)));
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutWarningActor, 1, Void >*>(static_cast<TimeoutWarningActor*>(this)));
															#line 2672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<TimeoutWarningActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutWarningActorState(); static_cast<TimeoutWarningActor*>(this)->destroy(); return 0; }
															#line 2687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TimeoutWarningActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutWarningActorState();
		static_cast<TimeoutWarningActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(T && t,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<TimeoutWarningActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutWarningActorState(); static_cast<TimeoutWarningActor*>(this)->destroy(); return 0; }
															#line 2699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<TimeoutWarningActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutWarningActorState();
		static_cast<TimeoutWarningActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(Void());
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		end = delay(time);
															#line 2713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(Void());
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		end = delay(time);
															#line 2724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> what;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	double time;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	PromiseStream<Void> output;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<Void> end;
															#line 2834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via timeoutWarning()
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class TimeoutWarningActor final : public Actor<T>, public ActorCallback< TimeoutWarningActor<T>, 0, T >, public ActorCallback< TimeoutWarningActor<T>, 1, Void >, public FastAllocated<TimeoutWarningActor<T>>, public TimeoutWarningActorState<T, TimeoutWarningActor<T>> {
															#line 2841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<TimeoutWarningActor<T>>::operator new;
	using FastAllocated<TimeoutWarningActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeoutWarningActor<T>, 0, T >;
friend struct ActorCallback< TimeoutWarningActor<T>, 1, Void >;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	TimeoutWarningActor(Future<T> const& what,double const& time,PromiseStream<Void> const& output) 
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<T> timeoutWarning( Future<T> const& what, double const& time, PromiseStream<Void> const& output ) {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<T>(new TimeoutWarningActor<T>(what, time, output));
															#line 2883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 2888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardPromise()
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class ForwardPromiseActor>
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ForwardPromiseActorState {
															#line 2895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ForwardPromiseActorState(Promise<T> const& output,Future<T> const& input) 
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : output(output),
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   input(input)
															#line 2904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 2922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				static_cast<ForwardPromiseActor*>(this)->actor_wait_state = 1;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardPromiseActor, 0, T >*>(static_cast<ForwardPromiseActor*>(this)));
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output.sendError(err);
															#line 2961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 2976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 2985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		delete static_cast<ForwardPromiseActor*>(this);
															#line 3070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Promise<T> output;
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 3079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardPromise()
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor final : public Actor<void>, public ActorCallback< ForwardPromiseActor<T>, 0, T >, public FastAllocated<ForwardPromiseActor<T>>, public ForwardPromiseActorState<T, ForwardPromiseActor<T>> {
															#line 3086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardPromiseActor<T>>::operator new;
	using FastAllocated<ForwardPromiseActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardPromiseActor<T>, 0, T >;
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor(Promise<T> const& output,Future<T> const& input) 
															#line 3097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
void forwardPromise( Promise<T> const& output, Future<T> const& input ) {
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	new ForwardPromiseActor<T>(output, input);
															#line 3118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 3123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardPromise()
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class ForwardPromiseActor1>
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor1State {
															#line 3130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor1State(ReplyPromise<T> const& output,Future<T> const& input) 
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : output(output),
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   input(input)
															#line 3139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 3157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				static_cast<ForwardPromiseActor1*>(this)->actor_wait_state = 1;
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardPromiseActor1, 0, T >*>(static_cast<ForwardPromiseActor1*>(this)));
															#line 3161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output.sendError(err);
															#line 3196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		delete static_cast<ForwardPromiseActor1*>(this);
															#line 3305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReplyPromise<T> output;
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 3314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardPromise()
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor1 final : public Actor<void>, public ActorCallback< ForwardPromiseActor1<T>, 0, T >, public FastAllocated<ForwardPromiseActor1<T>>, public ForwardPromiseActor1State<T, ForwardPromiseActor1<T>> {
															#line 3321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardPromiseActor1<T>>::operator new;
	using FastAllocated<ForwardPromiseActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardPromiseActor1<T>, 0, T >;
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor1(ReplyPromise<T> const& output,Future<T> const& input) 
															#line 3332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
void forwardPromise( ReplyPromise<T> const& output, Future<T> const& input ) {
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	new ForwardPromiseActor1<T>(output, input);
															#line 3353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 3358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardPromise()
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class ForwardPromiseActor2>
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor2State {
															#line 3365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor2State(PromiseStream<T> const& output,Future<T> const& input) 
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : output(output),
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   input(input)
															#line 3374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 3392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				static_cast<ForwardPromiseActor2*>(this)->actor_wait_state = 1;
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardPromiseActor2, 0, T >*>(static_cast<ForwardPromiseActor2*>(this)));
															#line 3396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output.sendError(e);
															#line 3431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output.send(value);
															#line 3455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		delete static_cast<ForwardPromiseActor2*>(this);
															#line 3540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	PromiseStream<T> output;
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 3549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardPromise()
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ForwardPromiseActor2 final : public Actor<void>, public ActorCallback< ForwardPromiseActor2<T>, 0, T >, public FastAllocated<ForwardPromiseActor2<T>>, public ForwardPromiseActor2State<T, ForwardPromiseActor2<T>> {
															#line 3556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardPromiseActor2<T>>::operator new;
	using FastAllocated<ForwardPromiseActor2<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardPromiseActor2<T>, 0, T >;
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ForwardPromiseActor2(PromiseStream<T> const& output,Future<T> const& input) 
															#line 3567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
void forwardPromise( PromiseStream<T> const& output, Future<T> const& input ) {
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	new ForwardPromiseActor2<T>(output, input);
															#line 3588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 3593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via broadcast()
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class BroadcastActor>
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class BroadcastActorState {
															#line 3600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	BroadcastActorState(Future<T> const& input,std::vector<Promise<T>> const& output) 
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   output(output)
															#line 3609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<BroadcastActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<BroadcastActor*>(this)->actor_wait_state = 1;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BroadcastActor, 0, T >*>(static_cast<BroadcastActor*>(this)));
															#line 3631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActorState(); static_cast<BroadcastActor*>(this)->destroy(); return 0; }
															#line 3660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<BroadcastActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~BroadcastActorState();
		static_cast<BroadcastActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && value,int loopDepth) 
	{
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActorState(); static_cast<BroadcastActor*>(this)->destroy(); return 0; }
															#line 3678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::vector<Promise<T>> output;
															#line 3753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via broadcast()
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class BroadcastActor final : public Actor<Void>, public ActorCallback< BroadcastActor<T>, 0, T >, public FastAllocated<BroadcastActor<T>>, public BroadcastActorState<T, BroadcastActor<T>> {
															#line 3760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<BroadcastActor<T>>::operator new;
	using FastAllocated<BroadcastActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BroadcastActor<T>, 0, T >;
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	BroadcastActor(Future<T> const& input,std::vector<Promise<T>> const& output) 
															#line 3771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> broadcast( Future<T> const& input, std::vector<Promise<T>> const& output ) {
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<Void>(new BroadcastActor<T>(input, output));
															#line 3801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 3806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via broadcast()
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class BroadcastActor1>
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class BroadcastActor1State {
															#line 3813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	BroadcastActor1State(Future<T> const& input,std::vector<ReplyPromise<T>> const& output) 
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   output(output)
															#line 3822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<BroadcastActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<BroadcastActor1*>(this)->actor_wait_state = 1;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BroadcastActor1, 0, T >*>(static_cast<BroadcastActor1*>(this)));
															#line 3844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActor1State(); static_cast<BroadcastActor1*>(this)->destroy(); return 0; }
															#line 3873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<BroadcastActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~BroadcastActor1State();
		static_cast<BroadcastActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && value,int loopDepth) 
	{
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		for(int i = 0;i < output.size();i++) {
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			output[i].send(value);
															#line 3887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		}
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<BroadcastActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~BroadcastActor1State(); static_cast<BroadcastActor1*>(this)->destroy(); return 0; }
															#line 3891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::vector<ReplyPromise<T>> output;
															#line 3966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via broadcast()
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class BroadcastActor1 final : public Actor<Void>, public ActorCallback< BroadcastActor1<T>, 0, T >, public FastAllocated<BroadcastActor1<T>>, public BroadcastActor1State<T, BroadcastActor1<T>> {
															#line 3973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<BroadcastActor1<T>>::operator new;
	using FastAllocated<BroadcastActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BroadcastActor1<T>, 0, T >;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	BroadcastActor1(Future<T> const& input,std::vector<ReplyPromise<T>> const& output) 
															#line 3984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> broadcast( Future<T> const& input, std::vector<ReplyPromise<T>> const& output ) {
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<Void>(new BroadcastActor1<T>(input, output));
															#line 4014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 4019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via incrementalBroadcast()
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class IncrementalBroadcastActor>
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActorState {
															#line 4026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActorState(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   batchSize(batchSize)
															#line 4037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state = 1;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor, 0, T >*>(static_cast<IncrementalBroadcastActor*>(this)));
															#line 4059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		i = 0;
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		;
															#line 4084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& __value,int loopDepth) 
	{
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		value = __value;
															#line 4093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<IncrementalBroadcastActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalBroadcastActorState(); static_cast<IncrementalBroadcastActor*>(this)->destroy(); return 0; }
															#line 4160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 4179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output[i].send(value);
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 4187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0);
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor*>(this)->actor_wait_state = 2;
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor, 1, Void >*>(static_cast<IncrementalBroadcastActor*>(this)));
															#line 4198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		i++;
															#line 4225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::vector<Promise<T>> output;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	int batchSize;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	T value;
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	int i;
															#line 4315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via incrementalBroadcast()
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActor final : public Actor<Void>, public ActorCallback< IncrementalBroadcastActor<T>, 0, T >, public ActorCallback< IncrementalBroadcastActor<T>, 1, Void >, public FastAllocated<IncrementalBroadcastActor<T>>, public IncrementalBroadcastActorState<T, IncrementalBroadcastActor<T>> {
															#line 4322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<IncrementalBroadcastActor<T>>::operator new;
	using FastAllocated<IncrementalBroadcastActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncrementalBroadcastActor<T>, 0, T >;
friend struct ActorCallback< IncrementalBroadcastActor<T>, 1, Void >;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActor(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 4334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> incrementalBroadcast( Future<T> const& input, std::vector<Promise<T>> const& output, int const& batchSize ) {
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<Void>(new IncrementalBroadcastActor<T>(input, output, batchSize));
															#line 4365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 4370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via incrementalBroadcast()
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class IncrementalBroadcastActor1>
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActor1State {
															#line 4377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActor1State(Future<T> const& input,std::vector<ReplyPromise<T>> const& output,int const& batchSize) 
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   batchSize(batchSize)
															#line 4388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = input;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state = 1;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor1, 0, T >*>(static_cast<IncrementalBroadcastActor1*>(this)));
															#line 4410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		i = 0;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		;
															#line 4435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& __value,int loopDepth) 
	{
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		value = __value;
															#line 4444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<IncrementalBroadcastActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalBroadcastActor1State(); static_cast<IncrementalBroadcastActor1*>(this)->destroy(); return 0; }
															#line 4511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 4530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output[i].send(value);
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 4538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0);
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncrementalBroadcastActor1*>(this)->actor_wait_state = 2;
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastActor1, 1, Void >*>(static_cast<IncrementalBroadcastActor1*>(this)));
															#line 4549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		i++;
															#line 4576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::vector<ReplyPromise<T>> output;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	int batchSize;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	T value;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	int i;
															#line 4666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via incrementalBroadcast()
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastActor1 final : public Actor<Void>, public ActorCallback< IncrementalBroadcastActor1<T>, 0, T >, public ActorCallback< IncrementalBroadcastActor1<T>, 1, Void >, public FastAllocated<IncrementalBroadcastActor1<T>>, public IncrementalBroadcastActor1State<T, IncrementalBroadcastActor1<T>> {
															#line 4673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<IncrementalBroadcastActor1<T>>::operator new;
	using FastAllocated<IncrementalBroadcastActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncrementalBroadcastActor1<T>, 0, T >;
friend struct ActorCallback< IncrementalBroadcastActor1<T>, 1, Void >;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastActor1(Future<T> const& input,std::vector<ReplyPromise<T>> const& output,int const& batchSize) 
															#line 4685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> incrementalBroadcast( Future<T> const& input, std::vector<ReplyPromise<T>> const& output, int const& batchSize ) {
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<Void>(new IncrementalBroadcastActor1<T>(input, output, batchSize));
															#line 4716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 4721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via incrementalBroadcastWithError()
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class IncrementalBroadcastWithErrorActor>
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastWithErrorActorState {
															#line 4728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastWithErrorActorState(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : input(input),
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   output(output),
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   batchSize(batchSize),
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   i(0)
															#line 4741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 1;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastWithErrorActor, 0, T >*>(static_cast<IncrementalBroadcastWithErrorActor*>(this)));
															#line 4764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<IncrementalBroadcastWithErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncrementalBroadcastWithErrorActorState(); static_cast<IncrementalBroadcastWithErrorActor*>(this)->destroy(); return 0; }
															#line 4793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<IncrementalBroadcastWithErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncrementalBroadcastWithErrorActorState();
		static_cast<IncrementalBroadcastWithErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& _e,int loopDepth=0) 
	{
		try {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (_e.code() == error_code_operation_cancelled)
															#line 4806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(_e, loopDepth);
															#line 4810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			e = _e;
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 4816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		;
															#line 4831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& __value,int loopDepth) 
	{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		value = __value;
															#line 4840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 4920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output[i].send(value);
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 4928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0);
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 2;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastWithErrorActor, 1, Void >*>(static_cast<IncrementalBroadcastWithErrorActor*>(this)));
															#line 4939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		i++;
															#line 4966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!(i < output.size()))
															#line 5076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
			return a_body1Catch2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		output[i].sendError(e);
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if ((i + 1) % batchSize == 0)
															#line 5084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_2 = delay(0);
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1Catch2loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<IncrementalBroadcastWithErrorActor*>(this)->actor_wait_state = 3;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< IncrementalBroadcastWithErrorActor, 2, Void >*>(static_cast<IncrementalBroadcastWithErrorActor*>(this)));
															#line 5095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		i++;
															#line 5122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<T> input;
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	std::vector<Promise<T>> output;
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	int batchSize;
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	int i;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	T value;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Error e;
															#line 5214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via incrementalBroadcastWithError()
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class IncrementalBroadcastWithErrorActor final : public Actor<Void>, public ActorCallback< IncrementalBroadcastWithErrorActor<T>, 0, T >, public ActorCallback< IncrementalBroadcastWithErrorActor<T>, 1, Void >, public ActorCallback< IncrementalBroadcastWithErrorActor<T>, 2, Void >, public FastAllocated<IncrementalBroadcastWithErrorActor<T>>, public IncrementalBroadcastWithErrorActorState<T, IncrementalBroadcastWithErrorActor<T>> {
															#line 5221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	IncrementalBroadcastWithErrorActor(Future<T> const& input,std::vector<Promise<T>> const& output,int const& batchSize) 
															#line 5234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<Void> incrementalBroadcastWithError( Future<T> const& input, std::vector<Promise<T>> const& output, int const& batchSize ) {
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<Void>(new IncrementalBroadcastWithErrorActor<T>(input, output, batchSize));
															#line 5266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

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
															#line 5294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via endStreamOnDisconnect()
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X, class EndStreamOnDisconnectActor>
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class EndStreamOnDisconnectActorState {
															#line 5301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	EndStreamOnDisconnectActorState(Future<Void> const& signal,ReplyPromiseStream<X> const& stream,Endpoint const& endpoint,Reference<Peer> const& peer = Reference<Peer>()) 
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : signal(signal),
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   stream(stream),
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint),
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   peer(peer),
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   holder(PeerHolder(peer))
															#line 5316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			stream.setRequestStreamEndpoint(endpoint);
															#line 5331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			try {
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = signal;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_1 = peer.isValid() ? peer->disconnect.getFuture() : Never();
															#line 5339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_2 = stream.getErrorFutureAndDelPromiseRef();
															#line 5343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1when3(__when_expr_2.get(), loopDepth); };
				static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state = 1;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 0, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 1, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 2, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 5352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 5387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (!stream.connected())
															#line 5391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				{
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
					StrictFuture<Void> __when_expr_3 = signal || stream.onConnected();
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
					if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_3.get(), loopDepth); };
															#line 5397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
					static_cast<EndStreamOnDisconnectActor*>(this)->actor_wait_state = 2;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
					__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< EndStreamOnDisconnectActor, 3, Void >*>(static_cast<EndStreamOnDisconnectActor*>(this)));
															#line 5401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		stream.sendError(connection_failed());
															#line 5459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		stream.notifyFailed();
															#line 5636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		delete static_cast<EndStreamOnDisconnectActor*>(this);
															#line 5726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<Void> signal;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReplyPromiseStream<X> stream;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Reference<Peer> peer;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	PeerHolder holder;
															#line 5741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via endStreamOnDisconnect()
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X>
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class EndStreamOnDisconnectActor final : public Actor<void>, public ActorCallback< EndStreamOnDisconnectActor<X>, 0, Void >, public ActorCallback< EndStreamOnDisconnectActor<X>, 1, Void >, public ActorCallback< EndStreamOnDisconnectActor<X>, 2, Void >, public ActorCallback< EndStreamOnDisconnectActor<X>, 3, Void >, public FastAllocated<EndStreamOnDisconnectActor<X>>, public EndStreamOnDisconnectActorState<X, EndStreamOnDisconnectActor<X>> {
															#line 5748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	EndStreamOnDisconnectActor(Future<Void> const& signal,ReplyPromiseStream<X> const& stream,Endpoint const& endpoint,Reference<Peer> const& peer = Reference<Peer>()) 
															#line 5762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X>
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
void endStreamOnDisconnect( Future<Void> const& signal, ReplyPromiseStream<X> const& stream, Endpoint const& endpoint, Reference<Peer> const& peer = Reference<Peer>() ) {
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	new EndStreamOnDisconnectActor<X>(signal, stream, endpoint, peer);
															#line 5783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

// Implements tryGetReply, getReplyUnlessFailedFor
															#line 5789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via waitValueOrSignal()
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X, class WaitValueOrSignalActor>
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class WaitValueOrSignalActorState {
															#line 5796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WaitValueOrSignalActorState(Future<X> const& value,Future<Void> const& signal,Endpoint const& endpoint,ReplyPromise<X> const& holdme = ReplyPromise<X>(),Reference<Peer> const& peer = Reference<Peer>()) 
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : value(value),
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   signal(signal),
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint),
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   holdme(holdme),
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   peer(peer),
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   holder(PeerHolder(peer))
															#line 5813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			;
															#line 5828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<X> __when_expr_0 = value;
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (static_cast<WaitValueOrSignalActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = signal;
															#line 5865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<WaitValueOrSignalActor*>(this)->actor_wait_state = 1;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitValueOrSignalActor, 0, X >*>(static_cast<WaitValueOrSignalActor*>(this)));
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitValueOrSignalActor, 1, Void >*>(static_cast<WaitValueOrSignalActor*>(this)));
															#line 5872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (signal.isError())
															#line 5894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				TraceEvent(SevError, "WaitValueOrSignalError").error(signal.getError());
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(internal_error())); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(internal_error()));
				this->~WaitValueOrSignalActorState();
				static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 5908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 5912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() != error_code_broken_promise || signal.isError())
															#line 5916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(e)); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(e));
				this->~WaitValueOrSignalActorState();
				static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			IFailureMonitor::failureMonitor().endpointNotFound(endpoint);
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			value = Never();
															#line 5930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(x); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(x);
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(X && x,int loopDepth) 
	{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(x); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(x);
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(IFailureMonitor::failureMonitor().knownUnauthorized(endpoint) ? unauthorized_attempt() : request_maybe_delivered())); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(IFailureMonitor::failureMonitor().knownUnauthorized(endpoint) ? unauthorized_attempt() : request_maybe_delivered()));
		this->~WaitValueOrSignalActorState();
		static_cast<WaitValueOrSignalActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<WaitValueOrSignalActor*>(this)->SAV<ErrorOr<X>>::futures) { (void)(ErrorOr<X>(IFailureMonitor::failureMonitor().knownUnauthorized(endpoint) ? unauthorized_attempt() : request_maybe_delivered())); this->~WaitValueOrSignalActorState(); static_cast<WaitValueOrSignalActor*>(this)->destroy(); return 0; }
															#line 5981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<WaitValueOrSignalActor*>(this)->SAV< ErrorOr<X> >::value()) ErrorOr<X>(ErrorOr<X>(IFailureMonitor::failureMonitor().knownUnauthorized(endpoint) ? unauthorized_attempt() : request_maybe_delivered()));
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
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<X> value;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<Void> signal;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReplyPromise<X> holdme;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Reference<Peer> peer;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	PeerHolder holder;
															#line 6098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via waitValueOrSignal()
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X>
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class WaitValueOrSignalActor final : public Actor<ErrorOr<X>>, public ActorCallback< WaitValueOrSignalActor<X>, 0, X >, public ActorCallback< WaitValueOrSignalActor<X>, 1, Void >, public FastAllocated<WaitValueOrSignalActor<X>>, public WaitValueOrSignalActorState<X, WaitValueOrSignalActor<X>> {
															#line 6105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<WaitValueOrSignalActor<X>>::operator new;
	using FastAllocated<WaitValueOrSignalActor<X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<X>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitValueOrSignalActor<X>, 0, X >;
friend struct ActorCallback< WaitValueOrSignalActor<X>, 1, Void >;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	WaitValueOrSignalActor(Future<X> const& value,Future<Void> const& signal,Endpoint const& endpoint,ReplyPromise<X> const& holdme = ReplyPromise<X>(),Reference<Peer> const& peer = Reference<Peer>()) 
															#line 6117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X>
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<X>> waitValueOrSignal( Future<X> const& value, Future<Void> const& signal, Endpoint const& endpoint, ReplyPromise<X> const& holdme = ReplyPromise<X>(), Reference<Peer> const& peer = Reference<Peer>() ) {
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<ErrorOr<X>>(new WaitValueOrSignalActor<X>(value, signal, endpoint, holdme, peer));
															#line 6147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 6152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via sendCanceler()
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T, class SendCancelerActor>
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class SendCancelerActorState {
															#line 6159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	SendCancelerActorState(ReplyPromise<T> const& reply,ReliablePacket* const& send,Endpoint const& endpoint) 
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : reply(reply),
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   send(send),
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint),
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   didCancelReliable(false)
															#line 6172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				;
															#line 6188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
		this->~SendCancelerActorState();
		static_cast<SendCancelerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (!didCancelReliable)
															#line 6218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				FlowTransport::transport().cancelReliable(send);
															#line 6222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 6226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				IFailureMonitor::failureMonitor().endpointNotFound(endpoint);
															#line 6230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 6234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (IFailureMonitor::failureMonitor().permanentlyFailed(endpoint))
															#line 6255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		{
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			FlowTransport::transport().cancelReliable(send);
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			didCancelReliable = true;
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (IFailureMonitor::failureMonitor().knownUnauthorized(endpoint))
															#line 6263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				return a_body1Catch2(unauthorized_attempt(), std::max(0, loopDepth - 1));
															#line 6267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
			else
			{
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = Never();
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (static_cast<SendCancelerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<SendCancelerActor*>(this)->actor_wait_state = 1;
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendCancelerActor, 0, Void >*>(static_cast<SendCancelerActor*>(this)));
															#line 6280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				loopDepth = 0;
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<T> __when_expr_1 = reply.getFuture();
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (static_cast<SendCancelerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		StrictFuture<Void> __when_expr_2 = IFailureMonitor::failureMonitor().onStateChanged(endpoint);
															#line 6301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
		static_cast<SendCancelerActor*>(this)->actor_wait_state = 2;
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SendCancelerActor, 1, T >*>(static_cast<SendCancelerActor*>(this)));
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SendCancelerActor, 2, Void >*>(static_cast<SendCancelerActor*>(this)));
															#line 6308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
		if (static_cast<SendCancelerActor*>(this)->actor_wait_state > 0) static_cast<SendCancelerActor*>(this)->actor_wait_state = 0;
		static_cast<SendCancelerActor*>(this)->ActorCallback< SendCancelerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendCancelerActor, 0, Void >*,Error err) 
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
	int a_body1loopBody1cont5(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(T const& t,int loopDepth) 
	{
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		FlowTransport::transport().cancelReliable(send);
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		didCancelReliable = true;
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<SendCancelerActor*>(this)->SAV<T>::futures) { (void)(t); this->~SendCancelerActorState(); static_cast<SendCancelerActor*>(this)->destroy(); return 0; }
															#line 6408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<SendCancelerActor*>(this)->SAV< T >::value()) T(t);
		this->~SendCancelerActorState();
		static_cast<SendCancelerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(T && t,int loopDepth) 
	{
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		FlowTransport::transport().cancelReliable(send);
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		didCancelReliable = true;
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<SendCancelerActor*>(this)->SAV<T>::futures) { (void)(t); this->~SendCancelerActorState(); static_cast<SendCancelerActor*>(this)->destroy(); return 0; }
															#line 6424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<SendCancelerActor*>(this)->SAV< T >::value()) T(t);
		this->~SendCancelerActorState();
		static_cast<SendCancelerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SendCancelerActor*>(this)->actor_wait_state > 0) static_cast<SendCancelerActor*>(this)->actor_wait_state = 0;
		static_cast<SendCancelerActor*>(this)->ActorCallback< SendCancelerActor, 1, T >::remove();
		static_cast<SendCancelerActor*>(this)->ActorCallback< SendCancelerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SendCancelerActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SendCancelerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SendCancelerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendCanceler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCanceler", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReplyPromise<T> reply;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReliablePacket* send;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	bool didCancelReliable;
															#line 6549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via sendCanceler()
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class SendCancelerActor final : public Actor<T>, public ActorCallback< SendCancelerActor<T>, 0, Void >, public ActorCallback< SendCancelerActor<T>, 1, T >, public ActorCallback< SendCancelerActor<T>, 2, Void >, public FastAllocated<SendCancelerActor<T>>, public SendCancelerActorState<T, SendCancelerActor<T>> {
															#line 6556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<SendCancelerActor<T>>::operator new;
	using FastAllocated<SendCancelerActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendCancelerActor<T>, 0, Void >;
friend struct ActorCallback< SendCancelerActor<T>, 1, T >;
friend struct ActorCallback< SendCancelerActor<T>, 2, Void >;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	SendCancelerActor(ReplyPromise<T> const& reply,ReliablePacket* const& send,Endpoint const& endpoint) 
															#line 6569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
		case 1: this->a_callback_error((ActorCallback< SendCancelerActor<T>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SendCancelerActor<T>, 1, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class T>
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<T> sendCanceler( ReplyPromise<T> const& reply, ReliablePacket* const& send, Endpoint const& endpoint ) {
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<T>(new SendCancelerActor<T>(reply, send, endpoint));
															#line 6600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

															#line 6605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via reportEndpointFailure()
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X, class ReportEndpointFailureActor>
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ReportEndpointFailureActorState {
															#line 6612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReportEndpointFailureActorState(Future<X> const& value,Endpoint const& endpoint) 
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		 : value(value),
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		   endpoint(endpoint)
															#line 6621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				StrictFuture<X> __when_expr_0 = value;
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				if (static_cast<ReportEndpointFailureActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReportEndpointFailureActor*>(this)->actor_wait_state = 1;
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReportEndpointFailureActor, 0, X >*>(static_cast<ReportEndpointFailureActor*>(this)));
															#line 6644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 6674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			{
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
				IFailureMonitor::failureMonitor().endpointNotFound(endpoint);
															#line 6678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
			}
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 6682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<ReportEndpointFailureActor*>(this)->SAV<X>::futures) { (void)(x); this->~ReportEndpointFailureActorState(); static_cast<ReportEndpointFailureActor*>(this)->destroy(); return 0; }
															#line 6696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
		new (&static_cast<ReportEndpointFailureActor*>(this)->SAV< X >::value()) X(x);
		this->~ReportEndpointFailureActorState();
		static_cast<ReportEndpointFailureActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(X && x,int loopDepth) 
	{
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
		if (!static_cast<ReportEndpointFailureActor*>(this)->SAV<X>::futures) { (void)(x); this->~ReportEndpointFailureActorState(); static_cast<ReportEndpointFailureActor*>(this)->destroy(); return 0; }
															#line 6708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Future<X> value;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	Endpoint endpoint;
															#line 6783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
};
// This generated class is to be used only via reportEndpointFailure()
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X>
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
class ReportEndpointFailureActor final : public Actor<X>, public ActorCallback< ReportEndpointFailureActor<X>, 0, X >, public FastAllocated<ReportEndpointFailureActor<X>>, public ReportEndpointFailureActorState<X, ReportEndpointFailureActor<X>> {
															#line 6790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
public:
	using FastAllocated<ReportEndpointFailureActor<X>>::operator new;
	using FastAllocated<ReportEndpointFailureActor<X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<X>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReportEndpointFailureActor<X>, 0, X >;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	ReportEndpointFailureActor(Future<X> const& value,Endpoint const& endpoint) 
															#line 6801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
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
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
template <class X>
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
[[nodiscard]] Future<X> reportEndpointFailure( Future<X> const& value, Endpoint const& endpoint ) {
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"
	return Future<X>(new ReportEndpointFailureActor<X>(value, endpoint));
															#line 6831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.g.h"
}

#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/genericactors.actor.h"

#include "flow/unactorcompiler.h"

#endif
