#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
/*
 * ThreadHelper.actor.h
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

#pragma once

// When actually compiled (NO_INTELLISENSE), include the generated
// version of this file.  In intellisense use the source version.
#include "flow/Error.h"
#include <cstddef>
#if defined(NO_INTELLISENSE) && !defined(FLOW_THREADHELPER_ACTOR_G_H)
#define FLOW_THREADHELPER_ACTOR_G_H
#include "flow/ThreadHelper.actor.g.h"
#elif !defined(FLOW_THREADHELPER_ACTOR_H)
#define FLOW_THREADHELPER_ACTOR_H

#include <utility>

#include "flow/flow.h"
#include "flow/actorcompiler.h" // This must be the last #include.

// Helper actor. Do not use directly!
namespace internal_thread_helper {

															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
// This generated class is to be used only via doOnMainThreadVoid()
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class F, class DoOnMainThreadVoidActor>
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class DoOnMainThreadVoidActorState {
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	DoOnMainThreadVoidActorState(Future<Void> const& signal,F const& f) 
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		 : signal(signal),
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		   f(f)
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
	{
		fdb_probe_actor_create("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this));

	}
	~DoOnMainThreadVoidActorState() 
	{
		fdb_probe_actor_destroy("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			static_cast<DoOnMainThreadVoidActor*>(this)->actor_wait_state = 1;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoOnMainThreadVoidActor, 0, Void >*>(static_cast<DoOnMainThreadVoidActor*>(this)));
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		delete static_cast<DoOnMainThreadVoidActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			f();
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			loopDepth = a_body1cont4(loopDepth);
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
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			f();
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			loopDepth = a_body1cont4(loopDepth);
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
		if (static_cast<DoOnMainThreadVoidActor*>(this)->actor_wait_state > 0) static_cast<DoOnMainThreadVoidActor*>(this)->actor_wait_state = 0;
		static_cast<DoOnMainThreadVoidActor*>(this)->ActorCallback< DoOnMainThreadVoidActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadVoidActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadVoidActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoOnMainThreadVoidActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
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
	int a_body1cont4(int loopDepth) 
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
	int a_body1cont5(int loopDepth) 
	{
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		delete static_cast<DoOnMainThreadVoidActor*>(this);
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	Future<Void> signal;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	F f;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
};
// This generated class is to be used only via doOnMainThreadVoid()
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class F>
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class DoOnMainThreadVoidActor final : public Actor<void>, public ActorCallback< DoOnMainThreadVoidActor<F>, 0, Void >, public FastAllocated<DoOnMainThreadVoidActor<F>>, public DoOnMainThreadVoidActorState<F, DoOnMainThreadVoidActor<F>> {
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
	using FastAllocated<DoOnMainThreadVoidActor<F>>::operator new;
	using FastAllocated<DoOnMainThreadVoidActor<F>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoOnMainThreadVoidActor<F>, 0, Void >;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	DoOnMainThreadVoidActor(Future<Void> const& signal,F const& f) 
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		 : Actor<void>(),
		   DoOnMainThreadVoidActorState<F, DoOnMainThreadVoidActor<F>>(signal, f)
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doOnMainThreadVoid");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class F>
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
void doOnMainThreadVoid( Future<Void> const& signal, F const& f ) {
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	new DoOnMainThreadVoidActor<F>(signal, f);
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
}

#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"

															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
// This generated class is to be used only via doOnMainThreadVoid()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class F, class T, class DoOnMainThreadVoidActor1>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class DoOnMainThreadVoidActor1State {
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	DoOnMainThreadVoidActor1State(Future<Void> const& signal,F const& f,T* const& t,Error T::* const& member) 
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		 : signal(signal),
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		   f(f),
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		   t(t),
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		   member(member)
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
	{
		fdb_probe_actor_create("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this));

	}
	~DoOnMainThreadVoidActor1State() 
	{
		fdb_probe_actor_destroy("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			static_cast<DoOnMainThreadVoidActor1*>(this)->actor_wait_state = 1;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoOnMainThreadVoidActor1, 0, Void >*>(static_cast<DoOnMainThreadVoidActor1*>(this)));
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		delete static_cast<DoOnMainThreadVoidActor1*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (t && (t->*member).code() != invalid_error_code)
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			delete static_cast<DoOnMainThreadVoidActor1*>(this);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			return 0;
		}
		try {
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			f();
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			loopDepth = a_body1cont5(loopDepth);
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
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (t && (t->*member).code() != invalid_error_code)
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			delete static_cast<DoOnMainThreadVoidActor1*>(this);
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			return 0;
		}
		try {
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			f();
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			loopDepth = a_body1cont5(loopDepth);
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
		if (static_cast<DoOnMainThreadVoidActor1*>(this)->actor_wait_state > 0) static_cast<DoOnMainThreadVoidActor1*>(this)->actor_wait_state = 0;
		static_cast<DoOnMainThreadVoidActor1*>(this)->ActorCallback< DoOnMainThreadVoidActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadVoidActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadVoidActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoOnMainThreadVoidActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (t)
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			{
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				t->*member = e;
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			}
			loopDepth = a_body1cont2(loopDepth);
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
	int a_body1cont6(int loopDepth) 
	{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		delete static_cast<DoOnMainThreadVoidActor1*>(this);
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	Future<Void> signal;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	F f;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	T* t;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	Error T::* member;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
};
// This generated class is to be used only via doOnMainThreadVoid()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class F, class T>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class DoOnMainThreadVoidActor1 final : public Actor<void>, public ActorCallback< DoOnMainThreadVoidActor1<F, T>, 0, Void >, public FastAllocated<DoOnMainThreadVoidActor1<F, T>>, public DoOnMainThreadVoidActor1State<F, T, DoOnMainThreadVoidActor1<F, T>> {
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
	using FastAllocated<DoOnMainThreadVoidActor1<F, T>>::operator new;
	using FastAllocated<DoOnMainThreadVoidActor1<F, T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DoOnMainThreadVoidActor1<F, T>, 0, Void >;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	DoOnMainThreadVoidActor1(Future<Void> const& signal,F const& f,T* const& t,Error T::* const& member) 
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		 : Actor<void>(),
		   DoOnMainThreadVoidActor1State<F, T, DoOnMainThreadVoidActor1<F, T>>(signal, f, t, member)
	{
		fdb_probe_actor_enter("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doOnMainThreadVoid");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doOnMainThreadVoid", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class F, class T>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
void doOnMainThreadVoid( Future<Void> const& signal, F const& f, T* const& t, Error T::* const& member ) {
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	new DoOnMainThreadVoidActor1<F, T>(signal, f, t, member);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
}

#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"

} // namespace internal_thread_helper

// onMainThreadVoid runs a functor on the FDB network thread. The value returned by the functor is ignored.
// There is no way to wait for the functor run to finish. For cases where you need a result back or simply need
// to know when the functor has finished running, use `onMainThread`.
//
// WARNING: Successive invocations of `onMainThreadVoid` with different task priorities may not run in the order they
// were called.
//
// WARNING: The error returned in `err` can only be read on the FDB network thread because there is no way to
// order the write to `err` with actions on other threads.
//
// WARNING: The Error member of `T` is accepted as a pointer to a data member so the caller can avoid dereferencing
// `T` until it is initialized on the main thread.
//
// `onMainThreadVoid` is defined here because of the dependency in `ThreadSingleAssignmentVarBase`.
template <class F, class T>
void onMainThreadVoid(F f, T* t, Error T::*member, TaskPriority taskID = TaskPriority::DefaultOnMainThread) {
	Promise<Void> signal;
	internal_thread_helper::doOnMainThreadVoid(signal.getFuture(), f, t, member);
	g_network->onMainThread(std::move(signal), taskID);
}

template <class F>
void onMainThreadVoid(F f, TaskPriority taskID = TaskPriority::DefaultOnMainThread) {
	Promise<Void> signal;
	internal_thread_helper::doOnMainThreadVoid(signal.getFuture(), f);
	g_network->onMainThread(std::move(signal), taskID);
}

class ThreadMultiCallback;

struct ThreadCallback {
	virtual bool canFire(int notMadeActive) const = 0;
	virtual void fire(const Void& unused, int& userParam) = 0;
	virtual void error(const Error&, int& userParam) = 0;
	virtual ThreadCallback* addCallback(ThreadCallback* cb);

	virtual bool clearCallback(ThreadCallback* cb) {
		// If this is the only registered callback this will be called with (possibly) arbitrary pointers
		return false;
	}

	// Note that when a ThreadCallback is destroyed it must have no MultiCallbackHolders, but this can't be
	// asserted on destruction because throwing is not allowed in ~ThreadCallback() and the default destroy()
	// implementation here is never called.
	// However, ThreadMultiCallback::destroy() ensures that no ThreadMultiCallback will be destroyed while
	// still holding a ThreadCallback so the invariant is effectively enforced there.  See
	// ThreadMultiCallback::destroy() for more details.
	virtual void destroy() { UNSTOPPABLE_ASSERT(false); }
	virtual bool isMultiCallback() const { return false; }

	// MultiCallbackHolder is a helper object for ThreadMultiCallback which allows it to store its index
	// within the callback vector inside the ThreadCallback rather than having a map of pointers or
	// some other scheme to store the indices by callback.
	// MultiCallbackHolder objects can form a doubly linked list.
	struct MultiCallbackHolder : public FastAllocated<MultiCallbackHolder> {
		// Construction requires no arguments or all the arguments
		MultiCallbackHolder() : multiCallback(nullptr), index(0), previous(nullptr), next(nullptr) {}
		MultiCallbackHolder(ThreadMultiCallback* multiCallback,
		                    int index,
		                    MultiCallbackHolder* prev,
		                    MultiCallbackHolder* next)
		  : multiCallback(multiCallback), index(0), previous(prev), next(next) {}

		ThreadMultiCallback* multiCallback;
		int index;
		MultiCallbackHolder* previous;
		MultiCallbackHolder* next;
	};

	// firstHolder is both the inline first record of a MultiCallbackHolder and the head of the
	// doubly linked list of MultiCallbackHolder entries.
	MultiCallbackHolder firstHolder;

	// Return a MultiCallbackHolder for the given holder, using the firstHolder if free or allocating
	// a new one.  No check for an existing record for holder is done.
	MultiCallbackHolder* addHolder(ThreadMultiCallback* multiCallback, int index) {
		if (firstHolder.multiCallback == nullptr) {
			firstHolder.multiCallback = multiCallback;
			firstHolder.index = index;
			return &firstHolder;
		}
		firstHolder.next = new MultiCallbackHolder(multiCallback, index, &firstHolder, firstHolder.next);
		return firstHolder.next;
	}

	// Get the MultiCallbackHolder for holder if it exists, or nullptr.
	MultiCallbackHolder* getHolder(ThreadMultiCallback* multiCallback) {
		MultiCallbackHolder* h = &firstHolder;
		while (h != nullptr && h->multiCallback != multiCallback) {
			h = h->next;
		}
		return h;
	}

	// Destroy the given MultiCallbackHolder, freeing it if it is not firstHolder.
	void destroyHolder(MultiCallbackHolder* h) {
		UNSTOPPABLE_ASSERT(h != nullptr);

		// If h is the firstHolder just clear its holder pointer to indicate unusedness
		if (h == &firstHolder) {
			h->multiCallback = nullptr;
		} else {
			// Otherwise unlink h from the doubly linked list and free it
			// h->previous is definitely valid
			h->previous->next = h->next;
			if (h->next) {
				h->next->previous = h->previous;
			}
			delete h;
		}
	}
};

class ThreadMultiCallback final : public ThreadCallback, public FastAllocated<ThreadMultiCallback> {
public:
	ThreadMultiCallback() {}

	ThreadCallback* addCallback(ThreadCallback* callback) override {
		// May be triggered by a waitForAll on a vector with the same future in it more than once
		UNSTOPPABLE_ASSERT(callback->getHolder(this) == nullptr);

		callback->addHolder(this, callbacks.size());
		callbacks.push_back(callback);
		return (ThreadCallback*)this;
	}

	bool clearCallback(ThreadCallback* callback) override {
		MultiCallbackHolder* h = callback->getHolder(this);
		if (h == nullptr) {
			return false;
		}

		UNSTOPPABLE_ASSERT(h->index < callbacks.size() && h->index >= 0);

		// Swap callback with last callback if it isn't the last
		if (h->index != callbacks.size() - 1) {
			callbacks[h->index] = callbacks.back();
			// Update the index of the Holder entry for the moved callback
			callbacks[h->index]->getHolder(this)->index = h->index;
		}

		callbacks.pop_back();
		callback->destroyHolder(h);
		return true;
	}

	bool canFire(int notMadeActive) const override { return true; }

	void fire(const Void& value, int& loopDepth) override {
		if (callbacks.size() > 10000)
			TraceEvent(SevWarn, "LargeMultiCallback").detail("CallbacksSize", callbacks.size());

		UNSTOPPABLE_ASSERT(loopDepth == 0);

		while (callbacks.size()) {
			auto cb = callbacks.back();
			callbacks.pop_back();
			cb->destroyHolder(cb->getHolder(this));
			if (cb->canFire(0)) {
				int ld = 0;
				cb->fire(value, ld);
			}
		}
	}

	void error(const Error& err, int& loopDepth) override {
		if (callbacks.size() > 10000)
			TraceEvent(SevWarn, "LargeMultiCallback").detail("CallbacksSize", callbacks.size());

		UNSTOPPABLE_ASSERT(loopDepth == 0);

		while (callbacks.size()) {
			auto cb = callbacks.back();
			callbacks.pop_back();
			cb->destroyHolder(cb->getHolder(this));
			if (cb->canFire(0)) {
				int ld = 0;
				cb->error(err, ld);
			}
		}
	}

	void destroy() override {
		// This assert assures that all ThreadMultiCallbacks remove themselves as a holder from
		// every ThreadCallback they hold prior to destruction, because if they do not then this
		// assert will fire, so ThreadCallback does not attempt to destroy its MultiCallbackHolder
		// linked list or verify that it is empty.
		UNSTOPPABLE_ASSERT(callbacks.empty());
		delete this;
	}

	bool isMultiCallback() const override { return true; }

private:
	std::vector<ThreadCallback*> callbacks;
};

struct SetCallbackResult {
	enum Result { FIRED, CANNOT_FIRE, CALLBACK_SET };
};

class ThreadSingleAssignmentVarBase {
public:
	enum Status { Unset, NeverSet, Set, ErrorSet }; // order is important
	// volatile long referenceCount;
	ThreadSpinLock mutex;
	std::atomic<Status> status;
	Error error;
	ThreadCallback* callback;

	bool isReady() {
		ThreadSpinLockHolder holder(mutex);
		return isReadyUnsafe();
	}

	bool isError() {
		ThreadSpinLockHolder holder(mutex);
		return isErrorUnsafe();
	}

	int getErrorCode() {
		ThreadSpinLockHolder holder(mutex);
		if (!isReadyUnsafe())
			return error_code_future_not_set;
		if (!isErrorUnsafe())
			return error_code_success;
		return error.code();
	}

	bool canBeSet() {
		ThreadSpinLockHolder holder(mutex);
		return canBeSetUnsafe();
	}

	class BlockCallback : public ThreadCallback {
	public:
		Event ev;

		BlockCallback(ThreadSingleAssignmentVarBase& sav) {
			int ignore = 0;
			sav.callOrSetAsCallback(this, ignore, 0);
			ev.block();
		}

		bool canFire(int notMadeActive) const override { return true; }
		void fire(const Void& unused, int& userParam) override { ev.set(); }
		void error(const Error&, int& userParam) override { ev.set(); }
	};

	void blockUntilReady() {
		if (!isReady()) {
			BlockCallback cb(*this);
		}
	}

	void blockUntilReadyCheckOnMainThread() {
		if (!isReady()) {
			if (g_network->isOnMainThread()) {
				throw blocked_from_network_thread();
			}
			BlockCallback cb(*this);
		}
	}

	ThreadSingleAssignmentVarBase() : status(Unset), callback(NULL), valueReferenceCount(0) {} //, referenceCount(1) {}
	~ThreadSingleAssignmentVarBase() {
		this->mutex.assertNotEntered();

		if (callback)
			callback->destroy();
	}

	virtual void addref() = 0;
	virtual void delref() = 0;

	void send(Never) {
		if (TRACE_SAMPLE())
			TraceEvent(SevSample, "Promise_sendNever").log();
		ThreadSpinLockHolder holder(mutex);
		if (!canBeSetUnsafe())
			ASSERT(false); // Promise fulfilled twice
		this->status = NeverSet;
	}

	// Sends an error through the assignment var if it is not already set. Otherwise does nothing.
	// Returns true if the assignment var was not already set; otherwise returns false.
	bool trySendError(const Error& err) {
		if (TRACE_SAMPLE())
			TraceEvent(SevSample, "Promise_sendError").detail("ErrorCode", err.code());
		this->mutex.enter();
		if (!canBeSetUnsafe()) {
			this->mutex.leave();
			return false;
		}
		error = err;
		status = ErrorSet;
		if (!callback) {
			this->mutex.leave();
			return true;
		}
		auto func = callback;
		if (!callback->isMultiCallback())
			callback = nullptr;

		if (!func->canFire(0)) {
			this->mutex.leave();
		} else {
			this->mutex.leave();

			// Thread safe because status is now ErrorSet and callback is nullptr, meaning than callback cannot change
			int userParam = 0;
			func->error(err, userParam);
		}

		return true;
	}

	// Like trySendError, except that it is assumed the assignment var is not already set.
	void sendError(const Error& err) { ASSERT(trySendError(err)); }

	SetCallbackResult::Result callOrSetAsCallback(ThreadCallback* callback, int& userParam1, int notMadeActive) {
		this->mutex.enter();
		if (isReadyUnsafe()) {
			if (callback->canFire(notMadeActive)) {
				this->mutex.leave();

				// Thread safe because the Future is ready, meaning that status and this->error will not change
				if (status == ErrorSet) {
					auto error = this->error; // Since callback might free this
					callback->error(error, userParam1);
				} else {
					callback->fire(Void(), userParam1);
				}

				return SetCallbackResult::FIRED;
			} else {
				this->mutex.leave();
				return SetCallbackResult::CANNOT_FIRE;
			}
		} else {
			if (this->callback)
				this->callback = this->callback->addCallback(callback);
			else
				this->callback = callback;

			this->mutex.leave();
			return SetCallbackResult::CALLBACK_SET;
		}
	}

	// If this function returns false, then this SAV has already been set and the callback has been or will be called.
	// If this function returns true, then the callback has not and will not be called by this SAV (unless it is set
	// later). This doesn't clear callbacks that are nested multiple levels inside of multi-callbacks
	bool clearCallback(ThreadCallback* cb) {
		this->mutex.enter();

		// If another thread is calling fire in send/sendError, it would be unsafe to clear the callback
		if (isReadyUnsafe()) {
			this->mutex.leave();
			return false;
		}

		// Only clear the callback if it belongs to the caller, because
		// another actor could be waiting on it now!
		bool cleared = false;
		if (callback == cb) {
			callback = nullptr;
			cleared = true;
		} else if (callback != nullptr) {
			cleared = callback->clearCallback(cb);
		}

		this->mutex.leave();
		return cleared;
	}

	void setCancel(Future<Void>&& cf) { cancelFuture = std::move(cf); }

	virtual void cancel() {
		// Cancels the action and decrements the reference count by 1. The if statement is just an optimization. It's ok
		// if we take the "wrong path" if we call this while someone else holds |mutex|. We can't take |mutex| since
		// this is called from releaseMemory. Trying to avoid going to the network thread here is important - without
		// this we see lower throughput on the client for e.g. GRV workloads.
		if (isReadyUnsafe()) {
			delref();
		} else {
			onMainThreadVoid([this]() {
				this->cancelFuture.cancel();
				this->delref();
			});
		}
	}

	void releaseMemory() {
		ThreadSpinLockHolder holder(mutex);
		if (--valueReferenceCount == 0)
			cleanupUnsafe();
	}

private:
	Future<Void> cancelFuture;
	int32_t valueReferenceCount;

protected:
	// The caller of any of these *Unsafe functions should be holding |mutex|
	//
	// |status| is an atomic, so these are not unsafe in the "data race"
	// sense. It appears that there are some class invariants (e.g. that
	// callback should be null if the future is ready), so we should still
	// hold |mutex| when calling these functions. One exception is for
	// cancel, which mustn't try to acquire |mutex| since it's called from
	// releaseMemory while holding the |mutex|. In cancel, we only need to
	// know if there's possibly work to cancel on the main thread, so it's safe to
	// call without holding |mutex|.
	//
	// A bit of history: the original implementation of cancel was not
	// thread safe (in practice it behaved as intended, but TSAN didn't like
	// it, and it was definitely a data race.) The first attempt to fix this[1]
	// was simply to cancel on the main thread, but this turns out to cause
	// a performance regression on the client. Now we simply make |status|
	// atomic so that it behaves (legally) how the original author intended.
	//
	// [1]: https://github.com/apple/foundationdb/pull/3750
	bool isReadyUnsafe() const { return status >= Set; }
	bool isErrorUnsafe() const { return status == ErrorSet; }
	bool canBeSetUnsafe() const { return status == Unset; }

	void addValueReferenceUnsafe() { ++valueReferenceCount; }

	virtual void cleanupUnsafe() {
		if (status != ErrorSet) {
			error = future_released();
			status = ErrorSet;
		}

		valueReferenceCount = 0;
		this->addref();
		cancel();
	}
};

template <class T>
class ThreadSingleAssignmentVar
  : public ThreadSingleAssignmentVarBase,
    /* public FastAllocated<ThreadSingleAssignmentVar<T>>,*/ public ThreadSafeReferenceCounted<
        ThreadSingleAssignmentVar<T>> {
public:
	virtual ~ThreadSingleAssignmentVar() {}

	T value;

	T get() {
		ThreadSpinLockHolder holder(mutex);
		if (!isReadyUnsafe())
			throw future_not_set();
		if (isErrorUnsafe())
			throw error;

		addValueReferenceUnsafe();
		return value;
	}

	void addref() override { ThreadSafeReferenceCounted<ThreadSingleAssignmentVar<T>>::addref(); }

	void delref() override { ThreadSafeReferenceCounted<ThreadSingleAssignmentVar<T>>::delref(); }

	void send(const T& value) {
		if (TRACE_SAMPLE())
			TraceEvent(SevSample, "Promise_send").log();
		this->mutex.enter();
		if (!canBeSetUnsafe()) {
			this->mutex.leave();
			ASSERT(false); // Promise fulfilled twice
		}
		this->value = value; //< Danger: polymorphic operation inside lock
		this->status = Set;
		if (!callback) {
			this->mutex.leave();
			return;
		}

		auto func = callback;
		if (!callback->isMultiCallback())
			callback = nullptr;

		if (!func->canFire(0)) {
			this->mutex.leave();
		} else {
			this->mutex.leave();

			// Thread safe because status is now Set and callback is nullptr, meaning than callback cannot change
			int userParam = 0;
			func->fire(Void(), userParam);
		}
	}

	void cleanupUnsafe() override {
		value = T();
		ThreadSingleAssignmentVarBase::cleanupUnsafe();
	}
};

