#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
/*
 * StorageWiggleMetrics.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_STORAGEWIGGLEMETRICS_ACTOR_G_H)
#define FDBCLIENT_STORAGEWIGGLEMETRICS_ACTOR_G_H
#include "fdbclient/StorageWiggleMetrics.actor.g.h"
#elif !defined(FDBCLIENT_STORAGEWIGGLEMETRICS_ACTOR_H)
#define FDBCLIENT_STORAGEWIGGLEMETRICS_ACTOR_H

#include "fdbrpc/Smoother.h"
#include "flow/ObjectSerializer.h"
#include "flow/serialize.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/KeyBackedTypes.actor.h"
#include "fdbclient/RunTransaction.actor.h"
#include "flow/actorcompiler.h"

FDB_BOOLEAN_PARAM(PrimaryRegion);

struct StorageWiggleMetrics {
	constexpr static FileIdentifier file_identifier = 4728961;

	// round statistics
	// One StorageServer wiggle round is considered 'complete', when all StorageServers with creationTime < T are
	// wiggled
	// Start and finish are in epoch seconds
	double last_round_start = 0;
	double last_round_finish = 0;
	TimerSmoother smoothed_round_duration;
	int finished_round = 0; // finished round since storage wiggle is open

	// step statistics
	// 1 wiggle step as 1 storage server is wiggled in the current round
	// Start and finish are in epoch seconds
	double last_wiggle_start = 0;
	double last_wiggle_finish = 0;
	TimerSmoother smoothed_wiggle_duration;
	int finished_wiggle = 0; // finished step since storage wiggle is open

	StorageWiggleMetrics() : smoothed_round_duration(20.0 * 60), smoothed_wiggle_duration(10.0 * 60) {}

	template <class Ar>
	void serialize(Ar& ar) {
		double step_total, round_total;
		if (!ar.isDeserializing) {
			step_total = smoothed_wiggle_duration.getTotal();
			round_total = smoothed_round_duration.getTotal();
		}
		serializer(ar,
		           last_wiggle_start,
		           last_wiggle_finish,
		           step_total,
		           finished_wiggle,
		           last_round_start,
		           last_round_finish,
		           round_total,
		           finished_round);
		if (ar.isDeserializing) {
			smoothed_round_duration.reset(round_total);
			smoothed_wiggle_duration.reset(step_total);
		}
	}

	StatusObject toJSON() const {
		StatusObject result;
		result["last_round_start_datetime"] = epochsToGMTString(last_round_start);
		result["last_round_finish_datetime"] = epochsToGMTString(last_round_finish);
		result["last_round_start_timestamp"] = last_round_start;
		result["last_round_finish_timestamp"] = last_round_finish;
		result["smoothed_round_seconds"] = smoothed_round_duration.smoothTotal();
		result["finished_round"] = finished_round;

		result["last_wiggle_start_datetime"] = epochsToGMTString(last_wiggle_start);
		result["last_wiggle_finish_datetime"] = epochsToGMTString(last_wiggle_finish);
		result["last_wiggle_start_timestamp"] = last_wiggle_start;
		result["last_wiggle_finish_timestamp"] = last_wiggle_finish;
		result["smoothed_wiggle_seconds"] = smoothed_wiggle_duration.smoothTotal();
		result["finished_wiggle"] = finished_wiggle;
		return result;
	}

	void reset() {
		StorageWiggleMetrics newMetrics;
		newMetrics.smoothed_round_duration.reset(smoothed_round_duration.getTotal());
		newMetrics.smoothed_wiggle_duration.reset(smoothed_wiggle_duration.getTotal());
		*this = std::move(newMetrics);
	}
};

struct StorageWiggleDelay {
	constexpr static FileIdentifier file_identifier = 102937;
	double delaySeconds = 0;
	explicit StorageWiggleDelay(double sec = 0) : delaySeconds(sec) {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, delaySeconds);
	}
};

namespace {
// Persistent the total delay time to the database, and return accumulated delay time.
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
// This generated class is to be used only via addPerpetualWiggleDelay_impl()
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <class TrType, class AddPerpetualWiggleDelay_implActor>
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
class AddPerpetualWiggleDelay_implActorState {
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
public:
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	AddPerpetualWiggleDelay_implActorState(TrType const& tr,KeyBackedObjectProperty<StorageWiggleDelay, decltype(IncludeVersion())> const& delayProperty,double const& secDelta) 
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		 : tr(tr),
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   delayProperty(delayProperty),
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   secDelta(secDelta)
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
	{
		fdb_probe_actor_create("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this));

	}
	~AddPerpetualWiggleDelay_implActorState() 
	{
		fdb_probe_actor_destroy("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			StrictFuture<StorageWiggleDelay> __when_expr_0 = delayProperty.getD(tr, Snapshot::False);
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			if (static_cast<AddPerpetualWiggleDelay_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AddPerpetualWiggleDelay_implActor*>(this)->actor_wait_state = 1;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddPerpetualWiggleDelay_implActor, 0, StorageWiggleDelay >*>(static_cast<AddPerpetualWiggleDelay_implActor*>(this)));
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
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
		this->~AddPerpetualWiggleDelay_implActorState();
		static_cast<AddPerpetualWiggleDelay_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		delayObj.delaySeconds += secDelta;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		delayProperty.set(tr, delayObj);
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (!static_cast<AddPerpetualWiggleDelay_implActor*>(this)->SAV<double>::futures) { (void)(delayObj.delaySeconds); this->~AddPerpetualWiggleDelay_implActorState(); static_cast<AddPerpetualWiggleDelay_implActor*>(this)->destroy(); return 0; }
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		new (&static_cast<AddPerpetualWiggleDelay_implActor*>(this)->SAV< double >::value()) double(delayObj.delaySeconds);
		this->~AddPerpetualWiggleDelay_implActorState();
		static_cast<AddPerpetualWiggleDelay_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(StorageWiggleDelay const& __delayObj,int loopDepth) 
	{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		delayObj = __delayObj;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(StorageWiggleDelay && __delayObj,int loopDepth) 
	{
		delayObj = std::move(__delayObj);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AddPerpetualWiggleDelay_implActor*>(this)->actor_wait_state > 0) static_cast<AddPerpetualWiggleDelay_implActor*>(this)->actor_wait_state = 0;
		static_cast<AddPerpetualWiggleDelay_implActor*>(this)->ActorCallback< AddPerpetualWiggleDelay_implActor, 0, StorageWiggleDelay >::remove();

	}
	void a_callback_fire(ActorCallback< AddPerpetualWiggleDelay_implActor, 0, StorageWiggleDelay >*,StorageWiggleDelay const& value) 
	{
		fdb_probe_actor_enter("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AddPerpetualWiggleDelay_implActor, 0, StorageWiggleDelay >*,StorageWiggleDelay && value) 
	{
		fdb_probe_actor_enter("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AddPerpetualWiggleDelay_implActor, 0, StorageWiggleDelay >*,Error err) 
	{
		fdb_probe_actor_enter("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	TrType tr;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	KeyBackedObjectProperty<StorageWiggleDelay, decltype(IncludeVersion())> delayProperty;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	double secDelta;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	StorageWiggleDelay delayObj;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
};
// This generated class is to be used only via addPerpetualWiggleDelay_impl()
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <class TrType>
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
class AddPerpetualWiggleDelay_implActor final : public Actor<double>, public ActorCallback< AddPerpetualWiggleDelay_implActor<TrType>, 0, StorageWiggleDelay >, public FastAllocated<AddPerpetualWiggleDelay_implActor<TrType>>, public AddPerpetualWiggleDelay_implActorState<TrType, AddPerpetualWiggleDelay_implActor<TrType>> {
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
public:
	using FastAllocated<AddPerpetualWiggleDelay_implActor<TrType>>::operator new;
	using FastAllocated<AddPerpetualWiggleDelay_implActor<TrType>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<double>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddPerpetualWiggleDelay_implActor<TrType>, 0, StorageWiggleDelay >;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	AddPerpetualWiggleDelay_implActor(TrType const& tr,KeyBackedObjectProperty<StorageWiggleDelay, decltype(IncludeVersion())> const& delayProperty,double const& secDelta) 
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		 : Actor<double>(),
		   AddPerpetualWiggleDelay_implActorState<TrType, AddPerpetualWiggleDelay_implActor<TrType>>(tr, delayProperty, secDelta)
	{
		fdb_probe_actor_enter("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("addPerpetualWiggleDelay_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("addPerpetualWiggleDelay_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AddPerpetualWiggleDelay_implActor<TrType>, 0, StorageWiggleDelay >*)0, actor_cancelled()); break;
		}

	}
};
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <class TrType>
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
[[nodiscard]] Future<double> addPerpetualWiggleDelay_impl( TrType const& tr, KeyBackedObjectProperty<StorageWiggleDelay, decltype(IncludeVersion())> const& delayProperty, double const& secDelta ) {
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	return Future<double>(new AddPerpetualWiggleDelay_implActor<TrType>(tr, delayProperty, secDelta));
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
}

#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"

// set all fields except for smoothed durations to default values. If the metrics is not given, load from system key
// space
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
// This generated class is to be used only via resetStorageWiggleMetrics_impl()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <class TrType, class ResetStorageWiggleMetrics_implActor>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
class ResetStorageWiggleMetrics_implActorState {
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
public:
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	ResetStorageWiggleMetrics_implActorState(TrType const& tr,KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> const& metricsProperty,Optional<StorageWiggleMetrics> const& metrics) 
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		 : tr(tr),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   metricsProperty(metricsProperty),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   metrics(metrics)
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
	{
		fdb_probe_actor_create("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this));

	}
	~ResetStorageWiggleMetrics_implActorState() 
	{
		fdb_probe_actor_destroy("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			if (!metrics.present())
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
			{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
				StrictFuture<Void> __when_expr_0 = store(metrics, metricsProperty.get(tr));
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
				if (static_cast<ResetStorageWiggleMetrics_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ResetStorageWiggleMetrics_implActor*>(this)->actor_wait_state = 1;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResetStorageWiggleMetrics_implActor, 0, Void >*>(static_cast<ResetStorageWiggleMetrics_implActor*>(this)));
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont1(loopDepth);
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
		this->~ResetStorageWiggleMetrics_implActorState();
		static_cast<ResetStorageWiggleMetrics_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (metrics.present())
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			metrics.get().reset();
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			metricsProperty.set(tr, metrics.get());
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		}
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (!static_cast<ResetStorageWiggleMetrics_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ResetStorageWiggleMetrics_implActorState(); static_cast<ResetStorageWiggleMetrics_implActor*>(this)->destroy(); return 0; }
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		new (&static_cast<ResetStorageWiggleMetrics_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ResetStorageWiggleMetrics_implActorState();
		static_cast<ResetStorageWiggleMetrics_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

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
		if (static_cast<ResetStorageWiggleMetrics_implActor*>(this)->actor_wait_state > 0) static_cast<ResetStorageWiggleMetrics_implActor*>(this)->actor_wait_state = 0;
		static_cast<ResetStorageWiggleMetrics_implActor*>(this)->ActorCallback< ResetStorageWiggleMetrics_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ResetStorageWiggleMetrics_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResetStorageWiggleMetrics_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ResetStorageWiggleMetrics_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	TrType tr;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> metricsProperty;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	Optional<StorageWiggleMetrics> metrics;
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
};
// This generated class is to be used only via resetStorageWiggleMetrics_impl()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <class TrType>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
class ResetStorageWiggleMetrics_implActor final : public Actor<Void>, public ActorCallback< ResetStorageWiggleMetrics_implActor<TrType>, 0, Void >, public FastAllocated<ResetStorageWiggleMetrics_implActor<TrType>>, public ResetStorageWiggleMetrics_implActorState<TrType, ResetStorageWiggleMetrics_implActor<TrType>> {
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
public:
	using FastAllocated<ResetStorageWiggleMetrics_implActor<TrType>>::operator new;
	using FastAllocated<ResetStorageWiggleMetrics_implActor<TrType>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResetStorageWiggleMetrics_implActor<TrType>, 0, Void >;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	ResetStorageWiggleMetrics_implActor(TrType const& tr,KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> const& metricsProperty,Optional<StorageWiggleMetrics> const& metrics) 
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		 : Actor<Void>(),
		   ResetStorageWiggleMetrics_implActorState<TrType, ResetStorageWiggleMetrics_implActor<TrType>>(tr, metricsProperty, metrics)
	{
		fdb_probe_actor_enter("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resetStorageWiggleMetrics_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("resetStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ResetStorageWiggleMetrics_implActor<TrType>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <class TrType>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
[[nodiscard]] Future<Void> resetStorageWiggleMetrics_impl( TrType const& tr, KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> const& metricsProperty, Optional<StorageWiggleMetrics> const& metrics ) {
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	return Future<Void>(new ResetStorageWiggleMetrics_implActor<TrType>(tr, metricsProperty, metrics));
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
}

#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
} // namespace

// After 7.3, the perpetual wiggle related keys should use format "\xff/storageWiggle/[primary | remote]/[fieldName]"
class StorageWiggleData {
protected:
	Key prefix;

public:
	struct DataForDc : public KeyBackedClass {
		DataForDc(StringRef prefix) : KeyBackedClass(prefix) {}

		auto storageWiggleDelay() const {
			auto key = subspace.pack("storageWiggleDelay"_sr);
			return KeyBackedObjectProperty<StorageWiggleDelay, decltype(IncludeVersion())>(key, IncludeVersion());
		}
	};

	StorageWiggleData() : prefix(perpetualStorageWigglePrefix) {}

	auto perpetualWiggleSpeed() const { return KeyBackedProperty<Value, NullCodec>(perpetualStorageWiggleKey); }

	auto wigglingStorageServer(PrimaryRegion primaryDc) const {
		Key mapPrefix = perpetualStorageWiggleIDPrefix.withSuffix(primaryDc ? "primary/"_sr : "remote/"_sr);
		return KeyBackedObjectMap<UID, StorageWiggleValue, decltype(IncludeVersion())>(mapPrefix, IncludeVersion());
	}

	auto storageWiggleMetrics(PrimaryRegion primaryDc) const {
		Key key = perpetualStorageWiggleStatsPrefix.withSuffix(primaryDc ? "primary"_sr : "remote"_sr);
		return KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())>(key, IncludeVersion());
	}

	DataForDc forDc(PrimaryRegion primaryDc) const {
		return DataForDc(primaryDc ? prefix.withSuffix("primary/"_sr) : prefix.withSuffix("remote/"_sr));
	}

	// Persistent the total delay time to the database, and return accumulated delay time.
	template <class DB>
	Future<double> addPerpetualWiggleDelay(Reference<DB> db, PrimaryRegion primary, double secDelta) {
		return runTransaction(db, [=, self = *this](Reference<typename DB::TransactionT> tr) {
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);

			return addPerpetualWiggleDelay_impl(tr, self.forDc(primary).storageWiggleDelay(), secDelta);
		});
	}

	// clear the persistent total delay in database
	template <class DB>
	Future<Void> clearPerpetualWiggleDelay(Reference<DB> db, PrimaryRegion primary) {
		return runTransaction(db, [=, self = *this](Reference<typename DB::TransactionT> tr) {
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
			self.forDc(primary).storageWiggleDelay().clear(tr);
			return Future<Void>(Void());
		});
	}

	// set all fields except for smoothed durations to default values. If the metrics is not given, load from system key
	// space
	template <class TrType>
	Future<Void> resetStorageWiggleMetrics(TrType tr,
	                                       PrimaryRegion primary,
	                                       Optional<StorageWiggleMetrics> metrics = Optional<StorageWiggleMetrics>()) {
		return resetStorageWiggleMetrics_impl(tr, storageWiggleMetrics(primary), metrics);
	}

																#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
// This generated class is to be used only via updateStorageWiggleMetrics_impl()
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <typename TrType, class UpdateStorageWiggleMetrics_implActor>
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
class UpdateStorageWiggleMetrics_implActorState {
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
public:
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	UpdateStorageWiggleMetrics_implActorState(KeyBackedProperty<Value, NullCodec> const& wiggleSpeed,KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> const& storageMetrics,TrType const& tr,StorageWiggleMetrics const& metrics,PrimaryRegion const& primary) 
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		 : wiggleSpeed(wiggleSpeed),
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   storageMetrics(storageMetrics),
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   tr(tr),
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   metrics(metrics),
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		   primary(primary)
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
	{
		fdb_probe_actor_create("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this));

	}
	~UpdateStorageWiggleMetrics_implActorState() 
	{
		fdb_probe_actor_destroy("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			StrictFuture<Optional<Value>> __when_expr_0 = wiggleSpeed.get(tr);
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			if (static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->actor_wait_state = 1;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateStorageWiggleMetrics_implActor, 0, Optional<Value> >*>(static_cast<UpdateStorageWiggleMetrics_implActor*>(this)));
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
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
		this->~UpdateStorageWiggleMetrics_implActorState();
		static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& v,int loopDepth) 
	{
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (v.present() && v == "1"_sr)
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			storageMetrics.set(tr, metrics);
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		}
		else
		{
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			CODE_PROBE(true, "Intend to update StorageWiggleMetrics after PW disabled");
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		}
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (!static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateStorageWiggleMetrics_implActorState(); static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->destroy(); return 0; }
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		new (&static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateStorageWiggleMetrics_implActorState();
		static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && v,int loopDepth) 
	{
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (v.present() && v == "1"_sr)
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			storageMetrics.set(tr, metrics);
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		}
		else
		{
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
			CODE_PROBE(true, "Intend to update StorageWiggleMetrics after PW disabled");
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		}
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
		if (!static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateStorageWiggleMetrics_implActorState(); static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->destroy(); return 0; }
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		new (&static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateStorageWiggleMetrics_implActorState();
		static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& v,int loopDepth) 
	{
		loopDepth = a_body1cont1(v, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && v,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->actor_wait_state > 0) static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->actor_wait_state = 0;
		static_cast<UpdateStorageWiggleMetrics_implActor*>(this)->ActorCallback< UpdateStorageWiggleMetrics_implActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateStorageWiggleMetrics_implActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UpdateStorageWiggleMetrics_implActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UpdateStorageWiggleMetrics_implActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	KeyBackedProperty<Value, NullCodec> wiggleSpeed;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> storageMetrics;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	TrType tr;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	StorageWiggleMetrics metrics;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	PrimaryRegion primary;
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
};
// This generated class is to be used only via updateStorageWiggleMetrics_impl()
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <typename TrType>
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
class UpdateStorageWiggleMetrics_implActor final : public Actor<Void>, public ActorCallback< UpdateStorageWiggleMetrics_implActor<TrType>, 0, Optional<Value> >, public FastAllocated<UpdateStorageWiggleMetrics_implActor<TrType>>, public UpdateStorageWiggleMetrics_implActorState<TrType, UpdateStorageWiggleMetrics_implActor<TrType>> {
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
public:
	using FastAllocated<UpdateStorageWiggleMetrics_implActor<TrType>>::operator new;
	using FastAllocated<UpdateStorageWiggleMetrics_implActor<TrType>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateStorageWiggleMetrics_implActor<TrType>, 0, Optional<Value> >;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	UpdateStorageWiggleMetrics_implActor(KeyBackedProperty<Value, NullCodec> const& wiggleSpeed,KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> const& storageMetrics,TrType const& tr,StorageWiggleMetrics const& metrics,PrimaryRegion const& primary) 
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
		 : Actor<Void>(),
		   UpdateStorageWiggleMetrics_implActorState<TrType, UpdateStorageWiggleMetrics_implActor<TrType>>(wiggleSpeed, storageMetrics, tr, metrics, primary)
	{
		fdb_probe_actor_enter("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updateStorageWiggleMetrics_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updateStorageWiggleMetrics_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UpdateStorageWiggleMetrics_implActor<TrType>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
template <typename TrType>
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
[[nodiscard]] static Future<Void> updateStorageWiggleMetrics_impl( KeyBackedProperty<Value, NullCodec> const& wiggleSpeed, KeyBackedObjectProperty<StorageWiggleMetrics, decltype(IncludeVersion())> const& storageMetrics, TrType const& tr, StorageWiggleMetrics const& metrics, PrimaryRegion const& primary ) {
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"
	return Future<Void>(new UpdateStorageWiggleMetrics_implActor<TrType>(wiggleSpeed, storageMetrics, tr, metrics, primary));
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.g.h"
}

#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/StorageWiggleMetrics.actor.h"

	// update the serialized metrics when the perpetual wiggle is enabled
	template <typename TrType>
	Future<Void> updateStorageWiggleMetrics(TrType tr, StorageWiggleMetrics metrics, PrimaryRegion primary) {
		return updateStorageWiggleMetrics_impl(
		    perpetualWiggleSpeed(), storageWiggleMetrics(primary), tr, metrics, primary);
	}
};

#include "flow/unactorcompiler.h"
#endif