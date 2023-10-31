#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
/*
 * KeyRangeMap.actor.cpp
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

#include "fdbclient/KeyRangeMap.h"
#include "fdbclient/NativeAPI.actor.h"
#include "fdbclient/CommitTransaction.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/ReadYourWrites.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // has to be last include

void KeyRangeActorMap::getRangesAffectedByInsertion(const KeyRangeRef& keys, std::vector<KeyRange>& affectedRanges) {
	auto s = map.rangeContaining(keys.begin);
	if (s.begin() != keys.begin && s.value().isValid() && !s.value().isReady())
		affectedRanges.push_back(KeyRangeRef(s.begin(), keys.begin));
	affectedRanges.push_back(keys);
	auto e = map.rangeContaining(keys.end);
	if (e.begin() != keys.end && e.value().isValid() && !e.value().isReady())
		affectedRanges.push_back(KeyRangeRef(keys.end, e.end()));
}

RangeResult krmDecodeRanges(KeyRef mapPrefix, KeyRange keys, RangeResult kv, bool align) {
	ASSERT(!kv.more || kv.size() > 1);
	KeyRange withPrefix =
	    KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());

	RangeResult result;
	result.arena().dependsOn(kv.arena());
	result.arena().dependsOn(keys.arena());

	// Always push a kv pair <= keys.begin.
	KeyRef beginKey = keys.begin;
	if (!align && !kv.empty() && kv.front().key.startsWith(mapPrefix) && kv.front().key < withPrefix.begin) {
		beginKey = kv[0].key.removePrefix(mapPrefix);
	}
	ValueRef beginValue;
	if (!kv.empty() && kv.front().key.startsWith(mapPrefix) && kv.front().key <= withPrefix.begin) {
		beginValue = kv.front().value;
	}
	result.push_back(result.arena(), KeyValueRef(beginKey, beginValue));

	for (int i = 0; i < kv.size(); i++) {
		if (kv[i].key > withPrefix.begin && kv[i].key < withPrefix.end) {
			KeyRef k = kv[i].key.removePrefix(mapPrefix);
			result.push_back(result.arena(), KeyValueRef(k, kv[i].value));
		} else if (kv[i].key >= withPrefix.end) {
			kv.more = false;
			// There should be at most 1 value past mapPrefix + keys.end.
			ASSERT(i == kv.size() - 1);
			break;
		}
	}

	if (!kv.more) {
		KeyRef endKey = keys.end;
		if (!align && !kv.empty() && kv.back().key.startsWith(mapPrefix) && kv.back().key >= withPrefix.end) {
			endKey = kv.back().key.removePrefix(mapPrefix);
		}
		ValueRef endValue;
		if (!kv.empty()) {
			// In the aligned case, carry the last value to be the end value.
			if (align && kv.back().key.startsWith(mapPrefix) && kv.back().key > withPrefix.end) {
				endValue = result.back().value;
			} else {
				endValue = kv.back().value;
			}
		}
		result.push_back(result.arena(), KeyValueRef(endKey, endValue));
	}
	result.more = kv.more;

	return result;
}

// Returns keys.begin, all transitional points in keys, and keys.end, and their values
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmGetRanges()
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmGetRangesActor>
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActorState {
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   keys(keys),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limit(limit),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limitBytes(limitBytes)
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmGetRanges", reinterpret_cast<unsigned long>(this));

	}
	~KrmGetRangesActorState() 
	{
		fdb_probe_actor_destroy("krmGetRanges", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			KeyRange withPrefix = KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits = GetRangeLimits(limit, limitBytes);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits.minRows = 2;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.begin), firstGreaterThan(withPrefix.end), limits);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmGetRangesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmGetRangesActor*>(this)->actor_wait_state = 1;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmGetRangesActor, 0, RangeResult >*>(static_cast<KrmGetRangesActor*>(this)));
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmGetRangesActorState();
		static_cast<KrmGetRangesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& kv,int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActorState(); static_cast<KrmGetRangesActor*>(this)->destroy(); return 0; }
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesActor*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv));
		this->~KrmGetRangesActorState();
		static_cast<KrmGetRangesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kv,int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActorState(); static_cast<KrmGetRangesActor*>(this)->destroy(); return 0; }
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesActor*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv));
		this->~KrmGetRangesActorState();
		static_cast<KrmGetRangesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(kv, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(kv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KrmGetRangesActor*>(this)->actor_wait_state > 0) static_cast<KrmGetRangesActor*>(this)->actor_wait_state = 0;
		static_cast<KrmGetRangesActor*>(this)->ActorCallback< KrmGetRangesActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< KrmGetRangesActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmGetRangesActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmGetRangesActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange keys;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limit;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limitBytes;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	GetRangeLimits limits;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmGetRanges()
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActor final : public Actor<RangeResult>, public ActorCallback< KrmGetRangesActor, 0, RangeResult >, public FastAllocated<KrmGetRangesActor>, public KrmGetRangesActorState<KrmGetRangesActor> {
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmGetRangesActor>::operator new;
	using FastAllocated<KrmGetRangesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmGetRangesActor, 0, RangeResult >;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<RangeResult>(),
		   KrmGetRangesActorState<KrmGetRangesActor>(tr, mapPrefix, keys, limit, limitBytes)
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmGetRanges");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmGetRangesActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<RangeResult> krmGetRanges( Transaction* const& tr, Key const& mapPrefix, KeyRange const& keys, int const& limit, int const& limitBytes ) {
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<RangeResult>(new KrmGetRangesActor(tr, mapPrefix, keys, limit, limitBytes));
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmGetRanges()
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmGetRangesActor1>
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActor1State {
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActor1State(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   keys(keys),
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limit(limit),
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limitBytes(limitBytes)
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmGetRanges", reinterpret_cast<unsigned long>(this));

	}
	~KrmGetRangesActor1State() 
	{
		fdb_probe_actor_destroy("krmGetRanges", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			KeyRange withPrefix = KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits = GetRangeLimits(limit, limitBytes);
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits.minRows = 2;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.begin), firstGreaterThan(withPrefix.end), limits);
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmGetRangesActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmGetRangesActor1*>(this)->actor_wait_state = 1;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmGetRangesActor1, 0, RangeResult >*>(static_cast<KrmGetRangesActor1*>(this)));
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmGetRangesActor1State();
		static_cast<KrmGetRangesActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& kv,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor1*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActor1State(); static_cast<KrmGetRangesActor1*>(this)->destroy(); return 0; }
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesActor1*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv));
		this->~KrmGetRangesActor1State();
		static_cast<KrmGetRangesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kv,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor1*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActor1State(); static_cast<KrmGetRangesActor1*>(this)->destroy(); return 0; }
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesActor1*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv));
		this->~KrmGetRangesActor1State();
		static_cast<KrmGetRangesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(kv, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(kv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KrmGetRangesActor1*>(this)->actor_wait_state > 0) static_cast<KrmGetRangesActor1*>(this)->actor_wait_state = 0;
		static_cast<KrmGetRangesActor1*>(this)->ActorCallback< KrmGetRangesActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< KrmGetRangesActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmGetRangesActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmGetRangesActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange keys;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limit;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limitBytes;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	GetRangeLimits limits;
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmGetRanges()
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActor1 final : public Actor<RangeResult>, public ActorCallback< KrmGetRangesActor1, 0, RangeResult >, public FastAllocated<KrmGetRangesActor1>, public KrmGetRangesActor1State<KrmGetRangesActor1> {
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmGetRangesActor1>::operator new;
	using FastAllocated<KrmGetRangesActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmGetRangesActor1, 0, RangeResult >;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActor1(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<RangeResult>(),
		   KrmGetRangesActor1State<KrmGetRangesActor1>(tr, mapPrefix, keys, limit, limitBytes)
	{
		fdb_probe_actor_enter("krmGetRanges", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmGetRanges");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmGetRanges", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmGetRangesActor1, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<RangeResult> krmGetRanges( Reference<ReadYourWritesTransaction> const& tr, Key const& mapPrefix, KeyRange const& keys, int const& limit, int const& limitBytes ) {
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<RangeResult>(new KrmGetRangesActor1(tr, mapPrefix, keys, limit, limitBytes));
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

// Returns keys.begin, all transitional points in keys, and keys.end, and their values
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmGetRangesUnaligned()
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmGetRangesUnalignedActor>
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesUnalignedActorState {
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesUnalignedActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   keys(keys),
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limit(limit),
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limitBytes(limitBytes)
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this));

	}
	~KrmGetRangesUnalignedActorState() 
	{
		fdb_probe_actor_destroy("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			KeyRange withPrefix = KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits = GetRangeLimits(limit, limitBytes);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits.minRows = 2;
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.begin), KeySelectorRef(withPrefix.end, false, +2), limits);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmGetRangesUnalignedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmGetRangesUnalignedActor*>(this)->actor_wait_state = 1;
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >*>(static_cast<KrmGetRangesUnalignedActor*>(this)));
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmGetRangesUnalignedActorState();
		static_cast<KrmGetRangesUnalignedActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& kv,int loopDepth) 
	{
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesUnalignedActor*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv, false)); this->~KrmGetRangesUnalignedActorState(); static_cast<KrmGetRangesUnalignedActor*>(this)->destroy(); return 0; }
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesUnalignedActor*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv, false));
		this->~KrmGetRangesUnalignedActorState();
		static_cast<KrmGetRangesUnalignedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kv,int loopDepth) 
	{
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesUnalignedActor*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv, false)); this->~KrmGetRangesUnalignedActorState(); static_cast<KrmGetRangesUnalignedActor*>(this)->destroy(); return 0; }
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesUnalignedActor*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv, false));
		this->~KrmGetRangesUnalignedActorState();
		static_cast<KrmGetRangesUnalignedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(kv, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(kv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KrmGetRangesUnalignedActor*>(this)->actor_wait_state > 0) static_cast<KrmGetRangesUnalignedActor*>(this)->actor_wait_state = 0;
		static_cast<KrmGetRangesUnalignedActor*>(this)->ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange keys;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limit;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limitBytes;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	GetRangeLimits limits;
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmGetRangesUnaligned()
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesUnalignedActor final : public Actor<RangeResult>, public ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >, public FastAllocated<KrmGetRangesUnalignedActor>, public KrmGetRangesUnalignedActorState<KrmGetRangesUnalignedActor> {
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmGetRangesUnalignedActor>::operator new;
	using FastAllocated<KrmGetRangesUnalignedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesUnalignedActor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<RangeResult>(),
		   KrmGetRangesUnalignedActorState<KrmGetRangesUnalignedActor>(tr, mapPrefix, keys, limit, limitBytes)
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmGetRangesUnaligned");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmGetRangesUnalignedActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<RangeResult> krmGetRangesUnaligned( Transaction* const& tr, Key const& mapPrefix, KeyRange const& keys, int const& limit, int const& limitBytes ) {
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<RangeResult>(new KrmGetRangesUnalignedActor(tr, mapPrefix, keys, limit, limitBytes));
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmGetRangesUnaligned()
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmGetRangesUnalignedActor1>
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesUnalignedActor1State {
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesUnalignedActor1State(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   keys(keys),
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limit(limit),
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   limitBytes(limitBytes)
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this));

	}
	~KrmGetRangesUnalignedActor1State() 
	{
		fdb_probe_actor_destroy("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			KeyRange withPrefix = KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits = GetRangeLimits(limit, limitBytes);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			limits.minRows = 2;
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.begin), KeySelectorRef(withPrefix.end, false, +2), limits);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmGetRangesUnalignedActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmGetRangesUnalignedActor1*>(this)->actor_wait_state = 1;
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >*>(static_cast<KrmGetRangesUnalignedActor1*>(this)));
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmGetRangesUnalignedActor1State();
		static_cast<KrmGetRangesUnalignedActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& kv,int loopDepth) 
	{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesUnalignedActor1*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv, false)); this->~KrmGetRangesUnalignedActor1State(); static_cast<KrmGetRangesUnalignedActor1*>(this)->destroy(); return 0; }
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesUnalignedActor1*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv, false));
		this->~KrmGetRangesUnalignedActor1State();
		static_cast<KrmGetRangesUnalignedActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kv,int loopDepth) 
	{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesUnalignedActor1*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv, false)); this->~KrmGetRangesUnalignedActor1State(); static_cast<KrmGetRangesUnalignedActor1*>(this)->destroy(); return 0; }
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesUnalignedActor1*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv, false));
		this->~KrmGetRangesUnalignedActor1State();
		static_cast<KrmGetRangesUnalignedActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(kv, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && kv,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(kv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KrmGetRangesUnalignedActor1*>(this)->actor_wait_state > 0) static_cast<KrmGetRangesUnalignedActor1*>(this)->actor_wait_state = 0;
		static_cast<KrmGetRangesUnalignedActor1*>(this)->ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange keys;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limit;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	int limitBytes;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	GetRangeLimits limits;
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmGetRangesUnaligned()
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesUnalignedActor1 final : public Actor<RangeResult>, public ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >, public FastAllocated<KrmGetRangesUnalignedActor1>, public KrmGetRangesUnalignedActor1State<KrmGetRangesUnalignedActor1> {
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmGetRangesUnalignedActor1>::operator new;
	using FastAllocated<KrmGetRangesUnalignedActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesUnalignedActor1(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<RangeResult>(),
		   KrmGetRangesUnalignedActor1State<KrmGetRangesUnalignedActor1>(tr, mapPrefix, keys, limit, limitBytes)
	{
		fdb_probe_actor_enter("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmGetRangesUnaligned");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmGetRangesUnaligned", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmGetRangesUnalignedActor1, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<RangeResult> krmGetRangesUnaligned( Reference<ReadYourWritesTransaction> const& tr, Key const& mapPrefix, KeyRange const& keys, int const& limit, int const& limitBytes ) {
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<RangeResult>(new KrmGetRangesUnalignedActor1(tr, mapPrefix, keys, limit, limitBytes));
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

void krmSetPreviouslyEmptyRange(Transaction* tr,
                                const KeyRef& mapPrefix,
                                const KeyRangeRef& keys,
                                const ValueRef& newValue,
                                const ValueRef& oldEndValue) {
	KeyRange withPrefix =
	    KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
	tr->set(withPrefix.begin, newValue);
	tr->set(withPrefix.end, oldEndValue);
}

void krmSetPreviouslyEmptyRange(CommitTransactionRef& tr,
                                Arena& trArena,
                                const KeyRef& mapPrefix,
                                const KeyRangeRef& keys,
                                const ValueRef& newValue,
                                const ValueRef& oldEndValue) {
	KeyRange withPrefix =
	    KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
	tr.set(trArena, withPrefix.begin, newValue);
	tr.set(trArena, withPrefix.end, oldEndValue);
}

															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmSetRange()
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmSetRangeActor>
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActorState {
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   range(range),
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   value(value),
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   withPrefix(KeyRangeRef(mapPrefix.toString() + range.begin.toString(), mapPrefix.toString() + range.end.toString()))
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmSetRange", reinterpret_cast<unsigned long>(this));

	}
	~KrmSetRangeActorState() 
	{
		fdb_probe_actor_destroy("krmSetRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.end), firstGreaterThan(withPrefix.end), 1, Snapshot::True);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmSetRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmSetRangeActor*>(this)->actor_wait_state = 1;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmSetRangeActor, 0, RangeResult >*>(static_cast<KrmSetRangeActor*>(this)));
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmSetRangeActorState();
		static_cast<KrmSetRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& old,int loopDepth) 
	{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActorState(); static_cast<KrmSetRangeActor*>(this)->destroy(); return 0; }
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeActorState();
		static_cast<KrmSetRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && old,int loopDepth) 
	{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActorState(); static_cast<KrmSetRangeActor*>(this)->destroy(); return 0; }
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeActorState();
		static_cast<KrmSetRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& old,int loopDepth) 
	{
		loopDepth = a_body1cont1(old, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && old,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(old), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KrmSetRangeActor*>(this)->actor_wait_state > 0) static_cast<KrmSetRangeActor*>(this)->actor_wait_state = 0;
		static_cast<KrmSetRangeActor*>(this)->ActorCallback< KrmSetRangeActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< KrmSetRangeActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmSetRangeActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmSetRangeActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange range;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Value value;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange withPrefix;
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmSetRange()
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActor final : public Actor<Void>, public ActorCallback< KrmSetRangeActor, 0, RangeResult >, public FastAllocated<KrmSetRangeActor>, public KrmSetRangeActorState<KrmSetRangeActor> {
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmSetRangeActor>::operator new;
	using FastAllocated<KrmSetRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmSetRangeActor, 0, RangeResult >;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<Void>(),
		   KrmSetRangeActorState<KrmSetRangeActor>(tr, mapPrefix, range, value)
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmSetRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmSetRangeActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<Void> krmSetRange( Transaction* const& tr, Key const& mapPrefix, KeyRange const& range, Value const& value ) {
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new KrmSetRangeActor(tr, mapPrefix, range, value));
															#line 1247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmSetRange()
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmSetRangeActor1>
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActor1State {
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActor1State(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   range(range),
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   value(value),
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   withPrefix(KeyRangeRef(mapPrefix.toString() + range.begin.toString(), mapPrefix.toString() + range.end.toString()))
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmSetRange", reinterpret_cast<unsigned long>(this));

	}
	~KrmSetRangeActor1State() 
	{
		fdb_probe_actor_destroy("krmSetRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.end), firstGreaterThan(withPrefix.end), 1, Snapshot::True);
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmSetRangeActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmSetRangeActor1*>(this)->actor_wait_state = 1;
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmSetRangeActor1, 0, RangeResult >*>(static_cast<KrmSetRangeActor1*>(this)));
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmSetRangeActor1State();
		static_cast<KrmSetRangeActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& old,int loopDepth) 
	{
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActor1State(); static_cast<KrmSetRangeActor1*>(this)->destroy(); return 0; }
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeActor1State();
		static_cast<KrmSetRangeActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && old,int loopDepth) 
	{
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActor1State(); static_cast<KrmSetRangeActor1*>(this)->destroy(); return 0; }
															#line 1387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeActor1State();
		static_cast<KrmSetRangeActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& old,int loopDepth) 
	{
		loopDepth = a_body1cont1(old, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && old,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(old), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KrmSetRangeActor1*>(this)->actor_wait_state > 0) static_cast<KrmSetRangeActor1*>(this)->actor_wait_state = 0;
		static_cast<KrmSetRangeActor1*>(this)->ActorCallback< KrmSetRangeActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< KrmSetRangeActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmSetRangeActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmSetRangeActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange range;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Value value;
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange withPrefix;
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmSetRange()
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActor1 final : public Actor<Void>, public ActorCallback< KrmSetRangeActor1, 0, RangeResult >, public FastAllocated<KrmSetRangeActor1>, public KrmSetRangeActor1State<KrmSetRangeActor1> {
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmSetRangeActor1>::operator new;
	using FastAllocated<KrmSetRangeActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmSetRangeActor1, 0, RangeResult >;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActor1(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<Void>(),
		   KrmSetRangeActor1State<KrmSetRangeActor1>(tr, mapPrefix, range, value)
	{
		fdb_probe_actor_enter("krmSetRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmSetRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmSetRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmSetRangeActor1, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<Void> krmSetRange( Reference<ReadYourWritesTransaction> const& tr, Key const& mapPrefix, KeyRange const& range, Value const& value ) {
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new KrmSetRangeActor1(tr, mapPrefix, range, value));
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

// Sets a range of keys in a key range map, coalescing with adjacent regions if the values match
// Ranges outside of maxRange will not be coalesced
// CAUTION: use care when attempting to coalesce multiple ranges in the same prefix in a single transaction
															#line 1520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmSetRangeCoalescing_()
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class Transaction, class KrmSetRangeCoalescing_Actor>
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeCoalescing_ActorState {
															#line 1527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeCoalescing_ActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,KeyRange const& maxRange,Value const& value) 
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   range(range),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   maxRange(maxRange),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		   value(value)
															#line 1542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this));

	}
	~KrmSetRangeCoalescing_ActorState() 
	{
		fdb_probe_actor_destroy("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(maxRange.contains(range));
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			withPrefix = KeyRangeRef(mapPrefix.toString() + range.begin.toString(), mapPrefix.toString() + range.end.toString());
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			maxWithPrefix = KeyRangeRef(mapPrefix.toString() + maxRange.begin.toString(), mapPrefix.toString() + maxRange.end.toString());
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			keys = std::vector<Future<RangeResult>>();
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			keys.push_back( tr->getRange(lastLessThan(withPrefix.begin), firstGreaterOrEqual(withPrefix.begin), 1, Snapshot::True));
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			keys.push_back( tr->getRange(lastLessOrEqual(withPrefix.end), firstGreaterThan(withPrefix.end) + 1, 2, Snapshot::True));
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(keys);
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmSetRangeCoalescing_Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmSetRangeCoalescing_Actor*>(this)->actor_wait_state = 1;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmSetRangeCoalescing_Actor, 0, Void >*>(static_cast<KrmSetRangeCoalescing_Actor*>(this)));
															#line 1576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
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
		this->~KrmSetRangeCoalescing_ActorState();
		static_cast<KrmSetRangeCoalescing_Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		auto beginRange = keys[0].get();
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasBegin = beginRange.size() > 0 && beginRange[0].key.startsWith(mapPrefix);
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value beginValue = hasBegin ? beginRange[0].value : ""_sr;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		beginKey = withPrefix.begin;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (beginValue == value)
															#line 1607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			bool outsideRange = !hasBegin || beginRange[0].key < maxWithPrefix.begin;
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			beginKey = outsideRange ? maxWithPrefix.begin : beginRange[0].key;
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		auto endRange = keys[1].get();
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasEnd = endRange.size() >= 1 && endRange[0].key.startsWith(mapPrefix) && endRange[0].key <= withPrefix.end;
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasNext = (endRange.size() == 2 && endRange[1].key.startsWith(mapPrefix)) || (endRange.size() == 1 && withPrefix.end < endRange[0].key && endRange[0].key.startsWith(mapPrefix));
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value existingValue = hasEnd ? endRange[0].value : ""_sr;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool valueMatches = value == existingValue;
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasBegin ? beginRange[0].key : mapPrefix, withPrefix.begin);
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		conflictRange = KeyRangeRef(hasEnd ? endRange[0].key : mapPrefix, hasNext ? keyAfter(endRange.end()[-1].key) : strinc(mapPrefix));
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		endKey = Key();
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		endValue = Value();
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (hasNext && endRange.end()[-1].key <= maxWithPrefix.end && valueMatches)
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			endKey = endRange.end()[-1].key;
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			endValue = endRange.end()[-1].value;
															#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
		else
		{
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (valueMatches)
															#line 1663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endKey = maxWithPrefix.end;
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			}
			else
			{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endKey = withPrefix.end;
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			}
		}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(KeyRangeRef(beginKey, endKey));
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		ASSERT(value != endValue || endKey == maxWithPrefix.end);
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(beginKey, value);
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(endKey, endValue);
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeCoalescing_ActorState(); static_cast<KrmSetRangeCoalescing_Actor*>(this)->destroy(); return 0; }
															#line 1690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeCoalescing_ActorState();
		static_cast<KrmSetRangeCoalescing_Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		auto beginRange = keys[0].get();
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasBegin = beginRange.size() > 0 && beginRange[0].key.startsWith(mapPrefix);
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value beginValue = hasBegin ? beginRange[0].value : ""_sr;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		beginKey = withPrefix.begin;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (beginValue == value)
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			bool outsideRange = !hasBegin || beginRange[0].key < maxWithPrefix.begin;
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			beginKey = outsideRange ? maxWithPrefix.begin : beginRange[0].key;
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		auto endRange = keys[1].get();
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasEnd = endRange.size() >= 1 && endRange[0].key.startsWith(mapPrefix) && endRange[0].key <= withPrefix.end;
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool hasNext = (endRange.size() == 2 && endRange[1].key.startsWith(mapPrefix)) || (endRange.size() == 1 && withPrefix.end < endRange[0].key && endRange[0].key.startsWith(mapPrefix));
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		Value existingValue = hasEnd ? endRange[0].value : ""_sr;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		bool valueMatches = value == existingValue;
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasBegin ? beginRange[0].key : mapPrefix, withPrefix.begin);
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		conflictRange = KeyRangeRef(hasEnd ? endRange[0].key : mapPrefix, hasNext ? keyAfter(endRange.end()[-1].key) : strinc(mapPrefix));
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		endKey = Key();
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		endValue = Value();
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (hasNext && endRange.end()[-1].key <= maxWithPrefix.end && valueMatches)
															#line 1754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			endKey = endRange.end()[-1].key;
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			endValue = endRange.end()[-1].value;
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		}
		else
		{
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (valueMatches)
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endKey = maxWithPrefix.end;
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			}
			else
			{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endKey = withPrefix.end;
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			}
		}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(KeyRangeRef(beginKey, endKey));
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		ASSERT(value != endValue || endKey == maxWithPrefix.end);
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(beginKey, value);
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(endKey, endValue);
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeCoalescing_ActorState(); static_cast<KrmSetRangeCoalescing_Actor*>(this)->destroy(); return 0; }
															#line 1793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeCoalescing_ActorState();
		static_cast<KrmSetRangeCoalescing_Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<KrmSetRangeCoalescing_Actor*>(this)->actor_wait_state > 0) static_cast<KrmSetRangeCoalescing_Actor*>(this)->actor_wait_state = 0;
		static_cast<KrmSetRangeCoalescing_Actor*>(this)->ActorCallback< KrmSetRangeCoalescing_Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< KrmSetRangeCoalescing_Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< KrmSetRangeCoalescing_Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< KrmSetRangeCoalescing_Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange range;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange maxRange;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Value value;
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange withPrefix;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange maxWithPrefix;
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	std::vector<Future<RangeResult>> keys;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key beginKey;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Key endKey;
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	Value endValue;
															#line 1886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmSetRangeCoalescing_()
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class Transaction>
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeCoalescing_Actor final : public Actor<Void>, public ActorCallback< KrmSetRangeCoalescing_Actor<Transaction>, 0, Void >, public FastAllocated<KrmSetRangeCoalescing_Actor<Transaction>>, public KrmSetRangeCoalescing_ActorState<Transaction, KrmSetRangeCoalescing_Actor<Transaction>> {
															#line 1893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmSetRangeCoalescing_Actor<Transaction>>::operator new;
	using FastAllocated<KrmSetRangeCoalescing_Actor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmSetRangeCoalescing_Actor<Transaction>, 0, Void >;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeCoalescing_Actor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,KeyRange const& maxRange,Value const& value) 
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<Void>(),
		   KrmSetRangeCoalescing_ActorState<Transaction, KrmSetRangeCoalescing_Actor<Transaction>>(tr, mapPrefix, range, maxRange, value)
	{
		fdb_probe_actor_enter("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("krmSetRangeCoalescing_");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("krmSetRangeCoalescing_", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< KrmSetRangeCoalescing_Actor<Transaction>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class Transaction>
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] static Future<Void> krmSetRangeCoalescing_( Transaction* const& tr, Key const& mapPrefix, KeyRange const& range, KeyRange const& maxRange, Value const& value ) {
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new KrmSetRangeCoalescing_Actor<Transaction>(tr, mapPrefix, range, maxRange, value));
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
Future<Void> krmSetRangeCoalescing(Transaction* const& tr,
                                   Key const& mapPrefix,
                                   KeyRange const& range,
                                   KeyRange const& maxRange,
                                   Value const& value) {
	return krmSetRangeCoalescing_(tr, mapPrefix, range, maxRange, value);
}
Future<Void> krmSetRangeCoalescing(Reference<ReadYourWritesTransaction> const& tr,
                                   Key const& mapPrefix,
                                   KeyRange const& range,
                                   KeyRange const& maxRange,
                                   Value const& value) {
	return holdWhile(tr, krmSetRangeCoalescing_(tr.getPtr(), mapPrefix, range, maxRange, value));
}

															#line 1953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase318()
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class FlowTestCase318Actor>
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class FlowTestCase318ActorState {
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	FlowTestCase318ActorState(UnitTestParameters const& params) 
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : params(params)
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase318", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase318ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase318", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			Arena arena;
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			Key prefix = "/prefix/"_sr;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyA = StringRef(arena, "/prefix/a"_sr);
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyB = StringRef(arena, "/prefix/b"_sr);
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyC = StringRef(arena, "/prefix/c"_sr);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyD = StringRef(arena, "/prefix/d"_sr);
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyA = StringRef(arena, "a"_sr);
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyB = StringRef(arena, "b"_sr);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyC = StringRef(arena, "c"_sr);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyD = StringRef(arena, "d"_sr);
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyE = StringRef(arena, "e"_sr);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyAB = StringRef(arena, "ab"_sr);
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyAC = StringRef(arena, "ac"_sr);
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyCD = StringRef(arena, "cd"_sr);
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			RangeResult kv;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyA, keyA));
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyB, keyB));
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			RangeResult decodedRanges = krmDecodeRanges(prefix, KeyRangeRef(keyAB, keyAC), kv);
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.size() == 2);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().key == keyAB);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().value == keyA);
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().key == keyAC);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().value == keyA);
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyC, keyC));
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyD, keyD));
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			decodedRanges = krmDecodeRanges(prefix, KeyRangeRef(keyAB, keyCD), kv);
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.size() == 4);
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().key == keyAB);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().value == keyA);
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().key == keyCD);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().value == keyC);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			decodedRanges = krmDecodeRanges(prefix, KeyRangeRef(StringRef(), keyE), kv);
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.size() == 6);
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().key == StringRef());
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().value == StringRef());
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().key == keyE);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().value == keyD);
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (!static_cast<FlowTestCase318Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase318ActorState(); static_cast<FlowTestCase318Actor*>(this)->destroy(); return 0; }
															#line 2056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			new (&static_cast<FlowTestCase318Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase318ActorState();
			static_cast<FlowTestCase318Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase318ActorState();
		static_cast<FlowTestCase318Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	UnitTestParameters params;
															#line 2080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase318()
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class FlowTestCase318Actor final : public Actor<Void>, public FastAllocated<FlowTestCase318Actor>, public FlowTestCase318ActorState<FlowTestCase318Actor> {
															#line 2085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase318Actor>::operator new;
	using FastAllocated<FlowTestCase318Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	FlowTestCase318Actor(UnitTestParameters const& params) 
															#line 2095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase318ActorState<FlowTestCase318Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase318", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase318");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase318", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
static Future<Void> flowTestCase318( UnitTestParameters const& params ) {
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new FlowTestCase318Actor(params));
															#line 2122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase318, "/keyrangemap/decoderange/aligned")

#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"

															#line 2128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase370()
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
template <class FlowTestCase370Actor>
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class FlowTestCase370ActorState {
															#line 2135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	FlowTestCase370ActorState(UnitTestParameters const& params) 
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
		 : params(params)
															#line 2142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase370", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase370ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase370", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			Arena arena;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			Key prefix = "/prefix/"_sr;
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyA = StringRef(arena, "/prefix/a"_sr);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyB = StringRef(arena, "/prefix/b"_sr);
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyC = StringRef(arena, "/prefix/c"_sr);
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef fullKeyD = StringRef(arena, "/prefix/d"_sr);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyA = StringRef(arena, "a"_sr);
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyB = StringRef(arena, "b"_sr);
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyC = StringRef(arena, "c"_sr);
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyD = StringRef(arena, "d"_sr);
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyE = StringRef(arena, "e"_sr);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyAB = StringRef(arena, "ab"_sr);
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyAC = StringRef(arena, "ac"_sr);
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			StringRef keyCD = StringRef(arena, "cd"_sr);
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			RangeResult kv;
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyA, keyA));
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyB, keyB));
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			RangeResult decodedRanges = krmDecodeRanges(prefix, KeyRangeRef(keyAB, keyAC), kv, false);
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.size() == 2);
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().key == keyA);
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().value == keyA);
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().key == keyB);
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().value == keyB);
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyC, keyC));
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			kv.push_back(arena, KeyValueRef(fullKeyD, keyD));
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			decodedRanges = krmDecodeRanges(prefix, KeyRangeRef(keyAB, keyCD), kv, false);
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.size() == 4);
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().key == keyA);
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().value == keyA);
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().key == keyD);
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().value == keyD);
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			decodedRanges = krmDecodeRanges(prefix, KeyRangeRef(StringRef(), keyE), kv, false);
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.size() == 6);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().key == StringRef());
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.front().value == StringRef());
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().key == keyE);
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(decodedRanges.back().value == keyD);
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
			if (!static_cast<FlowTestCase370Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase370ActorState(); static_cast<FlowTestCase370Actor*>(this)->destroy(); return 0; }
															#line 2231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
			new (&static_cast<FlowTestCase370Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase370ActorState();
			static_cast<FlowTestCase370Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase370ActorState();
		static_cast<FlowTestCase370Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	UnitTestParameters params;
															#line 2255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase370()
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
class FlowTestCase370Actor final : public Actor<Void>, public FastAllocated<FlowTestCase370Actor>, public FlowTestCase370ActorState<FlowTestCase370Actor> {
															#line 2260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase370Actor>::operator new;
	using FastAllocated<FlowTestCase370Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	FlowTestCase370Actor(UnitTestParameters const& params) 
															#line 2270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase370ActorState<FlowTestCase370Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase370", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase370");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase370", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
static Future<Void> flowTestCase370( UnitTestParameters const& params ) {
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new FlowTestCase370Actor(params));
															#line 2297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/KeyRangeMap.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase370, "/keyrangemap/decoderange/unaligned")