template <class T>
class ThreadFuture {
public:
	T get() { return sav->get(); }
	T getBlocking() {
		sav->blockUntilReady();
		return sav->get();
	}

	void blockUntilReady() { sav->blockUntilReady(); }

	void blockUntilReadyCheckOnMainThread() { sav->blockUntilReadyCheckOnMainThread(); }

	bool isValid() const { return sav != 0; }
	bool isReady() { return sav->isReady(); }
	bool isError() { return sav->isError(); }
	Error& getError() {
		if (!isError())
			throw future_not_error();

		return sav->error;
	}

	SetCallbackResult::Result callOrSetAsCallback(ThreadCallback* callback, int& userParam1, int notMadeActive) {
		return sav->callOrSetAsCallback(callback, userParam1, notMadeActive);
	}
	bool clearCallback(ThreadCallback* cb) { return sav->clearCallback(cb); }

	void cancel() { extractPtr()->cancel(); }

	ThreadFuture() : sav(0) {}
	explicit ThreadFuture(ThreadSingleAssignmentVar<T>* sav) : sav(sav) {
		// sav->addref();
	}
	ThreadFuture(const ThreadFuture<T>& rhs) : sav(rhs.sav) {
		if (sav)
			sav->addref();
	}
	ThreadFuture(ThreadFuture<T>&& rhs) noexcept : sav(rhs.sav) { rhs.sav = 0; }
	ThreadFuture(const T& presentValue) : sav(new ThreadSingleAssignmentVar<T>()) { sav->send(presentValue); }
	ThreadFuture(Never) : sav(new ThreadSingleAssignmentVar<T>()) {}
	ThreadFuture(const Error& error) : sav(new ThreadSingleAssignmentVar<T>()) { sav->sendError(error); }
	ThreadFuture(const ErrorOr<T>& errorOr) : sav(new ThreadSingleAssignmentVar<T>()) {
		if (errorOr.isError()) {
			sav->sendError(errorOr.getError());
		} else {
			sav->send(errorOr.get());
		}
	}
	~ThreadFuture() {
		if (sav)
			sav->delref();
	}
	void operator=(const ThreadFuture<T>& rhs) {
		if (rhs.sav)
			rhs.sav->addref();
		if (sav)
			sav->delref();
		sav = rhs.sav;
	}
	void operator=(ThreadFuture<T>&& rhs) noexcept {
		if (sav != rhs.sav) {
			if (sav)
				sav->delref();
			sav = rhs.sav;
			rhs.sav = 0;
		}
	}
	bool operator==(const ThreadFuture& rhs) { return rhs.sav == sav; }
	bool operator!=(const ThreadFuture& rhs) { return rhs.sav != sav; }

