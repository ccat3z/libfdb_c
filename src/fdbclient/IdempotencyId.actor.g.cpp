#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
/*
 * IdempotencyId.actor.cpp
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

#include "fdbclient/IdempotencyId.actor.h"
#include "fdbclient/ReadYourWrites.h"
#include "fdbclient/SystemData.h"
#include "flow/BooleanParam.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // this has to be the last include

struct IdempotencyIdKVBuilderImpl {
	Optional<Version> commitVersion;
	Optional<uint8_t> batchIndexHighOrderByte;
	BinaryWriter value{ IncludeVersion() };
};

IdempotencyIdKVBuilder::IdempotencyIdKVBuilder() : impl(PImpl<IdempotencyIdKVBuilderImpl>::create()) {}

void IdempotencyIdKVBuilder::setCommitVersion(Version commitVersion) {
	impl->commitVersion = commitVersion;
}

void IdempotencyIdKVBuilder::add(const IdempotencyIdRef& id, uint16_t batchIndex) {
	ASSERT(id.valid());
	if (impl->batchIndexHighOrderByte.present()) {
		ASSERT((batchIndex >> 8) == impl->batchIndexHighOrderByte.get());
	} else {
		impl->batchIndexHighOrderByte = batchIndex >> 8;
		impl->value << int64_t(now());
	}
	StringRef s = id.asStringRefUnsafe();
	impl->value << uint8_t(s.size());
	impl->value.serializeBytes(s);
	impl->value << uint8_t(batchIndex); // Low order byte of batchIndex
}

Optional<KeyValue> IdempotencyIdKVBuilder::buildAndClear() {
	ASSERT(impl->commitVersion.present());
	if (!impl->batchIndexHighOrderByte.present()) {
		return {};
	}

	Value v = impl->value.toValue();

	KeyRef key =
	    makeIdempotencySingleKeyRange(v.arena(), impl->commitVersion.get(), impl->batchIndexHighOrderByte.get()).begin;

	impl->value = BinaryWriter(IncludeVersion());
	impl->batchIndexHighOrderByte = Optional<uint8_t>();

	Optional<KeyValue> result = KeyValue();
	result.get().arena() = v.arena();
	result.get().key = key;
	result.get().value = v;
	return result;
}

IdempotencyIdKVBuilder::~IdempotencyIdKVBuilder() = default;

Optional<CommitResult> kvContainsIdempotencyId(const KeyValueRef& kv, const IdempotencyIdRef& id) {
	ASSERT(id.valid());
	StringRef needle = id.asStringRefUnsafe();
	StringRef haystack = kv.value;

#ifndef _WIN32
	// The common case is that the kv does not contain the idempotency id, so early return if memmem is available
	if (memmem(haystack.begin(), haystack.size(), needle.begin(), needle.size()) == nullptr) {
		return {};
	}
#endif

	// Even if id is a substring of value, it may still not actually contain it.
	BinaryReader reader(kv.value.begin(), kv.value.size(), IncludeVersion());
	int64_t timestamp; // ignored
	reader >> timestamp;
	while (!reader.empty()) {
		uint8_t length;
		reader >> length;
		StringRef candidate{ reinterpret_cast<const uint8_t*>(reader.readBytes(length)), length };
		uint8_t lowOrderBatchIndex;
		reader >> lowOrderBatchIndex;
		if (candidate == needle) {
			Version commitVersion;
			uint8_t highOrderBatchIndex;
			decodeIdempotencyKey(kv.key, commitVersion, highOrderBatchIndex);
			return CommitResult{ commitVersion,
				                 static_cast<uint16_t>((uint16_t(highOrderBatchIndex) << 8) |
				                                       uint16_t(lowOrderBatchIndex)) };
		}
	}
	return {};
}

void forceLinkIdempotencyIdTests() {}

namespace {
IdempotencyIdRef generate(Arena& arena) {
	int length = deterministicRandom()->coinflip() ? deterministicRandom()->randomInt(16, 256) : 16;
	StringRef id = makeString(length, arena);
	deterministicRandom()->randomBytes(mutateString(id), length);
	return IdempotencyIdRef(id);
}
} // namespace

															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase122()
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
template <class FlowTestCase122Actor>
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class FlowTestCase122ActorState {
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	FlowTestCase122ActorState(UnitTestParameters const& params) 
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		 : params(params)
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase122", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase122ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase122", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			Arena arena;
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			uint16_t firstBatchIndex = deterministicRandom()->randomUInt32();
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			firstBatchIndex &= 0xff7f;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			uint16_t batchIndex = firstBatchIndex;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			Version commitVersion = deterministicRandom()->randomInt64(0, std::numeric_limits<Version>::max());
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			std::vector<IdempotencyIdRef> idVector;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			std::unordered_set<IdempotencyIdRef> idSet;
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			IdempotencyIdKVBuilder builder;
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			builder.setCommitVersion(commitVersion);
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			for(int i = 0;i < 5;++i) {
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				auto id = generate(arena);
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				idVector.emplace_back(id);
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				idSet.emplace(id);
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				builder.add(id, batchIndex++);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			batchIndex = firstBatchIndex;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			Optional<KeyValue> kvOpt = builder.buildAndClear();
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			ASSERT(kvOpt.present());
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			const auto& kv = kvOpt.get();
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			ASSERT(idSet.size() == idVector.size());
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			for( const auto& id : idVector ) {
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				auto commitResult = kvContainsIdempotencyId(kv, id);
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ASSERT(commitResult.present());
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ASSERT(commitResult.get().commitVersion == commitVersion);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ASSERT(commitResult.get().batchIndex == batchIndex++);
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ASSERT(idSet.find(id) != idSet.end());
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				idSet.erase(id);
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ASSERT(idSet.find(id) == idSet.end());
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			ASSERT(idSet.size() == 0);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			ASSERT(!kvContainsIdempotencyId(kv, generate(arena)).present());
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (!static_cast<FlowTestCase122Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase122ActorState(); static_cast<FlowTestCase122Actor*>(this)->destroy(); return 0; }
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			new (&static_cast<FlowTestCase122Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase122ActorState();
			static_cast<FlowTestCase122Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase122ActorState();
		static_cast<FlowTestCase122Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	UnitTestParameters params;
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase122()
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class FlowTestCase122Actor final : public Actor<Void>, public FastAllocated<FlowTestCase122Actor>, public FlowTestCase122ActorState<FlowTestCase122Actor> {
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase122Actor>::operator new;
	using FastAllocated<FlowTestCase122Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	FlowTestCase122Actor(UnitTestParameters const& params) 
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase122ActorState<FlowTestCase122Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase122", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase122");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase122", reinterpret_cast<unsigned long>(this), -1);

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
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
static Future<Void> flowTestCase122( UnitTestParameters const& params ) {
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	return Future<Void>(new FlowTestCase122Actor(params));
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase122, "/fdbclient/IdempotencyId/basic")

#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"

															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase162()
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
template <class FlowTestCase162Actor>
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class FlowTestCase162ActorState {
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	FlowTestCase162ActorState(UnitTestParameters const& params) 
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		 : params(params)
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase162", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase162ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase162", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			ASSERT(ObjectReader::fromStringRef<IdempotencyIdRef>(ObjectWriter::toValue(IdempotencyIdRef(), Unversioned()), Unversioned()) == IdempotencyIdRef());
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			for(int i = 0;i < 1000;++i) {
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				Arena arena;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				auto id = generate(arena);
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				auto serialized = ObjectWriter::toValue(id, Unversioned());
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				IdempotencyIdRef t;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ObjectReader reader(serialized.begin(), Unversioned());
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				reader.deserialize(t);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
				ASSERT(t == id);
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (!static_cast<FlowTestCase162Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase162ActorState(); static_cast<FlowTestCase162Actor*>(this)->destroy(); return 0; }
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			new (&static_cast<FlowTestCase162Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase162ActorState();
			static_cast<FlowTestCase162Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase162ActorState();
		static_cast<FlowTestCase162Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	UnitTestParameters params;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase162()
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class FlowTestCase162Actor final : public Actor<Void>, public FastAllocated<FlowTestCase162Actor>, public FlowTestCase162ActorState<FlowTestCase162Actor> {
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase162Actor>::operator new;
	using FastAllocated<FlowTestCase162Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	FlowTestCase162Actor(UnitTestParameters const& params) 
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase162ActorState<FlowTestCase162Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase162", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase162");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase162", reinterpret_cast<unsigned long>(this), -1);

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
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
static Future<Void> flowTestCase162( UnitTestParameters const& params ) {
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	return Future<Void>(new FlowTestCase162Actor(params));
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase162, "/fdbclient/IdempotencyId/serialization")

#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"

KeyRangeRef makeIdempotencySingleKeyRange(Arena& arena, Version version, uint8_t highOrderBatchIndex) {
	static const auto size =
	    idempotencyIdKeys.begin.size() + sizeof(version) + sizeof(highOrderBatchIndex) + /*\x00*/ 1;

	StringRef second = makeString(size, arena);
	auto* dst = mutateString(second);

	memcpy(dst, idempotencyIdKeys.begin.begin(), idempotencyIdKeys.begin.size());
	dst += idempotencyIdKeys.begin.size();

	version = bigEndian64(version);
	memcpy(dst, &version, sizeof(version));
	dst += sizeof(version);

	*dst++ = highOrderBatchIndex;

	*dst++ = 0;

	ASSERT_EQ(dst - second.begin(), size);

	return KeyRangeRef(second.removeSuffix("\x00"_sr), second);
}

