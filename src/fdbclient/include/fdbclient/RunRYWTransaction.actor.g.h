#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
/*
 * RunRYWTransaction.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_RUNRYWTRANSACTION_ACTOR_G_H)
#define FDBCLIENT_RUNRYWTRANSACTION_ACTOR_G_H
#include "fdbclient/RunRYWTransaction.actor.g.h"
#elif !defined(FDBCLIENT_RUNRYWTRANSACTION_ACTOR_H)
#define FDBCLIENT_RUNRYWTRANSACTION_ACTOR_H

#include <utility>

#include "flow/flow.h"
#include "fdbclient/RunTransaction.actor.h"
#include "fdbclient/ReadYourWrites.h"
#include "flow/actorcompiler.h" // This must be the last #include.

// Runs a RYW transaction in a retry loop on the given Database.
//
// Takes a function func that accepts a Reference<ReadYourWritesTransaction> as a parameter and returns a non-Void
// Future. This function is run inside the transaction, and when the transaction is successfully committed the result of
// the function is returned.
//
// The supplied function should be idempotent. Otherwise, outcome of this function will depend on how many times the
// transaction is retried.
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransaction()
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function, class RunRYWTransactionActor>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionActorState {
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
public:
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionActorState(Database const& cx,Function const& func) 
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		 : cx(cx),
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   func(func),
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> __when_expr_0 = func(tr);
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 1;
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*>(static_cast<RunRYWTransactionActor*>(this)));
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 3;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionActor, 2, Void >*>(static_cast<RunRYWTransactionActor*>(this)));
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (static_cast<RunRYWTransactionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionActor*>(this)->actor_wait_state = 2;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionActor, 1, Void >*>(static_cast<RunRYWTransactionActor*>(this)));
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& __result,int loopDepth) 
	{
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		result = __result;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionActorState(); static_cast<RunRYWTransactionActor*>(this)->destroy(); return 0; }
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionActorState();
		static_cast<RunRYWTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionActorState(); static_cast<RunRYWTransactionActor*>(this)->destroy(); return 0; }
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Database cx;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Function func;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) result;
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransaction()
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionActor final : public Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>, public ActorCallback< RunRYWTransactionActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >, public ActorCallback< RunRYWTransactionActor<Function>, 1, Void >, public ActorCallback< RunRYWTransactionActor<Function>, 2, Void >, public FastAllocated<RunRYWTransactionActor<Function>>, public RunRYWTransactionActorState<Function, RunRYWTransactionActor<Function>> {
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionActor(Database const& cx,Function const& func) 
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> runRYWTransaction( Database const& cx, Function const& func ) {
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(new RunRYWTransactionActor<Function>(cx, func));
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
}

#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"

// Runs a RYW transaction in a retry loop on the given Database.
//
// Takes a function func that accepts a Reference<ReadYourWritesTransaction> as a parameter and returns a Void
// Future. This function is run inside the transaction.
//
// The supplied function should be idempotent. Otherwise, outcome of this function will depend on how many times the
// transaction is retried.
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransactionVoid()
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function, class RunRYWTransactionVoidActor>
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionVoidActorState {
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
public:
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionVoidActorState(Database const& cx,Function const& func) 
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		 : cx(cx),
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   func(func),
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
	{
		fdb_probe_actor_create("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this));

	}
	~RunRYWTransactionVoidActorState() 
	{
		fdb_probe_actor_destroy("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			;
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
		this->~RunRYWTransactionVoidActorState();
		static_cast<RunRYWTransactionVoidActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<Void> __when_expr_0 = func(tr);
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 1;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionVoidActor, 0, Void >*>(static_cast<RunRYWTransactionVoidActor*>(this)));
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 3;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionVoidActor, 2, Void >*>(static_cast<RunRYWTransactionVoidActor*>(this)));
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 2;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionVoidActor, 1, Void >*>(static_cast<RunRYWTransactionVoidActor*>(this)));
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 2;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionVoidActor, 1, Void >*>(static_cast<RunRYWTransactionVoidActor*>(this)));
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionVoidActor*>(this)->ActorCallback< RunRYWTransactionVoidActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionVoidActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionVoidActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionVoidActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionVoidActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RunRYWTransactionVoidActorState(); static_cast<RunRYWTransactionVoidActor*>(this)->destroy(); return 0; }
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionVoidActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RunRYWTransactionVoidActorState();
		static_cast<RunRYWTransactionVoidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionVoidActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RunRYWTransactionVoidActorState(); static_cast<RunRYWTransactionVoidActor*>(this)->destroy(); return 0; }
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionVoidActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RunRYWTransactionVoidActorState();
		static_cast<RunRYWTransactionVoidActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionVoidActor*>(this)->ActorCallback< RunRYWTransactionVoidActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionVoidActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionVoidActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionVoidActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state > 0) static_cast<RunRYWTransactionVoidActor*>(this)->actor_wait_state = 0;
		static_cast<RunRYWTransactionVoidActor*>(this)->ActorCallback< RunRYWTransactionVoidActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionVoidActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RunRYWTransactionVoidActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RunRYWTransactionVoidActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Database cx;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Function func;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransactionVoid()
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionVoidActor final : public Actor<Void>, public ActorCallback< RunRYWTransactionVoidActor<Function>, 0, Void >, public ActorCallback< RunRYWTransactionVoidActor<Function>, 1, Void >, public ActorCallback< RunRYWTransactionVoidActor<Function>, 2, Void >, public FastAllocated<RunRYWTransactionVoidActor<Function>>, public RunRYWTransactionVoidActorState<Function, RunRYWTransactionVoidActor<Function>> {
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
public:
	using FastAllocated<RunRYWTransactionVoidActor<Function>>::operator new;
	using FastAllocated<RunRYWTransactionVoidActor<Function>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunRYWTransactionVoidActor<Function>, 0, Void >;
friend struct ActorCallback< RunRYWTransactionVoidActor<Function>, 1, Void >;
friend struct ActorCallback< RunRYWTransactionVoidActor<Function>, 2, Void >;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionVoidActor(Database const& cx,Function const& func) 
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		 : Actor<Void>(),
		   RunRYWTransactionVoidActorState<Function, RunRYWTransactionVoidActor<Function>>(cx, func)
	{
		fdb_probe_actor_enter("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runRYWTransactionVoid");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runRYWTransactionVoid", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunRYWTransactionVoidActor<Function>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunRYWTransactionVoidActor<Function>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RunRYWTransactionVoidActor<Function>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
[[nodiscard]] Future<Void> runRYWTransactionVoid( Database const& cx, Function const& func ) {
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	return Future<Void>(new RunRYWTransactionVoidActor<Function>(cx, func));
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
}

#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"

															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransactionFailIfLocked()
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function, class RunRYWTransactionFailIfLockedActor>
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionFailIfLockedActorState {
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
public:
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionFailIfLockedActorState(Database const& cx,Function const& func) 
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		 : cx(cx),
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   func(func),
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			;
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> __when_expr_0 = func(tr);
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 1;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionFailIfLockedActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*>(static_cast<RunRYWTransactionFailIfLockedActor*>(this)));
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (e.code() == error_code_database_locked)
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			{
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 3;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionFailIfLockedActor, 2, Void >*>(static_cast<RunRYWTransactionFailIfLockedActor*>(this)));
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->actor_wait_state = 2;
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionFailIfLockedActor, 1, Void >*>(static_cast<RunRYWTransactionFailIfLockedActor*>(this)));
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& __result,int loopDepth) 
	{
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		result = __result;
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionFailIfLockedActorState(); static_cast<RunRYWTransactionFailIfLockedActor*>(this)->destroy(); return 0; }
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionFailIfLockedActorState();
		static_cast<RunRYWTransactionFailIfLockedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionFailIfLockedActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionFailIfLockedActorState(); static_cast<RunRYWTransactionFailIfLockedActor*>(this)->destroy(); return 0; }
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Database cx;
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Function func;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) result;
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransactionFailIfLocked()
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionFailIfLockedActor final : public Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>, public ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >, public ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 1, Void >, public ActorCallback< RunRYWTransactionFailIfLockedActor<Function>, 2, Void >, public FastAllocated<RunRYWTransactionFailIfLockedActor<Function>>, public RunRYWTransactionFailIfLockedActorState<Function, RunRYWTransactionFailIfLockedActor<Function>> {
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionFailIfLockedActor(Database const& cx,Function const& func) 
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> runRYWTransactionFailIfLocked( Database const& cx, Function const& func ) {
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(new RunRYWTransactionFailIfLockedActor<Function>(cx, func));
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
}

#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"

															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runRYWTransactionNoRetry()
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function, class RunRYWTransactionNoRetryActor>
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionNoRetryActorState {
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
public:
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionNoRetryActorState(Database const& cx,Function const& func) 
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		 : cx(cx),
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   func(func),
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		   tr(new ReadYourWritesTransaction(cx))
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> __when_expr_0 = func(tr);
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			if (static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state = 1;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionNoRetryActor, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >*>(static_cast<RunRYWTransactionNoRetryActor*>(this)));
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = tr->commit();
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunRYWTransactionNoRetryActor*>(this)->actor_wait_state = 2;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunRYWTransactionNoRetryActor, 1, Void >*>(static_cast<RunRYWTransactionNoRetryActor*>(this)));
															#line 1406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) const& __result,int loopDepth) 
	{
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		result = __result;
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionNoRetryActorState(); static_cast<RunRYWTransactionNoRetryActor*>(this)->destroy(); return 0; }
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
		new (&static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunRYWTransactionNoRetryActorState();
		static_cast<RunRYWTransactionNoRetryActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
		if (!static_cast<RunRYWTransactionNoRetryActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>::futures) { (void)(result); this->~RunRYWTransactionNoRetryActorState(); static_cast<RunRYWTransactionNoRetryActor*>(this)->destroy(); return 0; }
															#line 1494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Database cx;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Function func;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) result;
															#line 1573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
};
// This generated class is to be used only via runRYWTransactionNoRetry()
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
class RunRYWTransactionNoRetryActor final : public Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>, public ActorCallback< RunRYWTransactionNoRetryActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >, public ActorCallback< RunRYWTransactionNoRetryActor<Function>, 1, Void >, public FastAllocated<RunRYWTransactionNoRetryActor<Function>>, public RunRYWTransactionNoRetryActorState<Function, RunRYWTransactionNoRetryActor<Function>> {
															#line 1580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
public:
	using FastAllocated<RunRYWTransactionNoRetryActor<Function>>::operator new;
	using FastAllocated<RunRYWTransactionNoRetryActor<Function>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunRYWTransactionNoRetryActor<Function>, 0, decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue()) >;
friend struct ActorCallback< RunRYWTransactionNoRetryActor<Function>, 1, Void >;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	RunRYWTransactionNoRetryActor(Database const& cx,Function const& func) 
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
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
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
template <class Function>
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())> runRYWTransactionNoRetry( Database const& cx, Function const& func ) {
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<ReadYourWritesTransaction>()).getValue())>(new RunRYWTransactionNoRetryActor<Function>(cx, func));
															#line 1623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.g.h"
}

#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunRYWTransaction.actor.h"

#include "flow/unactorcompiler.h"
#endif
