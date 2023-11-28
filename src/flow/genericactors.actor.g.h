#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/flow/genericactors.actor.h"
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

#pragma once

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#include <utility>
#if defined(NO_INTELLISENSE) && !defined(FLOW_GENERICACTORS_ACTOR_G_H)
#define FLOW_GENERICACTORS_ACTOR_G_H
#include "flow/genericactors.actor.g.h"
#elif !defined(GENERICACTORS_ACTOR_H)
#define GENERICACTORS_ACTOR_H

#include <list>
#include <utility>

#include "flow/IndexedSet.h"
#include "flow/Knobs.h"
#include "flow/Util.h"
#include "flow/flow.h"

#include "flow/actorcompiler.h" // This must be the last #include.

#ifdef _MSC_VER
#pragma warning(disable : 4355) // 'this' : used in base member initializer list
#endif

															#line 48 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via traceAfter()
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X, class TraceAfterActor>
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TraceAfterActorState {
															#line 55 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TraceAfterActorState(Future<T> const& what,const char* const& type,const char* const& key,X const& value,bool const& traceErrors = false) 
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   type(type),
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   key(key),
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   value(value),
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   traceErrors(traceErrors)
															#line 70 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("traceAfter", reinterpret_cast<unsigned long>(this));

	}
	~TraceAfterActorState() 
	{
		fdb_probe_actor_destroy("traceAfter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 49 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = what;
															#line 49 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TraceAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 88 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TraceAfterActor*>(this)->actor_wait_state = 1;
															#line 49 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TraceAfterActor, 0, T >*>(static_cast<TraceAfterActor*>(this)));
															#line 93 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TraceAfterActorState();
		static_cast<TraceAfterActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 53 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (traceErrors)
															#line 123 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 54 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				TraceEvent(type).errorUnsuppressed(e).detail(key, value);
															#line 127 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 55 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 131 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& val,int loopDepth) 
	{
															#line 50 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		TraceEvent(type).detail(key, value);
															#line 51 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TraceAfterActor*>(this)->SAV<T>::futures) { (void)(val); this->~TraceAfterActorState(); static_cast<TraceAfterActor*>(this)->destroy(); return 0; }
															#line 147 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TraceAfterActor*>(this)->SAV< T >::value()) T(val);
		this->~TraceAfterActorState();
		static_cast<TraceAfterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && val,int loopDepth) 
	{
															#line 50 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		TraceEvent(type).detail(key, value);
															#line 51 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TraceAfterActor*>(this)->SAV<T>::futures) { (void)(val); this->~TraceAfterActorState(); static_cast<TraceAfterActor*>(this)->destroy(); return 0; }
															#line 161 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TraceAfterActor*>(this)->SAV< T >::value()) T(val);
		this->~TraceAfterActorState();
		static_cast<TraceAfterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont2(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TraceAfterActor*>(this)->actor_wait_state > 0) static_cast<TraceAfterActor*>(this)->actor_wait_state = 0;
		static_cast<TraceAfterActor*>(this)->ActorCallback< TraceAfterActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< TraceAfterActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("traceAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TraceAfterActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("traceAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TraceAfterActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("traceAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceAfter", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	const char* type;
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	const char* key;
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	X value;
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool traceErrors;
															#line 242 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via traceAfter()
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TraceAfterActor final : public Actor<T>, public ActorCallback< TraceAfterActor<T, X>, 0, T >, public FastAllocated<TraceAfterActor<T, X>>, public TraceAfterActorState<T, X, TraceAfterActor<T, X>> {
															#line 249 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TraceAfterActor<T, X>>::operator new;
	using FastAllocated<TraceAfterActor<T, X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TraceAfterActor<T, X>, 0, T >;
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TraceAfterActor(Future<T> const& what,const char* const& type,const char* const& key,X const& value,bool const& traceErrors = false) 
															#line 260 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TraceAfterActorState<T, X, TraceAfterActor<T, X>>(what, type, key, value, traceErrors)
	{
		fdb_probe_actor_enter("traceAfter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("traceAfter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("traceAfter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TraceAfterActor<T, X>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> traceAfter( Future<T> const& what, const char* const& type, const char* const& key, X const& value, bool const& traceErrors = false ) {
															#line 46 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TraceAfterActor<T, X>(what, type, key, value, traceErrors));
															#line 290 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 58 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via traceAfterCall()
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X, class TraceAfterCallActor>
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TraceAfterCallActorState {
															#line 302 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TraceAfterCallActorState(Future<T> const& what,const char* const& type,const char* const& key,X const& func,bool const& traceErrors = false) 
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   type(type),
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   key(key),
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   func(func),
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   traceErrors(traceErrors)
															#line 317 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("traceAfterCall", reinterpret_cast<unsigned long>(this));

	}
	~TraceAfterCallActorState() 
	{
		fdb_probe_actor_destroy("traceAfterCall", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 62 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = what;
															#line 62 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TraceAfterCallActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 335 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TraceAfterCallActor*>(this)->actor_wait_state = 1;
															#line 62 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TraceAfterCallActor, 0, T >*>(static_cast<TraceAfterCallActor*>(this)));
															#line 340 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TraceAfterCallActorState();
		static_cast<TraceAfterCallActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 70 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (traceErrors)
															#line 370 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 71 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				TraceEvent(type).errorUnsuppressed(e);
															#line 374 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 72 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 378 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		try {
															#line 64 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			TraceEvent(type).detail(key, func(val));
															#line 393 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = a_body1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont2Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont2Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(T const& __val,int loopDepth) 
	{
															#line 62 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		val = __val;
															#line 408 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && __val,int loopDepth) 
	{
		val = std::move(__val);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TraceAfterCallActor*>(this)->actor_wait_state > 0) static_cast<TraceAfterCallActor*>(this)->actor_wait_state = 0;
		static_cast<TraceAfterCallActor*>(this)->ActorCallback< TraceAfterCallActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< TraceAfterCallActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("traceAfterCall", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceAfterCall", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TraceAfterCallActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("traceAfterCall", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceAfterCall", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TraceAfterCallActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("traceAfterCall", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("traceAfterCall", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 68 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TraceAfterCallActor*>(this)->SAV<T>::futures) { (void)(val); this->~TraceAfterCallActorState(); static_cast<TraceAfterCallActor*>(this)->destroy(); return 0; }
															#line 475 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TraceAfterCallActor*>(this)->SAV< T >::value()) T(std::move(val)); // state_var_RVO
		this->~TraceAfterCallActorState();
		static_cast<TraceAfterCallActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 66 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			TraceEvent(SevError, "TraceAfterCallError").error(e);
															#line 488 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	const char* type;
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	const char* key;
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	X func;
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool traceErrors;
															#line 62 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T val;
															#line 524 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via traceAfterCall()
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TraceAfterCallActor final : public Actor<T>, public ActorCallback< TraceAfterCallActor<T, X>, 0, T >, public FastAllocated<TraceAfterCallActor<T, X>>, public TraceAfterCallActorState<T, X, TraceAfterCallActor<T, X>> {
															#line 531 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TraceAfterCallActor<T, X>>::operator new;
	using FastAllocated<TraceAfterCallActor<T, X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TraceAfterCallActor<T, X>, 0, T >;
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TraceAfterCallActor(Future<T> const& what,const char* const& type,const char* const& key,X const& func,bool const& traceErrors = false) 
															#line 542 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TraceAfterCallActorState<T, X, TraceAfterCallActor<T, X>>(what, type, key, func, traceErrors)
	{
		fdb_probe_actor_enter("traceAfterCall", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("traceAfterCall");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("traceAfterCall", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TraceAfterCallActor<T, X>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> traceAfterCall( Future<T> const& what, const char* const& type, const char* const& key, X const& func, bool const& traceErrors = false ) {
															#line 59 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TraceAfterCallActor<T, X>(what, type, key, func, traceErrors));
															#line 572 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 75 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 577 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via stopAfter()
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class StopAfterActor>
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class StopAfterActorState {
															#line 584 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	StopAfterActorState(Future<T> const& what) 
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 78 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   ret(T())
															#line 593 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("stopAfter", reinterpret_cast<unsigned long>(this));

	}
	~StopAfterActorState() 
	{
		fdb_probe_actor_destroy("stopAfter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 80 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = what;
															#line 80 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<StopAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 611 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<StopAfterActor*>(this)->actor_wait_state = 1;
															#line 80 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< StopAfterActor, 0, T >*>(static_cast<StopAfterActor*>(this)));
															#line 616 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~StopAfterActorState();
		static_cast<StopAfterActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 91 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		g_network->stop();
															#line 92 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<StopAfterActor*>(this)->SAV<Optional<T>>::futures) { (void)(ret); this->~StopAfterActorState(); static_cast<StopAfterActor*>(this)->destroy(); return 0; }
															#line 647 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<StopAfterActor*>(this)->SAV< Optional<T> >::value()) Optional<T>(std::move(ret)); // state_var_RVO
		this->~StopAfterActorState();
		static_cast<StopAfterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 83 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			bool ok = e.code() == error_code_please_reboot || e.code() == error_code_please_reboot_delete || e.code() == error_code_actor_cancelled || e.code() == error_code_please_reboot_remote_kv_store;
															#line 85 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			TraceEvent(ok ? SevInfo : SevError, "StopAfterError").error(e);
															#line 86 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!ok)
															#line 664 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 87 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				fprintf(stderr, "Fatal Error: %s\n", e.what());
															#line 88 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				ret = Optional<T>();
															#line 670 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& _,int loopDepth) 
	{
															#line 81 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ret = Optional<T>(_);
															#line 686 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && _,int loopDepth) 
	{
															#line 81 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ret = Optional<T>(_);
															#line 695 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<StopAfterActor*>(this)->actor_wait_state > 0) static_cast<StopAfterActor*>(this)->actor_wait_state = 0;
		static_cast<StopAfterActor*>(this)->ActorCallback< StopAfterActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< StopAfterActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("stopAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stopAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< StopAfterActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("stopAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stopAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< StopAfterActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("stopAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stopAfter", reinterpret_cast<unsigned long>(this), 0);

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
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 78 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Optional<T> ret;
															#line 780 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via stopAfter()
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class StopAfterActor final : public Actor<Optional<T>>, public ActorCallback< StopAfterActor<T>, 0, T >, public FastAllocated<StopAfterActor<T>>, public StopAfterActorState<T, StopAfterActor<T>> {
															#line 787 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<StopAfterActor<T>>::operator new;
	using FastAllocated<StopAfterActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< StopAfterActor<T>, 0, T >;
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	StopAfterActor(Future<T> const& what) 
															#line 798 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Optional<T>>(),
		   StopAfterActorState<T, StopAfterActor<T>>(what)
	{
		fdb_probe_actor_enter("stopAfter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("stopAfter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("stopAfter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< StopAfterActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Optional<T>> stopAfter( Future<T> const& what ) {
															#line 76 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Optional<T>>(new StopAfterActor<T>(what));
															#line 828 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 94 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class T>
T sorted(T range) {
	std::sort(range.begin(), range.end());
	return range;
}

template <class T>
ErrorOr<T> errorOr(T t) {
	return ErrorOr<T>(t);
}

															#line 844 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via errorOr()
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ErrorOrActor>
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ErrorOrActorState {
															#line 851 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ErrorOrActorState(Future<T> const& f) 
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f)
															#line 858 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("errorOr", reinterpret_cast<unsigned long>(this));

	}
	~ErrorOrActorState() 
	{
		fdb_probe_actor_destroy("errorOr", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 109 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = f;
															#line 109 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<ErrorOrActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 876 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ErrorOrActor*>(this)->actor_wait_state = 1;
															#line 109 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ErrorOrActor, 0, T >*>(static_cast<ErrorOrActor*>(this)));
															#line 881 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ErrorOrActorState();
		static_cast<ErrorOrActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 112 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!static_cast<ErrorOrActor*>(this)->SAV<ErrorOr<T>>::futures) { (void)(ErrorOr<T>(e)); this->~ErrorOrActorState(); static_cast<ErrorOrActor*>(this)->destroy(); return 0; }
															#line 911 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			new (&static_cast<ErrorOrActor*>(this)->SAV< ErrorOr<T> >::value()) ErrorOr<T>(ErrorOr<T>(e));
			this->~ErrorOrActorState();
			static_cast<ErrorOrActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
															#line 110 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ErrorOrActor*>(this)->SAV<ErrorOr<T>>::futures) { (void)(ErrorOr<T>(t)); this->~ErrorOrActorState(); static_cast<ErrorOrActor*>(this)->destroy(); return 0; }
															#line 929 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ErrorOrActor*>(this)->SAV< ErrorOr<T> >::value()) ErrorOr<T>(ErrorOr<T>(t));
		this->~ErrorOrActorState();
		static_cast<ErrorOrActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 110 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ErrorOrActor*>(this)->SAV<ErrorOr<T>>::futures) { (void)(ErrorOr<T>(t)); this->~ErrorOrActorState(); static_cast<ErrorOrActor*>(this)->destroy(); return 0; }
															#line 941 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ErrorOrActor*>(this)->SAV< ErrorOr<T> >::value()) ErrorOr<T>(ErrorOr<T>(t));
		this->~ErrorOrActorState();
		static_cast<ErrorOrActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ErrorOrActor*>(this)->actor_wait_state > 0) static_cast<ErrorOrActor*>(this)->actor_wait_state = 0;
		static_cast<ErrorOrActor*>(this)->ActorCallback< ErrorOrActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ErrorOrActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("errorOr", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("errorOr", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ErrorOrActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("errorOr", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("errorOr", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ErrorOrActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("errorOr", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("errorOr", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 1014 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via errorOr()
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ErrorOrActor final : public Actor<ErrorOr<T>>, public ActorCallback< ErrorOrActor<T>, 0, T >, public FastAllocated<ErrorOrActor<T>>, public ErrorOrActorState<T, ErrorOrActor<T>> {
															#line 1021 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ErrorOrActor<T>>::operator new;
	using FastAllocated<ErrorOrActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ErrorOr<T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ErrorOrActor<T>, 0, T >;
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ErrorOrActor(Future<T> const& f) 
															#line 1032 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<ErrorOr<T>>(),
		   ErrorOrActorState<T, ErrorOrActor<T>>(f)
	{
		fdb_probe_actor_enter("errorOr", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("errorOr");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("errorOr", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ErrorOrActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<ErrorOr<T>> errorOr( Future<T> const& f ) {
															#line 106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<ErrorOr<T>>(new ErrorOrActor<T>(f));
															#line 1062 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 115 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 1067 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via throwErrorOr()
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ThrowErrorOrActor>
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ThrowErrorOrActorState {
															#line 1074 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ThrowErrorOrActorState(Future<ErrorOr<T>> const& f) 
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f)
															#line 1081 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("throwErrorOr", reinterpret_cast<unsigned long>(this));

	}
	~ThrowErrorOrActorState() 
	{
		fdb_probe_actor_destroy("throwErrorOr", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 118 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<ErrorOr<T>> __when_expr_0 = f;
															#line 118 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<ThrowErrorOrActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1098 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ThrowErrorOrActor*>(this)->actor_wait_state = 1;
															#line 118 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ThrowErrorOrActor, 0, ErrorOr<T> >*>(static_cast<ThrowErrorOrActor*>(this)));
															#line 1103 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ThrowErrorOrActorState();
		static_cast<ThrowErrorOrActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(ErrorOr<T> const& t,int loopDepth) 
	{
															#line 119 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (t.isError())
															#line 1126 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 120 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(t.getError(), loopDepth);
															#line 1130 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 121 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ThrowErrorOrActor*>(this)->SAV<T>::futures) { (void)(t.get()); this->~ThrowErrorOrActorState(); static_cast<ThrowErrorOrActor*>(this)->destroy(); return 0; }
															#line 1134 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ThrowErrorOrActor*>(this)->SAV< T >::value()) T(t.get());
		this->~ThrowErrorOrActorState();
		static_cast<ThrowErrorOrActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(ErrorOr<T> && t,int loopDepth) 
	{
															#line 119 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (t.isError())
															#line 1146 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 120 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(t.getError(), loopDepth);
															#line 1150 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 121 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ThrowErrorOrActor*>(this)->SAV<T>::futures) { (void)(t.get()); this->~ThrowErrorOrActorState(); static_cast<ThrowErrorOrActor*>(this)->destroy(); return 0; }
															#line 1154 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ThrowErrorOrActor*>(this)->SAV< T >::value()) T(t.get());
		this->~ThrowErrorOrActorState();
		static_cast<ThrowErrorOrActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(ErrorOr<T> const& t,int loopDepth) 
	{
		loopDepth = a_body1cont1(t, loopDepth);

		return loopDepth;
	}
	int a_body1when1(ErrorOr<T> && t,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(t), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ThrowErrorOrActor*>(this)->actor_wait_state > 0) static_cast<ThrowErrorOrActor*>(this)->actor_wait_state = 0;
		static_cast<ThrowErrorOrActor*>(this)->ActorCallback< ThrowErrorOrActor, 0, ErrorOr<T> >::remove();

	}
	void a_callback_fire(ActorCallback< ThrowErrorOrActor, 0, ErrorOr<T> >*,ErrorOr<T> const& value) 
	{
		fdb_probe_actor_enter("throwErrorOr", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throwErrorOr", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ThrowErrorOrActor, 0, ErrorOr<T> >*,ErrorOr<T> && value) 
	{
		fdb_probe_actor_enter("throwErrorOr", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throwErrorOr", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ThrowErrorOrActor, 0, ErrorOr<T> >*,Error err) 
	{
		fdb_probe_actor_enter("throwErrorOr", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throwErrorOr", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<ErrorOr<T>> f;
															#line 1227 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via throwErrorOr()
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ThrowErrorOrActor final : public Actor<T>, public ActorCallback< ThrowErrorOrActor<T>, 0, ErrorOr<T> >, public FastAllocated<ThrowErrorOrActor<T>>, public ThrowErrorOrActorState<T, ThrowErrorOrActor<T>> {
															#line 1234 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ThrowErrorOrActor<T>>::operator new;
	using FastAllocated<ThrowErrorOrActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ThrowErrorOrActor<T>, 0, ErrorOr<T> >;
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ThrowErrorOrActor(Future<ErrorOr<T>> const& f) 
															#line 1245 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   ThrowErrorOrActorState<T, ThrowErrorOrActor<T>>(f)
	{
		fdb_probe_actor_enter("throwErrorOr", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("throwErrorOr");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("throwErrorOr", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ThrowErrorOrActor<T>, 0, ErrorOr<T> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> throwErrorOr( Future<ErrorOr<T>> const& f ) {
															#line 116 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new ThrowErrorOrActor<T>(f));
															#line 1275 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 123 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via transformErrors()
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TransformErrorsActor>
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TransformErrorsActorState {
															#line 1287 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TransformErrorsActorState(Future<T> const& f,Error const& err) 
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f),
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   err(err)
															#line 1296 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("transformErrors", reinterpret_cast<unsigned long>(this));

	}
	~TransformErrorsActorState() 
	{
		fdb_probe_actor_destroy("transformErrors", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 127 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = f;
															#line 127 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TransformErrorsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1314 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TransformErrorsActor*>(this)->actor_wait_state = 1;
															#line 127 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TransformErrorsActor, 0, T >*>(static_cast<TransformErrorsActor*>(this)));
															#line 1319 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TransformErrorsActorState();
		static_cast<TransformErrorsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 130 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 1349 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 131 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, loopDepth);
															#line 1353 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 132 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(err, loopDepth);
															#line 1357 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TransformErrorsActor*>(this)->SAV<T>::futures) { (void)(t); this->~TransformErrorsActorState(); static_cast<TransformErrorsActor*>(this)->destroy(); return 0; }
															#line 1371 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TransformErrorsActor*>(this)->SAV< T >::value()) T(t);
		this->~TransformErrorsActorState();
		static_cast<TransformErrorsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TransformErrorsActor*>(this)->SAV<T>::futures) { (void)(t); this->~TransformErrorsActorState(); static_cast<TransformErrorsActor*>(this)->destroy(); return 0; }
															#line 1383 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TransformErrorsActor*>(this)->SAV< T >::value()) T(t);
		this->~TransformErrorsActorState();
		static_cast<TransformErrorsActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TransformErrorsActor*>(this)->actor_wait_state > 0) static_cast<TransformErrorsActor*>(this)->actor_wait_state = 0;
		static_cast<TransformErrorsActor*>(this)->ActorCallback< TransformErrorsActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< TransformErrorsActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("transformErrors", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("transformErrors", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TransformErrorsActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("transformErrors", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("transformErrors", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TransformErrorsActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("transformErrors", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("transformErrors", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error err;
															#line 1458 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via transformErrors()
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TransformErrorsActor final : public Actor<T>, public ActorCallback< TransformErrorsActor<T>, 0, T >, public FastAllocated<TransformErrorsActor<T>>, public TransformErrorsActorState<T, TransformErrorsActor<T>> {
															#line 1465 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TransformErrorsActor<T>>::operator new;
	using FastAllocated<TransformErrorsActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TransformErrorsActor<T>, 0, T >;
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TransformErrorsActor(Future<T> const& f,Error const& err) 
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TransformErrorsActorState<T, TransformErrorsActor<T>>(f, err)
	{
		fdb_probe_actor_enter("transformErrors", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("transformErrors");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("transformErrors", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TransformErrorsActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> transformErrors( Future<T> const& f, Error const& err ) {
															#line 124 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TransformErrorsActor<T>(f, err));
															#line 1506 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 135 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 1511 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via transformError()
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TransformErrorActor>
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TransformErrorActorState {
															#line 1518 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TransformErrorActorState(Future<T> const& f,Error const& inErr,Error const& outErr) 
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f),
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   inErr(inErr),
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   outErr(outErr)
															#line 1529 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("transformError", reinterpret_cast<unsigned long>(this));

	}
	~TransformErrorActorState() 
	{
		fdb_probe_actor_destroy("transformError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 139 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = f;
															#line 139 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TransformErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1547 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TransformErrorActor*>(this)->actor_wait_state = 1;
															#line 139 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TransformErrorActor, 0, T >*>(static_cast<TransformErrorActor*>(this)));
															#line 1552 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TransformErrorActorState();
		static_cast<TransformErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 142 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == inErr.code())
															#line 1582 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 143 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(outErr, loopDepth);
															#line 1586 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 144 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 1590 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 140 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TransformErrorActor*>(this)->SAV<T>::futures) { (void)(t); this->~TransformErrorActorState(); static_cast<TransformErrorActor*>(this)->destroy(); return 0; }
															#line 1604 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TransformErrorActor*>(this)->SAV< T >::value()) T(t);
		this->~TransformErrorActorState();
		static_cast<TransformErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 140 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TransformErrorActor*>(this)->SAV<T>::futures) { (void)(t); this->~TransformErrorActorState(); static_cast<TransformErrorActor*>(this)->destroy(); return 0; }
															#line 1616 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TransformErrorActor*>(this)->SAV< T >::value()) T(t);
		this->~TransformErrorActorState();
		static_cast<TransformErrorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TransformErrorActor*>(this)->actor_wait_state > 0) static_cast<TransformErrorActor*>(this)->actor_wait_state = 0;
		static_cast<TransformErrorActor*>(this)->ActorCallback< TransformErrorActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< TransformErrorActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("transformError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("transformError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TransformErrorActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("transformError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("transformError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TransformErrorActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("transformError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("transformError", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error inErr;
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error outErr;
															#line 1693 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via transformError()
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TransformErrorActor final : public Actor<T>, public ActorCallback< TransformErrorActor<T>, 0, T >, public FastAllocated<TransformErrorActor<T>>, public TransformErrorActorState<T, TransformErrorActor<T>> {
															#line 1700 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TransformErrorActor<T>>::operator new;
	using FastAllocated<TransformErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TransformErrorActor<T>, 0, T >;
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TransformErrorActor(Future<T> const& f,Error const& inErr,Error const& outErr) 
															#line 1711 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TransformErrorActorState<T, TransformErrorActor<T>>(f, inErr, outErr)
	{
		fdb_probe_actor_enter("transformError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("transformError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("transformError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TransformErrorActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> transformError( Future<T> const& f, Error const& inErr, Error const& outErr ) {
															#line 136 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TransformErrorActor<T>(f, inErr, outErr));
															#line 1741 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 147 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Note that the RequestStream<T> version of forwardPromise doesn't exist, because what to do with errors?

															#line 1748 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardEvent()
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ForwardEventActor>
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardEventActorState {
															#line 1755 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardEventActorState(Event* const& ev,Future<T> const& input) 
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : ev(ev),
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   input(input)
															#line 1764 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardEvent", reinterpret_cast<unsigned long>(this));

	}
	~ForwardEventActorState() 
	{
		fdb_probe_actor_destroy("forwardEvent", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 153 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 153 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1782 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				static_cast<ForwardEventActor*>(this)->actor_wait_state = 1;
															#line 153 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardEventActor, 0, T >*>(static_cast<ForwardEventActor*>(this)));
															#line 1786 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<ForwardEventActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 156 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ev->set();
															#line 1814 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
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
	int a_body1cont2(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<ForwardEventActor*>(this)->actor_wait_state > 0) static_cast<ForwardEventActor*>(this)->actor_wait_state = 0;
		static_cast<ForwardEventActor*>(this)->ActorCallback< ForwardEventActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardEventActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardEventActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardEventActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont5(int loopDepth) 
	{
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<ForwardEventActor*>(this);
															#line 1924 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Event* ev;
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> input;
															#line 1933 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardEvent()
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardEventActor final : public Actor<void>, public ActorCallback< ForwardEventActor<T>, 0, T >, public FastAllocated<ForwardEventActor<T>>, public ForwardEventActorState<T, ForwardEventActor<T>> {
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardEventActor<T>>::operator new;
	using FastAllocated<ForwardEventActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardEventActor<T>, 0, T >;
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardEventActor(Event* const& ev,Future<T> const& input) 
															#line 1951 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   ForwardEventActorState<T, ForwardEventActor<T>>(ev, input)
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardEvent");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void forwardEvent( Event* const& ev, Future<T> const& input ) {
															#line 150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new ForwardEventActor<T>(ev, input);
															#line 1972 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 158 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 1977 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardEvent()
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ForwardEventActor1>
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardEventActor1State {
															#line 1984 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardEventActor1State(Event* const& ev,T* const& t,Error* const& err,FutureStream<T> const& input) 
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : ev(ev),
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   t(t),
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   err(err),
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   input(input)
															#line 1997 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardEvent", reinterpret_cast<unsigned long>(this));

	}
	~ForwardEventActor1State() 
	{
		fdb_probe_actor_destroy("forwardEvent", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 162 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				FutureStream<T> __when_expr_0 = input;
															#line 162 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
															#line 2015 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				static_cast<ForwardEventActor1*>(this)->actor_wait_state = 1;
															#line 162 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ForwardEventActor1, 0, T >*>(static_cast<ForwardEventActor1*>(this)));
															#line 2019 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<ForwardEventActor1*>(this);
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
															#line 166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			*err = e;
															#line 167 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			ev->set();
															#line 2056 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 163 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		*t = std::move(value);
															#line 164 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ev->set();
															#line 2073 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 163 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		*t = std::move(value);
															#line 164 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ev->set();
															#line 2084 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<ForwardEventActor1*>(this)->actor_wait_state > 0) static_cast<ForwardEventActor1*>(this)->actor_wait_state = 0;
		static_cast<ForwardEventActor1*>(this)->ActorSingleCallback< ForwardEventActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ForwardEventActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ForwardEventActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< ForwardEventActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), 0);

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
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<ForwardEventActor1*>(this);
															#line 2169 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Event* ev;
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T* t;
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error* err;
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 2182 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardEvent()
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardEventActor1 final : public Actor<void>, public ActorSingleCallback< ForwardEventActor1<T>, 0, T >, public FastAllocated<ForwardEventActor1<T>>, public ForwardEventActor1State<T, ForwardEventActor1<T>> {
															#line 2189 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardEventActor1<T>>::operator new;
	using FastAllocated<ForwardEventActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ForwardEventActor1<T>, 0, T >;
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardEventActor1(Event* const& ev,T* const& t,Error* const& err,FutureStream<T> const& input) 
															#line 2200 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   ForwardEventActor1State<T, ForwardEventActor1<T>>(ev, t, err, input)
	{
		fdb_probe_actor_enter("forwardEvent", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardEvent");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardEvent", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void forwardEvent( Event* const& ev, T* const& t, Error* const& err, FutureStream<T> const& input ) {
															#line 159 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new ForwardEventActor1<T>(ev, t, err, input);
															#line 2221 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 170 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 2226 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via waitForAllReady()
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class WaitForAllReadyActor>
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitForAllReadyActorState {
															#line 2233 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitForAllReadyActorState(std::vector<Future<T>> const& results) 
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : results(results),
															#line 173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   i(0)
															#line 2242 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("waitForAllReady", reinterpret_cast<unsigned long>(this));

	}
	~WaitForAllReadyActorState() 
	{
		fdb_probe_actor_destroy("waitForAllReady", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 174 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 2257 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~WaitForAllReadyActorState();
		static_cast<WaitForAllReadyActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 175 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (i == results.size())
															#line 2287 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 176 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!static_cast<WaitForAllReadyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForAllReadyActorState(); static_cast<WaitForAllReadyActor*>(this)->destroy(); return 0; }
															#line 2291 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			new (&static_cast<WaitForAllReadyActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForAllReadyActorState();
			static_cast<WaitForAllReadyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 178 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = results[i];
															#line 178 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<WaitForAllReadyActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2302 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitForAllReadyActor*>(this)->actor_wait_state = 1;
															#line 178 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForAllReadyActor, 0, T >*>(static_cast<WaitForAllReadyActor*>(this)));
															#line 2307 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 182 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		i++;
															#line 2322 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& __current_error,int loopDepth=0) 
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
	int a_body1loopBody1cont3(T const& t,int loopDepth) 
	{
															#line 179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)t;
															#line 2344 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(T && t,int loopDepth) 
	{
															#line 179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)t;
															#line 2353 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& t,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(t, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && t,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(t), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitForAllReadyActor*>(this)->actor_wait_state > 0) static_cast<WaitForAllReadyActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForAllReadyActor*>(this)->ActorCallback< WaitForAllReadyActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForAllReadyActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("waitForAllReady", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForAllReady", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForAllReadyActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("waitForAllReady", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForAllReady", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForAllReadyActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("waitForAllReady", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForAllReady", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
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
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::vector<Future<T>> results;
															#line 173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int i;
															#line 2438 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via waitForAllReady()
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitForAllReadyActor final : public Actor<Void>, public ActorCallback< WaitForAllReadyActor<T>, 0, T >, public FastAllocated<WaitForAllReadyActor<T>>, public WaitForAllReadyActorState<T, WaitForAllReadyActor<T>> {
															#line 2445 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<WaitForAllReadyActor<T>>::operator new;
	using FastAllocated<WaitForAllReadyActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForAllReadyActor<T>, 0, T >;
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitForAllReadyActor(std::vector<Future<T>> const& results) 
															#line 2456 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   WaitForAllReadyActorState<T, WaitForAllReadyActor<T>>(results)
	{
		fdb_probe_actor_enter("waitForAllReady", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForAllReady");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForAllReady", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForAllReadyActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> waitForAllReady( std::vector<Future<T>> const& results ) {
															#line 171 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new WaitForAllReadyActor<T>(results));
															#line 2486 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 185 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 2491 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via timeout()
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TimeoutActor>
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeoutActorState {
															#line 2498 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeoutActorState(Future<T> const& what,double const& time,T const& timedoutValue,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time),
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   timedoutValue(timedoutValue),
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID)
															#line 2511 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("timeout", reinterpret_cast<unsigned long>(this));

	}
	~TimeoutActorState() 
	{
		fdb_probe_actor_destroy("timeout", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 188 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			Future<Void> end = delay(time, taskID);
															#line 190 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 189 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TimeoutActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2530 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 193 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = end;
															#line 2534 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<TimeoutActor*>(this)->actor_wait_state = 1;
															#line 190 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeoutActor, 0, T >*>(static_cast<TimeoutActor*>(this)));
															#line 193 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutActor, 1, Void >*>(static_cast<TimeoutActor*>(this)));
															#line 2541 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TimeoutActorState();
		static_cast<TimeoutActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
															#line 191 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutActorState(); static_cast<TimeoutActor*>(this)->destroy(); return 0; }
															#line 2564 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutActorState();
		static_cast<TimeoutActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
															#line 191 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutActorState(); static_cast<TimeoutActor*>(this)->destroy(); return 0; }
															#line 2576 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutActorState();
		static_cast<TimeoutActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 194 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor*>(this)->SAV<T>::futures) { (void)(timedoutValue); this->~TimeoutActorState(); static_cast<TimeoutActor*>(this)->destroy(); return 0; }
															#line 2588 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor*>(this)->SAV< T >::value()) T(std::move(timedoutValue)); // state_var_RVO
		this->~TimeoutActorState();
		static_cast<TimeoutActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 194 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor*>(this)->SAV<T>::futures) { (void)(timedoutValue); this->~TimeoutActorState(); static_cast<TimeoutActor*>(this)->destroy(); return 0; }
															#line 2600 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor*>(this)->SAV< T >::value()) T(std::move(timedoutValue)); // state_var_RVO
		this->~TimeoutActorState();
		static_cast<TimeoutActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeoutActor*>(this)->actor_wait_state > 0) static_cast<TimeoutActor*>(this)->actor_wait_state = 0;
		static_cast<TimeoutActor*>(this)->ActorCallback< TimeoutActor, 0, T >::remove();
		static_cast<TimeoutActor*>(this)->ActorCallback< TimeoutActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeoutActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeoutActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeoutActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeoutActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T timedoutValue;
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 2713 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via timeout()
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeoutActor final : public Actor<T>, public ActorCallback< TimeoutActor<T>, 0, T >, public ActorCallback< TimeoutActor<T>, 1, Void >, public FastAllocated<TimeoutActor<T>>, public TimeoutActorState<T, TimeoutActor<T>> {
															#line 2720 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TimeoutActor<T>>::operator new;
	using FastAllocated<TimeoutActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeoutActor<T>, 0, T >;
friend struct ActorCallback< TimeoutActor<T>, 1, Void >;
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeoutActor(Future<T> const& what,double const& time,T const& timedoutValue,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 2732 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TimeoutActorState<T, TimeoutActor<T>>(what, time, timedoutValue, taskID)
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeout");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeoutActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> timeout( Future<T> const& what, double const& time, T const& timedoutValue, TaskPriority const& taskID = TaskPriority::DefaultDelay ) {
															#line 186 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TimeoutActor<T>(what, time, timedoutValue, taskID));
															#line 2762 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 198 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 2767 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via timeout()
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TimeoutActor1>
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeoutActor1State {
															#line 2774 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeoutActor1State(Future<T> const& what,double const& time) 
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time)
															#line 2783 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("timeout", reinterpret_cast<unsigned long>(this));

	}
	~TimeoutActor1State() 
	{
		fdb_probe_actor_destroy("timeout", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 201 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			Future<Void> end = delay(time);
															#line 203 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 202 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TimeoutActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2802 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 206 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = end;
															#line 2806 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<TimeoutActor1*>(this)->actor_wait_state = 1;
															#line 203 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeoutActor1, 0, T >*>(static_cast<TimeoutActor1*>(this)));
															#line 206 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutActor1, 1, Void >*>(static_cast<TimeoutActor1*>(this)));
															#line 2813 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TimeoutActor1State();
		static_cast<TimeoutActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
															#line 204 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor1*>(this)->SAV<Optional<T>>::futures) { (void)(t); this->~TimeoutActor1State(); static_cast<TimeoutActor1*>(this)->destroy(); return 0; }
															#line 2836 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor1*>(this)->SAV< Optional<T> >::value()) Optional<T>(t);
		this->~TimeoutActor1State();
		static_cast<TimeoutActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
															#line 204 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor1*>(this)->SAV<Optional<T>>::futures) { (void)(t); this->~TimeoutActor1State(); static_cast<TimeoutActor1*>(this)->destroy(); return 0; }
															#line 2848 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor1*>(this)->SAV< Optional<T> >::value()) Optional<T>(t);
		this->~TimeoutActor1State();
		static_cast<TimeoutActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 207 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor1*>(this)->SAV<Optional<T>>::futures) { (void)(Optional<T>()); this->~TimeoutActor1State(); static_cast<TimeoutActor1*>(this)->destroy(); return 0; }
															#line 2860 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor1*>(this)->SAV< Optional<T> >::value()) Optional<T>(Optional<T>());
		this->~TimeoutActor1State();
		static_cast<TimeoutActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 207 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutActor1*>(this)->SAV<Optional<T>>::futures) { (void)(Optional<T>()); this->~TimeoutActor1State(); static_cast<TimeoutActor1*>(this)->destroy(); return 0; }
															#line 2872 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutActor1*>(this)->SAV< Optional<T> >::value()) Optional<T>(Optional<T>());
		this->~TimeoutActor1State();
		static_cast<TimeoutActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeoutActor1*>(this)->actor_wait_state > 0) static_cast<TimeoutActor1*>(this)->actor_wait_state = 0;
		static_cast<TimeoutActor1*>(this)->ActorCallback< TimeoutActor1, 0, T >::remove();
		static_cast<TimeoutActor1*>(this)->ActorCallback< TimeoutActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeoutActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeoutActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeoutActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeoutActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 2981 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via timeout()
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeoutActor1 final : public Actor<Optional<T>>, public ActorCallback< TimeoutActor1<T>, 0, T >, public ActorCallback< TimeoutActor1<T>, 1, Void >, public FastAllocated<TimeoutActor1<T>>, public TimeoutActor1State<T, TimeoutActor1<T>> {
															#line 2988 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TimeoutActor1<T>>::operator new;
	using FastAllocated<TimeoutActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeoutActor1<T>, 0, T >;
friend struct ActorCallback< TimeoutActor1<T>, 1, Void >;
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeoutActor1(Future<T> const& what,double const& time) 
															#line 3000 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Optional<T>>(),
		   TimeoutActor1State<T, TimeoutActor1<T>>(what, time)
	{
		fdb_probe_actor_enter("timeout", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeout");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeout", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeoutActor1<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Optional<T>> timeout( Future<T> const& what, double const& time ) {
															#line 199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Optional<T>>(new TimeoutActor1<T>(what, time));
															#line 3030 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 211 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 3035 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via timeoutError()
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TimeoutErrorActor>
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeoutErrorActorState {
															#line 3042 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeoutErrorActorState(Future<T> const& what,double const& time,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time),
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID)
															#line 3053 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("timeoutError", reinterpret_cast<unsigned long>(this));

	}
	~TimeoutErrorActorState() 
	{
		fdb_probe_actor_destroy("timeoutError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 214 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			Future<Void> end = delay(time, taskID);
															#line 216 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 215 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TimeoutErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3072 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 219 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = end;
															#line 3076 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<TimeoutErrorActor*>(this)->actor_wait_state = 1;
															#line 216 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeoutErrorActor, 0, T >*>(static_cast<TimeoutErrorActor*>(this)));
															#line 219 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutErrorActor, 1, Void >*>(static_cast<TimeoutErrorActor*>(this)));
															#line 3083 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TimeoutErrorActorState();
		static_cast<TimeoutErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
															#line 217 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutErrorActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutErrorActorState(); static_cast<TimeoutErrorActor*>(this)->destroy(); return 0; }
															#line 3106 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutErrorActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutErrorActorState();
		static_cast<TimeoutErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
															#line 217 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeoutErrorActor*>(this)->SAV<T>::futures) { (void)(t); this->~TimeoutErrorActorState(); static_cast<TimeoutErrorActor*>(this)->destroy(); return 0; }
															#line 3118 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeoutErrorActor*>(this)->SAV< T >::value()) T(t);
		this->~TimeoutErrorActorState();
		static_cast<TimeoutErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 220 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(timed_out(), loopDepth);
															#line 3130 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 220 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(timed_out(), loopDepth);
															#line 3138 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeoutErrorActor*>(this)->actor_wait_state > 0) static_cast<TimeoutErrorActor*>(this)->actor_wait_state = 0;
		static_cast<TimeoutErrorActor*>(this)->ActorCallback< TimeoutErrorActor, 0, T >::remove();
		static_cast<TimeoutErrorActor*>(this)->ActorCallback< TimeoutErrorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeoutErrorActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutErrorActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeoutErrorActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutErrorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeoutErrorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeoutErrorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 3245 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via timeoutError()
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeoutErrorActor final : public Actor<T>, public ActorCallback< TimeoutErrorActor<T>, 0, T >, public ActorCallback< TimeoutErrorActor<T>, 1, Void >, public FastAllocated<TimeoutErrorActor<T>>, public TimeoutErrorActorState<T, TimeoutErrorActor<T>> {
															#line 3252 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TimeoutErrorActor<T>>::operator new;
	using FastAllocated<TimeoutErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeoutErrorActor<T>, 0, T >;
friend struct ActorCallback< TimeoutErrorActor<T>, 1, Void >;
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeoutErrorActor(Future<T> const& what,double const& time,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 3264 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TimeoutErrorActorState<T, TimeoutErrorActor<T>>(what, time, taskID)
	{
		fdb_probe_actor_enter("timeoutError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeoutError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeoutError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeoutErrorActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> timeoutError( Future<T> const& what, double const& time, TaskPriority const& taskID = TaskPriority::DefaultDelay ) {
															#line 212 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TimeoutErrorActor<T>(what, time, taskID));
															#line 3294 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 224 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 3299 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via delayed()
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class DelayedActor>
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DelayedActorState {
															#line 3306 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DelayedActorState(Future<T> const& what,double const& time = 0.0,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time),
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID)
															#line 3317 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("delayed", reinterpret_cast<unsigned long>(this));

	}
	~DelayedActorState() 
	{
		fdb_probe_actor_destroy("delayed", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 228 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = what;
															#line 228 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<DelayedActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3335 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DelayedActor*>(this)->actor_wait_state = 1;
															#line 228 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DelayedActor, 0, T >*>(static_cast<DelayedActor*>(this)));
															#line 3340 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DelayedActorState();
		static_cast<DelayedActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 232 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			err = e;
															#line 233 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_2 = delay(time, taskID);
															#line 233 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<DelayedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3374 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_2.get(), loopDepth); };
			static_cast<DelayedActor*>(this)->actor_wait_state = 3;
															#line 233 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DelayedActor, 2, Void >*>(static_cast<DelayedActor*>(this)));
															#line 3379 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
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
															#line 229 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(time, taskID);
															#line 229 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DelayedActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3396 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<DelayedActor*>(this)->actor_wait_state = 2;
															#line 229 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DelayedActor, 1, Void >*>(static_cast<DelayedActor*>(this)));
															#line 3401 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& __t,int loopDepth) 
	{
															#line 228 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		t = __t;
															#line 3410 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && __t,int loopDepth) 
	{
		t = std::move(__t);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DelayedActor*>(this)->actor_wait_state > 0) static_cast<DelayedActor*>(this)->actor_wait_state = 0;
		static_cast<DelayedActor*>(this)->ActorCallback< DelayedActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< DelayedActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DelayedActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DelayedActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 230 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DelayedActor*>(this)->SAV<T>::futures) { (void)(t); this->~DelayedActorState(); static_cast<DelayedActor*>(this)->destroy(); return 0; }
															#line 3477 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DelayedActor*>(this)->SAV< T >::value()) T(std::move(t)); // state_var_RVO
		this->~DelayedActorState();
		static_cast<DelayedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 230 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DelayedActor*>(this)->SAV<T>::futures) { (void)(t); this->~DelayedActorState(); static_cast<DelayedActor*>(this)->destroy(); return 0; }
															#line 3489 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DelayedActor*>(this)->SAV< T >::value()) T(std::move(t)); // state_var_RVO
		this->~DelayedActorState();
		static_cast<DelayedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DelayedActor*>(this)->actor_wait_state > 0) static_cast<DelayedActor*>(this)->actor_wait_state = 0;
		static_cast<DelayedActor*>(this)->ActorCallback< DelayedActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DelayedActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DelayedActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DelayedActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1Catch2cont1(Void const& _,int loopDepth) 
	{
															#line 234 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 3564 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1Catch2cont1(Void && _,int loopDepth) 
	{
															#line 234 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 3572 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DelayedActor*>(this)->actor_wait_state > 0) static_cast<DelayedActor*>(this)->actor_wait_state = 0;
		static_cast<DelayedActor*>(this)->ActorCallback< DelayedActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DelayedActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DelayedActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DelayedActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 228 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T t;
															#line 232 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error err;
															#line 3649 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via delayed()
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DelayedActor final : public Actor<T>, public ActorCallback< DelayedActor<T>, 0, T >, public ActorCallback< DelayedActor<T>, 1, Void >, public ActorCallback< DelayedActor<T>, 2, Void >, public FastAllocated<DelayedActor<T>>, public DelayedActorState<T, DelayedActor<T>> {
															#line 3656 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DelayedActor<T>>::operator new;
	using FastAllocated<DelayedActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DelayedActor<T>, 0, T >;
friend struct ActorCallback< DelayedActor<T>, 1, Void >;
friend struct ActorCallback< DelayedActor<T>, 2, Void >;
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DelayedActor(Future<T> const& what,double const& time = 0.0,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 3669 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   DelayedActorState<T, DelayedActor<T>>(what, time, taskID)
	{
		fdb_probe_actor_enter("delayed", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("delayed");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("delayed", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DelayedActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DelayedActor<T>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DelayedActor<T>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> delayed( Future<T> const& what, double const& time = 0.0, TaskPriority const& taskID = TaskPriority::DefaultDelay ) {
															#line 225 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new DelayedActor<T>(what, time, taskID));
															#line 3701 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 237 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// wait <interval> then call what() in a loop forever
															#line 3707 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via recurring()
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func, class RecurringActor>
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RecurringActorState {
															#line 3714 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RecurringActorState(Func const& what,double const& interval,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   interval(interval),
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID)
															#line 3725 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("recurring", reinterpret_cast<unsigned long>(this));

	}
	~RecurringActorState() 
	{
		fdb_probe_actor_destroy("recurring", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 241 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 3740 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~RecurringActorState();
		static_cast<RecurringActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 242 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = delay(interval, taskID);
															#line 241 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<RecurringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3772 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RecurringActor*>(this)->actor_wait_state = 1;
															#line 242 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RecurringActor, 0, Void >*>(static_cast<RecurringActor*>(this)));
															#line 3777 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 243 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		what();
															#line 3792 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 243 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		what();
															#line 3801 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RecurringActor*>(this)->actor_wait_state > 0) static_cast<RecurringActor*>(this)->actor_wait_state = 0;
		static_cast<RecurringActor*>(this)->ActorCallback< RecurringActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RecurringActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("recurring", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("recurring", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RecurringActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("recurring", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("recurring", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RecurringActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("recurring", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("recurring", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Func what;
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double interval;
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 3863 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via recurring()
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func>
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RecurringActor final : public Actor<Void>, public ActorCallback< RecurringActor<Func>, 0, Void >, public FastAllocated<RecurringActor<Func>>, public RecurringActorState<Func, RecurringActor<Func>> {
															#line 3870 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<RecurringActor<Func>>::operator new;
	using FastAllocated<RecurringActor<Func>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RecurringActor<Func>, 0, Void >;
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RecurringActor(Func const& what,double const& interval,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 3881 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   RecurringActorState<Func, RecurringActor<Func>>(what, interval, taskID)
	{
		fdb_probe_actor_enter("recurring", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("recurring");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("recurring", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RecurringActor<Func>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func>
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> recurring( Func const& what, double const& interval, TaskPriority const& taskID = TaskPriority::DefaultDelay ) {
															#line 239 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new RecurringActor<Func>(what, interval, taskID));
															#line 3911 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 247 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 3916 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via trigger()
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func, class TriggerActor>
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TriggerActorState {
															#line 3923 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TriggerActorState(Func const& what,Future<Void> const& signal) 
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal)
															#line 3932 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("trigger", reinterpret_cast<unsigned long>(this));

	}
	~TriggerActorState() 
	{
		fdb_probe_actor_destroy("trigger", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 250 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 250 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TriggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3949 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TriggerActor*>(this)->actor_wait_state = 1;
															#line 250 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TriggerActor, 0, Void >*>(static_cast<TriggerActor*>(this)));
															#line 3954 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TriggerActorState();
		static_cast<TriggerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 251 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		what();
															#line 252 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TriggerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TriggerActorState(); static_cast<TriggerActor*>(this)->destroy(); return 0; }
															#line 3979 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TriggerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TriggerActorState();
		static_cast<TriggerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 251 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		what();
															#line 252 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TriggerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TriggerActorState(); static_cast<TriggerActor*>(this)->destroy(); return 0; }
															#line 3993 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TriggerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TriggerActorState();
		static_cast<TriggerActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TriggerActor*>(this)->actor_wait_state > 0) static_cast<TriggerActor*>(this)->actor_wait_state = 0;
		static_cast<TriggerActor*>(this)->ActorCallback< TriggerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TriggerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("trigger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trigger", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TriggerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("trigger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trigger", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TriggerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("trigger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("trigger", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Func what;
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 4068 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via trigger()
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func>
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TriggerActor final : public Actor<Void>, public ActorCallback< TriggerActor<Func>, 0, Void >, public FastAllocated<TriggerActor<Func>>, public TriggerActorState<Func, TriggerActor<Func>> {
															#line 4075 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TriggerActor<Func>>::operator new;
	using FastAllocated<TriggerActor<Func>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TriggerActor<Func>, 0, Void >;
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TriggerActor(Func const& what,Future<Void> const& signal) 
															#line 4086 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TriggerActorState<Func, TriggerActor<Func>>(what, signal)
	{
		fdb_probe_actor_enter("trigger", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("trigger");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("trigger", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TriggerActor<Func>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func>
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> trigger( Func const& what, Future<Void> const& signal ) {
															#line 248 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new TriggerActor<Func>(what, signal));
															#line 4116 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 254 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 4121 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via triggerOnError()
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func, class TriggerOnErrorActor>
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TriggerOnErrorActorState {
															#line 4128 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TriggerOnErrorActorState(Func const& what,Future<Void> const& signal) 
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal)
															#line 4137 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("triggerOnError", reinterpret_cast<unsigned long>(this));

	}
	~TriggerOnErrorActorState() 
	{
		fdb_probe_actor_destroy("triggerOnError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 258 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = signal;
															#line 258 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TriggerOnErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4155 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TriggerOnErrorActor*>(this)->actor_wait_state = 1;
															#line 258 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TriggerOnErrorActor, 0, Void >*>(static_cast<TriggerOnErrorActor*>(this)));
															#line 4160 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TriggerOnErrorActorState();
		static_cast<TriggerOnErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 263 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TriggerOnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TriggerOnErrorActorState(); static_cast<TriggerOnErrorActor*>(this)->destroy(); return 0; }
															#line 4189 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TriggerOnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TriggerOnErrorActorState();
		static_cast<TriggerOnErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			what();
															#line 4202 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<TriggerOnErrorActor*>(this)->actor_wait_state > 0) static_cast<TriggerOnErrorActor*>(this)->actor_wait_state = 0;
		static_cast<TriggerOnErrorActor*>(this)->ActorCallback< TriggerOnErrorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TriggerOnErrorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("triggerOnError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("triggerOnError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TriggerOnErrorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("triggerOnError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("triggerOnError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TriggerOnErrorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("triggerOnError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("triggerOnError", reinterpret_cast<unsigned long>(this), 0);

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
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Func what;
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 4305 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via triggerOnError()
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func>
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TriggerOnErrorActor final : public Actor<Void>, public ActorCallback< TriggerOnErrorActor<Func>, 0, Void >, public FastAllocated<TriggerOnErrorActor<Func>>, public TriggerOnErrorActorState<Func, TriggerOnErrorActor<Func>> {
															#line 4312 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TriggerOnErrorActor<Func>>::operator new;
	using FastAllocated<TriggerOnErrorActor<Func>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TriggerOnErrorActor<Func>, 0, Void >;
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TriggerOnErrorActor(Func const& what,Future<Void> const& signal) 
															#line 4323 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TriggerOnErrorActorState<Func, TriggerOnErrorActor<Func>>(what, signal)
	{
		fdb_probe_actor_enter("triggerOnError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("triggerOnError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("triggerOnError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TriggerOnErrorActor<Func>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Func>
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> triggerOnError( Func const& what, Future<Void> const& signal ) {
															#line 255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new TriggerOnErrorActor<Func>(what, signal));
															#line 4353 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 265 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Waits for a future to complete and cannot be cancelled
// Most situations will use the overload below, which does not require a promise
															#line 4360 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via uncancellable()
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class UncancellableActor>
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class UncancellableActorState {
															#line 4367 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UncancellableActorState(Future<T> const& what,Promise<T> const& result) 
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   result(result)
															#line 4376 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("uncancellable", reinterpret_cast<unsigned long>(this));

	}
	~UncancellableActorState() 
	{
		fdb_probe_actor_destroy("uncancellable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 271 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = what;
															#line 271 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 4394 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				static_cast<UncancellableActor*>(this)->actor_wait_state = 1;
															#line 271 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UncancellableActor, 0, T >*>(static_cast<UncancellableActor*>(this)));
															#line 4398 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<UncancellableActor*>(this);
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
															#line 274 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			result.sendError(e);
															#line 4433 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& val,int loopDepth) 
	{
															#line 272 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		result.send(val);
															#line 4448 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && val,int loopDepth) 
	{
															#line 272 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		result.send(val);
															#line 4457 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont2(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UncancellableActor*>(this)->actor_wait_state > 0) static_cast<UncancellableActor*>(this)->actor_wait_state = 0;
		static_cast<UncancellableActor*>(this)->ActorCallback< UncancellableActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< UncancellableActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UncancellableActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UncancellableActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), 0);

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
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<UncancellableActor*>(this);
															#line 4542 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Promise<T> result;
															#line 4551 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via uncancellable()
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class UncancellableActor final : public Actor<void>, public ActorCallback< UncancellableActor<T>, 0, T >, public FastAllocated<UncancellableActor<T>>, public UncancellableActorState<T, UncancellableActor<T>> {
															#line 4558 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<UncancellableActor<T>>::operator new;
	using FastAllocated<UncancellableActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< UncancellableActor<T>, 0, T >;
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UncancellableActor(Future<T> const& what,Promise<T> const& result) 
															#line 4569 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   UncancellableActorState<T, UncancellableActor<T>>(what, result)
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("uncancellable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void uncancellable( Future<T> const& what, Promise<T> const& result ) {
															#line 268 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new UncancellableActor<T>(what, result);
															#line 4590 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 277 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Waits for a future to complete and cannot be cancelled
															#line 4596 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via uncancellable()
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class UncancellableActor1>
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class UncancellableActor1State {
															#line 4603 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UncancellableActor1State(Future<T> const& what) 
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what)
															#line 4610 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("uncancellable", reinterpret_cast<unsigned long>(this));

	}
	~UncancellableActor1State() 
	{
		fdb_probe_actor_destroy("uncancellable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 281 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			Promise<T> resultPromise;
															#line 282 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			Future<T> result = resultPromise.getFuture();
															#line 284 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			uncancellable(what, resultPromise);
															#line 285 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = result;
															#line 285 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<UncancellableActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4633 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UncancellableActor1*>(this)->actor_wait_state = 1;
															#line 285 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UncancellableActor1, 0, T >*>(static_cast<UncancellableActor1*>(this)));
															#line 4638 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~UncancellableActor1State();
		static_cast<UncancellableActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val,int loopDepth) 
	{
															#line 287 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<UncancellableActor1*>(this)->SAV<T>::futures) { (void)(val); this->~UncancellableActor1State(); static_cast<UncancellableActor1*>(this)->destroy(); return 0; }
															#line 4661 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<UncancellableActor1*>(this)->SAV< T >::value()) T(val);
		this->~UncancellableActor1State();
		static_cast<UncancellableActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && val,int loopDepth) 
	{
															#line 287 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<UncancellableActor1*>(this)->SAV<T>::futures) { (void)(val); this->~UncancellableActor1State(); static_cast<UncancellableActor1*>(this)->destroy(); return 0; }
															#line 4673 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<UncancellableActor1*>(this)->SAV< T >::value()) T(val);
		this->~UncancellableActor1State();
		static_cast<UncancellableActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UncancellableActor1*>(this)->actor_wait_state > 0) static_cast<UncancellableActor1*>(this)->actor_wait_state = 0;
		static_cast<UncancellableActor1*>(this)->ActorCallback< UncancellableActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< UncancellableActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UncancellableActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UncancellableActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 4746 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via uncancellable()
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class UncancellableActor1 final : public Actor<T>, public ActorCallback< UncancellableActor1<T>, 0, T >, public FastAllocated<UncancellableActor1<T>>, public UncancellableActor1State<T, UncancellableActor1<T>> {
															#line 4753 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<UncancellableActor1<T>>::operator new;
	using FastAllocated<UncancellableActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UncancellableActor1<T>, 0, T >;
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UncancellableActor1(Future<T> const& what) 
															#line 4764 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   UncancellableActor1State<T, UncancellableActor1<T>>(what)
	{
		fdb_probe_actor_enter("uncancellable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("uncancellable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("uncancellable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UncancellableActor1<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
Future<T> uncancellable( Future<T> const& what ) {
															#line 279 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new UncancellableActor1<T>(what));
															#line 4794 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 289 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Holds onto an object until a future either completes or is cancelled
// Used to prevent the object from being reclaimed
//
// NOTE: the order of the arguments is important. The arguments will be destructed in
// reverse order, and we need the object to be destructed last.
															#line 4804 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via holdWhile()
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X, class HoldWhileActor>
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class HoldWhileActorState {
															#line 4811 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	HoldWhileActorState(X const& object,Future<T> const& what) 
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : object(object),
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   what(what)
															#line 4820 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("holdWhile", reinterpret_cast<unsigned long>(this));

	}
	~HoldWhileActorState() 
	{
		fdb_probe_actor_destroy("holdWhile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 297 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 297 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<HoldWhileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4837 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<HoldWhileActor*>(this)->actor_wait_state = 1;
															#line 297 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< HoldWhileActor, 0, T >*>(static_cast<HoldWhileActor*>(this)));
															#line 4842 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~HoldWhileActorState();
		static_cast<HoldWhileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val,int loopDepth) 
	{
															#line 298 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<HoldWhileActor*>(this)->SAV<T>::futures) { (void)(val); this->~HoldWhileActorState(); static_cast<HoldWhileActor*>(this)->destroy(); return 0; }
															#line 4865 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<HoldWhileActor*>(this)->SAV< T >::value()) T(val);
		this->~HoldWhileActorState();
		static_cast<HoldWhileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && val,int loopDepth) 
	{
															#line 298 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<HoldWhileActor*>(this)->SAV<T>::futures) { (void)(val); this->~HoldWhileActorState(); static_cast<HoldWhileActor*>(this)->destroy(); return 0; }
															#line 4877 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<HoldWhileActor*>(this)->SAV< T >::value()) T(val);
		this->~HoldWhileActorState();
		static_cast<HoldWhileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<HoldWhileActor*>(this)->actor_wait_state > 0) static_cast<HoldWhileActor*>(this)->actor_wait_state = 0;
		static_cast<HoldWhileActor*>(this)->ActorCallback< HoldWhileActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< HoldWhileActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("holdWhile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("holdWhile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< HoldWhileActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("holdWhile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("holdWhile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< HoldWhileActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("holdWhile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("holdWhile", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	X object;
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 4952 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via holdWhile()
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class HoldWhileActor final : public Actor<T>, public ActorCallback< HoldWhileActor<T, X>, 0, T >, public FastAllocated<HoldWhileActor<T, X>>, public HoldWhileActorState<T, X, HoldWhileActor<T, X>> {
															#line 4959 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<HoldWhileActor<T, X>>::operator new;
	using FastAllocated<HoldWhileActor<T, X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< HoldWhileActor<T, X>, 0, T >;
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	HoldWhileActor(X const& object,Future<T> const& what) 
															#line 4970 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   HoldWhileActorState<T, X, HoldWhileActor<T, X>>(object, what)
	{
		fdb_probe_actor_enter("holdWhile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("holdWhile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("holdWhile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< HoldWhileActor<T, X>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> holdWhile( X const& object, Future<T> const& what ) {
															#line 295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new HoldWhileActor<T, X>(object, what));
															#line 5000 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 300 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 5005 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via holdWhileVoid()
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X, class HoldWhileVoidActor>
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class HoldWhileVoidActorState {
															#line 5012 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	HoldWhileVoidActorState(X const& object,Future<T> const& what) 
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : object(object),
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   what(what)
															#line 5021 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("holdWhileVoid", reinterpret_cast<unsigned long>(this));

	}
	~HoldWhileVoidActorState() 
	{
		fdb_probe_actor_destroy("holdWhileVoid", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 303 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 303 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<HoldWhileVoidActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5038 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<HoldWhileVoidActor*>(this)->actor_wait_state = 1;
															#line 303 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< HoldWhileVoidActor, 0, T >*>(static_cast<HoldWhileVoidActor*>(this)));
															#line 5043 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~HoldWhileVoidActorState();
		static_cast<HoldWhileVoidActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val,int loopDepth) 
	{
															#line 304 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<HoldWhileVoidActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~HoldWhileVoidActorState(); static_cast<HoldWhileVoidActor*>(this)->destroy(); return 0; }
															#line 5066 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<HoldWhileVoidActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~HoldWhileVoidActorState();
		static_cast<HoldWhileVoidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && val,int loopDepth) 
	{
															#line 304 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<HoldWhileVoidActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~HoldWhileVoidActorState(); static_cast<HoldWhileVoidActor*>(this)->destroy(); return 0; }
															#line 5078 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<HoldWhileVoidActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~HoldWhileVoidActorState();
		static_cast<HoldWhileVoidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<HoldWhileVoidActor*>(this)->actor_wait_state > 0) static_cast<HoldWhileVoidActor*>(this)->actor_wait_state = 0;
		static_cast<HoldWhileVoidActor*>(this)->ActorCallback< HoldWhileVoidActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< HoldWhileVoidActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("holdWhileVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("holdWhileVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< HoldWhileVoidActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("holdWhileVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("holdWhileVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< HoldWhileVoidActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("holdWhileVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("holdWhileVoid", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	X object;
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 5153 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via holdWhileVoid()
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class HoldWhileVoidActor final : public Actor<Void>, public ActorCallback< HoldWhileVoidActor<T, X>, 0, T >, public FastAllocated<HoldWhileVoidActor<T, X>>, public HoldWhileVoidActorState<T, X, HoldWhileVoidActor<T, X>> {
															#line 5160 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<HoldWhileVoidActor<T, X>>::operator new;
	using FastAllocated<HoldWhileVoidActor<T, X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< HoldWhileVoidActor<T, X>, 0, T >;
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	HoldWhileVoidActor(X const& object,Future<T> const& what) 
															#line 5171 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   HoldWhileVoidActorState<T, X, HoldWhileVoidActor<T, X>>(object, what)
	{
		fdb_probe_actor_enter("holdWhileVoid", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("holdWhileVoid");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("holdWhileVoid", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< HoldWhileVoidActor<T, X>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class X>
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> holdWhileVoid( X const& object, Future<T> const& what ) {
															#line 301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new HoldWhileVoidActor<T, X>(object, what));
															#line 5201 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 306 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Assign the future value of what to out
template <class T>
Future<Void> store(T& out, Future<T> what) {
	return map(what, [&out](T const& v) {
		out = v;
		return Void();
	});
}

template <class T>
Future<Void> storeOrThrow(T& out, Future<Optional<T>> what, Error e = key_not_found()) {
	return map(what, [&out, e](Optional<T> const& o) {
		if (!o.present())
			throw e;
		out = o.get();
		return Void();
	});
}

// Waits for a future to be ready, and then applies an asynchronous function to it.
															#line 5226 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via mapAsync()
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class U, class MapAsyncActor>
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapAsyncActorState {
															#line 5233 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapAsyncActorState(Future<T> const& what,F const& actorFunc) 
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   actorFunc(actorFunc)
															#line 5242 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("mapAsync", reinterpret_cast<unsigned long>(this));

	}
	~MapAsyncActorState() 
	{
		fdb_probe_actor_destroy("mapAsync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 329 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 329 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<MapAsyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5259 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MapAsyncActor*>(this)->actor_wait_state = 1;
															#line 329 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MapAsyncActor, 0, T >*>(static_cast<MapAsyncActor*>(this)));
															#line 5264 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~MapAsyncActorState();
		static_cast<MapAsyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val,int loopDepth) 
	{
															#line 330 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<U> __when_expr_1 = actorFunc(val);
															#line 330 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<MapAsyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5289 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MapAsyncActor*>(this)->actor_wait_state = 2;
															#line 330 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MapAsyncActor, 1, U >*>(static_cast<MapAsyncActor*>(this)));
															#line 5294 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T && val,int loopDepth) 
	{
															#line 330 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<U> __when_expr_1 = actorFunc(val);
															#line 330 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<MapAsyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5305 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MapAsyncActor*>(this)->actor_wait_state = 2;
															#line 330 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MapAsyncActor, 1, U >*>(static_cast<MapAsyncActor*>(this)));
															#line 5310 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MapAsyncActor*>(this)->actor_wait_state > 0) static_cast<MapAsyncActor*>(this)->actor_wait_state = 0;
		static_cast<MapAsyncActor*>(this)->ActorCallback< MapAsyncActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< MapAsyncActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MapAsyncActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MapAsyncActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(U const& ret,int loopDepth) 
	{
															#line 331 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MapAsyncActor*>(this)->SAV<U>::futures) { (void)(ret); this->~MapAsyncActorState(); static_cast<MapAsyncActor*>(this)->destroy(); return 0; }
															#line 5382 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MapAsyncActor*>(this)->SAV< U >::value()) U(ret);
		this->~MapAsyncActorState();
		static_cast<MapAsyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(U && ret,int loopDepth) 
	{
															#line 331 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MapAsyncActor*>(this)->SAV<U>::futures) { (void)(ret); this->~MapAsyncActorState(); static_cast<MapAsyncActor*>(this)->destroy(); return 0; }
															#line 5394 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MapAsyncActor*>(this)->SAV< U >::value()) U(ret);
		this->~MapAsyncActorState();
		static_cast<MapAsyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(U const& ret,int loopDepth) 
	{
		loopDepth = a_body1cont2(ret, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(U && ret,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(ret), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MapAsyncActor*>(this)->actor_wait_state > 0) static_cast<MapAsyncActor*>(this)->actor_wait_state = 0;
		static_cast<MapAsyncActor*>(this)->ActorCallback< MapAsyncActor, 1, U >::remove();

	}
	void a_callback_fire(ActorCallback< MapAsyncActor, 1, U >*,U const& value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MapAsyncActor, 1, U >*,U && value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MapAsyncActor, 1, U >*,Error err) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F actorFunc;
															#line 5469 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via mapAsync()
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class U>
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapAsyncActor final : public Actor<U>, public ActorCallback< MapAsyncActor<T, F, U>, 0, T >, public ActorCallback< MapAsyncActor<T, F, U>, 1, U >, public FastAllocated<MapAsyncActor<T, F, U>>, public MapAsyncActorState<T, F, U, MapAsyncActor<T, F, U>> {
															#line 5476 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<MapAsyncActor<T, F, U>>::operator new;
	using FastAllocated<MapAsyncActor<T, F, U>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<U>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MapAsyncActor<T, F, U>, 0, T >;
friend struct ActorCallback< MapAsyncActor<T, F, U>, 1, U >;
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapAsyncActor(Future<T> const& what,F const& actorFunc) 
															#line 5488 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<U>(),
		   MapAsyncActorState<T, F, U, MapAsyncActor<T, F, U>>(what, actorFunc)
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("mapAsync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MapAsyncActor<T, F, U>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MapAsyncActor<T, F, U>, 1, U >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class U>
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<U> mapAsync( Future<T> const& what, F const& actorFunc ) {
															#line 327 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<U>(new MapAsyncActor<T, F, U>(what, actorFunc));
															#line 5519 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 333 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// maps a vector of futures with an asynchronous function
template <class T, class F>
auto mapAsync(std::vector<Future<T>> const& what, F const& actorFunc) {
	std::vector<std::invoke_result_t<F, T>> ret;
	ret.reserve(what.size());
	for (const auto& f : what)
		ret.push_back(mapAsync(f, actorFunc));
	return ret;
}

// maps a stream with an asynchronous function
															#line 5535 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via mapAsync()
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class U, class MapAsyncActor1>
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapAsyncActor1State {
															#line 5542 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapAsyncActor1State(FutureStream<T> const& input,F const& actorFunc,PromiseStream<U> const& output) 
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   actorFunc(actorFunc),
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output),
															#line 347 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   futures()
															#line 5555 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("mapAsync", reinterpret_cast<unsigned long>(this));

	}
	~MapAsyncActor1State() 
	{
		fdb_probe_actor_destroy("mapAsync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 349 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 5570 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~MapAsyncActor1State();
		static_cast<MapAsyncActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 370 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		;
															#line 5593 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		try {
															#line 352 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<T> __when_expr_0 = input;
															#line 351 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<MapAsyncActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5612 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 355 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<U> __when_expr_1 = futures.size() == 0 ? Never() : futures.front();
															#line 5616 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<MapAsyncActor1*>(this)->actor_wait_state = 1;
															#line 352 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< MapAsyncActor1, 0, T >*>(static_cast<MapAsyncActor1*>(this)));
															#line 355 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MapAsyncActor1, 1, U >*>(static_cast<MapAsyncActor1*>(this)));
															#line 5623 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 361 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 5658 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 364 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				output.sendError(e);
															#line 365 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 5668 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& nextInput,int loopDepth) 
	{
															#line 353 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.push_back(actorFunc(nextInput));
															#line 5689 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && nextInput,int loopDepth) 
	{
															#line 353 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.push_back(actorFunc(nextInput));
															#line 5698 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(U const& nextOutput,int loopDepth) 
	{
															#line 356 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(nextOutput);
															#line 357 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 5709 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(U && nextOutput,int loopDepth) 
	{
															#line 356 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(nextOutput);
															#line 357 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 5720 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MapAsyncActor1*>(this)->actor_wait_state > 0) static_cast<MapAsyncActor1*>(this)->actor_wait_state = 0;
		static_cast<MapAsyncActor1*>(this)->ActorSingleCallback< MapAsyncActor1, 0, T >::remove();
		static_cast<MapAsyncActor1*>(this)->ActorCallback< MapAsyncActor1, 1, U >::remove();

	}
	void a_callback_fire(ActorSingleCallback< MapAsyncActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< MapAsyncActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< MapAsyncActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MapAsyncActor1, 1, U >*,U const& value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MapAsyncActor1, 1, U >*,U && value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MapAsyncActor1, 1, U >*,Error err) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont3(int loopDepth) 
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
	int a_body1cont2(int loopDepth) 
	{
															#line 376 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.sendError(end_of_stream());
															#line 378 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MapAsyncActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MapAsyncActor1State(); static_cast<MapAsyncActor1*>(this)->destroy(); return 0; }
															#line 5841 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MapAsyncActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~MapAsyncActor1State();
		static_cast<MapAsyncActor1*>(this)->finishSendAndDelPromiseRef();
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
															#line 370 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!(futures.size()))
															#line 5860 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 371 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<U> __when_expr_2 = futures.front();
															#line 371 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<MapAsyncActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5868 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<MapAsyncActor1*>(this)->actor_wait_state = 2;
															#line 371 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MapAsyncActor1, 2, U >*>(static_cast<MapAsyncActor1*>(this)));
															#line 5873 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

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
	int a_body1cont1loopBody1cont1(U const& nextOutput,int loopDepth) 
	{
															#line 372 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(nextOutput);
															#line 373 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 5897 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(U && nextOutput,int loopDepth) 
	{
															#line 372 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(nextOutput);
															#line 373 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 5908 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(U const& nextOutput,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(nextOutput, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(U && nextOutput,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(nextOutput), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MapAsyncActor1*>(this)->actor_wait_state > 0) static_cast<MapAsyncActor1*>(this)->actor_wait_state = 0;
		static_cast<MapAsyncActor1*>(this)->ActorCallback< MapAsyncActor1, 2, U >::remove();

	}
	void a_callback_fire(ActorCallback< MapAsyncActor1, 2, U >*,U const& value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< MapAsyncActor1, 2, U >*,U && value) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< MapAsyncActor1, 2, U >*,Error err) 
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F actorFunc;
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<U> output;
															#line 347 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Deque<Future<U>> futures;
															#line 5984 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via mapAsync()
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class U>
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapAsyncActor1 final : public Actor<Void>, public ActorSingleCallback< MapAsyncActor1<T, F, U>, 0, T >, public ActorCallback< MapAsyncActor1<T, F, U>, 1, U >, public ActorCallback< MapAsyncActor1<T, F, U>, 2, U >, public FastAllocated<MapAsyncActor1<T, F, U>>, public MapAsyncActor1State<T, F, U, MapAsyncActor1<T, F, U>> {
															#line 5991 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<MapAsyncActor1<T, F, U>>::operator new;
	using FastAllocated<MapAsyncActor1<T, F, U>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< MapAsyncActor1<T, F, U>, 0, T >;
friend struct ActorCallback< MapAsyncActor1<T, F, U>, 1, U >;
friend struct ActorCallback< MapAsyncActor1<T, F, U>, 2, U >;
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapAsyncActor1(FutureStream<T> const& input,F const& actorFunc,PromiseStream<U> const& output) 
															#line 6004 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   MapAsyncActor1State<T, F, U, MapAsyncActor1<T, F, U>>(input, actorFunc, output)
	{
		fdb_probe_actor_enter("mapAsync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("mapAsync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("mapAsync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< MapAsyncActor1<T, F, U>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MapAsyncActor1<T, F, U>, 2, U >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class U>
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> mapAsync( FutureStream<T> const& input, F const& actorFunc, PromiseStream<U> const& output ) {
															#line 345 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new MapAsyncActor1<T, F, U>(input, actorFunc, output));
															#line 6035 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 380 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Waits for a future to be ready, and then applies a function to it.
															#line 6041 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via map()
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class MapActor>
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapActorState {
															#line 6048 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapActorState(Future<T> const& what,F const& func) 
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   func(func)
															#line 6057 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("map", reinterpret_cast<unsigned long>(this));

	}
	~MapActorState() 
	{
		fdb_probe_actor_destroy("map", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 384 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = what;
															#line 384 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<MapActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6074 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MapActor*>(this)->actor_wait_state = 1;
															#line 384 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MapActor, 0, T >*>(static_cast<MapActor*>(this)));
															#line 6079 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~MapActorState();
		static_cast<MapActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val,int loopDepth) 
	{
															#line 385 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MapActor*>(this)->SAV<std::invoke_result_t<F, T>>::futures) { (void)(func(val)); this->~MapActorState(); static_cast<MapActor*>(this)->destroy(); return 0; }
															#line 6102 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MapActor*>(this)->SAV< std::invoke_result_t<F, T> >::value()) std::invoke_result_t<F, T>(func(val));
		this->~MapActorState();
		static_cast<MapActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && val,int loopDepth) 
	{
															#line 385 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MapActor*>(this)->SAV<std::invoke_result_t<F, T>>::futures) { (void)(func(val)); this->~MapActorState(); static_cast<MapActor*>(this)->destroy(); return 0; }
															#line 6114 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MapActor*>(this)->SAV< std::invoke_result_t<F, T> >::value()) std::invoke_result_t<F, T>(func(val));
		this->~MapActorState();
		static_cast<MapActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MapActor*>(this)->actor_wait_state > 0) static_cast<MapActor*>(this)->actor_wait_state = 0;
		static_cast<MapActor*>(this)->ActorCallback< MapActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< MapActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MapActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MapActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F func;
															#line 6189 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via map()
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapActor final : public Actor<std::invoke_result_t<F, T>>, public ActorCallback< MapActor<T, F>, 0, T >, public FastAllocated<MapActor<T, F>>, public MapActorState<T, F, MapActor<T, F>> {
															#line 6196 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<MapActor<T, F>>::operator new;
	using FastAllocated<MapActor<T, F>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::invoke_result_t<F, T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MapActor<T, F>, 0, T >;
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapActor(Future<T> const& what,F const& func) 
															#line 6207 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<std::invoke_result_t<F, T>>(),
		   MapActorState<T, F, MapActor<T, F>>(what, func)
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("map");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MapActor<T, F>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<std::invoke_result_t<F, T>> map( Future<T> const& what, F const& func ) {
															#line 382 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<std::invoke_result_t<F, T>>(new MapActor<T, F>(what, func));
															#line 6237 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 387 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// maps a vector of futures
template <class T, class F>
auto map(std::vector<Future<T>> const& what, F const& func) {
	std::vector<Future<std::invoke_result_t<F, T>>> ret;
	ret.reserve(what.size());
	for (const auto& f : what)
		ret.push_back(map(f, func));
	return ret;
}

// maps a stream
															#line 6253 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via map()
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class MapActor1>
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapActor1State {
															#line 6260 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapActor1State(FutureStream<T> const& input,F const& func,PromiseStream<std::invoke_result_t<F, T>> const& output) 
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   func(func),
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output)
															#line 6271 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("map", reinterpret_cast<unsigned long>(this));

	}
	~MapActor1State() 
	{
		fdb_probe_actor_destroy("map", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 401 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 6286 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~MapActor1State();
		static_cast<MapActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 413 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.sendError(end_of_stream());
															#line 415 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MapActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MapActor1State(); static_cast<MapActor1*>(this)->destroy(); return 0; }
															#line 6311 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MapActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~MapActor1State();
		static_cast<MapActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 403 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<T> __when_expr_0 = input;
															#line 403 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<MapActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6333 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<MapActor1*>(this)->actor_wait_state = 1;
															#line 403 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< MapActor1, 0, T >*>(static_cast<MapActor1*>(this)));
															#line 6338 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 406 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 6373 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 409 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 6381 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(T const& nextInput,int loopDepth) 
	{
															#line 404 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(func(nextInput));
															#line 6396 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(T && nextInput,int loopDepth) 
	{
															#line 404 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(func(nextInput));
															#line 6405 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& nextInput,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(nextInput, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && nextInput,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(nextInput), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MapActor1*>(this)->actor_wait_state > 0) static_cast<MapActor1*>(this)->actor_wait_state = 0;
		static_cast<MapActor1*>(this)->ActorSingleCallback< MapActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< MapActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< MapActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< MapActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
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
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F func;
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<std::invoke_result_t<F, T>> output;
															#line 6492 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via map()
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MapActor1 final : public Actor<Void>, public ActorSingleCallback< MapActor1<T, F>, 0, T >, public FastAllocated<MapActor1<T, F>>, public MapActor1State<T, F, MapActor1<T, F>> {
															#line 6499 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<MapActor1<T, F>>::operator new;
	using FastAllocated<MapActor1<T, F>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< MapActor1<T, F>, 0, T >;
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MapActor1(FutureStream<T> const& input,F const& func,PromiseStream<std::invoke_result_t<F, T>> const& output) 
															#line 6510 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   MapActor1State<T, F, MapActor1<T, F>>(input, func, output)
	{
		fdb_probe_actor_enter("map", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("map");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("map", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< MapActor1<T, F>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> map( FutureStream<T> const& input, F const& func, PromiseStream<std::invoke_result_t<F, T>> const& output ) {
															#line 399 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new MapActor1<T, F>(input, func, output));
															#line 6540 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 417 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// X + Y will wait for X, then wait for and return the result of Y
															#line 6546 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via operatorPlus()
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class A, class B, class OperatorPlusActor>
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class OperatorPlusActorState {
															#line 6553 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	OperatorPlusActorState(Future<A> const& a,Future<B> const& b) 
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : a(a),
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   b(b)
															#line 6562 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("operatorPlus", reinterpret_cast<unsigned long>(this));

	}
	~OperatorPlusActorState() 
	{
		fdb_probe_actor_destroy("operatorPlus", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 421 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<A> __when_expr_0 = a;
															#line 421 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<OperatorPlusActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6579 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OperatorPlusActor*>(this)->actor_wait_state = 1;
															#line 421 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OperatorPlusActor, 0, A >*>(static_cast<OperatorPlusActor*>(this)));
															#line 6584 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~OperatorPlusActorState();
		static_cast<OperatorPlusActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(A const& resultA,int loopDepth) 
	{
															#line 422 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)resultA;
															#line 423 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<B> __when_expr_1 = b;
															#line 423 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<OperatorPlusActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6611 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OperatorPlusActor*>(this)->actor_wait_state = 2;
															#line 423 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OperatorPlusActor, 1, B >*>(static_cast<OperatorPlusActor*>(this)));
															#line 6616 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(A && resultA,int loopDepth) 
	{
															#line 422 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)resultA;
															#line 423 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<B> __when_expr_1 = b;
															#line 423 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<OperatorPlusActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6629 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OperatorPlusActor*>(this)->actor_wait_state = 2;
															#line 423 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OperatorPlusActor, 1, B >*>(static_cast<OperatorPlusActor*>(this)));
															#line 6634 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(A const& resultA,int loopDepth) 
	{
		loopDepth = a_body1cont1(resultA, loopDepth);

		return loopDepth;
	}
	int a_body1when1(A && resultA,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(resultA), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OperatorPlusActor*>(this)->actor_wait_state > 0) static_cast<OperatorPlusActor*>(this)->actor_wait_state = 0;
		static_cast<OperatorPlusActor*>(this)->ActorCallback< OperatorPlusActor, 0, A >::remove();

	}
	void a_callback_fire(ActorCallback< OperatorPlusActor, 0, A >*,A const& value) 
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OperatorPlusActor, 0, A >*,A && value) 
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OperatorPlusActor, 0, A >*,Error err) 
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(B const& resultB,int loopDepth) 
	{
															#line 424 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<OperatorPlusActor*>(this)->SAV<B>::futures) { (void)(resultB); this->~OperatorPlusActorState(); static_cast<OperatorPlusActor*>(this)->destroy(); return 0; }
															#line 6706 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<OperatorPlusActor*>(this)->SAV< B >::value()) B(resultB);
		this->~OperatorPlusActorState();
		static_cast<OperatorPlusActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(B && resultB,int loopDepth) 
	{
															#line 424 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<OperatorPlusActor*>(this)->SAV<B>::futures) { (void)(resultB); this->~OperatorPlusActorState(); static_cast<OperatorPlusActor*>(this)->destroy(); return 0; }
															#line 6718 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<OperatorPlusActor*>(this)->SAV< B >::value()) B(resultB);
		this->~OperatorPlusActorState();
		static_cast<OperatorPlusActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(B const& resultB,int loopDepth) 
	{
		loopDepth = a_body1cont2(resultB, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(B && resultB,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(resultB), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<OperatorPlusActor*>(this)->actor_wait_state > 0) static_cast<OperatorPlusActor*>(this)->actor_wait_state = 0;
		static_cast<OperatorPlusActor*>(this)->ActorCallback< OperatorPlusActor, 1, B >::remove();

	}
	void a_callback_fire(ActorCallback< OperatorPlusActor, 1, B >*,B const& value) 
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OperatorPlusActor, 1, B >*,B && value) 
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OperatorPlusActor, 1, B >*,Error err) 
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<A> a;
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<B> b;
															#line 6793 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via operatorPlus()
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class A, class B>
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class OperatorPlusActor final : public Actor<B>, public ActorCallback< OperatorPlusActor<A, B>, 0, A >, public ActorCallback< OperatorPlusActor<A, B>, 1, B >, public FastAllocated<OperatorPlusActor<A, B>>, public OperatorPlusActorState<A, B, OperatorPlusActor<A, B>> {
															#line 6800 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<OperatorPlusActor<A, B>>::operator new;
	using FastAllocated<OperatorPlusActor<A, B>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<B>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OperatorPlusActor<A, B>, 0, A >;
friend struct ActorCallback< OperatorPlusActor<A, B>, 1, B >;
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	OperatorPlusActor(Future<A> const& a,Future<B> const& b) 
															#line 6812 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<B>(),
		   OperatorPlusActorState<A, B, OperatorPlusActor<A, B>>(a, b)
	{
		fdb_probe_actor_enter("operatorPlus", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("operatorPlus");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("operatorPlus", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OperatorPlusActor<A, B>, 0, A >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OperatorPlusActor<A, B>, 1, B >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class A, class B>
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<B> operatorPlus( Future<A> const& a, Future<B> const& b ) {
															#line 419 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<B>(new OperatorPlusActor<A, B>(a, b));
															#line 6843 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 426 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class A, class B>
Future<B> operator+(Future<A> a, Future<B> b) {
	return operatorPlus(a, b);
}

// Returns if the future returns true, otherwise waits forever.
															#line 6854 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
[[nodiscard]] Future<Void> returnIfTrue( Future<bool> const& f );

#line 434 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Returns if the future, when waited on and then evaluated with the predicate, returns true, otherwise waits forever
template <class T, class F>
Future<Void> returnIfTrue(Future<T> what, F pred) {
	return returnIfTrue(map(what, pred));
}

// filters a stream
															#line 6866 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via filter()
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class FilterActor>
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class FilterActorState {
															#line 6873 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FilterActorState(FutureStream<T> const& input,F const& pred,PromiseStream<T> const& output) 
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   pred(pred),
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output)
															#line 6884 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("filter", reinterpret_cast<unsigned long>(this));

	}
	~FilterActorState() 
	{
		fdb_probe_actor_destroy("filter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 444 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 6899 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~FilterActorState();
		static_cast<FilterActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 457 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.sendError(end_of_stream());
															#line 459 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<FilterActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FilterActorState(); static_cast<FilterActor*>(this)->destroy(); return 0; }
															#line 6924 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<FilterActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FilterActorState();
		static_cast<FilterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<T> __when_expr_0 = input;
															#line 446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<FilterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6946 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<FilterActor*>(this)->actor_wait_state = 1;
															#line 446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< FilterActor, 0, T >*>(static_cast<FilterActor*>(this)));
															#line 6951 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 450 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 6986 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 453 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 6994 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(T const& nextInput,int loopDepth) 
	{
															#line 447 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (func(nextInput))
															#line 7009 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 448 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(nextInput);
															#line 7013 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(T && nextInput,int loopDepth) 
	{
															#line 447 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (func(nextInput))
															#line 7023 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 448 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(nextInput);
															#line 7027 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& nextInput,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(nextInput, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && nextInput,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(nextInput), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FilterActor*>(this)->actor_wait_state > 0) static_cast<FilterActor*>(this)->actor_wait_state = 0;
		static_cast<FilterActor*>(this)->ActorSingleCallback< FilterActor, 0, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FilterActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("filter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("filter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< FilterActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("filter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("filter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< FilterActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("filter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("filter", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
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
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F pred;
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> output;
															#line 7115 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via filter()
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class FilterActor final : public Actor<Void>, public ActorSingleCallback< FilterActor<T, F>, 0, T >, public FastAllocated<FilterActor<T, F>>, public FilterActorState<T, F, FilterActor<T, F>> {
															#line 7122 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<FilterActor<T, F>>::operator new;
	using FastAllocated<FilterActor<T, F>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< FilterActor<T, F>, 0, T >;
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FilterActor(FutureStream<T> const& input,F const& pred,PromiseStream<T> const& output) 
															#line 7133 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   FilterActorState<T, F, FilterActor<T, F>>(input, pred, output)
	{
		fdb_probe_actor_enter("filter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("filter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("filter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< FilterActor<T, F>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> filter( FutureStream<T> const& input, F const& pred, PromiseStream<T> const& output ) {
															#line 442 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new FilterActor<T, F>(input, pred, output));
															#line 7163 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 461 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// filters a stream asynchronously
															#line 7169 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via asyncFilter()
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F, class AsyncFilterActor>
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class AsyncFilterActorState {
															#line 7176 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncFilterActorState(FutureStream<T> const& input,F const& actorPred,PromiseStream<T> const& output) 
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   actorPred(actorPred),
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output),
															#line 465 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   futures(),
															#line 466 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   p()
															#line 7191 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("asyncFilter", reinterpret_cast<unsigned long>(this));

	}
	~AsyncFilterActorState() 
	{
		fdb_probe_actor_destroy("asyncFilter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 468 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 7206 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~AsyncFilterActorState();
		static_cast<AsyncFilterActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 489 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		;
															#line 7229 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		try {
															#line 471 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<T> __when_expr_0 = input;
															#line 470 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<AsyncFilterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7248 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 474 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<bool> __when_expr_1 = futures.size() == 0 ? Never() : futures.front().second;
															#line 7252 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFilterActor*>(this)->actor_wait_state = 1;
															#line 471 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< AsyncFilterActor, 0, T >*>(static_cast<AsyncFilterActor*>(this)));
															#line 474 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFilterActor, 1, bool >*>(static_cast<AsyncFilterActor*>(this)));
															#line 7259 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 481 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 7294 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 7302 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& nextInput,int loopDepth) 
	{
															#line 472 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.emplace_back(nextInput, actorPred(nextInput));
															#line 7323 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && nextInput,int loopDepth) 
	{
															#line 472 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.emplace_back(nextInput, actorPred(nextInput));
															#line 7332 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(bool const& pass,int loopDepth) 
	{
															#line 475 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (pass)
															#line 7341 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(futures.front().first);
															#line 7345 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 477 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 7349 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(bool && pass,int loopDepth) 
	{
															#line 475 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (pass)
															#line 7358 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(futures.front().first);
															#line 7362 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 477 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 7366 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AsyncFilterActor*>(this)->actor_wait_state > 0) static_cast<AsyncFilterActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFilterActor*>(this)->ActorSingleCallback< AsyncFilterActor, 0, T >::remove();
		static_cast<AsyncFilterActor*>(this)->ActorCallback< AsyncFilterActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorSingleCallback< AsyncFilterActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< AsyncFilterActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< AsyncFilterActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AsyncFilterActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AsyncFilterActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AsyncFilterActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont3(int loopDepth) 
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
	int a_body1cont2(int loopDepth) 
	{
															#line 497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.sendError(end_of_stream());
															#line 499 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<AsyncFilterActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AsyncFilterActorState(); static_cast<AsyncFilterActor*>(this)->destroy(); return 0; }
															#line 7487 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<AsyncFilterActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AsyncFilterActorState();
		static_cast<AsyncFilterActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 489 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!(futures.size()))
															#line 7506 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 490 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		p = futures.front();
															#line 491 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<bool> __when_expr_2 = p.second;
															#line 491 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<AsyncFilterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7516 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<AsyncFilterActor*>(this)->actor_wait_state = 2;
															#line 491 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AsyncFilterActor, 2, bool >*>(static_cast<AsyncFilterActor*>(this)));
															#line 7521 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

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
	int a_body1cont1loopBody1cont1(bool const& pass,int loopDepth) 
	{
															#line 492 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (pass)
															#line 7543 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 493 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(p.first);
															#line 7547 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 494 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 7551 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(bool && pass,int loopDepth) 
	{
															#line 492 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (pass)
															#line 7560 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 493 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(p.first);
															#line 7564 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 494 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		futures.pop_front();
															#line 7568 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(bool const& pass,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(pass, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(bool && pass,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(pass), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<AsyncFilterActor*>(this)->actor_wait_state > 0) static_cast<AsyncFilterActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFilterActor*>(this)->ActorCallback< AsyncFilterActor, 2, bool >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFilterActor, 2, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AsyncFilterActor, 2, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< AsyncFilterActor, 2, bool >*,Error err) 
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F actorPred;
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> output;
															#line 465 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Deque<std::pair<T, Future<bool>>> futures;
															#line 466 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::pair<T, Future<bool>> p;
															#line 7646 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via asyncFilter()
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class AsyncFilterActor final : public Actor<Void>, public ActorSingleCallback< AsyncFilterActor<T, F>, 0, T >, public ActorCallback< AsyncFilterActor<T, F>, 1, bool >, public ActorCallback< AsyncFilterActor<T, F>, 2, bool >, public FastAllocated<AsyncFilterActor<T, F>>, public AsyncFilterActorState<T, F, AsyncFilterActor<T, F>> {
															#line 7653 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<AsyncFilterActor<T, F>>::operator new;
	using FastAllocated<AsyncFilterActor<T, F>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< AsyncFilterActor<T, F>, 0, T >;
friend struct ActorCallback< AsyncFilterActor<T, F>, 1, bool >;
friend struct ActorCallback< AsyncFilterActor<T, F>, 2, bool >;
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncFilterActor(FutureStream<T> const& input,F const& actorPred,PromiseStream<T> const& output) 
															#line 7666 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   AsyncFilterActorState<T, F, AsyncFilterActor<T, F>>(input, actorPred, output)
	{
		fdb_probe_actor_enter("asyncFilter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("asyncFilter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("asyncFilter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< AsyncFilterActor<T, F>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AsyncFilterActor<T, F>, 2, bool >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class F>
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> asyncFilter( FutureStream<T> const& input, F const& actorPred, PromiseStream<T> const& output ) {
															#line 463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new AsyncFilterActor<T, F>(input, actorPred, output));
															#line 7697 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 501 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class T>
struct WorkerCache {
	// SOMEDAY: Would we do better to use "unreliable" (at most once) transport for the initialize requests and get rid
	// of this? It doesn't provide true at most once behavior because things are removed from the cache after they have
	// terminated.
	bool exists(UID id) { return id_interface.count(id) != 0; }
	void set(UID id, const Future<T>& onReady) {
		ASSERT(!exists(id));
		id_interface[id] = onReady;
	}
	Future<T> get(UID id) {
		ASSERT(exists(id));
		return id_interface[id];
	}

	Future<Void> removeOnReady(UID id, Future<Void> const& ready) { return removeOnReady(this, id, ready); }

private:
																#line 7720 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via removeOnReady()
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class RemoveOnReadyActor>
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RemoveOnReadyActorState {
															#line 7726 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RemoveOnReadyActorState(WorkerCache* const& self,UID const& id,Future<Void> const& ready) 
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : self(self),
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   id(id),
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   ready(ready)
															#line 7737 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("removeOnReady", reinterpret_cast<unsigned long>(this));

	}
	~RemoveOnReadyActorState() 
	{
		fdb_probe_actor_destroy("removeOnReady", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 522 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = ready;
															#line 522 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<RemoveOnReadyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7755 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<RemoveOnReadyActor*>(this)->actor_wait_state = 1;
															#line 522 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RemoveOnReadyActor, 0, Void >*>(static_cast<RemoveOnReadyActor*>(this)));
															#line 7760 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~RemoveOnReadyActorState();
		static_cast<RemoveOnReadyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 526 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			self->id_interface.erase(id);
															#line 527 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 7792 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 523 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->id_interface.erase(id);
															#line 524 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<RemoveOnReadyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RemoveOnReadyActorState(); static_cast<RemoveOnReadyActor*>(this)->destroy(); return 0; }
															#line 7808 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<RemoveOnReadyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RemoveOnReadyActorState();
		static_cast<RemoveOnReadyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 523 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->id_interface.erase(id);
															#line 524 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<RemoveOnReadyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RemoveOnReadyActorState(); static_cast<RemoveOnReadyActor*>(this)->destroy(); return 0; }
															#line 7822 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<RemoveOnReadyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RemoveOnReadyActorState();
		static_cast<RemoveOnReadyActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<RemoveOnReadyActor*>(this)->actor_wait_state > 0) static_cast<RemoveOnReadyActor*>(this)->actor_wait_state = 0;
		static_cast<RemoveOnReadyActor*>(this)->ActorCallback< RemoveOnReadyActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RemoveOnReadyActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("removeOnReady", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeOnReady", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RemoveOnReadyActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("removeOnReady", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeOnReady", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RemoveOnReadyActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("removeOnReady", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("removeOnReady", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WorkerCache* self;
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UID id;
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> ready;
															#line 7899 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via removeOnReady()
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RemoveOnReadyActor final : public Actor<Void>, public ActorCallback< RemoveOnReadyActor, 0, Void >, public FastAllocated<RemoveOnReadyActor>, public RemoveOnReadyActorState<RemoveOnReadyActor> {
															#line 7904 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<RemoveOnReadyActor>::operator new;
	using FastAllocated<RemoveOnReadyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RemoveOnReadyActor, 0, Void >;
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RemoveOnReadyActor(WorkerCache* const& self,UID const& id,Future<Void> const& ready) 
															#line 7915 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   RemoveOnReadyActorState<RemoveOnReadyActor>(self, id, ready)
	{
		fdb_probe_actor_enter("removeOnReady", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("removeOnReady");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("removeOnReady", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RemoveOnReadyActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> removeOnReady( WorkerCache* const& self, UID const& id, Future<Void> const& ready ) {
															#line 520 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new RemoveOnReadyActor(self, id, ready));
															#line 7942 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 530 "/usr/src/libfdb_c/flow/genericactors.actor.h"

	std::map<UID, Future<T>> id_interface;
};

template <class K, class V>
class AsyncMap : NonCopyable {
public:
	// Represents a complete function from keys to values (K -> V)
	// All values not explicitly inserted map to V()
	// If this isn't appropriate, use V=Optional<X>

	AsyncMap() : defaultValue(), destructing(false) {}

	virtual ~AsyncMap() {
		destructing = true;
		items.clear();
	}

	void set(K const& k, V const& v) {
		auto& i = items[k];
		if (i.value != v)
			setUnconditional(k, v, i);
	}
	void setUnconditional(K const& k, V const& v) { setUnconditional(k, v, items[k]); }
	void triggerAll() {
		std::vector<Promise<Void>> ps;
		for (auto it = items.begin(); it != items.end(); ++it) {
			ps.resize(ps.size() + 1);
			ps.back().swap(it->second.change);
		}
		std::vector<Promise<Void>> noDestroy = ps; // See explanation of noDestroy in setUnconditional()
		for (auto p = ps.begin(); p != ps.end(); ++p)
			p->send(Void());
	}
	void triggerRange(K const& begin, K const& end) {
		std::vector<Promise<Void>> ps;
		for (auto it = items.lower_bound(begin); it != items.end() && it->first < end; ++it) {
			ps.resize(ps.size() + 1);
			ps.back().swap(it->second.change);
		}
		std::vector<Promise<Void>> noDestroy = ps; // See explanation of noDestroy in setUnconditional()
		for (auto p = ps.begin(); p != ps.end(); ++p)
			p->send(Void());
	}
	void trigger(K const& key) {
		if (items.count(key) != 0) {
			auto& i = items[key];
			Promise<Void> trigger;
			i.change.swap(trigger);
			Promise<Void> noDestroy = trigger; // See explanation of noDestroy in setUnconditional()

			if (i.value == defaultValue)
				items.erase(key);

			trigger.send(Void());
		}
	}
	void clear(K const& k) { set(k, V()); }
	V const& get(K const& k) const {
		auto it = items.find(k);
		if (it != items.end())
			return it->second.value;
		else
			return defaultValue;
	}
	int count(K const& k) const {
		auto it = items.find(k);
		if (it != items.end())
			return 1;
		return 0;
	}
	virtual Future<Void> onChange(K const& k) { // throws broken_promise if this is destroyed
		auto& item = items[k];
		if (item.value == defaultValue)
			return destroyOnCancel(this, k, item.change.getFuture());
		return item.change.getFuture();
	}
	std::vector<K> getKeys() const {
		std::vector<K> keys;
		keys.reserve(items.size());
		for (auto i = items.begin(); i != items.end(); ++i)
			keys.push_back(i->first);
		return keys;
	}
	void resetNoWaiting() {
		for (auto i = items.begin(); i != items.end(); ++i)
			ASSERT(i->second.change.getFuture().getFutureReferenceCount() == 1);
		items.clear();
	}

protected:
	// Invariant: Every item in the map either has value!=defaultValue xor a destroyOnCancel actor waiting on
	// change.getFuture()
	struct P {
		V value;
		Promise<Void> change;
		P() : value() {}
	};
	std::map<K, P> items;
	const V defaultValue;
	bool destructing;

	void setUnconditional(K const& k, V const& v, P& i) {
		Promise<Void> trigger;
		i.change.swap(trigger);
		Promise<Void> noDestroy =
		    trigger; // The send(Void()) or even V::operator= could cause destroyOnCancel,
		             // which could undo the change to i.value here.  Keeping the promise reference count >= 2
		             // prevents destroyOnCancel from erasing anything from the map.
		if (v == defaultValue) {
			items.erase(k);
		} else {
			i.value = v;
		}

		trigger.send(Void());
	}

																#line 8064 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via destroyOnCancel()
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class DestroyOnCancelActor>
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DestroyOnCancelActorState {
															#line 8070 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DestroyOnCancelActorState(AsyncMap* const& self,K const& key,Future<Void> const& change) 
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : self(self),
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   key(key),
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   change(change)
															#line 8081 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("destroyOnCancel", reinterpret_cast<unsigned long>(this));

	}
	~DestroyOnCancelActorState() 
	{
		fdb_probe_actor_destroy("destroyOnCancel", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 650 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = change;
															#line 650 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<DestroyOnCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 8099 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DestroyOnCancelActor*>(this)->actor_wait_state = 1;
															#line 650 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DestroyOnCancelActor, 0, Void >*>(static_cast<DestroyOnCancelActor*>(this)));
															#line 8104 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DestroyOnCancelActorState();
		static_cast<DestroyOnCancelActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 653 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled && !self->destructing && change.getFutureReferenceCount() == 1 && change.getPromiseReferenceCount() == 1)
															#line 8134 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 655 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (EXPENSIVE_VALIDATION)
															#line 8138 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				{
															#line 656 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					auto& p = self->items[key];
															#line 657 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					ASSERT(p.change.getFuture() == change);
															#line 8144 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				}
															#line 659 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				self->items.erase(key);
															#line 8148 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 661 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 8152 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 651 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DestroyOnCancelActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DestroyOnCancelActorState(); static_cast<DestroyOnCancelActor*>(this)->destroy(); return 0; }
															#line 8166 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DestroyOnCancelActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DestroyOnCancelActorState();
		static_cast<DestroyOnCancelActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 651 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DestroyOnCancelActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DestroyOnCancelActorState(); static_cast<DestroyOnCancelActor*>(this)->destroy(); return 0; }
															#line 8178 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DestroyOnCancelActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DestroyOnCancelActorState();
		static_cast<DestroyOnCancelActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<DestroyOnCancelActor*>(this)->actor_wait_state > 0) static_cast<DestroyOnCancelActor*>(this)->actor_wait_state = 0;
		static_cast<DestroyOnCancelActor*>(this)->ActorCallback< DestroyOnCancelActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DestroyOnCancelActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("destroyOnCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("destroyOnCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DestroyOnCancelActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("destroyOnCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("destroyOnCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DestroyOnCancelActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("destroyOnCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("destroyOnCancel", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncMap* self;
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	K key;
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> change;
															#line 8255 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via destroyOnCancel()
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DestroyOnCancelActor final : public Actor<Void>, public ActorCallback< DestroyOnCancelActor, 0, Void >, public FastAllocated<DestroyOnCancelActor>, public DestroyOnCancelActorState<DestroyOnCancelActor> {
															#line 8260 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DestroyOnCancelActor>::operator new;
	using FastAllocated<DestroyOnCancelActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DestroyOnCancelActor, 0, Void >;
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DestroyOnCancelActor(AsyncMap* const& self,K const& key,Future<Void> const& change) 
															#line 8271 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   DestroyOnCancelActorState<DestroyOnCancelActor>(self, key, change)
	{
		fdb_probe_actor_enter("destroyOnCancel", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("destroyOnCancel");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("destroyOnCancel", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DestroyOnCancelActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> destroyOnCancel( AsyncMap* const& self, K const& key, Future<Void> const& change ) {
															#line 648 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new DestroyOnCancelActor(self, key, change));
															#line 8298 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 664 "/usr/src/libfdb_c/flow/genericactors.actor.h"
};

template <class V>
class ReferencedObject : NonCopyable, public ReferenceCounted<ReferencedObject<V>> {
public:
	ReferencedObject() : value() {}
	ReferencedObject(V const& v) : value(v) {}
	ReferencedObject(V&& v) : value(std::move(v)) {}
	ReferencedObject(ReferencedObject&& r) : value(std::move(r.value)) {}

	void operator=(ReferencedObject&& r) { value = std::move(r.value); }

	V const& get() const { return value; }

	V& mutate() { return value; }

	void set(V const& v) { value = v; }

	void set(V&& v) { value = std::move(v); }

	static Reference<ReferencedObject<V>> from(V const& v) { return makeReference<ReferencedObject<V>>(v); }

	static Reference<ReferencedObject<V>> from(V&& v) { return makeReference<ReferencedObject<V>>(std::move(v)); }

private:
	V value;
};

template <class V>
class AsyncVar : NonCopyable, public ReferenceCounted<AsyncVar<V>> {
public:
	AsyncVar() : value() {}
	AsyncVar(V const& v) : value(v) {}
	AsyncVar(AsyncVar&& av) : value(std::move(av.value)), nextChange(std::move(av.nextChange)) {}
	void operator=(AsyncVar&& av) {
		value = std::move(av.value);
		nextChange = std::move(av.nextChange);
	}

	V const& get() const { return value; }
	Future<Void> onChange() const { return nextChange.getFuture(); }
	void set(V const& v) {
		if (v != value)
			setUnconditional(v);
	}
	void setUnconditional(V const& v) {
		Promise<Void> t;
		this->nextChange.swap(t);
		this->value = v;
		t.send(Void());
	}
	void trigger() {
		Promise<Void> t;
		this->nextChange.swap(t);
		t.send(Void());
	}

private:
	V value;
	Promise<Void> nextChange;
};

class AsyncTrigger : NonCopyable {
public:
	AsyncTrigger() {}
	AsyncTrigger(AsyncTrigger&& at) : v(std::move(at.v)) {}
	void operator=(AsyncTrigger&& at) { v = std::move(at.v); }
	Future<Void> onTrigger() const { return v.onChange(); }
	void trigger() { v.trigger(); }

private:
	AsyncVar<Void> v;
};

// Binds an AsyncTrigger object to an AsyncVar, so when the AsyncVar changes
// the AsyncTrigger is triggered.
															#line 8378 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forward()
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ForwardActor>
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardActorState {
															#line 8385 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardActorState(Reference<AsyncVar<T> const> const& from,AsyncTrigger* const& to) 
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : from(from),
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   to(to)
															#line 8394 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forward", reinterpret_cast<unsigned long>(this));

	}
	~ForwardActorState() 
	{
		fdb_probe_actor_destroy("forward", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 742 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 8409 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ForwardActorState();
		static_cast<ForwardActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = from->onChange();
															#line 743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<ForwardActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8441 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ForwardActor*>(this)->actor_wait_state = 1;
															#line 743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardActor, 0, Void >*>(static_cast<ForwardActor*>(this)));
															#line 8446 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 744 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		to->trigger();
															#line 8455 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 744 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		to->trigger();
															#line 8464 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<ForwardActor*>(this)->actor_wait_state > 0) static_cast<ForwardActor*>(this)->actor_wait_state = 0;
		static_cast<ForwardActor*>(this)->ActorCallback< ForwardActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<T> const> from;
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncTrigger* to;
															#line 8536 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via forward()
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardActor final : public Actor<Void>, public ActorCallback< ForwardActor<T>, 0, Void >, public FastAllocated<ForwardActor<T>>, public ForwardActorState<T, ForwardActor<T>> {
															#line 8543 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardActor<T>>::operator new;
	using FastAllocated<ForwardActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardActor<T>, 0, Void >;
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardActor(Reference<AsyncVar<T> const> const& from,AsyncTrigger* const& to) 
															#line 8554 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   ForwardActorState<T, ForwardActor<T>>(from, to)
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forward");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ForwardActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> forward( Reference<AsyncVar<T> const> const& from, AsyncTrigger* const& to ) {
															#line 740 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new ForwardActor<T>(from, to));
															#line 8584 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 747 "/usr/src/libfdb_c/flow/genericactors.actor.h"

class Debouncer : NonCopyable {
public:
	explicit Debouncer(double delay) { worker = debounceWorker(this, delay); }
	Debouncer(Debouncer&& at) = default;
	Debouncer& operator=(Debouncer&& at) = default;
	Future<Void> onTrigger() { return output.onChange(); }
	void trigger() { input.setUnconditional(Void()); }

private:
	AsyncVar<Void> input;
	AsyncVar<Void> output;
	Future<Void> worker;

																#line 8602 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via debounceWorker()
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class DebounceWorkerActor>
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DebounceWorkerActorState {
															#line 8608 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DebounceWorkerActorState(Debouncer* const& self,double const& bounceTime) 
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : self(self),
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   bounceTime(bounceTime)
															#line 8617 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("debounceWorker", reinterpret_cast<unsigned long>(this));

	}
	~DebounceWorkerActorState() 
	{
		fdb_probe_actor_destroy("debounceWorker", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 762 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 8632 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DebounceWorkerActorState();
		static_cast<DebounceWorkerActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 763 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = self->input.onChange();
															#line 763 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DebounceWorkerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8664 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DebounceWorkerActor*>(this)->actor_wait_state = 1;
															#line 763 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DebounceWorkerActor, 0, Void >*>(static_cast<DebounceWorkerActor*>(this)));
															#line 8669 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 764 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		;
															#line 8678 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 764 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		;
															#line 8687 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

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
		if (static_cast<DebounceWorkerActor*>(this)->actor_wait_state > 0) static_cast<DebounceWorkerActor*>(this)->actor_wait_state = 0;
		static_cast<DebounceWorkerActor*>(this)->ActorCallback< DebounceWorkerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DebounceWorkerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DebounceWorkerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DebounceWorkerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 772 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->output.setUnconditional(Void());
															#line 8759 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

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
															#line 766 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = self->input.onChange();
															#line 765 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DebounceWorkerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 8777 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
															#line 767 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_2 = delay(bounceTime);
															#line 8781 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when2(__when_expr_2.get(), loopDepth); };
		static_cast<DebounceWorkerActor*>(this)->actor_wait_state = 2;
															#line 766 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DebounceWorkerActor, 1, Void >*>(static_cast<DebounceWorkerActor*>(this)));
															#line 767 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DebounceWorkerActor, 2, Void >*>(static_cast<DebounceWorkerActor*>(this)));
															#line 8788 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when2(Void const& _,int loopDepth) 
	{
		return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when2(Void && _,int loopDepth) 
	{
		return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DebounceWorkerActor*>(this)->actor_wait_state > 0) static_cast<DebounceWorkerActor*>(this)->actor_wait_state = 0;
		static_cast<DebounceWorkerActor*>(this)->ActorCallback< DebounceWorkerActor, 1, Void >::remove();
		static_cast<DebounceWorkerActor*>(this)->ActorCallback< DebounceWorkerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DebounceWorkerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DebounceWorkerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DebounceWorkerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DebounceWorkerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DebounceWorkerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DebounceWorkerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Debouncer* self;
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double bounceTime;
															#line 8937 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via debounceWorker()
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DebounceWorkerActor final : public Actor<Void>, public ActorCallback< DebounceWorkerActor, 0, Void >, public ActorCallback< DebounceWorkerActor, 1, Void >, public ActorCallback< DebounceWorkerActor, 2, Void >, public FastAllocated<DebounceWorkerActor>, public DebounceWorkerActorState<DebounceWorkerActor> {
															#line 8942 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DebounceWorkerActor>::operator new;
	using FastAllocated<DebounceWorkerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DebounceWorkerActor, 0, Void >;
friend struct ActorCallback< DebounceWorkerActor, 1, Void >;
friend struct ActorCallback< DebounceWorkerActor, 2, Void >;
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DebounceWorkerActor(Debouncer* const& self,double const& bounceTime) 
															#line 8955 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   DebounceWorkerActorState<DebounceWorkerActor>(self, bounceTime)
	{
		fdb_probe_actor_enter("debounceWorker", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("debounceWorker");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("debounceWorker", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DebounceWorkerActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DebounceWorkerActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> debounceWorker( Debouncer* const& self, double const& bounceTime ) {
															#line 761 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new DebounceWorkerActor(self, bounceTime));
															#line 8983 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 775 "/usr/src/libfdb_c/flow/genericactors.actor.h"
};

															#line 8989 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via asyncDeserialize()
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class AsyncDeserializeActor>
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class AsyncDeserializeActorState {
															#line 8996 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncDeserializeActorState(Reference<AsyncVar<Standalone<StringRef>>> const& input,Reference<AsyncVar<Optional<T>>> const& output) 
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output)
															#line 9005 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("asyncDeserialize", reinterpret_cast<unsigned long>(this));

	}
	~AsyncDeserializeActorState() 
	{
		fdb_probe_actor_destroy("asyncDeserialize", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 780 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 9020 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~AsyncDeserializeActorState();
		static_cast<AsyncDeserializeActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 781 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (input->get().size())
															#line 9050 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 782 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			ObjectReader reader(input->get().begin(), IncludeVersion());
															#line 783 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			T res;
															#line 784 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			reader.deserialize(res);
															#line 785 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output->set(res);
															#line 9060 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		else
		{
															#line 787 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output->set(Optional<T>());
															#line 9066 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 788 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = input->onChange();
															#line 788 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<AsyncDeserializeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 9072 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AsyncDeserializeActor*>(this)->actor_wait_state = 1;
															#line 788 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncDeserializeActor, 0, Void >*>(static_cast<AsyncDeserializeActor*>(this)));
															#line 9077 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<AsyncDeserializeActor*>(this)->actor_wait_state > 0) static_cast<AsyncDeserializeActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncDeserializeActor*>(this)->ActorCallback< AsyncDeserializeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncDeserializeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("asyncDeserialize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserialize", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AsyncDeserializeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("asyncDeserialize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserialize", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AsyncDeserializeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("asyncDeserialize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncDeserialize", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<Standalone<StringRef>>> input;
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<Optional<T>>> output;
															#line 9161 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via asyncDeserialize()
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class AsyncDeserializeActor final : public Actor<Void>, public ActorCallback< AsyncDeserializeActor<T>, 0, Void >, public FastAllocated<AsyncDeserializeActor<T>>, public AsyncDeserializeActorState<T, AsyncDeserializeActor<T>> {
															#line 9168 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<AsyncDeserializeActor<T>>::operator new;
	using FastAllocated<AsyncDeserializeActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncDeserializeActor<T>, 0, Void >;
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncDeserializeActor(Reference<AsyncVar<Standalone<StringRef>>> const& input,Reference<AsyncVar<Optional<T>>> const& output) 
															#line 9179 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   AsyncDeserializeActorState<T, AsyncDeserializeActor<T>>(input, output)
	{
		fdb_probe_actor_enter("asyncDeserialize", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("asyncDeserialize");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("asyncDeserialize", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AsyncDeserializeActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> asyncDeserialize( Reference<AsyncVar<Standalone<StringRef>>> const& input, Reference<AsyncVar<Optional<T>>> const& output ) {
															#line 777 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new AsyncDeserializeActor<T>(input, output));
															#line 9209 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 791 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 9214 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardVector()
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class V, class T, class ForwardVectorActor>
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardVectorActorState {
															#line 9221 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardVectorActorState(Future<V> const& values,std::vector<Promise<T>> const& out) 
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : values(values),
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   out(out)
															#line 9230 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardVector", reinterpret_cast<unsigned long>(this));

	}
	~ForwardVectorActorState() 
	{
		fdb_probe_actor_destroy("forwardVector", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 794 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<V> __when_expr_0 = values;
															#line 794 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 9247 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			static_cast<ForwardVectorActor*>(this)->actor_wait_state = 1;
															#line 794 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardVectorActor, 0, V >*>(static_cast<ForwardVectorActor*>(this)));
															#line 9251 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<ForwardVectorActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(V const& in,int loopDepth) 
	{
															#line 795 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ASSERT(in.size() == out.size());
															#line 796 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for(int i = 0;i < out.size();i++) {
															#line 797 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			out[i].send(in[i]);
															#line 9277 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(V && in,int loopDepth) 
	{
															#line 795 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ASSERT(in.size() == out.size());
															#line 796 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for(int i = 0;i < out.size();i++) {
															#line 797 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			out[i].send(in[i]);
															#line 9291 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(V const& in,int loopDepth) 
	{
		loopDepth = a_body1cont1(in, loopDepth);

		return loopDepth;
	}
	int a_body1when1(V && in,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(in), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ForwardVectorActor*>(this)->actor_wait_state > 0) static_cast<ForwardVectorActor*>(this)->actor_wait_state = 0;
		static_cast<ForwardVectorActor*>(this)->ActorCallback< ForwardVectorActor, 0, V >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardVectorActor, 0, V >*,V const& value) 
	{
		fdb_probe_actor_enter("forwardVector", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardVector", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardVectorActor, 0, V >*,V && value) 
	{
		fdb_probe_actor_enter("forwardVector", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardVector", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardVectorActor, 0, V >*,Error err) 
	{
		fdb_probe_actor_enter("forwardVector", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardVector", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<ForwardVectorActor*>(this);
															#line 9364 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<V> values;
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::vector<Promise<T>> out;
															#line 9373 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardVector()
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class V, class T>
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardVectorActor final : public Actor<void>, public ActorCallback< ForwardVectorActor<V, T>, 0, V >, public FastAllocated<ForwardVectorActor<V, T>>, public ForwardVectorActorState<V, T, ForwardVectorActor<V, T>> {
															#line 9380 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardVectorActor<V, T>>::operator new;
	using FastAllocated<ForwardVectorActor<V, T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardVectorActor<V, T>, 0, V >;
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardVectorActor(Future<V> const& values,std::vector<Promise<T>> const& out) 
															#line 9391 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   ForwardVectorActorState<V, T, ForwardVectorActor<V, T>>(values, out)
	{
		fdb_probe_actor_enter("forwardVector", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardVector");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardVector", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class V, class T>
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void forwardVector( Future<V> const& values, std::vector<Promise<T>> const& out ) {
															#line 792 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new ForwardVectorActor<V, T>(values, out);
															#line 9412 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 799 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 9417 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via delayedAsyncVar()
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class DelayedAsyncVarActor>
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DelayedAsyncVarActorState {
															#line 9424 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DelayedAsyncVarActorState(Reference<AsyncVar<T>> const& in,Reference<AsyncVar<T>> const& out,double const& time) 
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : in(in),
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   out(out),
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time)
															#line 9435 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("delayedAsyncVar", reinterpret_cast<unsigned long>(this));

	}
	~DelayedAsyncVarActorState() 
	{
		fdb_probe_actor_destroy("delayedAsyncVar", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 803 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				;
															#line 9451 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DelayedAsyncVarActorState();
		static_cast<DelayedAsyncVarActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 809 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			out->set(in->get());
															#line 810 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 9483 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 804 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = delay(time);
															#line 804 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 9506 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state = 1;
															#line 804 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DelayedAsyncVarActor, 0, Void >*>(static_cast<DelayedAsyncVarActor*>(this)));
															#line 9511 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 805 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		out->set(in->get());
															#line 806 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = in->onChange();
															#line 806 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 9524 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state = 2;
															#line 806 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DelayedAsyncVarActor, 1, Void >*>(static_cast<DelayedAsyncVarActor*>(this)));
															#line 9529 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 805 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		out->set(in->get());
															#line 806 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = in->onChange();
															#line 806 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 9542 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state = 2;
															#line 806 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DelayedAsyncVarActor, 1, Void >*>(static_cast<DelayedAsyncVarActor*>(this)));
															#line 9547 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state > 0) static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state = 0;
		static_cast<DelayedAsyncVarActor*>(this)->ActorCallback< DelayedAsyncVarActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DelayedAsyncVarActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DelayedAsyncVarActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DelayedAsyncVarActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
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
		if (static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state > 0) static_cast<DelayedAsyncVarActor*>(this)->actor_wait_state = 0;
		static_cast<DelayedAsyncVarActor*>(this)->ActorCallback< DelayedAsyncVarActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DelayedAsyncVarActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DelayedAsyncVarActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DelayedAsyncVarActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<T>> in;
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<T>> out;
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 9696 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via delayedAsyncVar()
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DelayedAsyncVarActor final : public Actor<Void>, public ActorCallback< DelayedAsyncVarActor<T>, 0, Void >, public ActorCallback< DelayedAsyncVarActor<T>, 1, Void >, public FastAllocated<DelayedAsyncVarActor<T>>, public DelayedAsyncVarActorState<T, DelayedAsyncVarActor<T>> {
															#line 9703 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DelayedAsyncVarActor<T>>::operator new;
	using FastAllocated<DelayedAsyncVarActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DelayedAsyncVarActor<T>, 0, Void >;
friend struct ActorCallback< DelayedAsyncVarActor<T>, 1, Void >;
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DelayedAsyncVarActor(Reference<AsyncVar<T>> const& in,Reference<AsyncVar<T>> const& out,double const& time) 
															#line 9715 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   DelayedAsyncVarActorState<T, DelayedAsyncVarActor<T>>(in, out, time)
	{
		fdb_probe_actor_enter("delayedAsyncVar", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("delayedAsyncVar");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("delayedAsyncVar", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DelayedAsyncVarActor<T>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DelayedAsyncVarActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> delayedAsyncVar( Reference<AsyncVar<T>> const& in, Reference<AsyncVar<T>> const& out, double const& time ) {
															#line 800 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new DelayedAsyncVarActor<T>(in, out, time));
															#line 9746 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 813 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 9751 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via setAfter()
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class SetAfterActor>
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SetAfterActorState {
															#line 9758 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SetAfterActorState(Reference<AsyncVar<T>> const& var,double const& time,T const& val) 
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : var(var),
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time),
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   val(val)
															#line 9769 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("setAfter", reinterpret_cast<unsigned long>(this));

	}
	~SetAfterActorState() 
	{
		fdb_probe_actor_destroy("setAfter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 816 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = delay(time);
															#line 816 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<SetAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9786 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetAfterActor*>(this)->actor_wait_state = 1;
															#line 816 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetAfterActor, 0, Void >*>(static_cast<SetAfterActor*>(this)));
															#line 9791 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~SetAfterActorState();
		static_cast<SetAfterActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 817 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		var->set(val);
															#line 818 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SetAfterActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetAfterActorState(); static_cast<SetAfterActor*>(this)->destroy(); return 0; }
															#line 9816 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SetAfterActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetAfterActorState();
		static_cast<SetAfterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 817 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		var->set(val);
															#line 818 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SetAfterActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetAfterActorState(); static_cast<SetAfterActor*>(this)->destroy(); return 0; }
															#line 9830 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SetAfterActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetAfterActorState();
		static_cast<SetAfterActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SetAfterActor*>(this)->actor_wait_state > 0) static_cast<SetAfterActor*>(this)->actor_wait_state = 0;
		static_cast<SetAfterActor*>(this)->ActorCallback< SetAfterActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetAfterActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetAfterActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetAfterActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setAfter", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<T>> var;
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T val;
															#line 9907 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via setAfter()
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SetAfterActor final : public Actor<Void>, public ActorCallback< SetAfterActor<T>, 0, Void >, public FastAllocated<SetAfterActor<T>>, public SetAfterActorState<T, SetAfterActor<T>> {
															#line 9914 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<SetAfterActor<T>>::operator new;
	using FastAllocated<SetAfterActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetAfterActor<T>, 0, Void >;
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SetAfterActor(Reference<AsyncVar<T>> const& var,double const& time,T const& val) 
															#line 9925 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   SetAfterActorState<T, SetAfterActor<T>>(var, time, val)
	{
		fdb_probe_actor_enter("setAfter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setAfter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setAfter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetAfterActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> setAfter( Reference<AsyncVar<T>> const& var, double const& time, T const& val ) {
															#line 814 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new SetAfterActor<T>(var, time, val));
															#line 9955 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 820 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 9960 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via resetAfter()
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ResetAfterActor>
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ResetAfterActorState {
															#line 9967 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ResetAfterActorState(Reference<AsyncVar<T>> const& var,double const& time,T const& val,int const& warningLimit = -1,double const& warningResetDelay = 0,const char* const& context = nullptr) 
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : var(var),
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time),
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   val(val),
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   warningLimit(warningLimit),
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   warningResetDelay(warningResetDelay),
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   context(context),
															#line 828 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   isEqual(var->get() == val),
															#line 829 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   resetDelay(isEqual ? Never() : delay(time)),
															#line 830 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   resetCount(0),
															#line 831 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   lastReset(now())
															#line 9992 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("resetAfter", reinterpret_cast<unsigned long>(this));

	}
	~ResetAfterActorState() 
	{
		fdb_probe_actor_destroy("resetAfter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 832 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 10007 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ResetAfterActorState();
		static_cast<ResetAfterActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 834 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = resetDelay;
															#line 833 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<ResetAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10039 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 849 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = var->onChange();
															#line 10043 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<ResetAfterActor*>(this)->actor_wait_state = 1;
															#line 834 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResetAfterActor, 0, Void >*>(static_cast<ResetAfterActor*>(this)));
															#line 849 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ResetAfterActor, 1, Void >*>(static_cast<ResetAfterActor*>(this)));
															#line 10050 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 851 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (isEqual && var->get() != val)
															#line 10059 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 852 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			isEqual = false;
															#line 853 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			resetDelay = delay(time);
															#line 10065 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 855 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!isEqual && var->get() == val)
															#line 10069 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 856 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			isEqual = true;
															#line 857 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			resetDelay = Never();
															#line 10075 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 835 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		var->set(val);
															#line 836 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (now() - lastReset > warningResetDelay)
															#line 10087 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 837 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			resetCount = 0;
															#line 10091 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 839 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		resetCount++;
															#line 840 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (context && warningLimit >= 0 && resetCount > warningLimit)
															#line 10097 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 841 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			TraceEvent(SevWarnAlways, context) .detail("ResetCount", resetCount) .detail("LastReset", now() - lastReset);
															#line 10101 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 845 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		lastReset = now();
															#line 846 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		isEqual = true;
															#line 847 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		resetDelay = Never();
															#line 10109 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 835 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		var->set(val);
															#line 836 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (now() - lastReset > warningResetDelay)
															#line 10120 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 837 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			resetCount = 0;
															#line 10124 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 839 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		resetCount++;
															#line 840 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (context && warningLimit >= 0 && resetCount > warningLimit)
															#line 10130 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 841 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			TraceEvent(SevWarnAlways, context) .detail("ResetCount", resetCount) .detail("LastReset", now() - lastReset);
															#line 10134 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 845 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		lastReset = now();
															#line 846 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		isEqual = true;
															#line 847 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		resetDelay = Never();
															#line 10142 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ResetAfterActor*>(this)->actor_wait_state > 0) static_cast<ResetAfterActor*>(this)->actor_wait_state = 0;
		static_cast<ResetAfterActor*>(this)->ActorCallback< ResetAfterActor, 0, Void >::remove();
		static_cast<ResetAfterActor*>(this)->ActorCallback< ResetAfterActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ResetAfterActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResetAfterActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ResetAfterActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResetAfterActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ResetAfterActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ResetAfterActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<T>> var;
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T val;
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int warningLimit;
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double warningResetDelay;
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	const char* context;
															#line 828 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool isEqual;
															#line 829 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> resetDelay;
															#line 830 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int resetCount;
															#line 831 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double lastReset;
															#line 10276 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via resetAfter()
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ResetAfterActor final : public Actor<Void>, public ActorCallback< ResetAfterActor<T>, 0, Void >, public ActorCallback< ResetAfterActor<T>, 1, Void >, public FastAllocated<ResetAfterActor<T>>, public ResetAfterActorState<T, ResetAfterActor<T>> {
															#line 10283 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ResetAfterActor<T>>::operator new;
	using FastAllocated<ResetAfterActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResetAfterActor<T>, 0, Void >;
friend struct ActorCallback< ResetAfterActor<T>, 1, Void >;
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ResetAfterActor(Reference<AsyncVar<T>> const& var,double const& time,T const& val,int const& warningLimit = -1,double const& warningResetDelay = 0,const char* const& context = nullptr) 
															#line 10295 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   ResetAfterActorState<T, ResetAfterActor<T>>(var, time, val, warningLimit, warningResetDelay, context)
	{
		fdb_probe_actor_enter("resetAfter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resetAfter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("resetAfter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ResetAfterActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> resetAfter( Reference<AsyncVar<T>> const& var, double const& time, T const& val, int const& warningLimit = -1, double const& warningResetDelay = 0, const char* const& context = nullptr ) {
															#line 821 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new ResetAfterActor<T>(var, time, val, warningLimit, warningResetDelay, context));
															#line 10325 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 861 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 10330 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via setWhenDoneOrError()
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class SetWhenDoneOrErrorActor>
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SetWhenDoneOrErrorActorState {
															#line 10337 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SetWhenDoneOrErrorActorState(Future<Void> const& condition,Reference<AsyncVar<T>> const& var,T const& val) 
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : condition(condition),
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   var(var),
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   val(val)
															#line 10348 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("setWhenDoneOrError", reinterpret_cast<unsigned long>(this));

	}
	~SetWhenDoneOrErrorActorState() 
	{
		fdb_probe_actor_destroy("setWhenDoneOrError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 865 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = condition;
															#line 865 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<SetWhenDoneOrErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 10366 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<SetWhenDoneOrErrorActor*>(this)->actor_wait_state = 1;
															#line 865 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetWhenDoneOrErrorActor, 0, Void >*>(static_cast<SetWhenDoneOrErrorActor*>(this)));
															#line 10371 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~SetWhenDoneOrErrorActorState();
		static_cast<SetWhenDoneOrErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		var->set(val);
															#line 871 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SetWhenDoneOrErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetWhenDoneOrErrorActorState(); static_cast<SetWhenDoneOrErrorActor*>(this)->destroy(); return 0; }
															#line 10402 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SetWhenDoneOrErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetWhenDoneOrErrorActorState();
		static_cast<SetWhenDoneOrErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 867 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 10415 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 868 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, loopDepth);
															#line 10419 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
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
		if (static_cast<SetWhenDoneOrErrorActor*>(this)->actor_wait_state > 0) static_cast<SetWhenDoneOrErrorActor*>(this)->actor_wait_state = 0;
		static_cast<SetWhenDoneOrErrorActor*>(this)->ActorCallback< SetWhenDoneOrErrorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetWhenDoneOrErrorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetWhenDoneOrErrorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetWhenDoneOrErrorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), 0);

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
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> condition;
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<T>> var;
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T val;
															#line 10525 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via setWhenDoneOrError()
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SetWhenDoneOrErrorActor final : public Actor<Void>, public ActorCallback< SetWhenDoneOrErrorActor<T>, 0, Void >, public FastAllocated<SetWhenDoneOrErrorActor<T>>, public SetWhenDoneOrErrorActorState<T, SetWhenDoneOrErrorActor<T>> {
															#line 10532 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<SetWhenDoneOrErrorActor<T>>::operator new;
	using FastAllocated<SetWhenDoneOrErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetWhenDoneOrErrorActor<T>, 0, Void >;
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SetWhenDoneOrErrorActor(Future<Void> const& condition,Reference<AsyncVar<T>> const& var,T const& val) 
															#line 10543 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   SetWhenDoneOrErrorActorState<T, SetWhenDoneOrErrorActor<T>>(condition, var, val)
	{
		fdb_probe_actor_enter("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setWhenDoneOrError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setWhenDoneOrError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetWhenDoneOrErrorActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> setWhenDoneOrError( Future<Void> const& condition, Reference<AsyncVar<T>> const& var, T const& val ) {
															#line 862 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new SetWhenDoneOrErrorActor<T>(condition, var, val));
															#line 10573 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 873 "/usr/src/libfdb_c/flow/genericactors.actor.h"

Future<bool> allTrue(const std::vector<Future<bool>>& all);
Future<Void> anyTrue(std::vector<Reference<AsyncVar<bool>>> const& input, Reference<AsyncVar<bool>> const& output);
Future<Void> cancelOnly(std::vector<Future<Void>> const& futures);
Future<Void> timeoutWarningCollector(FutureStream<Void> const& input,
                                     double const& logDelay,
                                     const char* const& context,
                                     UID const& id);
Future<bool> quorumEqualsTrue(std::vector<Future<bool>> const& futures, int const& required);
Future<Void> lowPriorityDelay(double const& waitTime);

															#line 10588 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via streamHelper()
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class StreamHelperActor>
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class StreamHelperActorState {
															#line 10595 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	StreamHelperActorState(PromiseStream<T> const& output,PromiseStream<Error> const& errors,Future<T> const& input) 
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : output(output),
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   errors(errors),
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   input(input)
															#line 10606 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("streamHelper", reinterpret_cast<unsigned long>(this));

	}
	~StreamHelperActorState() 
	{
		fdb_probe_actor_destroy("streamHelper", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 887 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 887 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<StreamHelperActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 10624 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<StreamHelperActor*>(this)->actor_wait_state = 1;
															#line 887 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< StreamHelperActor, 0, T >*>(static_cast<StreamHelperActor*>(this)));
															#line 10629 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~StreamHelperActorState();
		static_cast<StreamHelperActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 894 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<StreamHelperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~StreamHelperActorState(); static_cast<StreamHelperActor*>(this)->destroy(); return 0; }
															#line 10658 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<StreamHelperActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~StreamHelperActorState();
		static_cast<StreamHelperActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 890 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 10671 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 891 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, loopDepth);
															#line 10675 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 892 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			errors.send(e);
															#line 10679 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 888 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(value);
															#line 10694 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 888 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(value);
															#line 10703 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<StreamHelperActor*>(this)->actor_wait_state > 0) static_cast<StreamHelperActor*>(this)->actor_wait_state = 0;
		static_cast<StreamHelperActor*>(this)->ActorCallback< StreamHelperActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< StreamHelperActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("streamHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("streamHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< StreamHelperActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("streamHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("streamHelper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< StreamHelperActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("streamHelper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("streamHelper", reinterpret_cast<unsigned long>(this), 0);

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
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> output;
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<Error> errors;
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> input;
															#line 10790 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via streamHelper()
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class StreamHelperActor final : public Actor<Void>, public ActorCallback< StreamHelperActor<T>, 0, T >, public FastAllocated<StreamHelperActor<T>>, public StreamHelperActorState<T, StreamHelperActor<T>> {
															#line 10797 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<StreamHelperActor<T>>::operator new;
	using FastAllocated<StreamHelperActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< StreamHelperActor<T>, 0, T >;
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	StreamHelperActor(PromiseStream<T> const& output,PromiseStream<Error> const& errors,Future<T> const& input) 
															#line 10808 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   StreamHelperActorState<T, StreamHelperActor<T>>(output, errors, input)
	{
		fdb_probe_actor_enter("streamHelper", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("streamHelper");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("streamHelper", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< StreamHelperActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> streamHelper( PromiseStream<T> const& output, PromiseStream<Error> const& errors, Future<T> const& input ) {
															#line 884 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new StreamHelperActor<T>(output, errors, input));
															#line 10838 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 896 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class T>
Future<Void> makeStream(const std::vector<Future<T>>& futures, PromiseStream<T>& stream, PromiseStream<Error>& errors) {
	std::vector<Future<Void>> forwarders;
	forwarders.reserve(futures.size());
	for (int f = 0; f < futures.size(); f++)
		forwarders.push_back(streamHelper(stream, errors, futures[f]));
	return cancelOnly(forwarders);
}

template <class T>
class QuorumCallback;

template <class T>
struct Quorum final : SAV<Void> {
	int antiQuorum;
	int count;

	static inline int sizeFor(int count) { return sizeof(Quorum<T>) + sizeof(QuorumCallback<T>) * count; }

	void destroy() override {
		int size = sizeFor(this->count);
		this->~Quorum();
		freeFast(size, this);
	}
	void cancel() override {
		int cancelled_callbacks = 0;
		for (int i = 0; i < count; i++)
			if (callbacks()[i].next) {
				callbacks()[i].remove();
				callbacks()[i].next = 0;
				++cancelled_callbacks;
			}
		if (canBeSet())
			sendError(actor_cancelled());
		for (int i = 0; i < cancelled_callbacks; i++)
			delPromiseRef();
	}
	explicit Quorum(int quorum, int count) : SAV<Void>(1, count), antiQuorum(count - quorum + 1), count(count) {
		if (!quorum)
			this->send(Void());
	}
	void oneSuccess() {
		if (getPromiseReferenceCount() == antiQuorum && canBeSet())
			this->sendAndDelPromiseRef(Void());
		else
			delPromiseRef();
	}
	void oneError(Error err) {
		if (canBeSet())
			this->sendErrorAndDelPromiseRef(err);
		else
			delPromiseRef();
	}

	QuorumCallback<T>* callbacks() { return (QuorumCallback<T>*)(this + 1); }
};

template <class T>
class QuorumCallback : public Callback<T> {
public:
	void fire(const T& value) override {
		Callback<T>::remove();
		Callback<T>::next = 0;
		head->oneSuccess();
	}
	void error(Error error) override {
		Callback<T>::remove();
		Callback<T>::next = 0;
		head->oneError(error);
	}

private:
	template <class U>
	friend Future<Void> quorum(const Future<U>* pItems, int itemCount, int n);
	Quorum<T>* head;
	QuorumCallback() = default;
	QuorumCallback(Future<T> future, Quorum<T>* head) : head(head) { future.addCallbackAndClear(this); }
};

template <class T>
Future<Void> quorum(const Future<T>* pItems, int itemCount, int n) {
	ASSERT(n >= 0 && n <= itemCount);

	int size = Quorum<T>::sizeFor(itemCount);
	Quorum<T>* q = new (allocateFast(size)) Quorum<T>(n, itemCount);

	QuorumCallback<T>* nextCallback = q->callbacks();
	for (int i = 0; i < itemCount; ++i) {
		auto& r = pItems[i];
		if (r.isReady()) {
			new (nextCallback) QuorumCallback<T>();
			nextCallback->next = 0;
			if (r.isError())
				q->oneError(r.getError());
			else
				q->oneSuccess();
		} else
			new (nextCallback) QuorumCallback<T>(r, q);
		++nextCallback;
	}
	return Future<Void>(q);
}

template <class T>
Future<Void> quorum(std::vector<Future<T>> const& results, int n) {
	return quorum(&results.front(), results.size(), n);
}

															#line 10951 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via smartQuorum()
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class SmartQuorumActor>
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SmartQuorumActorState {
															#line 10958 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SmartQuorumActorState(std::vector<Future<T>> const& results,int const& required,double const& extraSeconds,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : results(results),
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   required(required),
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   extraSeconds(extraSeconds),
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID)
															#line 10971 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("smartQuorum", reinterpret_cast<unsigned long>(this));

	}
	~SmartQuorumActorState() 
	{
		fdb_probe_actor_destroy("smartQuorum", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1010 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (results.empty() && required == 0)
															#line 10986 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1011 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (!static_cast<SmartQuorumActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SmartQuorumActorState(); static_cast<SmartQuorumActor*>(this)->destroy(); return 0; }
															#line 10990 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				new (&static_cast<SmartQuorumActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~SmartQuorumActorState();
				static_cast<SmartQuorumActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1012 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = quorum(results, required);
															#line 1012 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<SmartQuorumActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11000 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SmartQuorumActor*>(this)->actor_wait_state = 1;
															#line 1012 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SmartQuorumActor, 0, Void >*>(static_cast<SmartQuorumActor*>(this)));
															#line 11005 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~SmartQuorumActorState();
		static_cast<SmartQuorumActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1014 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = quorum(results, (int)results.size());
															#line 1013 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<SmartQuorumActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11030 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1017 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_2 = delay(extraSeconds, taskID);
															#line 11034 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
		static_cast<SmartQuorumActor*>(this)->actor_wait_state = 2;
															#line 1014 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SmartQuorumActor, 1, Void >*>(static_cast<SmartQuorumActor*>(this)));
															#line 1017 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SmartQuorumActor, 2, Void >*>(static_cast<SmartQuorumActor*>(this)));
															#line 11041 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1014 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = quorum(results, (int)results.size());
															#line 1013 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<SmartQuorumActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11052 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1017 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_2 = delay(extraSeconds, taskID);
															#line 11056 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
		static_cast<SmartQuorumActor*>(this)->actor_wait_state = 2;
															#line 1014 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SmartQuorumActor, 1, Void >*>(static_cast<SmartQuorumActor*>(this)));
															#line 1017 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SmartQuorumActor, 2, Void >*>(static_cast<SmartQuorumActor*>(this)));
															#line 11063 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<SmartQuorumActor*>(this)->actor_wait_state > 0) static_cast<SmartQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<SmartQuorumActor*>(this)->ActorCallback< SmartQuorumActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SmartQuorumActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SmartQuorumActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SmartQuorumActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
															#line 1015 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SmartQuorumActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SmartQuorumActorState(); static_cast<SmartQuorumActor*>(this)->destroy(); return 0; }
															#line 11135 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SmartQuorumActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SmartQuorumActorState();
		static_cast<SmartQuorumActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
															#line 1015 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SmartQuorumActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SmartQuorumActorState(); static_cast<SmartQuorumActor*>(this)->destroy(); return 0; }
															#line 11147 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SmartQuorumActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SmartQuorumActorState();
		static_cast<SmartQuorumActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
															#line 1018 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SmartQuorumActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SmartQuorumActorState(); static_cast<SmartQuorumActor*>(this)->destroy(); return 0; }
															#line 11159 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SmartQuorumActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SmartQuorumActorState();
		static_cast<SmartQuorumActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
															#line 1018 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SmartQuorumActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SmartQuorumActorState(); static_cast<SmartQuorumActor*>(this)->destroy(); return 0; }
															#line 11171 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SmartQuorumActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SmartQuorumActorState();
		static_cast<SmartQuorumActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SmartQuorumActor*>(this)->actor_wait_state > 0) static_cast<SmartQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<SmartQuorumActor*>(this)->ActorCallback< SmartQuorumActor, 1, Void >::remove();
		static_cast<SmartQuorumActor*>(this)->ActorCallback< SmartQuorumActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SmartQuorumActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SmartQuorumActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SmartQuorumActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SmartQuorumActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SmartQuorumActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SmartQuorumActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::vector<Future<T>> results;
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int required;
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double extraSeconds;
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 11284 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via smartQuorum()
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SmartQuorumActor final : public Actor<Void>, public ActorCallback< SmartQuorumActor<T>, 0, Void >, public ActorCallback< SmartQuorumActor<T>, 1, Void >, public ActorCallback< SmartQuorumActor<T>, 2, Void >, public FastAllocated<SmartQuorumActor<T>>, public SmartQuorumActorState<T, SmartQuorumActor<T>> {
															#line 11291 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<SmartQuorumActor<T>>::operator new;
	using FastAllocated<SmartQuorumActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SmartQuorumActor<T>, 0, Void >;
friend struct ActorCallback< SmartQuorumActor<T>, 1, Void >;
friend struct ActorCallback< SmartQuorumActor<T>, 2, Void >;
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SmartQuorumActor(std::vector<Future<T>> const& results,int const& required,double const& extraSeconds,TaskPriority const& taskID = TaskPriority::DefaultDelay) 
															#line 11304 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   SmartQuorumActorState<T, SmartQuorumActor<T>>(results, required, extraSeconds, taskID)
	{
		fdb_probe_actor_enter("smartQuorum", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("smartQuorum");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("smartQuorum", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SmartQuorumActor<T>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SmartQuorumActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> smartQuorum( std::vector<Future<T>> const& results, int const& required, double const& extraSeconds, TaskPriority const& taskID = TaskPriority::DefaultDelay ) {
															#line 1005 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new SmartQuorumActor<T>(results, required, extraSeconds, taskID));
															#line 11335 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1022 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class T>
Future<Void> waitForAll(std::vector<Future<T>> const& results) {
	if (results.empty())
		return Void();
	return quorum(results, (int)results.size());
}

// Wait for all futures in results to be ready and then throw the first (in execution order) error
// if any of them resulted in an error.
template <class T>
Future<Void> waitForAllReadyThenThrow(std::vector<Future<T>> const& results) {
	Future<Void> f = waitForAll(results);
	Future<Void> fReady = waitForAllReady(results);
	return fReady + f;
}

template <class T>
Future<Void> waitForAny(std::vector<Future<T>> const& results) {
	if (results.empty())
		return Void();
	return quorum(results, 1);
}

															#line 11363 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
[[nodiscard]] Future<bool> shortCircuitAny( std::vector<Future<bool>> const& f );

#line 1047 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 11368 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via getAll()
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class GetAllActor>
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class GetAllActorState {
															#line 11375 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	GetAllActorState(std::vector<Future<T>> const& input) 
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input)
															#line 11382 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("getAll", reinterpret_cast<unsigned long>(this));

	}
	~GetAllActorState() 
	{
		fdb_probe_actor_destroy("getAll", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1050 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (input.empty())
															#line 11397 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1051 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (!static_cast<GetAllActor*>(this)->SAV<std::vector<T>>::futures) { (void)(std::vector<T>()); this->~GetAllActorState(); static_cast<GetAllActor*>(this)->destroy(); return 0; }
															#line 11401 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				new (&static_cast<GetAllActor*>(this)->SAV< std::vector<T> >::value()) std::vector<T>(std::vector<T>());
				this->~GetAllActorState();
				static_cast<GetAllActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1052 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = quorum(input, input.size());
															#line 1052 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<GetAllActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11411 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAllActor*>(this)->actor_wait_state = 1;
															#line 1052 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAllActor, 0, Void >*>(static_cast<GetAllActor*>(this)));
															#line 11416 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~GetAllActorState();
		static_cast<GetAllActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1054 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		std::vector<T> output;
															#line 1055 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.reserve(input.size());
															#line 1056 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for(int i = 0;i < input.size();i++) {
															#line 1057 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.push_back(input[i].get());
															#line 11445 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 1058 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<GetAllActor*>(this)->SAV<std::vector<T>>::futures) { (void)(output); this->~GetAllActorState(); static_cast<GetAllActor*>(this)->destroy(); return 0; }
															#line 11449 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<GetAllActor*>(this)->SAV< std::vector<T> >::value()) std::vector<T>(output);
		this->~GetAllActorState();
		static_cast<GetAllActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1054 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		std::vector<T> output;
															#line 1055 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.reserve(input.size());
															#line 1056 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for(int i = 0;i < input.size();i++) {
															#line 1057 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.push_back(input[i].get());
															#line 11467 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 1058 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<GetAllActor*>(this)->SAV<std::vector<T>>::futures) { (void)(output); this->~GetAllActorState(); static_cast<GetAllActor*>(this)->destroy(); return 0; }
															#line 11471 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<GetAllActor*>(this)->SAV< std::vector<T> >::value()) std::vector<T>(output);
		this->~GetAllActorState();
		static_cast<GetAllActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetAllActor*>(this)->actor_wait_state > 0) static_cast<GetAllActor*>(this)->actor_wait_state = 0;
		static_cast<GetAllActor*>(this)->ActorCallback< GetAllActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAll", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAll", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAllActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAll", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAll", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAllActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAll", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAll", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::vector<Future<T>> input;
															#line 11544 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via getAll()
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class GetAllActor final : public Actor<std::vector<T>>, public ActorCallback< GetAllActor<T>, 0, Void >, public FastAllocated<GetAllActor<T>>, public GetAllActorState<T, GetAllActor<T>> {
															#line 11551 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<GetAllActor<T>>::operator new;
	using FastAllocated<GetAllActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAllActor<T>, 0, Void >;
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	GetAllActor(std::vector<Future<T>> const& input) 
															#line 11562 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<std::vector<T>>(),
		   GetAllActorState<T, GetAllActor<T>>(input)
	{
		fdb_probe_actor_enter("getAll", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAll");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAll", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAllActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<std::vector<T>> getAll( std::vector<Future<T>> const& input ) {
															#line 1048 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<std::vector<T>>(new GetAllActor<T>(input));
															#line 11592 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1060 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 11597 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via appendAll()
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class AppendAllActor>
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class AppendAllActorState {
															#line 11604 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AppendAllActorState(std::vector<Future<std::vector<T>>> const& input) 
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input)
															#line 11611 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("appendAll", reinterpret_cast<unsigned long>(this));

	}
	~AppendAllActorState() 
	{
		fdb_probe_actor_destroy("appendAll", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1063 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = quorum(input, input.size());
															#line 1063 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<AppendAllActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11628 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AppendAllActor*>(this)->actor_wait_state = 1;
															#line 1063 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AppendAllActor, 0, Void >*>(static_cast<AppendAllActor*>(this)));
															#line 11633 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~AppendAllActorState();
		static_cast<AppendAllActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1065 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		std::vector<T> output;
															#line 1066 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		size_t sz = 0;
															#line 1067 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for( const auto& f : input ) {
															#line 1068 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			sz += f.get().size();
															#line 11662 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 1070 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.reserve(sz);
															#line 1072 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for(int i = 0;i < input.size();i++) {
															#line 1073 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			auto const& r = input[i].get();
															#line 1074 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.insert(output.end(), r.begin(), r.end());
															#line 11672 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 1076 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<AppendAllActor*>(this)->SAV<std::vector<T>>::futures) { (void)(output); this->~AppendAllActorState(); static_cast<AppendAllActor*>(this)->destroy(); return 0; }
															#line 11676 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<AppendAllActor*>(this)->SAV< std::vector<T> >::value()) std::vector<T>(output);
		this->~AppendAllActorState();
		static_cast<AppendAllActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1065 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		std::vector<T> output;
															#line 1066 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		size_t sz = 0;
															#line 1067 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for( const auto& f : input ) {
															#line 1068 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			sz += f.get().size();
															#line 11694 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 1070 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.reserve(sz);
															#line 1072 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		for(int i = 0;i < input.size();i++) {
															#line 1073 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			auto const& r = input[i].get();
															#line 1074 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.insert(output.end(), r.begin(), r.end());
															#line 11704 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
															#line 1076 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<AppendAllActor*>(this)->SAV<std::vector<T>>::futures) { (void)(output); this->~AppendAllActorState(); static_cast<AppendAllActor*>(this)->destroy(); return 0; }
															#line 11708 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<AppendAllActor*>(this)->SAV< std::vector<T> >::value()) std::vector<T>(output);
		this->~AppendAllActorState();
		static_cast<AppendAllActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AppendAllActor*>(this)->actor_wait_state > 0) static_cast<AppendAllActor*>(this)->actor_wait_state = 0;
		static_cast<AppendAllActor*>(this)->ActorCallback< AppendAllActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AppendAllActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("appendAll", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendAll", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AppendAllActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("appendAll", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendAll", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AppendAllActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("appendAll", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendAll", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::vector<Future<std::vector<T>>> input;
															#line 11781 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via appendAll()
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class AppendAllActor final : public Actor<std::vector<T>>, public ActorCallback< AppendAllActor<T>, 0, Void >, public FastAllocated<AppendAllActor<T>>, public AppendAllActorState<T, AppendAllActor<T>> {
															#line 11788 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<AppendAllActor<T>>::operator new;
	using FastAllocated<AppendAllActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<T>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AppendAllActor<T>, 0, Void >;
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AppendAllActor(std::vector<Future<std::vector<T>>> const& input) 
															#line 11799 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<std::vector<T>>(),
		   AppendAllActorState<T, AppendAllActor<T>>(input)
	{
		fdb_probe_actor_enter("appendAll", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("appendAll");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("appendAll", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AppendAllActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<std::vector<T>> appendAll( std::vector<Future<std::vector<T>>> const& input ) {
															#line 1061 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<std::vector<T>>(new AppendAllActor<T>(input));
															#line 11829 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1078 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 11834 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via onEqual()
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class OnEqualActor>
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class OnEqualActorState {
															#line 11841 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	OnEqualActorState(Future<T> const& in,T const& equalTo) 
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : in(in),
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   equalTo(equalTo)
															#line 11850 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("onEqual", reinterpret_cast<unsigned long>(this));

	}
	~OnEqualActorState() 
	{
		fdb_probe_actor_destroy("onEqual", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1081 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = in;
															#line 1081 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<OnEqualActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11867 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnEqualActor*>(this)->actor_wait_state = 1;
															#line 1081 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnEqualActor, 0, T >*>(static_cast<OnEqualActor*>(this)));
															#line 11872 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~OnEqualActorState();
		static_cast<OnEqualActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& t,int loopDepth) 
	{
															#line 1082 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (t == equalTo)
															#line 11895 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 1083 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!static_cast<OnEqualActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnEqualActorState(); static_cast<OnEqualActor*>(this)->destroy(); return 0; }
															#line 11899 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			new (&static_cast<OnEqualActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~OnEqualActorState();
			static_cast<OnEqualActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1084 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = Never();
															#line 1084 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<OnEqualActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11909 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnEqualActor*>(this)->actor_wait_state = 2;
															#line 1084 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnEqualActor, 1, Void >*>(static_cast<OnEqualActor*>(this)));
															#line 11914 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T && t,int loopDepth) 
	{
															#line 1082 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (t == equalTo)
															#line 11923 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 1083 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!static_cast<OnEqualActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnEqualActorState(); static_cast<OnEqualActor*>(this)->destroy(); return 0; }
															#line 11927 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			new (&static_cast<OnEqualActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~OnEqualActorState();
			static_cast<OnEqualActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1084 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = Never();
															#line 1084 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<OnEqualActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11937 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnEqualActor*>(this)->actor_wait_state = 2;
															#line 1084 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnEqualActor, 1, Void >*>(static_cast<OnEqualActor*>(this)));
															#line 11942 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
		loopDepth = a_body1cont1(t, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(t), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OnEqualActor*>(this)->actor_wait_state > 0) static_cast<OnEqualActor*>(this)->actor_wait_state = 0;
		static_cast<OnEqualActor*>(this)->ActorCallback< OnEqualActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< OnEqualActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnEqualActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnEqualActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1085 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 12014 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1085 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 12022 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

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
		if (static_cast<OnEqualActor*>(this)->actor_wait_state > 0) static_cast<OnEqualActor*>(this)->actor_wait_state = 0;
		static_cast<OnEqualActor*>(this)->ActorCallback< OnEqualActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnEqualActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnEqualActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnEqualActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> in;
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T equalTo;
															#line 12093 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via onEqual()
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class OnEqualActor final : public Actor<Void>, public ActorCallback< OnEqualActor<T>, 0, T >, public ActorCallback< OnEqualActor<T>, 1, Void >, public FastAllocated<OnEqualActor<T>>, public OnEqualActorState<T, OnEqualActor<T>> {
															#line 12100 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<OnEqualActor<T>>::operator new;
	using FastAllocated<OnEqualActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnEqualActor<T>, 0, T >;
friend struct ActorCallback< OnEqualActor<T>, 1, Void >;
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	OnEqualActor(Future<T> const& in,T const& equalTo) 
															#line 12112 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   OnEqualActorState<T, OnEqualActor<T>>(in, equalTo)
	{
		fdb_probe_actor_enter("onEqual", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onEqual");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onEqual", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnEqualActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnEqualActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> onEqual( Future<T> const& in, T const& equalTo ) {
															#line 1079 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new OnEqualActor<T>(in, equalTo));
															#line 12143 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1087 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 12148 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via success()
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class SuccessActor>
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SuccessActorState {
															#line 12155 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SuccessActorState(Future<T> const& of) 
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : of(of)
															#line 12162 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("success", reinterpret_cast<unsigned long>(this));

	}
	~SuccessActorState() 
	{
		fdb_probe_actor_destroy("success", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1090 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = of;
															#line 1090 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<SuccessActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12179 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SuccessActor*>(this)->actor_wait_state = 1;
															#line 1090 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SuccessActor, 0, T >*>(static_cast<SuccessActor*>(this)));
															#line 12184 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~SuccessActorState();
		static_cast<SuccessActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& t,int loopDepth) 
	{
															#line 1091 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)t;
															#line 1092 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SuccessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SuccessActorState(); static_cast<SuccessActor*>(this)->destroy(); return 0; }
															#line 12209 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SuccessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SuccessActorState();
		static_cast<SuccessActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && t,int loopDepth) 
	{
															#line 1091 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)t;
															#line 1092 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SuccessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SuccessActorState(); static_cast<SuccessActor*>(this)->destroy(); return 0; }
															#line 12223 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SuccessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SuccessActorState();
		static_cast<SuccessActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
		loopDepth = a_body1cont1(t, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(t), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SuccessActor*>(this)->actor_wait_state > 0) static_cast<SuccessActor*>(this)->actor_wait_state = 0;
		static_cast<SuccessActor*>(this)->ActorCallback< SuccessActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< SuccessActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("success", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("success", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SuccessActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("success", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("success", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SuccessActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("success", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("success", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> of;
															#line 12296 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via success()
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SuccessActor final : public Actor<Void>, public ActorCallback< SuccessActor<T>, 0, T >, public FastAllocated<SuccessActor<T>>, public SuccessActorState<T, SuccessActor<T>> {
															#line 12303 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<SuccessActor<T>>::operator new;
	using FastAllocated<SuccessActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SuccessActor<T>, 0, T >;
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SuccessActor(Future<T> const& of) 
															#line 12314 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   SuccessActorState<T, SuccessActor<T>>(of)
	{
		fdb_probe_actor_enter("success", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("success");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("success", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SuccessActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> success( Future<T> const& of ) {
															#line 1088 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new SuccessActor<T>(of));
															#line 12344 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1094 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 12349 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via ready()
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ReadyActor>
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ReadyActorState {
															#line 12356 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ReadyActorState(Future<T> const& f) 
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f)
															#line 12363 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("ready", reinterpret_cast<unsigned long>(this));

	}
	~ReadyActorState() 
	{
		fdb_probe_actor_destroy("ready", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1098 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = f;
															#line 1098 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<ReadyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 12381 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadyActor*>(this)->actor_wait_state = 1;
															#line 1098 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadyActor, 0, T >*>(static_cast<ReadyActor*>(this)));
															#line 12386 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ReadyActorState();
		static_cast<ReadyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1102 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ReadyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadyActorState(); static_cast<ReadyActor*>(this)->destroy(); return 0; }
															#line 12415 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ReadyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadyActorState();
		static_cast<ReadyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
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
	int a_body1cont2(T const& t,int loopDepth) 
	{
															#line 1099 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)t;
															#line 12440 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 1099 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		(void)t;
															#line 12449 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

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
		if (static_cast<ReadyActor*>(this)->actor_wait_state > 0) static_cast<ReadyActor*>(this)->actor_wait_state = 0;
		static_cast<ReadyActor*>(this)->ActorCallback< ReadyActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ReadyActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("ready", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ready", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadyActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("ready", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ready", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadyActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("ready", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ready", reinterpret_cast<unsigned long>(this), 0);

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
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 12532 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via ready()
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ReadyActor final : public Actor<Void>, public ActorCallback< ReadyActor<T>, 0, T >, public FastAllocated<ReadyActor<T>>, public ReadyActorState<T, ReadyActor<T>> {
															#line 12539 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ReadyActor<T>>::operator new;
	using FastAllocated<ReadyActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadyActor<T>, 0, T >;
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ReadyActor(Future<T> const& f) 
															#line 12550 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   ReadyActorState<T, ReadyActor<T>>(f)
	{
		fdb_probe_actor_enter("ready", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("ready");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("ready", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadyActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> ready( Future<T> const& f ) {
															#line 1095 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new ReadyActor<T>(f));
															#line 12580 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1104 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 12585 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via waitAndForward()
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class WaitAndForwardActor>
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitAndForwardActorState {
															#line 12592 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitAndForwardActorState(FutureStream<T> const& input) 
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input)
															#line 12599 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("waitAndForward", reinterpret_cast<unsigned long>(this));

	}
	~WaitAndForwardActorState() 
	{
		fdb_probe_actor_destroy("waitAndForward", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1107 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<T> __when_expr_0 = input;
															#line 1107 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<WaitAndForwardActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12616 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<WaitAndForwardActor*>(this)->actor_wait_state = 1;
															#line 1107 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< WaitAndForwardActor, 0, T >*>(static_cast<WaitAndForwardActor*>(this)));
															#line 12621 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~WaitAndForwardActorState();
		static_cast<WaitAndForwardActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& output,int loopDepth) 
	{
															#line 1108 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<WaitAndForwardActor*>(this)->SAV<T>::futures) { (void)(output); this->~WaitAndForwardActorState(); static_cast<WaitAndForwardActor*>(this)->destroy(); return 0; }
															#line 12644 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<WaitAndForwardActor*>(this)->SAV< T >::value()) T(output);
		this->~WaitAndForwardActorState();
		static_cast<WaitAndForwardActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && output,int loopDepth) 
	{
															#line 1108 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<WaitAndForwardActor*>(this)->SAV<T>::futures) { (void)(output); this->~WaitAndForwardActorState(); static_cast<WaitAndForwardActor*>(this)->destroy(); return 0; }
															#line 12656 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<WaitAndForwardActor*>(this)->SAV< T >::value()) T(output);
		this->~WaitAndForwardActorState();
		static_cast<WaitAndForwardActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& output,int loopDepth) 
	{
		loopDepth = a_body1cont1(output, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && output,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(output), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitAndForwardActor*>(this)->actor_wait_state > 0) static_cast<WaitAndForwardActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndForwardActor*>(this)->ActorSingleCallback< WaitAndForwardActor, 0, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< WaitAndForwardActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("waitAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< WaitAndForwardActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("waitAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< WaitAndForwardActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 12729 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via waitAndForward()
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitAndForwardActor final : public Actor<T>, public ActorSingleCallback< WaitAndForwardActor<T>, 0, T >, public FastAllocated<WaitAndForwardActor<T>>, public WaitAndForwardActorState<T, WaitAndForwardActor<T>> {
															#line 12736 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<WaitAndForwardActor<T>>::operator new;
	using FastAllocated<WaitAndForwardActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< WaitAndForwardActor<T>, 0, T >;
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitAndForwardActor(FutureStream<T> const& input) 
															#line 12747 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   WaitAndForwardActorState<T, WaitAndForwardActor<T>>(input)
	{
		fdb_probe_actor_enter("waitAndForward", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitAndForward");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitAndForward", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< WaitAndForwardActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> waitAndForward( FutureStream<T> const& input ) {
															#line 1105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new WaitAndForwardActor<T>(input));
															#line 12777 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1110 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 12782 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via reportErrorsExcept()
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ReportErrorsExceptActor>
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ReportErrorsExceptActorState {
															#line 12789 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ReportErrorsExceptActorState(Future<T> const& in,const char* const& context,UID const& id,std::set<int> const* const& pExceptErrors) 
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : in(in),
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   context(context),
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   id(id),
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   pExceptErrors(pExceptErrors)
															#line 12802 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("reportErrorsExcept", reinterpret_cast<unsigned long>(this));

	}
	~ReportErrorsExceptActorState() 
	{
		fdb_probe_actor_destroy("reportErrorsExcept", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1114 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = in;
															#line 1114 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<ReportErrorsExceptActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 12820 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReportErrorsExceptActor*>(this)->actor_wait_state = 1;
															#line 1114 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReportErrorsExceptActor, 0, T >*>(static_cast<ReportErrorsExceptActor*>(this)));
															#line 12825 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ReportErrorsExceptActorState();
		static_cast<ReportErrorsExceptActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1117 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() != error_code_actor_cancelled && (!pExceptErrors || !pExceptErrors->count(e.code())))
															#line 12855 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1118 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				TraceEvent(SevError, context, id).error(e);
															#line 12859 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1119 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 12863 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 1115 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ReportErrorsExceptActor*>(this)->SAV<T>::futures) { (void)(t); this->~ReportErrorsExceptActorState(); static_cast<ReportErrorsExceptActor*>(this)->destroy(); return 0; }
															#line 12877 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ReportErrorsExceptActor*>(this)->SAV< T >::value()) T(t);
		this->~ReportErrorsExceptActorState();
		static_cast<ReportErrorsExceptActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 1115 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ReportErrorsExceptActor*>(this)->SAV<T>::futures) { (void)(t); this->~ReportErrorsExceptActorState(); static_cast<ReportErrorsExceptActor*>(this)->destroy(); return 0; }
															#line 12889 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ReportErrorsExceptActor*>(this)->SAV< T >::value()) T(t);
		this->~ReportErrorsExceptActorState();
		static_cast<ReportErrorsExceptActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ReportErrorsExceptActor*>(this)->actor_wait_state > 0) static_cast<ReportErrorsExceptActor*>(this)->actor_wait_state = 0;
		static_cast<ReportErrorsExceptActor*>(this)->ActorCallback< ReportErrorsExceptActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ReportErrorsExceptActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("reportErrorsExcept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reportErrorsExcept", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReportErrorsExceptActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("reportErrorsExcept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reportErrorsExcept", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReportErrorsExceptActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("reportErrorsExcept", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("reportErrorsExcept", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> in;
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	const char* context;
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UID id;
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::set<int> const* pExceptErrors;
															#line 12968 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via reportErrorsExcept()
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ReportErrorsExceptActor final : public Actor<T>, public ActorCallback< ReportErrorsExceptActor<T>, 0, T >, public FastAllocated<ReportErrorsExceptActor<T>>, public ReportErrorsExceptActorState<T, ReportErrorsExceptActor<T>> {
															#line 12975 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ReportErrorsExceptActor<T>>::operator new;
	using FastAllocated<ReportErrorsExceptActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReportErrorsExceptActor<T>, 0, T >;
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ReportErrorsExceptActor(Future<T> const& in,const char* const& context,UID const& id,std::set<int> const* const& pExceptErrors) 
															#line 12986 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   ReportErrorsExceptActorState<T, ReportErrorsExceptActor<T>>(in, context, id, pExceptErrors)
	{
		fdb_probe_actor_enter("reportErrorsExcept", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("reportErrorsExcept");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("reportErrorsExcept", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReportErrorsExceptActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> reportErrorsExcept( Future<T> const& in, const char* const& context, UID const& id, std::set<int> const* const& pExceptErrors ) {
															#line 1111 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new ReportErrorsExceptActor<T>(in, context, id, pExceptErrors));
															#line 13016 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1122 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class T>
Future<T> reportErrors(Future<T> const& in, const char* context, UID id = UID()) {
	return reportErrorsExcept(in, context, id, nullptr);
}

															#line 13026 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via require()
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class RequireActor>
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RequireActorState {
															#line 13033 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RequireActorState(Future<Optional<T>> const& in,int const& errorCode) 
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : in(in),
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   errorCode(errorCode)
															#line 13042 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("require", reinterpret_cast<unsigned long>(this));

	}
	~RequireActorState() 
	{
		fdb_probe_actor_destroy("require", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1130 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Optional<T>> __when_expr_0 = in;
															#line 1130 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<RequireActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13059 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RequireActor*>(this)->actor_wait_state = 1;
															#line 1130 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RequireActor, 0, Optional<T> >*>(static_cast<RequireActor*>(this)));
															#line 13064 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~RequireActorState();
		static_cast<RequireActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<T> const& o,int loopDepth) 
	{
															#line 1131 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (o.present())
															#line 13087 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 1132 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!static_cast<RequireActor*>(this)->SAV<T>::futures) { (void)(o.get()); this->~RequireActorState(); static_cast<RequireActor*>(this)->destroy(); return 0; }
															#line 13091 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			new (&static_cast<RequireActor*>(this)->SAV< T >::value()) T(o.get());
			this->~RequireActorState();
			static_cast<RequireActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 1134 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(Error(errorCode), loopDepth);
															#line 13101 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}

		return loopDepth;
	}
	int a_body1cont1(Optional<T> && o,int loopDepth) 
	{
															#line 1131 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (o.present())
															#line 13110 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 1132 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!static_cast<RequireActor*>(this)->SAV<T>::futures) { (void)(o.get()); this->~RequireActorState(); static_cast<RequireActor*>(this)->destroy(); return 0; }
															#line 13114 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			new (&static_cast<RequireActor*>(this)->SAV< T >::value()) T(o.get());
			this->~RequireActorState();
			static_cast<RequireActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 1134 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(Error(errorCode), loopDepth);
															#line 13124 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}

		return loopDepth;
	}
	int a_body1when1(Optional<T> const& o,int loopDepth) 
	{
		loopDepth = a_body1cont1(o, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<T> && o,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(o), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RequireActor*>(this)->actor_wait_state > 0) static_cast<RequireActor*>(this)->actor_wait_state = 0;
		static_cast<RequireActor*>(this)->ActorCallback< RequireActor, 0, Optional<T> >::remove();

	}
	void a_callback_fire(ActorCallback< RequireActor, 0, Optional<T> >*,Optional<T> const& value) 
	{
		fdb_probe_actor_enter("require", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("require", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RequireActor, 0, Optional<T> >*,Optional<T> && value) 
	{
		fdb_probe_actor_enter("require", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("require", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RequireActor, 0, Optional<T> >*,Error err) 
	{
		fdb_probe_actor_enter("require", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("require", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Optional<T>> in;
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int errorCode;
															#line 13196 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via require()
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RequireActor final : public Actor<T>, public ActorCallback< RequireActor<T>, 0, Optional<T> >, public FastAllocated<RequireActor<T>>, public RequireActorState<T, RequireActor<T>> {
															#line 13203 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<RequireActor<T>>::operator new;
	using FastAllocated<RequireActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RequireActor<T>, 0, Optional<T> >;
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RequireActor(Future<Optional<T>> const& in,int const& errorCode) 
															#line 13214 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   RequireActorState<T, RequireActor<T>>(in, errorCode)
	{
		fdb_probe_actor_enter("require", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("require");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("require", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RequireActor<T>, 0, Optional<T> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> require( Future<Optional<T>> const& in, int const& errorCode ) {
															#line 1128 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new RequireActor<T>(in, errorCode));
															#line 13244 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1137 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 13249 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via waitForFirst()
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class WaitForFirstActor>
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitForFirstActorState {
															#line 13256 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitForFirstActorState(std::vector<Future<T>> const& items) 
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : items(items),
															#line 1140 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   resultStream(),
															#line 1141 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   errorStream(),
															#line 1143 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   forCancellation(makeStream(items, resultStream, errorStream)),
															#line 1145 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   resultFutureStream(resultStream.getFuture()),
															#line 1146 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   errorFutureStream(errorStream.getFuture())
															#line 13273 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("waitForFirst", reinterpret_cast<unsigned long>(this));

	}
	~WaitForFirstActorState() 
	{
		fdb_probe_actor_destroy("waitForFirst", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1149 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<T> __when_expr_0 = resultFutureStream;
															#line 1148 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<WaitForFirstActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13290 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
															#line 1153 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			FutureStream<Error> __when_expr_1 = errorFutureStream;
															#line 13294 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.pop(), loopDepth); };
			static_cast<WaitForFirstActor*>(this)->actor_wait_state = 1;
															#line 1149 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< WaitForFirstActor, 0, T >*>(static_cast<WaitForFirstActor*>(this)));
															#line 1153 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< WaitForFirstActor, 1, Error >*>(static_cast<WaitForFirstActor*>(this)));
															#line 13301 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~WaitForFirstActorState();
		static_cast<WaitForFirstActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
															#line 1150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		forCancellation = Future<Void>();
															#line 1151 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<WaitForFirstActor*>(this)->SAV<T>::futures) { (void)(val); this->~WaitForFirstActorState(); static_cast<WaitForFirstActor*>(this)->destroy(); return 0; }
															#line 13326 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<WaitForFirstActor*>(this)->SAV< T >::value()) T(val);
		this->~WaitForFirstActorState();
		static_cast<WaitForFirstActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
															#line 1150 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		forCancellation = Future<Void>();
															#line 1151 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<WaitForFirstActor*>(this)->SAV<T>::futures) { (void)(val); this->~WaitForFirstActorState(); static_cast<WaitForFirstActor*>(this)->destroy(); return 0; }
															#line 13340 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<WaitForFirstActor*>(this)->SAV< T >::value()) T(val);
		this->~WaitForFirstActorState();
		static_cast<WaitForFirstActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Error const& e,int loopDepth) 
	{
															#line 1154 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		forCancellation = Future<Void>();
															#line 1155 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 13354 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1when2(Error && e,int loopDepth) 
	{
															#line 1154 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		forCancellation = Future<Void>();
															#line 1155 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 13364 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitForFirstActor*>(this)->actor_wait_state > 0) static_cast<WaitForFirstActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForFirstActor*>(this)->ActorSingleCallback< WaitForFirstActor, 0, T >::remove();
		static_cast<WaitForFirstActor*>(this)->ActorSingleCallback< WaitForFirstActor, 1, Error >::remove();

	}
	void a_callback_fire(ActorSingleCallback< WaitForFirstActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< WaitForFirstActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< WaitForFirstActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< WaitForFirstActor, 1, Error >*,Error const& value) 
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< WaitForFirstActor, 1, Error >*,Error && value) 
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorSingleCallback< WaitForFirstActor, 1, Error >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::vector<Future<T>> items;
															#line 1140 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> resultStream;
															#line 1141 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<Error> errorStream;
															#line 1143 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> forCancellation;
															#line 1145 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> resultFutureStream;
															#line 1146 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<Error> errorFutureStream;
															#line 13477 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via waitForFirst()
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitForFirstActor final : public Actor<T>, public ActorSingleCallback< WaitForFirstActor<T>, 0, T >, public ActorSingleCallback< WaitForFirstActor<T>, 1, Error >, public FastAllocated<WaitForFirstActor<T>>, public WaitForFirstActorState<T, WaitForFirstActor<T>> {
															#line 13484 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<WaitForFirstActor<T>>::operator new;
	using FastAllocated<WaitForFirstActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< WaitForFirstActor<T>, 0, T >;
friend struct ActorSingleCallback< WaitForFirstActor<T>, 1, Error >;
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitForFirstActor(std::vector<Future<T>> const& items) 
															#line 13496 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   WaitForFirstActorState<T, WaitForFirstActor<T>>(items)
	{
		fdb_probe_actor_enter("waitForFirst", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForFirst");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForFirst", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< WaitForFirstActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> waitForFirst( std::vector<Future<T>> const& items ) {
															#line 1138 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new WaitForFirstActor<T>(items));
															#line 13526 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1159 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 13531 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tag()
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TagActor>
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagActorState {
															#line 13538 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagActorState(Future<Void> const& future,T const& what) 
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : future(future),
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   what(what)
															#line 13547 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tag", reinterpret_cast<unsigned long>(this));

	}
	~TagActorState() 
	{
		fdb_probe_actor_destroy("tag", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1162 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = future;
															#line 1162 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TagActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13564 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TagActor*>(this)->actor_wait_state = 1;
															#line 1162 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagActor, 0, Void >*>(static_cast<TagActor*>(this)));
															#line 13569 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TagActorState();
		static_cast<TagActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1163 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TagActor*>(this)->SAV<T>::futures) { (void)(what); this->~TagActorState(); static_cast<TagActor*>(this)->destroy(); return 0; }
															#line 13592 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TagActor*>(this)->SAV< T >::value()) T(std::move(what)); // state_var_RVO
		this->~TagActorState();
		static_cast<TagActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1163 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TagActor*>(this)->SAV<T>::futures) { (void)(what); this->~TagActorState(); static_cast<TagActor*>(this)->destroy(); return 0; }
															#line 13604 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TagActor*>(this)->SAV< T >::value()) T(std::move(what)); // state_var_RVO
		this->~TagActorState();
		static_cast<TagActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TagActor*>(this)->actor_wait_state > 0) static_cast<TagActor*>(this)->actor_wait_state = 0;
		static_cast<TagActor*>(this)->ActorCallback< TagActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> future;
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T what;
															#line 13679 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tag()
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagActor final : public Actor<T>, public ActorCallback< TagActor<T>, 0, Void >, public FastAllocated<TagActor<T>>, public TagActorState<T, TagActor<T>> {
															#line 13686 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagActor<T>>::operator new;
	using FastAllocated<TagActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TagActor<T>, 0, Void >;
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagActor(Future<Void> const& future,T const& what) 
															#line 13697 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TagActorState<T, TagActor<T>>(future, what)
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tag");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TagActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> tag( Future<Void> const& future, T const& what ) {
															#line 1160 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TagActor<T>(future, what));
															#line 13727 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1165 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 13732 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tag()
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TagActor1>
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagActor1State {
															#line 13739 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagActor1State(Future<Void> const& future,T const& what,PromiseStream<T> const& stream) 
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : future(future),
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   what(what),
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   stream(stream)
															#line 13750 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tag", reinterpret_cast<unsigned long>(this));

	}
	~TagActor1State() 
	{
		fdb_probe_actor_destroy("tag", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1168 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = future;
															#line 1168 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TagActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13767 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TagActor1*>(this)->actor_wait_state = 1;
															#line 1168 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagActor1, 0, Void >*>(static_cast<TagActor1*>(this)));
															#line 13772 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TagActor1State();
		static_cast<TagActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1169 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		stream.send(what);
															#line 1170 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TagActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TagActor1State(); static_cast<TagActor1*>(this)->destroy(); return 0; }
															#line 13797 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TagActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~TagActor1State();
		static_cast<TagActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1169 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		stream.send(what);
															#line 1170 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TagActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TagActor1State(); static_cast<TagActor1*>(this)->destroy(); return 0; }
															#line 13811 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TagActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~TagActor1State();
		static_cast<TagActor1*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TagActor1*>(this)->actor_wait_state > 0) static_cast<TagActor1*>(this)->actor_wait_state = 0;
		static_cast<TagActor1*>(this)->ActorCallback< TagActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> future;
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T what;
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> stream;
															#line 13888 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tag()
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagActor1 final : public Actor<Void>, public ActorCallback< TagActor1<T>, 0, Void >, public FastAllocated<TagActor1<T>>, public TagActor1State<T, TagActor1<T>> {
															#line 13895 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagActor1<T>>::operator new;
	using FastAllocated<TagActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TagActor1<T>, 0, Void >;
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagActor1(Future<Void> const& future,T const& what,PromiseStream<T> const& stream) 
															#line 13906 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TagActor1State<T, TagActor1<T>>(future, what, stream)
	{
		fdb_probe_actor_enter("tag", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tag");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tag", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TagActor1<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> tag( Future<Void> const& future, T const& what, PromiseStream<T> const& stream ) {
															#line 1166 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new TagActor1<T>(future, what, stream));
															#line 13936 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1172 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 13941 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tagError()
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TagErrorActor>
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagErrorActorState {
															#line 13948 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagErrorActorState(Future<Void> const& future,Error const& e) 
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : future(future),
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   e(e)
															#line 13957 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tagError", reinterpret_cast<unsigned long>(this));

	}
	~TagErrorActorState() 
	{
		fdb_probe_actor_destroy("tagError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1175 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = future;
															#line 1175 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TagErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13974 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TagErrorActor*>(this)->actor_wait_state = 1;
															#line 1175 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagErrorActor, 0, Void >*>(static_cast<TagErrorActor*>(this)));
															#line 13979 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TagErrorActorState();
		static_cast<TagErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1176 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 14002 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1176 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 14010 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

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
		if (static_cast<TagErrorActor*>(this)->actor_wait_state > 0) static_cast<TagErrorActor*>(this)->actor_wait_state = 0;
		static_cast<TagErrorActor*>(this)->ActorCallback< TagErrorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagErrorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tagError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagErrorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tagError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagErrorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tagError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagError", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> future;
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error e;
															#line 14081 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tagError()
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagErrorActor final : public Actor<T>, public ActorCallback< TagErrorActor<T>, 0, Void >, public FastAllocated<TagErrorActor<T>>, public TagErrorActorState<T, TagErrorActor<T>> {
															#line 14088 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagErrorActor<T>>::operator new;
	using FastAllocated<TagErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TagErrorActor<T>, 0, Void >;
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagErrorActor(Future<Void> const& future,Error const& e) 
															#line 14099 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   TagErrorActorState<T, TagErrorActor<T>>(future, e)
	{
		fdb_probe_actor_enter("tagError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tagError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tagError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TagErrorActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> tagError( Future<Void> const& future, Error const& e ) {
															#line 1173 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new TagErrorActor<T>(future, e));
															#line 14129 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1178 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 14134 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via detach()
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class DetachActor>
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DetachActorState {
															#line 14141 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DetachActorState(Future<T> const& f) 
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f)
															#line 14148 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("detach", reinterpret_cast<unsigned long>(this));

	}
	~DetachActorState() 
	{
		fdb_probe_actor_destroy("detach", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1181 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = f;
															#line 1181 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<DetachActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 14165 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DetachActor*>(this)->actor_wait_state = 1;
															#line 1181 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DetachActor, 0, T >*>(static_cast<DetachActor*>(this)));
															#line 14170 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DetachActorState();
		static_cast<DetachActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& x,int loopDepth) 
	{
															#line 1182 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DetachActor*>(this)->SAV<T>::futures) { (void)(x); this->~DetachActorState(); static_cast<DetachActor*>(this)->destroy(); return 0; }
															#line 14193 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DetachActor*>(this)->SAV< T >::value()) T(x);
		this->~DetachActorState();
		static_cast<DetachActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && x,int loopDepth) 
	{
															#line 1182 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DetachActor*>(this)->SAV<T>::futures) { (void)(x); this->~DetachActorState(); static_cast<DetachActor*>(this)->destroy(); return 0; }
															#line 14205 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DetachActor*>(this)->SAV< T >::value()) T(x);
		this->~DetachActorState();
		static_cast<DetachActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& x,int loopDepth) 
	{
		loopDepth = a_body1cont1(x, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && x,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(x), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DetachActor*>(this)->actor_wait_state > 0) static_cast<DetachActor*>(this)->actor_wait_state = 0;
		static_cast<DetachActor*>(this)->ActorCallback< DetachActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< DetachActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("detach", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("detach", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DetachActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("detach", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("detach", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DetachActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("detach", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("detach", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 14278 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via detach()
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DetachActor final : public Actor<T>, public ActorCallback< DetachActor<T>, 0, T >, public FastAllocated<DetachActor<T>>, public DetachActorState<T, DetachActor<T>> {
															#line 14285 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DetachActor<T>>::operator new;
	using FastAllocated<DetachActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DetachActor<T>, 0, T >;
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DetachActor(Future<T> const& f) 
															#line 14296 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   DetachActorState<T, DetachActor<T>>(f)
	{
		fdb_probe_actor_enter("detach", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("detach");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("detach", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DetachActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> detach( Future<T> const& f ) {
															#line 1179 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new DetachActor<T>(f));
															#line 14326 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1184 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// If the future is ready, yields and returns. Otherwise, returns when future is set.
template <class T>
Future<T> orYield(Future<T> f) {
	if (f.isReady()) {
		if (f.isError())
			return tagError<T>(yield(), f.getError());
		else
			return tag(yield(), f.get());
	} else
		return f;
}

Future<Void> orYield(Future<Void> f);

															#line 14345 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via chooseActor()
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ChooseActorActor>
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ChooseActorActorState {
															#line 14352 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ChooseActorActorState(Future<T> const& lhs,Future<T> const& rhs) 
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lhs(lhs),
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   rhs(rhs)
															#line 14361 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("chooseActor", reinterpret_cast<unsigned long>(this));

	}
	~ChooseActorActorState() 
	{
		fdb_probe_actor_destroy("chooseActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1202 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = lhs;
															#line 1201 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<ChooseActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 14378 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1205 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_1 = rhs;
															#line 14382 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ChooseActorActor*>(this)->actor_wait_state = 1;
															#line 1202 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChooseActorActor, 0, T >*>(static_cast<ChooseActorActor*>(this)));
															#line 1205 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChooseActorActor, 1, T >*>(static_cast<ChooseActorActor*>(this)));
															#line 14389 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ChooseActorActorState();
		static_cast<ChooseActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& t,int loopDepth) 
	{
															#line 1203 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ChooseActorActor*>(this)->SAV<T>::futures) { (void)(t); this->~ChooseActorActorState(); static_cast<ChooseActorActor*>(this)->destroy(); return 0; }
															#line 14412 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ChooseActorActor*>(this)->SAV< T >::value()) T(t);
		this->~ChooseActorActorState();
		static_cast<ChooseActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T && t,int loopDepth) 
	{
															#line 1203 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ChooseActorActor*>(this)->SAV<T>::futures) { (void)(t); this->~ChooseActorActorState(); static_cast<ChooseActorActor*>(this)->destroy(); return 0; }
															#line 14424 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ChooseActorActor*>(this)->SAV< T >::value()) T(t);
		this->~ChooseActorActorState();
		static_cast<ChooseActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(T const& t,int loopDepth) 
	{
															#line 1206 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ChooseActorActor*>(this)->SAV<T>::futures) { (void)(t); this->~ChooseActorActorState(); static_cast<ChooseActorActor*>(this)->destroy(); return 0; }
															#line 14436 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ChooseActorActor*>(this)->SAV< T >::value()) T(t);
		this->~ChooseActorActorState();
		static_cast<ChooseActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(T && t,int loopDepth) 
	{
															#line 1206 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ChooseActorActor*>(this)->SAV<T>::futures) { (void)(t); this->~ChooseActorActorState(); static_cast<ChooseActorActor*>(this)->destroy(); return 0; }
															#line 14448 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ChooseActorActor*>(this)->SAV< T >::value()) T(t);
		this->~ChooseActorActorState();
		static_cast<ChooseActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChooseActorActor*>(this)->actor_wait_state > 0) static_cast<ChooseActorActor*>(this)->actor_wait_state = 0;
		static_cast<ChooseActorActor*>(this)->ActorCallback< ChooseActorActor, 0, T >::remove();
		static_cast<ChooseActorActor*>(this)->ActorCallback< ChooseActorActor, 1, T >::remove();

	}
	void a_callback_fire(ActorCallback< ChooseActorActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChooseActorActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChooseActorActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChooseActorActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChooseActorActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChooseActorActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> lhs;
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> rhs;
															#line 14557 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via chooseActor()
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ChooseActorActor final : public Actor<T>, public ActorCallback< ChooseActorActor<T>, 0, T >, public ActorCallback< ChooseActorActor<T>, 1, T >, public FastAllocated<ChooseActorActor<T>>, public ChooseActorActorState<T, ChooseActorActor<T>> {
															#line 14564 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ChooseActorActor<T>>::operator new;
	using FastAllocated<ChooseActorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChooseActorActor<T>, 0, T >;
friend struct ActorCallback< ChooseActorActor<T>, 1, T >;
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ChooseActorActor(Future<T> const& lhs,Future<T> const& rhs) 
															#line 14576 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   ChooseActorActorState<T, ChooseActorActor<T>>(lhs, rhs)
	{
		fdb_probe_actor_enter("chooseActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("chooseActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("chooseActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChooseActorActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> chooseActor( Future<T> const& lhs, Future<T> const& rhs ) {
															#line 1199 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new ChooseActorActor<T>(lhs, rhs));
															#line 14606 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1210 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// set && set -> set
// error && x -> error
// all others -> unset
inline Future<Void> operator&&(Future<Void> const& lhs, Future<Void> const& rhs) {
	if (lhs.isReady()) {
		if (lhs.isError())
			return lhs;
		else
			return rhs;
	}
	if (rhs.isReady()) {
		if (rhs.isError())
			return rhs;
		else
			return lhs;
	}

	Future<Void> x[] = { lhs, rhs };
	return quorum(x, 2, 2);
}

// error || unset -> error
// unset || unset -> unset
// all others -> set
inline Future<Void> operator||(Future<Void> const& lhs, Future<Void> const& rhs) {
	if (lhs.isReady()) {
		if (lhs.isError())
			return lhs;
		if (rhs.isReady())
			return rhs;
		return lhs;
	}

	return chooseActor(lhs, rhs);
}

															#line 14647 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via brokenPromiseToNever()
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class BrokenPromiseToNeverActor>
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class BrokenPromiseToNeverActorState {
															#line 14654 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	BrokenPromiseToNeverActorState(Future<T> const& in) 
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : in(in)
															#line 14661 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("brokenPromiseToNever", reinterpret_cast<unsigned long>(this));

	}
	~BrokenPromiseToNeverActorState() 
	{
		fdb_probe_actor_destroy("brokenPromiseToNever", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1250 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = in;
															#line 1250 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 14679 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state = 1;
															#line 1250 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BrokenPromiseToNeverActor, 0, T >*>(static_cast<BrokenPromiseToNeverActor*>(this)));
															#line 14684 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~BrokenPromiseToNeverActorState();
		static_cast<BrokenPromiseToNeverActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1253 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() != error_code_broken_promise)
															#line 14714 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1254 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, loopDepth);
															#line 14718 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = Never();
															#line 1255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 14724 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_1.get(), loopDepth); };
			static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state = 2;
															#line 1255 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< BrokenPromiseToNeverActor, 1, Void >*>(static_cast<BrokenPromiseToNeverActor*>(this)));
															#line 14729 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
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
															#line 1251 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<BrokenPromiseToNeverActor*>(this)->SAV<T>::futures) { (void)(t); this->~BrokenPromiseToNeverActorState(); static_cast<BrokenPromiseToNeverActor*>(this)->destroy(); return 0; }
															#line 14744 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<BrokenPromiseToNeverActor*>(this)->SAV< T >::value()) T(t);
		this->~BrokenPromiseToNeverActorState();
		static_cast<BrokenPromiseToNeverActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 1251 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<BrokenPromiseToNeverActor*>(this)->SAV<T>::futures) { (void)(t); this->~BrokenPromiseToNeverActorState(); static_cast<BrokenPromiseToNeverActor*>(this)->destroy(); return 0; }
															#line 14756 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<BrokenPromiseToNeverActor*>(this)->SAV< T >::value()) T(t);
		this->~BrokenPromiseToNeverActorState();
		static_cast<BrokenPromiseToNeverActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state > 0) static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state = 0;
		static_cast<BrokenPromiseToNeverActor*>(this)->ActorCallback< BrokenPromiseToNeverActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< BrokenPromiseToNeverActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< BrokenPromiseToNeverActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< BrokenPromiseToNeverActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1Catch2cont1(Void const& _,int loopDepth) 
	{
															#line 1256 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 14831 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1Catch2cont1(Void && _,int loopDepth) 
	{
															#line 1256 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 14839 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state > 0) static_cast<BrokenPromiseToNeverActor*>(this)->actor_wait_state = 0;
		static_cast<BrokenPromiseToNeverActor*>(this)->ActorCallback< BrokenPromiseToNeverActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< BrokenPromiseToNeverActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< BrokenPromiseToNeverActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< BrokenPromiseToNeverActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> in;
															#line 14908 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via brokenPromiseToNever()
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class BrokenPromiseToNeverActor final : public Actor<T>, public ActorCallback< BrokenPromiseToNeverActor<T>, 0, T >, public ActorCallback< BrokenPromiseToNeverActor<T>, 1, Void >, public FastAllocated<BrokenPromiseToNeverActor<T>>, public BrokenPromiseToNeverActorState<T, BrokenPromiseToNeverActor<T>> {
															#line 14915 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<BrokenPromiseToNeverActor<T>>::operator new;
	using FastAllocated<BrokenPromiseToNeverActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BrokenPromiseToNeverActor<T>, 0, T >;
friend struct ActorCallback< BrokenPromiseToNeverActor<T>, 1, Void >;
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	BrokenPromiseToNeverActor(Future<T> const& in) 
															#line 14927 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   BrokenPromiseToNeverActorState<T, BrokenPromiseToNeverActor<T>>(in)
	{
		fdb_probe_actor_enter("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("brokenPromiseToNever");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("brokenPromiseToNever", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< BrokenPromiseToNeverActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< BrokenPromiseToNeverActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> brokenPromiseToNever( Future<T> const& in ) {
															#line 1247 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new BrokenPromiseToNeverActor<T>(in));
															#line 14958 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1259 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 14963 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via brokenPromiseToMaybeDelivered()
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class BrokenPromiseToMaybeDeliveredActor>
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class BrokenPromiseToMaybeDeliveredActorState {
															#line 14970 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	BrokenPromiseToMaybeDeliveredActorState(Future<T> const& in) 
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : in(in)
															#line 14977 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this));

	}
	~BrokenPromiseToMaybeDeliveredActorState() 
	{
		fdb_probe_actor_destroy("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1263 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = in;
															#line 1263 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 14995 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->actor_wait_state = 1;
															#line 1263 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BrokenPromiseToMaybeDeliveredActor, 0, T >*>(static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)));
															#line 15000 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~BrokenPromiseToMaybeDeliveredActorState();
		static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1266 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 15030 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1267 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(request_maybe_delivered(), loopDepth);
															#line 15034 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1269 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 15038 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 1264 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->SAV<T>::futures) { (void)(t); this->~BrokenPromiseToMaybeDeliveredActorState(); static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->destroy(); return 0; }
															#line 15052 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->SAV< T >::value()) T(t);
		this->~BrokenPromiseToMaybeDeliveredActorState();
		static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 1264 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->SAV<T>::futures) { (void)(t); this->~BrokenPromiseToMaybeDeliveredActorState(); static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->destroy(); return 0; }
															#line 15064 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->SAV< T >::value()) T(t);
		this->~BrokenPromiseToMaybeDeliveredActorState();
		static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->actor_wait_state > 0) static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->actor_wait_state = 0;
		static_cast<BrokenPromiseToMaybeDeliveredActor*>(this)->ActorCallback< BrokenPromiseToMaybeDeliveredActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< BrokenPromiseToMaybeDeliveredActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< BrokenPromiseToMaybeDeliveredActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< BrokenPromiseToMaybeDeliveredActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> in;
															#line 15137 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via brokenPromiseToMaybeDelivered()
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class BrokenPromiseToMaybeDeliveredActor final : public Actor<T>, public ActorCallback< BrokenPromiseToMaybeDeliveredActor<T>, 0, T >, public FastAllocated<BrokenPromiseToMaybeDeliveredActor<T>>, public BrokenPromiseToMaybeDeliveredActorState<T, BrokenPromiseToMaybeDeliveredActor<T>> {
															#line 15144 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<BrokenPromiseToMaybeDeliveredActor<T>>::operator new;
	using FastAllocated<BrokenPromiseToMaybeDeliveredActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BrokenPromiseToMaybeDeliveredActor<T>, 0, T >;
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	BrokenPromiseToMaybeDeliveredActor(Future<T> const& in) 
															#line 15155 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   BrokenPromiseToMaybeDeliveredActorState<T, BrokenPromiseToMaybeDeliveredActor<T>>(in)
	{
		fdb_probe_actor_enter("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("brokenPromiseToMaybeDelivered");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("brokenPromiseToMaybeDelivered", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< BrokenPromiseToMaybeDeliveredActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> brokenPromiseToMaybeDelivered( Future<T> const& in ) {
															#line 1260 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new BrokenPromiseToMaybeDeliveredActor<T>(in));
															#line 15185 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1272 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 15190 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tagAndForward()
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class U, class TagAndForwardActor>
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardActorState {
															#line 15197 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardActorState(Promise<T>* const& pOutputPromise,U const& value,Future<Void> const& signal) 
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : pOutputPromise(pOutputPromise),
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   value(value),
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal),
															#line 1275 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   out(std::move(*pOutputPromise))
															#line 15210 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tagAndForward", reinterpret_cast<unsigned long>(this));

	}
	~TagAndForwardActorState() 
	{
		fdb_probe_actor_destroy("tagAndForward", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1276 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 1276 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 15227 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			static_cast<TagAndForwardActor*>(this)->actor_wait_state = 1;
															#line 1276 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagAndForwardActor, 0, Void >*>(static_cast<TagAndForwardActor*>(this)));
															#line 15231 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<TagAndForwardActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1277 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		out.send(std::move(value));
															#line 15253 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1277 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		out.send(std::move(value));
															#line 15262 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<TagAndForwardActor*>(this)->actor_wait_state > 0) static_cast<TagAndForwardActor*>(this)->actor_wait_state = 0;
		static_cast<TagAndForwardActor*>(this)->ActorCallback< TagAndForwardActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagAndForwardActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagAndForwardActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagAndForwardActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<TagAndForwardActor*>(this);
															#line 15334 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Promise<T>* pOutputPromise;
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	U value;
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 1275 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Promise<T> out;
															#line 15347 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tagAndForward()
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class U>
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardActor final : public Actor<void>, public ActorCallback< TagAndForwardActor<T, U>, 0, Void >, public FastAllocated<TagAndForwardActor<T, U>>, public TagAndForwardActorState<T, U, TagAndForwardActor<T, U>> {
															#line 15354 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagAndForwardActor<T, U>>::operator new;
	using FastAllocated<TagAndForwardActor<T, U>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< TagAndForwardActor<T, U>, 0, Void >;
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardActor(Promise<T>* const& pOutputPromise,U const& value,Future<Void> const& signal) 
															#line 15365 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   TagAndForwardActorState<T, U, TagAndForwardActor<T, U>>(pOutputPromise, value, signal)
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tagAndForward");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class U>
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void tagAndForward( Promise<T>* const& pOutputPromise, U const& value, Future<Void> const& signal ) {
															#line 1273 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new TagAndForwardActor<T, U>(pOutputPromise, value, signal);
															#line 15386 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1279 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 15391 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tagAndForward()
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TagAndForwardActor1>
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardActor1State {
															#line 15398 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardActor1State(PromiseStream<T>* const& pOutput,T const& value,Future<Void> const& signal) 
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : pOutput(pOutput),
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   value(value),
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal)
															#line 15409 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tagAndForward", reinterpret_cast<unsigned long>(this));

	}
	~TagAndForwardActor1State() 
	{
		fdb_probe_actor_destroy("tagAndForward", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1282 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 1282 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 15426 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			static_cast<TagAndForwardActor1*>(this)->actor_wait_state = 1;
															#line 1282 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagAndForwardActor1, 0, Void >*>(static_cast<TagAndForwardActor1*>(this)));
															#line 15430 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<TagAndForwardActor1*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1283 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		pOutput->send(std::move(value));
															#line 15452 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1283 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		pOutput->send(std::move(value));
															#line 15461 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<TagAndForwardActor1*>(this)->actor_wait_state > 0) static_cast<TagAndForwardActor1*>(this)->actor_wait_state = 0;
		static_cast<TagAndForwardActor1*>(this)->ActorCallback< TagAndForwardActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagAndForwardActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagAndForwardActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagAndForwardActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<TagAndForwardActor1*>(this);
															#line 15533 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T>* pOutput;
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	T value;
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 15544 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tagAndForward()
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardActor1 final : public Actor<void>, public ActorCallback< TagAndForwardActor1<T>, 0, Void >, public FastAllocated<TagAndForwardActor1<T>>, public TagAndForwardActor1State<T, TagAndForwardActor1<T>> {
															#line 15551 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagAndForwardActor1<T>>::operator new;
	using FastAllocated<TagAndForwardActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< TagAndForwardActor1<T>, 0, Void >;
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardActor1(PromiseStream<T>* const& pOutput,T const& value,Future<Void> const& signal) 
															#line 15562 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   TagAndForwardActor1State<T, TagAndForwardActor1<T>>(pOutput, value, signal)
	{
		fdb_probe_actor_enter("tagAndForward", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tagAndForward");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tagAndForward", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void tagAndForward( PromiseStream<T>* const& pOutput, T const& value, Future<Void> const& signal ) {
															#line 1280 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new TagAndForwardActor1<T>(pOutput, value, signal);
															#line 15583 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1285 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 15588 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tagAndForwardError()
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TagAndForwardErrorActor>
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardErrorActorState {
															#line 15595 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardErrorActorState(Promise<T>* const& pOutputPromise,Error const& value,Future<Void> const& signal) 
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : pOutputPromise(pOutputPromise),
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   value(value),
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal),
															#line 1288 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   out(std::move(*pOutputPromise))
															#line 15608 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tagAndForwardError", reinterpret_cast<unsigned long>(this));

	}
	~TagAndForwardErrorActorState() 
	{
		fdb_probe_actor_destroy("tagAndForwardError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1289 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 1289 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 15625 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			static_cast<TagAndForwardErrorActor*>(this)->actor_wait_state = 1;
															#line 1289 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagAndForwardErrorActor, 0, Void >*>(static_cast<TagAndForwardErrorActor*>(this)));
															#line 15629 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<TagAndForwardErrorActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1290 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		out.sendError(value);
															#line 15651 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1290 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		out.sendError(value);
															#line 15660 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<TagAndForwardErrorActor*>(this)->actor_wait_state > 0) static_cast<TagAndForwardErrorActor*>(this)->actor_wait_state = 0;
		static_cast<TagAndForwardErrorActor*>(this)->ActorCallback< TagAndForwardErrorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagAndForwardErrorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagAndForwardErrorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagAndForwardErrorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<TagAndForwardErrorActor*>(this);
															#line 15732 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Promise<T>* pOutputPromise;
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error value;
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 1288 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Promise<T> out;
															#line 15745 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tagAndForwardError()
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardErrorActor final : public Actor<void>, public ActorCallback< TagAndForwardErrorActor<T>, 0, Void >, public FastAllocated<TagAndForwardErrorActor<T>>, public TagAndForwardErrorActorState<T, TagAndForwardErrorActor<T>> {
															#line 15752 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagAndForwardErrorActor<T>>::operator new;
	using FastAllocated<TagAndForwardErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< TagAndForwardErrorActor<T>, 0, Void >;
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardErrorActor(Promise<T>* const& pOutputPromise,Error const& value,Future<Void> const& signal) 
															#line 15763 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   TagAndForwardErrorActorState<T, TagAndForwardErrorActor<T>>(pOutputPromise, value, signal)
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tagAndForwardError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void tagAndForwardError( Promise<T>* const& pOutputPromise, Error const& value, Future<Void> const& signal ) {
															#line 1286 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new TagAndForwardErrorActor<T>(pOutputPromise, value, signal);
															#line 15784 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1292 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 15789 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via tagAndForwardError()
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TagAndForwardErrorActor1>
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardErrorActor1State {
															#line 15796 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardErrorActor1State(PromiseStream<T>* const& pOutput,Error const& value,Future<Void> const& signal) 
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : pOutput(pOutput),
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   value(value),
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal)
															#line 15807 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("tagAndForwardError", reinterpret_cast<unsigned long>(this));

	}
	~TagAndForwardErrorActor1State() 
	{
		fdb_probe_actor_destroy("tagAndForwardError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 1295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 15824 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			static_cast<TagAndForwardErrorActor1*>(this)->actor_wait_state = 1;
															#line 1295 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TagAndForwardErrorActor1, 0, Void >*>(static_cast<TagAndForwardErrorActor1*>(this)));
															#line 15828 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		delete static_cast<TagAndForwardErrorActor1*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1296 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		pOutput->sendError(value);
															#line 15850 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1296 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		pOutput->sendError(value);
															#line 15859 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<TagAndForwardErrorActor1*>(this)->actor_wait_state > 0) static_cast<TagAndForwardErrorActor1*>(this)->actor_wait_state = 0;
		static_cast<TagAndForwardErrorActor1*>(this)->ActorCallback< TagAndForwardErrorActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TagAndForwardErrorActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TagAndForwardErrorActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TagAndForwardErrorActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		delete static_cast<TagAndForwardErrorActor1*>(this);
															#line 15931 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T>* pOutput;
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Error value;
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 15942 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via tagAndForwardError()
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TagAndForwardErrorActor1 final : public Actor<void>, public ActorCallback< TagAndForwardErrorActor1<T>, 0, Void >, public FastAllocated<TagAndForwardErrorActor1<T>>, public TagAndForwardErrorActor1State<T, TagAndForwardErrorActor1<T>> {
															#line 15949 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TagAndForwardErrorActor1<T>>::operator new;
	using FastAllocated<TagAndForwardErrorActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< TagAndForwardErrorActor1<T>, 0, Void >;
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TagAndForwardErrorActor1(PromiseStream<T>* const& pOutput,Error const& value,Future<Void> const& signal) 
															#line 15960 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<void>(),
		   TagAndForwardErrorActor1State<T, TagAndForwardErrorActor1<T>>(pOutput, value, signal)
	{
		fdb_probe_actor_enter("tagAndForwardError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tagAndForwardError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tagAndForwardError", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
void tagAndForwardError( PromiseStream<T>* const& pOutput, Error const& value, Future<Void> const& signal ) {
															#line 1293 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	new TagAndForwardErrorActor1<T>(pOutput, value, signal);
															#line 15981 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1298 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 15986 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via waitOrError()
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class WaitOrErrorActor>
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitOrErrorActorState {
															#line 15993 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitOrErrorActorState(Future<T> const& f,Future<Void> const& errorSignal) 
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f),
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   errorSignal(errorSignal)
															#line 16002 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("waitOrError", reinterpret_cast<unsigned long>(this));

	}
	~WaitOrErrorActorState() 
	{
		fdb_probe_actor_destroy("waitOrError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1302 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = f;
															#line 1301 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<WaitOrErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 16019 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1305 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = errorSignal;
															#line 16023 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<WaitOrErrorActor*>(this)->actor_wait_state = 1;
															#line 1302 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitOrErrorActor, 0, T >*>(static_cast<WaitOrErrorActor*>(this)));
															#line 1305 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitOrErrorActor, 1, Void >*>(static_cast<WaitOrErrorActor*>(this)));
															#line 16030 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~WaitOrErrorActorState();
		static_cast<WaitOrErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
															#line 1303 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<WaitOrErrorActor*>(this)->SAV<T>::futures) { (void)(val); this->~WaitOrErrorActorState(); static_cast<WaitOrErrorActor*>(this)->destroy(); return 0; }
															#line 16053 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<WaitOrErrorActor*>(this)->SAV< T >::value()) T(val);
		this->~WaitOrErrorActorState();
		static_cast<WaitOrErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
															#line 1303 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<WaitOrErrorActor*>(this)->SAV<T>::futures) { (void)(val); this->~WaitOrErrorActorState(); static_cast<WaitOrErrorActor*>(this)->destroy(); return 0; }
															#line 16065 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<WaitOrErrorActor*>(this)->SAV< T >::value()) T(val);
		this->~WaitOrErrorActorState();
		static_cast<WaitOrErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 1306 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ASSERT(false);
															#line 1307 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 16079 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 1306 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		ASSERT(false);
															#line 1307 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 16089 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitOrErrorActor*>(this)->actor_wait_state > 0) static_cast<WaitOrErrorActor*>(this)->actor_wait_state = 0;
		static_cast<WaitOrErrorActor*>(this)->ActorCallback< WaitOrErrorActor, 0, T >::remove();
		static_cast<WaitOrErrorActor*>(this)->ActorCallback< WaitOrErrorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitOrErrorActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitOrErrorActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitOrErrorActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitOrErrorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitOrErrorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitOrErrorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> errorSignal;
															#line 16194 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via waitOrError()
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class WaitOrErrorActor final : public Actor<T>, public ActorCallback< WaitOrErrorActor<T>, 0, T >, public ActorCallback< WaitOrErrorActor<T>, 1, Void >, public FastAllocated<WaitOrErrorActor<T>>, public WaitOrErrorActorState<T, WaitOrErrorActor<T>> {
															#line 16201 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<WaitOrErrorActor<T>>::operator new;
	using FastAllocated<WaitOrErrorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitOrErrorActor<T>, 0, T >;
friend struct ActorCallback< WaitOrErrorActor<T>, 1, Void >;
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	WaitOrErrorActor(Future<T> const& f,Future<Void> const& errorSignal) 
															#line 16213 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   WaitOrErrorActorState<T, WaitOrErrorActor<T>>(f, errorSignal)
	{
		fdb_probe_actor_enter("waitOrError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitOrError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitOrError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitOrErrorActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> waitOrError( Future<T> const& f, Future<Void> const& errorSignal ) {
															#line 1299 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new WaitOrErrorActor<T>(f, errorSignal));
															#line 16243 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1311 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// A low-overhead FIFO mutex made with no internal queue structure (no list, deque, vector, etc)
// The lock is implemented as a Promise<Void>, which is returned to callers in a convenient wrapper
// called Lock.
//
// Usage:
//   Lock lock = wait(mutex.take());
//   lock.release();  // Next waiter will get the lock, OR
//   lock.error(e);   // Next waiter will get e, future waiters will see broken_promise
//   lock = Lock();   // Or let Lock and any copies go out of scope.  All waiters will see broken_promise.
struct FlowMutex {
	FlowMutex() { lastPromise.send(Void()); }

	bool available() { return lastPromise.isSet(); }

	struct Lock {
		void release() { promise.send(Void()); }

		void error(Error e = broken_promise()) { promise.sendError(e); }

		// This is exposed in case the caller wants to use/copy it directly
		Promise<Void> promise;
	};

	Future<Lock> take() {
		Lock newLock;
		Future<Lock> f = lastPromise.isSet() ? newLock : tag(lastPromise.getFuture(), newLock);
		lastPromise = newLock.promise;
		return f;
	}

private:
	Promise<Void> lastPromise;
};

															#line 16282 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forwardErrors()
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class V, class ForwardErrorsActor>
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardErrorsActorState {
															#line 16289 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardErrorsActorState(Future<T> const& f,PromiseStream<V> const& output) 
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : f(f),
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output)
															#line 16298 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forwardErrors", reinterpret_cast<unsigned long>(this));

	}
	~ForwardErrorsActorState() 
	{
		fdb_probe_actor_destroy("forwardErrors", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1349 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = f;
															#line 1349 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<ForwardErrorsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 16316 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ForwardErrorsActor*>(this)->actor_wait_state = 1;
															#line 1349 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardErrorsActor, 0, T >*>(static_cast<ForwardErrorsActor*>(this)));
															#line 16321 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ForwardErrorsActorState();
		static_cast<ForwardErrorsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1352 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.sendError(e);
															#line 1353 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 16353 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& val,int loopDepth) 
	{
															#line 1350 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ForwardErrorsActor*>(this)->SAV<T>::futures) { (void)(val); this->~ForwardErrorsActorState(); static_cast<ForwardErrorsActor*>(this)->destroy(); return 0; }
															#line 16367 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ForwardErrorsActor*>(this)->SAV< T >::value()) T(val);
		this->~ForwardErrorsActorState();
		static_cast<ForwardErrorsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && val,int loopDepth) 
	{
															#line 1350 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ForwardErrorsActor*>(this)->SAV<T>::futures) { (void)(val); this->~ForwardErrorsActorState(); static_cast<ForwardErrorsActor*>(this)->destroy(); return 0; }
															#line 16379 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ForwardErrorsActor*>(this)->SAV< T >::value()) T(val);
		this->~ForwardErrorsActorState();
		static_cast<ForwardErrorsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont2(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ForwardErrorsActor*>(this)->actor_wait_state > 0) static_cast<ForwardErrorsActor*>(this)->actor_wait_state = 0;
		static_cast<ForwardErrorsActor*>(this)->ActorCallback< ForwardErrorsActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardErrorsActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forwardErrors", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardErrors", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardErrorsActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forwardErrors", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardErrors", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardErrorsActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forwardErrors", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forwardErrors", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<V> output;
															#line 16454 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via forwardErrors()
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class V>
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardErrorsActor final : public Actor<T>, public ActorCallback< ForwardErrorsActor<T, V>, 0, T >, public FastAllocated<ForwardErrorsActor<T, V>>, public ForwardErrorsActorState<T, V, ForwardErrorsActor<T, V>> {
															#line 16461 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardErrorsActor<T, V>>::operator new;
	using FastAllocated<ForwardErrorsActor<T, V>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardErrorsActor<T, V>, 0, T >;
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardErrorsActor(Future<T> const& f,PromiseStream<V> const& output) 
															#line 16472 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   ForwardErrorsActorState<T, V, ForwardErrorsActor<T, V>>(f, output)
	{
		fdb_probe_actor_enter("forwardErrors", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forwardErrors");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forwardErrors", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ForwardErrorsActor<T, V>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class V>
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> forwardErrors( Future<T> const& f, PromiseStream<V> const& output ) {
															#line 1346 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new ForwardErrorsActor<T, V>(f, output));
															#line 16502 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1356 "/usr/src/libfdb_c/flow/genericactors.actor.h"

struct FlowLock : NonCopyable, public ReferenceCounted<FlowLock> {
	// FlowLock implements a nonblocking critical section: there can be only a limited number of clients executing code
	// between wait(take()) and release(). Not thread safe. take() returns only when the number of holders of the lock
	// is fewer than the number of permits, and release() makes the caller no longer a holder of the lock. release()
	// only runs waiting take()rs after the caller wait()s

	struct Releaser : NonCopyable {
		FlowLock* lock;
		int remaining;
		Releaser() : lock(0), remaining(0) {}
		Releaser(FlowLock& lock, int64_t amount = 1) : lock(&lock), remaining(amount) {}
		Releaser(Releaser&& r) noexcept : lock(r.lock), remaining(r.remaining) { r.remaining = 0; }
		void operator=(Releaser&& r) {
			if (remaining)
				lock->release(remaining);
			lock = r.lock;
			remaining = r.remaining;
			r.remaining = 0;
		}

		void release(int64_t amount = -1) {
			if (amount == -1 || amount > remaining)
				amount = remaining;

			if (remaining)
				lock->release(amount);
			remaining -= amount;
		}

		~Releaser() {
			if (remaining)
				lock->release(remaining);
		}
	};

	FlowLock() : permits(1), active(0) {}
	explicit FlowLock(int64_t permits) : permits(permits), active(0) {}

	Future<Void> take(TaskPriority taskID = TaskPriority::DefaultYield, int64_t amount = 1) {
		if (active + amount <= permits || active == 0) {
			active += amount;
			return safeYieldActor(this, taskID, amount);
		}
		return takeActor(this, taskID, amount);
	}
	void release(int64_t amount = 1) {
		ASSERT((active > 0 || amount == 0) && active - amount >= 0);
		active -= amount;

		while (!takers.empty()) {
			if (active + takers.begin()->second <= permits || active == 0) {
				std::pair<Promise<Void>, int64_t> next = std::move(*takers.begin());
				active += next.second;
				takers.pop_front();
				next.first.send(Void());
			} else {
				break;
			}
		}
	}

	Future<Void> releaseWhen(Future<Void> const& signal, int amount = 1) {
		return releaseWhenActor(this, signal, amount);
	}

	// returns when any permits are available, having taken as many as possible up to the given amount, and modifies
	// amount to the number of permits taken
	Future<Void> takeUpTo(int64_t& amount) { return takeMoreActor(this, &amount); }

	int64_t available() const { return permits - active; }
	int64_t activePermits() const { return active; }
	int waiters() const { return takers.size(); }

	// Try to send error to all current and future waiters
	// Only works if broken_on_destruct.canBeSet()
	void kill(Error e = broken_promise()) {
		if (broken_on_destruct.canBeSet()) {
			auto local = broken_on_destruct;
			// It could be the case that calling broken_on_destruct destroys this FlowLock
			local.sendError(e);
		}
	}

private:
	std::list<std::pair<Promise<Void>, int64_t>> takers;
	const int64_t permits;
	int64_t active;
	Promise<Void> broken_on_destruct;

																#line 16596 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via takeActor()
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class TakeActorActor>
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TakeActorActorState {
															#line 16602 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TakeActorActorState(FlowLock* const& lock,TaskPriority const& taskID,int64_t const& amount) 
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lock(lock),
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID),
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   amount(amount),
															#line 1447 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   it(lock->takers.emplace(lock->takers.end(), Promise<Void>(), amount))
															#line 16615 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("takeActor", reinterpret_cast<unsigned long>(this));

	}
	~TakeActorActorState() 
	{
		fdb_probe_actor_destroy("takeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1451 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = it->first.getFuture();
															#line 1451 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TakeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 16633 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TakeActorActor*>(this)->actor_wait_state = 1;
															#line 1451 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TakeActorActor, 0, Void >*>(static_cast<TakeActorActor*>(this)));
															#line 16638 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TakeActorActorState();
		static_cast<TakeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		try {
															#line 1460 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			double duration = BUGGIFY_WITH_PROB(.001) ? deterministicRandom()->random01() * FLOW_KNOBS->BUGGIFY_FLOW_LOCK_RELEASE_DELAY : 0.0;
															#line 1464 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(duration, taskID);
															#line 1463 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TakeActorActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 16672 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 1466 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_2 = lock->broken_on_destruct.getFuture();
															#line 16676 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<TakeActorActor*>(this)->actor_wait_state = 2;
															#line 1464 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TakeActorActor, 1, Void >*>(static_cast<TakeActorActor*>(this)));
															#line 1466 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TakeActorActor, 2, Void >*>(static_cast<TakeActorActor*>(this)));
															#line 16683 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1453 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 16699 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1454 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				lock->takers.erase(it);
															#line 1455 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				lock->release(0);
															#line 16705 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1457 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 16709 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<TakeActorActor*>(this)->actor_wait_state > 0) static_cast<TakeActorActor*>(this)->actor_wait_state = 0;
		static_cast<TakeActorActor*>(this)->ActorCallback< TakeActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TakeActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TakeActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TakeActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont1Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1470 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			TEST(true);
															#line 1471 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			lock->release(amount);
															#line 1472 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 16816 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 1468 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TakeActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TakeActorActorState(); static_cast<TakeActorActor*>(this)->destroy(); return 0; }
															#line 16830 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TakeActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TakeActorActorState();
		static_cast<TakeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TakeActorActor*>(this)->actor_wait_state > 0) static_cast<TakeActorActor*>(this)->actor_wait_state = 0;
		static_cast<TakeActorActor*>(this)->ActorCallback< TakeActorActor, 1, Void >::remove();
		static_cast<TakeActorActor*>(this)->ActorCallback< TakeActorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TakeActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TakeActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TakeActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TakeActorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TakeActorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TakeActorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FlowLock* lock;
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int64_t amount;
															#line 1447 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	std::list<std::pair<Promise<Void>, int64_t>>::iterator it;
															#line 16967 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via takeActor()
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TakeActorActor final : public Actor<Void>, public ActorCallback< TakeActorActor, 0, Void >, public ActorCallback< TakeActorActor, 1, Void >, public ActorCallback< TakeActorActor, 2, Void >, public FastAllocated<TakeActorActor>, public TakeActorActorState<TakeActorActor> {
															#line 16972 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TakeActorActor>::operator new;
	using FastAllocated<TakeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TakeActorActor, 0, Void >;
friend struct ActorCallback< TakeActorActor, 1, Void >;
friend struct ActorCallback< TakeActorActor, 2, Void >;
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TakeActorActor(FlowLock* const& lock,TaskPriority const& taskID,int64_t const& amount) 
															#line 16985 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TakeActorActorState<TakeActorActor>(lock, taskID, amount)
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("takeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TakeActorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TakeActorActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> takeActor( FlowLock* const& lock, TaskPriority const& taskID, int64_t const& amount ) {
															#line 1446 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new TakeActorActor(lock, taskID, amount));
															#line 17013 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1475 "/usr/src/libfdb_c/flow/genericactors.actor.h"

																#line 17018 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via takeMoreActor()
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class TakeMoreActorActor>
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TakeMoreActorActorState {
															#line 17024 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TakeMoreActorActorState(FlowLock* const& lock,int64_t* const& amount) 
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lock(lock),
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   amount(amount)
															#line 17033 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("takeMoreActor", reinterpret_cast<unsigned long>(this));

	}
	~TakeMoreActorActorState() 
	{
		fdb_probe_actor_destroy("takeMoreActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1477 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = lock->take();
															#line 1477 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TakeMoreActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 17050 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TakeMoreActorActor*>(this)->actor_wait_state = 1;
															#line 1477 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TakeMoreActorActor, 0, Void >*>(static_cast<TakeMoreActorActor*>(this)));
															#line 17055 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TakeMoreActorActorState();
		static_cast<TakeMoreActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1478 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		int64_t extra = std::min(lock->available(), *amount - 1);
															#line 1479 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		lock->active += extra;
															#line 1480 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		*amount = 1 + extra;
															#line 1481 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TakeMoreActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TakeMoreActorActorState(); static_cast<TakeMoreActorActor*>(this)->destroy(); return 0; }
															#line 17084 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TakeMoreActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TakeMoreActorActorState();
		static_cast<TakeMoreActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1478 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		int64_t extra = std::min(lock->available(), *amount - 1);
															#line 1479 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		lock->active += extra;
															#line 1480 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		*amount = 1 + extra;
															#line 1481 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TakeMoreActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TakeMoreActorActorState(); static_cast<TakeMoreActorActor*>(this)->destroy(); return 0; }
															#line 17102 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TakeMoreActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TakeMoreActorActorState();
		static_cast<TakeMoreActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TakeMoreActorActor*>(this)->actor_wait_state > 0) static_cast<TakeMoreActorActor*>(this)->actor_wait_state = 0;
		static_cast<TakeMoreActorActor*>(this)->ActorCallback< TakeMoreActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TakeMoreActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("takeMoreActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeMoreActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TakeMoreActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("takeMoreActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeMoreActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TakeMoreActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("takeMoreActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeMoreActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FlowLock* lock;
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int64_t* amount;
															#line 17177 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via takeMoreActor()
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TakeMoreActorActor final : public Actor<Void>, public ActorCallback< TakeMoreActorActor, 0, Void >, public FastAllocated<TakeMoreActorActor>, public TakeMoreActorActorState<TakeMoreActorActor> {
															#line 17182 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TakeMoreActorActor>::operator new;
	using FastAllocated<TakeMoreActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TakeMoreActorActor, 0, Void >;
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TakeMoreActorActor(FlowLock* const& lock,int64_t* const& amount) 
															#line 17193 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TakeMoreActorActorState<TakeMoreActorActor>(lock, amount)
	{
		fdb_probe_actor_enter("takeMoreActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("takeMoreActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("takeMoreActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TakeMoreActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> takeMoreActor( FlowLock* const& lock, int64_t* const& amount ) {
															#line 1476 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new TakeMoreActorActor(lock, amount));
															#line 17220 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1483 "/usr/src/libfdb_c/flow/genericactors.actor.h"

																#line 17225 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via safeYieldActor()
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class SafeYieldActorActor>
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SafeYieldActorActorState {
															#line 17231 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SafeYieldActorActorState(FlowLock* const& lock,TaskPriority const& taskID,int64_t const& amount) 
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lock(lock),
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID),
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   amount(amount)
															#line 17242 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("safeYieldActor", reinterpret_cast<unsigned long>(this));

	}
	~SafeYieldActorActorState() 
	{
		fdb_probe_actor_destroy("safeYieldActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1487 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = yield(taskID);
															#line 1486 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<SafeYieldActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 17260 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1488 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_1 = lock->broken_on_destruct.getFuture();
															#line 17264 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<SafeYieldActorActor*>(this)->actor_wait_state = 1;
															#line 1487 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SafeYieldActorActor, 0, Void >*>(static_cast<SafeYieldActorActor*>(this)));
															#line 1488 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SafeYieldActorActor, 1, Void >*>(static_cast<SafeYieldActorActor*>(this)));
															#line 17271 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~SafeYieldActorActorState();
		static_cast<SafeYieldActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1492 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			lock->release(amount);
															#line 1493 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 17303 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 1490 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<SafeYieldActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SafeYieldActorActorState(); static_cast<SafeYieldActorActor*>(this)->destroy(); return 0; }
															#line 17317 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<SafeYieldActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SafeYieldActorActorState();
		static_cast<SafeYieldActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SafeYieldActorActor*>(this)->actor_wait_state > 0) static_cast<SafeYieldActorActor*>(this)->actor_wait_state = 0;
		static_cast<SafeYieldActorActor*>(this)->ActorCallback< SafeYieldActorActor, 0, Void >::remove();
		static_cast<SafeYieldActorActor*>(this)->ActorCallback< SafeYieldActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SafeYieldActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SafeYieldActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SafeYieldActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SafeYieldActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SafeYieldActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SafeYieldActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FlowLock* lock;
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int64_t amount;
															#line 17452 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via safeYieldActor()
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class SafeYieldActorActor final : public Actor<Void>, public ActorCallback< SafeYieldActorActor, 0, Void >, public ActorCallback< SafeYieldActorActor, 1, Void >, public FastAllocated<SafeYieldActorActor>, public SafeYieldActorActorState<SafeYieldActorActor> {
															#line 17457 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<SafeYieldActorActor>::operator new;
	using FastAllocated<SafeYieldActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SafeYieldActorActor, 0, Void >;
friend struct ActorCallback< SafeYieldActorActor, 1, Void >;
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	SafeYieldActorActor(FlowLock* const& lock,TaskPriority const& taskID,int64_t const& amount) 
															#line 17469 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   SafeYieldActorActorState<SafeYieldActorActor>(lock, taskID, amount)
	{
		fdb_probe_actor_enter("safeYieldActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("safeYieldActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("safeYieldActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SafeYieldActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> safeYieldActor( FlowLock* const& lock, TaskPriority const& taskID, int64_t const& amount ) {
															#line 1484 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new SafeYieldActorActor(lock, taskID, amount));
															#line 17496 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1496 "/usr/src/libfdb_c/flow/genericactors.actor.h"

																#line 17501 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via releaseWhenActor()
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class ReleaseWhenActorActor>
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ReleaseWhenActorActorState {
															#line 17507 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ReleaseWhenActorActorState(FlowLock* const& self,Future<Void> const& signal,int64_t const& amount) 
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : self(self),
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   signal(signal),
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   amount(amount)
															#line 17518 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("releaseWhenActor", reinterpret_cast<unsigned long>(this));

	}
	~ReleaseWhenActorActorState() 
	{
		fdb_probe_actor_destroy("releaseWhenActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1498 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = signal;
															#line 1498 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<ReleaseWhenActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 17535 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReleaseWhenActorActor*>(this)->actor_wait_state = 1;
															#line 1498 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReleaseWhenActorActor, 0, Void >*>(static_cast<ReleaseWhenActorActor*>(this)));
															#line 17540 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ReleaseWhenActorActorState();
		static_cast<ReleaseWhenActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1499 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->release(amount);
															#line 1500 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ReleaseWhenActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReleaseWhenActorActorState(); static_cast<ReleaseWhenActorActor*>(this)->destroy(); return 0; }
															#line 17565 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ReleaseWhenActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReleaseWhenActorActorState();
		static_cast<ReleaseWhenActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1499 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->release(amount);
															#line 1500 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ReleaseWhenActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReleaseWhenActorActorState(); static_cast<ReleaseWhenActorActor*>(this)->destroy(); return 0; }
															#line 17579 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ReleaseWhenActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReleaseWhenActorActorState();
		static_cast<ReleaseWhenActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ReleaseWhenActorActor*>(this)->actor_wait_state > 0) static_cast<ReleaseWhenActorActor*>(this)->actor_wait_state = 0;
		static_cast<ReleaseWhenActorActor*>(this)->ActorCallback< ReleaseWhenActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReleaseWhenActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("releaseWhenActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("releaseWhenActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReleaseWhenActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("releaseWhenActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("releaseWhenActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReleaseWhenActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("releaseWhenActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("releaseWhenActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FlowLock* self;
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> signal;
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int64_t amount;
															#line 17656 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via releaseWhenActor()
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ReleaseWhenActorActor final : public Actor<Void>, public ActorCallback< ReleaseWhenActorActor, 0, Void >, public FastAllocated<ReleaseWhenActorActor>, public ReleaseWhenActorActorState<ReleaseWhenActorActor> {
															#line 17661 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ReleaseWhenActorActor>::operator new;
	using FastAllocated<ReleaseWhenActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReleaseWhenActorActor, 0, Void >;
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ReleaseWhenActorActor(FlowLock* const& self,Future<Void> const& signal,int64_t const& amount) 
															#line 17672 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   ReleaseWhenActorActorState<ReleaseWhenActorActor>(self, signal, amount)
	{
		fdb_probe_actor_enter("releaseWhenActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("releaseWhenActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("releaseWhenActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReleaseWhenActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> releaseWhenActor( FlowLock* const& self, Future<Void> const& signal, int64_t const& amount ) {
															#line 1497 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new ReleaseWhenActorActor(self, signal, amount));
															#line 17699 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1502 "/usr/src/libfdb_c/flow/genericactors.actor.h"
};

struct NotifiedInt {
	NotifiedInt(int64_t val = 0) : val(val) {}

	Future<Void> whenAtLeast(int64_t limit) {
		if (val >= limit)
			return Void();
		Promise<Void> p;
		waiting.emplace(limit, p);
		return p.getFuture();
	}

	int64_t get() const { return val; }

	void set(int64_t v) {
		ASSERT(v >= val);
		if (v != val) {
			val = v;

			std::vector<Promise<Void>> toSend;
			while (waiting.size() && v >= waiting.top().first) {
				Promise<Void> p = std::move(waiting.top().second);
				waiting.pop();
				toSend.push_back(p);
			}
			for (auto& p : toSend) {
				p.send(Void());
			}
		}
	}

	void operator=(int64_t v) { set(v); }

	NotifiedInt(NotifiedInt&& r) noexcept : waiting(std::move(r.waiting)), val(r.val) {}
	void operator=(NotifiedInt&& r) noexcept {
		waiting = std::move(r.waiting);
		val = r.val;
	}

private:
	typedef std::pair<int64_t, Promise<Void>> Item;
	struct ItemCompare {
		bool operator()(const Item& a, const Item& b) { return a.first > b.first; }
	};
	std::priority_queue<Item, std::vector<Item>, ItemCompare> waiting;
	int64_t val;
};

struct BoundedFlowLock : NonCopyable, public ReferenceCounted<BoundedFlowLock> {
	// BoundedFlowLock is different from a FlowLock in that it has a bound on how many locks can be taken from the
	// oldest outstanding lock. For instance, with a FlowLock that has two permits, if one permit is taken but never
	// released, the other permit can be reused an unlimited amount of times, but with a BoundedFlowLock, it can only be
	// reused a fixed number of times.

	struct Releaser : NonCopyable {
		BoundedFlowLock* lock;
		int64_t permitNumber;
		Releaser() : lock(nullptr), permitNumber(0) {}
		Releaser(BoundedFlowLock* lock, int64_t permitNumber) : lock(lock), permitNumber(permitNumber) {}
		Releaser(Releaser&& r) noexcept : lock(r.lock), permitNumber(r.permitNumber) { r.permitNumber = 0; }
		void operator=(Releaser&& r) {
			if (permitNumber)
				lock->release(permitNumber);
			lock = r.lock;
			permitNumber = r.permitNumber;
			r.permitNumber = 0;
		}

		void release() {
			if (permitNumber) {
				lock->release(permitNumber);
			}
			permitNumber = 0;
		}

		~Releaser() {
			if (permitNumber)
				lock->release(permitNumber);
		}
	};

	BoundedFlowLock() : minOutstanding(0), nextPermitNumber(0), unrestrictedPermits(1), boundedPermits(0) {}
	explicit BoundedFlowLock(int64_t unrestrictedPermits, int64_t boundedPermits)
	  : minOutstanding(0), nextPermitNumber(0), unrestrictedPermits(unrestrictedPermits),
	    boundedPermits(boundedPermits) {}

	Future<int64_t> take() { return takeActor(this); }
	void release(int64_t permitNumber) {
		outstanding.erase(permitNumber);
		updateMinOutstanding();
	}

private:
	IndexedSet<int64_t, int64_t> outstanding;
	NotifiedInt minOutstanding;
	int64_t nextPermitNumber;
	const int64_t unrestrictedPermits;
	const int64_t boundedPermits;

	void updateMinOutstanding() {
		auto it = outstanding.index(unrestrictedPermits - 1);
		if (it == outstanding.end()) {
			minOutstanding.set(nextPermitNumber);
		} else {
			minOutstanding.set(*it);
		}
	}

																#line 17812 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via takeActor()
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class TakeActorActor1>
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TakeActorActor1State {
															#line 17818 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TakeActorActor1State(BoundedFlowLock* const& lock) 
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lock(lock),
															#line 1612 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   permitNumber(++lock->nextPermitNumber)
															#line 17827 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("takeActor", reinterpret_cast<unsigned long>(this));

	}
	~TakeActorActor1State() 
	{
		fdb_probe_actor_destroy("takeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1613 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			lock->outstanding.insert(permitNumber, 1);
															#line 1614 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			lock->updateMinOutstanding();
															#line 1615 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = lock->minOutstanding.whenAtLeast(std::max<int64_t>(0, permitNumber - lock->boundedPermits));
															#line 1615 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<TakeActorActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 17848 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TakeActorActor1*>(this)->actor_wait_state = 1;
															#line 1615 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TakeActorActor1, 0, Void >*>(static_cast<TakeActorActor1*>(this)));
															#line 17853 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TakeActorActor1State();
		static_cast<TakeActorActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1616 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TakeActorActor1*>(this)->SAV<int64_t>::futures) { (void)(permitNumber); this->~TakeActorActor1State(); static_cast<TakeActorActor1*>(this)->destroy(); return 0; }
															#line 17876 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TakeActorActor1*>(this)->SAV< int64_t >::value()) int64_t(std::move(permitNumber)); // state_var_RVO
		this->~TakeActorActor1State();
		static_cast<TakeActorActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1616 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TakeActorActor1*>(this)->SAV<int64_t>::futures) { (void)(permitNumber); this->~TakeActorActor1State(); static_cast<TakeActorActor1*>(this)->destroy(); return 0; }
															#line 17888 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TakeActorActor1*>(this)->SAV< int64_t >::value()) int64_t(std::move(permitNumber)); // state_var_RVO
		this->~TakeActorActor1State();
		static_cast<TakeActorActor1*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TakeActorActor1*>(this)->actor_wait_state > 0) static_cast<TakeActorActor1*>(this)->actor_wait_state = 0;
		static_cast<TakeActorActor1*>(this)->ActorCallback< TakeActorActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TakeActorActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TakeActorActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TakeActorActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	BoundedFlowLock* lock;
															#line 1612 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	int64_t permitNumber;
															#line 17963 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via takeActor()
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TakeActorActor1 final : public Actor<int64_t>, public ActorCallback< TakeActorActor1, 0, Void >, public FastAllocated<TakeActorActor1>, public TakeActorActor1State<TakeActorActor1> {
															#line 17968 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TakeActorActor1>::operator new;
	using FastAllocated<TakeActorActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TakeActorActor1, 0, Void >;
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TakeActorActor1(BoundedFlowLock* const& lock) 
															#line 17979 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<int64_t>(),
		   TakeActorActor1State<TakeActorActor1>(lock)
	{
		fdb_probe_actor_enter("takeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("takeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("takeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TakeActorActor1, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<int64_t> takeActor( BoundedFlowLock* const& lock ) {
															#line 1611 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<int64_t>(new TakeActorActor1(lock));
															#line 18006 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1618 "/usr/src/libfdb_c/flow/genericactors.actor.h"
};

															#line 18012 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via yieldPromiseStream()
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class YieldPromiseStreamActor>
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class YieldPromiseStreamActorState {
															#line 18019 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	YieldPromiseStreamActorState(FutureStream<T> const& input,PromiseStream<T> const& output,TaskPriority const& taskID = TaskPriority::DefaultYield) 
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output),
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   taskID(taskID)
															#line 18030 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("yieldPromiseStream", reinterpret_cast<unsigned long>(this));

	}
	~YieldPromiseStreamActorState() 
	{
		fdb_probe_actor_destroy("yieldPromiseStream", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1624 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 18045 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~YieldPromiseStreamActorState();
		static_cast<YieldPromiseStreamActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1625 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		FutureStream<T> __when_expr_0 = input;
															#line 1625 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 18077 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state = 1;
															#line 1625 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< YieldPromiseStreamActor, 0, T >*>(static_cast<YieldPromiseStreamActor*>(this)));
															#line 18082 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(T const& f,int loopDepth) 
	{
															#line 1626 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(f);
															#line 1627 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = yield(taskID);
															#line 1627 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 18095 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state = 2;
															#line 1627 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< YieldPromiseStreamActor, 1, Void >*>(static_cast<YieldPromiseStreamActor*>(this)));
															#line 18100 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(T && f,int loopDepth) 
	{
															#line 1626 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(f);
															#line 1627 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = yield(taskID);
															#line 1627 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 18113 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state = 2;
															#line 1627 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< YieldPromiseStreamActor, 1, Void >*>(static_cast<YieldPromiseStreamActor*>(this)));
															#line 18118 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& f,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(f, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && f,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(f), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state > 0) static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state = 0;
		static_cast<YieldPromiseStreamActor*>(this)->ActorSingleCallback< YieldPromiseStreamActor, 0, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< YieldPromiseStreamActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< YieldPromiseStreamActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< YieldPromiseStreamActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
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
		if (static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state > 0) static_cast<YieldPromiseStreamActor*>(this)->actor_wait_state = 0;
		static_cast<YieldPromiseStreamActor*>(this)->ActorCallback< YieldPromiseStreamActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< YieldPromiseStreamActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< YieldPromiseStreamActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< YieldPromiseStreamActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> input;
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> output;
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TaskPriority taskID;
															#line 18267 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via yieldPromiseStream()
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class YieldPromiseStreamActor final : public Actor<Void>, public ActorSingleCallback< YieldPromiseStreamActor<T>, 0, T >, public ActorCallback< YieldPromiseStreamActor<T>, 1, Void >, public FastAllocated<YieldPromiseStreamActor<T>>, public YieldPromiseStreamActorState<T, YieldPromiseStreamActor<T>> {
															#line 18274 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<YieldPromiseStreamActor<T>>::operator new;
	using FastAllocated<YieldPromiseStreamActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< YieldPromiseStreamActor<T>, 0, T >;
friend struct ActorCallback< YieldPromiseStreamActor<T>, 1, Void >;
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	YieldPromiseStreamActor(FutureStream<T> const& input,PromiseStream<T> const& output,TaskPriority const& taskID = TaskPriority::DefaultYield) 
															#line 18286 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   YieldPromiseStreamActorState<T, YieldPromiseStreamActor<T>>(input, output, taskID)
	{
		fdb_probe_actor_enter("yieldPromiseStream", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("yieldPromiseStream");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("yieldPromiseStream", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< YieldPromiseStreamActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< YieldPromiseStreamActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> yieldPromiseStream( FutureStream<T> const& input, PromiseStream<T> const& output, TaskPriority const& taskID = TaskPriority::DefaultYield ) {
															#line 1620 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new YieldPromiseStreamActor<T>(input, output, taskID));
															#line 18317 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1630 "/usr/src/libfdb_c/flow/genericactors.actor.h"

struct YieldedFutureActor final : SAV<Void>,
                                  ActorCallback<YieldedFutureActor, 1, Void>,
                                  FastAllocated<YieldedFutureActor> {
	Error in_error_state;

	typedef ActorCallback<YieldedFutureActor, 1, Void> CB1;

	using FastAllocated<YieldedFutureActor>::operator new;
	using FastAllocated<YieldedFutureActor>::operator delete;

	YieldedFutureActor(Future<Void>&& f) : SAV<Void>(1, 1), in_error_state(Error::fromCode(UNSET_ERROR_CODE)) {
		f.addYieldedCallbackAndClear(static_cast<ActorCallback<YieldedFutureActor, 1, Void>*>(this));
	}

	void cancel() override {
		if (!SAV<Void>::canBeSet())
			return; // Cancel could be invoked *by* a callback within finish().  Otherwise it's guaranteed that we are
			        // waiting either on the original future or on a delay().
		ActorCallback<YieldedFutureActor, 1, Void>::remove();
		SAV<Void>::sendErrorAndDelPromiseRef(actor_cancelled());
	}

	void destroy() override { delete this; }

#ifdef ENABLE_SAMPLING
	LineageReference* lineageAddr() { return currentLineage; }
#endif

	void a_callback_fire(ActorCallback<YieldedFutureActor, 1, Void>*, Void) {
		if (int16_t(in_error_state.code()) == UNSET_ERROR_CODE) {
			in_error_state = Error::fromCode(SET_ERROR_CODE);
			if (check_yield())
				doYield();
			else
				finish();
		} else {
			// We hit this case when and only when the delay() created by a previous doYield() fires.  Then we want to
			// get at least one task done, regardless of what check_yield() would say.
			finish();
		}
	}
	void a_callback_error(ActorCallback<YieldedFutureActor, 1, Void>*, Error const& err) {
		ASSERT(int16_t(in_error_state.code()) == UNSET_ERROR_CODE);
		in_error_state = err;
		if (check_yield())
			doYield();
		else
			finish();
	}
	void finish() {
		ActorCallback<YieldedFutureActor, 1, Void>::remove();
		if (int16_t(in_error_state.code()) == SET_ERROR_CODE)
			SAV<Void>::sendAndDelPromiseRef(Void());
		else
			SAV<Void>::sendErrorAndDelPromiseRef(in_error_state);
	}
	void doYield() {
		// Since we are being fired, we are the first callback in the ring, and `prev` is the source future
		Callback<Void>* source = CB1::prev;
		ASSERT(source->next == static_cast<CB1*>(this));

		// Remove the source future from the ring.  All the remaining callbacks in the ring should be yielded, since
		// yielded callbacks are installed at the end
		CB1::prev = source->prev;
		CB1::prev->next = static_cast<CB1*>(this);

		// The source future's ring is now empty, since we have removed all the callbacks
		source->next = source->prev = source;
		source->unwait();

		// Link all the callbacks, including this one, into the ring of a delay future so that after a short time they
		// will be fired again
		delay(0, g_network->getCurrentTask()).addCallbackChainAndClear(static_cast<CB1*>(this));
	}
};

inline Future<Void> yieldedFuture(Future<Void> f) {
	if (f.isReady())
		return yield();
	else
		return Future<Void>(new YieldedFutureActor(std::move(f)));
}

// An AsyncMap that uses a yieldedFuture in its onChange method.
template <class K, class V>
class YieldedAsyncMap : public AsyncMap<K, V> {
public:
	Future<Void> onChange(K const& k) override { // throws broken_promise if this is destroyed
		auto& item = AsyncMap<K, V>::items[k];
		if (item.value == AsyncMap<K, V>::defaultValue)
			return destroyOnCancelYield(this, k, item.change.getFuture());
		return yieldedFuture(item.change.getFuture());
	}

																#line 18416 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via destroyOnCancelYield()
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class DestroyOnCancelYieldActor>
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DestroyOnCancelYieldActorState {
															#line 18422 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DestroyOnCancelYieldActorState(YieldedAsyncMap* const& self,K const& key,Future<Void> const& change) 
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : self(self),
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   key(key),
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   change(change)
															#line 18433 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("destroyOnCancelYield", reinterpret_cast<unsigned long>(this));

	}
	~DestroyOnCancelYieldActorState() 
	{
		fdb_probe_actor_destroy("destroyOnCancelYield", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1727 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<Void> __when_expr_0 = yieldedFuture(change);
															#line 1727 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<DestroyOnCancelYieldActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 18451 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DestroyOnCancelYieldActor*>(this)->actor_wait_state = 1;
															#line 1727 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DestroyOnCancelYieldActor, 0, Void >*>(static_cast<DestroyOnCancelYieldActor*>(this)));
															#line 18456 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DestroyOnCancelYieldActorState();
		static_cast<DestroyOnCancelYieldActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1730 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_actor_cancelled && !self->destructing && change.getFutureReferenceCount() == 1 && change.getPromiseReferenceCount() == 1)
															#line 18486 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1732 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (EXPENSIVE_VALIDATION)
															#line 18490 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				{
															#line 1733 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					auto& p = self->items[key];
															#line 1734 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					ASSERT(p.change.getFuture() == change);
															#line 18496 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				}
															#line 1736 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				self->items.erase(key);
															#line 18500 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1738 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 18504 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
															#line 1728 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DestroyOnCancelYieldActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DestroyOnCancelYieldActorState(); static_cast<DestroyOnCancelYieldActor*>(this)->destroy(); return 0; }
															#line 18518 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DestroyOnCancelYieldActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DestroyOnCancelYieldActorState();
		static_cast<DestroyOnCancelYieldActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1728 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DestroyOnCancelYieldActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DestroyOnCancelYieldActorState(); static_cast<DestroyOnCancelYieldActor*>(this)->destroy(); return 0; }
															#line 18530 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DestroyOnCancelYieldActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DestroyOnCancelYieldActorState();
		static_cast<DestroyOnCancelYieldActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<DestroyOnCancelYieldActor*>(this)->actor_wait_state > 0) static_cast<DestroyOnCancelYieldActor*>(this)->actor_wait_state = 0;
		static_cast<DestroyOnCancelYieldActor*>(this)->ActorCallback< DestroyOnCancelYieldActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DestroyOnCancelYieldActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DestroyOnCancelYieldActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DestroyOnCancelYieldActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	YieldedAsyncMap* self;
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	K key;
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<Void> change;
															#line 18607 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via destroyOnCancelYield()
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DestroyOnCancelYieldActor final : public Actor<Void>, public ActorCallback< DestroyOnCancelYieldActor, 0, Void >, public FastAllocated<DestroyOnCancelYieldActor>, public DestroyOnCancelYieldActorState<DestroyOnCancelYieldActor> {
															#line 18612 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DestroyOnCancelYieldActor>::operator new;
	using FastAllocated<DestroyOnCancelYieldActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DestroyOnCancelYieldActor, 0, Void >;
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DestroyOnCancelYieldActor(YieldedAsyncMap* const& self,K const& key,Future<Void> const& change) 
															#line 18623 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   DestroyOnCancelYieldActorState<DestroyOnCancelYieldActor>(self, key, change)
	{
		fdb_probe_actor_enter("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("destroyOnCancelYield");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("destroyOnCancelYield", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DestroyOnCancelYieldActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> destroyOnCancelYield( YieldedAsyncMap* const& self, K const& key, Future<Void> const& change ) {
															#line 1725 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new DestroyOnCancelYieldActor(self, key, change));
															#line 18650 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1741 "/usr/src/libfdb_c/flow/genericactors.actor.h"
};

															#line 18656 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via delayActionJittered()
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class DelayActionJitteredActor>
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DelayActionJitteredActorState {
															#line 18663 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DelayActionJitteredActorState(Future<T> const& what,double const& time) 
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : what(what),
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   time(time)
															#line 18672 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("delayActionJittered", reinterpret_cast<unsigned long>(this));

	}
	~DelayActionJitteredActorState() 
	{
		fdb_probe_actor_destroy("delayActionJittered", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1745 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Void> __when_expr_0 = delayJittered(time);
															#line 1745 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<DelayActionJitteredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 18689 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DelayActionJitteredActor*>(this)->actor_wait_state = 1;
															#line 1745 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DelayActionJitteredActor, 0, Void >*>(static_cast<DelayActionJitteredActor*>(this)));
															#line 18694 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~DelayActionJitteredActorState();
		static_cast<DelayActionJitteredActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1746 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<T> __when_expr_1 = what;
															#line 1746 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DelayActionJitteredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 18719 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DelayActionJitteredActor*>(this)->actor_wait_state = 2;
															#line 1746 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DelayActionJitteredActor, 1, T >*>(static_cast<DelayActionJitteredActor*>(this)));
															#line 18724 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1746 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<T> __when_expr_1 = what;
															#line 1746 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<DelayActionJitteredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 18735 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DelayActionJitteredActor*>(this)->actor_wait_state = 2;
															#line 1746 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DelayActionJitteredActor, 1, T >*>(static_cast<DelayActionJitteredActor*>(this)));
															#line 18740 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<DelayActionJitteredActor*>(this)->actor_wait_state > 0) static_cast<DelayActionJitteredActor*>(this)->actor_wait_state = 0;
		static_cast<DelayActionJitteredActor*>(this)->ActorCallback< DelayActionJitteredActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DelayActionJitteredActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DelayActionJitteredActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DelayActionJitteredActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(T const& t,int loopDepth) 
	{
															#line 1747 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DelayActionJitteredActor*>(this)->SAV<T>::futures) { (void)(t); this->~DelayActionJitteredActorState(); static_cast<DelayActionJitteredActor*>(this)->destroy(); return 0; }
															#line 18812 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DelayActionJitteredActor*>(this)->SAV< T >::value()) T(t);
		this->~DelayActionJitteredActorState();
		static_cast<DelayActionJitteredActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && t,int loopDepth) 
	{
															#line 1747 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<DelayActionJitteredActor*>(this)->SAV<T>::futures) { (void)(t); this->~DelayActionJitteredActorState(); static_cast<DelayActionJitteredActor*>(this)->destroy(); return 0; }
															#line 18824 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<DelayActionJitteredActor*>(this)->SAV< T >::value()) T(t);
		this->~DelayActionJitteredActorState();
		static_cast<DelayActionJitteredActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(T const& t,int loopDepth) 
	{
		loopDepth = a_body1cont2(t, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(T && t,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(t), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DelayActionJitteredActor*>(this)->actor_wait_state > 0) static_cast<DelayActionJitteredActor*>(this)->actor_wait_state = 0;
		static_cast<DelayActionJitteredActor*>(this)->ActorCallback< DelayActionJitteredActor, 1, T >::remove();

	}
	void a_callback_fire(ActorCallback< DelayActionJitteredActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DelayActionJitteredActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DelayActionJitteredActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> what;
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double time;
															#line 18899 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via delayActionJittered()
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class DelayActionJitteredActor final : public Actor<T>, public ActorCallback< DelayActionJitteredActor<T>, 0, Void >, public ActorCallback< DelayActionJitteredActor<T>, 1, T >, public FastAllocated<DelayActionJitteredActor<T>>, public DelayActionJitteredActorState<T, DelayActionJitteredActor<T>> {
															#line 18906 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<DelayActionJitteredActor<T>>::operator new;
	using FastAllocated<DelayActionJitteredActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DelayActionJitteredActor<T>, 0, Void >;
friend struct ActorCallback< DelayActionJitteredActor<T>, 1, T >;
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	DelayActionJitteredActor(Future<T> const& what,double const& time) 
															#line 18918 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   DelayActionJitteredActorState<T, DelayActionJitteredActor<T>>(what, time)
	{
		fdb_probe_actor_enter("delayActionJittered", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("delayActionJittered");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("delayActionJittered", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DelayActionJitteredActor<T>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DelayActionJitteredActor<T>, 1, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> delayActionJittered( Future<T> const& what, double const& time ) {
															#line 1743 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new DelayActionJitteredActor<T>(what, time));
															#line 18949 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1749 "/usr/src/libfdb_c/flow/genericactors.actor.h"

class AndFuture {
public:
	AndFuture() {}

	AndFuture(AndFuture const& f) { futures = f.futures; }

	AndFuture(AndFuture&& f) noexcept { futures = std::move(f.futures); }

	AndFuture(Future<Void> const& f) { futures.push_back(f); }

	AndFuture(Error const& e) { futures.push_back(e); }

	operator Future<Void>() { return getFuture(); }

	void operator=(AndFuture const& f) { futures = f.futures; }

	void operator=(AndFuture&& f) noexcept { futures = std::move(f.futures); }

	void operator=(Future<Void> const& f) { futures.push_back(f); }

	void operator=(Error const& e) { futures.push_back(e); }

	Future<Void> getFuture() {
		if (futures.empty())
			return Void();

		if (futures.size() == 1)
			return futures[0];

		Future<Void> f = waitForAll(futures);
		futures = std::vector<Future<Void>>{ f };
		return f;
	}

	bool isReady() {
		for (int i = futures.size() - 1; i >= 0; --i) {
			if (!futures[i].isReady()) {
				return false;
			} else if (!futures[i].isError()) {
				swapAndPop(&futures, i);
			}
		}
		return true;
	}

	bool isError() {
		for (int i = 0; i < futures.size(); i++)
			if (futures[i].isError())
				return true;
		return false;
	}

	void cleanup() {
		for (int i = 0; i < futures.size(); i++) {
			if (futures[i].isReady() && !futures[i].isError()) {
				swapAndPop(&futures, i--);
			}
		}
	}

	void add(Future<Void> const& f) {
		if (!f.isReady() || f.isError())
			futures.push_back(f);
	}

	void add(AndFuture f) { add(f.getFuture()); }

private:
	std::vector<Future<Void>> futures;
};

// Performs an unordered merge of a and b.
															#line 19026 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via unorderedMergeStreams()
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class UnorderedMergeStreamsActor>
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class UnorderedMergeStreamsActorState {
															#line 19033 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UnorderedMergeStreamsActorState(FutureStream<T> const& a,FutureStream<T> const& b,PromiseStream<T> const& output) 
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : a(a),
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   b(b),
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output),
															#line 1824 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   aFuture(waitAndForward(a)),
															#line 1825 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   bFuture(waitAndForward(b)),
															#line 1826 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   aOpen(true),
															#line 1827 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   bOpen(true)
															#line 19052 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("unorderedMergeStreams", reinterpret_cast<unsigned long>(this));

	}
	~UnorderedMergeStreamsActorState() 
	{
		fdb_probe_actor_destroy("unorderedMergeStreams", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1829 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 19067 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~UnorderedMergeStreamsActorState();
		static_cast<UnorderedMergeStreamsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1865 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<UnorderedMergeStreamsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnorderedMergeStreamsActorState(); static_cast<UnorderedMergeStreamsActor*>(this)->destroy(); return 0; }
															#line 19090 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<UnorderedMergeStreamsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnorderedMergeStreamsActorState();
		static_cast<UnorderedMergeStreamsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 1832 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = aFuture;
															#line 1831 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<UnorderedMergeStreamsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19112 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 1836 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_1 = bFuture;
															#line 19116 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<UnorderedMergeStreamsActor*>(this)->actor_wait_state = 1;
															#line 1832 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnorderedMergeStreamsActor, 0, T >*>(static_cast<UnorderedMergeStreamsActor*>(this)));
															#line 1836 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnorderedMergeStreamsActor, 1, T >*>(static_cast<UnorderedMergeStreamsActor*>(this)));
															#line 19123 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1842 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() != error_code_end_of_stream)
															#line 19158 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1843 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				output.sendError(e);
															#line 19162 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 1847 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			ASSERT(!aFuture.isError() || !bFuture.isError() || aFuture.getError().code() == bFuture.getError().code());
															#line 1849 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (aFuture.isError())
															#line 19169 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1850 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				aFuture = Never();
															#line 1851 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				aOpen = false;
															#line 19175 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1853 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (bFuture.isError())
															#line 19179 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1854 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				bFuture = Never();
															#line 1855 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				bOpen = false;
															#line 19185 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1858 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!aOpen && !bOpen)
															#line 19189 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1859 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				output.sendError(e);
															#line 19193 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
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
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& val,int loopDepth) 
	{
															#line 1833 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(val);
															#line 1834 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		aFuture = waitAndForward(a);
															#line 19218 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && val,int loopDepth) 
	{
															#line 1833 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(val);
															#line 1834 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		aFuture = waitAndForward(a);
															#line 19229 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(T const& val,int loopDepth) 
	{
															#line 1837 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(val);
															#line 1838 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		bFuture = waitAndForward(b);
															#line 19240 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(T && val,int loopDepth) 
	{
															#line 1837 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output.send(val);
															#line 1838 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		bFuture = waitAndForward(b);
															#line 19251 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UnorderedMergeStreamsActor*>(this)->actor_wait_state > 0) static_cast<UnorderedMergeStreamsActor*>(this)->actor_wait_state = 0;
		static_cast<UnorderedMergeStreamsActor*>(this)->ActorCallback< UnorderedMergeStreamsActor, 0, T >::remove();
		static_cast<UnorderedMergeStreamsActor*>(this)->ActorCallback< UnorderedMergeStreamsActor, 1, T >::remove();

	}
	void a_callback_fire(ActorCallback< UnorderedMergeStreamsActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnorderedMergeStreamsActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnorderedMergeStreamsActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnorderedMergeStreamsActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< UnorderedMergeStreamsActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< UnorderedMergeStreamsActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont3(int loopDepth) 
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
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> a;
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> b;
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> output;
															#line 1824 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> aFuture;
															#line 1825 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> bFuture;
															#line 1826 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool aOpen;
															#line 1827 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool bOpen;
															#line 19380 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via unorderedMergeStreams()
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class UnorderedMergeStreamsActor final : public Actor<Void>, public ActorCallback< UnorderedMergeStreamsActor<T>, 0, T >, public ActorCallback< UnorderedMergeStreamsActor<T>, 1, T >, public FastAllocated<UnorderedMergeStreamsActor<T>>, public UnorderedMergeStreamsActorState<T, UnorderedMergeStreamsActor<T>> {
															#line 19387 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<UnorderedMergeStreamsActor<T>>::operator new;
	using FastAllocated<UnorderedMergeStreamsActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnorderedMergeStreamsActor<T>, 0, T >;
friend struct ActorCallback< UnorderedMergeStreamsActor<T>, 1, T >;
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UnorderedMergeStreamsActor(FutureStream<T> const& a,FutureStream<T> const& b,PromiseStream<T> const& output) 
															#line 19399 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   UnorderedMergeStreamsActorState<T, UnorderedMergeStreamsActor<T>>(a, b, output)
	{
		fdb_probe_actor_enter("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unorderedMergeStreams");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unorderedMergeStreams", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnorderedMergeStreamsActor<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> unorderedMergeStreams( FutureStream<T> const& a, FutureStream<T> const& b, PromiseStream<T> const& output ) {
															#line 1822 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new UnorderedMergeStreamsActor<T>(a, b, output));
															#line 19429 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1867 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Returns the ordered merge of a and b, assuming that a and b are both already ordered (prefer a over b if keys are
// equal). T must be a class that implements compare()
															#line 19436 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via orderedMergeStreams()
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class OrderedMergeStreamsActor>
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class OrderedMergeStreamsActorState {
															#line 19443 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	OrderedMergeStreamsActorState(FutureStream<T> const& a,FutureStream<T> const& b,PromiseStream<T> const& output) 
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : a(a),
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   b(b),
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output),
															#line 1872 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   savedKVa(),
															#line 1873 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   aOpen(),
															#line 1874 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   savedKVb(),
															#line 1875 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   bOpen()
															#line 19462 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("orderedMergeStreams", reinterpret_cast<unsigned long>(this));

	}
	~OrderedMergeStreamsActorState() 
	{
		fdb_probe_actor_destroy("orderedMergeStreams", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1877 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			aOpen = bOpen = true;
															#line 1879 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 19479 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~OrderedMergeStreamsActorState();
		static_cast<OrderedMergeStreamsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1937 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<OrderedMergeStreamsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OrderedMergeStreamsActorState(); static_cast<OrderedMergeStreamsActor*>(this)->destroy(); return 0; }
															#line 19502 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<OrderedMergeStreamsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OrderedMergeStreamsActorState();
		static_cast<OrderedMergeStreamsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 1880 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (aOpen && !savedKVa.present())
															#line 19521 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
			try {
															#line 1882 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				FutureStream<T> __when_expr_0 = a;
															#line 1882 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19528 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
				static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state = 1;
															#line 1882 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< OrderedMergeStreamsActor, 0, T >*>(static_cast<OrderedMergeStreamsActor*>(this)));
															#line 19533 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1loopBody1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 1896 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (bOpen && !savedKVb.present())
															#line 19566 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
			try {
															#line 1898 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				FutureStream<T> __when_expr_1 = b;
															#line 1898 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1Catch1(actor_cancelled(), loopDepth);
															#line 19573 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont1when1(__when_expr_1.pop(), loopDepth); };
				static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state = 2;
															#line 1898 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< OrderedMergeStreamsActor, 1, T >*>(static_cast<OrderedMergeStreamsActor*>(this)));
															#line 19578 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1loopBody1cont1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1loopBody1cont1Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1885 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 19605 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1886 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				aOpen = false;
															#line 1887 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (!bOpen)
															#line 19611 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				{
															#line 1888 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					output.sendError(e);
															#line 19615 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				}
			}
			else
			{
															#line 1891 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				output.sendError(e);
															#line 19622 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(T const& KVa,int loopDepth) 
	{
															#line 1883 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		savedKVa = Optional<T>(KVa);
															#line 19639 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(T && KVa,int loopDepth) 
	{
															#line 1883 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		savedKVa = Optional<T>(KVa);
															#line 19648 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T const& KVa,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(KVa, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(T && KVa,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(KVa), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state > 0) static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state = 0;
		static_cast<OrderedMergeStreamsActor*>(this)->ActorSingleCallback< OrderedMergeStreamsActor, 0, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< OrderedMergeStreamsActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< OrderedMergeStreamsActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< OrderedMergeStreamsActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 1913 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!aOpen)
															#line 19733 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		{
															#line 1914 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			output.send(savedKVb.get());
															#line 1915 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			savedKVb = Optional<T>();
															#line 19739 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		else
		{
															#line 1916 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (!bOpen)
															#line 19745 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1917 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				output.send(savedKVa.get());
															#line 1918 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				savedKVa = Optional<T>();
															#line 19751 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
			else
			{
															#line 1920 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				int cmp = savedKVa.get().compare(savedKVb.get());
															#line 1922 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (cmp == 0)
															#line 19759 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				{
															#line 1924 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					output.send(savedKVa.get());
															#line 1925 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					savedKVa = Optional<T>();
															#line 1926 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					savedKVb = Optional<T>();
															#line 19767 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				}
				else
				{
															#line 1927 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					if (cmp < 0)
															#line 19773 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
					{
															#line 1928 "/usr/src/libfdb_c/flow/genericactors.actor.h"
						output.send(savedKVa.get());
															#line 1929 "/usr/src/libfdb_c/flow/genericactors.actor.h"
						savedKVa = Optional<T>();
															#line 19779 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
					}
					else
					{
															#line 1931 "/usr/src/libfdb_c/flow/genericactors.actor.h"
						output.send(savedKVb.get());
															#line 1932 "/usr/src/libfdb_c/flow/genericactors.actor.h"
						savedKVb = Optional<T>();
															#line 19787 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
					}
				}
			}
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1901 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 19807 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1902 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				bOpen = false;
															#line 1903 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (!aOpen)
															#line 19813 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				{
															#line 1904 "/usr/src/libfdb_c/flow/genericactors.actor.h"
					output.sendError(e);
															#line 19817 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				}
			}
			else
			{
															#line 1907 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				output.sendError(e);
															#line 19824 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			loopDepth = a_body1loopBody1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont8(T const& KVb,int loopDepth) 
	{
															#line 1899 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		savedKVb = Optional<T>(KVb);
															#line 19841 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(T && KVb,int loopDepth) 
	{
															#line 1899 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		savedKVb = Optional<T>(KVb);
															#line 19850 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(T const& KVb,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(KVb, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(T && KVb,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(KVb), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state > 0) static_cast<OrderedMergeStreamsActor*>(this)->actor_wait_state = 0;
		static_cast<OrderedMergeStreamsActor*>(this)->ActorSingleCallback< OrderedMergeStreamsActor, 1, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< OrderedMergeStreamsActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< OrderedMergeStreamsActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorSingleCallback< OrderedMergeStreamsActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont10(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> a;
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FutureStream<T> b;
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<T> output;
															#line 1872 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Optional<T> savedKVa;
															#line 1873 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool aOpen;
															#line 1874 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Optional<T> savedKVb;
															#line 1875 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	bool bOpen;
															#line 19945 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via orderedMergeStreams()
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class OrderedMergeStreamsActor final : public Actor<Void>, public ActorSingleCallback< OrderedMergeStreamsActor<T>, 0, T >, public ActorSingleCallback< OrderedMergeStreamsActor<T>, 1, T >, public FastAllocated<OrderedMergeStreamsActor<T>>, public OrderedMergeStreamsActorState<T, OrderedMergeStreamsActor<T>> {
															#line 19952 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<OrderedMergeStreamsActor<T>>::operator new;
	using FastAllocated<OrderedMergeStreamsActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< OrderedMergeStreamsActor<T>, 0, T >;
friend struct ActorSingleCallback< OrderedMergeStreamsActor<T>, 1, T >;
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	OrderedMergeStreamsActor(FutureStream<T> const& a,FutureStream<T> const& b,PromiseStream<T> const& output) 
															#line 19964 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   OrderedMergeStreamsActorState<T, OrderedMergeStreamsActor<T>>(a, b, output)
	{
		fdb_probe_actor_enter("orderedMergeStreams", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("orderedMergeStreams");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("orderedMergeStreams", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< OrderedMergeStreamsActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorSingleCallback< OrderedMergeStreamsActor<T>, 1, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> orderedMergeStreams( FutureStream<T> const& a, FutureStream<T> const& b, PromiseStream<T> const& output ) {
															#line 1870 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new OrderedMergeStreamsActor<T>(a, b, output));
															#line 19995 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1939 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 20000 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via timeReply()
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class TimeReplyActor>
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeReplyActorState {
															#line 20007 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeReplyActorState(Future<T> const& replyToTime,PromiseStream<double> const& timeOutput) 
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : replyToTime(replyToTime),
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   timeOutput(timeOutput),
															#line 1942 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   startTime(now())
															#line 20018 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("timeReply", reinterpret_cast<unsigned long>(this));

	}
	~TimeReplyActorState() 
	{
		fdb_probe_actor_destroy("timeReply", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1944 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = replyToTime;
															#line 1944 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<TimeReplyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 20036 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TimeReplyActor*>(this)->actor_wait_state = 1;
															#line 1944 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeReplyActor, 0, T >*>(static_cast<TimeReplyActor*>(this)));
															#line 20041 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~TimeReplyActorState();
		static_cast<TimeReplyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1954 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<TimeReplyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TimeReplyActorState(); static_cast<TimeReplyActor*>(this)->destroy(); return 0; }
															#line 20070 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<TimeReplyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TimeReplyActorState();
		static_cast<TimeReplyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1951 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() != error_code_broken_promise)
															#line 20083 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1952 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				return a_body1Catch1(e, loopDepth);
															#line 20087 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& _,int loopDepth) 
	{
															#line 1945 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0);
															#line 1945 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<TimeReplyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 20105 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<TimeReplyActor*>(this)->actor_wait_state = 2;
															#line 1945 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeReplyActor, 1, Void >*>(static_cast<TimeReplyActor*>(this)));
															#line 20110 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(T && _,int loopDepth) 
	{
															#line 1945 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0);
															#line 1945 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<TimeReplyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 20121 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<TimeReplyActor*>(this)->actor_wait_state = 2;
															#line 1945 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeReplyActor, 1, Void >*>(static_cast<TimeReplyActor*>(this)));
															#line 20126 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeReplyActor*>(this)->actor_wait_state > 0) static_cast<TimeReplyActor*>(this)->actor_wait_state = 0;
		static_cast<TimeReplyActor*>(this)->ActorCallback< TimeReplyActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< TimeReplyActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeReplyActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeReplyActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1946 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		timeOutput.send(now() - startTime);
															#line 20198 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1946 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		timeOutput.send(now() - startTime);
															#line 20207 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TimeReplyActor*>(this)->actor_wait_state > 0) static_cast<TimeReplyActor*>(this)->actor_wait_state = 0;
		static_cast<TimeReplyActor*>(this)->ActorCallback< TimeReplyActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeReplyActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeReplyActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeReplyActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), 1);

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
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> replyToTime;
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	PromiseStream<double> timeOutput;
															#line 1942 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	double startTime;
															#line 20294 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via timeReply()
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class TimeReplyActor final : public Actor<Void>, public ActorCallback< TimeReplyActor<T>, 0, T >, public ActorCallback< TimeReplyActor<T>, 1, Void >, public FastAllocated<TimeReplyActor<T>>, public TimeReplyActorState<T, TimeReplyActor<T>> {
															#line 20301 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<TimeReplyActor<T>>::operator new;
	using FastAllocated<TimeReplyActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeReplyActor<T>, 0, T >;
friend struct ActorCallback< TimeReplyActor<T>, 1, Void >;
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	TimeReplyActor(Future<T> const& replyToTime,PromiseStream<double> const& timeOutput) 
															#line 20313 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   TimeReplyActorState<T, TimeReplyActor<T>>(replyToTime, timeOutput)
	{
		fdb_probe_actor_enter("timeReply", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeReply");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeReply", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeReplyActor<T>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TimeReplyActor<T>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> timeReply( Future<T> const& replyToTime, PromiseStream<double> const& timeOutput ) {
															#line 1940 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new TimeReplyActor<T>(replyToTime, timeOutput));
															#line 20344 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1956 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 20349 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via forward()
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class ForwardActor1>
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardActor1State {
															#line 20356 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardActor1State(Future<T> const& from,Promise<T> const& to) 
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : from(from),
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   to(to)
															#line 20365 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("forward", reinterpret_cast<unsigned long>(this));

	}
	~ForwardActor1State() 
	{
		fdb_probe_actor_destroy("forward", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1960 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				StrictFuture<T> __when_expr_0 = from;
															#line 1960 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				if (static_cast<ForwardActor1*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 20383 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ForwardActor1*>(this)->actor_wait_state = 1;
															#line 1960 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForwardActor1, 0, T >*>(static_cast<ForwardActor1*>(this)));
															#line 20388 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~ForwardActor1State();
		static_cast<ForwardActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1964 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (e.code() != error_code_actor_cancelled)
															#line 20418 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			{
															#line 1965 "/usr/src/libfdb_c/flow/genericactors.actor.h"
				to.sendError(e);
															#line 20422 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			}
															#line 1967 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 20426 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& res,int loopDepth) 
	{
															#line 1961 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		to.send(res);
															#line 1962 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ForwardActor1*>(this)->SAV<T>::futures) { (void)(res); this->~ForwardActor1State(); static_cast<ForwardActor1*>(this)->destroy(); return 0; }
															#line 20442 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ForwardActor1*>(this)->SAV< T >::value()) T(res);
		this->~ForwardActor1State();
		static_cast<ForwardActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(T && res,int loopDepth) 
	{
															#line 1961 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		to.send(res);
															#line 1962 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<ForwardActor1*>(this)->SAV<T>::futures) { (void)(res); this->~ForwardActor1State(); static_cast<ForwardActor1*>(this)->destroy(); return 0; }
															#line 20456 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<ForwardActor1*>(this)->SAV< T >::value()) T(res);
		this->~ForwardActor1State();
		static_cast<ForwardActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& res,int loopDepth) 
	{
		loopDepth = a_body1cont2(res, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && res,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ForwardActor1*>(this)->actor_wait_state > 0) static_cast<ForwardActor1*>(this)->actor_wait_state = 0;
		static_cast<ForwardActor1*>(this)->ActorCallback< ForwardActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< ForwardActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForwardActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForwardActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> from;
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Promise<T> to;
															#line 20531 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via forward()
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class ForwardActor1 final : public Actor<T>, public ActorCallback< ForwardActor1<T>, 0, T >, public FastAllocated<ForwardActor1<T>>, public ForwardActor1State<T, ForwardActor1<T>> {
															#line 20538 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<ForwardActor1<T>>::operator new;
	using FastAllocated<ForwardActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ForwardActor1<T>, 0, T >;
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	ForwardActor1(Future<T> const& from,Promise<T> const& to) 
															#line 20549 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<T>(),
		   ForwardActor1State<T, ForwardActor1<T>>(from, to)
	{
		fdb_probe_actor_enter("forward", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forward");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forward", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ForwardActor1<T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T>
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<T> forward( Future<T> const& from, Promise<T> const& to ) {
															#line 1957 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<T>(new ForwardActor1<T>(from, to));
															#line 20579 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1970 "/usr/src/libfdb_c/flow/genericactors.actor.h"

// Monad

															#line 20586 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via fmap()
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Fun, class T, class FmapActor>
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class FmapActorState {
															#line 20593 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FmapActorState(Fun const& fun,Future<T> const& f) 
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : fun(fun),
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   f(f)
															#line 20602 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("fmap", reinterpret_cast<unsigned long>(this));

	}
	~FmapActorState() 
	{
		fdb_probe_actor_destroy("fmap", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1975 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = f;
															#line 1975 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<FmapActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 20619 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FmapActor*>(this)->actor_wait_state = 1;
															#line 1975 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FmapActor, 0, T >*>(static_cast<FmapActor*>(this)));
															#line 20624 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~FmapActorState();
		static_cast<FmapActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val,int loopDepth) 
	{
															#line 1976 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<FmapActor*>(this)->SAV<decltype(std::declval<Fun>()(std::declval<T>()))>::futures) { (void)(fun(val)); this->~FmapActorState(); static_cast<FmapActor*>(this)->destroy(); return 0; }
															#line 20647 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<FmapActor*>(this)->SAV< decltype(std::declval<Fun>()(std::declval<T>())) >::value()) decltype(std::declval<Fun>()(std::declval<T>()))(fun(val));
		this->~FmapActorState();
		static_cast<FmapActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(T && val,int loopDepth) 
	{
															#line 1976 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<FmapActor*>(this)->SAV<decltype(std::declval<Fun>()(std::declval<T>()))>::futures) { (void)(fun(val)); this->~FmapActorState(); static_cast<FmapActor*>(this)->destroy(); return 0; }
															#line 20659 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<FmapActor*>(this)->SAV< decltype(std::declval<Fun>()(std::declval<T>())) >::value()) decltype(std::declval<Fun>()(std::declval<T>()))(fun(val));
		this->~FmapActorState();
		static_cast<FmapActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(T const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FmapActor*>(this)->actor_wait_state > 0) static_cast<FmapActor*>(this)->actor_wait_state = 0;
		static_cast<FmapActor*>(this)->ActorCallback< FmapActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< FmapActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("fmap", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fmap", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FmapActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("fmap", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fmap", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FmapActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("fmap", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fmap", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Fun fun;
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> f;
															#line 20734 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via fmap()
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Fun, class T>
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class FmapActor final : public Actor<decltype(std::declval<Fun>()(std::declval<T>()))>, public ActorCallback< FmapActor<Fun, T>, 0, T >, public FastAllocated<FmapActor<Fun, T>>, public FmapActorState<Fun, T, FmapActor<Fun, T>> {
															#line 20741 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<FmapActor<Fun, T>>::operator new;
	using FastAllocated<FmapActor<Fun, T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Fun>()(std::declval<T>()))>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FmapActor<Fun, T>, 0, T >;
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	FmapActor(Fun const& fun,Future<T> const& f) 
															#line 20752 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<decltype(std::declval<Fun>()(std::declval<T>()))>(),
		   FmapActorState<Fun, T, FmapActor<Fun, T>>(fun, f)
	{
		fdb_probe_actor_enter("fmap", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fmap");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fmap", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FmapActor<Fun, T>, 0, T >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class Fun, class T>
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<decltype(std::declval<Fun>()(std::declval<T>()))> fmap( Fun const& fun, Future<T> const& f ) {
															#line 1973 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<decltype(std::declval<Fun>()(std::declval<T>()))>(new FmapActor<Fun, T>(fun, f));
															#line 20782 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1978 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 20787 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via runAfter()
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class Fun, class RunAfterActor>
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RunAfterActorState {
															#line 20794 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RunAfterActorState(Future<T> const& lhs,Fun const& rhs) 
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lhs(lhs),
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   rhs(rhs)
															#line 20803 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("runAfter", reinterpret_cast<unsigned long>(this));

	}
	~RunAfterActorState() 
	{
		fdb_probe_actor_destroy("runAfter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1981 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = lhs;
															#line 1981 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<RunAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 20820 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunAfterActor*>(this)->actor_wait_state = 1;
															#line 1981 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunAfterActor, 0, T >*>(static_cast<RunAfterActor*>(this)));
															#line 20825 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~RunAfterActorState();
		static_cast<RunAfterActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val1,int loopDepth) 
	{
															#line 1982 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<decltype(std::declval<Fun>()(std::declval<T>()).getValue())> __when_expr_1 = rhs(val1);
															#line 1982 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<RunAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 20850 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunAfterActor*>(this)->actor_wait_state = 2;
															#line 1982 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunAfterActor, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >*>(static_cast<RunAfterActor*>(this)));
															#line 20855 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T && val1,int loopDepth) 
	{
															#line 1982 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<decltype(std::declval<Fun>()(std::declval<T>()).getValue())> __when_expr_1 = rhs(val1);
															#line 1982 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<RunAfterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 20866 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunAfterActor*>(this)->actor_wait_state = 2;
															#line 1982 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunAfterActor, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >*>(static_cast<RunAfterActor*>(this)));
															#line 20871 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& val1,int loopDepth) 
	{
		loopDepth = a_body1cont1(val1, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val1,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val1), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunAfterActor*>(this)->actor_wait_state > 0) static_cast<RunAfterActor*>(this)->actor_wait_state = 0;
		static_cast<RunAfterActor*>(this)->ActorCallback< RunAfterActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< RunAfterActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunAfterActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunAfterActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(decltype(std::declval<Fun>()(std::declval<T>()).getValue()) const& res,int loopDepth) 
	{
															#line 1983 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<RunAfterActor*>(this)->SAV<decltype(std::declval<Fun>()(std::declval<T>()).getValue())>::futures) { (void)(res); this->~RunAfterActorState(); static_cast<RunAfterActor*>(this)->destroy(); return 0; }
															#line 20943 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<RunAfterActor*>(this)->SAV< decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >::value()) decltype(std::declval<Fun>()(std::declval<T>()).getValue())(res);
		this->~RunAfterActorState();
		static_cast<RunAfterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(decltype(std::declval<Fun>()(std::declval<T>()).getValue()) && res,int loopDepth) 
	{
															#line 1983 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<RunAfterActor*>(this)->SAV<decltype(std::declval<Fun>()(std::declval<T>()).getValue())>::futures) { (void)(res); this->~RunAfterActorState(); static_cast<RunAfterActor*>(this)->destroy(); return 0; }
															#line 20955 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<RunAfterActor*>(this)->SAV< decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >::value()) decltype(std::declval<Fun>()(std::declval<T>()).getValue())(res);
		this->~RunAfterActorState();
		static_cast<RunAfterActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(decltype(std::declval<Fun>()(std::declval<T>()).getValue()) const& res,int loopDepth) 
	{
		loopDepth = a_body1cont2(res, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(decltype(std::declval<Fun>()(std::declval<T>()).getValue()) && res,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RunAfterActor*>(this)->actor_wait_state > 0) static_cast<RunAfterActor*>(this)->actor_wait_state = 0;
		static_cast<RunAfterActor*>(this)->ActorCallback< RunAfterActor, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >::remove();

	}
	void a_callback_fire(ActorCallback< RunAfterActor, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >*,decltype(std::declval<Fun>()(std::declval<T>()).getValue()) const& value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunAfterActor, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >*,decltype(std::declval<Fun>()(std::declval<T>()).getValue()) && value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunAfterActor, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >*,Error err) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> lhs;
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Fun rhs;
															#line 21030 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via runAfter()
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class Fun>
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RunAfterActor final : public Actor<decltype(std::declval<Fun>()(std::declval<T>()).getValue())>, public ActorCallback< RunAfterActor<T, Fun>, 0, T >, public ActorCallback< RunAfterActor<T, Fun>, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >, public FastAllocated<RunAfterActor<T, Fun>>, public RunAfterActorState<T, Fun, RunAfterActor<T, Fun>> {
															#line 21037 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<RunAfterActor<T, Fun>>::operator new;
	using FastAllocated<RunAfterActor<T, Fun>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Fun>()(std::declval<T>()).getValue())>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunAfterActor<T, Fun>, 0, T >;
friend struct ActorCallback< RunAfterActor<T, Fun>, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >;
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RunAfterActor(Future<T> const& lhs,Fun const& rhs) 
															#line 21049 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<decltype(std::declval<Fun>()(std::declval<T>()).getValue())>(),
		   RunAfterActorState<T, Fun, RunAfterActor<T, Fun>>(lhs, rhs)
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runAfter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunAfterActor<T, Fun>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunAfterActor<T, Fun>, 1, decltype(std::declval<Fun>()(std::declval<T>()).getValue()) >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class Fun>
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<decltype(std::declval<Fun>()(std::declval<T>()).getValue())> runAfter( Future<T> const& lhs, Fun const& rhs ) {
															#line 1979 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<decltype(std::declval<Fun>()(std::declval<T>()).getValue())>(new RunAfterActor<T, Fun>(lhs, rhs));
															#line 21080 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1985 "/usr/src/libfdb_c/flow/genericactors.actor.h"

															#line 21085 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
namespace {
// This generated class is to be used only via runAfter()
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class U, class RunAfterActor1>
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RunAfterActor1State {
															#line 21092 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RunAfterActor1State(Future<T> const& lhs,Future<U> const& rhs) 
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : lhs(lhs),
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   rhs(rhs)
															#line 21101 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("runAfter", reinterpret_cast<unsigned long>(this));

	}
	~RunAfterActor1State() 
	{
		fdb_probe_actor_destroy("runAfter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1988 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<T> __when_expr_0 = lhs;
															#line 1988 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<RunAfterActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 21118 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunAfterActor1*>(this)->actor_wait_state = 1;
															#line 1988 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunAfterActor1, 0, T >*>(static_cast<RunAfterActor1*>(this)));
															#line 21123 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~RunAfterActor1State();
		static_cast<RunAfterActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T const& val1,int loopDepth) 
	{
															#line 1989 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<U> __when_expr_1 = rhs;
															#line 1989 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<RunAfterActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 21148 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunAfterActor1*>(this)->actor_wait_state = 2;
															#line 1989 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunAfterActor1, 1, U >*>(static_cast<RunAfterActor1*>(this)));
															#line 21153 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(T && val1,int loopDepth) 
	{
															#line 1989 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<U> __when_expr_1 = rhs;
															#line 1989 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<RunAfterActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 21164 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunAfterActor1*>(this)->actor_wait_state = 2;
															#line 1989 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunAfterActor1, 1, U >*>(static_cast<RunAfterActor1*>(this)));
															#line 21169 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(T const& val1,int loopDepth) 
	{
		loopDepth = a_body1cont1(val1, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && val1,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val1), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunAfterActor1*>(this)->actor_wait_state > 0) static_cast<RunAfterActor1*>(this)->actor_wait_state = 0;
		static_cast<RunAfterActor1*>(this)->ActorCallback< RunAfterActor1, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< RunAfterActor1, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunAfterActor1, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunAfterActor1, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(U const& res,int loopDepth) 
	{
															#line 1990 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<RunAfterActor1*>(this)->SAV<U>::futures) { (void)(res); this->~RunAfterActor1State(); static_cast<RunAfterActor1*>(this)->destroy(); return 0; }
															#line 21241 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<RunAfterActor1*>(this)->SAV< U >::value()) U(res);
		this->~RunAfterActor1State();
		static_cast<RunAfterActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(U && res,int loopDepth) 
	{
															#line 1990 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<RunAfterActor1*>(this)->SAV<U>::futures) { (void)(res); this->~RunAfterActor1State(); static_cast<RunAfterActor1*>(this)->destroy(); return 0; }
															#line 21253 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<RunAfterActor1*>(this)->SAV< U >::value()) U(res);
		this->~RunAfterActor1State();
		static_cast<RunAfterActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(U const& res,int loopDepth) 
	{
		loopDepth = a_body1cont2(res, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(U && res,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RunAfterActor1*>(this)->actor_wait_state > 0) static_cast<RunAfterActor1*>(this)->actor_wait_state = 0;
		static_cast<RunAfterActor1*>(this)->ActorCallback< RunAfterActor1, 1, U >::remove();

	}
	void a_callback_fire(ActorCallback< RunAfterActor1, 1, U >*,U const& value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunAfterActor1, 1, U >*,U && value) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunAfterActor1, 1, U >*,Error err) 
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<T> lhs;
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Future<U> rhs;
															#line 21328 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via runAfter()
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class U>
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class RunAfterActor1 final : public Actor<U>, public ActorCallback< RunAfterActor1<T, U>, 0, T >, public ActorCallback< RunAfterActor1<T, U>, 1, U >, public FastAllocated<RunAfterActor1<T, U>>, public RunAfterActor1State<T, U, RunAfterActor1<T, U>> {
															#line 21335 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<RunAfterActor1<T, U>>::operator new;
	using FastAllocated<RunAfterActor1<T, U>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<U>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunAfterActor1<T, U>, 0, T >;
friend struct ActorCallback< RunAfterActor1<T, U>, 1, U >;
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	RunAfterActor1(Future<T> const& lhs,Future<U> const& rhs) 
															#line 21347 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<U>(),
		   RunAfterActor1State<T, U, RunAfterActor1<T, U>>(lhs, rhs)
	{
		fdb_probe_actor_enter("runAfter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runAfter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runAfter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunAfterActor1<T, U>, 0, T >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunAfterActor1<T, U>, 1, U >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class T, class U>
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<U> runAfter( Future<T> const& lhs, Future<U> const& rhs ) {
															#line 1986 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<U>(new RunAfterActor1<T, U>(lhs, rhs));
															#line 21378 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 1992 "/usr/src/libfdb_c/flow/genericactors.actor.h"

template <class T, class Fun>
auto operator>>=(Future<T> lhs, Fun&& rhs) -> Future<decltype(rhs(std::declval<T>()))> {
	return runAfter(lhs, std::forward<Fun>(rhs));
}

template <class T, class U>
Future<U> operator>>(Future<T> const& lhs, Future<U> const& rhs) {
	return runAfter(lhs, rhs);
}

/*
 * IAsyncListener is similar to AsyncVar, but it decouples the input and output, so the translation unit
 * responsible for handling the output does not need to have knowledge of how the output is generated
 */
template <class Output>
class IAsyncListener : public ReferenceCounted<IAsyncListener<Output>> {
public:
	virtual ~IAsyncListener() = default;
	virtual Output const& get() const = 0;
	virtual Future<Void> onChange() const = 0;
	template <class Input, class F>
	static Reference<IAsyncListener> create(Reference<AsyncVar<Input> const> const& input, F const& f);
	template <class Input, class F>
	static Reference<IAsyncListener> create(Reference<AsyncVar<Input>> const& input, F const& f);
	static Reference<IAsyncListener> create(Reference<AsyncVar<Output>> const& output);
};

namespace IAsyncListenerImpl {

template <class Input, class Output, class F>
class AsyncListener final : public IAsyncListener<Output> {
	// Order matters here, output must outlive monitorActor
	AsyncVar<Output> output;
	Future<Void> monitorActor;
																#line 21417 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via monitor()
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class MonitorActor>
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MonitorActorState {
															#line 21423 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MonitorActorState(Reference<AsyncVar<Input> const> const& input,AsyncVar<Output>* const& output,F const& f) 
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : input(input),
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   output(output),
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		   f(f)
															#line 21434 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("monitor", reinterpret_cast<unsigned long>(this));

	}
	~MonitorActorState() 
	{
		fdb_probe_actor_destroy("monitor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2028 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			;
															#line 21449 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~MonitorActorState();
		static_cast<MonitorActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2029 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		StrictFuture<Void> __when_expr_0 = input->onChange();
															#line 2029 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (static_cast<MonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 21481 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MonitorActor*>(this)->actor_wait_state = 1;
															#line 2029 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MonitorActor, 0, Void >*>(static_cast<MonitorActor*>(this)));
															#line 21486 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 2030 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output->set(f(input->get()));
															#line 21495 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 2030 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		output->set(f(input->get()));
															#line 21504 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		if (static_cast<MonitorActor*>(this)->actor_wait_state > 0) static_cast<MonitorActor*>(this)->actor_wait_state = 0;
		static_cast<MonitorActor*>(this)->ActorCallback< MonitorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MonitorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("monitor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MonitorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("monitor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MonitorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("monitor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("monitor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	Reference<AsyncVar<Input> const> input;
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	AsyncVar<Output>* output;
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	F f;
															#line 21578 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via monitor()
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MonitorActor final : public Actor<Void>, public ActorCallback< MonitorActor, 0, Void >, public FastAllocated<MonitorActor>, public MonitorActorState<MonitorActor> {
															#line 21583 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<MonitorActor>::operator new;
	using FastAllocated<MonitorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MonitorActor, 0, Void >;
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MonitorActor(Reference<AsyncVar<Input> const> const& input,AsyncVar<Output>* const& output,F const& f) 
															#line 21594 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   MonitorActorState<MonitorActor>(input, output, f)
	{
		fdb_probe_actor_enter("monitor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("monitor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("monitor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MonitorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] static Future<Void> monitor( Reference<AsyncVar<Input> const> const& input, AsyncVar<Output>* const& output, F const& f ) {
															#line 2027 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new MonitorActor(input, output, f));
															#line 21621 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 2033 "/usr/src/libfdb_c/flow/genericactors.actor.h"

public:
	AsyncListener(Reference<AsyncVar<Input> const> const& input, F const& f)
	  : output(f(input->get())), monitorActor(monitor(input, &output, f)) {}
	Output const& get() const override { return output.get(); }
	Future<Void> onChange() const override { return output.onChange(); }
};

} // namespace IAsyncListenerImpl

template <class Output>
template <class Input, class F>
Reference<IAsyncListener<Output>> IAsyncListener<Output>::create(Reference<AsyncVar<Input> const> const& input,
                                                                 F const& f) {
	return makeReference<IAsyncListenerImpl::AsyncListener<Input, Output, F>>(input, f);
}

template <class Output>
template <class Input, class F>
Reference<IAsyncListener<Output>> IAsyncListener<Output>::create(Reference<AsyncVar<Input>> const& input, F const& f) {
	return create(Reference<AsyncVar<Input> const>(input), f);
}

template <class Output>
Reference<IAsyncListener<Output>> IAsyncListener<Output>::create(Reference<AsyncVar<Output>> const& input) {
	auto identity = [](const auto& x) { return x; };
	return makeReference<IAsyncListenerImpl::AsyncListener<Output, Output, decltype(identity)>>(input, identity);
}

// A weak reference type to wrap a future Reference<T> object.
// Once the future is complete, this object holds a pointer to the referenced object but does
// not contribute to its reference count.
//
// WARNING: this class will not be aware when the underlying object is destroyed. It is up to the
// user to make sure that an UnsafeWeakFutureReference is discarded at the same time the object is.
template <class T>
class UnsafeWeakFutureReference {
public:
	UnsafeWeakFutureReference() {}
	UnsafeWeakFutureReference(Future<Reference<T>> future) : data(new UnsafeWeakFutureReferenceData(future)) {}

	// Returns a future to obtain a normal reference handle
	// If the future is ready, this creates a Reference<T> to wrap the object
	Future<Reference<T>> get() {
		if (!data) {
			return Reference<T>();
		} else if (data->ptr.present()) {
			return Reference<T>::addRef(data->ptr.get());
		} else {
			return data->future;
		}
	}

	// Returns the raw pointer, if the object is ready
	// Note: this should be used with care, as this pointer is not counted as a reference to the object and
	// it could be deleted if all normal references are destroyed.
	Optional<T*> getPtrIfReady() { return data->ptr; }

private:
	// A class to hold the state for an UnsafeWeakFutureReference
	struct UnsafeWeakFutureReferenceData : public ReferenceCounted<UnsafeWeakFutureReferenceData>, NonCopyable {
		Optional<T*> ptr;
		Future<Reference<T>> future;
		Future<Void> moveResultFuture;

		UnsafeWeakFutureReferenceData(Future<Reference<T>> future) : future(future) {
			moveResultFuture = moveResult(this);
		}

		// Waits for the future to complete and then stores the pointer in local storage
		// When this completes, we will no longer be counted toward the reference count of the object
																	#line 21696 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
// This generated class is to be used only via moveResult()
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
template <class MoveResultActor>
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MoveResultActorState {
															#line 21702 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MoveResultActorState(UnsafeWeakFutureReferenceData* const& self) 
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		 : self(self)
															#line 21709 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
	{
		fdb_probe_actor_create("moveResult", reinterpret_cast<unsigned long>(this));

	}
	~MoveResultActorState() 
	{
		fdb_probe_actor_destroy("moveResult", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			StrictFuture<Reference<T>> __when_expr_0 = self->future;
															#line 2105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			if (static_cast<MoveResultActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 21726 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MoveResultActor*>(this)->actor_wait_state = 1;
															#line 2105 "/usr/src/libfdb_c/flow/genericactors.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MoveResultActor, 0, Reference<T> >*>(static_cast<MoveResultActor*>(this)));
															#line 21731 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
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
		this->~MoveResultActorState();
		static_cast<MoveResultActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<T> const& result,int loopDepth) 
	{
															#line 2106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->ptr = result.getPtr();
															#line 2107 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->future = Future<Reference<T>>();
															#line 2108 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MoveResultActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MoveResultActorState(); static_cast<MoveResultActor*>(this)->destroy(); return 0; }
															#line 21758 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MoveResultActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MoveResultActorState();
		static_cast<MoveResultActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<T> && result,int loopDepth) 
	{
															#line 2106 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->ptr = result.getPtr();
															#line 2107 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		self->future = Future<Reference<T>>();
															#line 2108 "/usr/src/libfdb_c/flow/genericactors.actor.h"
		if (!static_cast<MoveResultActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MoveResultActorState(); static_cast<MoveResultActor*>(this)->destroy(); return 0; }
															#line 21774 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		new (&static_cast<MoveResultActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MoveResultActorState();
		static_cast<MoveResultActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<T> const& result,int loopDepth) 
	{
		loopDepth = a_body1cont1(result, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<T> && result,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MoveResultActor*>(this)->actor_wait_state > 0) static_cast<MoveResultActor*>(this)->actor_wait_state = 0;
		static_cast<MoveResultActor*>(this)->ActorCallback< MoveResultActor, 0, Reference<T> >::remove();

	}
	void a_callback_fire(ActorCallback< MoveResultActor, 0, Reference<T> >*,Reference<T> const& value) 
	{
		fdb_probe_actor_enter("moveResult", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveResult", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MoveResultActor, 0, Reference<T> >*,Reference<T> && value) 
	{
		fdb_probe_actor_enter("moveResult", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveResult", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MoveResultActor, 0, Reference<T> >*,Error err) 
	{
		fdb_probe_actor_enter("moveResult", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveResult", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	UnsafeWeakFutureReferenceData* self;
															#line 21847 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
};
// This generated class is to be used only via moveResult()
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
class MoveResultActor final : public Actor<Void>, public ActorCallback< MoveResultActor, 0, Reference<T> >, public FastAllocated<MoveResultActor>, public MoveResultActorState<MoveResultActor> {
															#line 21852 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
public:
	using FastAllocated<MoveResultActor>::operator new;
	using FastAllocated<MoveResultActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MoveResultActor, 0, Reference<T> >;
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	MoveResultActor(UnsafeWeakFutureReferenceData* const& self) 
															#line 21863 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
		 : Actor<Void>(),
		   MoveResultActorState<MoveResultActor>(self)
	{
		fdb_probe_actor_enter("moveResult", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("moveResult");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("moveResult", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MoveResultActor, 0, Reference<T> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
[[nodiscard]] Future<Void> moveResult( UnsafeWeakFutureReferenceData* const& self ) {
															#line 2104 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	return Future<Void>(new MoveResultActor(self));
															#line 21890 "/usr/src/libfdb_c/flow/genericactors.actor.g.h"
}

#line 2110 "/usr/src/libfdb_c/flow/genericactors.actor.h"
	};

	Reference<UnsafeWeakFutureReferenceData> data;
};

#include "flow/unactorcompiler.h"

#endif
