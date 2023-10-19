#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
/*
 * CompressedInt.actor.cpp
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

#include "flow/UnitTest.h"
#include "flow/CompressedInt.h"

void printBitsLittle(size_t const size, void const* const ptr) {
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
		printf(" ");
	}
	puts("");
}

void printBitsBig(size_t const size, void const* const ptr) {
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = 0; i < size; ++i) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
		printf(" ");
	}
	puts("");
}

template <typename IntType>
void testCompressedInt(IntType n, StringRef rep = StringRef()) {
	BinaryWriter w(AssumeVersion(g_network->protocolVersion()));
	CompressedInt<IntType> cn(n);

	w << cn;
	if (rep.size() != 0 && w.toValue() != rep) {
		printf("WRONG ENCODING:\n");
		printf("  test value (BigE):  ");
		printBitsLittle(sizeof(IntType), &n);
		printf("  encoded:            ");
		printBitsBig(w.toValue().size(), w.toValue().begin());
		printf("    expected:         ");
		printBitsBig(rep.size(), rep.begin());
		puts("");
	} else
		rep = w.toValue();

	cn.value = 0;
	BinaryReader r(rep, AssumeVersion(g_network->protocolVersion()));
	r >> cn;

	if (cn.value != n) {
		printf("FAILURE:\n");
		printf("  test value: (Big): ");
		printBitsLittle(sizeof(IntType), &n);
		printf("  encoded:           ");
		printBitsBig(rep.size(), rep.begin());
		printf("  decoded value:     ");
		printBitsLittle(sizeof(IntType), &cn.value);
		puts("");
	}
}

															#line 90 "/src/flow/CompressedInt.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase88()
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
template <class FlowTestCase88Actor>
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
class FlowTestCase88ActorState {
															#line 97 "/src/flow/CompressedInt.actor.g.cpp"
public:
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
	FlowTestCase88ActorState(UnitTestParameters const& params) 
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
		 : params(params)
															#line 104 "/src/flow/CompressedInt.actor.g.cpp"
	{

	}
	~FlowTestCase88ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 89 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			testCompressedInt<int>(-2, LiteralStringRef("\x7e"));
															#line 90 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			testCompressedInt<int>(-1, LiteralStringRef("\x7f"));
															#line 91 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			testCompressedInt<int>(0, LiteralStringRef("\x80"));
															#line 92 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			testCompressedInt<int>(1, LiteralStringRef("\x81"));
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			testCompressedInt<int>(2, LiteralStringRef("\x82"));
															#line 94 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			testCompressedInt<int64_t>(0x4000000000000000, LiteralStringRef("\xFF\xC0\x40\x00\x00\x00\x00\x00\x00\x00"));
															#line 96 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			int64_t n = 0;
															#line 97 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			for(int i = 0;i < 10000000;++i) {
															#line 98 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
				n <<= 1;
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
				if (deterministicRandom()->coinflip())
															#line 135 "/src/flow/CompressedInt.actor.g.cpp"
				{
															#line 100 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
					n |= 1;
															#line 139 "/src/flow/CompressedInt.actor.g.cpp"
				}
															#line 101 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
				testCompressedInt<int64_t>(n);
															#line 102 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
				testCompressedInt<int32_t>(n);
															#line 103 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
				testCompressedInt<int16_t>(n);
															#line 147 "/src/flow/CompressedInt.actor.g.cpp"
			}
															#line 105 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
			if (!static_cast<FlowTestCase88Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase88ActorState(); static_cast<FlowTestCase88Actor*>(this)->destroy(); return 0; }
															#line 151 "/src/flow/CompressedInt.actor.g.cpp"
			new (&static_cast<FlowTestCase88Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase88ActorState();
			static_cast<FlowTestCase88Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase88ActorState();
		static_cast<FlowTestCase88Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
	UnitTestParameters params;
															#line 175 "/src/flow/CompressedInt.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase88()
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
class FlowTestCase88Actor final : public Actor<Void>, public FastAllocated<FlowTestCase88Actor>, public FlowTestCase88ActorState<FlowTestCase88Actor> {
															#line 180 "/src/flow/CompressedInt.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase88Actor>::operator new;
	using FastAllocated<FlowTestCase88Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
	FlowTestCase88Actor(UnitTestParameters const& params) 
															#line 190 "/src/flow/CompressedInt.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase88ActorState<FlowTestCase88Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase88");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
static Future<Void> flowTestCase88( UnitTestParameters const& params ) {
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
	return Future<Void>(new FlowTestCase88Actor(params));
															#line 215 "/src/flow/CompressedInt.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase88, "/flow/compressed_ints")

#line 107 "/tmp/fdb_c/foundationdb_ep/flow/CompressedInt.actor.cpp"
