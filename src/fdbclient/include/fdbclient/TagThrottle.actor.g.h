#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
/*
 * TagThrottle.actor.h
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

#include "flow/Arena.h"
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_TAG_THROTTLE_ACTOR_G_H)
#define FDBCLIENT_TAG_THROTTLE_ACTOR_G_H
#include "fdbclient/TagThrottle.actor.g.h"
#elif !defined(FDBCLIENT_TAG_THROTTLE_ACTOR_H)
#define FDBCLIENT_TAG_THROTTLE_ACTOR_H

#pragma once

#include "fmt/format.h"
#include "flow/Error.h"
#include "flow/flow.h"
#include "flow/network.h"
#include "flow/ThreadHelper.actor.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/CommitTransaction.h"
#include "flow/actorcompiler.h" // This must be the last #include.

typedef StringRef TransactionTagRef;
typedef Standalone<TransactionTagRef> TransactionTag;

FDB_BOOLEAN_PARAM(ContainsRecommended);
FDB_BOOLEAN_PARAM(Capitalize);

class TagSet {
public:
	typedef std::vector<TransactionTagRef>::const_iterator const_iterator;

	TagSet() : bytes(0) {}

	void addTag(TransactionTagRef tag);
	size_t size() const;

	const_iterator begin() const { return tags.begin(); }

	const_iterator end() const { return tags.end(); }

	void clear() {
		tags.clear();
		bytes = 0;
	}

	template <class Context>
	void save(uint8_t* out, Context&) const {
		uint8_t* start = out;
		for (const auto& tag : *this) {
			*(out++) = (uint8_t)tag.size();

			std::copy(tag.begin(), tag.end(), out);
			out += tag.size();
		}

		ASSERT((size_t)(out - start) == size() + bytes);
	}

	template <class Context>
	void load(const uint8_t* data, size_t size, Context& context) {
		// const uint8_t *start = data;
		const uint8_t* end = data + size;
		while (data < end) {
			uint8_t len = *(data++);
			// Tags are already deduplicated
			const auto& tag = tags.emplace_back(context.tryReadZeroCopy(data, len), len);
			data += len;
			bytes += tag.size();
		}

		ASSERT(data == end);

		// Deserialized tag sets share the arena with the request that contained them
		// For this reason, persisting a TagSet that shares memory with other request
		// members should be done with caution.
		arena = context.arena();
	}

	size_t getBytes() const { return bytes; }

	const Arena& getArena() const { return arena; }

	// Used by fdbcli commands
	std::string toString(Capitalize = Capitalize::False) const;

private:
	size_t bytes;
	Arena arena;
	// Currently there are never >= 256 tags, so
	// std::vector is faster than std::set. This may
	// change if we allow more tags in the future.
	std::vector<TransactionTagRef> tags;
};

template <>
struct dynamic_size_traits<TagSet> : std::true_type {
	// May be called multiple times during one serialization
	template <class Context>
	static size_t size(const TagSet& t, Context&) {
		return t.size() + t.getBytes();
	}

	// Guaranteed to be called only once during serialization
	template <class Context>
	static void save(uint8_t* out, const TagSet& t, Context& c) {
		t.save(out, c);
	}

	// Context is an arbitrary type that is plumbed by reference throughout the
	// load call tree.
	template <class Context>
	static void load(const uint8_t* data, size_t size, TagSet& t, Context& context) {
		t.load(data, size, context);
	}
};

enum class TagThrottleType : uint8_t { MANUAL, AUTO };

enum class TagThrottledReason : uint8_t { UNSET = 0, MANUAL, BUSY_READ, BUSY_WRITE };

struct TagThrottleKey {
	TagSet tags;
	TagThrottleType throttleType;
	TransactionPriority priority;

	TagThrottleKey() : throttleType(TagThrottleType::MANUAL), priority(TransactionPriority::DEFAULT) {}
	TagThrottleKey(TagSet tags, TagThrottleType throttleType, TransactionPriority priority)
	  : tags(tags), throttleType(throttleType), priority(priority) {}

	Key toKey() const;
	static TagThrottleKey fromKey(const KeyRef& key);
};

struct TagThrottleValue {
	double tpsRate;
	double expirationTime;
	double initialDuration;
	TagThrottledReason reason;

	TagThrottleValue() : tpsRate(0), expirationTime(0), initialDuration(0), reason(TagThrottledReason::UNSET) {}
	TagThrottleValue(double tpsRate, double expirationTime, double initialDuration, TagThrottledReason reason)
	  : tpsRate(tpsRate), expirationTime(expirationTime), initialDuration(initialDuration), reason(reason) {}

	static TagThrottleValue fromValue(const ValueRef& value);

	// To change this serialization, ProtocolVersion::TagThrottleValue must be updated, and downgrades need to be
	// considered
	template <class Ar>
	void serialize(Ar& ar) {
		if (ar.protocolVersion().hasTagThrottleValueReason()) {
			serializer(ar, tpsRate, expirationTime, initialDuration, reason);
		} else if (ar.protocolVersion().hasTagThrottleValue()) {
			serializer(ar, tpsRate, expirationTime, initialDuration);
			if (ar.isDeserializing) {
				reason = TagThrottledReason::UNSET;
			}
		}
	}
};

struct TagThrottleInfo {
	TransactionTag tag;
	TagThrottleType throttleType;
	TransactionPriority priority;
	double tpsRate;
	double expirationTime;
	double initialDuration;
	TagThrottledReason reason;

	TagThrottleInfo(TransactionTag tag,
	                TagThrottleType throttleType,
	                TransactionPriority priority,
	                double tpsRate,
	                double expirationTime,
	                double initialDuration,
	                TagThrottledReason reason = TagThrottledReason::UNSET)
	  : tag(tag), throttleType(throttleType), priority(priority), tpsRate(tpsRate), expirationTime(expirationTime),
	    initialDuration(initialDuration), reason(reason) {}

	TagThrottleInfo(TagThrottleKey key, TagThrottleValue value)
	  : throttleType(key.throttleType), priority(key.priority), tpsRate(value.tpsRate),
	    expirationTime(value.expirationTime), initialDuration(value.initialDuration), reason(value.reason) {
		ASSERT(key.tags.size() == 1); // Multiple tags per throttle is not currently supported
		tag = *key.tags.begin();
	}
};

struct ClientTagThrottleLimits {
	double tpsRate;
	double expiration;

	static double const NO_EXPIRATION;

	ClientTagThrottleLimits() : tpsRate(0), expiration(0) {}
	ClientTagThrottleLimits(double tpsRate, double expiration) : tpsRate(tpsRate), expiration(expiration) {}

	template <class Archive>
	void serialize(Archive& ar) {
		// Convert expiration time to a duration to avoid clock differences
		double duration = 0;
		if (!ar.isDeserializing) {
			duration = expiration - now();
		}

		serializer(ar, tpsRate, duration);

		if (ar.isDeserializing) {
			expiration = now() + duration;
		}
	}
};

struct ClientTrCommitCostEstimation {
	int opsCount = 0;
	uint64_t writeCosts = 0;
	std::deque<std::pair<int, uint64_t>> clearIdxCosts;
	uint32_t expensiveCostEstCount = 0;
	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, opsCount, writeCosts, clearIdxCosts, expensiveCostEstCount);
	}
};

// Keys to view and control tag throttling
extern const KeyRangeRef tagThrottleKeys;
extern const KeyRef tagThrottleKeysPrefix;
extern const KeyRef tagThrottleAutoKeysPrefix;
extern const KeyRef tagThrottleSignalKey;
extern const KeyRef tagThrottleAutoEnabledKey;
extern const KeyRef tagThrottleLimitKey;
extern const KeyRef tagThrottleCountKey;

namespace ThrottleApi {

// The template functions can be called with Native API like DatabaseContext, Transaction/ReadYourWritesTransaction
// or using IClientAPI like IDatabase, ITransaction

															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via getValidAutoEnabled()
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class Tr, class GetValidAutoEnabledActor>
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class GetValidAutoEnabledActorState {
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	GetValidAutoEnabledActorState(Reference<Tr> const& tr) 
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : tr(tr),
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   valueF(tr->get(tagThrottleAutoEnabledKey))
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("getValidAutoEnabled", reinterpret_cast<unsigned long>(this));

	}
	~GetValidAutoEnabledActorState() 
	{
		fdb_probe_actor_destroy("getValidAutoEnabled", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(valueF);
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 1;
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetValidAutoEnabledActor, 0, Optional<Value> >*>(static_cast<GetValidAutoEnabledActor*>(this)));
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~GetValidAutoEnabledActorState();
		static_cast<GetValidAutoEnabledActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& value,int loopDepth) 
	{
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!value.present())
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)({}); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>({});
			this->~GetValidAutoEnabledActorState();
			static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (value.get() == "1"_sr)
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)(true); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>(true);
				this->~GetValidAutoEnabledActorState();
				static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (value.get() == "0"_sr)
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				{
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)(false); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>(false);
					this->~GetValidAutoEnabledActorState();
					static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					TraceEvent(SevWarnAlways, "InvalidAutoTagThrottlingValue").detail("Value", value.get());
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)({}); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>({});
					this->~GetValidAutoEnabledActorState();
					static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
		}

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && value,int loopDepth) 
	{
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!value.present())
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)({}); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>({});
			this->~GetValidAutoEnabledActorState();
			static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (value.get() == "1"_sr)
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)(true); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>(true);
				this->~GetValidAutoEnabledActorState();
				static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (value.get() == "0"_sr)
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				{
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)(false); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>(false);
					this->~GetValidAutoEnabledActorState();
					static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					TraceEvent(SevWarnAlways, "InvalidAutoTagThrottlingValue").detail("Value", value.get());
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<Optional<bool>>::futures) { (void)({}); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< Optional<bool> >::value()) Optional<bool>({});
					this->~GetValidAutoEnabledActorState();
					static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
		}

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& value,int loopDepth) 
	{
		loopDepth = a_body1cont1(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && value,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state > 0) static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 0;
		static_cast<GetValidAutoEnabledActor*>(this)->ActorCallback< GetValidAutoEnabledActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetValidAutoEnabledActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetValidAutoEnabledActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetValidAutoEnabledActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<Tr> tr;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename Tr::template FutureT<Optional<Value>> valueF;
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via getValidAutoEnabled()
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class GetValidAutoEnabledActor final : public Actor<Optional<bool>>, public ActorCallback< GetValidAutoEnabledActor<Tr>, 0, Optional<Value> >, public FastAllocated<GetValidAutoEnabledActor<Tr>>, public GetValidAutoEnabledActorState<Tr, GetValidAutoEnabledActor<Tr>> {
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<GetValidAutoEnabledActor<Tr>>::operator new;
	using FastAllocated<GetValidAutoEnabledActor<Tr>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<bool>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetValidAutoEnabledActor<Tr>, 0, Optional<Value> >;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	GetValidAutoEnabledActor(Reference<Tr> const& tr) 
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<Optional<bool>>(),
		   GetValidAutoEnabledActorState<Tr, GetValidAutoEnabledActor<Tr>>(tr)
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getValidAutoEnabled");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetValidAutoEnabledActor<Tr>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Optional<bool>> getValidAutoEnabled( Reference<Tr> const& tr ) {
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<Optional<bool>>(new GetValidAutoEnabledActor<Tr>(tr));
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via getRecommendedTags()
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB, class GetRecommendedTagsActor>
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class GetRecommendedTagsActorState {
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	GetRecommendedTagsActorState(Reference<DB> const& db,int const& limit) 
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   limit(limit),
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction())
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("getRecommendedTags", reinterpret_cast<unsigned long>(this));

	}
	~GetRecommendedTagsActorState() 
	{
		fdb_probe_actor_destroy("getRecommendedTags", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			;
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~GetRecommendedTagsActorState();
		static_cast<GetRecommendedTagsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Optional<bool>> __when_expr_0 = getValidAutoEnabled(tr);
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 1;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 0, Optional<bool> >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 4;
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 3, Void >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<bool> const& enableAuto,int loopDepth) 
	{
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!enableAuto.present())
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->reset();
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 2;
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 1, Void >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (enableAuto.get())
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(std::vector<TagThrottleInfo>()); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(std::vector<TagThrottleInfo>());
				this->~GetRecommendedTagsActorState();
				static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<bool> && enableAuto,int loopDepth) 
	{
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!enableAuto.present())
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->reset();
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 2;
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 1, Void >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (enableAuto.get())
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(std::vector<TagThrottleInfo>()); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(std::vector<TagThrottleInfo>());
				this->~GetRecommendedTagsActorState();
				static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<bool> const& enableAuto,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(enableAuto, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<bool> && enableAuto,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(enableAuto), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 0, Optional<bool> >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 0, Optional<bool> >*,Optional<bool> const& value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 0, Optional<bool> >*,Optional<bool> && value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 0, Optional<bool> >*,Error err) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		f = tr->getRange(KeyRangeRef(tagThrottleAutoKeysPrefix, tagThrottleKeys.end), limit);
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		StrictFuture<RangeResult> __when_expr_2 = safeThreadFutureToFuture(f);
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 3;
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 2, RangeResult >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont8(RangeResult const& throttles,int loopDepth) 
	{
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetRecommendedTagsActorState();
		static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont8(RangeResult && throttles,int loopDepth) 
	{
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetRecommendedTagsActorState();
		static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(RangeResult const& throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(throttles, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(RangeResult && throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(throttles), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	int limit;
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> f;
															#line 1083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via getRecommendedTags()
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class GetRecommendedTagsActor final : public Actor<std::vector<TagThrottleInfo>>, public ActorCallback< GetRecommendedTagsActor<DB>, 0, Optional<bool> >, public ActorCallback< GetRecommendedTagsActor<DB>, 1, Void >, public ActorCallback< GetRecommendedTagsActor<DB>, 2, RangeResult >, public ActorCallback< GetRecommendedTagsActor<DB>, 3, Void >, public FastAllocated<GetRecommendedTagsActor<DB>>, public GetRecommendedTagsActorState<DB, GetRecommendedTagsActor<DB>> {
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<GetRecommendedTagsActor<DB>>::operator new;
	using FastAllocated<GetRecommendedTagsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<TagThrottleInfo>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 0, Optional<bool> >;
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 1, Void >;
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 2, RangeResult >;
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 3, Void >;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	GetRecommendedTagsActor(Reference<DB> const& db,int const& limit) 
															#line 1104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<std::vector<TagThrottleInfo>>(),
		   GetRecommendedTagsActorState<DB, GetRecommendedTagsActor<DB>>(db, limit)
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRecommendedTags");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 0, Optional<bool> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 2, RangeResult >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<std::vector<TagThrottleInfo>> getRecommendedTags( Reference<DB> const& db, int const& limit ) {
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<std::vector<TagThrottleInfo>>(new GetRecommendedTagsActor<DB>(db, limit));
															#line 1136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via getThrottledTags()
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB, class GetThrottledTagsActor>
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class GetThrottledTagsActorState {
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	GetThrottledTagsActorState(Reference<DB> const& db,int const& limit,ContainsRecommended const& containsRecommended = ContainsRecommended::False) 
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   limit(limit),
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   containsRecommended(containsRecommended),
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   reportAuto()
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("getThrottledTags", reinterpret_cast<unsigned long>(this));

	}
	~GetThrottledTagsActorState() 
	{
		fdb_probe_actor_destroy("getThrottledTags", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			;
															#line 1177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~GetThrottledTagsActorState();
		static_cast<GetThrottledTagsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_0 = store(reportAuto, getValidAutoEnabled(tr));
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 1;
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 0, Void >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 4;
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 3, Void >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!reportAuto.present())
															#line 1261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->reset();
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 2;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 1, Void >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!reportAuto.present())
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->reset();
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 2;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 1, Void >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

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
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state > 0) static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetThrottledTagsActor*>(this)->ActorCallback< GetThrottledTagsActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetThrottledTagsActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		f = tr->getRange( reportAuto.get() ? tagThrottleKeys : KeyRangeRef(tagThrottleKeysPrefix, tagThrottleAutoKeysPrefix), limit);
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		StrictFuture<RangeResult> __when_expr_2 = safeThreadFutureToFuture(f);
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 3;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 2, RangeResult >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state > 0) static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetThrottledTagsActor*>(this)->ActorCallback< GetThrottledTagsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetThrottledTagsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(RangeResult const& throttles,int loopDepth) 
	{
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetThrottledTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetThrottledTagsActorState(); static_cast<GetThrottledTagsActor*>(this)->destroy(); return 0; }
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetThrottledTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetThrottledTagsActorState();
		static_cast<GetThrottledTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(RangeResult && throttles,int loopDepth) 
	{
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 1495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetThrottledTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetThrottledTagsActorState(); static_cast<GetThrottledTagsActor*>(this)->destroy(); return 0; }
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetThrottledTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetThrottledTagsActorState();
		static_cast<GetThrottledTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(RangeResult const& throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(throttles, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(RangeResult && throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(throttles), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state > 0) static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetThrottledTagsActor*>(this)->ActorCallback< GetThrottledTagsActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetThrottledTagsActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state > 0) static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetThrottledTagsActor*>(this)->ActorCallback< GetThrottledTagsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetThrottledTagsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	int limit;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	ContainsRecommended containsRecommended;
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Optional<bool> reportAuto;
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> f;
															#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via getThrottledTags()
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class GetThrottledTagsActor final : public Actor<std::vector<TagThrottleInfo>>, public ActorCallback< GetThrottledTagsActor<DB>, 0, Void >, public ActorCallback< GetThrottledTagsActor<DB>, 1, Void >, public ActorCallback< GetThrottledTagsActor<DB>, 2, RangeResult >, public ActorCallback< GetThrottledTagsActor<DB>, 3, Void >, public FastAllocated<GetThrottledTagsActor<DB>>, public GetThrottledTagsActorState<DB, GetThrottledTagsActor<DB>> {
															#line 1664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<GetThrottledTagsActor<DB>>::operator new;
	using FastAllocated<GetThrottledTagsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<TagThrottleInfo>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetThrottledTagsActor<DB>, 0, Void >;
friend struct ActorCallback< GetThrottledTagsActor<DB>, 1, Void >;
friend struct ActorCallback< GetThrottledTagsActor<DB>, 2, RangeResult >;
friend struct ActorCallback< GetThrottledTagsActor<DB>, 3, Void >;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	GetThrottledTagsActor(Reference<DB> const& db,int const& limit,ContainsRecommended const& containsRecommended = ContainsRecommended::False) 
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<std::vector<TagThrottleInfo>>(),
		   GetThrottledTagsActorState<DB, GetThrottledTagsActor<DB>>(db, limit, containsRecommended)
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getThrottledTags");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetThrottledTagsActor<DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetThrottledTagsActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetThrottledTagsActor<DB>, 2, RangeResult >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GetThrottledTagsActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<std::vector<TagThrottleInfo>> getThrottledTags( Reference<DB> const& db, int const& limit, ContainsRecommended const& containsRecommended = ContainsRecommended::False ) {
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<std::vector<TagThrottleInfo>>(new GetThrottledTagsActor<DB>(db, limit, containsRecommended));
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

template <class Tr>
void signalThrottleChange(Reference<Tr> tr) {
	tr->atomicOp(tagThrottleSignalKey, "XXXXXXXXXX\x00\x00\x00\x00"_sr, MutationRef::SetVersionstampedValue);
}

															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via updateThrottleCount()
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class Tr, class UpdateThrottleCountActor>
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class UpdateThrottleCountActorState {
															#line 1726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	UpdateThrottleCountActorState(Reference<Tr> const& tr,int64_t const& delta) 
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : tr(tr),
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   delta(delta),
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   countVal(tr->get(tagThrottleCountKey)),
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   limitVal(tr->get(tagThrottleLimitKey))
															#line 1739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("updateThrottleCount", reinterpret_cast<unsigned long>(this));

	}
	~UpdateThrottleCountActorState() 
	{
		fdb_probe_actor_destroy("updateThrottleCount", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_0 = success(safeThreadFutureToFuture(countVal)) && success(safeThreadFutureToFuture(limitVal));
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UpdateThrottleCountActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateThrottleCountActor*>(this)->actor_wait_state = 1;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateThrottleCountActor, 0, Void >*>(static_cast<UpdateThrottleCountActor*>(this)));
															#line 1761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~UpdateThrottleCountActorState();
		static_cast<UpdateThrottleCountActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		int64_t count = 0;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		int64_t limit = 0;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (countVal.get().present())
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(countVal.get().get(), Unversioned());
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			reader >> count;
															#line 1794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (limitVal.get().present())
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(limitVal.get().get(), Unversioned());
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			reader >> limit;
															#line 1804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		count += delta;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (count > limit)
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			return a_body1Catch1(too_many_tag_throttles(), loopDepth);
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		BinaryWriter writer(Unversioned());
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		writer << count;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->set(tagThrottleCountKey, writer.toValue());
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<UpdateThrottleCountActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateThrottleCountActorState(); static_cast<UpdateThrottleCountActor*>(this)->destroy(); return 0; }
															#line 1824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<UpdateThrottleCountActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateThrottleCountActorState();
		static_cast<UpdateThrottleCountActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		int64_t count = 0;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		int64_t limit = 0;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (countVal.get().present())
															#line 1840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(countVal.get().get(), Unversioned());
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			reader >> count;
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (limitVal.get().present())
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(limitVal.get().get(), Unversioned());
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			reader >> limit;
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		count += delta;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (count > limit)
															#line 1862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			return a_body1Catch1(too_many_tag_throttles(), loopDepth);
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		BinaryWriter writer(Unversioned());
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		writer << count;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->set(tagThrottleCountKey, writer.toValue());
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<UpdateThrottleCountActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateThrottleCountActorState(); static_cast<UpdateThrottleCountActor*>(this)->destroy(); return 0; }
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<UpdateThrottleCountActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateThrottleCountActorState();
		static_cast<UpdateThrottleCountActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<UpdateThrottleCountActor*>(this)->actor_wait_state > 0) static_cast<UpdateThrottleCountActor*>(this)->actor_wait_state = 0;
		static_cast<UpdateThrottleCountActor*>(this)->ActorCallback< UpdateThrottleCountActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateThrottleCountActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updateThrottleCount", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateThrottleCount", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UpdateThrottleCountActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updateThrottleCount", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateThrottleCount", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UpdateThrottleCountActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updateThrottleCount", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateThrottleCount", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<Tr> tr;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	int64_t delta;
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename Tr::template FutureT<Optional<Value>> countVal;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename Tr::template FutureT<Optional<Value>> limitVal;
															#line 1955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via updateThrottleCount()
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class UpdateThrottleCountActor final : public Actor<Void>, public ActorCallback< UpdateThrottleCountActor<Tr>, 0, Void >, public FastAllocated<UpdateThrottleCountActor<Tr>>, public UpdateThrottleCountActorState<Tr, UpdateThrottleCountActor<Tr>> {
															#line 1962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<UpdateThrottleCountActor<Tr>>::operator new;
	using FastAllocated<UpdateThrottleCountActor<Tr>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateThrottleCountActor<Tr>, 0, Void >;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	UpdateThrottleCountActor(Reference<Tr> const& tr,int64_t const& delta) 
															#line 1973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<Void>(),
		   UpdateThrottleCountActorState<Tr, UpdateThrottleCountActor<Tr>>(tr, delta)
	{
		fdb_probe_actor_enter("updateThrottleCount", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updateThrottleCount");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updateThrottleCount", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UpdateThrottleCountActor<Tr>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Void> updateThrottleCount( Reference<Tr> const& tr, int64_t const& delta ) {
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<Void>(new UpdateThrottleCountActor<Tr>(tr, delta));
															#line 2002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

															#line 2007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via unthrottleMatchingThrottles()
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB, class UnthrottleMatchingThrottlesActor>
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class UnthrottleMatchingThrottlesActorState {
															#line 2013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	UnthrottleMatchingThrottlesActorState(Reference<DB> const& db,KeyRef const& beginKey,KeyRef const& endKey,Optional<TransactionPriority> const& priority,bool const& onlyExpiredThrottles) 
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   beginKey(beginKey),
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   endKey(endKey),
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   priority(priority),
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   onlyExpiredThrottles(onlyExpiredThrottles),
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   begin(firstGreaterOrEqual(beginKey)),
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   end(firstGreaterOrEqual(endKey)),
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   removed(false)
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this));

	}
	~UnthrottleMatchingThrottlesActorState() 
	{
		fdb_probe_actor_destroy("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			;
															#line 2051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~UnthrottleMatchingThrottlesActorState();
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			f = tr->getRange(begin, end, 1000);
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(f);
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 1;
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 0, RangeResult >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 4;
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 3, Void >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		unthrottledTags = 0;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		uint64_t manualUnthrottledTags = 0;
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for( auto tag : tags ) {
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (onlyExpiredThrottles)
															#line 2144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				double expirationTime = TagThrottleValue::fromValue(tag.value).expirationTime;
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (expirationTime == 0 || expirationTime > now())
															#line 2150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				{
					continue;
				}
			}
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			TagThrottleKey key = TagThrottleKey::fromKey(tag.key);
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (priority.present() && key.priority != priority.get())
															#line 2159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
				continue;
			}
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (key.throttleType == TagThrottleType::MANUAL)
															#line 2165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				++manualUnthrottledTags;
															#line 2169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			}
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			removed = true;
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->clear(tag.key);
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			unthrottledTags++;
															#line 2177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (manualUnthrottledTags > 0)
															#line 2181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, -manualUnthrottledTags);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 2;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 1, Void >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& __tags,int loopDepth) 
	{
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tags = __tags;
															#line 2206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && __tags,int loopDepth) 
	{
		tags = std::move(__tags);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->ActorCallback< UnthrottleMatchingThrottlesActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnthrottleMatchingThrottlesActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (unthrottledTags > 0)
															#line 2273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			signalThrottleChange(tr);
															#line 2277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 3;
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 2, Void >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->ActorCallback< UnthrottleMatchingThrottlesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< UnthrottleMatchingThrottlesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!tags.more)
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (!static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV<bool>::futures) { (void)(removed); this->~UnthrottleMatchingThrottlesActorState(); static_cast<UnthrottleMatchingThrottlesActor*>(this)->destroy(); return 0; }
															#line 2376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV< bool >::value()) bool(std::move(removed)); // state_var_RVO
			this->~UnthrottleMatchingThrottlesActorState();
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		ASSERT(tags.size() > 0);
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		begin = KeySelector(firstGreaterThan(tags[tags.size() - 1].key), tags.arena());
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = a_body1loopBody1cont10cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!tags.more)
															#line 2395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (!static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV<bool>::futures) { (void)(removed); this->~UnthrottleMatchingThrottlesActorState(); static_cast<UnthrottleMatchingThrottlesActor*>(this)->destroy(); return 0; }
															#line 2399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV< bool >::value()) bool(std::move(removed)); // state_var_RVO
			this->~UnthrottleMatchingThrottlesActorState();
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		ASSERT(tags.size() > 0);
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		begin = KeySelector(firstGreaterThan(tags[tags.size() - 1].key), tags.arena());
															#line 2409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = a_body1loopBody1cont10cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->ActorCallback< UnthrottleMatchingThrottlesActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< UnthrottleMatchingThrottlesActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont10cont3(int loopDepth) 
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
	void a_exitChoose4() 
	{
		if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->ActorCallback< UnthrottleMatchingThrottlesActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< UnthrottleMatchingThrottlesActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< UnthrottleMatchingThrottlesActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	KeyRef beginKey;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	KeyRef endKey;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Optional<TransactionPriority> priority;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	bool onlyExpiredThrottles;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	KeySelector begin;
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	KeySelector end;
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	bool removed;
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> f;
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	RangeResult tags;
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	uint64_t unthrottledTags;
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via unthrottleMatchingThrottles()
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class UnthrottleMatchingThrottlesActor final : public Actor<bool>, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 0, RangeResult >, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 1, Void >, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 2, Void >, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 3, Void >, public FastAllocated<UnthrottleMatchingThrottlesActor<DB>>, public UnthrottleMatchingThrottlesActorState<DB, UnthrottleMatchingThrottlesActor<DB>> {
															#line 2596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<UnthrottleMatchingThrottlesActor<DB>>::operator new;
	using FastAllocated<UnthrottleMatchingThrottlesActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 0, RangeResult >;
friend struct ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 1, Void >;
friend struct ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 2, Void >;
friend struct ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 3, Void >;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	UnthrottleMatchingThrottlesActor(Reference<DB> const& db,KeyRef const& beginKey,KeyRef const& endKey,Optional<TransactionPriority> const& priority,bool const& onlyExpiredThrottles) 
															#line 2610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<bool>(),
		   UnthrottleMatchingThrottlesActorState<DB, UnthrottleMatchingThrottlesActor<DB>>(db, beginKey, endKey, priority, onlyExpiredThrottles)
	{
		fdb_probe_actor_enter("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unthrottleMatchingThrottles");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unthrottleMatchingThrottles", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<bool> unthrottleMatchingThrottles( Reference<DB> const& db, KeyRef const& beginKey, KeyRef const& endKey, Optional<TransactionPriority> const& priority, bool const& onlyExpiredThrottles ) {
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<bool>(new UnthrottleMatchingThrottlesActor<DB>(db, beginKey, endKey, priority, onlyExpiredThrottles));
															#line 2642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

template <class DB>
Future<bool> expire(DB db) {
	return unthrottleMatchingThrottles(
	    db, tagThrottleKeys.begin, tagThrottleKeys.end, Optional<TransactionPriority>(), true);
}

template <class DB>
Future<bool> unthrottleAll(Reference<DB> db,
                           Optional<TagThrottleType> tagThrottleType,
                           Optional<TransactionPriority> priority) {
	KeyRef begin = tagThrottleKeys.begin;
	KeyRef end = tagThrottleKeys.end;

	if (tagThrottleType.present() && tagThrottleType == TagThrottleType::AUTO) {
		begin = tagThrottleAutoKeysPrefix;
	} else if (tagThrottleType.present() && tagThrottleType == TagThrottleType::MANUAL) {
		end = tagThrottleAutoKeysPrefix;
	}

	return unthrottleMatchingThrottles(db, begin, end, priority, false);
}

															#line 2669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via unthrottleTags()
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB, class UnthrottleTagsActor>
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class UnthrottleTagsActorState {
															#line 2675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	UnthrottleTagsActorState(Reference<DB> const& db,TagSet const& tags,Optional<TagThrottleType> const& throttleType,Optional<TransactionPriority> const& priority) 
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tags(tags),
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   throttleType(throttleType),
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   priority(priority),
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   keys()
															#line 2692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("unthrottleTags", reinterpret_cast<unsigned long>(this));

	}
	~UnthrottleTagsActorState() 
	{
		fdb_probe_actor_destroy("unthrottleTags", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			for( auto p : allTransactionPriorities ) {
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (!priority.present() || priority.get() == p)
															#line 2709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				{
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					if (!throttleType.present() || throttleType.get() == TagThrottleType::AUTO)
															#line 2713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					{
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
						keys.push_back(TagThrottleKey(tags, TagThrottleType::AUTO, p).toKey());
															#line 2717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					}
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					if (!throttleType.present() || throttleType.get() == TagThrottleType::MANUAL)
															#line 2721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					{
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
						keys.push_back(TagThrottleKey(tags, TagThrottleType::MANUAL, p).toKey());
															#line 2725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
					}
				}
			}
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			removed = false;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			;
															#line 2733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~UnthrottleTagsActorState();
		static_cast<UnthrottleTagsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			valueFutures = std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>>();
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			values = std::vector<Future<Optional<Value>>>();
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			values.reserve(keys.size());
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			for( auto key : keys ) {
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				valueFutures.push_back(tr->get(key));
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				values.push_back(safeThreadFutureToFuture(valueFutures.back()));
															#line 2777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			}
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_0 = waitForAll(values);
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 1;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 0, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 4;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 3, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		int delta = 0;
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for(int i = 0;i < values.size();++i) {
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (values[i].get().present())
															#line 2836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (TagThrottleKey::fromKey(keys[i]).throttleType == TagThrottleType::MANUAL)
															#line 2840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					delta -= 1;
															#line 2844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				tr->clear(keys[i]);
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				removed = true;
															#line 2850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			}
		}
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (delta != 0)
															#line 2855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, delta);
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 1, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		int delta = 0;
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		for(int i = 0;i < values.size();++i) {
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (values[i].get().present())
															#line 2884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (TagThrottleKey::fromKey(keys[i]).throttleType == TagThrottleType::MANUAL)
															#line 2888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
					delta -= 1;
															#line 2892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				tr->clear(keys[i]);
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				removed = true;
															#line 2898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			}
		}
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (delta != 0)
															#line 2903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, delta);
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 1, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

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
		if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleTagsActor*>(this)->ActorCallback< UnthrottleTagsActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnthrottleTagsActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (removed)
															#line 2991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			signalThrottleChange(tr);
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 3;
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 2, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 3004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont9(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleTagsActor*>(this)->ActorCallback< UnthrottleTagsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< UnthrottleTagsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont9(int loopDepth) 
	{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<UnthrottleTagsActor*>(this)->SAV<bool>::futures) { (void)(removed); this->~UnthrottleTagsActorState(); static_cast<UnthrottleTagsActor*>(this)->destroy(); return 0; }
															#line 3093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<UnthrottleTagsActor*>(this)->SAV< bool >::value()) bool(std::move(removed)); // state_var_RVO
		this->~UnthrottleTagsActorState();
		static_cast<UnthrottleTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleTagsActor*>(this)->ActorCallback< UnthrottleTagsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< UnthrottleTagsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<UnthrottleTagsActor*>(this)->ActorCallback< UnthrottleTagsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< UnthrottleTagsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< UnthrottleTagsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	TagSet tags;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Optional<TagThrottleType> throttleType;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Optional<TransactionPriority> priority;
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	std::vector<Key> keys;
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	bool removed;
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>> valueFutures;
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	std::vector<Future<Optional<Value>>> values;
															#line 3269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via unthrottleTags()
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class UnthrottleTagsActor final : public Actor<bool>, public ActorCallback< UnthrottleTagsActor<DB>, 0, Void >, public ActorCallback< UnthrottleTagsActor<DB>, 1, Void >, public ActorCallback< UnthrottleTagsActor<DB>, 2, Void >, public ActorCallback< UnthrottleTagsActor<DB>, 3, Void >, public FastAllocated<UnthrottleTagsActor<DB>>, public UnthrottleTagsActorState<DB, UnthrottleTagsActor<DB>> {
															#line 3276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<UnthrottleTagsActor<DB>>::operator new;
	using FastAllocated<UnthrottleTagsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnthrottleTagsActor<DB>, 0, Void >;
friend struct ActorCallback< UnthrottleTagsActor<DB>, 1, Void >;
friend struct ActorCallback< UnthrottleTagsActor<DB>, 2, Void >;
friend struct ActorCallback< UnthrottleTagsActor<DB>, 3, Void >;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	UnthrottleTagsActor(Reference<DB> const& db,TagSet const& tags,Optional<TagThrottleType> const& throttleType,Optional<TransactionPriority> const& priority) 
															#line 3290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<bool>(),
		   UnthrottleTagsActorState<DB, UnthrottleTagsActor<DB>>(db, tags, throttleType, priority)
	{
		fdb_probe_actor_enter("unthrottleTags", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unthrottleTags");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unthrottleTags", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnthrottleTagsActor<DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< UnthrottleTagsActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< UnthrottleTagsActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< UnthrottleTagsActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<bool> unthrottleTags( Reference<DB> const& db, TagSet const& tags, Optional<TagThrottleType> const& throttleType, Optional<TransactionPriority> const& priority ) {
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<bool>(new UnthrottleTagsActor<DB>(db, tags, throttleType, priority));
															#line 3322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

															#line 3327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via throttleTags()
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB, class ThrottleTagsActor>
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class ThrottleTagsActorState {
															#line 3333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	ThrottleTagsActorState(Reference<DB> const& db,TagSet const& tags,double const& tpsRate,double const& initialDuration,TagThrottleType const& throttleType,TransactionPriority const& priority,Optional<double> const& expirationTime = Optional<double>(),Optional<TagThrottledReason> const& reason = Optional<TagThrottledReason>()) 
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tags(tags),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tpsRate(tpsRate),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   initialDuration(initialDuration),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   throttleType(throttleType),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   priority(priority),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   expirationTime(expirationTime),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   reason(reason),
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   key(TagThrottleKey(tags, throttleType, priority).toKey())
															#line 3358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("throttleTags", reinterpret_cast<unsigned long>(this));

	}
	~ThrottleTagsActorState() 
	{
		fdb_probe_actor_destroy("throttleTags", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			ASSERT(initialDuration > 0);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (throttleType == TagThrottleType::MANUAL)
															#line 3375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				reason = TagThrottledReason::MANUAL;
															#line 3379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			}
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			TagThrottleValue throttle(tpsRate, expirationTime.present() ? expirationTime.get() : 0, initialDuration, reason.present() ? reason.get() : TagThrottledReason::UNSET);
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			BinaryWriter wr(IncludeVersion(ProtocolVersion::withTagThrottleValueReason()));
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			wr << throttle;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			value = wr.toValue();
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			;
															#line 3391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~ThrottleTagsActorState();
		static_cast<ThrottleTagsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 3421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (throttleType == TagThrottleType::MANUAL)
															#line 3425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			{
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				oldThrottleF = tr->get(key);
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(oldThrottleF);
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 1;
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 0, Optional<Value> >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
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
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 4;
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 3, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->set(key, value);
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (throttleType == TagThrottleType::MANUAL)
															#line 3489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			signalThrottleChange(tr);
															#line 3493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		}
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 3;
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 2, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<Value> const& oldThrottle,int loopDepth) 
	{
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!oldThrottle.present())
															#line 3513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, 1);
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 1, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<Value> && oldThrottle,int loopDepth) 
	{
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!oldThrottle.present())
															#line 3538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, 1);
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 1, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& oldThrottle,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(oldThrottle, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && oldThrottle,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(oldThrottle), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<ThrottleTagsActor*>(this)->ActorCallback< ThrottleTagsActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ThrottleTagsActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<ThrottleTagsActor*>(this)->ActorCallback< ThrottleTagsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ThrottleTagsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<ThrottleTagsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ThrottleTagsActorState(); static_cast<ThrottleTagsActor*>(this)->destroy(); return 0; }
															#line 3707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<ThrottleTagsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ThrottleTagsActorState();
		static_cast<ThrottleTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<ThrottleTagsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ThrottleTagsActorState(); static_cast<ThrottleTagsActor*>(this)->destroy(); return 0; }
															#line 3719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<ThrottleTagsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ThrottleTagsActorState();
		static_cast<ThrottleTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<ThrottleTagsActor*>(this)->ActorCallback< ThrottleTagsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ThrottleTagsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state > 0) static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 0;
		static_cast<ThrottleTagsActor*>(this)->ActorCallback< ThrottleTagsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ThrottleTagsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ThrottleTagsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	TagSet tags;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	double tpsRate;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	double initialDuration;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	TagThrottleType throttleType;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	TransactionPriority priority;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Optional<double> expirationTime;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Optional<TagThrottledReason> reason;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Key key;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Value value;
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<Optional<Value>> oldThrottleF;
															#line 3889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via throttleTags()
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class ThrottleTagsActor final : public Actor<Void>, public ActorCallback< ThrottleTagsActor<DB>, 0, Optional<Value> >, public ActorCallback< ThrottleTagsActor<DB>, 1, Void >, public ActorCallback< ThrottleTagsActor<DB>, 2, Void >, public ActorCallback< ThrottleTagsActor<DB>, 3, Void >, public FastAllocated<ThrottleTagsActor<DB>>, public ThrottleTagsActorState<DB, ThrottleTagsActor<DB>> {
															#line 3896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<ThrottleTagsActor<DB>>::operator new;
	using FastAllocated<ThrottleTagsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ThrottleTagsActor<DB>, 0, Optional<Value> >;
friend struct ActorCallback< ThrottleTagsActor<DB>, 1, Void >;
friend struct ActorCallback< ThrottleTagsActor<DB>, 2, Void >;
friend struct ActorCallback< ThrottleTagsActor<DB>, 3, Void >;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	ThrottleTagsActor(Reference<DB> const& db,TagSet const& tags,double const& tpsRate,double const& initialDuration,TagThrottleType const& throttleType,TransactionPriority const& priority,Optional<double> const& expirationTime = Optional<double>(),Optional<TagThrottledReason> const& reason = Optional<TagThrottledReason>()) 
															#line 3910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<Void>(),
		   ThrottleTagsActorState<DB, ThrottleTagsActor<DB>>(db, tags, tpsRate, initialDuration, throttleType, priority, expirationTime, reason)
	{
		fdb_probe_actor_enter("throttleTags", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("throttleTags");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("throttleTags", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ThrottleTagsActor<DB>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ThrottleTagsActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ThrottleTagsActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ThrottleTagsActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Void> throttleTags( Reference<DB> const& db, TagSet const& tags, double const& tpsRate, double const& initialDuration, TagThrottleType const& throttleType, TransactionPriority const& priority, Optional<double> const& expirationTime = Optional<double>(), Optional<TagThrottledReason> const& reason = Optional<TagThrottledReason>() ) {
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<Void>(new ThrottleTagsActor<DB>(db, tags, tpsRate, initialDuration, throttleType, priority, expirationTime, reason));
															#line 3942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

															#line 3947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via enableAuto()
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB, class EnableAutoActor>
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class EnableAutoActorState {
															#line 3953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	EnableAutoActorState(Reference<DB> const& db,bool const& enabled) 
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   enabled(enabled),
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction())
															#line 3964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
	{
		fdb_probe_actor_create("enableAuto", reinterpret_cast<unsigned long>(this));

	}
	~EnableAutoActorState() 
	{
		fdb_probe_actor_destroy("enableAuto", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			;
															#line 3979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
		this->~EnableAutoActorState();
		static_cast<EnableAutoActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 4009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			valueF = tr->get(tagThrottleAutoEnabledKey);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(valueF);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 1;
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 0, Optional<Value> >*>(static_cast<EnableAutoActor*>(this)));
															#line 4022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
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
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 3;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 2, Void >*>(static_cast<EnableAutoActor*>(this)));
															#line 4051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& value,int loopDepth) 
	{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!value.present() || (enabled && value.get() != "1"_sr) || (!enabled && value.get() != "0"_sr))
															#line 4066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->set(tagThrottleAutoEnabledKey, enabled ? "1"_sr : "0"_sr);
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			signalThrottleChange<typename DB::TransactionT>(tr);
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 2;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 1, Void >*>(static_cast<EnableAutoActor*>(this)));
															#line 4081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && value,int loopDepth) 
	{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!value.present() || (enabled && value.get() != "1"_sr) || (!enabled && value.get() != "0"_sr))
															#line 4095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		{
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			tr->set(tagThrottleAutoEnabledKey, enabled ? "1"_sr : "0"_sr);
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			signalThrottleChange<typename DB::TransactionT>(tr);
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 2;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 1, Void >*>(static_cast<EnableAutoActor*>(this)));
															#line 4110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(value, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<EnableAutoActor*>(this)->actor_wait_state > 0) static_cast<EnableAutoActor*>(this)->actor_wait_state = 0;
		static_cast<EnableAutoActor*>(this)->ActorCallback< EnableAutoActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< EnableAutoActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< EnableAutoActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< EnableAutoActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
		if (!static_cast<EnableAutoActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EnableAutoActorState(); static_cast<EnableAutoActor*>(this)->destroy(); return 0; }
															#line 4187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<EnableAutoActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~EnableAutoActorState();
		static_cast<EnableAutoActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<EnableAutoActor*>(this)->actor_wait_state > 0) static_cast<EnableAutoActor*>(this)->actor_wait_state = 0;
		static_cast<EnableAutoActor*>(this)->ActorCallback< EnableAutoActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< EnableAutoActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< EnableAutoActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< EnableAutoActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<EnableAutoActor*>(this)->actor_wait_state > 0) static_cast<EnableAutoActor*>(this)->actor_wait_state = 0;
		static_cast<EnableAutoActor*>(this)->ActorCallback< EnableAutoActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< EnableAutoActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< EnableAutoActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< EnableAutoActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	bool enabled;
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<Optional<Value>> valueF;
															#line 4353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via enableAuto()
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
class EnableAutoActor final : public Actor<Void>, public ActorCallback< EnableAutoActor<DB>, 0, Optional<Value> >, public ActorCallback< EnableAutoActor<DB>, 1, Void >, public ActorCallback< EnableAutoActor<DB>, 2, Void >, public FastAllocated<EnableAutoActor<DB>>, public EnableAutoActorState<DB, EnableAutoActor<DB>> {
															#line 4360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<EnableAutoActor<DB>>::operator new;
	using FastAllocated<EnableAutoActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< EnableAutoActor<DB>, 0, Optional<Value> >;
friend struct ActorCallback< EnableAutoActor<DB>, 1, Void >;
friend struct ActorCallback< EnableAutoActor<DB>, 2, Void >;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	EnableAutoActor(Reference<DB> const& db,bool const& enabled) 
															#line 4373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
		 : Actor<Void>(),
		   EnableAutoActorState<DB, EnableAutoActor<DB>>(db, enabled)
	{
		fdb_probe_actor_enter("enableAuto", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("enableAuto");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("enableAuto", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< EnableAutoActor<DB>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< EnableAutoActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< EnableAutoActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Void> enableAuto( Reference<DB> const& db, bool const& enabled ) {
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"
	return Future<Void>(new EnableAutoActor<DB>(db, enabled));
															#line 4404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.g.h"
}

#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TagThrottle.actor.h"

class TagQuotaValue {
public:
	int64_t reservedQuota{ 0 };
	int64_t totalQuota{ 0 };
	bool isValid() const;
	Value toValue() const;
	static TagQuotaValue fromValue(ValueRef);
};

Key getTagQuotaKey(TransactionTagRef);

template <class Tr>
void setTagQuota(Reference<Tr> tr, TransactionTagRef tag, int64_t reservedQuota, int64_t totalQuota) {
	TagQuotaValue tagQuotaValue;
	tagQuotaValue.reservedQuota = reservedQuota;
	tagQuotaValue.totalQuota = totalQuota;
	if (!tagQuotaValue.isValid()) {
		throw invalid_throttle_quota_value();
	}
	tr->set(getTagQuotaKey(tag), tagQuotaValue.toValue());
	signalThrottleChange(tr);
}

}; // namespace ThrottleApi

template <class Value>
using TransactionTagMap = std::unordered_map<TransactionTag, Value, std::hash<TransactionTagRef>>;

template <class Value>
using PrioritizedTransactionTagMap = std::map<TransactionPriority, TransactionTagMap<Value>>;

template <class Value>
using UIDTransactionTagMap = std::unordered_map<UID, TransactionTagMap<Value>>;

#include "flow/unactorcompiler.h"
#endif