	ThreadSingleAssignmentVarBase* getPtr() const { return sav; }
	ThreadSingleAssignmentVarBase* extractPtr() {
		auto* p = sav;
		sav = nullptr;
		return p;
	}

private:
	ThreadSingleAssignmentVar<T>* sav;
};

// A callback class used to convert a ThreadFuture into a Future
template <class T>
struct CompletionCallback final : public ThreadCallback, ReferenceCounted<CompletionCallback<T>> {
	// The thread future being waited on
	ThreadFuture<T> threadFuture;

	// The promise whose future we are triggering when this callback gets called
	Promise<T> promise;

	// Unused
	int userParam;

	// Holds own reference to prevent deletion until callback is fired
	Reference<CompletionCallback<T>> self;

	CompletionCallback(ThreadFuture<T> threadFuture) { this->threadFuture = threadFuture; }

	bool canFire(int notMadeActive) const override { return true; }

	// Trigger the promise
	void fire(const Void& unused, int& userParam) override {
		promise.send(threadFuture.get());
		self.clear();
	}

	// Send the error through the promise
	void error(const Error& e, int& userParam) override {
		promise.sendError(e);
		self.clear();
	}
};

// Converts a ThreadFuture into a Future
// WARNING: This is not actually thread safe!  It can only be safely used from the main thread, on futures which are
// being set on the main thread
// FIXME: does not support cancellation
template <class T>
Future<T> unsafeThreadFutureToFuture(ThreadFuture<T> threadFuture) {
	auto callback = makeReference<CompletionCallback<T>>(threadFuture);
	callback->self = callback;
	threadFuture.callOrSetAsCallback(callback.getPtr(), callback->userParam, 0);
	return callback->promise.getFuture();
}

