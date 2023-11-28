#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
/*
 * RunTransaction.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_RUNTRANSACTION_ACTOR_G_H)
#define FDBCLIENT_RUNTRANSACTION_ACTOR_G_H
#include "fdbclient/RunTransaction.actor.g.h"
#elif !defined(FDBCLIENT_RUNTRANSACTION_ACTOR_H)
#define FDBCLIENT_RUNTRANSACTION_ACTOR_H

#include <utility>

#include "flow/flow.h"
#include "fdbclient/ReadYourWrites.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 39 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransaction()
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function, class RunRYWTransactionActor>
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
class RunRYWTransactionActorState {
															#line 46 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
public:
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	RunRYWTransactionActorState(Database const& cx,Function const& func) 
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		 : cx(cx),
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		   func(func),
															#line 41 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 57 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
	{
		fdb_probe_actor_create("runRYWTransaction", reinterpret_cast<unsigned long>(this));

	}
	~RunRYWTransactionActorState() 
	{
		fdb_probe_actor_destroy("runRYWTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 42 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			;
															#line 72 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
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
		this->~RunRYWTransactionActorState();
		static_cast<RunRYWTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 45 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> __when_expr_0 = func(tr);
															#line 45 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 105 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 1;
															#line 45 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*>(static_cast<RunRYWTransactionActor*>(this)));
															#line 110 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
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
															#line 50 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 50 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 134 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 3;
															#line 50 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionActor, 2, Void >*>(static_cast<RunRYWTransactionActor*>(this)));
															#line 139 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			loopDepth = 0;
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
															#line 47 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 47 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 156 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 2;
															#line 47 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionActor, 1, Void >*>(static_cast<RunRYWTransactionActor*>(this)));
															#line 161 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& __result,int loopDepth) 
	{
															#line 45 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		result = __result;
															#line 170 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) && __result,int loopDepth) 
	{
		result = std::move(__result);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionActor*>(this)->ActorCallback< RunRYWTransactionActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& value) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) && value) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 48 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunRYWTransactionActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionActorState(); static_cast<RunRYWTransactionActor*>(this)->destroy(); return 0; }
															#line 237 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionActorState();
		static_cast<RunRYWTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 48 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunRYWTransactionActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionActorState(); static_cast<RunRYWTransactionActor*>(this)->destroy(); return 0; }
															#line 249 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionActorState();
		static_cast<RunRYWTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionActor*>(this)->ActorCallback< RunRYWTransactionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
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
	void a_exitChoose3() 
	{
		if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionActor*>(this)->ActorCallback< RunRYWTransactionActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Database cx;
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Function func;
															#line 41 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 45 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) result;
															#line 403 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransaction()
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function>
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
class RunRYWTransactionActor final : public Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>, public ActorCallback< RunRYWTransactionActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >, public ActorCallback< RunRYWTransactionActor<Function>, 1, Void >, public ActorCallback< RunRYWTransactionActor<Function>, 2, Void >, public FastAllocated<RunRYWTransactionActor<Function>>, public RunRYWTransactionActorState<Function, RunRYWTransactionActor<Function>> {
															#line 410 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
public:
	using FastAllocated<RunRYWTransactionActor<Function>>::operator new;
	using FastAllocated<RunRYWTransactionActor<Function>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunRYWTransactionActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >;
friend struct ActorCallback< RunRYWTransactionActor<Function>, 1, Void >;
friend struct ActorCallback< RunRYWTransactionActor<Function>, 2, Void >;
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	RunRYWTransactionActor(Database const& cx,Function const& func) 
															#line 423 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		 : Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(),
		   RunRYWTransactionActorState<Function, RunRYWTransactionActor<Function>>(cx, func)
	{
		fdb_probe_actor_enter("runRYWTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runRYWTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runRYWTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunRYWTransactionActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunRYWTransactionActor<Function>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RunRYWTransactionActor<Function>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function>
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> runRYWTransaction( Database const& cx, Function const& func ) {
															#line 37 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(new RunRYWTransactionActor<Function>(cx, func));
															#line 455 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
}

#line 54 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"

															#line 460 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransactionFailIfLocked()
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function, class RunRYWTransactionFailIfLockedActor>
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
class RunRYWTransactionFailIfLockedActorState {
															#line 467 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
public:
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	RunRYWTransactionFailIfLockedActorState(Database const& cx,Function const& func) 
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		 : cx(cx),
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		   func(func),
															#line 58 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 478 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
	{
		fdb_probe_actor_create("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this));

	}
	~RunRYWTransactionFailIfLockedActorState() 
	{
		fdb_probe_actor_destroy("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 59 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			;
															#line 493 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
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
		this->~RunRYWTransactionFailIfLockedActorState();
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 61 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> __when_expr_0 = func(tr);
															#line 61 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 526 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 1;
															#line 61 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionFailIfLockedActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*>(static_cast<RunRYWTransactionFailIfLockedActor*>(this)));
															#line 531 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
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
															#line 66 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			if (e.code() == error_code_database_locked)
															#line 553 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			{
															#line 67 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 557 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			}
															#line 68 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 68 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 563 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 3;
															#line 68 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionFailIfLockedActor, 2, Void >*>(static_cast<RunRYWTransactionFailIfLockedActor*>(this)));
															#line 568 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			loopDepth = 0;
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
															#line 63 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 63 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 585 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 2;
															#line 63 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionFailIfLockedActor, 1, Void >*>(static_cast<RunRYWTransactionFailIfLockedActor*>(this)));
															#line 590 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& __result,int loopDepth) 
	{
															#line 61 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		result = __result;
															#line 599 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) && __result,int loopDepth) 
	{
		result = std::move(__result);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->ActorCallback< RunRYWTransactionFailIfLockedActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionFailIfLockedActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionFailIfLockedActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionFailIfLockedActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 64 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionFailIfLockedActorState(); static_cast<RunRYWTransactionFailIfLockedActor*>(this)->destroy(); return 0; }
															#line 666 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionFailIfLockedActorState();
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 64 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionFailIfLockedActorState(); static_cast<RunRYWTransactionFailIfLockedActor*>(this)->destroy(); return 0; }
															#line 678 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionFailIfLockedActorState();
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->ActorCallback< RunRYWTransactionFailIfLockedActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionFailIfLockedActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionFailIfLockedActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionFailIfLockedActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
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
	void a_exitChoose3() 
	{
		if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->ActorCallback< RunRYWTransactionFailIfLockedActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionFailIfLockedActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionFailIfLockedActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionFailIfLockedActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Database cx;
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Function func;
															#line 58 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 61 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) result;
															#line 832 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransactionFailIfLocked()
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function>
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
class RunRYWTransactionFailIfLockedActor final : public Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>, public ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >, public ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 1, Void >, public ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 2, Void >, public FastAllocated<RunRYWTransactionFailIfLockedActor<Function>>, public RunRYWTransactionFailIfLockedActorState<Function, RunRYWTransactionFailIfLockedActor<Function>> {
															#line 839 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
public:
	using FastAllocated<RunRYWTransactionFailIfLockedActor<Function>>::operator new;
	using FastAllocated<RunRYWTransactionFailIfLockedActor<Function>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >;
friend struct ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 1, Void >;
friend struct ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 2, Void >;
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	RunRYWTransactionFailIfLockedActor(Database const& cx,Function const& func) 
															#line 852 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		 : Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(),
		   RunRYWTransactionFailIfLockedActorState<Function, RunRYWTransactionFailIfLockedActor<Function>>(cx, func)
	{
		fdb_probe_actor_enter("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runRYWTransactionFailIfLocked");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runRYWTransactionFailIfLocked", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function>
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> runRYWTransactionFailIfLocked( Database const& cx, Function const& func ) {
															#line 55 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(new RunRYWTransactionFailIfLockedActor<Function>(cx, func));
															#line 884 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
}

#line 72 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"

															#line 889 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransactionNoRetry()
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function, class RunRYWTransactionNoRetryActor>
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
class RunRYWTransactionNoRetryActorState {
															#line 896 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
public:
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	RunRYWTransactionNoRetryActorState(Database const& cx,Function const& func) 
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		 : cx(cx),
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		   func(func),
															#line 77 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 907 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
	{
		fdb_probe_actor_create("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this));

	}
	~RunRYWTransactionNoRetryActorState() 
	{
		fdb_probe_actor_destroy("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 78 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> __when_expr_0 = func(tr);
															#line 78 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 924 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state = 1;
															#line 78 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionNoRetryActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*>(static_cast<RunRYWTransactionNoRetryActor*>(this)));
															#line 929 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
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
		this->~RunRYWTransactionNoRetryActorState();
		static_cast<RunRYWTransactionNoRetryActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 79 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 79 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 954 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state = 2;
															#line 79 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionNoRetryActor, 1, Void >*>(static_cast<RunRYWTransactionNoRetryActor*>(this)));
															#line 959 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& __result,int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		result = __result;
															#line 968 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) && __result,int loopDepth) 
	{
		result = std::move(__result);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionNoRetryActor*>(this)->ActorCallback< RunRYWTransactionNoRetryActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionNoRetryActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionNoRetryActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionNoRetryActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 80 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionNoRetryActorState(); static_cast<RunRYWTransactionNoRetryActor*>(this)->destroy(); return 0; }
															#line 1035 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionNoRetryActorState();
		static_cast<RunRYWTransactionNoRetryActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 80 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionNoRetryActorState(); static_cast<RunRYWTransactionNoRetryActor*>(this)->destroy(); return 0; }
															#line 1047 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionNoRetryActorState();
		static_cast<RunRYWTransactionNoRetryActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionNoRetryActor*>(this)->ActorCallback< RunRYWTransactionNoRetryActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionNoRetryActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionNoRetryActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionNoRetryActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Database cx;
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Function func;
															#line 77 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 78 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) result;
															#line 1126 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransactionNoRetry()
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function>
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
class RunRYWTransactionNoRetryActor final : public Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>, public ActorCallback< RunRYWTransactionNoRetryActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >, public ActorCallback< RunRYWTransactionNoRetryActor<Function>, 1, Void >, public FastAllocated<RunRYWTransactionNoRetryActor<Function>>, public RunRYWTransactionNoRetryActorState<Function, RunRYWTransactionNoRetryActor<Function>> {
															#line 1133 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
public:
	using FastAllocated<RunRYWTransactionNoRetryActor<Function>>::operator new;
	using FastAllocated<RunRYWTransactionNoRetryActor<Function>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunRYWTransactionNoRetryActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >;
friend struct ActorCallback< RunRYWTransactionNoRetryActor<Function>, 1, Void >;
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	RunRYWTransactionNoRetryActor(Database const& cx,Function const& func) 
															#line 1145 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
		 : Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(),
		   RunRYWTransactionNoRetryActorState<Function, RunRYWTransactionNoRetryActor<Function>>(cx, func)
	{
		fdb_probe_actor_enter("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runRYWTransactionNoRetry");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runRYWTransactionNoRetry", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunRYWTransactionNoRetryActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunRYWTransactionNoRetryActor<Function>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
template <class Function>
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> runRYWTransactionNoRetry( Database const& cx, Function const& func ) {
															#line 73 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(new RunRYWTransactionNoRetryActor<Function>(cx, func));
															#line 1176 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.g.h"
}

#line 82 "/usr/src/libfdb_c/fdbclient/RunTransaction.actor.h"

#include "flow/unactorcompiler.h"
#endif
