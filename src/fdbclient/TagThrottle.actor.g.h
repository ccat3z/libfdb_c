#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
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

FDB_DECLARE_BOOLEAN_PARAM(ContainsRecommended);
FDB_DECLARE_BOOLEAN_PARAM(Capitalize);

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

															#line 256 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via getValidAutoEnabled()
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class Tr, class GetValidAutoEnabledActor>
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class GetValidAutoEnabledActorState {
															#line 262 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	GetValidAutoEnabledActorState(Reference<Tr> const& tr) 
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : tr(tr),
															#line 256 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   result()
															#line 271 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 257 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 286 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
		this->~GetValidAutoEnabledActorState();
		static_cast<GetValidAutoEnabledActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 259 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		valueF = tr->get(tagThrottleAutoEnabledKey);
															#line 260 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(valueF);
															#line 260 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 320 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 1;
															#line 260 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetValidAutoEnabledActor, 0, Optional<Value> >*>(static_cast<GetValidAutoEnabledActor*>(this)));
															#line 325 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Optional<Value> const& value,int loopDepth) 
	{
															#line 261 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!value.present())
															#line 334 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 262 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			tr->reset();
															#line 263 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 263 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 342 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 2;
															#line 263 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetValidAutoEnabledActor, 1, Void >*>(static_cast<GetValidAutoEnabledActor*>(this)));
															#line 347 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 265 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (value.get() == LiteralStringRef("1"))
															#line 354 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 266 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				result = true;
															#line 358 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				loopDepth = a_body1loopBody1cont5(loopDepth);
			}
			else
			{
															#line 267 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (value.get() == LiteralStringRef("0"))
															#line 365 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				{
															#line 268 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					result = false;
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					loopDepth = a_body1loopBody1cont7(loopDepth);
				}
				else
				{
															#line 270 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					TraceEvent(SevWarnAlways, "InvalidAutoTagThrottlingValue").detail("Value", value.get());
															#line 271 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					tr->reset();
															#line 272 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					StrictFuture<Void> __when_expr_2 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 272 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 382 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
					static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 3;
															#line 272 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetValidAutoEnabledActor, 2, Void >*>(static_cast<GetValidAutoEnabledActor*>(this)));
															#line 387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					loopDepth = 0;
				}
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(Optional<Value> && value,int loopDepth) 
	{
															#line 261 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!value.present())
															#line 399 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 262 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			tr->reset();
															#line 263 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 263 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 407 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 2;
															#line 263 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetValidAutoEnabledActor, 1, Void >*>(static_cast<GetValidAutoEnabledActor*>(this)));
															#line 412 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 265 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (value.get() == LiteralStringRef("1"))
															#line 419 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 266 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				result = true;
															#line 423 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				loopDepth = a_body1loopBody1cont5(loopDepth);
			}
			else
			{
															#line 267 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (value.get() == LiteralStringRef("0"))
															#line 430 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				{
															#line 268 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					result = false;
															#line 434 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					loopDepth = a_body1loopBody1cont7(loopDepth);
				}
				else
				{
															#line 270 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					TraceEvent(SevWarnAlways, "InvalidAutoTagThrottlingValue").detail("Value", value.get());
															#line 271 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					tr->reset();
															#line 272 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					StrictFuture<Void> __when_expr_2 = delay(CLIENT_KNOBS->DEFAULT_BACKOFF);
															#line 272 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 447 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when2(__when_expr_2.get(), loopDepth); };
					static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 3;
															#line 272 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetValidAutoEnabledActor, 2, Void >*>(static_cast<GetValidAutoEnabledActor*>(this)));
															#line 452 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					loopDepth = 0;
				}
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(value, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(value), loopDepth);

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
			a_body1loopBody1when1(value, 0);
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
			a_body1loopBody1when1(std::move(value), 0);
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
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 275 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetValidAutoEnabledActor*>(this)->SAV<bool>::futures) { (void)(result); this->~GetValidAutoEnabledActorState(); static_cast<GetValidAutoEnabledActor*>(this)->destroy(); return 0; }
															#line 527 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetValidAutoEnabledActor*>(this)->SAV< bool >::value()) bool(std::move(result)); // state_var_RVO
		this->~GetValidAutoEnabledActorState();
		static_cast<GetValidAutoEnabledActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state > 0) static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 0;
		static_cast<GetValidAutoEnabledActor*>(this)->ActorCallback< GetValidAutoEnabledActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetValidAutoEnabledActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetValidAutoEnabledActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetValidAutoEnabledActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state > 0) static_cast<GetValidAutoEnabledActor*>(this)->actor_wait_state = 0;
		static_cast<GetValidAutoEnabledActor*>(this)->ActorCallback< GetValidAutoEnabledActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetValidAutoEnabledActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetValidAutoEnabledActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetValidAutoEnabledActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getValidAutoEnabled", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<Tr> tr;
															#line 256 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	bool result;
															#line 259 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename Tr::template FutureT<Optional<Value>> valueF;
															#line 703 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via getValidAutoEnabled()
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class GetValidAutoEnabledActor final : public Actor<bool>, public ActorCallback< GetValidAutoEnabledActor<Tr>, 0, Optional<Value> >, public ActorCallback< GetValidAutoEnabledActor<Tr>, 1, Void >, public ActorCallback< GetValidAutoEnabledActor<Tr>, 2, Void >, public FastAllocated<GetValidAutoEnabledActor<Tr>>, public GetValidAutoEnabledActorState<Tr, GetValidAutoEnabledActor<Tr>> {
															#line 710 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<GetValidAutoEnabledActor<Tr>>::operator new;
	using FastAllocated<GetValidAutoEnabledActor<Tr>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetValidAutoEnabledActor<Tr>, 0, Optional<Value> >;
friend struct ActorCallback< GetValidAutoEnabledActor<Tr>, 1, Void >;
friend struct ActorCallback< GetValidAutoEnabledActor<Tr>, 2, Void >;
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	GetValidAutoEnabledActor(Reference<Tr> const& tr) 
															#line 723 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		 : Actor<bool>(),
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
		case 2: this->a_callback_error((ActorCallback< GetValidAutoEnabledActor<Tr>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetValidAutoEnabledActor<Tr>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<bool> getValidAutoEnabled( Reference<Tr> const& tr ) {
															#line 254 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<bool>(new GetValidAutoEnabledActor<Tr>(tr));
															#line 754 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 278 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

															#line 759 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via getRecommendedTags()
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB, class GetRecommendedTagsActor>
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class GetRecommendedTagsActorState {
															#line 765 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	GetRecommendedTagsActorState(Reference<DB> const& db,int const& limit) 
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   limit(limit),
															#line 281 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction())
															#line 776 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 282 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 791 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 283 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 821 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 285 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<bool> __when_expr_0 = getValidAutoEnabled(tr);
															#line 285 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 827 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 1;
															#line 285 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 0, bool >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 832 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 299 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 299 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 856 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 3;
															#line 299 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 2, Void >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 861 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& enableAuto,int loopDepth) 
	{
															#line 286 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (enableAuto)
															#line 876 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 287 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(std::vector<TagThrottleInfo>()); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 880 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(std::vector<TagThrottleInfo>());
			this->~GetRecommendedTagsActorState();
			static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 289 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		f = tr->getRange(KeyRangeRef(tagThrottleAutoKeysPrefix, tagThrottleKeys.end), limit);
															#line 291 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(f);
															#line 291 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 892 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 2;
															#line 291 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 1, RangeResult >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 897 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && enableAuto,int loopDepth) 
	{
															#line 286 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (enableAuto)
															#line 906 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 287 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(std::vector<TagThrottleInfo>()); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 910 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(std::vector<TagThrottleInfo>());
			this->~GetRecommendedTagsActorState();
			static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 289 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		f = tr->getRange(KeyRangeRef(tagThrottleAutoKeysPrefix, tagThrottleKeys.end), limit);
															#line 291 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(f);
															#line 291 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 922 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 2;
															#line 291 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRecommendedTagsActor, 1, RangeResult >*>(static_cast<GetRecommendedTagsActor*>(this)));
															#line 927 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& enableAuto,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(enableAuto, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && enableAuto,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(enableAuto), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 0, bool >*,bool const& value) 
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
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 0, bool >*,bool && value) 
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
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 0, bool >*,Error err) 
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
	int a_body1loopBody1cont3(RangeResult const& throttles,int loopDepth) 
	{
															#line 292 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 293 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 294 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 1003 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 297 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 1007 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetRecommendedTagsActorState();
		static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(RangeResult && throttles,int loopDepth) 
	{
															#line 292 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 293 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 294 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 1023 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 297 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetRecommendedTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetRecommendedTagsActorState(); static_cast<GetRecommendedTagsActor*>(this)->destroy(); return 0; }
															#line 1027 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetRecommendedTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetRecommendedTagsActorState();
		static_cast<GetRecommendedTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(throttles, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(throttles), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 1, RangeResult >*,RangeResult const& value) 
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
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 1, RangeResult >*,RangeResult && value) 
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
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 1, RangeResult >*,Error err) 
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
		if (static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state > 0) static_cast<GetRecommendedTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetRecommendedTagsActor*>(this)->ActorCallback< GetRecommendedTagsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetRecommendedTagsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetRecommendedTagsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRecommendedTags", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	int limit;
															#line 281 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 289 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> f;
															#line 1181 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via getRecommendedTags()
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class GetRecommendedTagsActor final : public Actor<std::vector<TagThrottleInfo>>, public ActorCallback< GetRecommendedTagsActor<DB>, 0, bool >, public ActorCallback< GetRecommendedTagsActor<DB>, 1, RangeResult >, public ActorCallback< GetRecommendedTagsActor<DB>, 2, Void >, public FastAllocated<GetRecommendedTagsActor<DB>>, public GetRecommendedTagsActorState<DB, GetRecommendedTagsActor<DB>> {
															#line 1188 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<GetRecommendedTagsActor<DB>>::operator new;
	using FastAllocated<GetRecommendedTagsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<TagThrottleInfo>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 0, bool >;
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 1, RangeResult >;
friend struct ActorCallback< GetRecommendedTagsActor<DB>, 2, Void >;
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	GetRecommendedTagsActor(Reference<DB> const& db,int const& limit) 
															#line 1201 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
		case 1: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetRecommendedTagsActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<std::vector<TagThrottleInfo>> getRecommendedTags( Reference<DB> const& db, int const& limit ) {
															#line 279 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<std::vector<TagThrottleInfo>>(new GetRecommendedTagsActor<DB>(db, limit));
															#line 1232 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 303 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

															#line 1237 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via getThrottledTags()
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB, class GetThrottledTagsActor>
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class GetThrottledTagsActorState {
															#line 1243 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	GetThrottledTagsActorState(Reference<DB> const& db,int const& limit,ContainsRecommended const& containsRecommended = ContainsRecommended::False) 
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   limit(limit),
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   containsRecommended(containsRecommended),
															#line 307 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 308 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   reportAuto(containsRecommended)
															#line 1258 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 309 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 1273 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 310 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 1303 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 312 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (!containsRecommended)
															#line 1307 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 313 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				StrictFuture<Void> __when_expr_0 = store(reportAuto, getValidAutoEnabled(tr));
															#line 313 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1313 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 1;
															#line 313 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 0, Void >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1318 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 325 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 325 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1347 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 3;
															#line 325 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 2, Void >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1352 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 315 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		f = tr->getRange( reportAuto ? tagThrottleKeys : KeyRangeRef(tagThrottleKeysPrefix, tagThrottleAutoKeysPrefix), limit);
															#line 317 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(f);
															#line 317 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1371 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 2;
															#line 317 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetThrottledTagsActor, 1, RangeResult >*>(static_cast<GetThrottledTagsActor*>(this)));
															#line 1376 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
	int a_body1loopBody1cont4(RangeResult const& throttles,int loopDepth) 
	{
															#line 318 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 319 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 320 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 1464 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 323 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetThrottledTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetThrottledTagsActorState(); static_cast<GetThrottledTagsActor*>(this)->destroy(); return 0; }
															#line 1468 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetThrottledTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetThrottledTagsActorState();
		static_cast<GetThrottledTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(RangeResult && throttles,int loopDepth) 
	{
															#line 318 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		std::vector<TagThrottleInfo> results;
															#line 319 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for( auto throttle : throttles ) {
															#line 320 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			results.push_back(TagThrottleInfo(TagThrottleKey::fromKey(throttle.key), TagThrottleValue::fromValue(throttle.value)));
															#line 1484 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 323 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<GetThrottledTagsActor*>(this)->SAV<std::vector<TagThrottleInfo>>::futures) { (void)(results); this->~GetThrottledTagsActorState(); static_cast<GetThrottledTagsActor*>(this)->destroy(); return 0; }
															#line 1488 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<GetThrottledTagsActor*>(this)->SAV< std::vector<TagThrottleInfo> >::value()) std::vector<TagThrottleInfo>(results);
		this->~GetThrottledTagsActorState();
		static_cast<GetThrottledTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(throttles, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && throttles,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(throttles), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state > 0) static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetThrottledTagsActor*>(this)->ActorCallback< GetThrottledTagsActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 1, RangeResult >*,RangeResult const& value) 
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
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 1, RangeResult >*,RangeResult && value) 
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
	void a_callback_error(ActorCallback< GetThrottledTagsActor, 1, RangeResult >*,Error err) 
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
		if (static_cast<GetThrottledTagsActor*>(this)->actor_wait_state > 0) static_cast<GetThrottledTagsActor*>(this)->actor_wait_state = 0;
		static_cast<GetThrottledTagsActor*>(this)->ActorCallback< GetThrottledTagsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetThrottledTagsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetThrottledTagsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThrottledTags", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	int limit;
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	ContainsRecommended containsRecommended;
															#line 307 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 308 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	bool reportAuto;
															#line 315 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> f;
															#line 1646 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via getThrottledTags()
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class GetThrottledTagsActor final : public Actor<std::vector<TagThrottleInfo>>, public ActorCallback< GetThrottledTagsActor<DB>, 0, Void >, public ActorCallback< GetThrottledTagsActor<DB>, 1, RangeResult >, public ActorCallback< GetThrottledTagsActor<DB>, 2, Void >, public FastAllocated<GetThrottledTagsActor<DB>>, public GetThrottledTagsActorState<DB, GetThrottledTagsActor<DB>> {
															#line 1653 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<GetThrottledTagsActor<DB>>::operator new;
	using FastAllocated<GetThrottledTagsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<TagThrottleInfo>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetThrottledTagsActor<DB>, 0, Void >;
friend struct ActorCallback< GetThrottledTagsActor<DB>, 1, RangeResult >;
friend struct ActorCallback< GetThrottledTagsActor<DB>, 2, Void >;
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	GetThrottledTagsActor(Reference<DB> const& db,int const& limit,ContainsRecommended const& containsRecommended = ContainsRecommended::False) 
															#line 1666 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
		case 2: this->a_callback_error((ActorCallback< GetThrottledTagsActor<DB>, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetThrottledTagsActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<std::vector<TagThrottleInfo>> getThrottledTags( Reference<DB> const& db, int const& limit, ContainsRecommended const& containsRecommended = ContainsRecommended::False ) {
															#line 304 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<std::vector<TagThrottleInfo>>(new GetThrottledTagsActor<DB>(db, limit, containsRecommended));
															#line 1697 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 329 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

template <class Tr>
void signalThrottleChange(Reference<Tr> tr) {
	tr->atomicOp(
	    tagThrottleSignalKey, LiteralStringRef("XXXXXXXXXX\x00\x00\x00\x00"), MutationRef::SetVersionstampedValue);
}

															#line 1708 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via updateThrottleCount()
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class Tr, class UpdateThrottleCountActor>
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class UpdateThrottleCountActorState {
															#line 1714 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	UpdateThrottleCountActorState(Reference<Tr> const& tr,int64_t const& delta) 
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : tr(tr),
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   delta(delta),
															#line 338 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   countVal(tr->get(tagThrottleCountKey)),
															#line 339 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   limitVal(tr->get(tagThrottleLimitKey))
															#line 1727 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 341 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_0 = success(safeThreadFutureToFuture(countVal)) && success(safeThreadFutureToFuture(limitVal));
															#line 341 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UpdateThrottleCountActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1744 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateThrottleCountActor*>(this)->actor_wait_state = 1;
															#line 341 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateThrottleCountActor, 0, Void >*>(static_cast<UpdateThrottleCountActor*>(this)));
															#line 1749 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 343 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		int64_t count = 0;
															#line 344 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		int64_t limit = 0;
															#line 346 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (countVal.get().present())
															#line 1776 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 347 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(countVal.get().get(), Unversioned());
															#line 348 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			reader >> count;
															#line 1782 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 351 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (limitVal.get().present())
															#line 1786 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 352 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(limitVal.get().get(), Unversioned());
															#line 353 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			reader >> limit;
															#line 1792 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 356 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		count += delta;
															#line 358 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (count > limit)
															#line 1798 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 359 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			return a_body1Catch1(too_many_tag_throttles(), loopDepth);
															#line 1802 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 362 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		BinaryWriter writer(Unversioned());
															#line 363 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		writer << count;
															#line 365 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->set(tagThrottleCountKey, writer.toValue());
															#line 366 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<UpdateThrottleCountActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateThrottleCountActorState(); static_cast<UpdateThrottleCountActor*>(this)->destroy(); return 0; }
															#line 1812 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<UpdateThrottleCountActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateThrottleCountActorState();
		static_cast<UpdateThrottleCountActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 343 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		int64_t count = 0;
															#line 344 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		int64_t limit = 0;
															#line 346 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (countVal.get().present())
															#line 1828 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 347 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(countVal.get().get(), Unversioned());
															#line 348 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			reader >> count;
															#line 1834 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 351 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (limitVal.get().present())
															#line 1838 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 352 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			BinaryReader reader(limitVal.get().get(), Unversioned());
															#line 353 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			reader >> limit;
															#line 1844 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 356 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		count += delta;
															#line 358 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (count > limit)
															#line 1850 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 359 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			return a_body1Catch1(too_many_tag_throttles(), loopDepth);
															#line 1854 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 362 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		BinaryWriter writer(Unversioned());
															#line 363 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		writer << count;
															#line 365 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->set(tagThrottleCountKey, writer.toValue());
															#line 366 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<UpdateThrottleCountActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateThrottleCountActorState(); static_cast<UpdateThrottleCountActor*>(this)->destroy(); return 0; }
															#line 1864 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<Tr> tr;
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	int64_t delta;
															#line 338 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename Tr::template FutureT<Optional<Value>> countVal;
															#line 339 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename Tr::template FutureT<Optional<Value>> limitVal;
															#line 1943 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via updateThrottleCount()
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class UpdateThrottleCountActor final : public Actor<Void>, public ActorCallback< UpdateThrottleCountActor<Tr>, 0, Void >, public FastAllocated<UpdateThrottleCountActor<Tr>>, public UpdateThrottleCountActorState<Tr, UpdateThrottleCountActor<Tr>> {
															#line 1950 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
	using FastAllocated<UpdateThrottleCountActor<Tr>>::operator new;
	using FastAllocated<UpdateThrottleCountActor<Tr>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateThrottleCountActor<Tr>, 0, Void >;
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	UpdateThrottleCountActor(Reference<Tr> const& tr,int64_t const& delta) 
															#line 1961 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class Tr>
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Void> updateThrottleCount( Reference<Tr> const& tr, int64_t const& delta ) {
															#line 336 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<Void>(new UpdateThrottleCountActor<Tr>(tr, delta));
															#line 1990 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 368 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

															#line 1995 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via unthrottleMatchingThrottles()
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB, class UnthrottleMatchingThrottlesActor>
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class UnthrottleMatchingThrottlesActorState {
															#line 2001 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	UnthrottleMatchingThrottlesActorState(Reference<DB> const& db,KeyRef const& beginKey,KeyRef const& endKey,Optional<TransactionPriority> const& priority,bool const& onlyExpiredThrottles) 
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   beginKey(beginKey),
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   endKey(endKey),
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   priority(priority),
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   onlyExpiredThrottles(onlyExpiredThrottles),
															#line 375 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 377 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   begin(firstGreaterOrEqual(beginKey)),
															#line 378 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   end(firstGreaterOrEqual(endKey)),
															#line 380 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   removed(false)
															#line 2024 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 382 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 2039 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 383 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2069 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 386 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			f = tr->getRange(begin, end, 1000);
															#line 387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(f);
															#line 387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2077 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 1;
															#line 387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 0, RangeResult >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2082 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 429 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 429 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2106 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 4;
															#line 429 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 3, Void >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2111 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 388 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		unthrottledTags = 0;
															#line 389 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		uint64_t manualUnthrottledTags = 0;
															#line 390 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for( auto tag : tags ) {
															#line 391 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (onlyExpiredThrottles)
															#line 2132 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 392 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				double expirationTime = TagThrottleValue::fromValue(tag.value).expirationTime;
															#line 393 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (expirationTime == 0 || expirationTime > now())
															#line 2138 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				{
					continue;
				}
			}
															#line 398 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			TagThrottleKey key = TagThrottleKey::fromKey(tag.key);
															#line 399 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (priority.present() && key.priority != priority.get())
															#line 2147 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
				continue;
			}
															#line 403 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (key.throttleType == TagThrottleType::MANUAL)
															#line 2153 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 404 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				++manualUnthrottledTags;
															#line 2157 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			}
															#line 407 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			removed = true;
															#line 408 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			tr->clear(tag.key);
															#line 409 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			unthrottledTags++;
															#line 2165 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 412 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (manualUnthrottledTags > 0)
															#line 2169 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 413 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, -manualUnthrottledTags);
															#line 413 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2175 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 2;
															#line 413 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 1, Void >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2180 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tags = __tags;
															#line 2194 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 416 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (unthrottledTags > 0)
															#line 2261 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 417 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			signalThrottleChange(tr);
															#line 2265 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 420 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 420 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2271 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<UnthrottleMatchingThrottlesActor*>(this)->actor_wait_state = 3;
															#line 420 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< UnthrottleMatchingThrottlesActor, 2, Void >*>(static_cast<UnthrottleMatchingThrottlesActor*>(this)));
															#line 2276 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 422 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!tags.more)
															#line 2360 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 423 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (!static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV<bool>::futures) { (void)(removed); this->~UnthrottleMatchingThrottlesActorState(); static_cast<UnthrottleMatchingThrottlesActor*>(this)->destroy(); return 0; }
															#line 2364 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV< bool >::value()) bool(std::move(removed)); // state_var_RVO
			this->~UnthrottleMatchingThrottlesActorState();
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 426 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		ASSERT(tags.size() > 0);
															#line 427 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		begin = KeySelector(firstGreaterThan(tags[tags.size() - 1].key), tags.arena());
															#line 2374 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		loopDepth = a_body1loopBody1cont10cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 422 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!tags.more)
															#line 2383 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 423 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (!static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV<bool>::futures) { (void)(removed); this->~UnthrottleMatchingThrottlesActorState(); static_cast<UnthrottleMatchingThrottlesActor*>(this)->destroy(); return 0; }
															#line 2387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			new (&static_cast<UnthrottleMatchingThrottlesActor*>(this)->SAV< bool >::value()) bool(std::move(removed)); // state_var_RVO
			this->~UnthrottleMatchingThrottlesActorState();
			static_cast<UnthrottleMatchingThrottlesActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 426 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		ASSERT(tags.size() > 0);
															#line 427 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		begin = KeySelector(firstGreaterThan(tags[tags.size() - 1].key), tags.arena());
															#line 2397 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	KeyRef beginKey;
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	KeyRef endKey;
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Optional<TransactionPriority> priority;
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	bool onlyExpiredThrottles;
															#line 375 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 377 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	KeySelector begin;
															#line 378 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	KeySelector end;
															#line 380 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	bool removed;
															#line 386 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> f;
															#line 387 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	RangeResult tags;
															#line 388 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	uint64_t unthrottledTags;
															#line 2577 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via unthrottleMatchingThrottles()
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class UnthrottleMatchingThrottlesActor final : public Actor<bool>, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 0, RangeResult >, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 1, Void >, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 2, Void >, public ActorCallback< UnthrottleMatchingThrottlesActor<DB>, 3, Void >, public FastAllocated<UnthrottleMatchingThrottlesActor<DB>>, public UnthrottleMatchingThrottlesActorState<DB, UnthrottleMatchingThrottlesActor<DB>> {
															#line 2584 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	UnthrottleMatchingThrottlesActor(Reference<DB> const& db,KeyRef const& beginKey,KeyRef const& endKey,Optional<TransactionPriority> const& priority,bool const& onlyExpiredThrottles) 
															#line 2598 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<bool> unthrottleMatchingThrottles( Reference<DB> const& db, KeyRef const& beginKey, KeyRef const& endKey, Optional<TransactionPriority> const& priority, bool const& onlyExpiredThrottles ) {
															#line 369 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<bool>(new UnthrottleMatchingThrottlesActor<DB>(db, beginKey, endKey, priority, onlyExpiredThrottles));
															#line 2630 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 433 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

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

															#line 2657 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via unthrottleTags()
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB, class UnthrottleTagsActor>
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class UnthrottleTagsActorState {
															#line 2663 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	UnthrottleTagsActorState(Reference<DB> const& db,TagSet const& tags,Optional<TagThrottleType> const& throttleType,Optional<TransactionPriority> const& priority) 
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tags(tags),
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   throttleType(throttleType),
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   priority(priority),
															#line 461 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 463 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   keys()
															#line 2680 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 464 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			for( auto p : allTransactionPriorities ) {
															#line 465 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (!priority.present() || priority.get() == p)
															#line 2697 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				{
															#line 466 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					if (!throttleType.present() || throttleType.get() == TagThrottleType::AUTO)
															#line 2701 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					{
															#line 467 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
						keys.push_back(TagThrottleKey(tags, TagThrottleType::AUTO, p).toKey());
															#line 2705 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					}
															#line 469 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					if (!throttleType.present() || throttleType.get() == TagThrottleType::MANUAL)
															#line 2709 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					{
															#line 470 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
						keys.push_back(TagThrottleKey(tags, TagThrottleType::MANUAL, p).toKey());
															#line 2713 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
					}
				}
			}
															#line 475 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			removed = false;
															#line 477 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 2721 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 478 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2751 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 480 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			valueFutures = std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>>();
															#line 481 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			values = std::vector<Future<Optional<Value>>>();
															#line 482 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			values.reserve(keys.size());
															#line 483 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			for( auto key : keys ) {
															#line 484 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				valueFutures.push_back(tr->get(key));
															#line 485 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				values.push_back(safeThreadFutureToFuture(valueFutures.back()));
															#line 2765 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			}
															#line 488 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_0 = waitForAll(values);
															#line 488 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2771 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 1;
															#line 488 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 0, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2776 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 517 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 517 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2800 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 4;
															#line 517 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 3, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2805 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 490 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		int delta = 0;
															#line 491 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for(int i = 0;i < values.size();++i) {
															#line 492 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (values[i].get().present())
															#line 2824 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 493 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (TagThrottleKey::fromKey(keys[i]).throttleType == TagThrottleType::MANUAL)
															#line 2828 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				{
															#line 494 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					delta -= 1;
															#line 2832 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				}
															#line 497 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				tr->clear(keys[i]);
															#line 503 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				removed = true;
															#line 2838 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			}
		}
															#line 507 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (delta != 0)
															#line 2843 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 508 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, delta);
															#line 508 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2849 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 508 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 1, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2854 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 490 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		int delta = 0;
															#line 491 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		for(int i = 0;i < values.size();++i) {
															#line 492 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (values[i].get().present())
															#line 2872 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 493 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (TagThrottleKey::fromKey(keys[i]).throttleType == TagThrottleType::MANUAL)
															#line 2876 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				{
															#line 494 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
					delta -= 1;
															#line 2880 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				}
															#line 497 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				tr->clear(keys[i]);
															#line 503 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				removed = true;
															#line 2886 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			}
		}
															#line 507 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (delta != 0)
															#line 2891 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 508 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, delta);
															#line 508 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2897 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 508 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 1, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2902 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 510 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (removed)
															#line 2979 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 511 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			signalThrottleChange(tr);
															#line 512 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 512 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<UnthrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2987 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<UnthrottleTagsActor*>(this)->actor_wait_state = 3;
															#line 512 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< UnthrottleTagsActor, 2, Void >*>(static_cast<UnthrottleTagsActor*>(this)));
															#line 2992 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 515 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<UnthrottleTagsActor*>(this)->SAV<bool>::futures) { (void)(removed); this->~UnthrottleTagsActorState(); static_cast<UnthrottleTagsActor*>(this)->destroy(); return 0; }
															#line 3081 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	TagSet tags;
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Optional<TagThrottleType> throttleType;
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Optional<TransactionPriority> priority;
															#line 461 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 463 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	std::vector<Key> keys;
															#line 475 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	bool removed;
															#line 480 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>> valueFutures;
															#line 481 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	std::vector<Future<Optional<Value>>> values;
															#line 3257 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via unthrottleTags()
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class UnthrottleTagsActor final : public Actor<bool>, public ActorCallback< UnthrottleTagsActor<DB>, 0, Void >, public ActorCallback< UnthrottleTagsActor<DB>, 1, Void >, public ActorCallback< UnthrottleTagsActor<DB>, 2, Void >, public ActorCallback< UnthrottleTagsActor<DB>, 3, Void >, public FastAllocated<UnthrottleTagsActor<DB>>, public UnthrottleTagsActorState<DB, UnthrottleTagsActor<DB>> {
															#line 3264 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	UnthrottleTagsActor(Reference<DB> const& db,TagSet const& tags,Optional<TagThrottleType> const& throttleType,Optional<TransactionPriority> const& priority) 
															#line 3278 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<bool> unthrottleTags( Reference<DB> const& db, TagSet const& tags, Optional<TagThrottleType> const& throttleType, Optional<TransactionPriority> const& priority ) {
															#line 456 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<bool>(new UnthrottleTagsActor<DB>(db, tags, throttleType, priority));
															#line 3310 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 521 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

															#line 3315 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via throttleTags()
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB, class ThrottleTagsActor>
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class ThrottleTagsActorState {
															#line 3321 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	ThrottleTagsActorState(Reference<DB> const& db,TagSet const& tags,double const& tpsRate,double const& initialDuration,TagThrottleType const& throttleType,TransactionPriority const& priority,Optional<double> const& expirationTime = Optional<double>(),Optional<TagThrottledReason> const& reason = Optional<TagThrottledReason>()) 
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tags(tags),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tpsRate(tpsRate),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   initialDuration(initialDuration),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   throttleType(throttleType),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   priority(priority),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   expirationTime(expirationTime),
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   reason(reason),
															#line 531 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction()),
															#line 532 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   key(TagThrottleKey(tags, throttleType, priority).toKey())
															#line 3346 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 534 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			ASSERT(initialDuration > 0);
															#line 536 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (throttleType == TagThrottleType::MANUAL)
															#line 3363 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 537 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				reason = TagThrottledReason::MANUAL;
															#line 3367 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			}
															#line 539 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			TagThrottleValue throttle(tpsRate, expirationTime.present() ? expirationTime.get() : 0, initialDuration, reason.present() ? reason.get() : TagThrottledReason::UNSET);
															#line 543 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			BinaryWriter wr(IncludeVersion(ProtocolVersion::withTagThrottleValueReason()));
															#line 544 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			wr << throttle;
															#line 545 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			value = wr.toValue();
															#line 547 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 3379 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 548 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 3409 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 550 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (throttleType == TagThrottleType::MANUAL)
															#line 3413 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			{
															#line 552 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				oldThrottleF = tr->get(key);
															#line 553 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(oldThrottleF);
															#line 553 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3421 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 1;
															#line 553 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 0, Optional<Value> >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3426 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 568 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 568 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3455 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 4;
															#line 568 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 3, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3460 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 559 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->set(key, value);
															#line 561 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (throttleType == TagThrottleType::MANUAL)
															#line 3477 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 562 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			signalThrottleChange(tr);
															#line 3481 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		}
															#line 565 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 565 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3487 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 3;
															#line 565 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 2, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3492 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<Value> const& oldThrottle,int loopDepth) 
	{
															#line 554 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!oldThrottle.present())
															#line 3501 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 555 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, 1);
															#line 555 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3507 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 555 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 1, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3512 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 554 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!oldThrottle.present())
															#line 3526 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 555 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = updateThrottleCount(tr, 1);
															#line 555 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<ThrottleTagsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3532 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
			static_cast<ThrottleTagsActor*>(this)->actor_wait_state = 2;
															#line 555 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ThrottleTagsActor, 1, Void >*>(static_cast<ThrottleTagsActor*>(this)));
															#line 3537 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 566 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<ThrottleTagsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ThrottleTagsActorState(); static_cast<ThrottleTagsActor*>(this)->destroy(); return 0; }
															#line 3695 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		new (&static_cast<ThrottleTagsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ThrottleTagsActorState();
		static_cast<ThrottleTagsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 566 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<ThrottleTagsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ThrottleTagsActorState(); static_cast<ThrottleTagsActor*>(this)->destroy(); return 0; }
															#line 3707 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	TagSet tags;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	double tpsRate;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	double initialDuration;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	TagThrottleType throttleType;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	TransactionPriority priority;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Optional<double> expirationTime;
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Optional<TagThrottledReason> reason;
															#line 531 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 532 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Key key;
															#line 545 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Value value;
															#line 552 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<Optional<Value>> oldThrottleF;
															#line 3877 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via throttleTags()
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class ThrottleTagsActor final : public Actor<Void>, public ActorCallback< ThrottleTagsActor<DB>, 0, Optional<Value> >, public ActorCallback< ThrottleTagsActor<DB>, 1, Void >, public ActorCallback< ThrottleTagsActor<DB>, 2, Void >, public ActorCallback< ThrottleTagsActor<DB>, 3, Void >, public FastAllocated<ThrottleTagsActor<DB>>, public ThrottleTagsActorState<DB, ThrottleTagsActor<DB>> {
															#line 3884 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	ThrottleTagsActor(Reference<DB> const& db,TagSet const& tags,double const& tpsRate,double const& initialDuration,TagThrottleType const& throttleType,TransactionPriority const& priority,Optional<double> const& expirationTime = Optional<double>(),Optional<TagThrottledReason> const& reason = Optional<TagThrottledReason>()) 
															#line 3898 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Void> throttleTags( Reference<DB> const& db, TagSet const& tags, double const& tpsRate, double const& initialDuration, TagThrottleType const& throttleType, TransactionPriority const& priority, Optional<double> const& expirationTime = Optional<double>(), Optional<TagThrottledReason> const& reason = Optional<TagThrottledReason>() ) {
															#line 522 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<Void>(new ThrottleTagsActor<DB>(db, tags, tpsRate, initialDuration, throttleType, priority, expirationTime, reason));
															#line 3930 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 572 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

															#line 3935 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
// This generated class is to be used only via enableAuto()
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB, class EnableAutoActor>
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class EnableAutoActorState {
															#line 3941 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
public:
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	EnableAutoActorState(Reference<DB> const& db,bool const& enabled) 
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		 : db(db),
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   enabled(enabled),
															#line 575 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		   tr(db->createTransaction())
															#line 3952 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 577 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			;
															#line 3967 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 578 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 3997 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		try {
															#line 581 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			valueF = tr->get(tagThrottleAutoEnabledKey);
															#line 583 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(valueF);
															#line 583 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4005 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 1;
															#line 583 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 0, Optional<Value> >*>(static_cast<EnableAutoActor*>(this)));
															#line 4010 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 593 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 593 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4034 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 3;
															#line 593 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 2, Void >*>(static_cast<EnableAutoActor*>(this)));
															#line 4039 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 584 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!value.present() || (enabled && value.get() != LiteralStringRef("1")) || (!enabled && value.get() != LiteralStringRef("0")))
															#line 4054 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 586 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			tr->set(tagThrottleAutoEnabledKey, LiteralStringRef(enabled ? "1" : "0"));
															#line 587 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			signalThrottleChange<typename DB::TransactionT>(tr);
															#line 589 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 589 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4064 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 2;
															#line 589 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 1, Void >*>(static_cast<EnableAutoActor*>(this)));
															#line 4069 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 584 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!value.present() || (enabled && value.get() != LiteralStringRef("1")) || (!enabled && value.get() != LiteralStringRef("0")))
															#line 4083 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
		{
															#line 586 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			tr->set(tagThrottleAutoEnabledKey, LiteralStringRef(enabled ? "1" : "0"));
															#line 587 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			signalThrottleChange<typename DB::TransactionT>(tr);
															#line 589 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 589 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			if (static_cast<EnableAutoActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4093 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<EnableAutoActor*>(this)->actor_wait_state = 2;
															#line 589 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< EnableAutoActor, 1, Void >*>(static_cast<EnableAutoActor*>(this)));
															#line 4098 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 591 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
		if (!static_cast<EnableAutoActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EnableAutoActorState(); static_cast<EnableAutoActor*>(this)->destroy(); return 0; }
															#line 4175 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<DB> db;
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	bool enabled;
															#line 575 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 581 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	typename DB::TransactionT::template FutureT<Optional<Value>> valueF;
															#line 4341 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
};
// This generated class is to be used only via enableAuto()
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
class EnableAutoActor final : public Actor<Void>, public ActorCallback< EnableAutoActor<DB>, 0, Optional<Value> >, public ActorCallback< EnableAutoActor<DB>, 1, Void >, public ActorCallback< EnableAutoActor<DB>, 2, Void >, public FastAllocated<EnableAutoActor<DB>>, public EnableAutoActorState<DB, EnableAutoActor<DB>> {
															#line 4348 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	EnableAutoActor(Reference<DB> const& db,bool const& enabled) 
															#line 4361 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
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
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
template <class DB>
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
[[nodiscard]] Future<Void> enableAuto( Reference<DB> const& db, bool const& enabled ) {
															#line 573 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"
	return Future<Void>(new EnableAutoActor<DB>(db, enabled));
															#line 4392 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.h"
}

#line 597 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.h"

}; // namespace ThrottleApi

template <class Value>
using TransactionTagMap = std::unordered_map<TransactionTag, Value, std::hash<TransactionTagRef>>;

template <class Value>
using PrioritizedTransactionTagMap = std::map<TransactionPriority, TransactionTagMap<Value>>;

template <class Value>
using UIDTransactionTagMap = std::unordered_map<UID, TransactionTagMap<Value>>;

#include "flow/unactorcompiler.h"
#endif
