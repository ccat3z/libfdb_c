#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
/*
 * KeyBackedRangeMap.actor.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2023 Apple Inc. and the FoundationDB project authors
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

#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_KEYBACKEDRANGEMAP_ACTOR_G_H)
#define FDBCLIENT_KEYBACKEDRANGEMAP_ACTOR_G_H
#include "fdbclient/KeyBackedRangeMap.actor.g.h"
#elif !defined(FDBCLIENT_KEYBACKEDRANGEMAP_ACTOR_H)
#define FDBCLIENT_KEYBACKEDRANGEMAP_ACTOR_H

#include "flow/FastRef.h"
#include "fdbclient/KeyBackedTypes.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

// A local in-memory representation of a KeyBackedRangeMap snapshot
// It is invalid to look up a range in this map which not within the range of
// [first key of snapshot, last key of snapshot)
// This is ReferenceCounted as it can be large and there is no reason to copy it as
// it should not be modified locally.
template <typename KeyType, typename ValueType>
struct KeyRangeMapSnapshot : public ReferenceCounted<KeyRangeMapSnapshot<KeyType, ValueType>> {
	typedef std::map<KeyType, ValueType> Map;

	// A default constructed map snapshot can't be used to look anything up because no ranges are covered.
	KeyRangeMapSnapshot() {}

	// Initialize map with a single range from min to max with a given ValueType
	KeyRangeMapSnapshot(const KeyType& min, const KeyType& max, const ValueType& val = {}) {
		map[min] = val;
		map[max] = val;
	}

	struct RangeValue {
		TypedRange<KeyType> range;
		ValueType value;
	};

	// Iterator for ranges in the map.  Ranges are represented by a key, its value, and the next key in the map.
	struct RangeIter {
		typename Map::const_iterator impl;

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = RangeIter;
		using pointer = RangeIter*;
		using reference = RangeIter&;

		TypedRange<const KeyType&> range() const { return { impl->first, std::next(impl)->first }; }
		const ValueType& value() const { return impl->second; }

		const RangeIter& operator*() const { return *this; }
		const RangeIter* operator->() const { return this; }

		RangeIter operator++(int) { return { impl++ }; }
		RangeIter operator--(int) { return { impl-- }; }
		RangeIter& operator++() {
			++impl;
			return *this;
		}
		RangeIter& operator--() {
			--impl;
			return *this;
		}

		bool operator==(const RangeIter& rhs) const { return impl == rhs.impl; }
		bool operator!=(const RangeIter& rhs) const { return impl != rhs.impl; }
	};

	// Range-for compatible object representing a list of contiguous ranges.
	struct Ranges {
		RangeIter iBegin, iEnd;
		RangeIter begin() const { return iBegin; }
		RangeIter end() const { return iEnd; }
	};

	RangeIter rangeContaining(const KeyType& begin) const {
		ASSERT(map.size() >= 2);
		auto i = map.upper_bound(begin);
		ASSERT(i != map.begin());
		ASSERT(i != map.end());
		return { --i };
	}

	// Get a set of Ranges which cover [begin, end)
	Ranges intersectingRanges(const KeyType& begin, const KeyType& end) const {
		return { rangeContaining(begin), { map.lower_bound(end) } };
	}

	Ranges ranges() const { return { { map.begin() }, { std::prev(map.end()) } }; }

	Map map;
};

// KeyBackedKeyRangeMap is similar to KeyRangeMap but without a Metric
// It is assumed that any range not covered by the map is set to a default ValueType()
// The ValueType must have
//     // Return a copy of *this updated with properties in value
//     ValueType apply(ValueType const& value) const;
//
//     // Return true if the two values are identical in meaning so adjacent ranges using either value can be merged
//     bool operator==(ValueType const& value) const;
// For debug output, KeyType and ValueType must both be supported by fmt::formatter<>
template <typename KeyType,
          typename ValueType,
          typename KeyCodec = TupleCodec<KeyType>,
          typename ValueCodec = TupleCodec<ValueType>>
class KeyBackedRangeMap {
public:
	typedef KeyBackedMap<KeyType, ValueType, KeyCodec, ValueCodec> Map;
	typedef typename Map::KeySelector KeySelector;
	typedef typename Map::RangeResultType RangeResultType;
	typedef KeyRangeMapSnapshot<KeyType, ValueType> LocalSnapshot;
	typedef typename LocalSnapshot::RangeValue RangeValue;

	KeyBackedRangeMap(KeyRef prefix = invalidKey, Optional<WatchableTrigger> trigger = {}, ValueCodec valueCodec = {})
	  : kvMap(prefix, trigger, valueCodec) {}

	// Get the RangeValue for the range that contains key, if there is a begin and end in the map which contain key
																#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
// This generated class is to be used only via getRangeForKey()
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction, class GetRangeForKeyActor>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
class GetRangeForKeyActorState {
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
public:
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	GetRangeForKeyActorState(KeyBackedRangeMap const& self,Transaction const& tr,KeyType const& key,Snapshot const& snapshot = Snapshot::False) 
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		 : self(self),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   tr(tr),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   key(key),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   snapshot(snapshot),
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   begin(self.kvMap.seekLessOrEqual(tr, key, snapshot)),
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   end(self.kvMap.seekGreaterThan(tr, key, snapshot))
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
	{
		fdb_probe_actor_create("getRangeForKey", reinterpret_cast<unsigned long>(this));

	}
	~GetRangeForKeyActorState() 
	{
		fdb_probe_actor_destroy("getRangeForKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			StrictFuture<Void> __when_expr_0 = success(begin) && success(end);
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (static_cast<GetRangeForKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetRangeForKeyActor*>(this)->actor_wait_state = 1;
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRangeForKeyActor, 0, Void >*>(static_cast<GetRangeForKeyActor*>(this)));
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
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
		this->~GetRangeForKeyActorState();
		static_cast<GetRangeForKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (begin.get().present() && end.get().present())
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (!static_cast<GetRangeForKeyActor*>(this)->SAV<Optional<RangeValue>>::futures) { (void)(RangeValue{ { begin.get()->key, end.get()->key }, begin.get()->value }); this->~GetRangeForKeyActorState(); static_cast<GetRangeForKeyActor*>(this)->destroy(); return 0; }
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			new (&static_cast<GetRangeForKeyActor*>(this)->SAV< Optional<RangeValue> >::value()) Optional<RangeValue>(RangeValue{ { begin.get()->key, end.get()->key }, begin.get()->value });
			this->~GetRangeForKeyActorState();
			static_cast<GetRangeForKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!static_cast<GetRangeForKeyActor*>(this)->SAV<Optional<RangeValue>>::futures) { (void)(Optional<RangeValue>()); this->~GetRangeForKeyActorState(); static_cast<GetRangeForKeyActor*>(this)->destroy(); return 0; }
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		new (&static_cast<GetRangeForKeyActor*>(this)->SAV< Optional<RangeValue> >::value()) Optional<RangeValue>(Optional<RangeValue>());
		this->~GetRangeForKeyActorState();
		static_cast<GetRangeForKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (begin.get().present() && end.get().present())
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (!static_cast<GetRangeForKeyActor*>(this)->SAV<Optional<RangeValue>>::futures) { (void)(RangeValue{ { begin.get()->key, end.get()->key }, begin.get()->value }); this->~GetRangeForKeyActorState(); static_cast<GetRangeForKeyActor*>(this)->destroy(); return 0; }
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			new (&static_cast<GetRangeForKeyActor*>(this)->SAV< Optional<RangeValue> >::value()) Optional<RangeValue>(RangeValue{ { begin.get()->key, end.get()->key }, begin.get()->value });
			this->~GetRangeForKeyActorState();
			static_cast<GetRangeForKeyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!static_cast<GetRangeForKeyActor*>(this)->SAV<Optional<RangeValue>>::futures) { (void)(Optional<RangeValue>()); this->~GetRangeForKeyActorState(); static_cast<GetRangeForKeyActor*>(this)->destroy(); return 0; }
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		new (&static_cast<GetRangeForKeyActor*>(this)->SAV< Optional<RangeValue> >::value()) Optional<RangeValue>(Optional<RangeValue>());
		this->~GetRangeForKeyActorState();
		static_cast<GetRangeForKeyActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetRangeForKeyActor*>(this)->actor_wait_state > 0) static_cast<GetRangeForKeyActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeForKeyActor*>(this)->ActorCallback< GetRangeForKeyActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeForKeyActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRangeForKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeForKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRangeForKeyActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRangeForKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeForKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRangeForKeyActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeForKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeForKey", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyBackedRangeMap self;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Transaction tr;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyType key;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Snapshot snapshot;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Future<Optional<typename Map::KVType>> begin;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Future<Optional<typename Map::KVType>> end;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
};
// This generated class is to be used only via getRangeForKey()
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
class GetRangeForKeyActor final : public Actor<Optional<RangeValue>>, public ActorCallback< GetRangeForKeyActor<Transaction>, 0, Void >, public FastAllocated<GetRangeForKeyActor<Transaction>>, public GetRangeForKeyActorState<Transaction, GetRangeForKeyActor<Transaction>> {
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
public:
	using FastAllocated<GetRangeForKeyActor<Transaction>>::operator new;
	using FastAllocated<GetRangeForKeyActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<RangeValue>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRangeForKeyActor<Transaction>, 0, Void >;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	GetRangeForKeyActor(KeyBackedRangeMap const& self,Transaction const& tr,KeyType const& key,Snapshot const& snapshot = Snapshot::False) 
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		 : Actor<Optional<RangeValue>>(),
		   GetRangeForKeyActorState<Transaction, GetRangeForKeyActor<Transaction>>(self, tr, key, snapshot)
	{
		fdb_probe_actor_enter("getRangeForKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRangeForKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRangeForKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRangeForKeyActor<Transaction>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
[[nodiscard]] static Future<Optional<RangeValue>> getRangeForKey( KeyBackedRangeMap const& self, Transaction const& tr, KeyType const& key, Snapshot const& snapshot = Snapshot::False ) {
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	return Future<Optional<RangeValue>>(new GetRangeForKeyActor<Transaction>(self, tr, key, snapshot));
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
}

#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"

	template <class Transaction>
	Future<Optional<RangeValue>> getRangeForKey(Transaction tr,
	                                            KeyType const& key,
	                                            Snapshot snapshot = Snapshot::False) const {
		return getRangeForKey(*this, tr, key, snapshot);
	}

	// Update the range from begin to end by either applying valueUpdate to it, or if replace is true then replace
	// the the range with the given value.
	// Adjacent ranges that are identical will be coalesced in the update transaction.
	// Since the transaction type may not be RYW, this method must take care to not rely on reading its own updates.
																#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
// This generated class is to be used only via updateRangeActor()
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction, class UpdateRangeActorActor>
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
class UpdateRangeActorActorState {
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
public:
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	UpdateRangeActorActorState(KeyBackedRangeMap const& self,Transaction const& tr,KeyType const& begin,KeyType const& end,ValueType const& valueUpdate,bool const& replace) 
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		 : self(self),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   tr(tr),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   begin(begin),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   end(end),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   valueUpdate(valueUpdate),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   replace(replace)
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
	{
		fdb_probe_actor_create("updateRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~UpdateRangeActorActorState() 
	{
		fdb_probe_actor_destroy("updateRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			kbt_debug("RANGEMAP updateRange start {} to {} value {}\n", begin, end, valueUpdate);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			StrictFuture<Optional<typename Map::KVType>> __when_expr_0 = self.kvMap.seekLessThan(tr, begin);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (static_cast<UpdateRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateRangeActorActor*>(this)->actor_wait_state = 1;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateRangeActorActor, 0, Optional<typename Map::KVType> >*>(static_cast<UpdateRangeActorActor*>(this)));
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
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
		this->~UpdateRangeActorActorState();
		static_cast<UpdateRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<typename Map::KVType> const& beginKV,int loopDepth) 
	{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterOrEqual(beginKV.present() ? beginKV->key : begin);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeEnd = KeySelector::firstGreaterThan(end);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		readSize = BUGGIFY ? 1 : 100000;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		original = ValueType();
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		previous = Optional<ValueType>();
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		beginDone = false;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		endFound = false;
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		;
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Optional<typename Map::KVType> && beginKV,int loopDepth) 
	{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterOrEqual(beginKV.present() ? beginKV->key : begin);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeEnd = KeySelector::firstGreaterThan(end);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		readSize = BUGGIFY ? 1 : 100000;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		original = ValueType();
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		previous = Optional<ValueType>();
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		beginDone = false;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		endFound = false;
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		;
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<typename Map::KVType> const& beginKV,int loopDepth) 
	{
		loopDepth = a_body1cont1(beginKV, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<typename Map::KVType> && beginKV,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(beginKV), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UpdateRangeActorActor*>(this)->actor_wait_state > 0) static_cast<UpdateRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<UpdateRangeActorActor*>(this)->ActorCallback< UpdateRangeActorActor, 0, Optional<typename Map::KVType> >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateRangeActorActor, 0, Optional<typename Map::KVType> >*,Optional<typename Map::KVType> const& value) 
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UpdateRangeActorActor, 0, Optional<typename Map::KVType> >*,Optional<typename Map::KVType> && value) 
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UpdateRangeActorActor, 0, Optional<typename Map::KVType> >*,Error err) 
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		kbt_debug("RANGEMAP updateRange beginDone {} endFound {}  previous {}  default {}\n", beginDone, endFound, previous, ValueType());
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!beginDone)
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			ValueType val = replace ? valueUpdate : previous.orDefault(ValueType()).apply(valueUpdate);
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (previous != val)
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				kbt_debug("RANGEMAP updateRange set begin\n");
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				self.kvMap.set(tr, begin, valueUpdate);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				previous = val;
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			}
		}
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!endFound && previous != original)
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			kbt_debug("RANGEMAP updateRange set end\n");
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			self.kvMap.set(tr, end, original);
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		}
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!static_cast<UpdateRangeActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateRangeActorActorState(); static_cast<UpdateRangeActorActor*>(this)->destroy(); return 0; }
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		new (&static_cast<UpdateRangeActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateRangeActorActorState();
		static_cast<UpdateRangeActorActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		kbt_debug("RANGEMAP updateRange loop\n");
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		StrictFuture<RangeResultType> __when_expr_1 = boundariesFuture;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (static_cast<UpdateRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<UpdateRangeActorActor*>(this)->actor_wait_state = 2;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UpdateRangeActorActor, 1, RangeResultType >*>(static_cast<UpdateRangeActorActor*>(this)));
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
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
	int a_body1cont1loopBody1cont1(RangeResultType const& boundaries,int loopDepth) 
	{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		for( auto const& bv : boundaries.results ) {
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			kbt_debug("RANGEMAP updateRange   result key={} value={}\n", bv.first, bv.second);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			ASSERT(!endFound);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			original = bv.second;
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (bv.first > begin)
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				if (!beginDone)
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange !beginDone\n");
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					ValueType val = replace ? valueUpdate : previous.orDefault(ValueType()).apply(valueUpdate);
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (previous != val)
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.set(tr, begin, val);
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						previous = val;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					beginDone = true;
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				}
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				if (bv.first < end)
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				{
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange Case C\n");
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					ValueType val = replace ? valueUpdate : bv.second.apply(valueUpdate);
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (previous == val)
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.erase(tr, bv.first);
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
					else
					{
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.set(tr, bv.first, val);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						previous = val;
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
				}
				else
				{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (bv.first == end)
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						kbt_debug("RANGEMAP updateRange Case D\n");
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						if (previous == bv.second)
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
						{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
							self.kvMap.erase(tr, end);
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
						}
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						endFound = true;
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
				}
			}
			else
			{
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				if (bv.first == begin)
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange Case B\n");
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					ValueType val = replace ? valueUpdate : bv.second.apply(valueUpdate);
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (previous == val)
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.erase(tr, begin);
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
					else
					{
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.set(tr, begin, val);
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					beginDone = true;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					previous = val;
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				}
				else
				{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange Case A\n");
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					previous = bv.second;
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				}
			}
		}
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!boundaries.more)
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		ASSERT(!boundaries.results.empty());
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterThan(boundaries.results.back().first);
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(RangeResultType && boundaries,int loopDepth) 
	{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		for( auto const& bv : boundaries.results ) {
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			kbt_debug("RANGEMAP updateRange   result key={} value={}\n", bv.first, bv.second);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			ASSERT(!endFound);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			original = bv.second;
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (bv.first > begin)
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				if (!beginDone)
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange !beginDone\n");
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					ValueType val = replace ? valueUpdate : previous.orDefault(ValueType()).apply(valueUpdate);
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (previous != val)
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.set(tr, begin, val);
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						previous = val;
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					beginDone = true;
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				}
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				if (bv.first < end)
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				{
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange Case C\n");
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					ValueType val = replace ? valueUpdate : bv.second.apply(valueUpdate);
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (previous == val)
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.erase(tr, bv.first);
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
					else
					{
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.set(tr, bv.first, val);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						previous = val;
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
				}
				else
				{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (bv.first == end)
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						kbt_debug("RANGEMAP updateRange Case D\n");
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						if (previous == bv.second)
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
						{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
							self.kvMap.erase(tr, end);
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
						}
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						endFound = true;
															#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
				}
			}
			else
			{
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
				if (bv.first == begin)
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange Case B\n");
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					ValueType val = replace ? valueUpdate : bv.second.apply(valueUpdate);
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					if (previous == val)
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					{
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.erase(tr, begin);
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
					else
					{
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
						self.kvMap.set(tr, begin, val);
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
					}
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					beginDone = true;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					previous = val;
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				}
				else
				{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					kbt_debug("RANGEMAP updateRange Case A\n");
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
					previous = bv.second;
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
				}
			}
		}
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!boundaries.more)
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		ASSERT(!boundaries.results.empty());
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterThan(boundaries.results.back().first);
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(RangeResultType const& boundaries,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(boundaries, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(RangeResultType && boundaries,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(boundaries), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<UpdateRangeActorActor*>(this)->actor_wait_state > 0) static_cast<UpdateRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<UpdateRangeActorActor*>(this)->ActorCallback< UpdateRangeActorActor, 1, RangeResultType >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateRangeActorActor, 1, RangeResultType >*,RangeResultType const& value) 
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< UpdateRangeActorActor, 1, RangeResultType >*,RangeResultType && value) 
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< UpdateRangeActorActor, 1, RangeResultType >*,Error err) 
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyBackedRangeMap self;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Transaction tr;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyType begin;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyType end;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	ValueType valueUpdate;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	bool replace;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeySelector rangeBegin;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeySelector rangeEnd;
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	int readSize;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Future<RangeResultType> boundariesFuture;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	ValueType original;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Optional<ValueType> previous;
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	bool beginDone;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	bool endFound;
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
};
// This generated class is to be used only via updateRangeActor()
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction>
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
class UpdateRangeActorActor final : public Actor<Void>, public ActorCallback< UpdateRangeActorActor<Transaction>, 0, Optional<typename Map::KVType> >, public ActorCallback< UpdateRangeActorActor<Transaction>, 1, RangeResultType >, public FastAllocated<UpdateRangeActorActor<Transaction>>, public UpdateRangeActorActorState<Transaction, UpdateRangeActorActor<Transaction>> {
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
public:
	using FastAllocated<UpdateRangeActorActor<Transaction>>::operator new;
	using FastAllocated<UpdateRangeActorActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateRangeActorActor<Transaction>, 0, Optional<typename Map::KVType> >;
friend struct ActorCallback< UpdateRangeActorActor<Transaction>, 1, RangeResultType >;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	UpdateRangeActorActor(KeyBackedRangeMap const& self,Transaction const& tr,KeyType const& begin,KeyType const& end,ValueType const& valueUpdate,bool const& replace) 
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		 : Actor<Void>(),
		   UpdateRangeActorActorState<Transaction, UpdateRangeActorActor<Transaction>>(self, tr, begin, end, valueUpdate, replace)
	{
		fdb_probe_actor_enter("updateRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updateRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updateRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UpdateRangeActorActor<Transaction>, 0, Optional<typename Map::KVType> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< UpdateRangeActorActor<Transaction>, 1, RangeResultType >*)0, actor_cancelled()); break;
		}

	}
};
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction>
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
[[nodiscard]] static Future<Void> updateRangeActor( KeyBackedRangeMap const& self, Transaction const& tr, KeyType const& begin, KeyType const& end, ValueType const& valueUpdate, bool const& replace ) {
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	return Future<Void>(new UpdateRangeActorActor<Transaction>(self, tr, begin, end, valueUpdate, replace));
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
}

#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"

	template <class Transaction>
	Future<Void> updateRange(Transaction tr,
	                         KeyType const& begin,
	                         KeyType const& end,
	                         ValueType const& valueUpdate,
	                         bool replace = false) const {
		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				return self.updateRange(tr, begin, end, valueUpdate, replace);
			});
		} else {
			return updateRangeActor(*this, tr, begin, end, valueUpdate, replace);
		}
	}

																#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
// This generated class is to be used only via getSnapshotActor()
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction, class GetSnapshotActorActor>
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
class GetSnapshotActorActorState {
															#line 1093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
public:
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	GetSnapshotActorActorState(KeyBackedRangeMap const& self,Transaction const& tr,KeyType const& begin,KeyType const& end) 
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		 : self(self),
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   tr(tr),
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   begin(begin),
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		   end(end)
															#line 1106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
	{
		fdb_probe_actor_create("getSnapshotActor", reinterpret_cast<unsigned long>(this));

	}
	~GetSnapshotActorActorState() 
	{
		fdb_probe_actor_destroy("getSnapshotActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			kbt_debug("RANGEMAP snapshot start\n");
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			StrictFuture<Optional<typename Map::KVType>> __when_expr_0 = self.kvMap.seekLessOrEqual(tr, begin);
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			if (static_cast<GetSnapshotActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetSnapshotActorActor*>(this)->actor_wait_state = 1;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotActorActor, 0, Optional<typename Map::KVType> >*>(static_cast<GetSnapshotActorActor*>(this)));
															#line 1130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
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
		this->~GetSnapshotActorActorState();
		static_cast<GetSnapshotActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<typename Map::KVType> const& beginKV,int loopDepth) 
	{
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterOrEqual(beginKV.present() ? beginKV->key : begin);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeEnd = KeySelector::firstGreaterThan(end);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		readSize = BUGGIFY ? 1 : 100000;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		result = makeReference<LocalSnapshot>();
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		;
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Optional<typename Map::KVType> && beginKV,int loopDepth) 
	{
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterOrEqual(beginKV.present() ? beginKV->key : begin);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeEnd = KeySelector::firstGreaterThan(end);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		readSize = BUGGIFY ? 1 : 100000;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		result = makeReference<LocalSnapshot>();
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		;
															#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<typename Map::KVType> const& beginKV,int loopDepth) 
	{
		loopDepth = a_body1cont1(beginKV, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<typename Map::KVType> && beginKV,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(beginKV), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetSnapshotActorActor*>(this)->actor_wait_state > 0) static_cast<GetSnapshotActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetSnapshotActorActor*>(this)->ActorCallback< GetSnapshotActorActor, 0, Optional<typename Map::KVType> >::remove();

	}
	void a_callback_fire(ActorCallback< GetSnapshotActorActor, 0, Optional<typename Map::KVType> >*,Optional<typename Map::KVType> const& value) 
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetSnapshotActorActor, 0, Optional<typename Map::KVType> >*,Optional<typename Map::KVType> && value) 
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetSnapshotActorActor, 0, Optional<typename Map::KVType> >*,Error err) 
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (result->map.empty() || result->map.begin()->first > begin)
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			result->map[begin] = ValueType();
															#line 1258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		}
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (result->map.rbegin()->first < end)
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			result->map[end] = ValueType();
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		}
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		kbt_debug("RANGEMAP snapshot end\n");
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!static_cast<GetSnapshotActorActor*>(this)->SAV<Reference<LocalSnapshot>>::futures) { (void)(result); this->~GetSnapshotActorActorState(); static_cast<GetSnapshotActorActor*>(this)->destroy(); return 0; }
															#line 1272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		new (&static_cast<GetSnapshotActorActor*>(this)->SAV< Reference<LocalSnapshot> >::value()) Reference<LocalSnapshot>(std::move(result)); // state_var_RVO
		this->~GetSnapshotActorActorState();
		static_cast<GetSnapshotActorActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		kbt_debug("RANGEMAP snapshot loop\n");
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		StrictFuture<RangeResultType> __when_expr_1 = boundariesFuture;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (static_cast<GetSnapshotActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetSnapshotActorActor*>(this)->actor_wait_state = 2;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotActorActor, 1, RangeResultType >*>(static_cast<GetSnapshotActorActor*>(this)));
															#line 1300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
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
	int a_body1cont1loopBody1cont1(RangeResultType const& boundaries,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		for( auto const& bv : boundaries.results ) {
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			result->map[bv.first] = bv.second;
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		}
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!boundaries.more)
															#line 1328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		ASSERT(!boundaries.results.empty());
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterThan(boundaries.results.back().first);
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(RangeResultType && boundaries,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		for( auto const& bv : boundaries.results ) {
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
			result->map[bv.first] = bv.second;
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		}
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		if (!boundaries.more)
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		ASSERT(!boundaries.results.empty());
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		rangeBegin = KeySelector::firstGreaterThan(boundaries.results.back().first);
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
		boundariesFuture = self.kvMap.getRange(tr, rangeBegin, rangeEnd, GetRangeLimits(readSize));
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(RangeResultType const& boundaries,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(boundaries, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(RangeResultType && boundaries,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(boundaries), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetSnapshotActorActor*>(this)->actor_wait_state > 0) static_cast<GetSnapshotActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetSnapshotActorActor*>(this)->ActorCallback< GetSnapshotActorActor, 1, RangeResultType >::remove();

	}
	void a_callback_fire(ActorCallback< GetSnapshotActorActor, 1, RangeResultType >*,RangeResultType const& value) 
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetSnapshotActorActor, 1, RangeResultType >*,RangeResultType && value) 
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetSnapshotActorActor, 1, RangeResultType >*,Error err) 
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyBackedRangeMap self;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Transaction tr;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyType begin;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeyType end;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeySelector rangeBegin;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	KeySelector rangeEnd;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	int readSize;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Future<RangeResultType> boundariesFuture;
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	Reference<LocalSnapshot> result;
															#line 1449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
};
// This generated class is to be used only via getSnapshotActor()
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction>
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
class GetSnapshotActorActor final : public Actor<Reference<LocalSnapshot>>, public ActorCallback< GetSnapshotActorActor<Transaction>, 0, Optional<typename Map::KVType> >, public ActorCallback< GetSnapshotActorActor<Transaction>, 1, RangeResultType >, public FastAllocated<GetSnapshotActorActor<Transaction>>, public GetSnapshotActorActorState<Transaction, GetSnapshotActorActor<Transaction>> {
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
public:
	using FastAllocated<GetSnapshotActorActor<Transaction>>::operator new;
	using FastAllocated<GetSnapshotActorActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<LocalSnapshot>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetSnapshotActorActor<Transaction>, 0, Optional<typename Map::KVType> >;
friend struct ActorCallback< GetSnapshotActorActor<Transaction>, 1, RangeResultType >;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	GetSnapshotActorActor(KeyBackedRangeMap const& self,Transaction const& tr,KeyType const& begin,KeyType const& end) 
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
		 : Actor<Reference<LocalSnapshot>>(),
		   GetSnapshotActorActorState<Transaction, GetSnapshotActorActor<Transaction>>(self, tr, begin, end)
	{
		fdb_probe_actor_enter("getSnapshotActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getSnapshotActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getSnapshotActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetSnapshotActorActor<Transaction>, 0, Optional<typename Map::KVType> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetSnapshotActorActor<Transaction>, 1, RangeResultType >*)0, actor_cancelled()); break;
		}

	}
};
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
template <class Transaction>
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
[[nodiscard]] static Future<Reference<LocalSnapshot>> getSnapshotActor( KeyBackedRangeMap const& self, Transaction const& tr, KeyType const& begin, KeyType const& end ) {
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"
	return Future<Reference<LocalSnapshot>>(new GetSnapshotActorActor<Transaction>(self, tr, begin, end));
															#line 1498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.g.h"
}

#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedRangeMap.actor.h"

	// Return a LocalSnapshot of all ranges from the map which cover the range of begin through end.
	// If the map in the database does not have boundaries <=begin or >=end then these boundaries will be
	// added to the returned snapshot with a default ValueType.
	template <class Transaction>
	Future<Reference<LocalSnapshot>> getSnapshot(Transaction tr, KeyType const& begin, KeyType const& end) const {
		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				return self.getSnapshot(tr, begin, end);
			});
		} else {
			return getSnapshotActor(*this, tr, begin, end);
		}
	}

private:
	Map kvMap;
};

#include "flow/unactorcompiler.h"

#endif
