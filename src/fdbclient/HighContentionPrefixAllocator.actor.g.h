#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
/*
 * HighContentionPrefixAllocator.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_HIGHCONTENTIONPREFIXALLOCATOR_ACTOR_G_H)
#define FDBCLIENT_HIGHCONTENTIONPREFIXALLOCATOR_ACTOR_G_H
#include "fdbclient/HighContentionPrefixAllocator.actor.g.h"
#elif !defined(FDBCLIENT_HIGHCONTENTIONPREFIXALLOCATOR_ACTOR_H)
#define FDBCLIENT_HIGHCONTENTIONPREFIXALLOCATOR_ACTOR_H

#include "fdbclient/ClientBooleanParams.h"
#include "fdbclient/CommitTransaction.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/Subspace.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

class HighContentionPrefixAllocator {
public:
	HighContentionPrefixAllocator(Subspace subspace) : counters(subspace.get(0)), recent(subspace.get(1)) {}

	template <class TransactionT>
	Future<Standalone<StringRef>> allocate(Reference<TransactionT> tr) {
		return allocate(this, tr);
	}

	static int64_t windowSize(int64_t start) {
		if (start < 255) {
			return 64;
		}
		if (start < 65535) {
			return 1024;
		}

		return 8192;
	}

private:
	Subspace counters;
	Subspace recent;

																#line 64 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
// This generated class is to be used only via allocate()
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
template <class TransactionT, class AllocateActor>
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
class AllocateActorState {
															#line 70 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
public:
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	AllocateActorState(HighContentionPrefixAllocator* const& self,Reference<TransactionT> const& tr) 
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		 : self(self),
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		   tr(tr),
															#line 64 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		   start(0),
															#line 65 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		   window(0)
															#line 83 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
	{
		fdb_probe_actor_create("allocate", reinterpret_cast<unsigned long>(this));

	}
	~AllocateActorState() 
	{
		fdb_probe_actor_destroy("allocate", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 67 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			;
															#line 98 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
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
		this->~AllocateActorState();
		static_cast<AllocateActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 68 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		rangeFuture = tr->getRange(self->counters.range(), 1, Snapshot::True, Reverse::True);
															#line 70 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(rangeFuture);
															#line 70 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (static_cast<AllocateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 132 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AllocateActor*>(this)->actor_wait_state = 1;
															#line 70 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AllocateActor, 0, RangeResult >*>(static_cast<AllocateActor*>(this)));
															#line 137 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult const& range,int loopDepth) 
	{
															#line 72 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (range.size() > 0)
															#line 146 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 73 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			start = self->counters.unpack(range[0].key).getInt(0);
															#line 150 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 76 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		windowAdvanced = false;
															#line 77 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		;
															#line 156 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && range,int loopDepth) 
	{
															#line 72 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (range.size() > 0)
															#line 165 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 73 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			start = self->counters.unpack(range[0].key).getInt(0);
															#line 169 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 76 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		windowAdvanced = false;
															#line 77 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		;
															#line 175 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& range,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(range, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && range,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(range), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AllocateActor*>(this)->actor_wait_state > 0) static_cast<AllocateActor*>(this)->actor_wait_state = 0;
		static_cast<AllocateActor*>(this)->ActorCallback< AllocateActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< AllocateActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AllocateActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AllocateActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 111 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		;
															#line 247 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

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
															#line 79 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (windowAdvanced)
															#line 263 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 80 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			tr->clear(KeyRangeRef(self->counters.key(), self->counters.get(start).key()));
															#line 81 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			tr->setOption(FDBTransactionOptions::NEXT_WRITE_NO_WRITE_CONFLICT_RANGE);
															#line 82 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			tr->clear(KeyRangeRef(self->recent.key(), self->recent.get(start).key()));
															#line 271 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 85 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		int64_t inc = 1;
															#line 86 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		tr->atomicOp(self->counters.get(start).key(), StringRef((uint8_t*)&inc, 8), MutationRef::AddValue);
															#line 88 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		countFuture = tr->get(self->counters.get(start).key(), Snapshot::True);
															#line 92 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		StrictFuture<Optional<Value>> __when_expr_1 = safeThreadFutureToFuture(countFuture);
															#line 92 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (static_cast<AllocateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 283 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AllocateActor*>(this)->actor_wait_state = 2;
															#line 92 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AllocateActor, 1, Optional<Value> >*>(static_cast<AllocateActor*>(this)));
															#line 288 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
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
	int a_body1loopBody1cont1loopBody1cont1(Optional<Value> const& countValue,int loopDepth) 
	{
															#line 94 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		int64_t count = 0;
															#line 95 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (countValue.present())
															#line 312 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 96 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			if (countValue.get().size() != 8)
															#line 316 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
			{
															#line 97 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
				return a_body1Catch1(invalid_directory_layer_metadata(), std::max(0, loopDepth - 2));
															#line 320 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
			}
															#line 99 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			count = *(int64_t*)countValue.get().begin();
															#line 324 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 102 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		window = HighContentionPrefixAllocator::windowSize(start);
															#line 103 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (count * 2 < window)
															#line 330 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 107 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		start += window;
															#line 108 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		windowAdvanced = true;
															#line 338 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(Optional<Value> && countValue,int loopDepth) 
	{
															#line 94 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		int64_t count = 0;
															#line 95 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (countValue.present())
															#line 349 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 96 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			if (countValue.get().size() != 8)
															#line 353 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
			{
															#line 97 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
				return a_body1Catch1(invalid_directory_layer_metadata(), std::max(0, loopDepth - 2));
															#line 357 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
			}
															#line 99 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			count = *(int64_t*)countValue.get().begin();
															#line 361 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 102 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		window = HighContentionPrefixAllocator::windowSize(start);
															#line 103 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (count * 2 < window)
															#line 367 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 107 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		start += window;
															#line 108 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		windowAdvanced = true;
															#line 375 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Optional<Value> const& countValue,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(countValue, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Optional<Value> && countValue,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(std::move(countValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<AllocateActor*>(this)->actor_wait_state > 0) static_cast<AllocateActor*>(this)->actor_wait_state = 0;
		static_cast<AllocateActor*>(this)->ActorCallback< AllocateActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< AllocateActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AllocateActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AllocateActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1(int loopDepth) 
	{
															#line 112 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		candidate = deterministicRandom()->randomInt(start, start + window);
															#line 115 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		latestCounterFuture = tr->getRange(self->counters.range(), 1, Snapshot::True, Reverse::True);
															#line 117 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		candidateValueFuture = tr->get(self->recent.get(candidate).key());
															#line 119 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		tr->setOption(FDBTransactionOptions::NEXT_WRITE_NO_WRITE_CONFLICT_RANGE);
															#line 120 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		tr->set(self->recent.get(candidate).key(), ValueRef());
															#line 123 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		StrictFuture<Void> __when_expr_2 = success(safeThreadFutureToFuture(latestCounterFuture)) && success(safeThreadFutureToFuture(candidateValueFuture));
															#line 123 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (static_cast<AllocateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 472 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<AllocateActor*>(this)->actor_wait_state = 3;
															#line 123 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AllocateActor, 2, Void >*>(static_cast<AllocateActor*>(this)));
															#line 477 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont4(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 126 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		int64_t currentWindowStart = 0;
															#line 127 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (latestCounterFuture.get().size() > 0)
															#line 501 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 128 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			currentWindowStart = self->counters.unpack(latestCounterFuture.get()[0].key).getInt(0);
															#line 505 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 131 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (currentWindowStart > start)
															#line 509 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 135 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (!candidateValueFuture.get().present())
															#line 515 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 136 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			tr->addWriteConflictRange(singleKeyRange(self->recent.get(candidate).key()));
															#line 137 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			if (!static_cast<AllocateActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Tuple().append(candidate).pack()); this->~AllocateActorState(); static_cast<AllocateActor*>(this)->destroy(); return 0; }
															#line 521 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
			new (&static_cast<AllocateActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Tuple().append(candidate).pack());
			this->~AllocateActorState();
			static_cast<AllocateActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 126 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		int64_t currentWindowStart = 0;
															#line 127 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (latestCounterFuture.get().size() > 0)
															#line 537 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 128 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			currentWindowStart = self->counters.unpack(latestCounterFuture.get()[0].key).getInt(0);
															#line 541 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		}
															#line 131 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (currentWindowStart > start)
															#line 545 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 135 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
		if (!candidateValueFuture.get().present())
															#line 551 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		{
															#line 136 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			tr->addWriteConflictRange(singleKeyRange(self->recent.get(candidate).key()));
															#line 137 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
			if (!static_cast<AllocateActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Tuple().append(candidate).pack()); this->~AllocateActorState(); static_cast<AllocateActor*>(this)->destroy(); return 0; }
															#line 557 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
			new (&static_cast<AllocateActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Tuple().append(candidate).pack());
			this->~AllocateActorState();
			static_cast<AllocateActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<AllocateActor*>(this)->actor_wait_state > 0) static_cast<AllocateActor*>(this)->actor_wait_state = 0;
		static_cast<AllocateActor*>(this)->ActorCallback< AllocateActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AllocateActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AllocateActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< AllocateActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	HighContentionPrefixAllocator* self;
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	Reference<TransactionT> tr;
															#line 64 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	int64_t start;
															#line 65 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	int64_t window;
															#line 68 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	typename TransactionT::template FutureT<RangeResult> rangeFuture;
															#line 76 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	bool windowAdvanced;
															#line 88 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	typename TransactionT::template FutureT<Optional<Value>> countFuture;
															#line 112 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	int64_t candidate;
															#line 115 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	typename TransactionT::template FutureT<RangeResult> latestCounterFuture;
															#line 117 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	typename TransactionT::template FutureT<Optional<Value>> candidateValueFuture;
															#line 650 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
};
// This generated class is to be used only via allocate()
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
template <class TransactionT>
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
class AllocateActor final : public Actor<Standalone<StringRef>>, public ActorCallback< AllocateActor<TransactionT>, 0, RangeResult >, public ActorCallback< AllocateActor<TransactionT>, 1, Optional<Value> >, public ActorCallback< AllocateActor<TransactionT>, 2, Void >, public FastAllocated<AllocateActor<TransactionT>>, public AllocateActorState<TransactionT, AllocateActor<TransactionT>> {
															#line 657 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
public:
	using FastAllocated<AllocateActor<TransactionT>>::operator new;
	using FastAllocated<AllocateActor<TransactionT>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<StringRef>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AllocateActor<TransactionT>, 0, RangeResult >;
friend struct ActorCallback< AllocateActor<TransactionT>, 1, Optional<Value> >;
friend struct ActorCallback< AllocateActor<TransactionT>, 2, Void >;
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	AllocateActor(HighContentionPrefixAllocator* const& self,Reference<TransactionT> const& tr) 
															#line 670 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
		 : Actor<Standalone<StringRef>>(),
		   AllocateActorState<TransactionT, AllocateActor<TransactionT>>(self, tr)
	{
		fdb_probe_actor_enter("allocate", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("allocate");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("allocate", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AllocateActor<TransactionT>, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AllocateActor<TransactionT>, 1, Optional<Value> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AllocateActor<TransactionT>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
template <class TransactionT>
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
[[nodiscard]] Future<Standalone<StringRef>> allocate( HighContentionPrefixAllocator* const& self, Reference<TransactionT> const& tr ) {
															#line 62 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
	return Future<Standalone<StringRef>>(new AllocateActor<TransactionT>(self, tr));
															#line 701 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.g.h"
}

#line 142 "/usr/src/libfdb_c/fdbclient/HighContentionPrefixAllocator.actor.h"
};

#include "flow/unactorcompiler.h"
#endif