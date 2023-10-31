#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
/*
 * TagThrottle.actor.cpp
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

#include "fdbclient/CommitProxyInterface.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/TagThrottle.actor.h"
#include "fdbclient/Tuple.h"

#include "flow/actorcompiler.h" // has to be last include

double const ClientTagThrottleLimits::NO_EXPIRATION = std::numeric_limits<double>::max();

void TagSet::addTag(TransactionTagRef tag) {
	ASSERT(CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH < 256); // Tag length is encoded with a single byte
	ASSERT(CLIENT_KNOBS->MAX_TAGS_PER_TRANSACTION < 256); // Number of tags is encoded with a single byte

	if (tag.size() > CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH) {
		throw tag_too_long();
	}
	if (tags.size() >= CLIENT_KNOBS->MAX_TAGS_PER_TRANSACTION) {
		throw too_many_tags();
	}

	TransactionTagRef tagRef(arena, tag);
	auto it = find(tags.begin(), tags.end(), tagRef);
	if (it == tags.end()) {
		tags.push_back(std::move(tagRef));
		bytes += tag.size();
	}
}

size_t TagSet::size() const {
	return tags.size();
}

std::string TagSet::toString(Capitalize capitalize) const {
	ASSERT(!tags.empty());
	if (tags.size() == 1) {
		std::string start = capitalize ? "Tag" : "tag";
		return format("%s `%s'", start.c_str(), tags[0].toString().c_str());
	}
	std::string result = capitalize ? "Tags (" : "tags (";
	for (int index = 0; index < tags.size() - 1; ++index) {
		result += format("`%s', ", tags[index].toString().c_str());
	}
	return result + format("`%s')", tags.back().toString().c_str());
}

// Format for throttle key:
//
// tagThrottleKeysPrefix + [auto-throttled (1-byte 0/1)] + [priority (1-byte)] + [tag list]
// tag list consists of 1 or more consecutive tags, each encoded as:
// tag.size() (1 byte) + tag's bytes. For example, tag 'foo' is: \x03foo
// The tags are listed in sorted order
//
// Currently, the throttle API supports only 1 tag per throttle
Key TagThrottleKey::toKey() const {
	ASSERT(CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH < 256);
	ASSERT(tags.size() > 0);

	ASSERT(tags.size() == 1); // SOMEDAY: support multiple tags per throttle

	int size = tagThrottleKeysPrefix.size() + tags.size() + 2;
	for (auto tag : tags) {
		ASSERT(tag.size() <= CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH);
		size += tag.size();
	}

	Key result;

	uint8_t* str = new (result.arena()) uint8_t[size];
	result.contents() = StringRef(str, size);

	memcpy(str, tagThrottleKeysPrefix.begin(), tagThrottleKeysPrefix.size());
	str += tagThrottleKeysPrefix.size();

	*(str++) = (uint8_t)throttleType;
	*(str++) = (uint8_t)priority;

	for (auto tag : tags) {
		*(str++) = (uint8_t)tag.size();
		if (tag.size() > 0) {
			memcpy(str, tag.begin(), tag.size());
			str += tag.size();
		}
	}

	return result;
}

TagThrottleKey TagThrottleKey::fromKey(const KeyRef& key) {
	const uint8_t* str = key.substr(tagThrottleKeysPrefix.size()).begin();
	TagThrottleType throttleType = TagThrottleType(*(str++));
	TransactionPriority priority = TransactionPriority(*(str++));
	TagSet tags;

	while (str < key.end()) {
		uint8_t size = *(str++);
		tags.addTag(TransactionTagRef(str, size));
		str += size;
	}

	return TagThrottleKey(tags, throttleType, priority);
}

TagThrottleValue TagThrottleValue::fromValue(const ValueRef& value) {
	TagThrottleValue throttleValue;
	BinaryReader reader(value, IncludeVersion(ProtocolVersion::withTagThrottleValueReason()));
	reader >> throttleValue;
	return throttleValue;
}

KeyRangeRef const tagQuotaKeys = KeyRangeRef("\xff/tagQuota/"_sr, "\xff/tagQuota0"_sr);
KeyRef const tagQuotaPrefix = tagQuotaKeys.begin;

Key ThrottleApi::getTagQuotaKey(TransactionTagRef tag) {
	return tag.withPrefix(tagQuotaPrefix);
}

bool ThrottleApi::TagQuotaValue::isValid() const {
	return reservedQuota <= totalQuota && reservedQuota >= 0;
}

Value ThrottleApi::TagQuotaValue::toValue() const {
	return Tuple::makeTuple(reservedQuota, totalQuota).pack();
}

ThrottleApi::TagQuotaValue ThrottleApi::TagQuotaValue::fromValue(ValueRef value) {
	auto tuple = Tuple::unpack(value);
	if (tuple.size() != 2) {
		throw invalid_throttle_quota_value();
	}
	TagQuotaValue result;
	try {
		result.reservedQuota = tuple.getInt(0);
		result.totalQuota = tuple.getInt(1);
	} catch (Error& e) {
		TraceEvent(SevWarnAlways, "TagQuotaValueFailedToDeserialize").error(e);
		throw invalid_throttle_quota_value();
	}
	if (!result.isValid()) {
		TraceEvent(SevWarnAlways, "TagQuotaValueInvalidQuotas")
		    .detail("ReservedQuota", result.reservedQuota)
		    .detail("TotalQuota", result.totalQuota);
		throw invalid_throttle_quota_value();
	}
	return result;
}

															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase168()
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
template <class FlowTestCase168Actor>
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
class FlowTestCase168ActorState {
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
public:
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
	FlowTestCase168ActorState(UnitTestParameters const& params) 
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
		 : params(params)
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase168", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase168ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase168", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				TagSet tagSet;
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				tagSet.addTag("a"_sr);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				ASSERT(tagSet.toString() == "tag `a'");
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				ASSERT(tagSet.toString(Capitalize::True) == "Tag `a'");
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
			}
			{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				TagSet tagSet;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				tagSet.addTag("a"_sr);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				tagSet.addTag("b"_sr);
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				auto tagString = tagSet.toString();
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				ASSERT(tagString == "tags (`a', `b')" || tagString == "tags (`b', `a')");
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				auto capitalizedTagString = tagSet.toString(Capitalize::True);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
				ASSERT(capitalizedTagString == "Tags (`a', `b')" || capitalizedTagString == "Tags (`b', `a')");
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
			}
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
			if (!static_cast<FlowTestCase168Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase168ActorState(); static_cast<FlowTestCase168Actor*>(this)->destroy(); return 0; }
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
			new (&static_cast<FlowTestCase168Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase168ActorState();
			static_cast<FlowTestCase168Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase168ActorState();
		static_cast<FlowTestCase168Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
	UnitTestParameters params;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase168()
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
class FlowTestCase168Actor final : public Actor<Void>, public FastAllocated<FlowTestCase168Actor>, public FlowTestCase168ActorState<FlowTestCase168Actor> {
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase168Actor>::operator new;
	using FastAllocated<FlowTestCase168Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
	FlowTestCase168Actor(UnitTestParameters const& params) 
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase168ActorState<FlowTestCase168Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase168", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase168");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase168", reinterpret_cast<unsigned long>(this), -1);

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
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
static Future<Void> flowTestCase168( UnitTestParameters const& params ) {
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
	return Future<Void>(new FlowTestCase168Actor(params));
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase168, "TagSet/toString")

#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/TagThrottle.actor.cpp"
