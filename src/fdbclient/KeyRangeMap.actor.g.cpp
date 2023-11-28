#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
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

RangeResult krmDecodeRanges(KeyRef mapPrefix, KeyRange keys, RangeResult kv) {
	ASSERT(!kv.more || kv.size() > 1);
	KeyRange withPrefix =
	    KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());

	ValueRef beginValue, endValue;
	if (kv.size() && kv[0].key.startsWith(mapPrefix))
		beginValue = kv[0].value;
	if (kv.size() && kv.end()[-1].key.startsWith(mapPrefix))
		endValue = kv.end()[-1].value;

	RangeResult result;
	result.arena().dependsOn(kv.arena());
	result.arena().dependsOn(keys.arena());

	result.push_back(result.arena(), KeyValueRef(keys.begin, beginValue));
	for (int i = 0; i < kv.size(); i++) {
		if (kv[i].key > withPrefix.begin && kv[i].key < withPrefix.end) {
			KeyRef k = kv[i].key.removePrefix(mapPrefix);
			result.push_back(result.arena(), KeyValueRef(k, kv[i].value));
		} else if (kv[i].key >= withPrefix.end)
			kv.more = false;
	}

	if (!kv.more)
		result.push_back(result.arena(), KeyValueRef(keys.end, endValue));
	result.more = kv.more;

	return result;
}