// A callback waiting on a thread future and will delete itself once fired
template <class T>
struct UtilCallback final : public ThreadCallback {
public:
	UtilCallback(ThreadFuture<T> f, void* userdata) : f(f), userdata(userdata) {}

	bool canFire(int notMadeActive) const override { return true; }
	void fire(const Void& unused, int& userParam) override {
		g_network->onMainThread(Promise<Void>((SAV<Void>*)userdata), TaskPriority::DefaultOnMainThread);
		delete this;
	}
	void error(const Error&, int& userParam) override {
		g_network->onMainThread(Promise<Void>((SAV<Void>*)userdata), TaskPriority::DefaultOnMainThread);
		delete this;
	}
	void destroy() override {}

private:
	ThreadFuture<T> f;
	void* userdata;
};

// The underlying actor that converts ThreadFuture to Future
// Note: should be used from main thread
// The cancellation here works both way
// If the underlying "threadFuture" is cancelled, this actor will get actor_cancelled.
// If instead, this actor is cancelled, we will also cancel the underlying "threadFuture"
// Note: we are required to have unique ownership of the "threadFuture"
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
namespace {
// This generated class is to be used only via safeThreadFutureToFutureImpl()
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class T, class SafeThreadFutureToFutureImplActor>
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class SafeThreadFutureToFutureImplActorState {
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	SafeThreadFutureToFutureImplActorState(ThreadFuture<T> const& threadFuture) 
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		 : threadFuture(threadFuture)
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
	{
		fdb_probe_actor_create("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this));

	}
	~SafeThreadFutureToFutureImplActorState() 
	{
		fdb_probe_actor_destroy("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			Promise<Void> ready;
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			Future<Void> onReady = ready.getFuture();
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			UtilCallback<T>* callback = new UtilCallback<T>(threadFuture, ready.extractRawPointer());
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			int unused = 0;
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			threadFuture.callOrSetAsCallback(callback, unused, 0);
															#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			try {
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				StrictFuture<Void> __when_expr_0 = onReady;
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				if (static_cast<SafeThreadFutureToFutureImplActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<SafeThreadFutureToFutureImplActor*>(this)->actor_wait_state = 1;
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SafeThreadFutureToFutureImplActor, 0, Void >*>(static_cast<SafeThreadFutureToFutureImplActor*>(this)));
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		this->~SafeThreadFutureToFutureImplActorState();
		static_cast<SafeThreadFutureToFutureImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		ASSERT(threadFuture.isReady());
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (threadFuture.isError())
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		{
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			return a_body1Catch1(threadFuture.getError(), loopDepth);
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		}
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (!static_cast<SafeThreadFutureToFutureImplActor*>(this)->SAV<T>::futures) { (void)(threadFuture.get()); this->~SafeThreadFutureToFutureImplActorState(); static_cast<SafeThreadFutureToFutureImplActor*>(this)->destroy(); return 0; }
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		new (&static_cast<SafeThreadFutureToFutureImplActor*>(this)->SAV< T >::value()) T(threadFuture.get());
		this->~SafeThreadFutureToFutureImplActorState();
		static_cast<SafeThreadFutureToFutureImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			ASSERT(e.code() == error_code_operation_cancelled || e.code() == error_code_broken_promise);
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (e.code() == error_code_operation_cancelled)
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			{
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				threadFuture.cancel();
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			}
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		if (static_cast<SafeThreadFutureToFutureImplActor*>(this)->actor_wait_state > 0) static_cast<SafeThreadFutureToFutureImplActor*>(this)->actor_wait_state = 0;
		static_cast<SafeThreadFutureToFutureImplActor*>(this)->ActorCallback< SafeThreadFutureToFutureImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SafeThreadFutureToFutureImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SafeThreadFutureToFutureImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SafeThreadFutureToFutureImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), 0);

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
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	ThreadFuture<T> threadFuture;
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
};
// This generated class is to be used only via safeThreadFutureToFutureImpl()
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class T>
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class SafeThreadFutureToFutureImplActor final : public Actor<T>, public ActorCallback< SafeThreadFutureToFutureImplActor<T>, 0, Void >, public FastAllocated<SafeThreadFutureToFutureImplActor<T>>, public SafeThreadFutureToFutureImplActorState<T, SafeThreadFutureToFutureImplActor<T>> {
															#line 1426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
	using FastAllocated<SafeThreadFutureToFutureImplActor<T>>::operator new;
	using FastAllocated<SafeThreadFutureToFutureImplActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SafeThreadFutureToFutureImplActor<T>, 0, Void >;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	SafeThreadFutureToFutureImplActor(ThreadFuture<T> const& threadFuture) 
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		 : Actor<T>(),
		   SafeThreadFutureToFutureImplActorState<T, SafeThreadFutureToFutureImplActor<T>>(threadFuture)
	{
		fdb_probe_actor_enter("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("safeThreadFutureToFutureImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("safeThreadFutureToFutureImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SafeThreadFutureToFutureImplActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class T>
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
[[nodiscard]] Future<T> safeThreadFutureToFutureImpl( ThreadFuture<T> const& threadFuture ) {
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	return Future<T>(new SafeThreadFutureToFutureImplActor<T>(threadFuture));
															#line 1467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
}

#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"

// The removeArenaFromStandalone() actors simulate the behavior of DLApi. In this case,
// the memory is not owned by the Standalone. If the `future` goes out of scope, subsequent
// access to the memory via the returned standalone will be invalid.
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
namespace {
// This generated class is to be used only via removeArenaFromStandalone()
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <typename T, class RemoveArenaFromStandaloneActor>
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class RemoveArenaFromStandaloneActorState {
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	RemoveArenaFromStandaloneActorState(Future<Standalone<T>> const& future) 
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		 : future(future)
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
	{
		fdb_probe_actor_create("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this));

	}
	~RemoveArenaFromStandaloneActorState() 
	{
		fdb_probe_actor_destroy("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			StrictFuture<Standalone<T>> __when_expr_0 = future;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (static_cast<RemoveArenaFromStandaloneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RemoveArenaFromStandaloneActor*>(this)->actor_wait_state = 1;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RemoveArenaFromStandaloneActor, 0, Standalone<T> >*>(static_cast<RemoveArenaFromStandaloneActor*>(this)));
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		this->~RemoveArenaFromStandaloneActorState();
		static_cast<RemoveArenaFromStandaloneActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Standalone<T> const& _,int loopDepth) 
	{
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (!static_cast<RemoveArenaFromStandaloneActor*>(this)->SAV<Standalone<T>>::futures) { (void)(Standalone<T>(future.get(), Arena())); this->~RemoveArenaFromStandaloneActorState(); static_cast<RemoveArenaFromStandaloneActor*>(this)->destroy(); return 0; }
															#line 1534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		new (&static_cast<RemoveArenaFromStandaloneActor*>(this)->SAV< Standalone<T> >::value()) Standalone<T>(Standalone<T>(future.get(), Arena()));
		this->~RemoveArenaFromStandaloneActorState();
		static_cast<RemoveArenaFromStandaloneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Standalone<T> && _,int loopDepth) 
	{
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (!static_cast<RemoveArenaFromStandaloneActor*>(this)->SAV<Standalone<T>>::futures) { (void)(Standalone<T>(future.get(), Arena())); this->~RemoveArenaFromStandaloneActorState(); static_cast<RemoveArenaFromStandaloneActor*>(this)->destroy(); return 0; }
															#line 1546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		new (&static_cast<RemoveArenaFromStandaloneActor*>(this)->SAV< Standalone<T> >::value()) Standalone<T>(Standalone<T>(future.get(), Arena()));
		this->~RemoveArenaFromStandaloneActorState();
		static_cast<RemoveArenaFromStandaloneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Standalone<T> const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Standalone<T> && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RemoveArenaFromStandaloneActor*>(this)->actor_wait_state > 0) static_cast<RemoveArenaFromStandaloneActor*>(this)->actor_wait_state = 0;
		static_cast<RemoveArenaFromStandaloneActor*>(this)->ActorCallback< RemoveArenaFromStandaloneActor, 0, Standalone<T> >::remove();

	}
	void a_callback_fire(ActorCallback< RemoveArenaFromStandaloneActor, 0, Standalone<T> >*,Standalone<T> const& value) 
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RemoveArenaFromStandaloneActor, 0, Standalone<T> >*,Standalone<T> && value) 
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RemoveArenaFromStandaloneActor, 0, Standalone<T> >*,Error err) 
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	Future<Standalone<T>> future;
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
};
// This generated class is to be used only via removeArenaFromStandalone()
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <typename T>
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class RemoveArenaFromStandaloneActor final : public Actor<Standalone<T>>, public ActorCallback< RemoveArenaFromStandaloneActor<T>, 0, Standalone<T> >, public FastAllocated<RemoveArenaFromStandaloneActor<T>>, public RemoveArenaFromStandaloneActorState<T, RemoveArenaFromStandaloneActor<T>> {
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
	using FastAllocated<RemoveArenaFromStandaloneActor<T>>::operator new;
	using FastAllocated<RemoveArenaFromStandaloneActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RemoveArenaFromStandaloneActor<T>, 0, Standalone<T> >;
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	RemoveArenaFromStandaloneActor(Future<Standalone<T>> const& future) 
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		 : Actor<Standalone<T>>(),
		   RemoveArenaFromStandaloneActorState<T, RemoveArenaFromStandaloneActor<T>>(future)
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("removeArenaFromStandalone");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RemoveArenaFromStandaloneActor<T>, 0, Standalone<T> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <typename T>
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
[[nodiscard]] Future<Standalone<T>> removeArenaFromStandalone( Future<Standalone<T>> const& future ) {
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	return Future<Standalone<T>>(new RemoveArenaFromStandaloneActor<T>(future));
															#line 1667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
}

#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"

															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
namespace {
// This generated class is to be used only via removeArenaFromStandalone()
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <typename T, class RemoveArenaFromStandaloneActor1>
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class RemoveArenaFromStandaloneActor1State {
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	RemoveArenaFromStandaloneActor1State(Future<Optional<Standalone<T>>> const& future) 
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		 : future(future)
															#line 1686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
	{
		fdb_probe_actor_create("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this));

	}
	~RemoveArenaFromStandaloneActor1State() 
	{
		fdb_probe_actor_destroy("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			StrictFuture<Optional<Standalone<T>>> __when_expr_0 = future;
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (static_cast<RemoveArenaFromStandaloneActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RemoveArenaFromStandaloneActor1*>(this)->actor_wait_state = 1;
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RemoveArenaFromStandaloneActor1, 0, Optional<Standalone<T>> >*>(static_cast<RemoveArenaFromStandaloneActor1*>(this)));
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		this->~RemoveArenaFromStandaloneActor1State();
		static_cast<RemoveArenaFromStandaloneActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Standalone<T>> const& val,int loopDepth) 
	{
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (val.present())
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		{
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (!static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV<Optional<Standalone<T>>>::futures) { (void)(Standalone<T>(future.get().get(), Arena())); this->~RemoveArenaFromStandaloneActor1State(); static_cast<RemoveArenaFromStandaloneActor1*>(this)->destroy(); return 0; }
															#line 1735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			new (&static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV< Optional<Standalone<T>> >::value()) Optional<Standalone<T>>(Standalone<T>(future.get().get(), Arena()));
			this->~RemoveArenaFromStandaloneActor1State();
			static_cast<RemoveArenaFromStandaloneActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (!static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV<Optional<Standalone<T>>>::futures) { (void)(Optional<Standalone<T>>()); this->~RemoveArenaFromStandaloneActor1State(); static_cast<RemoveArenaFromStandaloneActor1*>(this)->destroy(); return 0; }
															#line 1745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			new (&static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV< Optional<Standalone<T>> >::value()) Optional<Standalone<T>>(Optional<Standalone<T>>());
			this->~RemoveArenaFromStandaloneActor1State();
			static_cast<RemoveArenaFromStandaloneActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Optional<Standalone<T>> && val,int loopDepth) 
	{
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (val.present())
															#line 1758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		{
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (!static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV<Optional<Standalone<T>>>::futures) { (void)(Standalone<T>(future.get().get(), Arena())); this->~RemoveArenaFromStandaloneActor1State(); static_cast<RemoveArenaFromStandaloneActor1*>(this)->destroy(); return 0; }
															#line 1762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			new (&static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV< Optional<Standalone<T>> >::value()) Optional<Standalone<T>>(Standalone<T>(future.get().get(), Arena()));
			this->~RemoveArenaFromStandaloneActor1State();
			static_cast<RemoveArenaFromStandaloneActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (!static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV<Optional<Standalone<T>>>::futures) { (void)(Optional<Standalone<T>>()); this->~RemoveArenaFromStandaloneActor1State(); static_cast<RemoveArenaFromStandaloneActor1*>(this)->destroy(); return 0; }
															#line 1772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			new (&static_cast<RemoveArenaFromStandaloneActor1*>(this)->SAV< Optional<Standalone<T>> >::value()) Optional<Standalone<T>>(Optional<Standalone<T>>());
			this->~RemoveArenaFromStandaloneActor1State();
			static_cast<RemoveArenaFromStandaloneActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1when1(Optional<Standalone<T>> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Standalone<T>> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RemoveArenaFromStandaloneActor1*>(this)->actor_wait_state > 0) static_cast<RemoveArenaFromStandaloneActor1*>(this)->actor_wait_state = 0;
		static_cast<RemoveArenaFromStandaloneActor1*>(this)->ActorCallback< RemoveArenaFromStandaloneActor1, 0, Optional<Standalone<T>> >::remove();

	}
	void a_callback_fire(ActorCallback< RemoveArenaFromStandaloneActor1, 0, Optional<Standalone<T>> >*,Optional<Standalone<T>> const& value) 
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RemoveArenaFromStandaloneActor1, 0, Optional<Standalone<T>> >*,Optional<Standalone<T>> && value) 
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RemoveArenaFromStandaloneActor1, 0, Optional<Standalone<T>> >*,Error err) 
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	Future<Optional<Standalone<T>>> future;
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
};
// This generated class is to be used only via removeArenaFromStandalone()
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <typename T>
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class RemoveArenaFromStandaloneActor1 final : public Actor<Optional<Standalone<T>>>, public ActorCallback< RemoveArenaFromStandaloneActor1<T>, 0, Optional<Standalone<T>> >, public FastAllocated<RemoveArenaFromStandaloneActor1<T>>, public RemoveArenaFromStandaloneActor1State<T, RemoveArenaFromStandaloneActor1<T>> {
															#line 1853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
	using FastAllocated<RemoveArenaFromStandaloneActor1<T>>::operator new;
	using FastAllocated<RemoveArenaFromStandaloneActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Standalone<T>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RemoveArenaFromStandaloneActor1<T>, 0, Optional<Standalone<T>> >;
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	RemoveArenaFromStandaloneActor1(Future<Optional<Standalone<T>>> const& future) 
															#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		 : Actor<Optional<Standalone<T>>>(),
		   RemoveArenaFromStandaloneActor1State<T, RemoveArenaFromStandaloneActor1<T>>(future)
	{
		fdb_probe_actor_enter("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("removeArenaFromStandalone");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("removeArenaFromStandalone", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RemoveArenaFromStandaloneActor1<T>, 0, Optional<Standalone<T>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <typename T>
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
[[nodiscard]] Future<Optional<Standalone<T>>> removeArenaFromStandalone( Future<Optional<Standalone<T>>> const& future ) {
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	return Future<Optional<Standalone<T>>>(new RemoveArenaFromStandaloneActor1<T>(future));
															#line 1894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
}

#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"

// The allow_anonymous_future type is used to prevent misuse of ThreadFutures.
// For Standalone types, the memory in some cases is actually stored in the ThreadFuture object,
// in which case we expect the caller to keep that ThreadFuture around until the result is no
// longer needed.
//
// We can provide some compile-time detection of this misuse by disallowing anonymous thread futures
// being passed in for certain types.
template <typename T>
struct allow_anonymous_future : std::true_type {};

template <typename T>
struct allow_anonymous_future<Standalone<T>> : std::false_type {};

template <typename T>
struct allow_anonymous_future<Optional<Standalone<T>>> : std::false_type {};

template <class T>
typename std::enable_if<allow_anonymous_future<T>::value, Future<T>>::type safeThreadFutureToFuture(
    const ThreadFuture<T>& threadFuture) {
	return safeThreadFutureToFutureImpl(threadFuture);
}

template <class T>
typename std::enable_if<!allow_anonymous_future<T>::value, Future<T>>::type safeThreadFutureToFuture(
    ThreadFuture<T>& threadFuture) {
	Future<T> f = safeThreadFutureToFutureImpl(threadFuture);
	if (BUGGIFY) {
		return removeArenaFromStandalone(f);
	}
	return f;
}

template <class T>
typename std::enable_if<allow_anonymous_future<T>::value, Future<T>>::type safeThreadFutureToFuture(
    const Future<T>& future) {
	// Do nothing
	return future;
}

template <class T>
typename std::enable_if<!allow_anonymous_future<T>::value, Future<T>>::type safeThreadFutureToFuture(
    Future<T>& future) {
	if (BUGGIFY) {
		return removeArenaFromStandalone(future);
	}
	return future;
}

// Helper actor. Do not use directly!
namespace internal_thread_helper {

															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
// This generated class is to be used only via doOnMainThread()
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class R, class F, class DoOnMainThreadActor>
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class DoOnMainThreadActorState {
															#line 1956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	DoOnMainThreadActorState(Future<Void> const& signal,F const& f,ThreadSingleAssignmentVar<R>* const& result) 
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		 : signal(signal),
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		   f(f),
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		   result(result)
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
	{
		fdb_probe_actor_create("doOnMainThread", reinterpret_cast<unsigned long>(this));

	}
	~DoOnMainThreadActorState() 
	{
		fdb_probe_actor_destroy("doOnMainThread", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				StrictFuture<Void> __when_expr_0 = signal;
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				if (static_cast<DoOnMainThreadActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoOnMainThreadActor*>(this)->actor_wait_state = 1;
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoOnMainThreadActor, 0, Void >*>(static_cast<DoOnMainThreadActor*>(this)));
															#line 1990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		this->~DoOnMainThreadActorState();
		static_cast<DoOnMainThreadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		ThreadFuture<R> destroyResultAfterReturning( result);
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (!static_cast<DoOnMainThreadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoOnMainThreadActorState(); static_cast<DoOnMainThreadActor*>(this)->destroy(); return 0; }
															#line 2021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		new (&static_cast<DoOnMainThreadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoOnMainThreadActorState();
		static_cast<DoOnMainThreadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			if (!result->canBeSet())
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			{
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
				TraceEvent(SevError, "OnMainThreadSetTwice").errorUnsuppressed(e);
															#line 2038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
			}
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
			result->sendError(e);
															#line 2042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		StrictFuture<R> __when_expr_1 = f();
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (static_cast<DoOnMainThreadActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoOnMainThreadActor*>(this)->actor_wait_state = 2;
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoOnMainThreadActor, 1, R >*>(static_cast<DoOnMainThreadActor*>(this)));
															#line 2064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		StrictFuture<R> __when_expr_1 = f();
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		if (static_cast<DoOnMainThreadActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoOnMainThreadActor*>(this)->actor_wait_state = 2;
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoOnMainThreadActor, 1, R >*>(static_cast<DoOnMainThreadActor*>(this)));
															#line 2080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
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
		if (static_cast<DoOnMainThreadActor*>(this)->actor_wait_state > 0) static_cast<DoOnMainThreadActor*>(this)->actor_wait_state = 0;
		static_cast<DoOnMainThreadActor*>(this)->ActorCallback< DoOnMainThreadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoOnMainThreadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(R const& r,int loopDepth) 
	{
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		result->send(r);
															#line 2152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(R && r,int loopDepth) 
	{
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
		result->send(r);
															#line 2161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(R const& r,int loopDepth) 
	{
		loopDepth = a_body1cont3(r, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(R && r,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(r), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoOnMainThreadActor*>(this)->actor_wait_state > 0) static_cast<DoOnMainThreadActor*>(this)->actor_wait_state = 0;
		static_cast<DoOnMainThreadActor*>(this)->ActorCallback< DoOnMainThreadActor, 1, R >::remove();

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadActor, 1, R >*,R const& value) 
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoOnMainThreadActor, 1, R >*,R && value) 
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoOnMainThreadActor, 1, R >*,Error err) 
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
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
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	Future<Void> signal;
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	F f;
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	ThreadSingleAssignmentVar<R>* result;
															#line 2248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
};
// This generated class is to be used only via doOnMainThread()
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class R, class F>
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
class DoOnMainThreadActor final : public Actor<Void>, public ActorCallback< DoOnMainThreadActor<R, F>, 0, Void >, public ActorCallback< DoOnMainThreadActor<R, F>, 1, R >, public FastAllocated<DoOnMainThreadActor<R, F>>, public DoOnMainThreadActorState<R, F, DoOnMainThreadActor<R, F>> {
															#line 2255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
public:
	using FastAllocated<DoOnMainThreadActor<R, F>>::operator new;
	using FastAllocated<DoOnMainThreadActor<R, F>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoOnMainThreadActor<R, F>, 0, Void >;
friend struct ActorCallback< DoOnMainThreadActor<R, F>, 1, R >;
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	DoOnMainThreadActor(Future<Void> const& signal,F const& f,ThreadSingleAssignmentVar<R>* const& result) 
															#line 2267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
		 : Actor<Void>(),
		   DoOnMainThreadActorState<R, F, DoOnMainThreadActor<R, F>>(signal, f, result)
	{
		fdb_probe_actor_enter("doOnMainThread", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doOnMainThread");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doOnMainThread", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoOnMainThreadActor<R, F>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoOnMainThreadActor<R, F>, 1, R >*)0, actor_cancelled()); break;
		}

	}
};
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
template <class R, class F>
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
[[nodiscard]] Future<Void> doOnMainThread( Future<Void> const& signal, F const& f, ThreadSingleAssignmentVar<R>* const& result ) {
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"
	return Future<Void>(new DoOnMainThreadActor<R, F>(signal, f, result));
															#line 2297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.g.h"
}

#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/ThreadHelper.actor.h"

} // namespace internal_thread_helper

// `onMainThread` runs a functor returning a `Future` on the main thread, waits for the future, and sends either the
// value returned from the waited `Future` or an error through the `ThreadFuture` returned from the function call.
//
// A workaround for cases where your functor returns a non-`Future` value is to wrap the value in an immediately
// filled `Future`. In cases where the functor returns void, a workaround is to return a `Future<bool>(true)` that
// can be waited on.
//
// TODO: Add SFINAE overloads for functors returning void or a non-Future type.
template <class F>
ThreadFuture<decltype(std::declval<F>()().getValue())> onMainThread(F f) {
	Promise<Void> signal;
	auto returnValue = new ThreadSingleAssignmentVar<decltype(std::declval<F>()().getValue())>();
	returnValue->addref(); // For the ThreadFuture we return
	// TODO: Is this cancellation logic actually needed?
	Future<Void> cancelFuture = internal_thread_helper::doOnMainThread<decltype(std::declval<F>()().getValue()), F>(
	    signal.getFuture(), f, returnValue);
	returnValue->setCancel(std::move(cancelFuture));
	g_network->onMainThread(std::move(signal), TaskPriority::DefaultOnMainThread);
	return ThreadFuture<decltype(std::declval<F>()().getValue())>(returnValue);
}

template <class V>
class ThreadSafeAsyncVar : NonCopyable, public ThreadSafeReferenceCounted<ThreadSafeAsyncVar<V>> {
public:
	struct State {
		State(V value, ThreadFuture<Void> onChange) : value(value), onChange(onChange) {}

		V value;
		ThreadFuture<Void> onChange;
	};

	ThreadSafeAsyncVar() : value(), nextChange(new ThreadSingleAssignmentVar<Void>()) {}
	ThreadSafeAsyncVar(V const& v) : value(v), nextChange(new ThreadSingleAssignmentVar<Void>()) {}

	State get() {
		ThreadSpinLockHolder holder(lock);
		nextChange->addref();
		return State(value, ThreadFuture<Void>(nextChange.getPtr()));
	}

	void set(V const& v, bool triggerIfSame = false) {
		Reference<ThreadSingleAssignmentVar<Void>> trigger(new ThreadSingleAssignmentVar<Void>());

		lock.enter();
		bool changed = this->value != v;
		if (changed || triggerIfSame) {
			std::swap(this->nextChange, trigger);
			this->value = v;
		}
		lock.leave();

		if (changed || triggerIfSame) {
			trigger->send(Void());
		}
	}

private:
	V value;
	Reference<ThreadSingleAssignmentVar<Void>> nextChange;
	ThreadSpinLock lock;
};

// Like a future (very similar to ThreadFuture) but only for computations that already completed. Reuses the SAV's
// implementation for memory management error handling though. Essentially a future that's returned from a synchronous
// computation and guaranteed to be complete.

template <class T>
class ThreadResult {
public:
	T get() { return sav->get(); }

	bool isValid() const { return sav != 0; }
	bool isError() { return sav->isError(); }
	Error& getError() {
		if (!isError())
			throw future_not_error();

		return sav->error;
	}

	ThreadResult() : sav(0) {}
	explicit ThreadResult(ThreadSingleAssignmentVar<T>* sav) : sav(sav) {
		ASSERT(sav->isReady());
		// sav->addref();
	}
	ThreadResult(const ThreadResult<T>& rhs) : sav(rhs.sav) {
		if (sav)
			sav->addref();
	}
	ThreadResult(ThreadResult<T>&& rhs) noexcept : sav(rhs.sav) { rhs.sav = 0; }
	ThreadResult(const T& presentValue) : sav(new ThreadSingleAssignmentVar<T>()) { sav->send(presentValue); }
	ThreadResult(const Error& error) : sav(new ThreadSingleAssignmentVar<T>()) { sav->sendError(error); }
	ThreadResult(const ErrorOr<T> errorOr) : sav(new ThreadSingleAssignmentVar<T>()) {
		if (errorOr.isError()) {
			sav->sendError(errorOr.getError());
		} else {
			sav->send(errorOr.get());
		}
	}
	~ThreadResult() {
		if (sav)
			sav->delref();
	}
	void operator=(const ThreadFuture<T>& rhs) {
		if (rhs.sav)
			rhs.sav->addref();
		if (sav)
			sav->delref();
		sav = rhs.sav;
	}
	void operator=(ThreadFuture<T>&& rhs) noexcept {
		if (sav != rhs.sav) {
			if (sav)
				sav->delref();
			sav = rhs.sav;
			rhs.sav = 0;
		}
	}
	bool operator==(const ThreadResult& rhs) { return rhs.sav == sav; }
	bool operator!=(const ThreadResult& rhs) { return rhs.sav != sav; }

	ThreadSingleAssignmentVarBase* getPtr() const { return sav; }
	ThreadSingleAssignmentVarBase* extractPtr() {
		auto* p = sav;
		sav = nullptr;
		return p;
	}

private:
	ThreadSingleAssignmentVar<T>* sav;
};

#include "flow/unactorcompiler.h"
#endif
