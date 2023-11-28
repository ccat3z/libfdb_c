#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
/*
 * FlowTests.actor.cpp
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

// Unit tests for the flow language and libraries

#include "flow/ProtocolVersion.h"
#include "flow/UnitTest.h"
#include "flow/DeterministicRandom.h"
#include "flow/IThreadPool.h"
#include "flow/WriteOnlySet.h"
#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/IAsyncFile.h"
#include "flow/TLSConfig.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

void forceLinkFlowTests() {}

constexpr int firstLine = __LINE__;
															#line 38 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase36()
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase36Actor>
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase36ActorState {
															#line 45 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase36ActorState(UnitTestParameters const& params) 
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 52 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase36", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase36ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase36", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			;
															#line 67 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase36ActorState();
		static_cast<FlowTestCase36Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 50 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(LiteralStringRef(__FILE__).endsWith(LiteralStringRef("FlowTests.actor.cpp")));
															#line 51 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase36Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase36ActorState(); static_cast<FlowTestCase36Actor*>(this)->destroy(); return 0; }
															#line 92 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase36Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase36ActorState();
		static_cast<FlowTestCase36Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 39 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(__LINE__ == firstLine + 4);
															#line 40 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>(Void());
															#line 40 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase36Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 116 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase36Actor*>(this)->actor_wait_state = 1;
															#line 40 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase36Actor, 0, Void >*>(static_cast<FlowTestCase36Actor*>(this)));
															#line 121 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 44 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(__LINE__ == firstLine + 9);
															#line 45 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = Future<Void>(Void());
															#line 45 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase36Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 160 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase36Actor*>(this)->actor_wait_state = 2;
															#line 45 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase36Actor, 1, Void >*>(static_cast<FlowTestCase36Actor*>(this)));
															#line 165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 41 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 6);
															#line 42 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		return a_body1loopBody1Catch1(success(), loopDepth);
															#line 182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 41 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 6);
															#line 42 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		return a_body1loopBody1Catch1(success(), loopDepth);
															#line 192 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"

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
		if (static_cast<FlowTestCase36Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase36Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase36Actor*>(this)->ActorCallback< FlowTestCase36Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase36Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase36Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase36Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 46 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 11);
															#line 263 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 46 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 11);
															#line 272 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase36Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase36Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase36Actor*>(this)->ActorCallback< FlowTestCase36Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase36Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase36Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase36Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 342 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase36()
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase36Actor final : public Actor<Void>, public ActorCallback< FlowTestCase36Actor, 0, Void >, public ActorCallback< FlowTestCase36Actor, 1, Void >, public FastAllocated<FlowTestCase36Actor>, public FlowTestCase36ActorState<FlowTestCase36Actor> {
															#line 347 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase36Actor>::operator new;
	using FastAllocated<FlowTestCase36Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase36Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase36Actor, 1, Void >;
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase36Actor(UnitTestParameters const& params) 
															#line 359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase36ActorState<FlowTestCase36Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase36", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase36");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase36", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase36Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase36Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase36( UnitTestParameters const& params ) {
															#line 36 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase36Actor(params));
															#line 388 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase36, "/flow/actorcompiler/lineNumbers")

#line 53 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 394 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase54()
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase54Actor>
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase54ActorState {
															#line 401 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase54ActorState(UnitTestParameters const& params) 
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 408 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase54", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase54ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase54", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 55 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (FLOW_KNOBS->MAX_BUGGIFIED_DELAY == 0)
															#line 423 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 56 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (!static_cast<FlowTestCase54Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase54ActorState(); static_cast<FlowTestCase54Actor*>(this)->destroy(); return 0; }
															#line 427 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				new (&static_cast<FlowTestCase54Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase54ActorState();
				static_cast<FlowTestCase54Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 58 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			;
															#line 435 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase54ActorState();
		static_cast<FlowTestCase54Actor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 59 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		x = deterministicRandom()->random01();
															#line 60 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		last = 0;
															#line 61 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		f1 = map(delay(x), [last = &last](const Void&) { *last = 1; return Void(); });
															#line 65 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		f2 = map(delay(x), [last = &last](const Void&) { *last = 2; return Void(); });
															#line 69 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = f1 && f2;
															#line 69 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 475 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 1;
															#line 69 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase54Actor, 0, Void >*>(static_cast<FlowTestCase54Actor*>(this)));
															#line 480 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 70 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (last == 1)
															#line 489 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 71 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			TEST(true);
															#line 72 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase54Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase54ActorState(); static_cast<FlowTestCase54Actor*>(this)->destroy(); return 0; }
															#line 495 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase54Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase54ActorState();
			static_cast<FlowTestCase54Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 70 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (last == 1)
															#line 509 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 71 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			TEST(true);
															#line 72 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase54Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase54ActorState(); static_cast<FlowTestCase54Actor*>(this)->destroy(); return 0; }
															#line 515 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase54Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase54ActorState();
			static_cast<FlowTestCase54Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase54Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase54Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase54Actor*>(this)->ActorCallback< FlowTestCase54Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase54Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase54Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 59 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	double x;
															#line 60 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int last;
															#line 61 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f1;
															#line 65 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f2;
															#line 598 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase54()
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase54Actor final : public Actor<Void>, public ActorCallback< FlowTestCase54Actor, 0, Void >, public FastAllocated<FlowTestCase54Actor>, public FlowTestCase54ActorState<FlowTestCase54Actor> {
															#line 603 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase54Actor>::operator new;
	using FastAllocated<FlowTestCase54Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase54Actor, 0, Void >;
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase54Actor(UnitTestParameters const& params) 
															#line 614 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase54ActorState<FlowTestCase54Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase54", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase54");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase54", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase54Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase54( UnitTestParameters const& params ) {
															#line 54 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase54Actor(params));
															#line 642 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase54, "/flow/buggifiedDelay")

#line 76 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

template <class T, class Func, class ErrFunc, class CallbackType>
class LambdaCallback final : public CallbackType, public FastAllocated<LambdaCallback<T, Func, ErrFunc, CallbackType>> {
	Func func;
	ErrFunc errFunc;

	void fire(T const& t) override {
		CallbackType::remove();
		func(t);
		delete this;
	}
	void fire(T&& t) override {
		CallbackType::remove();
		func(std::move(t));
		delete this;
	}
	void error(Error e) override {
		CallbackType::remove();
		errFunc(e);
		delete this;
	}

public:
	LambdaCallback(Func&& f, ErrFunc&& e) : func(std::move(f)), errFunc(std::move(e)) {}
};

template <class T, class Func, class ErrFunc>
void onReady(Future<T>&& f, Func&& func, ErrFunc&& errFunc) {
	if (f.isReady()) {
		if (f.isError())
			errFunc(f.getError());
		else
			func(f.get());
	} else
		f.addCallbackAndClear(new LambdaCallback<T, Func, ErrFunc, Callback<T>>(std::move(func), std::move(errFunc)));
}

template <class T, class Func, class ErrFunc>
void onReady(FutureStream<T>&& f, Func&& func, ErrFunc&& errFunc) {
	if (f.isReady()) {
		if (f.isError())
			errFunc(f.getError());
		else
			func(f.pop());
	} else
		f.addCallbackAndClear(
		    new LambdaCallback<T, Func, ErrFunc, SingleCallback<T>>(std::move(func), std::move(errFunc)));
}

															#line 696 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via emptyVoidActor()
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class EmptyVoidActorActor>
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class EmptyVoidActorActorState {
															#line 703 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	EmptyVoidActorActorState() 
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	{
															#line 709 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("emptyVoidActor", reinterpret_cast<unsigned long>(this));

	}
	~EmptyVoidActorActorState() 
	{
		fdb_probe_actor_destroy("emptyVoidActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
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
		delete static_cast<EmptyVoidActorActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<EmptyVoidActorActor*>(this);
															#line 742 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
};
// This generated class is to be used only via emptyVoidActor()
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class EmptyVoidActorActor final : public Actor<void>, public FastAllocated<EmptyVoidActorActor>, public EmptyVoidActorActorState<EmptyVoidActorActor> {
															#line 751 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<EmptyVoidActorActor>::operator new;
	using FastAllocated<EmptyVoidActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	EmptyVoidActorActor() 
															#line 761 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<void>(),
		   EmptyVoidActorActorState<EmptyVoidActorActor>()
	{
		fdb_probe_actor_enter("emptyVoidActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("emptyVoidActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("emptyVoidActor", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static void emptyVoidActor(  ) {
															#line 125 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	new EmptyVoidActorActor();
															#line 780 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 126 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 785 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via emptyActor()
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class EmptyActorActor>
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class EmptyActorActorState {
															#line 792 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	EmptyActorActorState() 
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	{
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("emptyActor", reinterpret_cast<unsigned long>(this));

	}
	~EmptyActorActorState() 
	{
		fdb_probe_actor_destroy("emptyActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 128 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<EmptyActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EmptyActorActorState(); static_cast<EmptyActorActor*>(this)->destroy(); return 0; }
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<EmptyActorActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~EmptyActorActorState();
			static_cast<EmptyActorActor*>(this)->finishSendAndDelPromiseRef();
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
		this->~EmptyActorActorState();
		static_cast<EmptyActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
};
// This generated class is to be used only via emptyActor()
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class EmptyActorActor final : public Actor<Void>, public FastAllocated<EmptyActorActor>, public EmptyActorActorState<EmptyActorActor> {
															#line 838 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<EmptyActorActor>::operator new;
	using FastAllocated<EmptyActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	EmptyActorActor() 
															#line 848 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   EmptyActorActorState<EmptyActorActor>()
	{
		fdb_probe_actor_enter("emptyActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("emptyActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("emptyActor", reinterpret_cast<unsigned long>(this), -1);

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
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> emptyActor(  ) {
															#line 127 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new EmptyActorActor());
															#line 875 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 130 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 880 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via oneWaitVoidActor()
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class OneWaitVoidActorActor>
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class OneWaitVoidActorActorState {
															#line 887 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	OneWaitVoidActorActorState(Future<Void> const& f) 
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : f(f)
															#line 894 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("oneWaitVoidActor", reinterpret_cast<unsigned long>(this));

	}
	~OneWaitVoidActorActorState() 
	{
		fdb_probe_actor_destroy("oneWaitVoidActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 132 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 132 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 911 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			static_cast<OneWaitVoidActorActor*>(this)->actor_wait_state = 1;
															#line 132 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OneWaitVoidActorActor, 0, Void >*>(static_cast<OneWaitVoidActorActor*>(this)));
															#line 915 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		delete static_cast<OneWaitVoidActorActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

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
		if (static_cast<OneWaitVoidActorActor*>(this)->actor_wait_state > 0) static_cast<OneWaitVoidActorActor*>(this)->actor_wait_state = 0;
		static_cast<OneWaitVoidActorActor*>(this)->ActorCallback< OneWaitVoidActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OneWaitVoidActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OneWaitVoidActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OneWaitVoidActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<OneWaitVoidActorActor*>(this);
															#line 1012 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 1019 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via oneWaitVoidActor()
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class OneWaitVoidActorActor final : public Actor<void>, public ActorCallback< OneWaitVoidActorActor, 0, Void >, public FastAllocated<OneWaitVoidActorActor>, public OneWaitVoidActorActorState<OneWaitVoidActorActor> {
															#line 1024 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<OneWaitVoidActorActor>::operator new;
	using FastAllocated<OneWaitVoidActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< OneWaitVoidActorActor, 0, Void >;
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	OneWaitVoidActorActor(Future<Void> const& f) 
															#line 1035 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<void>(),
		   OneWaitVoidActorActorState<OneWaitVoidActorActor>(f)
	{
		fdb_probe_actor_enter("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("oneWaitVoidActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("oneWaitVoidActor", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static void oneWaitVoidActor( Future<Void> const& f ) {
															#line 131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	new OneWaitVoidActorActor(f);
															#line 1054 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 134 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 1059 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via oneWaitActor()
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class OneWaitActorActor>
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class OneWaitActorActorState {
															#line 1066 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	OneWaitActorActorState(Future<Void> const& f) 
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : f(f)
															#line 1073 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("oneWaitActor", reinterpret_cast<unsigned long>(this));

	}
	~OneWaitActorActorState() 
	{
		fdb_probe_actor_destroy("oneWaitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 136 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 136 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<OneWaitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1090 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OneWaitActorActor*>(this)->actor_wait_state = 1;
															#line 136 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OneWaitActorActor, 0, Void >*>(static_cast<OneWaitActorActor*>(this)));
															#line 1095 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~OneWaitActorActorState();
		static_cast<OneWaitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 137 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<OneWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OneWaitActorActorState(); static_cast<OneWaitActorActor*>(this)->destroy(); return 0; }
															#line 1118 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<OneWaitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OneWaitActorActorState();
		static_cast<OneWaitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 137 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<OneWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OneWaitActorActorState(); static_cast<OneWaitActorActor*>(this)->destroy(); return 0; }
															#line 1130 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<OneWaitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OneWaitActorActorState();
		static_cast<OneWaitActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<OneWaitActorActor*>(this)->actor_wait_state > 0) static_cast<OneWaitActorActor*>(this)->actor_wait_state = 0;
		static_cast<OneWaitActorActor*>(this)->ActorCallback< OneWaitActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OneWaitActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("oneWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("oneWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OneWaitActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("oneWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("oneWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OneWaitActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("oneWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("oneWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via oneWaitActor()
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class OneWaitActorActor final : public Actor<Void>, public ActorCallback< OneWaitActorActor, 0, Void >, public FastAllocated<OneWaitActorActor>, public OneWaitActorActorState<OneWaitActorActor> {
															#line 1208 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<OneWaitActorActor>::operator new;
	using FastAllocated<OneWaitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OneWaitActorActor, 0, Void >;
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	OneWaitActorActor(Future<Void> const& f) 
															#line 1219 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   OneWaitActorActorState<OneWaitActorActor>(f)
	{
		fdb_probe_actor_enter("oneWaitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("oneWaitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("oneWaitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OneWaitActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> oneWaitActor( Future<Void> const& f ) {
															#line 135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new OneWaitActorActor(f));
															#line 1247 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 139 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

Future<Void> g_cheese;
															#line 1253 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via cheeseWaitActor()
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class CheeseWaitActorActor>
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class CheeseWaitActorActorState {
															#line 1260 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	CheeseWaitActorActorState() 
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	{
															#line 1266 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("cheeseWaitActor", reinterpret_cast<unsigned long>(this));

	}
	~CheeseWaitActorActorState() 
	{
		fdb_probe_actor_destroy("cheeseWaitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 142 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_cheese;
															#line 142 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<CheeseWaitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1282 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheeseWaitActorActor*>(this)->actor_wait_state = 1;
															#line 142 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheeseWaitActorActor, 0, Void >*>(static_cast<CheeseWaitActorActor*>(this)));
															#line 1287 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~CheeseWaitActorActorState();
		static_cast<CheeseWaitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 143 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<CheeseWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheeseWaitActorActorState(); static_cast<CheeseWaitActorActor*>(this)->destroy(); return 0; }
															#line 1310 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<CheeseWaitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheeseWaitActorActorState();
		static_cast<CheeseWaitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 143 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<CheeseWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheeseWaitActorActorState(); static_cast<CheeseWaitActorActor*>(this)->destroy(); return 0; }
															#line 1322 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<CheeseWaitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheeseWaitActorActorState();
		static_cast<CheeseWaitActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<CheeseWaitActorActor*>(this)->actor_wait_state > 0) static_cast<CheeseWaitActorActor*>(this)->actor_wait_state = 0;
		static_cast<CheeseWaitActorActor*>(this)->ActorCallback< CheeseWaitActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheeseWaitActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cheeseWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cheeseWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheeseWaitActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cheeseWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cheeseWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheeseWaitActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cheeseWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cheeseWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
};
// This generated class is to be used only via cheeseWaitActor()
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class CheeseWaitActorActor final : public Actor<Void>, public ActorCallback< CheeseWaitActorActor, 0, Void >, public FastAllocated<CheeseWaitActorActor>, public CheeseWaitActorActorState<CheeseWaitActorActor> {
															#line 1397 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<CheeseWaitActorActor>::operator new;
	using FastAllocated<CheeseWaitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheeseWaitActorActor, 0, Void >;
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	CheeseWaitActorActor() 
															#line 1408 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   CheeseWaitActorActorState<CheeseWaitActorActor>()
	{
		fdb_probe_actor_enter("cheeseWaitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cheeseWaitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cheeseWaitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheeseWaitActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> cheeseWaitActor(  ) {
															#line 141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new CheeseWaitActorActor());
															#line 1436 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 145 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 1441 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via trivialVoidActor()
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class TrivialVoidActorActor>
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TrivialVoidActorActorState {
															#line 1448 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TrivialVoidActorActorState(int* const& result) 
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : result(result)
															#line 1455 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("trivialVoidActor", reinterpret_cast<unsigned long>(this));

	}
	~TrivialVoidActorActorState() 
	{
		fdb_probe_actor_destroy("trivialVoidActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 147 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			*result = 1;
															#line 1470 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = a_body1cont2(loopDepth);
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
		delete static_cast<TrivialVoidActorActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<TrivialVoidActorActor*>(this);
															#line 1492 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int* result;
															#line 1499 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via trivialVoidActor()
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TrivialVoidActorActor final : public Actor<void>, public FastAllocated<TrivialVoidActorActor>, public TrivialVoidActorActorState<TrivialVoidActorActor> {
															#line 1504 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TrivialVoidActorActor>::operator new;
	using FastAllocated<TrivialVoidActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TrivialVoidActorActor(int* const& result) 
															#line 1514 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<void>(),
		   TrivialVoidActorActorState<TrivialVoidActorActor>(result)
	{
		fdb_probe_actor_enter("trivialVoidActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("trivialVoidActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("trivialVoidActor", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static void trivialVoidActor( int* const& result ) {
															#line 146 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	new TrivialVoidActorActor(result);
															#line 1533 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 149 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 1538 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via return42Actor()
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class Return42ActorActor>
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Return42ActorActorState {
															#line 1545 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Return42ActorActorState() 
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	{
															#line 1551 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("return42Actor", reinterpret_cast<unsigned long>(this));

	}
	~Return42ActorActorState() 
	{
		fdb_probe_actor_destroy("return42Actor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 151 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<Return42ActorActor*>(this)->SAV<int>::futures) { (void)(42); this->~Return42ActorActorState(); static_cast<Return42ActorActor*>(this)->destroy(); return 0; }
															#line 1565 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<Return42ActorActor*>(this)->SAV< int >::value()) int(42);
			this->~Return42ActorActorState();
			static_cast<Return42ActorActor*>(this)->finishSendAndDelPromiseRef();
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
		this->~Return42ActorActorState();
		static_cast<Return42ActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
};
// This generated class is to be used only via return42Actor()
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Return42ActorActor final : public Actor<int>, public FastAllocated<Return42ActorActor>, public Return42ActorActorState<Return42ActorActor> {
															#line 1591 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Return42ActorActor>::operator new;
	using FastAllocated<Return42ActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Return42ActorActor() 
															#line 1601 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   Return42ActorActorState<Return42ActorActor>()
	{
		fdb_probe_actor_enter("return42Actor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("return42Actor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("return42Actor", reinterpret_cast<unsigned long>(this), -1);

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
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> return42Actor(  ) {
															#line 150 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Return42ActorActor());
															#line 1628 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 153 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 1633 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via voidWaitActor()
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class VoidWaitActorActor>
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class VoidWaitActorActorState {
															#line 1640 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	VoidWaitActorActorState(Future<int> const& in,int* const& result) 
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : in(in),
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   result(result)
															#line 1649 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("voidWaitActor", reinterpret_cast<unsigned long>(this));

	}
	~VoidWaitActorActorState() 
	{
		fdb_probe_actor_destroy("voidWaitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 155 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<int> __when_expr_0 = in;
															#line 155 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1666 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			static_cast<VoidWaitActorActor*>(this)->actor_wait_state = 1;
															#line 155 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< VoidWaitActorActor, 0, int >*>(static_cast<VoidWaitActorActor*>(this)));
															#line 1670 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		delete static_cast<VoidWaitActorActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int const& i,int loopDepth) 
	{
															#line 156 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		*result = i;
															#line 1692 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 156 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		*result = i;
															#line 1701 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1when1(int const& i,int loopDepth) 
	{
		loopDepth = a_body1cont1(i, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && i,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(i), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<VoidWaitActorActor*>(this)->actor_wait_state > 0) static_cast<VoidWaitActorActor*>(this)->actor_wait_state = 0;
		static_cast<VoidWaitActorActor*>(this)->ActorCallback< VoidWaitActorActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< VoidWaitActorActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("voidWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("voidWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< VoidWaitActorActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("voidWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("voidWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< VoidWaitActorActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("voidWaitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("voidWaitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<VoidWaitActorActor*>(this);
															#line 1773 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<int> in;
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int* result;
															#line 1782 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via voidWaitActor()
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class VoidWaitActorActor final : public Actor<void>, public ActorCallback< VoidWaitActorActor, 0, int >, public FastAllocated<VoidWaitActorActor>, public VoidWaitActorActorState<VoidWaitActorActor> {
															#line 1787 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<VoidWaitActorActor>::operator new;
	using FastAllocated<VoidWaitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< VoidWaitActorActor, 0, int >;
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	VoidWaitActorActor(Future<int> const& in,int* const& result) 
															#line 1798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<void>(),
		   VoidWaitActorActorState<VoidWaitActorActor>(in, result)
	{
		fdb_probe_actor_enter("voidWaitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("voidWaitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("voidWaitActor", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static void voidWaitActor( Future<int> const& in, int* const& result ) {
															#line 154 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	new VoidWaitActorActor(in, result);
															#line 1817 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 158 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 1822 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via addOneActor()
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class AddOneActorActor>
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class AddOneActorActorState {
															#line 1829 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	AddOneActorActorState(Future<int> const& in) 
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : in(in)
															#line 1836 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("addOneActor", reinterpret_cast<unsigned long>(this));

	}
	~AddOneActorActorState() 
	{
		fdb_probe_actor_destroy("addOneActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 160 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<int> __when_expr_0 = in;
															#line 160 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<AddOneActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1853 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AddOneActorActor*>(this)->actor_wait_state = 1;
															#line 160 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddOneActorActor, 0, int >*>(static_cast<AddOneActorActor*>(this)));
															#line 1858 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~AddOneActorActorState();
		static_cast<AddOneActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int const& i,int loopDepth) 
	{
															#line 161 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<AddOneActorActor*>(this)->SAV<int>::futures) { (void)(i + 1); this->~AddOneActorActorState(); static_cast<AddOneActorActor*>(this)->destroy(); return 0; }
															#line 1881 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<AddOneActorActor*>(this)->SAV< int >::value()) int(i + 1);
		this->~AddOneActorActorState();
		static_cast<AddOneActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 161 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<AddOneActorActor*>(this)->SAV<int>::futures) { (void)(i + 1); this->~AddOneActorActorState(); static_cast<AddOneActorActor*>(this)->destroy(); return 0; }
															#line 1893 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<AddOneActorActor*>(this)->SAV< int >::value()) int(i + 1);
		this->~AddOneActorActorState();
		static_cast<AddOneActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int const& i,int loopDepth) 
	{
		loopDepth = a_body1cont1(i, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && i,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(i), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AddOneActorActor*>(this)->actor_wait_state > 0) static_cast<AddOneActorActor*>(this)->actor_wait_state = 0;
		static_cast<AddOneActorActor*>(this)->ActorCallback< AddOneActorActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< AddOneActorActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("addOneActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addOneActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AddOneActorActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("addOneActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addOneActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AddOneActorActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("addOneActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addOneActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<int> in;
															#line 1966 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via addOneActor()
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class AddOneActorActor final : public Actor<int>, public ActorCallback< AddOneActorActor, 0, int >, public FastAllocated<AddOneActorActor>, public AddOneActorActorState<AddOneActorActor> {
															#line 1971 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<AddOneActorActor>::operator new;
	using FastAllocated<AddOneActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddOneActorActor, 0, int >;
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	AddOneActorActor(Future<int> const& in) 
															#line 1982 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   AddOneActorActorState<AddOneActorActor>(in)
	{
		fdb_probe_actor_enter("addOneActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("addOneActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("addOneActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AddOneActorActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> addOneActor( Future<int> const& in ) {
															#line 159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new AddOneActorActor(in));
															#line 2010 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 163 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 2015 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via chooseTwoActor()
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class ChooseTwoActorActor>
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class ChooseTwoActorActorState {
															#line 2022 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	ChooseTwoActorActorState(Future<Void> const& f,Future<Void> const& g) 
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : f(f),
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   g(g)
															#line 2031 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("chooseTwoActor", reinterpret_cast<unsigned long>(this));

	}
	~ChooseTwoActorActorState() 
	{
		fdb_probe_actor_destroy("chooseTwoActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 166 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<ChooseTwoActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2048 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 167 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = g;
															#line 2052 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ChooseTwoActorActor*>(this)->actor_wait_state = 1;
															#line 166 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChooseTwoActorActor, 0, Void >*>(static_cast<ChooseTwoActorActor*>(this)));
															#line 167 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChooseTwoActorActor, 1, Void >*>(static_cast<ChooseTwoActorActor*>(this)));
															#line 2059 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~ChooseTwoActorActorState();
		static_cast<ChooseTwoActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 169 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ChooseTwoActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChooseTwoActorActorState(); static_cast<ChooseTwoActorActor*>(this)->destroy(); return 0; }
															#line 2082 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ChooseTwoActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChooseTwoActorActorState();
		static_cast<ChooseTwoActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChooseTwoActorActor*>(this)->actor_wait_state > 0) static_cast<ChooseTwoActorActor*>(this)->actor_wait_state = 0;
		static_cast<ChooseTwoActorActor*>(this)->ActorCallback< ChooseTwoActorActor, 0, Void >::remove();
		static_cast<ChooseTwoActorActor*>(this)->ActorCallback< ChooseTwoActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChooseTwoActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChooseTwoActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChooseTwoActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChooseTwoActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChooseTwoActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChooseTwoActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> g;
															#line 2215 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via chooseTwoActor()
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class ChooseTwoActorActor final : public Actor<Void>, public ActorCallback< ChooseTwoActorActor, 0, Void >, public ActorCallback< ChooseTwoActorActor, 1, Void >, public FastAllocated<ChooseTwoActorActor>, public ChooseTwoActorActorState<ChooseTwoActorActor> {
															#line 2220 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<ChooseTwoActorActor>::operator new;
	using FastAllocated<ChooseTwoActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChooseTwoActorActor, 0, Void >;
friend struct ActorCallback< ChooseTwoActorActor, 1, Void >;
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	ChooseTwoActorActor(Future<Void> const& f,Future<Void> const& g) 
															#line 2232 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   ChooseTwoActorActorState<ChooseTwoActorActor>(f, g)
	{
		fdb_probe_actor_enter("chooseTwoActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("chooseTwoActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("chooseTwoActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChooseTwoActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> chooseTwoActor( Future<Void> const& f, Future<Void> const& g ) {
															#line 164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new ChooseTwoActorActor(f, g));
															#line 2260 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 171 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 2265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via consumeOneActor()
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class ConsumeOneActorActor>
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class ConsumeOneActorActorState {
															#line 2272 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	ConsumeOneActorActorState(FutureStream<int> const& in) 
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : in(in)
															#line 2279 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("consumeOneActor", reinterpret_cast<unsigned long>(this));

	}
	~ConsumeOneActorActorState() 
	{
		fdb_probe_actor_destroy("consumeOneActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			FutureStream<int> __when_expr_0 = in;
															#line 173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<ConsumeOneActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2296 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<ConsumeOneActorActor*>(this)->actor_wait_state = 1;
															#line 173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ConsumeOneActorActor, 0, int >*>(static_cast<ConsumeOneActorActor*>(this)));
															#line 2301 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~ConsumeOneActorActorState();
		static_cast<ConsumeOneActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int const& i,int loopDepth) 
	{
															#line 174 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ConsumeOneActorActor*>(this)->SAV<int>::futures) { (void)(i); this->~ConsumeOneActorActorState(); static_cast<ConsumeOneActorActor*>(this)->destroy(); return 0; }
															#line 2324 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ConsumeOneActorActor*>(this)->SAV< int >::value()) int(i);
		this->~ConsumeOneActorActorState();
		static_cast<ConsumeOneActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 174 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ConsumeOneActorActor*>(this)->SAV<int>::futures) { (void)(i); this->~ConsumeOneActorActorState(); static_cast<ConsumeOneActorActor*>(this)->destroy(); return 0; }
															#line 2336 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ConsumeOneActorActor*>(this)->SAV< int >::value()) int(i);
		this->~ConsumeOneActorActorState();
		static_cast<ConsumeOneActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int const& i,int loopDepth) 
	{
		loopDepth = a_body1cont1(i, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && i,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(i), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConsumeOneActorActor*>(this)->actor_wait_state > 0) static_cast<ConsumeOneActorActor*>(this)->actor_wait_state = 0;
		static_cast<ConsumeOneActorActor*>(this)->ActorSingleCallback< ConsumeOneActorActor, 0, int >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ConsumeOneActorActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("consumeOneActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("consumeOneActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ConsumeOneActorActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("consumeOneActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("consumeOneActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< ConsumeOneActorActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("consumeOneActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("consumeOneActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FutureStream<int> in;
															#line 2409 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via consumeOneActor()
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class ConsumeOneActorActor final : public Actor<int>, public ActorSingleCallback< ConsumeOneActorActor, 0, int >, public FastAllocated<ConsumeOneActorActor>, public ConsumeOneActorActorState<ConsumeOneActorActor> {
															#line 2414 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<ConsumeOneActorActor>::operator new;
	using FastAllocated<ConsumeOneActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ConsumeOneActorActor, 0, int >;
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	ConsumeOneActorActor(FutureStream<int> const& in) 
															#line 2425 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   ConsumeOneActorActorState<ConsumeOneActorActor>(in)
	{
		fdb_probe_actor_enter("consumeOneActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("consumeOneActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("consumeOneActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< ConsumeOneActorActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> consumeOneActor( FutureStream<int> const& in ) {
															#line 172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new ConsumeOneActorActor(in));
															#line 2453 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 176 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 2458 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via sumActor()
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class SumActorActor>
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class SumActorActorState {
															#line 2465 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	SumActorActorState(FutureStream<int> const& in) 
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : in(in),
															#line 178 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   total(0)
															#line 2474 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("sumActor", reinterpret_cast<unsigned long>(this));

	}
	~SumActorActorState() 
	{
		fdb_probe_actor_destroy("sumActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 180 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				;
															#line 2490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
		this->~SumActorActorState();
		static_cast<SumActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 188 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<SumActorActor*>(this)->SAV<int>::futures) { (void)(total); this->~SumActorActorState(); static_cast<SumActorActor*>(this)->destroy(); return 0; }
															#line 2519 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<SumActorActor*>(this)->SAV< int >::value()) int(std::move(total)); // state_var_RVO
		this->~SumActorActorState();
		static_cast<SumActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 185 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (e.code() != error_code_end_of_stream)
															#line 2532 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 186 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 2536 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

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
															#line 181 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		FutureStream<int> __when_expr_0 = in;
															#line 181 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<SumActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2561 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<SumActorActor*>(this)->actor_wait_state = 1;
															#line 181 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< SumActorActor, 0, int >*>(static_cast<SumActorActor*>(this)));
															#line 2566 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int const& i,int loopDepth) 
	{
															#line 182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		total += i;
															#line 2575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int && i,int loopDepth) 
	{
															#line 182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		total += i;
															#line 2584 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(int const& i,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(i, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(int && i,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(i), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SumActorActor*>(this)->actor_wait_state > 0) static_cast<SumActorActor*>(this)->actor_wait_state = 0;
		static_cast<SumActorActor*>(this)->ActorSingleCallback< SumActorActor, 0, int >::remove();

	}
	void a_callback_fire(ActorSingleCallback< SumActorActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("sumActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sumActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< SumActorActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("sumActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sumActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< SumActorActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("sumActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sumActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FutureStream<int> in;
															#line 178 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int total;
															#line 2656 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via sumActor()
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class SumActorActor final : public Actor<int>, public ActorSingleCallback< SumActorActor, 0, int >, public FastAllocated<SumActorActor>, public SumActorActorState<SumActorActor> {
															#line 2661 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<SumActorActor>::operator new;
	using FastAllocated<SumActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< SumActorActor, 0, int >;
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	SumActorActor(FutureStream<int> const& in) 
															#line 2672 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   SumActorActorState<SumActorActor>(in)
	{
		fdb_probe_actor_enter("sumActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sumActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sumActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< SumActorActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> sumActor( FutureStream<int> const& in ) {
															#line 177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new SumActorActor(in));
															#line 2700 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 190 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 2705 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via templateActor()
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class T, class TemplateActorActor>
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TemplateActorActorState {
															#line 2712 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TemplateActorActorState(T const& t) 
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : t(t)
															#line 2719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("templateActor", reinterpret_cast<unsigned long>(this));

	}
	~TemplateActorActorState() 
	{
		fdb_probe_actor_destroy("templateActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 193 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<TemplateActorActor*>(this)->SAV<T>::futures) { (void)(t); this->~TemplateActorActorState(); static_cast<TemplateActorActor*>(this)->destroy(); return 0; }
															#line 2734 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<TemplateActorActor*>(this)->SAV< T >::value()) T(std::move(t)); // state_var_RVO
			this->~TemplateActorActorState();
			static_cast<TemplateActorActor*>(this)->finishSendAndDelPromiseRef();
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
		this->~TemplateActorActorState();
		static_cast<TemplateActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	T t;
															#line 2758 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via templateActor()
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class T>
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TemplateActorActor final : public Actor<T>, public FastAllocated<TemplateActorActor<T>>, public TemplateActorActorState<T, TemplateActorActor<T>> {
															#line 2765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TemplateActorActor<T>>::operator new;
	using FastAllocated<TemplateActorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TemplateActorActor(T const& t) 
															#line 2775 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<T>(),
		   TemplateActorActorState<T, TemplateActorActor<T>>(t)
	{
		fdb_probe_actor_enter("templateActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("templateActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("templateActor", reinterpret_cast<unsigned long>(this), -1);

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
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class T>
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<T> templateActor( T const& t ) {
															#line 191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<T>(new TemplateActorActor<T>(t));
															#line 2804 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

static int destroy() {
	return 666;
}
															#line 2812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via testHygeine()
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class TestHygeineActor>
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TestHygeineActorState {
															#line 2819 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TestHygeineActorState() 
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	{
															#line 2825 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("testHygeine", reinterpret_cast<unsigned long>(this));

	}
	~TestHygeineActorState() 
	{
		fdb_probe_actor_destroy("testHygeine", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 200 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(destroy() == 666);
															#line 201 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<TestHygeineActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestHygeineActorState(); static_cast<TestHygeineActor*>(this)->destroy(); return 0; }
															#line 2841 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<TestHygeineActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TestHygeineActorState();
			static_cast<TestHygeineActor*>(this)->finishSendAndDelPromiseRef();
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
		this->~TestHygeineActorState();
		static_cast<TestHygeineActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
};
// This generated class is to be used only via testHygeine()
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TestHygeineActor final : public Actor<Void>, public FastAllocated<TestHygeineActor>, public TestHygeineActorState<TestHygeineActor> {
															#line 2867 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TestHygeineActor>::operator new;
	using FastAllocated<TestHygeineActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TestHygeineActor() 
															#line 2877 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   TestHygeineActorState<TestHygeineActor>()
	{
		fdb_probe_actor_enter("testHygeine", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testHygeine");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testHygeine", reinterpret_cast<unsigned long>(this), -1);

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
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> testHygeine(  ) {
															#line 199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new TestHygeineActor());
															#line 2904 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

// bool expectActorCount(int x) { return actorCount == x; }
bool expectActorCount(int) {
	return true;
}

struct YieldMockNetwork final : INetwork, ReferenceCounted<YieldMockNetwork> {
	int ticks;
	Promise<Void> nextTick;
	int nextYield;
	INetwork* baseNetwork;

	flowGlobalType global(int id) const override { return baseNetwork->global(id); }
	void setGlobal(size_t id, flowGlobalType v) override {
		baseNetwork->setGlobal(id, v);
		return;
	}

	YieldMockNetwork() : ticks(0), nextYield(0) {
		baseNetwork = g_network;
		g_network = this;
	}
	~YieldMockNetwork() { g_network = baseNetwork; }

	void tick() {
		ticks++;
		Promise<Void> t;
		t.swap(nextTick);
		t.send(Void());
	}

	Future<class Void> delay(double seconds, TaskPriority taskID) override { return nextTick.getFuture(); }

	Future<class Void> orderedDelay(double seconds, TaskPriority taskID) override { return nextTick.getFuture(); }

	Future<class Void> yield(TaskPriority taskID) override {
		if (check_yield(taskID))
			return delay(0, taskID);
		return Void();
	}

	bool check_yield(TaskPriority taskID) override {
		if (nextYield > 0)
			--nextYield;
		return nextYield == 0;
	}

	// Delegate everything else.  TODO: Make a base class NetworkWrapper for delegating everything in INetwork
	TaskPriority getCurrentTask() const override { return baseNetwork->getCurrentTask(); }
	void setCurrentTask(TaskPriority taskID) override { baseNetwork->setCurrentTask(taskID); }
	double now() const override { return baseNetwork->now(); }
	double timer() override { return baseNetwork->timer(); }
	double timer_monotonic() override { return baseNetwork->timer_monotonic(); }
	void stop() override { return baseNetwork->stop(); }
	void addStopCallback(std::function<void()> fn) override {
		ASSERT(false);
		return;
	}
	bool isSimulated() const override { return baseNetwork->isSimulated(); }
	void onMainThread(Promise<Void>&& signal, TaskPriority taskID) override {
		return baseNetwork->onMainThread(std::move(signal), taskID);
	}
	bool isOnMainThread() const override { return baseNetwork->isOnMainThread(); }
	THREAD_HANDLE startThread(THREAD_FUNC_RETURN (*func)(void*), void* arg, int stackSize, const char* name) override {
		return baseNetwork->startThread(func, arg, stackSize, name);
	}
	Future<Reference<class IAsyncFile>> open(std::string filename, int64_t flags, int64_t mode) {
		return IAsyncFileSystem::filesystem()->open(filename, flags, mode);
	}
	Future<Void> deleteFile(std::string filename, bool mustBeDurable) {
		return IAsyncFileSystem::filesystem()->deleteFile(filename, mustBeDurable);
	}
	void run() override { return baseNetwork->run(); }
	bool checkRunnable() override { return baseNetwork->checkRunnable(); }
	void getDiskBytes(std::string const& directory, int64_t& free, int64_t& total) override {
		return baseNetwork->getDiskBytes(directory, free, total);
	}
	bool isAddressOnThisHost(NetworkAddress const& addr) const override {
		return baseNetwork->isAddressOnThisHost(addr);
	}
	const TLSConfig& getTLSConfig() const override {
		static TLSConfig emptyConfig;
		return emptyConfig;
	}
#ifdef ENABLE_SAMPLING
	ActorLineageSet& getActorLineageSet() override { throw std::exception(); }
#endif
	ProtocolVersion protocolVersion() const override { return baseNetwork->protocolVersion(); }
};

struct NonserializableThing {};
															#line 2999 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via testNonserializableThing()
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class TestNonserializableThingActor>
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TestNonserializableThingActorState {
															#line 3006 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TestNonserializableThingActorState() 
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	{
															#line 3012 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("testNonserializableThing", reinterpret_cast<unsigned long>(this));

	}
	~TestNonserializableThingActorState() 
	{
		fdb_probe_actor_destroy("testNonserializableThing", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 295 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<TestNonserializableThingActor*>(this)->SAV<NonserializableThing>::futures) { (void)(NonserializableThing()); this->~TestNonserializableThingActorState(); static_cast<TestNonserializableThingActor*>(this)->destroy(); return 0; }
															#line 3026 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<TestNonserializableThingActor*>(this)->SAV< NonserializableThing >::value()) NonserializableThing(NonserializableThing());
			this->~TestNonserializableThingActorState();
			static_cast<TestNonserializableThingActor*>(this)->finishSendAndDelPromiseRef();
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
		this->~TestNonserializableThingActorState();
		static_cast<TestNonserializableThingActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
};
// This generated class is to be used only via testNonserializableThing()
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TestNonserializableThingActor final : public Actor<NonserializableThing>, public FastAllocated<TestNonserializableThingActor>, public TestNonserializableThingActorState<TestNonserializableThingActor> {
															#line 3052 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TestNonserializableThingActor>::operator new;
	using FastAllocated<TestNonserializableThingActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<NonserializableThing>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TestNonserializableThingActor() 
															#line 3062 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<NonserializableThing>(),
		   TestNonserializableThingActorState<TestNonserializableThingActor>()
	{
		fdb_probe_actor_enter("testNonserializableThing", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testNonserializableThing");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testNonserializableThing", reinterpret_cast<unsigned long>(this), -1);

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
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<NonserializableThing> testNonserializableThing(  ) {
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<NonserializableThing>(new TestNonserializableThingActor());
															#line 3089 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 297 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 3094 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via testCancelled()
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class TestCancelledActor>
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TestCancelledActorState {
															#line 3101 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TestCancelledActorState(bool* const& exits,Future<Void> const& f) 
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : exits(exits),
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   f(f)
															#line 3110 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("testCancelled", reinterpret_cast<unsigned long>(this));

	}
	~TestCancelledActorState() 
	{
		fdb_probe_actor_destroy("testCancelled", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 300 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Future<Void>(Never());
															#line 300 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<TestCancelledActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3128 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TestCancelledActor*>(this)->actor_wait_state = 1;
															#line 300 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestCancelledActor, 0, Void >*>(static_cast<TestCancelledActor*>(this)));
															#line 3133 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
		this->~TestCancelledActorState();
		static_cast<TestCancelledActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 311 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<TestCancelledActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestCancelledActorState(); static_cast<TestCancelledActor*>(this)->destroy(); return 0; }
															#line 3162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<TestCancelledActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestCancelledActorState();
		static_cast<TestCancelledActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 302 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			err = e;
															#line 3175 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			try {
															#line 304 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_1 = Future<Void>(Never());
															#line 304 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<TestCancelledActor*>(this)->actor_wait_state < 0) return a_body1Catch2Catch1(actor_cancelled(), loopDepth);
															#line 3181 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2Catch1(__when_expr_1.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_1.get(), loopDepth); };
				static_cast<TestCancelledActor*>(this)->actor_wait_state = 2;
															#line 304 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestCancelledActor, 1, Void >*>(static_cast<TestCancelledActor*>(this)));
															#line 3186 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2Catch1(unknown_error(), loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<TestCancelledActor*>(this)->actor_wait_state > 0) static_cast<TestCancelledActor*>(this)->actor_wait_state = 0;
		static_cast<TestCancelledActor*>(this)->ActorCallback< TestCancelledActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestCancelledActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TestCancelledActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TestCancelledActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2cont1(int loopDepth) 
	{
															#line 309 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 3295 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"

		return loopDepth;
	}
	int a_body1Catch2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 306 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			*exits = true;
															#line 307 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 3306 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont3(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont3(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TestCancelledActor*>(this)->actor_wait_state > 0) static_cast<TestCancelledActor*>(this)->actor_wait_state = 0;
		static_cast<TestCancelledActor*>(this)->ActorCallback< TestCancelledActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestCancelledActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2Catch1(error, 0);
		} catch (...) {
			a_body1Catch2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TestCancelledActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2Catch1(error, 0);
		} catch (...) {
			a_body1Catch2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TestCancelledActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2Catch1(error, 0);
		} catch (...) {
			a_body1Catch2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1Catch2cont3(int loopDepth) 
	{
		try {
			loopDepth = a_body1Catch2cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	bool* exits;
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 302 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Error err;
															#line 3410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via testCancelled()
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class TestCancelledActor final : public Actor<Void>, public ActorCallback< TestCancelledActor, 0, Void >, public ActorCallback< TestCancelledActor, 1, Void >, public FastAllocated<TestCancelledActor>, public TestCancelledActorState<TestCancelledActor> {
															#line 3415 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TestCancelledActor>::operator new;
	using FastAllocated<TestCancelledActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestCancelledActor, 0, Void >;
friend struct ActorCallback< TestCancelledActor, 1, Void >;
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	TestCancelledActor(bool* const& exits,Future<Void> const& f) 
															#line 3427 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   TestCancelledActorState<TestCancelledActor>(exits, f)
	{
		fdb_probe_actor_enter("testCancelled", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testCancelled");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testCancelled", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TestCancelledActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TestCancelledActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<Void> testCancelled( bool* const& exits, Future<Void> const& f ) {
															#line 298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new TestCancelledActor(exits, f));
															#line 3456 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 3461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase314()
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase314Actor>
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase314ActorState {
															#line 3468 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase314ActorState(UnitTestParameters const& params) 
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 3475 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase314", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase314ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase314", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 315 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			bool exits = false;
															#line 316 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> p;
															#line 317 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> test = testCancelled(&exits, p.getFuture());
															#line 318 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getPromiseReferenceCount() == 1 && p.getFutureReferenceCount() == 1);
															#line 319 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			test.cancel();
															#line 320 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(exits);
															#line 321 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(test.getPromiseReferenceCount() == 0 && test.getFutureReferenceCount() == 1 && test.isReady() && test.isError() && test.getError().code() == error_code_actor_cancelled);
															#line 323 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getPromiseReferenceCount() == 1 && p.getFutureReferenceCount() == 0);
															#line 325 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase314Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase314ActorState(); static_cast<FlowTestCase314Actor*>(this)->destroy(); return 0; }
															#line 3506 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase314Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase314ActorState();
			static_cast<FlowTestCase314Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase314ActorState();
		static_cast<FlowTestCase314Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 3530 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase314()
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase314Actor final : public Actor<Void>, public FastAllocated<FlowTestCase314Actor>, public FlowTestCase314ActorState<FlowTestCase314Actor> {
															#line 3535 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase314Actor>::operator new;
	using FastAllocated<FlowTestCase314Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase314Actor(UnitTestParameters const& params) 
															#line 3545 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase314ActorState<FlowTestCase314Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase314", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase314");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase314", reinterpret_cast<unsigned long>(this), -1);

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
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase314( UnitTestParameters const& params ) {
															#line 314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase314Actor(params));
															#line 3572 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase314, "/flow/flow/cancel1")

#line 327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 3578 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via noteCancel()
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class NoteCancelActor>
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class NoteCancelActorState {
															#line 3585 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	NoteCancelActorState(int* const& cancelled) 
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : cancelled(cancelled)
															#line 3592 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("noteCancel", reinterpret_cast<unsigned long>(this));

	}
	~NoteCancelActorState() 
	{
		fdb_probe_actor_destroy("noteCancel", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 329 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			*cancelled = 0;
															#line 3607 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			try {
															#line 331 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Future<Void>(Never());
															#line 331 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<NoteCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3613 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<NoteCancelActor*>(this)->actor_wait_state = 1;
															#line 331 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< NoteCancelActor, 0, Void >*>(static_cast<NoteCancelActor*>(this)));
															#line 3618 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
		this->~NoteCancelActorState();
		static_cast<NoteCancelActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 334 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("Cancelled!\n");
															#line 335 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			*cancelled = 1;
															#line 336 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 3652 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 332 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		return a_body1Catch2(internal_error(), loopDepth);
															#line 3666 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 332 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		return a_body1Catch2(internal_error(), loopDepth);
															#line 3674 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"

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
		if (static_cast<NoteCancelActor*>(this)->actor_wait_state > 0) static_cast<NoteCancelActor*>(this)->actor_wait_state = 0;
		static_cast<NoteCancelActor*>(this)->ActorCallback< NoteCancelActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< NoteCancelActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("noteCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("noteCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< NoteCancelActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("noteCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("noteCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< NoteCancelActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("noteCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("noteCancel", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int* cancelled;
															#line 3743 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via noteCancel()
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class NoteCancelActor final : public Actor<Void>, public ActorCallback< NoteCancelActor, 0, Void >, public FastAllocated<NoteCancelActor>, public NoteCancelActorState<NoteCancelActor> {
															#line 3748 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<NoteCancelActor>::operator new;
	using FastAllocated<NoteCancelActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< NoteCancelActor, 0, Void >;
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	NoteCancelActor(int* const& cancelled) 
															#line 3759 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   NoteCancelActorState<NoteCancelActor>(cancelled)
	{
		fdb_probe_actor_enter("noteCancel", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("noteCancel");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("noteCancel", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< NoteCancelActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> noteCancel( int* const& cancelled ) {
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new NoteCancelActor(cancelled));
															#line 3787 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 339 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 3792 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase340()
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase340Actor>
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase340ActorState {
															#line 3799 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase340ActorState(UnitTestParameters const& params) 
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 3806 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase340", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase340ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase340", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 341 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int c1 = 0, c2 = 0, c3 = 0;
															#line 343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> cf = noteCancel(&c1);
															#line 344 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c1 == 0);
															#line 345 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			cf = Future<Void>();
															#line 346 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c1 == 1);
															#line 348 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			cf = noteCancel(&c2) && noteCancel(&c3);
															#line 349 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c2 == 0 && c3 == 0);
															#line 350 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			cf = Future<Void>();
															#line 351 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c2 == 1 && c3 == 1);
															#line 352 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase340Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase340ActorState(); static_cast<FlowTestCase340Actor*>(this)->destroy(); return 0; }
															#line 3839 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase340Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase340ActorState();
			static_cast<FlowTestCase340Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase340ActorState();
		static_cast<FlowTestCase340Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 3863 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase340()
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase340Actor final : public Actor<Void>, public FastAllocated<FlowTestCase340Actor>, public FlowTestCase340ActorState<FlowTestCase340Actor> {
															#line 3868 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase340Actor>::operator new;
	using FastAllocated<FlowTestCase340Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase340Actor(UnitTestParameters const& params) 
															#line 3878 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase340ActorState<FlowTestCase340Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase340", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase340");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase340", reinterpret_cast<unsigned long>(this), -1);

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
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase340( UnitTestParameters const& params ) {
															#line 340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase340Actor(params));
															#line 3905 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase340, "/flow/flow/cancel2")

#line 354 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

namespace {
// Simple message for flatbuffers unittests
struct Int {
	constexpr static FileIdentifier file_identifier = 12345;
	uint32_t value;
	Int() = default;
	Int(uint32_t value) : value(value) {}
	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, value);
	}
};
} // namespace

															#line 3925 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase369()
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase369Actor>
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase369ActorState {
															#line 3932 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase369ActorState(UnitTestParameters const& params) 
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 3939 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase369", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase369ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase369", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 372 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Future<NonserializableThing> f = testNonserializableThing();
															#line 373 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Promise<NonserializableThing> p;
															#line 374 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				p.send(NonserializableThing());
															#line 375 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				f = p.getFuture();
															#line 3961 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 383 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ReplyPromise<Int> rpInt;
															#line 384 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Future<Int> f = rpInt.getFuture();
															#line 385 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(!f.isReady());
															#line 386 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				rpInt.send(123);
															#line 387 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(f.get().value == 123);
															#line 3974 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 391 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				RequestStream<Int> rsInt;
															#line 392 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				FutureStream<Int> f = rsInt.getFuture();
															#line 393 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				rsInt.send(1);
															#line 394 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				rsInt.send(2);
															#line 395 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(f.pop().value == 1);
															#line 396 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(f.pop().value == 2);
															#line 3989 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 399 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase369Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase369ActorState(); static_cast<FlowTestCase369Actor*>(this)->destroy(); return 0; }
															#line 3993 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase369Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase369ActorState();
			static_cast<FlowTestCase369Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase369ActorState();
		static_cast<FlowTestCase369Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4017 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase369()
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase369Actor final : public Actor<Void>, public FastAllocated<FlowTestCase369Actor>, public FlowTestCase369ActorState<FlowTestCase369Actor> {
															#line 4022 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase369Actor>::operator new;
	using FastAllocated<FlowTestCase369Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase369Actor(UnitTestParameters const& params) 
															#line 4032 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase369ActorState<FlowTestCase369Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase369", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase369");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase369", reinterpret_cast<unsigned long>(this), -1);

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
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase369( UnitTestParameters const& params ) {
															#line 369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase369Actor(params));
															#line 4059 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase369, "/flow/flow/nonserializable futures")

#line 401 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4065 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase402()
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase402Actor>
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase402ActorState {
															#line 4072 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase402ActorState(UnitTestParameters const& params) 
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4079 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase402", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase402ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase402", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 405 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				RequestStream<Int> locInt;
															#line 406 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				BinaryWriter wr(IncludeVersion());
															#line 407 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				wr << locInt;
															#line 409 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(locInt.getEndpoint().isValid() && locInt.getEndpoint().isLocal() && locInt.getEndpoint().getPrimaryAddress() == FlowTransport::transport().getLocalAddress());
															#line 412 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				BinaryReader rd(wr.toValue(), IncludeVersion());
															#line 413 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				RequestStream<Int> remoteInt;
															#line 414 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				rd >> remoteInt;
															#line 416 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(remoteInt.getEndpoint() == locInt.getEndpoint());
															#line 4109 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 421 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (0)
															#line 4113 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 422 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ReplyPromise<Int> locInt;
															#line 423 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				BinaryWriter wr(IncludeVersion());
															#line 424 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				wr << locInt;
															#line 426 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(locInt.getEndpoint().isValid() && locInt.getEndpoint().isLocal());
															#line 428 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				BinaryReader rd(wr.toValue(), IncludeVersion());
															#line 429 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ReplyPromise<Int> remoteInt;
															#line 430 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				rd >> remoteInt;
															#line 432 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(remoteInt.getEndpoint() == locInt.getEndpoint());
															#line 4131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 435 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase402Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase402ActorState(); static_cast<FlowTestCase402Actor*>(this)->destroy(); return 0; }
															#line 4135 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase402Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase402ActorState();
			static_cast<FlowTestCase402Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase402ActorState();
		static_cast<FlowTestCase402Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4159 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase402()
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase402Actor final : public Actor<Void>, public FastAllocated<FlowTestCase402Actor>, public FlowTestCase402ActorState<FlowTestCase402Actor> {
															#line 4164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase402Actor>::operator new;
	using FastAllocated<FlowTestCase402Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase402Actor(UnitTestParameters const& params) 
															#line 4174 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase402ActorState<FlowTestCase402Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase402", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase402");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase402", reinterpret_cast<unsigned long>(this), -1);

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
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase402( UnitTestParameters const& params ) {
															#line 402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase402Actor(params));
															#line 4201 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase402, "/flow/flow/networked futures")

#line 437 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4207 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase438()
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase438Actor>
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase438ActorState {
															#line 4214 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase438ActorState(UnitTestParameters const& params) 
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4221 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase438", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase438ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase438", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 439 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			std::vector<Promise<int>> ps(5);
															#line 440 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<int>> fs;
															#line 441 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<Void>> qs;
															#line 442 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for( auto& p : ps ) {
															#line 443 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				fs.push_back(p.getFuture());
															#line 4244 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 445 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i <= ps.size();i++) {
															#line 446 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				qs.push_back(quorum(fs, i));
															#line 4250 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 448 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < ps.size();i++) {
															#line 449 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(qs[i].isReady());
															#line 450 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(!qs[i + 1].isReady());
															#line 451 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ps[i].send(i);
															#line 4260 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 453 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(qs[ps.size()].isReady());
															#line 454 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase438Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase438ActorState(); static_cast<FlowTestCase438Actor*>(this)->destroy(); return 0; }
															#line 4266 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase438Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase438ActorState();
			static_cast<FlowTestCase438Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase438ActorState();
		static_cast<FlowTestCase438Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4290 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase438()
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase438Actor final : public Actor<Void>, public FastAllocated<FlowTestCase438Actor>, public FlowTestCase438ActorState<FlowTestCase438Actor> {
															#line 4295 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase438Actor>::operator new;
	using FastAllocated<FlowTestCase438Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase438Actor(UnitTestParameters const& params) 
															#line 4305 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase438ActorState<FlowTestCase438Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase438", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase438");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase438", reinterpret_cast<unsigned long>(this), -1);

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
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase438( UnitTestParameters const& params ) {
															#line 438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase438Actor(params));
															#line 4332 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase438, "/flow/flow/quorum")

#line 456 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4338 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase457()
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase457Actor>
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase457ActorState {
															#line 4345 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase457ActorState(UnitTestParameters const& params) 
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4352 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase457", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase457ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase457", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 458 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<int> invalid;
															#line 459 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!invalid.isValid());
															#line 461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<int> never = Never();
															#line 462 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(never.isValid() && !never.isReady());
															#line 464 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<int> one = 1;
															#line 465 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(one.isValid() && one.isReady() && !one.isError());
															#line 466 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(one.get() == 1);
															#line 467 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(one.getFutureReferenceCount() == 1);
															#line 468 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase457Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase457ActorState(); static_cast<FlowTestCase457Actor*>(this)->destroy(); return 0; }
															#line 4383 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase457Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase457ActorState();
			static_cast<FlowTestCase457Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase457ActorState();
		static_cast<FlowTestCase457Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4407 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase457()
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase457Actor final : public Actor<Void>, public FastAllocated<FlowTestCase457Actor>, public FlowTestCase457ActorState<FlowTestCase457Actor> {
															#line 4412 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase457Actor>::operator new;
	using FastAllocated<FlowTestCase457Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase457Actor(UnitTestParameters const& params) 
															#line 4422 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase457ActorState<FlowTestCase457Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase457", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase457");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase457", reinterpret_cast<unsigned long>(this), -1);

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
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase457( UnitTestParameters const& params ) {
															#line 457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase457Actor(params));
															#line 4449 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase457, "/flow/flow/trivial futures")

#line 470 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4455 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase471()
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase471Actor>
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase471ActorState {
															#line 4462 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase471ActorState(UnitTestParameters const& params) 
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4469 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase471", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase471ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase471", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 472 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<int> f;
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p;
															#line 475 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid());
															#line 476 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!p.isSet());
															#line 477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(1);
															#line 478 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isSet());
															#line 479 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getFuture().get() == 1);
															#line 481 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p2;
															#line 482 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p2.getFuture();
															#line 483 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && !f.isReady());
															#line 484 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p2.send(2);
															#line 485 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && !f.isError());
															#line 486 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 2);
															#line 488 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p3;
															#line 489 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p3.getFuture();
															#line 490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p3.sendError(end_of_stream());
															#line 491 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && f.isError());
															#line 492 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_end_of_stream);
															#line 494 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p4;
															#line 495 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p4.getFuture();
															#line 496 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p4 = Promise<int>();
															#line 497 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p4.isValid() && !p4.isSet());
															#line 498 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && f.isError());
															#line 499 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_broken_promise);
															#line 500 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase471Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase471ActorState(); static_cast<FlowTestCase471Actor*>(this)->destroy(); return 0; }
															#line 4532 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase471Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase471ActorState();
			static_cast<FlowTestCase471Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase471ActorState();
		static_cast<FlowTestCase471Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4556 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase471()
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase471Actor final : public Actor<Void>, public FastAllocated<FlowTestCase471Actor>, public FlowTestCase471ActorState<FlowTestCase471Actor> {
															#line 4561 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase471Actor>::operator new;
	using FastAllocated<FlowTestCase471Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase471Actor(UnitTestParameters const& params) 
															#line 4571 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase471ActorState<FlowTestCase471Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase471", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase471");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase471", reinterpret_cast<unsigned long>(this), -1);

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
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase471( UnitTestParameters const& params ) {
															#line 471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase471Actor(params));
															#line 4598 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase471, "/flow/flow/trivial promises")

#line 502 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4604 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase503()
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase503Actor>
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase503ActorState {
															#line 4611 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase503ActorState(UnitTestParameters const& params) 
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4618 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase503", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase503ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase503", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 504 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			FutureStream<int> f;
															#line 506 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p;
															#line 507 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(1);
															#line 508 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getFuture().isReady());
															#line 509 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getFuture().pop() == 1);
															#line 511 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p2;
															#line 512 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p2.getFuture();
															#line 513 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && !f.isReady());
															#line 514 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p2.send(2);
															#line 515 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p2.send(3);
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && !f.isError());
															#line 517 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.pop() == 2);
															#line 518 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.pop() == 3);
															#line 520 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p3;
															#line 521 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p3.getFuture();
															#line 522 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p3.send(4);
															#line 523 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p3.sendError(end_of_stream());
															#line 524 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && !f.isError());
															#line 525 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.pop() == 4);
															#line 526 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isError());
															#line 527 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_end_of_stream);
															#line 529 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p4;
															#line 530 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p4.getFuture();
															#line 531 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p4 = PromiseStream<int>();
															#line 532 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && f.isError());
															#line 533 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_broken_promise);
															#line 534 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase503Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase503ActorState(); static_cast<FlowTestCase503Actor*>(this)->destroy(); return 0; }
															#line 4685 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase503Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase503ActorState();
			static_cast<FlowTestCase503Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase503ActorState();
		static_cast<FlowTestCase503Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4709 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase503()
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase503Actor final : public Actor<Void>, public FastAllocated<FlowTestCase503Actor>, public FlowTestCase503ActorState<FlowTestCase503Actor> {
															#line 4714 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase503Actor>::operator new;
	using FastAllocated<FlowTestCase503Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase503Actor(UnitTestParameters const& params) 
															#line 4724 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase503ActorState<FlowTestCase503Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase503", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase503");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase503", reinterpret_cast<unsigned long>(this), -1);

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
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase503( UnitTestParameters const& params ) {
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase503Actor(params));
															#line 4751 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase503, "/flow/flow/trivial promisestreams")

#line 536 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4757 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase537()
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase537Actor>
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase537ActorState {
															#line 4764 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase537ActorState(UnitTestParameters const& params) 
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4771 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase537", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase537ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase537", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 538 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p;
															#line 539 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<int> f = p.getFuture();
															#line 540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int result = 0;
															#line 541 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			bool happened = false;
															#line 543 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( std::move(f), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 545 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&happened](int) { happened = true; }, [&happened](Error) { happened = true; });
															#line 547 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isValid());
															#line 548 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && !p.isSet() && p.getFutureReferenceCount() == 1);
															#line 549 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0 && !happened);
															#line 551 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(123);
															#line 552 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 123 && happened);
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && p.isSet() && p.getFutureReferenceCount() == 0 && p.getFuture().get() == 123);
															#line 555 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 556 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 558 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 123);
															#line 559 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && p.isSet() && p.getFutureReferenceCount() == 0 && p.getFuture().get() == 123);
															#line 561 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p = Promise<int>();
															#line 562 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p.getFuture();
															#line 563 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 564 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( std::move(f), [&result](int x) { result = x; }, [&result](Error e) { result = -e.code(); });
															#line 566 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isValid());
															#line 567 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && !p.isSet() && p.getFutureReferenceCount() == 1);
															#line 568 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0);
															#line 570 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p = Promise<int>();
															#line 571 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == -error_code_broken_promise);
															#line 572 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase537Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase537ActorState(); static_cast<FlowTestCase537Actor*>(this)->destroy(); return 0; }
															#line 4836 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase537Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase537ActorState();
			static_cast<FlowTestCase537Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase537ActorState();
		static_cast<FlowTestCase537Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4860 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase537()
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase537Actor final : public Actor<Void>, public FastAllocated<FlowTestCase537Actor>, public FlowTestCase537ActorState<FlowTestCase537Actor> {
															#line 4865 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase537Actor>::operator new;
	using FastAllocated<FlowTestCase537Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase537Actor(UnitTestParameters const& params) 
															#line 4875 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase537ActorState<FlowTestCase537Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase537", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase537");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase537", reinterpret_cast<unsigned long>(this), -1);

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
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase537( UnitTestParameters const& params ) {
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase537Actor(params));
															#line 4902 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase537, "/flow/flow/callbacks")

#line 574 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 4908 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase575()
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase575Actor>
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase575ActorState {
															#line 4915 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase575ActorState(UnitTestParameters const& params) 
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4922 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase575", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase575ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase575", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p;
															#line 578 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int result = 0;
															#line 580 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 583 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0);
															#line 585 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(123);
															#line 586 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(456);
															#line 588 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 123);
															#line 589 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 591 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 594 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 456);
															#line 595 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 597 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 600 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0);
															#line 602 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p = PromiseStream<int>();
															#line 604 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == -1);
															#line 605 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase575Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase575ActorState(); static_cast<FlowTestCase575Actor*>(this)->destroy(); return 0; }
															#line 4967 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase575Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase575ActorState();
			static_cast<FlowTestCase575Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase575ActorState();
		static_cast<FlowTestCase575Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4991 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase575()
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase575Actor final : public Actor<Void>, public FastAllocated<FlowTestCase575Actor>, public FlowTestCase575ActorState<FlowTestCase575Actor> {
															#line 4996 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase575Actor>::operator new;
	using FastAllocated<FlowTestCase575Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase575Actor(UnitTestParameters const& params) 
															#line 5006 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase575ActorState<FlowTestCase575Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase575", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase575");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase575", reinterpret_cast<unsigned long>(this), -1);

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
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase575( UnitTestParameters const& params ) {
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase575Actor(params));
															#line 5033 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase575, "/flow/flow/promisestream callbacks")

#line 607 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

// Incompatible with --crash, so we are commenting it out for now
/*
TEST_CASE("/flow/flow/promisestream multiple wait error")
{
    state int result = 0;
    state PromiseStream<int> p;
    try {
        onReady(p.getFuture(), [&result](int x) { result = x; }, [&result](Error e){ result = -1; });
        result = 100;
        onReady(p.getFuture(), [&result](int x) { result = x; }, [&result](Error e){ result = -1; });
        ASSERT(false);
    }
    catch (Error& e) {
        ASSERT(e.code() == error_code_internal_error);
    }
    ASSERT(result == 100);
    p = PromiseStream<int>();
    ASSERT(result == -1);
    return Void();
}
*/

															#line 5061 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase630()
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase630Actor>
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase630ActorState {
															#line 5068 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase630ActorState(UnitTestParameters const& params) 
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5075 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase630", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase630ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase630", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 631 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 633 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int result = 0;
															#line 634 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			trivialVoidActor(&result);
															#line 635 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 1);
															#line 636 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 638 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<int> f = return42Actor();
															#line 639 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && !f.isError() && f.get() == 42 && f.getFutureReferenceCount() == 1 && f.getPromiseReferenceCount() == 0);
															#line 641 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(1));
															#line 642 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = Future<int>();
															#line 643 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 645 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = templateActor(24);
															#line 646 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && !f.isError() && f.get() == 24 && f.getFutureReferenceCount() == 1 && f.getPromiseReferenceCount() == 0);
															#line 648 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(1));
															#line 649 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = Future<int>();
															#line 650 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 652 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 653 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			voidWaitActor(2, &result);
															#line 654 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 2 && expectActorCount(0));
															#line 656 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p;
															#line 657 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = addOneActor(p.getFuture());
															#line 658 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isReady() && expectActorCount(1));
															#line 659 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(100);
															#line 660 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && f.get() == 101);
															#line 661 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(1));
															#line 662 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = Future<int>();
															#line 663 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 665 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> ps;
															#line 666 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = consumeOneActor(ps.getFuture());
															#line 667 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isReady() && expectActorCount(1));
															#line 668 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ps.send(101);
															#line 669 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 101 && ps.isEmpty());
															#line 670 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ps.send(102);
															#line 671 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!ps.isEmpty());
															#line 672 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = consumeOneActor(ps.getFuture());
															#line 673 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 102 && ps.isEmpty());
															#line 675 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = sumActor(ps.getFuture());
															#line 676 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ps.send(1);
															#line 677 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ps.send(10);
															#line 678 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ps.send(100);
															#line 679 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ps.sendError(end_of_stream());
															#line 680 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 111);
															#line 682 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(testHygeine().isReady());
															#line 683 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase630Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase630ActorState(); static_cast<FlowTestCase630Actor*>(this)->destroy(); return 0; }
															#line 5174 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase630Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase630ActorState();
			static_cast<FlowTestCase630Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase630ActorState();
		static_cast<FlowTestCase630Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase630()
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase630Actor final : public Actor<Void>, public FastAllocated<FlowTestCase630Actor>, public FlowTestCase630ActorState<FlowTestCase630Actor> {
															#line 5203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase630Actor>::operator new;
	using FastAllocated<FlowTestCase630Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase630Actor(UnitTestParameters const& params) 
															#line 5213 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase630ActorState<FlowTestCase630Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase630", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase630");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase630", reinterpret_cast<unsigned long>(this), -1);

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
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase630( UnitTestParameters const& params ) {
															#line 630 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase630Actor(params));
															#line 5240 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase630, "/flow/flow/trivial actors")

#line 685 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 5246 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase686()
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase686Actor>
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase686ActorState {
															#line 5253 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase686ActorState(UnitTestParameters const& params) 
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5260 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase686", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase686ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase686", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 690 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			auto yn = makeReference<YieldMockNetwork>();
															#line 692 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			yn->nextYield = 0;
															#line 694 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> p;
															#line 695 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> u = p.getFuture();
															#line 696 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> i = success(u);
															#line 698 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<Void>> v;
															#line 699 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 5;i++) {
															#line 700 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				v.push_back(yieldedFuture(u));
															#line 5289 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 701 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			auto numReady = [&v]() { return std::count_if(v.begin(), v.end(), [](Future<Void> v) { return v.isReady(); }); };
															#line 703 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(numReady() == 0);
															#line 704 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(Void());
															#line 705 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(u.isReady() && i.isReady() && numReady() == 0);
															#line 707 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 5;i++) {
															#line 708 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				yn->tick();
															#line 709 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(numReady() == i + 1);
															#line 5305 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 712 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 5;i++) {
															#line 713 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(v[i].getPromiseReferenceCount() == 0 && v[i].getFutureReferenceCount() == 1);
															#line 5311 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 716 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase686Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase686ActorState(); static_cast<FlowTestCase686Actor*>(this)->destroy(); return 0; }
															#line 5315 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase686Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase686ActorState();
			static_cast<FlowTestCase686Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase686ActorState();
		static_cast<FlowTestCase686Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5339 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase686()
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase686Actor final : public Actor<Void>, public FastAllocated<FlowTestCase686Actor>, public FlowTestCase686ActorState<FlowTestCase686Actor> {
															#line 5344 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase686Actor>::operator new;
	using FastAllocated<FlowTestCase686Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase686Actor(UnitTestParameters const& params) 
															#line 5354 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase686ActorState<FlowTestCase686Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase686", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase686");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase686", reinterpret_cast<unsigned long>(this), -1);

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
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase686( UnitTestParameters const& params ) {
															#line 686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase686Actor(params));
															#line 5381 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase686, "/flow/flow/yieldedFuture/progress")

#line 718 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 5387 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase719()
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase719Actor>
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase719ActorState {
															#line 5394 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase719ActorState(UnitTestParameters const& params) 
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5401 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase719", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase719ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase719", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 722 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			auto yn = makeReference<YieldMockNetwork>();
															#line 724 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int r = 0;r < 100;r++) {
															#line 725 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Promise<Void> p;
															#line 726 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Future<Void> u = p.getFuture();
															#line 727 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Future<Void> i = success(u);
															#line 729 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> v;
															#line 730 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < 25;i++) {
															#line 731 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					v.push_back(yieldedFuture(u));
															#line 5430 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 732 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				auto numReady = [&v]() { return std::count_if(v.begin(), v.end(), [](Future<Void> v) { return v.isReady(); }); };
															#line 736 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Future<Void> j = success(u);
															#line 738 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(numReady() == 0);
															#line 740 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				int expectYield = deterministicRandom()->randomInt(0, 4);
															#line 741 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				int expectReady = expectYield;
															#line 742 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				yn->nextYield = 1 + expectYield;
															#line 744 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				p.send(Void());
															#line 745 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(u.isReady() && i.isReady() && j.isReady() && numReady() == expectReady);
															#line 747 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(;numReady() != v.size();) {
															#line 748 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					expectYield = deterministicRandom()->randomInt(0, 4);
															#line 749 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					yn->nextYield = 1 + expectYield;
															#line 750 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					expectReady += 1 + expectYield;
															#line 751 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					yn->tick();
															#line 754 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(numReady() == std::min<int>(expectReady, v.size()));
															#line 5460 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 757 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < v.size();i++) {
															#line 758 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(v[i].getPromiseReferenceCount() == 0 && v[i].getFutureReferenceCount() == 1);
															#line 5466 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
			}
															#line 762 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase719Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase719ActorState(); static_cast<FlowTestCase719Actor*>(this)->destroy(); return 0; }
															#line 5471 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase719Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase719ActorState();
			static_cast<FlowTestCase719Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase719ActorState();
		static_cast<FlowTestCase719Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5495 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase719()
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase719Actor final : public Actor<Void>, public FastAllocated<FlowTestCase719Actor>, public FlowTestCase719ActorState<FlowTestCase719Actor> {
															#line 5500 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase719Actor>::operator new;
	using FastAllocated<FlowTestCase719Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase719Actor(UnitTestParameters const& params) 
															#line 5510 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase719ActorState<FlowTestCase719Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase719", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase719");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase719", reinterpret_cast<unsigned long>(this), -1);

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
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase719( UnitTestParameters const& params ) {
															#line 719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase719Actor(params));
															#line 5537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase719, "/flow/flow/yieldedFuture/random")

#line 764 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 5543 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase765()
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase765Actor>
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase765ActorState {
															#line 5550 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase765ActorState(UnitTestParameters const& params) 
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5557 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase765", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase765ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase765", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 766 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			double start;
															#line 767 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int N = 1000000;
															#line 769 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			auto yn = makeReference<YieldMockNetwork>();
															#line 771 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			yn->nextYield = 2 * N + 100;
															#line 773 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> p;
															#line 774 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> f = p.getFuture();
															#line 775 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<Void>> ys;
															#line 777 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 778 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 779 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ys.push_back(yieldedFuture(f));
															#line 5590 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 780 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("yieldedFuture(f) create: %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 781 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p.send(Void());
															#line 782 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("yieldedFuture(f) total: %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 784 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for( auto& y : ys ) {
															#line 785 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(y.isReady());
															#line 5602 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 787 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			p = Promise<Void>();
															#line 788 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f = p.getFuture();
															#line 790 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 791 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 792 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				yieldedFuture(f);
															#line 5614 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 793 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("yieldedFuture(f) cancel: %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 795 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase765Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase765ActorState(); static_cast<FlowTestCase765Actor*>(this)->destroy(); return 0; }
															#line 5620 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase765Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase765ActorState();
			static_cast<FlowTestCase765Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase765ActorState();
		static_cast<FlowTestCase765Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5644 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase765()
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase765Actor final : public Actor<Void>, public FastAllocated<FlowTestCase765Actor>, public FlowTestCase765ActorState<FlowTestCase765Actor> {
															#line 5649 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase765Actor>::operator new;
	using FastAllocated<FlowTestCase765Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase765Actor(UnitTestParameters const& params) 
															#line 5659 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase765ActorState<FlowTestCase765Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase765", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase765");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase765", reinterpret_cast<unsigned long>(this), -1);

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
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase765( UnitTestParameters const& params ) {
															#line 765 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase765Actor(params));
															#line 5686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase765, "/flow/perf/yieldedFuture")

#line 797 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 5692 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase798()
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase798Actor>
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase798ActorState {
															#line 5699 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase798ActorState(UnitTestParameters const& params) 
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5706 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase798", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase798ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase798", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 799 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 801 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> a, b;
															#line 802 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> c = chooseTwoActor(a.getFuture(), b.getFuture());
															#line 803 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a.getFutureReferenceCount() == 2 && b.getFutureReferenceCount() == 2 && !c.isReady());
															#line 804 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			b.send(Void());
															#line 805 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a.getFutureReferenceCount() == 0 && b.getFutureReferenceCount() == 0 && c.isReady() && !c.isError() && expectActorCount(1));
															#line 807 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			c = Future<Void>();
															#line 808 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a.getFutureReferenceCount() == 0 && b.getFutureReferenceCount() == 0 && expectActorCount(0));
															#line 809 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase798Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase798ActorState(); static_cast<FlowTestCase798Actor*>(this)->destroy(); return 0; }
															#line 5737 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase798Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase798ActorState();
			static_cast<FlowTestCase798Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase798ActorState();
		static_cast<FlowTestCase798Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5761 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase798()
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase798Actor final : public Actor<Void>, public FastAllocated<FlowTestCase798Actor>, public FlowTestCase798ActorState<FlowTestCase798Actor> {
															#line 5766 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase798Actor>::operator new;
	using FastAllocated<FlowTestCase798Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase798Actor(UnitTestParameters const& params) 
															#line 5776 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase798ActorState<FlowTestCase798Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase798", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase798");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase798", reinterpret_cast<unsigned long>(this), -1);

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
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase798( UnitTestParameters const& params ) {
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase798Actor(params));
															#line 5803 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase798, "/flow/flow/chooseTwoActor")

#line 811 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 5809 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase812()
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase812Actor>
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase812ActorState {
															#line 5816 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase812ActorState(UnitTestParameters const& params) 
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5823 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase812", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase812ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase812", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 813 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			double start;
															#line 814 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int N = 1000000;
															#line 816 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 818 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 819 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 820 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				emptyVoidActor();
															#line 5848 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 821 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("emptyVoidActor(): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 823 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 825 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 826 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 827 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				emptyActor();
															#line 5860 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 829 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("emptyActor(): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 831 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 833 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> neverSet;
															#line 834 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> never = neverSet.getFuture();
															#line 835 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> already = Void();
															#line 837 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 838 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 839 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				oneWaitVoidActor(already);
															#line 5878 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 840 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("oneWaitVoidActor(already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 842 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 5884 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 850 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 851 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 852 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(already);
															#line 853 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 5894 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 855 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5898 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 859 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 860 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 861 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(never);
															#line 862 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(!f.isReady());
															#line 5909 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 864 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("(cancelled) oneWaitActor(never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 865 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(expectActorCount(0));
															#line 5915 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 869 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 870 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 871 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 872 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(p.getFuture());
															#line 873 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 874 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 5930 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 876 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(after): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5934 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 880 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 881 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 882 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 883 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 884 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out[i] = oneWaitActor(pipe[i].getFuture());
															#line 5947 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 886 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 887 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 888 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 5955 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 890 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(fifo): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5959 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 894 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 895 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 896 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 897 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 898 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out[i] = oneWaitActor(pipe[i].getFuture());
															#line 5972 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 900 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = N - 1;i >= 0;i--) {
															#line 901 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 902 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 5980 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 904 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(lifo): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5984 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 908 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 909 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 910 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(already, already);
															#line 911 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 5995 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 913 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(already, already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5999 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 917 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 918 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 919 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(already, never);
															#line 920 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6010 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 922 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(already, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6014 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 926 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 927 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 928 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(never, already);
															#line 929 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6025 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 931 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(never, already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6029 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 935 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 936 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(never, never);
															#line 938 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(!f.isReady());
															#line 6040 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 941 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("(cancelled) chooseTwoActor(never, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6044 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 945 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 946 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 947 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 948 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(p.getFuture(), never);
															#line 949 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 950 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6059 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 952 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(after, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6063 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 956 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 957 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 958 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 959 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 960 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out[i] = chooseTwoActor(pipe[i].getFuture(), never);
															#line 6076 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 962 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 963 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 964 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6084 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 966 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(fifo, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6088 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 970 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 971 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 972 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 973 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 974 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out[i] = chooseTwoActor(pipe[i].getFuture(), pipe[i].getFuture());
															#line 6101 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 976 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 977 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 978 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6109 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 980 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(fifo, fifo): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6113 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 984 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 985 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 986 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 987 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 988 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out[i] = chooseTwoActor(chooseTwoActor(pipe[i].getFuture(), never), never);
															#line 6126 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 990 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 991 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 992 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6134 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 994 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor^2((fifo, never), never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6138 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 998 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 999 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1000 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 1001 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(chooseTwoActor(p.getFuture(), never));
															#line 1002 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 1003 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6153 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1005 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(chooseTwoActor(after, never)): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6157 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1009 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1010 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 1011 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1012 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1013 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out[i] = oneWaitActor(chooseTwoActor(pipe[i].getFuture(), never));
															#line 6170 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1015 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1016 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1017 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6178 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1019 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(chooseTwoActor(fifo, never)): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1023 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1024 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1025 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 1026 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(p.getFuture(), never);
															#line 1027 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> a = oneWaitActor(f);
															#line 1028 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> b = oneWaitActor(f);
															#line 1029 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 1030 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6201 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1032 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("2xoneWaitActor(chooseTwoActor(after, never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6205 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1036 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1037 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out1(N);
															#line 1038 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out2(N);
															#line 1039 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1040 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1041 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(pipe[i].getFuture(), never);
															#line 1042 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out1[i] = oneWaitActor(f);
															#line 1043 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out2[i] = oneWaitActor(f);
															#line 6224 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1045 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1046 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1047 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out2[i].isReady());
															#line 6232 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1049 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("2xoneWaitActor(chooseTwoActor(fifo, never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6236 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1053 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1054 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out1(N);
															#line 1055 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out2(N);
															#line 1056 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1057 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1058 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(oneWaitActor(pipe[i].getFuture()), never);
															#line 1059 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out1[i] = oneWaitActor(f);
															#line 1060 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out2[i] = oneWaitActor(f);
															#line 6255 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1062 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1063 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1064 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out2[i].isReady());
															#line 6263 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1066 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("2xoneWaitActor(chooseTwoActor(oneWaitActor(fifo), never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6267 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1070 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1071 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out1(N);
															#line 1072 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out2(N);
															#line 1073 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1074 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1075 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					g_cheese = pipe[i].getFuture();
															#line 1076 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(cheeseWaitActor(), never);
															#line 1077 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					g_cheese = f;
															#line 1078 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out1[i] = cheeseWaitActor();
															#line 1079 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					out2[i] = cheeseWaitActor();
															#line 6290 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1081 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1082 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1083 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out2[i].isReady());
															#line 6298 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1085 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("2xcheeseActor(chooseTwoActor(cheeseActor(fifo), never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 1086 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("sizeof(CheeseWaitActorActor) == %zu\n", sizeof(CheeseWaitActorActor));
															#line 6304 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1090 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				PromiseStream<int> data;
															#line 1091 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1092 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				Future<int> sum = sumActor(data.getFuture());
															#line 1093 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1094 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					data.send(1);
															#line 6317 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1095 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				data.sendError(end_of_stream());
															#line 1096 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				ASSERT(sum.get() == N);
															#line 1097 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("sumActor: %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6325 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1101 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1102 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> ps(3);
															#line 1103 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> fs(3);
															#line 1105 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1106 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ps.clear();
															#line 1107 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					ps.resize(3);
															#line 1108 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					for(int j = 0;j < ps.size();j++) {
															#line 1109 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
						fs[j] = ps[j].getFuture();
															#line 6344 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
					}
															#line 1111 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					Future<Void> q = quorum(fs, 2);
															#line 1112 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					for( auto& p : ps ) {
															#line 1113 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
						p.send(Void());
															#line 6352 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
					}
				}
															#line 1115 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("quorum(2/3): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6357 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1118 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase812Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase812ActorState(); static_cast<FlowTestCase812Actor*>(this)->destroy(); return 0; }
															#line 6361 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase812Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase812ActorState();
			static_cast<FlowTestCase812Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase812ActorState();
		static_cast<FlowTestCase812Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 6385 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase812()
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase812Actor final : public Actor<Void>, public FastAllocated<FlowTestCase812Actor>, public FlowTestCase812ActorState<FlowTestCase812Actor> {
															#line 6390 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase812Actor>::operator new;
	using FastAllocated<FlowTestCase812Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase812Actor(UnitTestParameters const& params) 
															#line 6400 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase812ActorState<FlowTestCase812Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase812", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase812");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase812", reinterpret_cast<unsigned long>(this), -1);

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
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase812( UnitTestParameters const& params ) {
															#line 812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase812Actor(params));
															#line 6427 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase812, "#flow/flow/perf/actor patterns")

#line 1120 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

template <class YAM>
struct YAMRandom {
	YAM yam;
	std::vector<Future<Void>> onchanges;
	int kmax;

	YAMRandom() : kmax(3) {}

	void randomOp() {
		if (deterministicRandom()->random01() < 0.01)
			while (!check_yield())
				;

		int k = deterministicRandom()->randomInt(0, kmax);
		int op = deterministicRandom()->randomInt(0, 7);
		// printf("%d",op);
		if (op == 0) {
			onchanges.push_back(yam.onChange(k));
		} else if (op == 1) {
			onchanges.push_back(trigger([this]() { this->randomOp(); }, yam.onChange(k)));
		} else if (op == 2) {
			if (onchanges.size()) {
				int i = deterministicRandom()->randomInt(0, onchanges.size());
				onchanges[i] = onchanges.back();
				onchanges.pop_back();
			}
		} else if (op == 3) {
			onchanges.clear();
		} else if (op == 4) {
			int v = deterministicRandom()->randomInt(0, 3);
			yam.set(k, v);
		} else if (op == 5) {
			yam.trigger(k);
		} else if (op == 6) {
			int a = deterministicRandom()->randomInt(0, kmax);
			int b = deterministicRandom()->randomInt(0, kmax);
			yam.triggerRange(std::min(a, b), std::max(a, b) + 1);
		}
	}
};

															#line 6474 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1162()
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1162Actor>
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1162ActorState {
															#line 6481 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1162ActorState(UnitTestParameters const& params) 
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1163 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   yamr(),
															#line 1164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   it()
															#line 6492 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1162", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1162ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1162", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			it = 0;
															#line 6507 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1162ActorState();
		static_cast<FlowTestCase1162Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1169 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1162Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1162ActorState(); static_cast<FlowTestCase1162Actor*>(this)->destroy(); return 0; }
															#line 6530 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1162Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1162ActorState();
		static_cast<FlowTestCase1162Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 1165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!(it < 100000))
															#line 6549 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1166 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		yamr.randomOp();
															#line 1167 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = yield();
															#line 1167 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1162Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6559 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase1162Actor*>(this)->actor_wait_state = 1;
															#line 1167 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1162Actor, 0, Void >*>(static_cast<FlowTestCase1162Actor*>(this)));
															#line 6564 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6586 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6595 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase1162Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1162Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1162Actor*>(this)->ActorCallback< FlowTestCase1162Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1162Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1162", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1162", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1162Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1162", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1162", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1162Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1162", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1162", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1163 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	YAMRandom<YieldedAsyncMap<int, int>> yamr;
															#line 1164 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int it;
															#line 6669 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1162()
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1162Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1162Actor, 0, Void >, public FastAllocated<FlowTestCase1162Actor>, public FlowTestCase1162ActorState<FlowTestCase1162Actor> {
															#line 6674 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1162Actor>::operator new;
	using FastAllocated<FlowTestCase1162Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1162Actor, 0, Void >;
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1162Actor(UnitTestParameters const& params) 
															#line 6685 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1162ActorState<FlowTestCase1162Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1162", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1162");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1162", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1162Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1162( UnitTestParameters const& params ) {
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1162Actor(params));
															#line 6713 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1162, "/flow/flow/YieldedAsyncMap/randomized")

#line 1171 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 6719 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1172()
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1172Actor>
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1172ActorState {
															#line 6726 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1172ActorState(UnitTestParameters const& params) 
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   yamr(),
															#line 1174 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   it()
															#line 6737 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1172", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1172ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1172", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1175 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			it = 0;
															#line 6752 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1172ActorState();
		static_cast<FlowTestCase1172Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1179 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1172Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1172ActorState(); static_cast<FlowTestCase1172Actor*>(this)->destroy(); return 0; }
															#line 6775 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1172Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1172ActorState();
		static_cast<FlowTestCase1172Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 1175 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!(it < 100000))
															#line 6794 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1176 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		yamr.randomOp();
															#line 1177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = yield();
															#line 1177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1172Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6804 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase1172Actor*>(this)->actor_wait_state = 1;
															#line 1177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1172Actor, 0, Void >*>(static_cast<FlowTestCase1172Actor*>(this)));
															#line 6809 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1175 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6831 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1175 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6840 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase1172Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1172Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1172Actor*>(this)->ActorCallback< FlowTestCase1172Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1172Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1172", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1172", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1172Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1172", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1172", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1172Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1172", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1172", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	YAMRandom<AsyncMap<int, int>> yamr;
															#line 1174 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int it;
															#line 6914 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1172()
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1172Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1172Actor, 0, Void >, public FastAllocated<FlowTestCase1172Actor>, public FlowTestCase1172ActorState<FlowTestCase1172Actor> {
															#line 6919 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1172Actor>::operator new;
	using FastAllocated<FlowTestCase1172Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1172Actor, 0, Void >;
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1172Actor(UnitTestParameters const& params) 
															#line 6930 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1172ActorState<FlowTestCase1172Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1172", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1172");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1172", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1172Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1172( UnitTestParameters const& params ) {
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1172Actor(params));
															#line 6958 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1172, "/flow/flow/AsyncMap/randomized")

#line 1181 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 6964 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1182()
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1182Actor>
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1182ActorState {
															#line 6971 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1182ActorState(UnitTestParameters const& params) 
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1183 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   yam(),
															#line 1184 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   y0(yam.onChange(1))
															#line 6982 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1182", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1182ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1182", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1185 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			yam.setUnconditional(1, 0);
															#line 1186 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1 = yam.onChange(1);
															#line 1187 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1a = yam.onChange(1);
															#line 1188 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1b = yam.onChange(1);
															#line 1189 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			yam.set(1, 1);
															#line 1193 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y2 = yam.onChange(1);
															#line 1194 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = reportErrors(y0, "Y0");
															#line 1194 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7011 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 1;
															#line 1194 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 0, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7016 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1182ActorState();
		static_cast<FlowTestCase1182Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reportErrors(y1, "Y1");
															#line 1195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7041 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 2;
															#line 1195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 1, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7046 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reportErrors(y1, "Y1");
															#line 1195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7057 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 2;
															#line 1195 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 1, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7062 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1182Actor*>(this)->ActorCallback< FlowTestCase1182Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1182Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_2 = reportErrors(y1a, "Y1a");
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7136 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 3;
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 2, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7141 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_2 = reportErrors(y1a, "Y1a");
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7152 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 3;
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 2, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7157 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1182Actor*>(this)->ActorCallback< FlowTestCase1182Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase1182Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_3 = reportErrors(y1b, "Y1b");
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7231 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 4;
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 3, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7236 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_3 = reportErrors(y1b, "Y1b");
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7247 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 4;
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 3, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7252 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1182Actor*>(this)->ActorCallback< FlowTestCase1182Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase1182Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 1198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_4 = reportErrors(timeout(y2, 5, Void()), "Y2");
															#line 1198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7326 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 5;
															#line 1198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 4, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7331 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_4 = reportErrors(timeout(y2, 5, Void()), "Y2");
															#line 1198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7342 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 5;
															#line 1198 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1182Actor, 4, Void >*>(static_cast<FlowTestCase1182Actor*>(this)));
															#line 7347 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1182Actor*>(this)->ActorCallback< FlowTestCase1182Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase1182Actor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 1200 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1182Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1182ActorState(); static_cast<FlowTestCase1182Actor*>(this)->destroy(); return 0; }
															#line 7419 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1182Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1182ActorState();
		static_cast<FlowTestCase1182Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 1200 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1182Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1182ActorState(); static_cast<FlowTestCase1182Actor*>(this)->destroy(); return 0; }
															#line 7431 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1182Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1182ActorState();
		static_cast<FlowTestCase1182Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1182Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1182Actor*>(this)->ActorCallback< FlowTestCase1182Actor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1182Actor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< FlowTestCase1182Actor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1183 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	YieldedAsyncMap<int, int> yam;
															#line 1184 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y0;
															#line 1186 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1;
															#line 1187 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1a;
															#line 1188 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1b;
															#line 1193 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y2;
															#line 7516 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1182()
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1182Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1182Actor, 0, Void >, public ActorCallback< FlowTestCase1182Actor, 1, Void >, public ActorCallback< FlowTestCase1182Actor, 2, Void >, public ActorCallback< FlowTestCase1182Actor, 3, Void >, public ActorCallback< FlowTestCase1182Actor, 4, Void >, public FastAllocated<FlowTestCase1182Actor>, public FlowTestCase1182ActorState<FlowTestCase1182Actor> {
															#line 7521 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1182Actor>::operator new;
	using FastAllocated<FlowTestCase1182Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1182Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase1182Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase1182Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase1182Actor, 3, Void >;
friend struct ActorCallback< FlowTestCase1182Actor, 4, Void >;
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1182Actor(UnitTestParameters const& params) 
															#line 7536 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1182ActorState<FlowTestCase1182Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1182", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1182");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1182", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1182Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase1182Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase1182Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase1182Actor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< FlowTestCase1182Actor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1182( UnitTestParameters const& params ) {
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1182Actor(params));
															#line 7568 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1182, "/flow/flow/YieldedAsyncMap/basic")

#line 1202 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 7574 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1203()
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1203Actor>
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1203ActorState {
															#line 7581 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1203ActorState(UnitTestParameters const& params) 
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1204 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   yam()
															#line 7590 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1203", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1203ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1203", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1210 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 0);
															#line 1211 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1 = yam.onChange(1);
															#line 1212 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1a = yam.onChange(1);
															#line 1213 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1b = yam.onChange(1);
															#line 1214 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 1);
															#line 1215 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1.cancel();
															#line 1216 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!y1a.isReady());
															#line 1217 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1a.cancel();
															#line 1218 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!y1b.isReady());
															#line 1219 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 1);
															#line 1220 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			y1b.cancel();
															#line 1221 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(y1b.getError().code() == error_code_actor_cancelled);
															#line 1222 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 0);
															#line 1224 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1203Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1203ActorState(); static_cast<FlowTestCase1203Actor*>(this)->destroy(); return 0; }
															#line 7631 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1203Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1203ActorState();
			static_cast<FlowTestCase1203Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1203ActorState();
		static_cast<FlowTestCase1203Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1204 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	YieldedAsyncMap<int, int> yam;
															#line 1211 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1;
															#line 1212 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1a;
															#line 1213 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1b;
															#line 7663 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1203()
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1203Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1203Actor>, public FlowTestCase1203ActorState<FlowTestCase1203Actor> {
															#line 7668 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1203Actor>::operator new;
	using FastAllocated<FlowTestCase1203Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1203Actor(UnitTestParameters const& params) 
															#line 7678 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1203ActorState<FlowTestCase1203Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1203", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1203");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1203", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1203( UnitTestParameters const& params ) {
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1203Actor(params));
															#line 7705 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1203, "/flow/flow/YieldedAsyncMap/cancel")

#line 1226 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 7711 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1227()
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1227Actor>
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1227ActorState {
															#line 7718 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1227ActorState(UnitTestParameters const& params) 
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1228 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   yam(),
															#line 1230 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   y1(yam.onChange(1)),
															#line 1231 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   y2(yam.onChange(2))
															#line 7731 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1227", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1227ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1227", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1233 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			auto* pyam = &yam;
															#line 1234 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			uncancellable(trigger( [pyam]() { printf("Triggered\n"); pyam->triggerAll(); }, delay(1)));
															#line 1241 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = y1;
															#line 1241 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1227Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7752 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1227Actor*>(this)->actor_wait_state = 1;
															#line 1241 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1227Actor, 0, Void >*>(static_cast<FlowTestCase1227Actor*>(this)));
															#line 7757 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1227ActorState();
		static_cast<FlowTestCase1227Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1242 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		printf("Got y1\n");
															#line 1243 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		y2.cancel();
															#line 1245 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1227Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1227ActorState(); static_cast<FlowTestCase1227Actor*>(this)->destroy(); return 0; }
															#line 7784 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1227Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1227ActorState();
		static_cast<FlowTestCase1227Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1242 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		printf("Got y1\n");
															#line 1243 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		y2.cancel();
															#line 1245 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1227Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1227ActorState(); static_cast<FlowTestCase1227Actor*>(this)->destroy(); return 0; }
															#line 7800 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1227Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1227ActorState();
		static_cast<FlowTestCase1227Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1227Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1227Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1227Actor*>(this)->ActorCallback< FlowTestCase1227Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1227Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1227", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1227", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1227Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1227", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1227", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1227Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1227", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1227", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1228 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	YieldedAsyncMap<int, int> yam;
															#line 1230 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1;
															#line 1231 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y2;
															#line 7879 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1227()
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1227Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1227Actor, 0, Void >, public FastAllocated<FlowTestCase1227Actor>, public FlowTestCase1227ActorState<FlowTestCase1227Actor> {
															#line 7884 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1227Actor>::operator new;
	using FastAllocated<FlowTestCase1227Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1227Actor, 0, Void >;
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1227Actor(UnitTestParameters const& params) 
															#line 7895 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1227ActorState<FlowTestCase1227Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1227", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1227");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1227", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1227Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1227( UnitTestParameters const& params ) {
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1227Actor(params));
															#line 7923 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1227, "/flow/flow/YieldedAsyncMap/cancel2")

#line 1247 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 7929 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1248()
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1248Actor>
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1248ActorState {
															#line 7936 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1248ActorState(UnitTestParameters const& params) 
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 7943 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1248", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1248ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1248", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1249 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			AsyncVar<int> av;
															#line 1250 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> ch = av.onChange();
															#line 1251 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!ch.isReady());
															#line 1252 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			av.set(5);
															#line 1253 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(ch.isReady());
															#line 1254 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(av.get() == 5);
															#line 1256 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ch = av.onChange();
															#line 1257 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!ch.isReady());
															#line 1258 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			av.set(6);
															#line 1259 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(ch.isReady());
															#line 1260 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(av.get() == 6);
															#line 1262 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1248Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1248ActorState(); static_cast<FlowTestCase1248Actor*>(this)->destroy(); return 0; }
															#line 7980 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1248Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1248ActorState();
			static_cast<FlowTestCase1248Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1248ActorState();
		static_cast<FlowTestCase1248Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 8004 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1248()
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1248Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1248Actor>, public FlowTestCase1248ActorState<FlowTestCase1248Actor> {
															#line 8009 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1248Actor>::operator new;
	using FastAllocated<FlowTestCase1248Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1248Actor(UnitTestParameters const& params) 
															#line 8019 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1248ActorState<FlowTestCase1248Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1248", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1248");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1248", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1248( UnitTestParameters const& params ) {
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1248Actor(params));
															#line 8046 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1248, "/flow/flow/AsyncVar/basic")

#line 1264 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 8052 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via waitAfterCancel()
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class WaitAfterCancelActor>
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class WaitAfterCancelActorState {
															#line 8059 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	WaitAfterCancelActorState(int* const& output) 
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : output(output)
															#line 8066 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("waitAfterCancel", reinterpret_cast<unsigned long>(this));

	}
	~WaitAfterCancelActorState() 
	{
		fdb_probe_actor_destroy("waitAfterCancel", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1266 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			*output = 0;
															#line 8081 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			try {
															#line 1268 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Never();
															#line 1268 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<WaitAfterCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 8087 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WaitAfterCancelActor*>(this)->actor_wait_state = 1;
															#line 1268 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitAfterCancelActor, 0, Void >*>(static_cast<WaitAfterCancelActor*>(this)));
															#line 8092 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
		this->~WaitAfterCancelActorState();
		static_cast<WaitAfterCancelActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1272 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(false);
															#line 1273 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<WaitAfterCancelActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAfterCancelActorState(); static_cast<WaitAfterCancelActor*>(this)->destroy(); return 0; }
															#line 8123 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<WaitAfterCancelActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAfterCancelActorState();
		static_cast<WaitAfterCancelActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1270 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = (*output = 1, Future<Void>(Void()));
															#line 1270 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<WaitAfterCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8138 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_1.get(), loopDepth); };
			static_cast<WaitAfterCancelActor*>(this)->actor_wait_state = 2;
															#line 1270 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitAfterCancelActor, 1, Void >*>(static_cast<WaitAfterCancelActor*>(this)));
															#line 8143 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<WaitAfterCancelActor*>(this)->actor_wait_state > 0) static_cast<WaitAfterCancelActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAfterCancelActor*>(this)->ActorCallback< WaitAfterCancelActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAfterCancelActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitAfterCancelActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitAfterCancelActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WaitAfterCancelActor*>(this)->actor_wait_state > 0) static_cast<WaitAfterCancelActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAfterCancelActor*>(this)->ActorCallback< WaitAfterCancelActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAfterCancelActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitAfterCancelActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitAfterCancelActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int* output;
															#line 8319 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via waitAfterCancel()
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class WaitAfterCancelActor final : public Actor<Void>, public ActorCallback< WaitAfterCancelActor, 0, Void >, public ActorCallback< WaitAfterCancelActor, 1, Void >, public FastAllocated<WaitAfterCancelActor>, public WaitAfterCancelActorState<WaitAfterCancelActor> {
															#line 8324 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<WaitAfterCancelActor>::operator new;
	using FastAllocated<WaitAfterCancelActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitAfterCancelActor, 0, Void >;
friend struct ActorCallback< WaitAfterCancelActor, 1, Void >;
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	WaitAfterCancelActor(int* const& output) 
															#line 8336 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   WaitAfterCancelActorState<WaitAfterCancelActor>(output)
	{
		fdb_probe_actor_enter("waitAfterCancel", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitAfterCancel");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitAfterCancel", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitAfterCancelActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitAfterCancelActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> waitAfterCancel( int* const& output ) {
															#line 1265 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new WaitAfterCancelActor(output));
															#line 8365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1275 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 8370 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1276()
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1276Actor>
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1276ActorState {
															#line 8377 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1276ActorState(UnitTestParameters const& params) 
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 8384 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1276", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1276ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1276", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1277 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			int a = -1;
															#line 1278 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Future<Void> f = waitAfterCancel(&a);
															#line 1279 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a == 0);
															#line 1280 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			f.cancel();
															#line 1281 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a == 1);
															#line 1282 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1276Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1276ActorState(); static_cast<FlowTestCase1276Actor*>(this)->destroy(); return 0; }
															#line 8409 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1276Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1276ActorState();
			static_cast<FlowTestCase1276Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1276ActorState();
		static_cast<FlowTestCase1276Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 8433 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1276()
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1276Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1276Actor>, public FlowTestCase1276ActorState<FlowTestCase1276Actor> {
															#line 8438 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1276Actor>::operator new;
	using FastAllocated<FlowTestCase1276Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1276Actor(UnitTestParameters const& params) 
															#line 8448 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1276ActorState<FlowTestCase1276Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1276", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1276");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1276", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1276( UnitTestParameters const& params ) {
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1276Actor(params));
															#line 8475 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1276, "/fdbrpc/flow/wait_expression_after_cancel")

#line 1284 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

// Tests for https://github.com/apple/foundationdb/issues/1226

template <class>
struct ShouldNotGoIntoClassContextStack;

class Foo1 {
public:
	explicit Foo1(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 8490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo1* const& self );
template <class> friend class Foo1_FooActorActorState;

#line 1295 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 8499 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class Foo1_FooActorActor>
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo1_FooActorActorState {
															#line 8505 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo1_FooActorActorState(Foo1* const& self) 
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 8512 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("fooActor", reinterpret_cast<unsigned long>(this));

	}
	~Foo1_FooActorActorState() 
	{
		fdb_probe_actor_destroy("fooActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1300 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1300 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo1_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8529 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo1_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1300 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo1_FooActorActor, 0, Void >*>(static_cast<Foo1_FooActorActor*>(this)));
															#line 8534 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~Foo1_FooActorActorState();
		static_cast<Foo1_FooActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1301 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo1_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo1_FooActorActorState(); static_cast<Foo1_FooActorActor*>(this)->destroy(); return 0; }
															#line 8557 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo1_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo1_FooActorActorState();
		static_cast<Foo1_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1301 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo1_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo1_FooActorActorState(); static_cast<Foo1_FooActorActor*>(this)->destroy(); return 0; }
															#line 8569 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo1_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo1_FooActorActorState();
		static_cast<Foo1_FooActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Foo1_FooActorActor*>(this)->actor_wait_state > 0) static_cast<Foo1_FooActorActor*>(this)->actor_wait_state = 0;
		static_cast<Foo1_FooActorActor*>(this)->ActorCallback< Foo1_FooActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Foo1_FooActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Foo1_FooActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Foo1_FooActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo1* self;
															#line 8642 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo1_FooActorActor final : public Actor<int>, public ActorCallback< Foo1_FooActorActor, 0, Void >, public FastAllocated<Foo1_FooActorActor>, public Foo1_FooActorActorState<Foo1_FooActorActor> {
															#line 8647 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo1_FooActorActor>::operator new;
	using FastAllocated<Foo1_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo1_FooActorActor, 0, Void >;
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo1_FooActorActor(Foo1* const& self) 
															#line 8658 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   Foo1_FooActorActorState<Foo1_FooActorActor>(self)
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fooActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Foo1_FooActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo1::fooActor( Foo1* const& self ) {
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo1_FooActorActor(self));
															#line 8685 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1303 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

class [[nodiscard]] Foo2 {
public:
	explicit Foo2(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 8694 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo2* const& self );
template <class> friend class Foo2_FooActorActorState;

#line 1309 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 8703 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class Foo2_FooActorActor>
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo2_FooActorActorState {
															#line 8709 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo2_FooActorActorState(Foo2* const& self) 
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 8716 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("fooActor", reinterpret_cast<unsigned long>(this));

	}
	~Foo2_FooActorActorState() 
	{
		fdb_probe_actor_destroy("fooActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo2_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8733 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo2_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1314 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo2_FooActorActor, 0, Void >*>(static_cast<Foo2_FooActorActor*>(this)));
															#line 8738 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~Foo2_FooActorActorState();
		static_cast<Foo2_FooActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1315 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo2_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo2_FooActorActorState(); static_cast<Foo2_FooActorActor*>(this)->destroy(); return 0; }
															#line 8761 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo2_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo2_FooActorActorState();
		static_cast<Foo2_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1315 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo2_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo2_FooActorActorState(); static_cast<Foo2_FooActorActor*>(this)->destroy(); return 0; }
															#line 8773 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo2_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo2_FooActorActorState();
		static_cast<Foo2_FooActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Foo2_FooActorActor*>(this)->actor_wait_state > 0) static_cast<Foo2_FooActorActor*>(this)->actor_wait_state = 0;
		static_cast<Foo2_FooActorActor*>(this)->ActorCallback< Foo2_FooActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Foo2_FooActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Foo2_FooActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Foo2_FooActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo2* self;
															#line 8846 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo2_FooActorActor final : public Actor<int>, public ActorCallback< Foo2_FooActorActor, 0, Void >, public FastAllocated<Foo2_FooActorActor>, public Foo2_FooActorActorState<Foo2_FooActorActor> {
															#line 8851 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo2_FooActorActor>::operator new;
	using FastAllocated<Foo2_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo2_FooActorActor, 0, Void >;
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo2_FooActorActor(Foo2* const& self) 
															#line 8862 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   Foo2_FooActorActorState<Foo2_FooActorActor>(self)
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fooActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Foo2_FooActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo2::fooActor( Foo2* const& self ) {
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo2_FooActorActor(self));
															#line 8889 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1317 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

class alignas(4) Foo3 {
public:
	explicit Foo3(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 8898 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo3* const& self );
template <class> friend class Foo3_FooActorActorState;

#line 1323 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 8907 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class Foo3_FooActorActor>
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo3_FooActorActorState {
															#line 8913 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo3_FooActorActorState(Foo3* const& self) 
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 8920 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("fooActor", reinterpret_cast<unsigned long>(this));

	}
	~Foo3_FooActorActorState() 
	{
		fdb_probe_actor_destroy("fooActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo3_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8937 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo3_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1328 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo3_FooActorActor, 0, Void >*>(static_cast<Foo3_FooActorActor*>(this)));
															#line 8942 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~Foo3_FooActorActorState();
		static_cast<Foo3_FooActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1329 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo3_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo3_FooActorActorState(); static_cast<Foo3_FooActorActor*>(this)->destroy(); return 0; }
															#line 8965 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo3_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo3_FooActorActorState();
		static_cast<Foo3_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1329 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo3_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo3_FooActorActorState(); static_cast<Foo3_FooActorActor*>(this)->destroy(); return 0; }
															#line 8977 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo3_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo3_FooActorActorState();
		static_cast<Foo3_FooActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Foo3_FooActorActor*>(this)->actor_wait_state > 0) static_cast<Foo3_FooActorActor*>(this)->actor_wait_state = 0;
		static_cast<Foo3_FooActorActor*>(this)->ActorCallback< Foo3_FooActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Foo3_FooActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Foo3_FooActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Foo3_FooActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo3* self;
															#line 9050 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo3_FooActorActor final : public Actor<int>, public ActorCallback< Foo3_FooActorActor, 0, Void >, public FastAllocated<Foo3_FooActorActor>, public Foo3_FooActorActorState<Foo3_FooActorActor> {
															#line 9055 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo3_FooActorActor>::operator new;
	using FastAllocated<Foo3_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo3_FooActorActor, 0, Void >;
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo3_FooActorActor(Foo3* const& self) 
															#line 9066 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   Foo3_FooActorActorState<Foo3_FooActorActor>(self)
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fooActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Foo3_FooActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo3::fooActor( Foo3* const& self ) {
															#line 1327 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo3_FooActorActor(self));
															#line 9093 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1331 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

struct Super {};

class Foo4 : Super {
public:
	explicit Foo4(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 9104 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo4* const& self );
template <class> friend class Foo4_FooActorActorState;

#line 1339 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 9113 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class Foo4_FooActorActor>
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo4_FooActorActorState {
															#line 9119 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo4_FooActorActorState(Foo4* const& self) 
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 9126 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("fooActor", reinterpret_cast<unsigned long>(this));

	}
	~Foo4_FooActorActorState() 
	{
		fdb_probe_actor_destroy("fooActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1344 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1344 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo4_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9143 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo4_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1344 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo4_FooActorActor, 0, Void >*>(static_cast<Foo4_FooActorActor*>(this)));
															#line 9148 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~Foo4_FooActorActorState();
		static_cast<Foo4_FooActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1345 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo4_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo4_FooActorActorState(); static_cast<Foo4_FooActorActor*>(this)->destroy(); return 0; }
															#line 9171 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo4_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo4_FooActorActorState();
		static_cast<Foo4_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1345 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo4_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo4_FooActorActorState(); static_cast<Foo4_FooActorActor*>(this)->destroy(); return 0; }
															#line 9183 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo4_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo4_FooActorActorState();
		static_cast<Foo4_FooActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Foo4_FooActorActor*>(this)->actor_wait_state > 0) static_cast<Foo4_FooActorActor*>(this)->actor_wait_state = 0;
		static_cast<Foo4_FooActorActor*>(this)->ActorCallback< Foo4_FooActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Foo4_FooActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Foo4_FooActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Foo4_FooActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo4* self;
															#line 9256 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Foo4_FooActorActor final : public Actor<int>, public ActorCallback< Foo4_FooActorActor, 0, Void >, public FastAllocated<Foo4_FooActorActor>, public Foo4_FooActorActorState<Foo4_FooActorActor> {
															#line 9261 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo4_FooActorActor>::operator new;
	using FastAllocated<Foo4_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo4_FooActorActor, 0, Void >;
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Foo4_FooActorActor(Foo4* const& self) 
															#line 9272 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   Foo4_FooActorActorState<Foo4_FooActorActor>(self)
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fooActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Foo4_FooActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo4::fooActor( Foo4* const& self ) {
															#line 1343 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo4_FooActorActor(self));
															#line 9299 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1347 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

struct Outer {
	class Foo5 : Super {
	public:
		explicit Foo5(int x) : x(x) {}
		Future<int> foo() { return fooActor(this); }
																	#line 9309 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo5* const& self );
template <class> friend class Outer_Foo5_FooActorActorState;

#line 1354 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

	private:
		int x;
	};
};
															#line 9319 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class Outer_Foo5_FooActorActor>
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Outer_Foo5_FooActorActorState {
															#line 9325 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Outer_Foo5_FooActorActorState(Outer::Foo5* const& self) 
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 9332 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("fooActor", reinterpret_cast<unsigned long>(this));

	}
	~Outer_Foo5_FooActorActorState() 
	{
		fdb_probe_actor_destroy("fooActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1360 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1360 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Outer_Foo5_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9349 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Outer_Foo5_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1360 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Outer_Foo5_FooActorActor, 0, Void >*>(static_cast<Outer_Foo5_FooActorActor*>(this)));
															#line 9354 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~Outer_Foo5_FooActorActorState();
		static_cast<Outer_Foo5_FooActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1361 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Outer_Foo5_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Outer_Foo5_FooActorActorState(); static_cast<Outer_Foo5_FooActorActor*>(this)->destroy(); return 0; }
															#line 9377 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Outer_Foo5_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Outer_Foo5_FooActorActorState();
		static_cast<Outer_Foo5_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1361 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Outer_Foo5_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Outer_Foo5_FooActorActorState(); static_cast<Outer_Foo5_FooActorActor*>(this)->destroy(); return 0; }
															#line 9389 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Outer_Foo5_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Outer_Foo5_FooActorActorState();
		static_cast<Outer_Foo5_FooActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Outer_Foo5_FooActorActor*>(this)->actor_wait_state > 0) static_cast<Outer_Foo5_FooActorActor*>(this)->actor_wait_state = 0;
		static_cast<Outer_Foo5_FooActorActor*>(this)->ActorCallback< Outer_Foo5_FooActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Outer_Foo5_FooActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Outer_Foo5_FooActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Outer_Foo5_FooActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Outer::Foo5* self;
															#line 9462 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class Outer_Foo5_FooActorActor final : public Actor<int>, public ActorCallback< Outer_Foo5_FooActorActor, 0, Void >, public FastAllocated<Outer_Foo5_FooActorActor>, public Outer_Foo5_FooActorActorState<Outer_Foo5_FooActorActor> {
															#line 9467 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Outer_Foo5_FooActorActor>::operator new;
	using FastAllocated<Outer_Foo5_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Outer_Foo5_FooActorActor, 0, Void >;
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Outer_Foo5_FooActorActor(Outer::Foo5* const& self) 
															#line 9478 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<int>(),
		   Outer_Foo5_FooActorActorState<Outer_Foo5_FooActorActor>(self)
	{
		fdb_probe_actor_enter("fooActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fooActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fooActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Outer_Foo5_FooActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Outer::Foo5::fooActor( Outer::Foo5* const& self ) {
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Outer_Foo5_FooActorActor(self));
															#line 9505 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1363 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

// Meant to be run with -fsanitize=undefined
															#line 9511 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1365()
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1365Actor>
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1365ActorState {
															#line 9518 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1365ActorState(UnitTestParameters const& params) 
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 9525 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1365", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1365ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1365", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1366 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt(std::numeric_limits<int>::min(), 0);
															#line 1367 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt(0, std::numeric_limits<int>::max());
															#line 1368 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
															#line 1369 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt(std::numeric_limits<int>::min(), std::numeric_limits<int>::min() + 1) == std::numeric_limits<int>::min());
															#line 1371 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt(std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::max()) == std::numeric_limits<int>::max() - 1);
															#line 1374 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::min(), 0);
															#line 1375 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt64(0, std::numeric_limits<int64_t>::max());
															#line 1376 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
															#line 1377 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min() + 1) == std::numeric_limits<int64_t>::min());
															#line 1380 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::max() - 1, std::numeric_limits<int64_t>::max()) == std::numeric_limits<int64_t>::max() - 1);
															#line 1383 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1365Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1365ActorState(); static_cast<FlowTestCase1365Actor*>(this)->destroy(); return 0; }
															#line 9560 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1365Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1365ActorState();
			static_cast<FlowTestCase1365Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1365ActorState();
		static_cast<FlowTestCase1365Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 9584 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1365()
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1365Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1365Actor>, public FlowTestCase1365ActorState<FlowTestCase1365Actor> {
															#line 9589 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1365Actor>::operator new;
	using FastAllocated<FlowTestCase1365Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1365Actor(UnitTestParameters const& params) 
															#line 9599 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1365ActorState<FlowTestCase1365Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1365", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1365");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1365", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1365( UnitTestParameters const& params ) {
															#line 1365 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1365Actor(params));
															#line 9626 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1365, "/flow/DeterministicRandom/SignedOverflow")

#line 1385 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

struct Tracker {
	int copied;
	bool moved;
	Tracker(int copied = 0) : copied(copied), moved(false) {}
	Tracker(Tracker&& other) : Tracker(other.copied) {
		ASSERT(!other.moved);
		other.moved = true;
	}
	Tracker& operator=(Tracker&& other) {
		ASSERT(!other.moved);
		other.moved = true;
		this->moved = false;
		this->copied = other.copied;
		return *this;
	}
	Tracker(const Tracker& other) : Tracker(other.copied + 1) { ASSERT(!other.moved); }
	Tracker& operator=(const Tracker& other) {
		ASSERT(!other.moved);
		this->moved = false;
		this->copied = other.copied + 1;
		return *this;
	}
	~Tracker() = default;

																#line 9656 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via listen()
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class ListenActor>
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class ListenActorState {
															#line 9662 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	ListenActorState(FutureStream<Tracker> const& stream) 
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : stream(stream)
															#line 9669 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("listen", reinterpret_cast<unsigned long>(this));

	}
	~ListenActorState() 
	{
		fdb_probe_actor_destroy("listen", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1411 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_0 = stream;
															#line 1411 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9686 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<ListenActor*>(this)->actor_wait_state = 1;
															#line 1411 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ListenActor, 0, Tracker >*>(static_cast<ListenActor*>(this)));
															#line 9691 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~ListenActorState();
		static_cast<ListenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker const& movedTracker,int loopDepth) 
	{
															#line 1412 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1413 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1414 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ListenActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ListenActorState(); static_cast<ListenActor*>(this)->destroy(); return 0; }
															#line 9718 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ListenActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ListenActorState();
		static_cast<ListenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker && movedTracker,int loopDepth) 
	{
															#line 1412 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1413 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1414 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ListenActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ListenActorState(); static_cast<ListenActor*>(this)->destroy(); return 0; }
															#line 9734 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ListenActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ListenActorState();
		static_cast<ListenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Tracker const& movedTracker,int loopDepth) 
	{
		loopDepth = a_body1cont1(movedTracker, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Tracker && movedTracker,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(movedTracker), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListenActor*>(this)->actor_wait_state > 0) static_cast<ListenActor*>(this)->actor_wait_state = 0;
		static_cast<ListenActor*>(this)->ActorSingleCallback< ListenActor, 0, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ListenActor, 0, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ListenActor, 0, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< ListenActor, 0, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FutureStream<Tracker> stream;
															#line 9807 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via listen()
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class ListenActor final : public Actor<Void>, public ActorSingleCallback< ListenActor, 0, Tracker >, public FastAllocated<ListenActor>, public ListenActorState<ListenActor> {
															#line 9812 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<ListenActor>::operator new;
	using FastAllocated<ListenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ListenActor, 0, Tracker >;
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	ListenActor(FutureStream<Tracker> const& stream) 
															#line 9823 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   ListenActorState<ListenActor>(stream)
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listen");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< ListenActor, 0, Tracker >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> listen( FutureStream<Tracker> const& stream ) {
															#line 1410 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new ListenActor(stream));
															#line 9850 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1416 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
};

															#line 9856 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1418()
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1418Actor>
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1418ActorState {
															#line 9863 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1418ActorState(UnitTestParameters const& params) 
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1419 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   stream(),
															#line 1420 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   listener()
															#line 9874 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1418", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1418ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1418", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 1424 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				listener = Tracker::listen(stream.getFuture());
															#line 1425 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				stream.send(Tracker{});
															#line 1426 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = listener;
															#line 1426 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9896 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 1;
															#line 1426 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1418Actor, 0, Void >*>(static_cast<FlowTestCase1418Actor*>(this)));
															#line 9901 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
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
		this->~FlowTestCase1418ActorState();
		static_cast<FlowTestCase1418Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 1432 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			listener = Tracker::listen(stream.getFuture());
															#line 1433 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Tracker namedTracker;
															#line 1434 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			stream.send(namedTracker);
															#line 1435 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = listener;
															#line 1435 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9934 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 2;
															#line 1435 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1418Actor, 1, Void >*>(static_cast<FlowTestCase1418Actor*>(this)));
															#line 9939 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}

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
		if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1418Actor*>(this)->ActorCallback< FlowTestCase1418Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1418Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1418Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1418Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		{
															#line 1440 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			stream.send(Tracker{});
															#line 1441 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			stream.send(Tracker{});
															#line 10027 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				FutureStream<Tracker> __when_expr_2 = stream.getFuture();
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10033 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.pop(), loopDepth); };
				static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 3;
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >*>(static_cast<FlowTestCase1418Actor*>(this)));
															#line 10038 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
		}

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1418Actor*>(this)->ActorCallback< FlowTestCase1418Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1418Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1418Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase1418Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
		{
															#line 1456 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Tracker namedTracker1;
															#line 1457 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			Tracker namedTracker2;
															#line 1458 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			stream.send(namedTracker1);
															#line 1459 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			stream.send(namedTracker2);
															#line 10131 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				FutureStream<Tracker> __when_expr_4 = stream.getFuture();
															#line 1461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10137 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.pop(), loopDepth); };
				static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 5;
															#line 1461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >*>(static_cast<FlowTestCase1418Actor*>(this)));
															#line 10142 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
		}

		return loopDepth;
	}
	int a_body1cont6(int loopDepth) 
	{
		{
															#line 1448 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_3 = stream.getFuture();
															#line 1448 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10156 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.pop(), loopDepth); };
			static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 4;
															#line 1448 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >*>(static_cast<FlowTestCase1418Actor*>(this)));
															#line 10161 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
															#line 1444 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1445 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 10173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Tracker const& __movedTracker,int loopDepth) 
	{
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		movedTracker = __movedTracker;
															#line 10182 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Tracker && __movedTracker,int loopDepth) 
	{
		movedTracker = std::move(__movedTracker);
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1418Actor*>(this)->ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont9(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Tracker const& movedTracker,int loopDepth) 
	{
															#line 1449 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1450 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 10257 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Tracker && movedTracker,int loopDepth) 
	{
															#line 1449 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1450 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 10268 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Tracker const& movedTracker,int loopDepth) 
	{
		loopDepth = a_body1cont10(movedTracker, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Tracker && movedTracker,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(movedTracker), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1418Actor*>(this)->ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont11(int loopDepth) 
	{
															#line 1474 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1418Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1418ActorState(); static_cast<FlowTestCase1418Actor*>(this)->destroy(); return 0; }
															#line 10340 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1418Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1418ActorState();
		static_cast<FlowTestCase1418Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12(int loopDepth) 
	{
		{
															#line 1467 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_5 = stream.getFuture();
															#line 1467 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10355 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont12when1(__when_expr_5.pop(), loopDepth); };
			static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 6;
															#line 1467 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >*>(static_cast<FlowTestCase1418Actor*>(this)));
															#line 10360 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont13(int loopDepth) 
	{
															#line 1462 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!copiedTracker.moved);
															#line 1464 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(copiedTracker.copied == 1);
															#line 10372 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Tracker const& __copiedTracker,int loopDepth) 
	{
															#line 1461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		copiedTracker = __copiedTracker;
															#line 10381 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Tracker && __copiedTracker,int loopDepth) 
	{
		copiedTracker = std::move(__copiedTracker);
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1418Actor*>(this)->ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont12cont1(int loopDepth) 
	{
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont2(Tracker const& copiedTracker,int loopDepth) 
	{
															#line 1468 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!copiedTracker.moved);
															#line 1470 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(copiedTracker.copied == 1);
															#line 10456 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont2(Tracker && copiedTracker,int loopDepth) 
	{
															#line 1468 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!copiedTracker.moved);
															#line 1470 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(copiedTracker.copied == 1);
															#line 10467 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Tracker const& copiedTracker,int loopDepth) 
	{
		loopDepth = a_body1cont12cont2(copiedTracker, loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Tracker && copiedTracker,int loopDepth) 
	{
		loopDepth = a_body1cont12cont2(std::move(copiedTracker), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1418Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1418Actor*>(this)->ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont12when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont12when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1419 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	PromiseStream<Tracker> stream;
															#line 1420 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Future<Void> listener;
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Tracker movedTracker;
															#line 1461 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Tracker copiedTracker;
															#line 10545 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1418()
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1418Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1418Actor, 0, Void >, public ActorCallback< FlowTestCase1418Actor, 1, Void >, public ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >, public ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >, public ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >, public ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >, public FastAllocated<FlowTestCase1418Actor>, public FlowTestCase1418ActorState<FlowTestCase1418Actor> {
															#line 10550 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1418Actor>::operator new;
	using FastAllocated<FlowTestCase1418Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1418Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase1418Actor, 1, Void >;
friend struct ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >;
friend struct ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >;
friend struct ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >;
friend struct ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >;
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1418Actor(UnitTestParameters const& params) 
															#line 10566 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1418ActorState<FlowTestCase1418Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1418", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1418");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1418", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1418Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase1418Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorSingleCallback< FlowTestCase1418Actor, 2, Tracker >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorSingleCallback< FlowTestCase1418Actor, 3, Tracker >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorSingleCallback< FlowTestCase1418Actor, 4, Tracker >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorSingleCallback< FlowTestCase1418Actor, 5, Tracker >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1418( UnitTestParameters const& params ) {
															#line 1418 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1418Actor(params));
															#line 10599 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1418, "/flow/flow/PromiseStream/move")

#line 1476 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 10605 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1477()
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1477Actor>
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1477ActorState {
															#line 10612 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1477ActorState(UnitTestParameters const& params) 
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 10619 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1477", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1477ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1477", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1478 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<Tracker> stream;
															#line 1479 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			stream.send(Tracker{});
															#line 1480 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_0 = stream.getFuture();
															#line 1480 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1477Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10640 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<FlowTestCase1477Actor*>(this)->actor_wait_state = 1;
															#line 1480 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >*>(static_cast<FlowTestCase1477Actor*>(this)));
															#line 10645 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1477ActorState();
		static_cast<FlowTestCase1477Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker const& tracker,int loopDepth) 
	{
															#line 1481 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		Tracker movedTracker = std::move(tracker);
															#line 1482 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(tracker.moved);
															#line 1483 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1484 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1485 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1477Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1477ActorState(); static_cast<FlowTestCase1477Actor*>(this)->destroy(); return 0; }
															#line 10676 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1477Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1477ActorState();
		static_cast<FlowTestCase1477Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker && tracker,int loopDepth) 
	{
															#line 1481 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		Tracker movedTracker = std::move(tracker);
															#line 1482 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(tracker.moved);
															#line 1483 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1484 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1485 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1477Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1477ActorState(); static_cast<FlowTestCase1477Actor*>(this)->destroy(); return 0; }
															#line 10696 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1477Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1477ActorState();
		static_cast<FlowTestCase1477Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Tracker const& tracker,int loopDepth) 
	{
		loopDepth = a_body1cont1(tracker, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Tracker && tracker,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tracker), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase1477Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1477Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1477Actor*>(this)->ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1477", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1477", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1477", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1477", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1477", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1477", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 10769 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1477()
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1477Actor final : public Actor<Void>, public ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >, public FastAllocated<FlowTestCase1477Actor>, public FlowTestCase1477ActorState<FlowTestCase1477Actor> {
															#line 10774 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1477Actor>::operator new;
	using FastAllocated<FlowTestCase1477Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >;
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1477Actor(UnitTestParameters const& params) 
															#line 10785 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1477ActorState<FlowTestCase1477Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1477", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1477");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1477", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< FlowTestCase1477Actor, 0, Tracker >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1477( UnitTestParameters const& params ) {
															#line 1477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1477Actor(params));
															#line 10813 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1477, "/flow/flow/PromiseStream/move2")

#line 1487 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

constexpr double mutexTestDelay = 0.00001;

															#line 10821 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via mutexTest()
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class MutexTestActor>
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class MutexTestActorState {
															#line 10828 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	MutexTestActorState(int const& id,FlowMutex* const& mutex,int const& n,bool const& allowError,bool* const& verbose) 
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : id(id),
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   mutex(mutex),
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   n(n),
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   allowError(allowError),
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   verbose(verbose)
															#line 10843 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("mutexTest", reinterpret_cast<unsigned long>(this));

	}
	~MutexTestActorState() 
	{
		fdb_probe_actor_destroy("mutexTest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1491 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			;
															#line 10858 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~MutexTestActorState();
		static_cast<MutexTestActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1534 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10881 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1535 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d Returning\n", id);
															#line 10885 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1537 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<MutexTestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MutexTestActorState(); static_cast<MutexTestActor*>(this)->destroy(); return 0; }
															#line 10889 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<MutexTestActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MutexTestActorState();
		static_cast<MutexTestActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 1491 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!(n-- > 0))
															#line 10908 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1492 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		d = deterministicRandom()->random01() * mutexTestDelay;
															#line 1493 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10916 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1494 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d wait %f while unlocked\n", id, n, d);
															#line 10920 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1496 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(d);
															#line 1496 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10926 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 1;
															#line 1496 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 0, Void >*>(static_cast<MutexTestActor*>(this)));
															#line 10931 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1498 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10953 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1499 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d locking\n", id, n);
															#line 10957 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<FlowMutex::Lock> __when_expr_1 = mutex->take();
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10963 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 2;
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*>(static_cast<MutexTestActor*>(this)));
															#line 10968 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1498 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10977 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1499 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d locking\n", id, n);
															#line 10981 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<FlowMutex::Lock> __when_expr_1 = mutex->take();
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10987 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 2;
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*>(static_cast<MutexTestActor*>(this)));
															#line 10992 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MutexTestActor*>(this)->actor_wait_state > 0) static_cast<MutexTestActor*>(this)->actor_wait_state = 0;
		static_cast<MutexTestActor*>(this)->ActorCallback< MutexTestActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MutexTestActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MutexTestActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MutexTestActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
															#line 1502 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 11064 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1503 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d locked\n", id, n);
															#line 11068 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1506 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		d = deterministicRandom()->random01() * mutexTestDelay;
															#line 1507 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 11074 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1508 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d wait %f while locked\n", id, n, d);
															#line 11078 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1510 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(d);
															#line 1510 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 11084 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 3;
															#line 1510 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 2, Void >*>(static_cast<MutexTestActor*>(this)));
															#line 11089 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(FlowMutex::Lock const& __lock,int loopDepth) 
	{
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		lock = __lock;
															#line 11098 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(FlowMutex::Lock && __lock,int loopDepth) 
	{
		lock = std::move(__lock);
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MutexTestActor*>(this)->actor_wait_state > 0) static_cast<MutexTestActor*>(this)->actor_wait_state = 0;
		static_cast<MutexTestActor*>(this)->ActorCallback< MutexTestActor, 1, FlowMutex::Lock >::remove();

	}
	void a_callback_fire(ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*,FlowMutex::Lock const& value) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*,FlowMutex::Lock && value) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*,Error err) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 1513 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (n == 0 && allowError)
															#line 11165 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1514 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (deterministicRandom()->coinflip())
															#line 11169 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1516 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11173 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1517 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d sending error\n", id, n);
															#line 11177 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1519 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				lock.error(end_of_stream());
															#line 11181 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			else
			{
															#line 1522 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11187 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1523 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d dropping promise, returning without unlock\n", id, n);
															#line 11191 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 1527 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (*verbose)
															#line 11199 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1528 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("%d:%d unlocking\n", id, n);
															#line 11203 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1530 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			lock.release();
															#line 11207 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 1513 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (n == 0 && allowError)
															#line 11217 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1514 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (deterministicRandom()->coinflip())
															#line 11221 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1516 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11225 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1517 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d sending error\n", id, n);
															#line 11229 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1519 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				lock.error(end_of_stream());
															#line 11233 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
			else
			{
															#line 1522 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11239 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1523 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d dropping promise, returning without unlock\n", id, n);
															#line 11243 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 1527 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (*verbose)
															#line 11251 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1528 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("%d:%d unlocking\n", id, n);
															#line 11255 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1530 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			lock.release();
															#line 11259 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<MutexTestActor*>(this)->actor_wait_state > 0) static_cast<MutexTestActor*>(this)->actor_wait_state = 0;
		static_cast<MutexTestActor*>(this)->ActorCallback< MutexTestActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MutexTestActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< MutexTestActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< MutexTestActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int id;
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowMutex* mutex;
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int n;
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	bool allowError;
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	bool* verbose;
															#line 1492 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	double d;
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowMutex::Lock lock;
															#line 11342 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via mutexTest()
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class MutexTestActor final : public Actor<Void>, public ActorCallback< MutexTestActor, 0, Void >, public ActorCallback< MutexTestActor, 1, FlowMutex::Lock >, public ActorCallback< MutexTestActor, 2, Void >, public FastAllocated<MutexTestActor>, public MutexTestActorState<MutexTestActor> {
															#line 11347 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<MutexTestActor>::operator new;
	using FastAllocated<MutexTestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MutexTestActor, 0, Void >;
friend struct ActorCallback< MutexTestActor, 1, FlowMutex::Lock >;
friend struct ActorCallback< MutexTestActor, 2, Void >;
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	MutexTestActor(int const& id,FlowMutex* const& mutex,int const& n,bool const& allowError,bool* const& verbose) 
															#line 11360 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   MutexTestActorState<MutexTestActor>(id, mutex, n, allowError, verbose)
	{
		fdb_probe_actor_enter("mutexTest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("mutexTest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("mutexTest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MutexTestActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< MutexTestActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<Void> mutexTest( int const& id, FlowMutex* const& mutex, int const& n, bool const& allowError, bool* const& verbose ) {
															#line 1490 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new MutexTestActor(id, mutex, n, allowError, verbose));
															#line 11390 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1539 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"

															#line 11395 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1540()
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1540Actor>
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1540ActorState {
															#line 11402 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1540ActorState(UnitTestParameters const& params) 
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1541 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   count(100000),
															#line 1544 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   verboseSetting(false),
															#line 1546 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		   verboseTestIteration(-1)
															#line 11415 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1540", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1540ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1540", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1549 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				verbose = verboseSetting || count == verboseTestIteration;
															#line 1551 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				;
															#line 11433 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
		this->~FlowTestCase1540ActorState();
		static_cast<FlowTestCase1540Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1607 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1540Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1540ActorState(); static_cast<FlowTestCase1540Actor*>(this)->destroy(); return 0; }
															#line 11462 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1540Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1540ActorState();
		static_cast<FlowTestCase1540Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1603 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("Error at count=%d\n", count + 1);
															#line 1604 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			ASSERT(false);
															#line 11477 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
															#line 1551 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!(--count > 0))
															#line 11505 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1552 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (count % 1000 == 0)
															#line 11511 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1553 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("%d tests left\n", count);
															#line 11515 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1556 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		mutex = FlowMutex();
															#line 1557 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		tests = std::vector<Future<Void>>();
															#line 1559 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		allowErrors = deterministicRandom()->coinflip();
															#line 1560 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (verbose)
															#line 11525 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1561 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("\nTesting allowErrors=%d\n", allowErrors);
															#line 11529 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1564 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		error = Optional<Error>();
															#line 11533 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		try {
															#line 1567 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 10;++i) {
															#line 1568 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				tests.push_back(mutexTest(i, &mutex, 10, allowErrors, &verbose));
															#line 11539 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1570 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(tests);
															#line 1570 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11545 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 1;
															#line 1570 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1540Actor, 0, Void >*>(static_cast<FlowTestCase1540Actor*>(this)));
															#line 11550 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
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
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 1600 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		ASSERT(error.present() == allowErrors);
															#line 11578 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1579 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11588 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1580 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("Caught error %s\n", e.what());
															#line 11592 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1582 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			error = e;
															#line 1586 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			i = int();
															#line 1587 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11600 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1588 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("Waiting for completions.  Future end states:\n");
															#line 11604 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1590 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			i = 0;
															#line 11608 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = a_body1loopBody1Catch1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 1572 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (allowErrors)
															#line 11623 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1573 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11627 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1574 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("Final wait in case error was injected by the last actor to finish\n");
															#line 11631 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(mutex.take());
															#line 1576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11637 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 2;
															#line 1576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1540Actor, 1, Void >*>(static_cast<FlowTestCase1540Actor*>(this)));
															#line 11642 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 1572 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (allowErrors)
															#line 11656 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1573 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11660 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1574 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
				printf("Final wait in case error was injected by the last actor to finish\n");
															#line 11664 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(mutex.take());
															#line 1576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11670 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 2;
															#line 1576 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1540Actor, 1, Void >*>(static_cast<FlowTestCase1540Actor*>(this)));
															#line 11675 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1540Actor*>(this)->ActorCallback< FlowTestCase1540Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1540Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1540Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1540Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1540Actor*>(this)->ActorCallback< FlowTestCase1540Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1540Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1540Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase1540Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont10(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1Catch1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1(int loopDepth) 
	{
															#line 1590 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (!(i < tests.size()))
															#line 11859 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1loopBody1Catch1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1591 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<ErrorOr<Void>> __when_expr_2 = errorOr(tests[i]);
															#line 1591 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 11867 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1Catch1loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 3;
															#line 1591 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >*>(static_cast<FlowTestCase1540Actor*>(this)));
															#line 11872 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1Catch1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont1(ErrorOr<Void> const& f,int loopDepth) 
	{
															#line 1592 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (verbose)
															#line 11894 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1593 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("  %d: %s\n", i, f.isError() ? f.getError().what() : "done");
															#line 11898 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1590 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		++i;
															#line 11902 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1Catch1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont1(ErrorOr<Void> && f,int loopDepth) 
	{
															#line 1592 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		if (verbose)
															#line 11911 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1593 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
			printf("  %d: %s\n", i, f.isError() ? f.getError().what() : "done");
															#line 11915 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1590 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
		++i;
															#line 11919 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1Catch1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1when1(ErrorOr<Void> const& f,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont1(f, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1when1(ErrorOr<Void> && f,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont1(std::move(f), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1540Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1540Actor*>(this)->ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >*,ErrorOr<Void> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >*,ErrorOr<Void> && value) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont3(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1541 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int count;
															#line 1544 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	bool verboseSetting;
															#line 1546 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int verboseTestIteration;
															#line 1549 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	bool verbose;
															#line 1556 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowMutex mutex;
															#line 1557 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	std::vector<Future<Void>> tests;
															#line 1559 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	bool allowErrors;
															#line 1564 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	Optional<Error> error;
															#line 1586 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	int i;
															#line 12020 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1540()
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1540Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1540Actor, 0, Void >, public ActorCallback< FlowTestCase1540Actor, 1, Void >, public ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >, public FastAllocated<FlowTestCase1540Actor>, public FlowTestCase1540ActorState<FlowTestCase1540Actor> {
															#line 12025 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1540Actor>::operator new;
	using FastAllocated<FlowTestCase1540Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1540Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase1540Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >;
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1540Actor(UnitTestParameters const& params) 
															#line 12038 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1540ActorState<FlowTestCase1540Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1540", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1540");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1540", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1540Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase1540Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase1540Actor, 2, ErrorOr<Void> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1540( UnitTestParameters const& params ) {
															#line 1540 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1540Actor(params));
															#line 12068 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1540, "/flow/flow/FlowMutex")

#line 1609 "/usr/src/libfdb_c/fdbrpc/FlowTests.actor.cpp"