// Returns keys.begin, all transitional points in keys, and keys.end, and their values
															#line 72 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmGetRanges()
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmGetRangesActor>
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActorState {
															#line 79 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   keys(keys),
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   limit(limit),
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   limitBytes(limitBytes)
															#line 94 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 71 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			KeyRange withPrefix = KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
															#line 74 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			limits = GetRangeLimits(limit, limitBytes);
															#line 75 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			limits.minRows = 2;
															#line 76 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.begin), firstGreaterThan(withPrefix.end), limits);
															#line 76 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmGetRangesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 117 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmGetRangesActor*>(this)->actor_wait_state = 1;
															#line 76 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmGetRangesActor, 0, RangeResult >*>(static_cast<KrmGetRangesActor*>(this)));
															#line 122 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 78 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActorState(); static_cast<KrmGetRangesActor*>(this)->destroy(); return 0; }
															#line 145 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesActor*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv));
		this->~KrmGetRangesActorState();
		static_cast<KrmGetRangesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kv,int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActorState(); static_cast<KrmGetRangesActor*>(this)->destroy(); return 0; }
															#line 157 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange keys;
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	int limit;
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	int limitBytes;
															#line 74 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	GetRangeLimits limits;
															#line 240 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmGetRanges()
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActor final : public Actor<RangeResult>, public ActorCallback< KrmGetRangesActor, 0, RangeResult >, public FastAllocated<KrmGetRangesActor>, public KrmGetRangesActorState<KrmGetRangesActor> {
															#line 245 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmGetRangesActor>::operator new;
	using FastAllocated<KrmGetRangesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmGetRangesActor, 0, RangeResult >;
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 256 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<RangeResult> krmGetRanges( Transaction* const& tr, Key const& mapPrefix, KeyRange const& keys, int const& limit, int const& limitBytes ) {
															#line 70 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	return Future<RangeResult>(new KrmGetRangesActor(tr, mapPrefix, keys, limit, limitBytes));
															#line 284 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 80 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"

															#line 289 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmGetRanges()
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmGetRangesActor1>
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActor1State {
															#line 296 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActor1State(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   keys(keys),
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   limit(limit),
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   limitBytes(limitBytes)
															#line 311 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 86 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			KeyRange withPrefix = KeyRangeRef(mapPrefix.toString() + keys.begin.toString(), mapPrefix.toString() + keys.end.toString());
															#line 89 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			limits = GetRangeLimits(limit, limitBytes);
															#line 90 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			limits.minRows = 2;
															#line 91 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.begin), firstGreaterThan(withPrefix.end), limits);
															#line 91 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmGetRangesActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 334 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmGetRangesActor1*>(this)->actor_wait_state = 1;
															#line 91 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmGetRangesActor1, 0, RangeResult >*>(static_cast<KrmGetRangesActor1*>(this)));
															#line 339 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 93 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor1*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActor1State(); static_cast<KrmGetRangesActor1*>(this)->destroy(); return 0; }
															#line 362 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmGetRangesActor1*>(this)->SAV< RangeResult >::value()) RangeResult(krmDecodeRanges(mapPrefix, keys, kv));
		this->~KrmGetRangesActor1State();
		static_cast<KrmGetRangesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kv,int loopDepth) 
	{
															#line 93 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmGetRangesActor1*>(this)->SAV<RangeResult>::futures) { (void)(krmDecodeRanges(mapPrefix, keys, kv)); this->~KrmGetRangesActor1State(); static_cast<KrmGetRangesActor1*>(this)->destroy(); return 0; }
															#line 374 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange keys;
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	int limit;
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	int limitBytes;
															#line 89 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	GetRangeLimits limits;
															#line 457 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmGetRanges()
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmGetRangesActor1 final : public Actor<RangeResult>, public ActorCallback< KrmGetRangesActor1, 0, RangeResult >, public FastAllocated<KrmGetRangesActor1>, public KrmGetRangesActor1State<KrmGetRangesActor1> {
															#line 462 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmGetRangesActor1>::operator new;
	using FastAllocated<KrmGetRangesActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmGetRangesActor1, 0, RangeResult >;
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmGetRangesActor1(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& keys,int const& limit,int const& limitBytes) 
															#line 473 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<RangeResult> krmGetRanges( Reference<ReadYourWritesTransaction> const& tr, Key const& mapPrefix, KeyRange const& keys, int const& limit, int const& limitBytes ) {
															#line 81 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	return Future<RangeResult>(new KrmGetRangesActor1(tr, mapPrefix, keys, limit, limitBytes));
															#line 501 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 95 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"

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

															#line 529 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmSetRange()
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmSetRangeActor>
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActorState {
															#line 536 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   range(range),
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   value(value),
															#line 120 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   withPrefix(KeyRangeRef(mapPrefix.toString() + range.begin.toString(), mapPrefix.toString() + range.end.toString()))
															#line 551 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 122 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.end), firstGreaterThan(withPrefix.end), 1, Snapshot::True);
															#line 122 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmSetRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 568 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmSetRangeActor*>(this)->actor_wait_state = 1;
															#line 122 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmSetRangeActor, 0, RangeResult >*>(static_cast<KrmSetRangeActor*>(this)));
															#line 573 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 125 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 126 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 127 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 600 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 128 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 604 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 130 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 131 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 610 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 132 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 614 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 134 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 135 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 136 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 138 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActorState(); static_cast<KrmSetRangeActor*>(this)->destroy(); return 0; }
															#line 624 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeActorState();
		static_cast<KrmSetRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && old,int loopDepth) 
	{
															#line 125 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 126 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 127 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 640 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 128 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 644 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 130 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 131 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 650 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 132 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 654 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 134 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 135 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 136 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 138 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActorState(); static_cast<KrmSetRangeActor*>(this)->destroy(); return 0; }
															#line 664 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange range;
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Value value;
															#line 120 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange withPrefix;
															#line 745 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmSetRange()
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActor final : public Actor<Void>, public ActorCallback< KrmSetRangeActor, 0, RangeResult >, public FastAllocated<KrmSetRangeActor>, public KrmSetRangeActorState<KrmSetRangeActor> {
															#line 750 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmSetRangeActor>::operator new;
	using FastAllocated<KrmSetRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmSetRangeActor, 0, RangeResult >;
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 761 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<Void> krmSetRange( Transaction* const& tr, Key const& mapPrefix, KeyRange const& range, Value const& value ) {
															#line 119 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new KrmSetRangeActor(tr, mapPrefix, range, value));
															#line 789 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 140 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"

															#line 794 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmSetRange()
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class KrmSetRangeActor1>
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActor1State {
															#line 801 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActor1State(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   range(range),
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   value(value),
															#line 142 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   withPrefix(KeyRangeRef(mapPrefix.toString() + range.begin.toString(), mapPrefix.toString() + range.end.toString()))
															#line 816 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 144 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(lastLessOrEqual(withPrefix.end), firstGreaterThan(withPrefix.end), 1, Snapshot::True);
															#line 144 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmSetRangeActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 833 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmSetRangeActor1*>(this)->actor_wait_state = 1;
															#line 144 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmSetRangeActor1, 0, RangeResult >*>(static_cast<KrmSetRangeActor1*>(this)));
															#line 838 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 147 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 148 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 149 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 865 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 150 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 869 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 152 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 153 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 875 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 154 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 879 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 156 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 157 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 158 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 160 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActor1State(); static_cast<KrmSetRangeActor1*>(this)->destroy(); return 0; }
															#line 889 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeActor1State();
		static_cast<KrmSetRangeActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && old,int loopDepth) 
	{
															#line 147 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value oldValue;
															#line 148 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasResult = old.size() > 0 && old[0].key.startsWith(mapPrefix);
															#line 149 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (hasResult)
															#line 905 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 150 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			oldValue = old[0].value;
															#line 909 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 152 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasResult ? old[0].key : mapPrefix.toString(), keyAfter(withPrefix.end));
															#line 153 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 915 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 154 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 919 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 156 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(withPrefix);
															#line 157 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.begin, value);
															#line 158 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(withPrefix.end, oldValue);
															#line 160 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeActor1State(); static_cast<KrmSetRangeActor1*>(this)->destroy(); return 0; }
															#line 929 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange range;
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Value value;
															#line 142 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange withPrefix;
															#line 1010 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmSetRange()
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeActor1 final : public Actor<Void>, public ActorCallback< KrmSetRangeActor1, 0, RangeResult >, public FastAllocated<KrmSetRangeActor1>, public KrmSetRangeActor1State<KrmSetRangeActor1> {
															#line 1015 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmSetRangeActor1>::operator new;
	using FastAllocated<KrmSetRangeActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmSetRangeActor1, 0, RangeResult >;
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeActor1(Reference<ReadYourWritesTransaction> const& tr,Key const& mapPrefix,KeyRange const& range,Value const& value) 
															#line 1026 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] Future<Void> krmSetRange( Reference<ReadYourWritesTransaction> const& tr, Key const& mapPrefix, KeyRange const& range, Value const& value ) {
															#line 141 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new KrmSetRangeActor1(tr, mapPrefix, range, value));
															#line 1054 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 162 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"

// Sets a range of keys in a key range map, coalescing with adjacent regions if the values match
// Ranges outside of maxRange will not be coalesced
// CAUTION: use care when attempting to coalesce multiple ranges in the same prefix in a single transaction
															#line 1062 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
namespace {
// This generated class is to be used only via krmSetRangeCoalescing_()
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class Transaction, class KrmSetRangeCoalescing_Actor>
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeCoalescing_ActorState {
															#line 1069 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeCoalescing_ActorState(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,KeyRange const& maxRange,Value const& value) 
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		 : tr(tr),
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   mapPrefix(mapPrefix),
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   range(range),
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   maxRange(maxRange),
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		   value(value)
															#line 1084 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 172 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			ASSERT(maxRange.contains(range));
															#line 174 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			withPrefix = KeyRangeRef(mapPrefix.toString() + range.begin.toString(), mapPrefix.toString() + range.end.toString());
															#line 176 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			maxWithPrefix = KeyRangeRef(mapPrefix.toString() + maxRange.begin.toString(), mapPrefix.toString() + maxRange.end.toString());
															#line 179 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			keys = std::vector<Future<RangeResult>>();
															#line 180 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			keys.push_back( tr->getRange(lastLessThan(withPrefix.begin), firstGreaterOrEqual(withPrefix.begin), 1, Snapshot::True));
															#line 182 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			keys.push_back( tr->getRange(lastLessOrEqual(withPrefix.end), firstGreaterThan(withPrefix.end) + 1, 2, Snapshot::True));
															#line 184 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(keys);
															#line 184 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (static_cast<KrmSetRangeCoalescing_Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1113 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<KrmSetRangeCoalescing_Actor*>(this)->actor_wait_state = 1;
															#line 184 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< KrmSetRangeCoalescing_Actor, 0, Void >*>(static_cast<KrmSetRangeCoalescing_Actor*>(this)));
															#line 1118 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 187 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		auto beginRange = keys[0].get();
															#line 188 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasBegin = beginRange.size() > 0 && beginRange[0].key.startsWith(mapPrefix);
															#line 189 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value beginValue = hasBegin ? beginRange[0].value : LiteralStringRef("");
															#line 191 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		beginKey = withPrefix.begin;
															#line 192 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (beginValue == value)
															#line 1149 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 193 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			bool outsideRange = !hasBegin || beginRange[0].key < maxWithPrefix.begin;
															#line 194 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			beginKey = outsideRange ? maxWithPrefix.begin : beginRange[0].key;
															#line 1155 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 198 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		auto endRange = keys[1].get();
															#line 199 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasEnd = endRange.size() >= 1 && endRange[0].key.startsWith(mapPrefix) && endRange[0].key <= withPrefix.end;
															#line 200 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasNext = (endRange.size() == 2 && endRange[1].key.startsWith(mapPrefix)) || (endRange.size() == 1 && withPrefix.end < endRange[0].key && endRange[0].key.startsWith(mapPrefix));
															#line 202 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value existingValue = hasEnd ? endRange[0].value : LiteralStringRef("");
															#line 203 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool valueMatches = value == existingValue;
															#line 205 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasBegin ? beginRange[0].key : mapPrefix, withPrefix.begin);
															#line 206 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1171 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 207 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1175 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 209 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		conflictRange = KeyRangeRef(hasEnd ? endRange[0].key : mapPrefix, hasNext ? keyAfter(endRange.end()[-1].key) : strinc(mapPrefix));
															#line 211 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1181 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 212 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1185 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 214 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		endKey = Key();
															#line 215 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		endValue = Value();
															#line 218 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (hasNext && endRange.end()[-1].key <= maxWithPrefix.end && valueMatches)
															#line 1193 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 219 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			endKey = endRange.end()[-1].key;
															#line 220 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			endValue = endRange.end()[-1].value;
															#line 1199 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
		else
		{
															#line 224 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (valueMatches)
															#line 1205 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			{
															#line 225 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endKey = maxWithPrefix.end;
															#line 226 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1211 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			}
			else
			{
															#line 231 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endKey = withPrefix.end;
															#line 232 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1219 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			}
		}
															#line 235 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(KeyRangeRef(beginKey, endKey));
															#line 237 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		ASSERT(value != endValue || endKey == maxWithPrefix.end);
															#line 238 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(beginKey, value);
															#line 239 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(endKey, endValue);
															#line 241 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeCoalescing_ActorState(); static_cast<KrmSetRangeCoalescing_Actor*>(this)->destroy(); return 0; }
															#line 1232 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		new (&static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~KrmSetRangeCoalescing_ActorState();
		static_cast<KrmSetRangeCoalescing_Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 187 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		auto beginRange = keys[0].get();
															#line 188 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasBegin = beginRange.size() > 0 && beginRange[0].key.startsWith(mapPrefix);
															#line 189 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value beginValue = hasBegin ? beginRange[0].value : LiteralStringRef("");
															#line 191 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		beginKey = withPrefix.begin;
															#line 192 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (beginValue == value)
															#line 1252 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 193 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			bool outsideRange = !hasBegin || beginRange[0].key < maxWithPrefix.begin;
															#line 194 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			beginKey = outsideRange ? maxWithPrefix.begin : beginRange[0].key;
															#line 1258 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 198 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		auto endRange = keys[1].get();
															#line 199 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasEnd = endRange.size() >= 1 && endRange[0].key.startsWith(mapPrefix) && endRange[0].key <= withPrefix.end;
															#line 200 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool hasNext = (endRange.size() == 2 && endRange[1].key.startsWith(mapPrefix)) || (endRange.size() == 1 && withPrefix.end < endRange[0].key && endRange[0].key.startsWith(mapPrefix));
															#line 202 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		Value existingValue = hasEnd ? endRange[0].value : LiteralStringRef("");
															#line 203 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		bool valueMatches = value == existingValue;
															#line 205 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		KeyRange conflictRange = KeyRangeRef(hasBegin ? beginRange[0].key : mapPrefix, withPrefix.begin);
															#line 206 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1274 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 207 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1278 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 209 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		conflictRange = KeyRangeRef(hasEnd ? endRange[0].key : mapPrefix, hasNext ? keyAfter(endRange.end()[-1].key) : strinc(mapPrefix));
															#line 211 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!conflictRange.empty())
															#line 1284 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 212 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			tr->addReadConflictRange(conflictRange);
															#line 1288 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
															#line 214 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		endKey = Key();
															#line 215 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		endValue = Value();
															#line 218 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (hasNext && endRange.end()[-1].key <= maxWithPrefix.end && valueMatches)
															#line 1296 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		{
															#line 219 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			endKey = endRange.end()[-1].key;
															#line 220 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			endValue = endRange.end()[-1].value;
															#line 1302 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
		}
		else
		{
															#line 224 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
			if (valueMatches)
															#line 1308 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			{
															#line 225 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endKey = maxWithPrefix.end;
															#line 226 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1314 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			}
			else
			{
															#line 231 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endKey = withPrefix.end;
															#line 232 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
				endValue = existingValue;
															#line 1322 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
			}
		}
															#line 235 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->clear(KeyRangeRef(beginKey, endKey));
															#line 237 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		ASSERT(value != endValue || endKey == maxWithPrefix.end);
															#line 238 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(beginKey, value);
															#line 239 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		tr->set(endKey, endValue);
															#line 241 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
		if (!static_cast<KrmSetRangeCoalescing_Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~KrmSetRangeCoalescing_ActorState(); static_cast<KrmSetRangeCoalescing_Actor*>(this)->destroy(); return 0; }
															#line 1335 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Transaction* tr;
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key mapPrefix;
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange range;
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange maxRange;
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Value value;
															#line 174 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange withPrefix;
															#line 176 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KeyRange maxWithPrefix;
															#line 179 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	std::vector<Future<RangeResult>> keys;
															#line 191 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key beginKey;
															#line 214 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Key endKey;
															#line 215 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	Value endValue;
															#line 1428 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
};
// This generated class is to be used only via krmSetRangeCoalescing_()
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class Transaction>
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
class KrmSetRangeCoalescing_Actor final : public Actor<Void>, public ActorCallback< KrmSetRangeCoalescing_Actor<Transaction>, 0, Void >, public FastAllocated<KrmSetRangeCoalescing_Actor<Transaction>>, public KrmSetRangeCoalescing_ActorState<Transaction, KrmSetRangeCoalescing_Actor<Transaction>> {
															#line 1435 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
public:
	using FastAllocated<KrmSetRangeCoalescing_Actor<Transaction>>::operator new;
	using FastAllocated<KrmSetRangeCoalescing_Actor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< KrmSetRangeCoalescing_Actor<Transaction>, 0, Void >;
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	KrmSetRangeCoalescing_Actor(Transaction* const& tr,Key const& mapPrefix,KeyRange const& range,KeyRange const& maxRange,Value const& value) 
															#line 1446 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
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
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
template <class Transaction>
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
[[nodiscard]] static Future<Void> krmSetRangeCoalescing_( Transaction* const& tr, Key const& mapPrefix, KeyRange const& range, KeyRange const& maxRange, Value const& value ) {
															#line 166 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
	return Future<Void>(new KrmSetRangeCoalescing_Actor<Transaction>(tr, mapPrefix, range, maxRange, value));
															#line 1476 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.g.cpp"
}

#line 243 "/usr/src/libfdb_c/fdbclient/KeyRangeMap.actor.cpp"
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