void decodeIdempotencyKey(KeyRef key, Version& commitVersion, uint8_t& highOrderBatchIndex) {
	BinaryReader reader(key, Unversioned());
	reader.readBytes(idempotencyIdKeys.begin.size());
	reader >> commitVersion;
	commitVersion = bigEndian64(commitVersion);
	reader >> highOrderBatchIndex;
}

FDB_BOOLEAN_PARAM(Oldest);

// Find the youngest or oldest idempotency id key in `range` (depending on `oldest`)
// Write the timestamp to `*time` and the version to `*version` when non-null.
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
namespace {
// This generated class is to be used only via getBoundary()
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
template <class GetBoundaryActor>
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class GetBoundaryActorState {
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	GetBoundaryActorState(Reference<ReadYourWritesTransaction> const& tr,KeyRange const& range,Oldest const& oldest,Version* const& version,int64_t* const& time) 
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		 : tr(tr),
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   range(range),
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   oldest(oldest),
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   version(version),
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   time(time)
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
	{
		fdb_probe_actor_create("getBoundary", reinterpret_cast<unsigned long>(this));

	}
	~GetBoundaryActorState() 
	{
		fdb_probe_actor_destroy("getBoundary", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(range, 1, Snapshot::False, oldest ? Reverse::False : Reverse::True);
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (static_cast<GetBoundaryActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetBoundaryActor*>(this)->actor_wait_state = 1;
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetBoundaryActor, 0, RangeResult >*>(static_cast<GetBoundaryActor*>(this)));
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
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
		this->~GetBoundaryActorState();
		static_cast<GetBoundaryActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& result,int loopDepth) 
	{
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!result.size())
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (!static_cast<GetBoundaryActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>()); this->~GetBoundaryActorState(); static_cast<GetBoundaryActor*>(this)->destroy(); return 0; }
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			new (&static_cast<GetBoundaryActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>());
			this->~GetBoundaryActorState();
			static_cast<GetBoundaryActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (version != nullptr)
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			BinaryReader rd(result.front().key, Unversioned());
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			rd.readBytes(idempotencyIdKeys.begin.size());
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			rd >> *version;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			*version = bigEndian64(*version);
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (time != nullptr)
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			BinaryReader rd(result.front().value, IncludeVersion());
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			rd >> *time;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!static_cast<GetBoundaryActor*>(this)->SAV<Optional<Key>>::futures) { (void)(result.front().key); this->~GetBoundaryActorState(); static_cast<GetBoundaryActor*>(this)->destroy(); return 0; }
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		new (&static_cast<GetBoundaryActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(result.front().key);
		this->~GetBoundaryActorState();
		static_cast<GetBoundaryActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && result,int loopDepth) 
	{
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!result.size())
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (!static_cast<GetBoundaryActor*>(this)->SAV<Optional<Key>>::futures) { (void)(Optional<Key>()); this->~GetBoundaryActorState(); static_cast<GetBoundaryActor*>(this)->destroy(); return 0; }
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			new (&static_cast<GetBoundaryActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(Optional<Key>());
			this->~GetBoundaryActorState();
			static_cast<GetBoundaryActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (version != nullptr)
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			BinaryReader rd(result.front().key, Unversioned());
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			rd.readBytes(idempotencyIdKeys.begin.size());
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			rd >> *version;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			*version = bigEndian64(*version);
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (time != nullptr)
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			BinaryReader rd(result.front().value, IncludeVersion());
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			rd >> *time;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!static_cast<GetBoundaryActor*>(this)->SAV<Optional<Key>>::futures) { (void)(result.front().key); this->~GetBoundaryActorState(); static_cast<GetBoundaryActor*>(this)->destroy(); return 0; }
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		new (&static_cast<GetBoundaryActor*>(this)->SAV< Optional<Key> >::value()) Optional<Key>(result.front().key);
		this->~GetBoundaryActorState();
		static_cast<GetBoundaryActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& result,int loopDepth) 
	{
		loopDepth = a_body1cont1(result, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && result,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetBoundaryActor*>(this)->actor_wait_state > 0) static_cast<GetBoundaryActor*>(this)->actor_wait_state = 0;
		static_cast<GetBoundaryActor*>(this)->ActorCallback< GetBoundaryActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetBoundaryActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getBoundary", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getBoundary", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetBoundaryActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getBoundary", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getBoundary", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetBoundaryActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getBoundary", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getBoundary", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	KeyRange range;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Oldest oldest;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Version* version;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t* time;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
};
// This generated class is to be used only via getBoundary()
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class GetBoundaryActor final : public Actor<Optional<Key>>, public ActorCallback< GetBoundaryActor, 0, RangeResult >, public FastAllocated<GetBoundaryActor>, public GetBoundaryActorState<GetBoundaryActor> {
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
	using FastAllocated<GetBoundaryActor>::operator new;
	using FastAllocated<GetBoundaryActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Key>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetBoundaryActor, 0, RangeResult >;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	GetBoundaryActor(Reference<ReadYourWritesTransaction> const& tr,KeyRange const& range,Oldest const& oldest,Version* const& version,int64_t* const& time) 
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		 : Actor<Optional<Key>>(),
		   GetBoundaryActorState<GetBoundaryActor>(tr, range, oldest, version, time)
	{
		fdb_probe_actor_enter("getBoundary", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getBoundary");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getBoundary", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetBoundaryActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
[[nodiscard]] static Future<Optional<Key>> getBoundary( Reference<ReadYourWritesTransaction> const& tr, KeyRange const& range, Oldest const& oldest, Version* const& version, int64_t* const& time ) {
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	return Future<Optional<Key>>(new GetBoundaryActor(tr, range, oldest, version, time));
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
}

#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"

															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
namespace {
// This generated class is to be used only via getIdmpKeyStatus()
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
template <class GetIdmpKeyStatusActor>
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class GetIdmpKeyStatusActorState {
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	GetIdmpKeyStatusActorState(Database const& db) 
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		 : db(db),
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   tr(makeReference<ReadYourWritesTransaction>(db)),
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   size(),
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   expired(),
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   expiredKey(idempotencyIdsExpiredVersion, Unversioned()),
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   oldestIdVersion(0),
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   oldestIdTime(0)
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
	{
		fdb_probe_actor_create("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this));

	}
	~GetIdmpKeyStatusActorState() 
	{
		fdb_probe_actor_destroy("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			;
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
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
		this->~GetIdmpKeyStatusActorState();
		static_cast<GetIdmpKeyStatusActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			StrictFuture<Void> __when_expr_0 = store(size, tr->getEstimatedRangeSizeBytes(idempotencyIdKeys)) && store(expired, expiredKey.getD(tr)) && success(getBoundary(tr, idempotencyIdKeys, Oldest::True, &oldestIdVersion, &oldestIdTime));
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state = 1;
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetIdmpKeyStatusActor, 0, Void >*>(static_cast<GetIdmpKeyStatusActor*>(this)));
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
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
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state = 2;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetIdmpKeyStatusActor, 1, Void >*>(static_cast<GetIdmpKeyStatusActor*>(this)));
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
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
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		JsonBuilderObject result;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		result["size_bytes"] = size;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (expired.expired != 0)
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["expired_version"] = expired.expired;
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (expired.expiredTime != 0)
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["expired_age"] = int64_t(now()) - expired.expiredTime;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (oldestIdVersion != 0)
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["oldest_id_version"] = oldestIdVersion;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (oldestIdTime != 0)
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["oldest_id_age"] = int64_t(now()) - oldestIdTime;
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!static_cast<GetIdmpKeyStatusActor*>(this)->SAV<JsonBuilderObject>::futures) { (void)(result); this->~GetIdmpKeyStatusActorState(); static_cast<GetIdmpKeyStatusActor*>(this)->destroy(); return 0; }
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		new (&static_cast<GetIdmpKeyStatusActor*>(this)->SAV< JsonBuilderObject >::value()) JsonBuilderObject(result);
		this->~GetIdmpKeyStatusActorState();
		static_cast<GetIdmpKeyStatusActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		JsonBuilderObject result;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		result["size_bytes"] = size;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (expired.expired != 0)
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["expired_version"] = expired.expired;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (expired.expiredTime != 0)
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["expired_age"] = int64_t(now()) - expired.expiredTime;
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (oldestIdVersion != 0)
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["oldest_id_version"] = oldestIdVersion;
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (oldestIdTime != 0)
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			result["oldest_id_age"] = int64_t(now()) - oldestIdTime;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!static_cast<GetIdmpKeyStatusActor*>(this)->SAV<JsonBuilderObject>::futures) { (void)(result); this->~GetIdmpKeyStatusActorState(); static_cast<GetIdmpKeyStatusActor*>(this)->destroy(); return 0; }
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		new (&static_cast<GetIdmpKeyStatusActor*>(this)->SAV< JsonBuilderObject >::value()) JsonBuilderObject(result);
		this->~GetIdmpKeyStatusActorState();
		static_cast<GetIdmpKeyStatusActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state > 0) static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state = 0;
		static_cast<GetIdmpKeyStatusActor*>(this)->ActorCallback< GetIdmpKeyStatusActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetIdmpKeyStatusActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetIdmpKeyStatusActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetIdmpKeyStatusActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 0);

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
	void a_exitChoose2() 
	{
		if (static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state > 0) static_cast<GetIdmpKeyStatusActor*>(this)->actor_wait_state = 0;
		static_cast<GetIdmpKeyStatusActor*>(this)->ActorCallback< GetIdmpKeyStatusActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetIdmpKeyStatusActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetIdmpKeyStatusActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetIdmpKeyStatusActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Database db;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t size;
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	IdempotencyIdsExpiredVersion expired;
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	KeyBackedObjectProperty<IdempotencyIdsExpiredVersion, _Unversioned> expiredKey;
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t oldestIdVersion;
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t oldestIdTime;
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
};
// This generated class is to be used only via getIdmpKeyStatus()
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class GetIdmpKeyStatusActor final : public Actor<JsonBuilderObject>, public ActorCallback< GetIdmpKeyStatusActor, 0, Void >, public ActorCallback< GetIdmpKeyStatusActor, 1, Void >, public FastAllocated<GetIdmpKeyStatusActor>, public GetIdmpKeyStatusActorState<GetIdmpKeyStatusActor> {
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
	using FastAllocated<GetIdmpKeyStatusActor>::operator new;
	using FastAllocated<GetIdmpKeyStatusActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<JsonBuilderObject>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetIdmpKeyStatusActor, 0, Void >;
friend struct ActorCallback< GetIdmpKeyStatusActor, 1, Void >;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	GetIdmpKeyStatusActor(Database const& db) 
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		 : Actor<JsonBuilderObject>(),
		   GetIdmpKeyStatusActorState<GetIdmpKeyStatusActor>(db)
	{
		fdb_probe_actor_enter("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getIdmpKeyStatus");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getIdmpKeyStatus", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetIdmpKeyStatusActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetIdmpKeyStatusActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
[[nodiscard]] Future<JsonBuilderObject> getIdmpKeyStatus( Database const& db ) {
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	return Future<JsonBuilderObject>(new GetIdmpKeyStatusActor(db));
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
}

#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"

															#line 1146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
namespace {
// This generated class is to be used only via cleanIdempotencyIds()
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
template <class CleanIdempotencyIdsActor>
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class CleanIdempotencyIdsActorState {
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	CleanIdempotencyIdsActorState(Database const& db,double const& minAgeSeconds) 
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		 : db(db),
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   minAgeSeconds(minAgeSeconds),
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   idmpKeySize(),
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   candidateDeleteSize(),
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   finalRange(),
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   tr(),
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   oldestKey(),
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   oldestVersion(),
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   oldestTime(),
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   candidateDeleteVersion(),
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   candidateDeleteTime(),
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		   candidateRangeToClean()
															#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
	{
		fdb_probe_actor_create("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this));

	}
	~CleanIdempotencyIdsActorState() 
	{
		fdb_probe_actor_destroy("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr = makeReference<ReadYourWritesTransaction>(db);
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			;
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
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
		this->~CleanIdempotencyIdsActorState();
		static_cast<CleanIdempotencyIdsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!static_cast<CleanIdempotencyIdsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanIdempotencyIdsActorState(); static_cast<CleanIdempotencyIdsActor*>(this)->destroy(); return 0; }
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		new (&static_cast<CleanIdempotencyIdsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CleanIdempotencyIdsActorState();
		static_cast<CleanIdempotencyIdsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			StrictFuture<Optional<Key>> __when_expr_0 = getBoundary(tr, idempotencyIdKeys, Oldest::True, &oldestVersion, &oldestTime);
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 1;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
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
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			TraceEvent("IdempotencyIdsCleanerError").error(e);
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			StrictFuture<Void> __when_expr_6 = tr->onError(e);
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_6.get(), loopDepth); };
			static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 7;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 6, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Key> const& oldestKey_,int loopDepth) 
	{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!oldestKey_.present())
															#line 1312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		oldestKey = oldestKey_.get();
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (int64_t(now()) - oldestTime < minAgeSeconds)
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_1 = store(idmpKeySize, tr->getEstimatedRangeSizeBytes(idempotencyIdKeys));
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 2;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 1, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Key> && oldestKey_,int loopDepth) 
	{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!oldestKey_.present())
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		oldestKey = oldestKey_.get();
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (int64_t(now()) - oldestTime < minAgeSeconds)
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_1 = store(idmpKeySize, tr->getEstimatedRangeSizeBytes(idempotencyIdKeys));
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 2;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 1, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Key> const& oldestKey_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(oldestKey_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Key> && oldestKey_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(oldestKey_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >*,Optional<Key> const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >*,Optional<Key> && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_2 = success( getBoundary(tr, idempotencyIdKeys, Oldest::False, &candidateDeleteVersion, &candidateDeleteTime));
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 3;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 2, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_2 = success( getBoundary(tr, idempotencyIdKeys, Oldest::False, &candidateDeleteVersion, &candidateDeleteTime));
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 3;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 2, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		;
															#line 1530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		;
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		finalRange = KeyRangeRef(idempotencyIdKeys.begin, candidateRangeToClean.end);
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (!finalRange.empty())
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->addReadConflictRange(finalRange);
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->clear(finalRange);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			tr->set( idempotencyIdsExpiredVersion, ObjectWriter::toValue(IdempotencyIdsExpiredVersion{ candidateDeleteVersion, candidateDeleteTime }, Unversioned()));
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			TraceEvent("IdempotencyIdsCleanerAttempt") .detail("Range", finalRange.toString()) .detail("IdmpKeySizeEstimate", idmpKeySize) .detail("ClearRangeSizeEstimate", candidateDeleteSize) .detail("ExpiredVersion", candidateDeleteVersion) .detail("ExpiredVersionAgeEstimate", static_cast<int64_t>(now()) - candidateDeleteTime);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			StrictFuture<Void> __when_expr_5 = tr->commit();
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont7when1(__when_expr_5.get(), loopDepth); };
			static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 6;
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 5, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont6loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1(int loopDepth) 
	{
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		candidateRangeToClean = KeyRangeRef(oldestKey, BinaryWriter::toValue(bigEndian64(candidateDeleteVersion + 1), Unversioned()) .withPrefix(idempotencyIdKeys.begin));
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (oldestVersion == candidateDeleteVersion)
															#line 1655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1loopBody1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_3 = success(getBoundary( tr, candidateRangeToClean, Oldest::False, &candidateDeleteVersion, &candidateDeleteTime));
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 4;
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 3, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		candidateRangeToClean = KeyRangeRef(oldestKey, BinaryWriter::toValue(bigEndian64(candidateDeleteVersion + 1), Unversioned()) .withPrefix(idempotencyIdKeys.begin));
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_4 = store(candidateDeleteSize, tr->getEstimatedRangeSizeBytes(candidateRangeToClean));
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 5;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 4, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		candidateRangeToClean = KeyRangeRef(oldestKey, BinaryWriter::toValue(bigEndian64(candidateDeleteVersion + 1), Unversioned()) .withPrefix(idempotencyIdKeys.begin));
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		StrictFuture<Void> __when_expr_4 = store(candidateDeleteSize, tr->getEstimatedRangeSizeBytes(candidateRangeToClean));
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont6loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 5;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CleanIdempotencyIdsActor, 4, Void >*>(static_cast<CleanIdempotencyIdsActor*>(this)));
															#line 1717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont6loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		int64_t youngestAge = int64_t(now()) - candidateDeleteTime;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		TraceEvent("IdempotencyIdsCleanerCandidateDelete") .detail("Range", candidateRangeToClean.toString()) .detail("IdmpKeySizeEstimate", idmpKeySize) .detail("YoungestIdAge", youngestAge) .detail("MinAgeSeconds", minAgeSeconds) .detail("ClearRangeSizeEstimate", candidateDeleteSize);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (youngestAge > minAgeSeconds)
															#line 1793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1loopBody1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		candidateDeleteVersion = (oldestVersion + candidateDeleteVersion) / 2;
															#line 1799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		int64_t youngestAge = int64_t(now()) - candidateDeleteTime;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		TraceEvent("IdempotencyIdsCleanerCandidateDelete") .detail("Range", candidateRangeToClean.toString()) .detail("IdmpKeySizeEstimate", idmpKeySize) .detail("YoungestIdAge", youngestAge) .detail("MinAgeSeconds", minAgeSeconds) .detail("ClearRangeSizeEstimate", candidateDeleteSize);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		if (youngestAge > minAgeSeconds)
															#line 1812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		{
			return a_body1loopBody1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
		candidateDeleteVersion = (oldestVersion + candidateDeleteVersion) / 2;
															#line 1818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont6loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont6loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont8(int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 5);

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
	void a_exitChoose7() 
	{
		if (static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state > 0) static_cast<CleanIdempotencyIdsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanIdempotencyIdsActor*>(this)->ActorCallback< CleanIdempotencyIdsActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< CleanIdempotencyIdsActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< CleanIdempotencyIdsActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Database db;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	double minAgeSeconds;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t idmpKeySize;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t candidateDeleteSize;
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	KeyRange finalRange;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Key oldestKey;
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Version oldestVersion;
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t oldestTime;
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	Version candidateDeleteVersion;
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	int64_t candidateDeleteTime;
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	KeyRange candidateRangeToClean;
															#line 2066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
};
// This generated class is to be used only via cleanIdempotencyIds()
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
class CleanIdempotencyIdsActor final : public Actor<Void>, public ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >, public ActorCallback< CleanIdempotencyIdsActor, 1, Void >, public ActorCallback< CleanIdempotencyIdsActor, 2, Void >, public ActorCallback< CleanIdempotencyIdsActor, 3, Void >, public ActorCallback< CleanIdempotencyIdsActor, 4, Void >, public ActorCallback< CleanIdempotencyIdsActor, 5, Void >, public ActorCallback< CleanIdempotencyIdsActor, 6, Void >, public FastAllocated<CleanIdempotencyIdsActor>, public CleanIdempotencyIdsActorState<CleanIdempotencyIdsActor> {
															#line 2071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
public:
	using FastAllocated<CleanIdempotencyIdsActor>::operator new;
	using FastAllocated<CleanIdempotencyIdsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >;
friend struct ActorCallback< CleanIdempotencyIdsActor, 1, Void >;
friend struct ActorCallback< CleanIdempotencyIdsActor, 2, Void >;
friend struct ActorCallback< CleanIdempotencyIdsActor, 3, Void >;
friend struct ActorCallback< CleanIdempotencyIdsActor, 4, Void >;
friend struct ActorCallback< CleanIdempotencyIdsActor, 5, Void >;
friend struct ActorCallback< CleanIdempotencyIdsActor, 6, Void >;
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	CleanIdempotencyIdsActor(Database const& db,double const& minAgeSeconds) 
															#line 2088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
		 : Actor<Void>(),
		   CleanIdempotencyIdsActorState<CleanIdempotencyIdsActor>(db, minAgeSeconds)
	{
		fdb_probe_actor_enter("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cleanIdempotencyIds");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cleanIdempotencyIds", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 0, Optional<Key> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< CleanIdempotencyIdsActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
[[nodiscard]] Future<Void> cleanIdempotencyIds( Database const& db, double const& minAgeSeconds ) {
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
	return Future<Void>(new CleanIdempotencyIdsActor(db, minAgeSeconds));
															#line 2122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.g.cpp"
}

#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/IdempotencyId.actor.cpp"
