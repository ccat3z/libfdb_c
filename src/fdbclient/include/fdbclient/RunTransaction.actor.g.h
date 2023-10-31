#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
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
#include "fdbclient/FDBOptions.g.h"
#include "flow/actorcompiler.h" // This must be the last #include.

template <typename, typename = void>
struct transaction_option_setter : std::false_type {};

template <typename T>
struct transaction_option_setter<Reference<T>> : transaction_option_setter<T> {};

template <typename T>
constexpr bool can_set_transaction_options = transaction_option_setter<T>::value;

															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runTransaction()
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
template <class Function, class DB, class RunTransactionActor>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
class RunTransactionActorState {
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
public:
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	RunTransactionActorState(Reference<DB> const& db,Function const& func) 
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		 : db(db),
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		   func(func),
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		   tr(db->createTransaction())
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
	{
		fdb_probe_actor_create("runTransaction", reinterpret_cast<unsigned long>(this));

	}
	~RunTransactionActorState() 
	{
		fdb_probe_actor_destroy("runTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
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
		this->~RunTransactionActorState();
		static_cast<RunTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if constexpr (can_set_transaction_options<DB>)
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		{
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			db->setOptions(tr);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		}
		try {
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			StrictFuture<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())> __when_expr_0 = func(tr);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunTransactionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunTransactionActor*>(this)->actor_wait_state = 1;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunTransactionActor, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >*>(static_cast<RunTransactionActor*>(this)));
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
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
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunTransactionActor*>(this)->actor_wait_state = 3;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunTransactionActor, 2, Void >*>(static_cast<RunTransactionActor*>(this)));
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (static_cast<RunTransactionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunTransactionActor*>(this)->actor_wait_state = 2;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunTransactionActor, 1, Void >*>(static_cast<RunTransactionActor*>(this)));
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) const& __result,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		result = __result;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) && __result,int loopDepth) 
	{
		result = std::move(__result);
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunTransactionActor*>(this)->actor_wait_state > 0) static_cast<RunTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RunTransactionActor*>(this)->ActorCallback< RunTransactionActor, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >::remove();

	}
	void a_callback_fire(ActorCallback< RunTransactionActor, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >*,decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) const& value) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunTransactionActor, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >*,decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) && value) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunTransactionActor, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >*,Error err) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunTransactionActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())>::futures) { (void)(result); this->~RunTransactionActorState(); static_cast<RunTransactionActor*>(this)->destroy(); return 0; }
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunTransactionActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunTransactionActorState();
		static_cast<RunTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunTransactionActor*>(this)->SAV<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())>::futures) { (void)(result); this->~RunTransactionActorState(); static_cast<RunTransactionActor*>(this)->destroy(); return 0; }
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunTransactionActor*>(this)->SAV< decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >::value()) decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())(std::move(result)); // state_var_RVO
		this->~RunTransactionActorState();
		static_cast<RunTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RunTransactionActor*>(this)->actor_wait_state > 0) static_cast<RunTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RunTransactionActor*>(this)->ActorCallback< RunTransactionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunTransactionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunTransactionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunTransactionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<RunTransactionActor*>(this)->actor_wait_state > 0) static_cast<RunTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RunTransactionActor*>(this)->ActorCallback< RunTransactionActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunTransactionActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RunTransactionActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RunTransactionActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	Reference<DB> db;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	Function func;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) result;
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
};
// This generated class is to be used only via runTransaction()
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
template <class Function, class DB>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
class RunTransactionActor final : public Actor<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())>, public ActorCallback< RunTransactionActor<Function, DB>, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >, public ActorCallback< RunTransactionActor<Function, DB>, 1, Void >, public ActorCallback< RunTransactionActor<Function, DB>, 2, Void >, public FastAllocated<RunTransactionActor<Function, DB>>, public RunTransactionActorState<Function, DB, RunTransactionActor<Function, DB>> {
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
public:
	using FastAllocated<RunTransactionActor<Function, DB>>::operator new;
	using FastAllocated<RunTransactionActor<Function, DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunTransactionActor<Function, DB>, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >;
friend struct ActorCallback< RunTransactionActor<Function, DB>, 1, Void >;
friend struct ActorCallback< RunTransactionActor<Function, DB>, 2, Void >;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	RunTransactionActor(Reference<DB> const& db,Function const& func) 
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		 : Actor<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())>(),
		   RunTransactionActorState<Function, DB, RunTransactionActor<Function, DB>>(db, func)
	{
		fdb_probe_actor_enter("runTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunTransactionActor<Function, DB>, 0, decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue()) >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunTransactionActor<Function, DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RunTransactionActor<Function, DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
template <class Function, class DB>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
[[nodiscard]] Future<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())> runTransaction( Reference<DB> const& db, Function const& func ) {
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	return Future<decltype(std::declval<Function>()(Reference<typename DB::TransactionT>()).getValue())>(new RunTransactionActor<Function, DB>(db, func));
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
}

#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"

															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
namespace {
// This generated class is to be used only via runTransactionVoid()
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
template <class Function, class DB, class RunTransactionVoidActor>
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
class RunTransactionVoidActorState {
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
public:
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	RunTransactionVoidActorState(Reference<DB> const& db,Function const& func) 
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		 : db(db),
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		   func(func),
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		   tr(db->createTransaction())
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
	{
		fdb_probe_actor_create("runTransactionVoid", reinterpret_cast<unsigned long>(this));

	}
	~RunTransactionVoidActorState() 
	{
		fdb_probe_actor_destroy("runTransactionVoid", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
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
		this->~RunTransactionVoidActorState();
		static_cast<RunTransactionVoidActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if constexpr (can_set_transaction_options<DB>)
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		{
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			db->setOptions(tr);
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		}
		try {
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			StrictFuture<Void> __when_expr_0 = func(tr);
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 1;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunTransactionVoidActor, 0, Void >*>(static_cast<RunTransactionVoidActor*>(this)));
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
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
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 3;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RunTransactionVoidActor, 2, Void >*>(static_cast<RunTransactionVoidActor*>(this)));
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 2;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunTransactionVoidActor, 1, Void >*>(static_cast<RunTransactionVoidActor*>(this)));
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 2;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RunTransactionVoidActor, 1, Void >*>(static_cast<RunTransactionVoidActor*>(this)));
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state > 0) static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 0;
		static_cast<RunTransactionVoidActor*>(this)->ActorCallback< RunTransactionVoidActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunTransactionVoidActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunTransactionVoidActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunTransactionVoidActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunTransactionVoidActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RunTransactionVoidActorState(); static_cast<RunTransactionVoidActor*>(this)->destroy(); return 0; }
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunTransactionVoidActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RunTransactionVoidActorState();
		static_cast<RunTransactionVoidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
		if (!static_cast<RunTransactionVoidActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RunTransactionVoidActorState(); static_cast<RunTransactionVoidActor*>(this)->destroy(); return 0; }
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		new (&static_cast<RunTransactionVoidActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RunTransactionVoidActorState();
		static_cast<RunTransactionVoidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state > 0) static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 0;
		static_cast<RunTransactionVoidActor*>(this)->ActorCallback< RunTransactionVoidActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunTransactionVoidActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RunTransactionVoidActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RunTransactionVoidActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<RunTransactionVoidActor*>(this)->actor_wait_state > 0) static_cast<RunTransactionVoidActor*>(this)->actor_wait_state = 0;
		static_cast<RunTransactionVoidActor*>(this)->ActorCallback< RunTransactionVoidActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunTransactionVoidActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RunTransactionVoidActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RunTransactionVoidActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	Reference<DB> db;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	Function func;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
};
// This generated class is to be used only via runTransactionVoid()
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
template <class Function, class DB>
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
class RunTransactionVoidActor final : public Actor<Void>, public ActorCallback< RunTransactionVoidActor<Function, DB>, 0, Void >, public ActorCallback< RunTransactionVoidActor<Function, DB>, 1, Void >, public ActorCallback< RunTransactionVoidActor<Function, DB>, 2, Void >, public FastAllocated<RunTransactionVoidActor<Function, DB>>, public RunTransactionVoidActorState<Function, DB, RunTransactionVoidActor<Function, DB>> {
															#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
public:
	using FastAllocated<RunTransactionVoidActor<Function, DB>>::operator new;
	using FastAllocated<RunTransactionVoidActor<Function, DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunTransactionVoidActor<Function, DB>, 0, Void >;
friend struct ActorCallback< RunTransactionVoidActor<Function, DB>, 1, Void >;
friend struct ActorCallback< RunTransactionVoidActor<Function, DB>, 2, Void >;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	RunTransactionVoidActor(Reference<DB> const& db,Function const& func) 
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
		 : Actor<Void>(),
		   RunTransactionVoidActorState<Function, DB, RunTransactionVoidActor<Function, DB>>(db, func)
	{
		fdb_probe_actor_enter("runTransactionVoid", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runTransactionVoid");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runTransactionVoid", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunTransactionVoidActor<Function, DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RunTransactionVoidActor<Function, DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RunTransactionVoidActor<Function, DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
template <class Function, class DB>
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
[[nodiscard]] Future<Void> runTransactionVoid( Reference<DB> const& db, Function const& func ) {
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"
	return Future<Void>(new RunTransactionVoidActor<Function, DB>(db, func));
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.g.h"
}

#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/RunTransaction.actor.h"

// SystemTransactionGenerator is a Database-like wrapper which produces transactions which have selected
// options set for lock awareness, reading and optionally writing the system keys, and immediate priority.
// All options are false by default.
template <typename DB>
struct SystemTransactionGenerator : ReferenceCounted<SystemTransactionGenerator<DB>> {
	typedef typename DB::TransactionT TransactionT;

	SystemTransactionGenerator(Reference<DB> db, bool write, bool lockAware, bool immediate)
	  : db(db), write(write), lockAware(lockAware), immediate(immediate) {}

	Reference<TransactionT> createTransaction() const { return db->createTransaction(); }

	void setOptions(Reference<TransactionT> tr) const {
		if (write) {
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
		} else {
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
		}

		if (immediate) {
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
		}

		if (lockAware) {
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
		}
	}

	Reference<DB> db;
	bool write;
	bool lockAware;
	bool immediate;
};

template <typename DB>
struct transaction_option_setter<SystemTransactionGenerator<DB>> : std::true_type {};

// Convenient wrapper for creating SystemTransactionGenerators.
template <typename DB>
auto SystemDB(Reference<DB> db, bool write = false, bool lockAware = false, bool immediate = false) {
	return makeReference<SystemTransactionGenerator<DB>>(db, write, lockAware, immediate);
}

// SystemDB with all options true
template <typename DB>
auto SystemDBWriteLockedNow(Reference<DB> db) {
	return makeReference<SystemTransactionGenerator<DB>>(db, true, true, true);
}

#include "flow/unactorcompiler.h"
#endif
