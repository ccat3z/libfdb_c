#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
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

#include "flow/Arena.h"
#include "flow/ProtocolVersion.h"
#include "flow/UnitTest.h"
#include "flow/DeterministicRandom.h"
#include "flow/IThreadPool.h"
#include "flow/WriteOnlySet.h"
#include "fdbrpc/fdbrpc.h"
#include "flow/IAsyncFile.h"
#include "flow/TLSConfig.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

void forceLinkFlowTests() {}

constexpr int firstLine = __LINE__;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase37()
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase37Actor>
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase37ActorState {
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase37ActorState(UnitTestParameters const& params) 
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase37", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase37ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase37", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase37ActorState();
		static_cast<FlowTestCase37Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__FILE__sr.endsWith("FlowTests.actor.cpp"_sr));
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase37Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase37ActorState(); static_cast<FlowTestCase37Actor*>(this)->destroy(); return 0; }
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase37Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase37ActorState();
		static_cast<FlowTestCase37Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(__LINE__ == firstLine + 4);
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>(Void());
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase37Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase37Actor*>(this)->actor_wait_state = 1;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase37Actor, 0, Void >*>(static_cast<FlowTestCase37Actor*>(this)));
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(__LINE__ == firstLine + 9);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = Future<Void>(Void());
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase37Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase37Actor*>(this)->actor_wait_state = 2;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase37Actor, 1, Void >*>(static_cast<FlowTestCase37Actor*>(this)));
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 6);
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		return a_body1loopBody1Catch1(success(), loopDepth);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 6);
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		return a_body1loopBody1Catch1(success(), loopDepth);
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"

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
		if (static_cast<FlowTestCase37Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase37Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase37Actor*>(this)->ActorCallback< FlowTestCase37Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase37Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase37Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase37Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 11);
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(__LINE__ == firstLine + 11);
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase37Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase37Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase37Actor*>(this)->ActorCallback< FlowTestCase37Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase37Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase37Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase37Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase37()
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase37Actor final : public Actor<Void>, public ActorCallback< FlowTestCase37Actor, 0, Void >, public ActorCallback< FlowTestCase37Actor, 1, Void >, public FastAllocated<FlowTestCase37Actor>, public FlowTestCase37ActorState<FlowTestCase37Actor> {
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase37Actor>::operator new;
	using FastAllocated<FlowTestCase37Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase37Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase37Actor, 1, Void >;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase37Actor(UnitTestParameters const& params) 
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase37ActorState<FlowTestCase37Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase37", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase37");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase37", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase37Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase37Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase37( UnitTestParameters const& params ) {
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase37Actor(params));
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase37, "/flow/actorcompiler/lineNumbers")

#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase55()
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase55Actor>
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase55ActorState {
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase55ActorState(UnitTestParameters const& params) 
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase55", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase55ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase55", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (FLOW_KNOBS->MAX_BUGGIFIED_DELAY == 0)
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (!static_cast<FlowTestCase55Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase55ActorState(); static_cast<FlowTestCase55Actor*>(this)->destroy(); return 0; }
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				new (&static_cast<FlowTestCase55Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase55ActorState();
				static_cast<FlowTestCase55Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			;
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase55ActorState();
		static_cast<FlowTestCase55Actor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		x = deterministicRandom()->random01();
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		last = 0;
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		f1 = map(delay(x), [last = &last](const Void&) { *last = 1; return Void(); });
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		f2 = map(delay(x), [last = &last](const Void&) { *last = 2; return Void(); });
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = f1 && f2;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase55Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase55Actor*>(this)->actor_wait_state = 1;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase55Actor, 0, Void >*>(static_cast<FlowTestCase55Actor*>(this)));
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (last == 1)
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			CODE_PROBE(true, "Delays can become ready out of order", probe::decoration::rare);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase55Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase55ActorState(); static_cast<FlowTestCase55Actor*>(this)->destroy(); return 0; }
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase55Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase55ActorState();
			static_cast<FlowTestCase55Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (last == 1)
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			CODE_PROBE(true, "Delays can become ready out of order", probe::decoration::rare);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase55Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase55ActorState(); static_cast<FlowTestCase55Actor*>(this)->destroy(); return 0; }
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase55Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase55ActorState();
			static_cast<FlowTestCase55Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase55Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase55Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase55Actor*>(this)->ActorCallback< FlowTestCase55Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase55Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase55", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase55", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase55Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase55", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase55", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase55Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase55", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase55", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	double x;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int last;
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f1;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f2;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase55()
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase55Actor final : public Actor<Void>, public ActorCallback< FlowTestCase55Actor, 0, Void >, public FastAllocated<FlowTestCase55Actor>, public FlowTestCase55ActorState<FlowTestCase55Actor> {
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase55Actor>::operator new;
	using FastAllocated<FlowTestCase55Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase55Actor, 0, Void >;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase55Actor(UnitTestParameters const& params) 
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase55ActorState<FlowTestCase55Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase55", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase55");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase55", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase55Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase55( UnitTestParameters const& params ) {
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase55Actor(params));
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase55, "/flow/buggifiedDelay")

#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

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

															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via emptyVoidActor()
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class EmptyVoidActorActor>
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class EmptyVoidActorActorState {
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	EmptyVoidActorActorState() 
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<EmptyVoidActorActor*>(this);
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
};
// This generated class is to be used only via emptyVoidActor()
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class EmptyVoidActorActor final : public Actor<void>, public FastAllocated<EmptyVoidActorActor>, public EmptyVoidActorActorState<EmptyVoidActorActor> {
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<EmptyVoidActorActor>::operator new;
	using FastAllocated<EmptyVoidActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	EmptyVoidActorActor() 
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static void emptyVoidActor(  ) {
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	new EmptyVoidActorActor();
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via emptyActor()
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class EmptyActorActor>
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class EmptyActorActorState {
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	EmptyActorActorState() 
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	{
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("emptyActor", reinterpret_cast<unsigned long>(this));

	}
	~EmptyActorActorState() 
	{
		fdb_probe_actor_destroy("emptyActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<EmptyActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EmptyActorActorState(); static_cast<EmptyActorActor*>(this)->destroy(); return 0; }
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class EmptyActorActor final : public Actor<Void>, public FastAllocated<EmptyActorActor>, public EmptyActorActorState<EmptyActorActor> {
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<EmptyActorActor>::operator new;
	using FastAllocated<EmptyActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	EmptyActorActor() 
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> emptyActor(  ) {
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new EmptyActorActor());
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via oneWaitVoidActor()
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class OneWaitVoidActorActor>
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class OneWaitVoidActorActorState {
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	OneWaitVoidActorActorState(Future<Void> const& f) 
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : f(f)
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			static_cast<OneWaitVoidActorActor*>(this)->actor_wait_state = 1;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OneWaitVoidActorActor, 0, Void >*>(static_cast<OneWaitVoidActorActor*>(this)));
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<OneWaitVoidActorActor*>(this);
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via oneWaitVoidActor()
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class OneWaitVoidActorActor final : public Actor<void>, public ActorCallback< OneWaitVoidActorActor, 0, Void >, public FastAllocated<OneWaitVoidActorActor>, public OneWaitVoidActorActorState<OneWaitVoidActorActor> {
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<OneWaitVoidActorActor>::operator new;
	using FastAllocated<OneWaitVoidActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< OneWaitVoidActorActor, 0, Void >;
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	OneWaitVoidActorActor(Future<Void> const& f) 
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static void oneWaitVoidActor( Future<Void> const& f ) {
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	new OneWaitVoidActorActor(f);
															#line 1055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via oneWaitActor()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class OneWaitActorActor>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class OneWaitActorActorState {
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	OneWaitActorActorState(Future<Void> const& f) 
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : f(f)
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<OneWaitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OneWaitActorActor*>(this)->actor_wait_state = 1;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OneWaitActorActor, 0, Void >*>(static_cast<OneWaitActorActor*>(this)));
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<OneWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OneWaitActorActorState(); static_cast<OneWaitActorActor*>(this)->destroy(); return 0; }
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<OneWaitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OneWaitActorActorState();
		static_cast<OneWaitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<OneWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OneWaitActorActorState(); static_cast<OneWaitActorActor*>(this)->destroy(); return 0; }
															#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via oneWaitActor()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class OneWaitActorActor final : public Actor<Void>, public ActorCallback< OneWaitActorActor, 0, Void >, public FastAllocated<OneWaitActorActor>, public OneWaitActorActorState<OneWaitActorActor> {
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<OneWaitActorActor>::operator new;
	using FastAllocated<OneWaitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OneWaitActorActor, 0, Void >;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	OneWaitActorActor(Future<Void> const& f) 
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> oneWaitActor( Future<Void> const& f ) {
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new OneWaitActorActor(f));
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

Future<Void> g_cheese;
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via cheeseWaitActor()
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class CheeseWaitActorActor>
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class CheeseWaitActorActorState {
															#line 1261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	CheeseWaitActorActorState() 
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	{
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("cheeseWaitActor", reinterpret_cast<unsigned long>(this));

	}
	~CheeseWaitActorActorState() 
	{
		fdb_probe_actor_destroy("cheeseWaitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_cheese;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<CheeseWaitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheeseWaitActorActor*>(this)->actor_wait_state = 1;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheeseWaitActorActor, 0, Void >*>(static_cast<CheeseWaitActorActor*>(this)));
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<CheeseWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheeseWaitActorActorState(); static_cast<CheeseWaitActorActor*>(this)->destroy(); return 0; }
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<CheeseWaitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheeseWaitActorActorState();
		static_cast<CheeseWaitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<CheeseWaitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheeseWaitActorActorState(); static_cast<CheeseWaitActorActor*>(this)->destroy(); return 0; }
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class CheeseWaitActorActor final : public Actor<Void>, public ActorCallback< CheeseWaitActorActor, 0, Void >, public FastAllocated<CheeseWaitActorActor>, public CheeseWaitActorActorState<CheeseWaitActorActor> {
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<CheeseWaitActorActor>::operator new;
	using FastAllocated<CheeseWaitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheeseWaitActorActor, 0, Void >;
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	CheeseWaitActorActor() 
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> cheeseWaitActor(  ) {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new CheeseWaitActorActor());
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

size_t cheeseWaitActorSize() {
#ifndef OPEN_FOR_IDE
	return sizeof(CheeseWaitActorActor);
#else
	return 0ul;
#endif
}

															#line 1450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via trivialVoidActor()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class TrivialVoidActorActor>
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TrivialVoidActorActorState {
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TrivialVoidActorActorState(int* const& result) 
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : result(result)
															#line 1464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			*result = 1;
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<TrivialVoidActorActor*>(this);
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int* result;
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via trivialVoidActor()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TrivialVoidActorActor final : public Actor<void>, public FastAllocated<TrivialVoidActorActor>, public TrivialVoidActorActorState<TrivialVoidActorActor> {
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TrivialVoidActorActor>::operator new;
	using FastAllocated<TrivialVoidActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TrivialVoidActorActor(int* const& result) 
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static void trivialVoidActor( int* const& result ) {
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	new TrivialVoidActorActor(result);
															#line 1542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 1547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via return42Actor()
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class Return42ActorActor>
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Return42ActorActorState {
															#line 1554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Return42ActorActorState() 
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	{
															#line 1560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("return42Actor", reinterpret_cast<unsigned long>(this));

	}
	~Return42ActorActorState() 
	{
		fdb_probe_actor_destroy("return42Actor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<Return42ActorActor*>(this)->SAV<int>::futures) { (void)(42); this->~Return42ActorActorState(); static_cast<Return42ActorActor*>(this)->destroy(); return 0; }
															#line 1574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Return42ActorActor final : public Actor<int>, public FastAllocated<Return42ActorActor>, public Return42ActorActorState<Return42ActorActor> {
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Return42ActorActor>::operator new;
	using FastAllocated<Return42ActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Return42ActorActor() 
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> return42Actor(  ) {
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Return42ActorActor());
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via voidWaitActor()
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class VoidWaitActorActor>
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class VoidWaitActorActorState {
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	VoidWaitActorActorState(Future<int> const& in,int* const& result) 
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : in(in),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   result(result)
															#line 1658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<int> __when_expr_0 = in;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			static_cast<VoidWaitActorActor*>(this)->actor_wait_state = 1;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< VoidWaitActorActor, 0, int >*>(static_cast<VoidWaitActorActor*>(this)));
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		*result = i;
															#line 1701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		*result = i;
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		delete static_cast<VoidWaitActorActor*>(this);
															#line 1782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<int> in;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int* result;
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via voidWaitActor()
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class VoidWaitActorActor final : public Actor<void>, public ActorCallback< VoidWaitActorActor, 0, int >, public FastAllocated<VoidWaitActorActor>, public VoidWaitActorActorState<VoidWaitActorActor> {
															#line 1796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<VoidWaitActorActor>::operator new;
	using FastAllocated<VoidWaitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< VoidWaitActorActor, 0, int >;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	VoidWaitActorActor(Future<int> const& in,int* const& result) 
															#line 1807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static void voidWaitActor( Future<int> const& in, int* const& result ) {
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	new VoidWaitActorActor(in, result);
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via addOneActor()
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class AddOneActorActor>
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class AddOneActorActorState {
															#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	AddOneActorActorState(Future<int> const& in) 
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : in(in)
															#line 1845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<int> __when_expr_0 = in;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<AddOneActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AddOneActorActor*>(this)->actor_wait_state = 1;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddOneActorActor, 0, int >*>(static_cast<AddOneActorActor*>(this)));
															#line 1867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<AddOneActorActor*>(this)->SAV<int>::futures) { (void)(i + 1); this->~AddOneActorActorState(); static_cast<AddOneActorActor*>(this)->destroy(); return 0; }
															#line 1890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<AddOneActorActor*>(this)->SAV< int >::value()) int(i + 1);
		this->~AddOneActorActorState();
		static_cast<AddOneActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<AddOneActorActor*>(this)->SAV<int>::futures) { (void)(i + 1); this->~AddOneActorActorState(); static_cast<AddOneActorActor*>(this)->destroy(); return 0; }
															#line 1902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<int> in;
															#line 1975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via addOneActor()
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class AddOneActorActor final : public Actor<int>, public ActorCallback< AddOneActorActor, 0, int >, public FastAllocated<AddOneActorActor>, public AddOneActorActorState<AddOneActorActor> {
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<AddOneActorActor>::operator new;
	using FastAllocated<AddOneActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddOneActorActor, 0, int >;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	AddOneActorActor(Future<int> const& in) 
															#line 1991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> addOneActor( Future<int> const& in ) {
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new AddOneActorActor(in));
															#line 2019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 2024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via chooseTwoActor()
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class ChooseTwoActorActor>
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class ChooseTwoActorActorState {
															#line 2031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	ChooseTwoActorActorState(Future<Void> const& f,Future<Void> const& g) 
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : f(f),
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   g(g)
															#line 2040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<ChooseTwoActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = g;
															#line 2061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ChooseTwoActorActor*>(this)->actor_wait_state = 1;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChooseTwoActorActor, 0, Void >*>(static_cast<ChooseTwoActorActor*>(this)));
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChooseTwoActorActor, 1, Void >*>(static_cast<ChooseTwoActorActor*>(this)));
															#line 2068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ChooseTwoActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChooseTwoActorActorState(); static_cast<ChooseTwoActorActor*>(this)->destroy(); return 0; }
															#line 2091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> g;
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via chooseTwoActor()
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class ChooseTwoActorActor final : public Actor<Void>, public ActorCallback< ChooseTwoActorActor, 0, Void >, public ActorCallback< ChooseTwoActorActor, 1, Void >, public FastAllocated<ChooseTwoActorActor>, public ChooseTwoActorActorState<ChooseTwoActorActor> {
															#line 2229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<ChooseTwoActorActor>::operator new;
	using FastAllocated<ChooseTwoActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChooseTwoActorActor, 0, Void >;
friend struct ActorCallback< ChooseTwoActorActor, 1, Void >;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	ChooseTwoActorActor(Future<Void> const& f,Future<Void> const& g) 
															#line 2241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> chooseTwoActor( Future<Void> const& f, Future<Void> const& g ) {
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new ChooseTwoActorActor(f, g));
															#line 2269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via consumeOneActor()
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class ConsumeOneActorActor>
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class ConsumeOneActorActorState {
															#line 2281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	ConsumeOneActorActorState(FutureStream<int> const& in) 
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : in(in)
															#line 2288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			FutureStream<int> __when_expr_0 = in;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<ConsumeOneActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<ConsumeOneActorActor*>(this)->actor_wait_state = 1;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ConsumeOneActorActor, 0, int >*>(static_cast<ConsumeOneActorActor*>(this)));
															#line 2310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ConsumeOneActorActor*>(this)->SAV<int>::futures) { (void)(i); this->~ConsumeOneActorActorState(); static_cast<ConsumeOneActorActor*>(this)->destroy(); return 0; }
															#line 2333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ConsumeOneActorActor*>(this)->SAV< int >::value()) int(i);
		this->~ConsumeOneActorActorState();
		static_cast<ConsumeOneActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ConsumeOneActorActor*>(this)->SAV<int>::futures) { (void)(i); this->~ConsumeOneActorActorState(); static_cast<ConsumeOneActorActor*>(this)->destroy(); return 0; }
															#line 2345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FutureStream<int> in;
															#line 2418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via consumeOneActor()
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class ConsumeOneActorActor final : public Actor<int>, public ActorSingleCallback< ConsumeOneActorActor, 0, int >, public FastAllocated<ConsumeOneActorActor>, public ConsumeOneActorActorState<ConsumeOneActorActor> {
															#line 2423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<ConsumeOneActorActor>::operator new;
	using FastAllocated<ConsumeOneActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ConsumeOneActorActor, 0, int >;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	ConsumeOneActorActor(FutureStream<int> const& in) 
															#line 2434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> consumeOneActor( FutureStream<int> const& in ) {
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new ConsumeOneActorActor(in));
															#line 2462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 2467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via sumActor()
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class SumActorActor>
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class SumActorActorState {
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	SumActorActorState(FutureStream<int> const& in) 
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : in(in),
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   total(0)
															#line 2483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				;
															#line 2499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<SumActorActor*>(this)->SAV<int>::futures) { (void)(total); this->~SumActorActorState(); static_cast<SumActorActor*>(this)->destroy(); return 0; }
															#line 2528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<SumActorActor*>(this)->SAV< int >::value()) int(std::move(total)); // state_var_RVO
		this->~SumActorActorState();
		static_cast<SumActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (e.code() != error_code_end_of_stream)
															#line 2541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		FutureStream<int> __when_expr_0 = in;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<SumActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<SumActorActor*>(this)->actor_wait_state = 1;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< SumActorActor, 0, int >*>(static_cast<SumActorActor*>(this)));
															#line 2575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int const& i,int loopDepth) 
	{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		total += i;
															#line 2584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int && i,int loopDepth) 
	{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		total += i;
															#line 2593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FutureStream<int> in;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int total;
															#line 2665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via sumActor()
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class SumActorActor final : public Actor<int>, public ActorSingleCallback< SumActorActor, 0, int >, public FastAllocated<SumActorActor>, public SumActorActorState<SumActorActor> {
															#line 2670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<SumActorActor>::operator new;
	using FastAllocated<SumActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< SumActorActor, 0, int >;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	SumActorActor(FutureStream<int> const& in) 
															#line 2681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<int> sumActor( FutureStream<int> const& in ) {
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new SumActorActor(in));
															#line 2709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via templateActor()
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class T, class TemplateActorActor>
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TemplateActorActorState {
															#line 2721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TemplateActorActorState(T const& t) 
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : t(t)
															#line 2728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<TemplateActorActor*>(this)->SAV<T>::futures) { (void)(t); this->~TemplateActorActorState(); static_cast<TemplateActorActor*>(this)->destroy(); return 0; }
															#line 2743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	T t;
															#line 2767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via templateActor()
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class T>
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TemplateActorActor final : public Actor<T>, public FastAllocated<TemplateActorActor<T>>, public TemplateActorActorState<T, TemplateActorActor<T>> {
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TemplateActorActor<T>>::operator new;
	using FastAllocated<TemplateActorActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TemplateActorActor(T const& t) 
															#line 2784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class T>
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<T> templateActor( T const& t ) {
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<T>(new TemplateActorActor<T>(t));
															#line 2813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

static int destroy() {
	return 666;
}
															#line 2821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via testHygeine()
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class TestHygeineActor>
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TestHygeineActorState {
															#line 2828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TestHygeineActorState() 
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	{
															#line 2834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("testHygeine", reinterpret_cast<unsigned long>(this));

	}
	~TestHygeineActorState() 
	{
		fdb_probe_actor_destroy("testHygeine", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(destroy() == 666);
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<TestHygeineActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestHygeineActorState(); static_cast<TestHygeineActor*>(this)->destroy(); return 0; }
															#line 2850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TestHygeineActor final : public Actor<Void>, public FastAllocated<TestHygeineActor>, public TestHygeineActorState<TestHygeineActor> {
															#line 2876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TestHygeineActor>::operator new;
	using FastAllocated<TestHygeineActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TestHygeineActor() 
															#line 2886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> testHygeine(  ) {
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new TestHygeineActor());
															#line 2913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

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
															#line 3008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via testNonserializableThing()
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class TestNonserializableThingActor>
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TestNonserializableThingActorState {
															#line 3015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TestNonserializableThingActorState() 
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	{
															#line 3021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		fdb_probe_actor_create("testNonserializableThing", reinterpret_cast<unsigned long>(this));

	}
	~TestNonserializableThingActorState() 
	{
		fdb_probe_actor_destroy("testNonserializableThing", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<TestNonserializableThingActor*>(this)->SAV<NonserializableThing>::futures) { (void)(NonserializableThing()); this->~TestNonserializableThingActorState(); static_cast<TestNonserializableThingActor*>(this)->destroy(); return 0; }
															#line 3035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TestNonserializableThingActor final : public Actor<NonserializableThing>, public FastAllocated<TestNonserializableThingActor>, public TestNonserializableThingActorState<TestNonserializableThingActor> {
															#line 3061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TestNonserializableThingActor>::operator new;
	using FastAllocated<TestNonserializableThingActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<NonserializableThing>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TestNonserializableThingActor() 
															#line 3071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<NonserializableThing> testNonserializableThing(  ) {
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<NonserializableThing>(new TestNonserializableThingActor());
															#line 3098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 3103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via testCancelled()
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class TestCancelledActor>
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TestCancelledActorState {
															#line 3110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TestCancelledActorState(bool* const& exits,Future<Void> const& f) 
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : exits(exits),
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   f(f)
															#line 3119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Future<Void>(Never());
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<TestCancelledActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TestCancelledActor*>(this)->actor_wait_state = 1;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestCancelledActor, 0, Void >*>(static_cast<TestCancelledActor*>(this)));
															#line 3142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<TestCancelledActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestCancelledActorState(); static_cast<TestCancelledActor*>(this)->destroy(); return 0; }
															#line 3171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<TestCancelledActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestCancelledActorState();
		static_cast<TestCancelledActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			err = e;
															#line 3184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			try {
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_1 = Future<Void>(Never());
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<TestCancelledActor*>(this)->actor_wait_state < 0) return a_body1Catch2Catch1(actor_cancelled(), loopDepth);
															#line 3190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2Catch1(__when_expr_1.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_1.get(), loopDepth); };
				static_cast<TestCancelledActor*>(this)->actor_wait_state = 2;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestCancelledActor, 1, Void >*>(static_cast<TestCancelledActor*>(this)));
															#line 3195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 3304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"

		return loopDepth;
	}
	int a_body1Catch2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			*exits = true;
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 3315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	bool* exits;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> f;
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Error err;
															#line 3419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via testCancelled()
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class TestCancelledActor final : public Actor<Void>, public ActorCallback< TestCancelledActor, 0, Void >, public ActorCallback< TestCancelledActor, 1, Void >, public FastAllocated<TestCancelledActor>, public TestCancelledActorState<TestCancelledActor> {
															#line 3424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<TestCancelledActor>::operator new;
	using FastAllocated<TestCancelledActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestCancelledActor, 0, Void >;
friend struct ActorCallback< TestCancelledActor, 1, Void >;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	TestCancelledActor(bool* const& exits,Future<Void> const& f) 
															#line 3436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<Void> testCancelled( bool* const& exits, Future<Void> const& f ) {
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new TestCancelledActor(exits, f));
															#line 3465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 3470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase323()
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase323Actor>
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase323ActorState {
															#line 3477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase323ActorState(UnitTestParameters const& params) 
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 3484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase323", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase323ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase323", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			bool exits = false;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> p;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> test = testCancelled(&exits, p.getFuture());
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getPromiseReferenceCount() == 1 && p.getFutureReferenceCount() == 1);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			test.cancel();
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(exits);
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(test.getPromiseReferenceCount() == 0 && test.getFutureReferenceCount() == 1 && test.isReady() && test.isError() && test.getError().code() == error_code_actor_cancelled);
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getPromiseReferenceCount() == 1 && p.getFutureReferenceCount() == 0);
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase323Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase323ActorState(); static_cast<FlowTestCase323Actor*>(this)->destroy(); return 0; }
															#line 3515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase323Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase323ActorState();
			static_cast<FlowTestCase323Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase323ActorState();
		static_cast<FlowTestCase323Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 3539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase323()
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase323Actor final : public Actor<Void>, public FastAllocated<FlowTestCase323Actor>, public FlowTestCase323ActorState<FlowTestCase323Actor> {
															#line 3544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase323Actor>::operator new;
	using FastAllocated<FlowTestCase323Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase323Actor(UnitTestParameters const& params) 
															#line 3554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase323ActorState<FlowTestCase323Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase323", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase323");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase323", reinterpret_cast<unsigned long>(this), -1);

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
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase323( UnitTestParameters const& params ) {
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase323Actor(params));
															#line 3581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase323, "/flow/flow/cancel1")

#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 3587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via noteCancel()
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class NoteCancelActor>
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class NoteCancelActorState {
															#line 3594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	NoteCancelActorState(int* const& cancelled) 
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : cancelled(cancelled)
															#line 3601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			*cancelled = 0;
															#line 3616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			try {
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Future<Void>(Never());
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<NoteCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<NoteCancelActor*>(this)->actor_wait_state = 1;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< NoteCancelActor, 0, Void >*>(static_cast<NoteCancelActor*>(this)));
															#line 3627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("Cancelled!\n");
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			*cancelled = 1;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 3661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		return a_body1Catch2(internal_error(), loopDepth);
															#line 3675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		return a_body1Catch2(internal_error(), loopDepth);
															#line 3683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"

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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int* cancelled;
															#line 3752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via noteCancel()
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class NoteCancelActor final : public Actor<Void>, public ActorCallback< NoteCancelActor, 0, Void >, public FastAllocated<NoteCancelActor>, public NoteCancelActorState<NoteCancelActor> {
															#line 3757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<NoteCancelActor>::operator new;
	using FastAllocated<NoteCancelActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< NoteCancelActor, 0, Void >;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	NoteCancelActor(int* const& cancelled) 
															#line 3768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> noteCancel( int* const& cancelled ) {
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new NoteCancelActor(cancelled));
															#line 3796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 3801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase349()
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase349Actor>
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase349ActorState {
															#line 3808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase349ActorState(UnitTestParameters const& params) 
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 3815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase349", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase349ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase349", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int c1 = 0, c2 = 0, c3 = 0;
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> cf = noteCancel(&c1);
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c1 == 0);
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			cf = Future<Void>();
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c1 == 1);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			cf = noteCancel(&c2) && noteCancel(&c3);
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c2 == 0 && c3 == 0);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			cf = Future<Void>();
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(c2 == 1 && c3 == 1);
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase349Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase349ActorState(); static_cast<FlowTestCase349Actor*>(this)->destroy(); return 0; }
															#line 3848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase349Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase349ActorState();
			static_cast<FlowTestCase349Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase349ActorState();
		static_cast<FlowTestCase349Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 3872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase349()
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase349Actor final : public Actor<Void>, public FastAllocated<FlowTestCase349Actor>, public FlowTestCase349ActorState<FlowTestCase349Actor> {
															#line 3877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase349Actor>::operator new;
	using FastAllocated<FlowTestCase349Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase349Actor(UnitTestParameters const& params) 
															#line 3887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase349ActorState<FlowTestCase349Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase349", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase349");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase349", reinterpret_cast<unsigned long>(this), -1);

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
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase349( UnitTestParameters const& params ) {
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase349Actor(params));
															#line 3914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase349, "/flow/flow/cancel2")

#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

namespace flow_tests_details {
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
} // namespace flow_tests_details

															#line 3934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase378()
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase378Actor>
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase378ActorState {
															#line 3941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase378ActorState(UnitTestParameters const& params) 
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 3948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase378", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase378ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase378", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Future<NonserializableThing> f = testNonserializableThing();
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Promise<NonserializableThing> p;
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				p.send(NonserializableThing());
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				f = p.getFuture();
															#line 3970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ReplyPromise<flow_tests_details::Int> rpInt;
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Future<flow_tests_details::Int> f = rpInt.getFuture();
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(!f.isReady());
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				rpInt.send(123);
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(f.get().value == 123);
															#line 3983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				RequestStream<flow_tests_details::Int> rsInt;
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				FutureStream<flow_tests_details::Int> f = rsInt.getFuture();
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				rsInt.send(1);
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				rsInt.send(2);
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(f.pop().value == 1);
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(f.pop().value == 2);
															#line 3998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase378Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase378ActorState(); static_cast<FlowTestCase378Actor*>(this)->destroy(); return 0; }
															#line 4002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase378Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase378ActorState();
			static_cast<FlowTestCase378Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase378ActorState();
		static_cast<FlowTestCase378Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase378()
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase378Actor final : public Actor<Void>, public FastAllocated<FlowTestCase378Actor>, public FlowTestCase378ActorState<FlowTestCase378Actor> {
															#line 4031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase378Actor>::operator new;
	using FastAllocated<FlowTestCase378Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase378Actor(UnitTestParameters const& params) 
															#line 4041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase378ActorState<FlowTestCase378Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase378", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase378");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase378", reinterpret_cast<unsigned long>(this), -1);

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
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase378( UnitTestParameters const& params ) {
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase378Actor(params));
															#line 4068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase378, "/flow/flow/nonserializable futures")

#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase411()
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase411Actor>
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase411ActorState {
															#line 4081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase411ActorState(UnitTestParameters const& params) 
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase411", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase411ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase411", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				RequestStream<flow_tests_details::Int> locInt;
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				BinaryWriter wr(IncludeVersion());
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				wr << locInt;
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(locInt.getEndpoint().isValid() && locInt.getEndpoint().isLocal() && locInt.getEndpoint().getPrimaryAddress() == FlowTransport::transport().getLocalAddress());
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				BinaryReader rd(wr.toValue(), IncludeVersion());
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				RequestStream<flow_tests_details::Int> remoteInt;
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				rd >> remoteInt;
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(remoteInt.getEndpoint() == locInt.getEndpoint());
															#line 4118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (0)
															#line 4122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ReplyPromise<flow_tests_details::Int> locInt;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				BinaryWriter wr(IncludeVersion());
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				wr << locInt;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(locInt.getEndpoint().isValid() && locInt.getEndpoint().isLocal());
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				BinaryReader rd(wr.toValue(), IncludeVersion());
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ReplyPromise<flow_tests_details::Int> remoteInt;
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				rd >> remoteInt;
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(remoteInt.getEndpoint() == locInt.getEndpoint());
															#line 4140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase411Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase411ActorState(); static_cast<FlowTestCase411Actor*>(this)->destroy(); return 0; }
															#line 4144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase411Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase411ActorState();
			static_cast<FlowTestCase411Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase411ActorState();
		static_cast<FlowTestCase411Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase411()
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase411Actor final : public Actor<Void>, public FastAllocated<FlowTestCase411Actor>, public FlowTestCase411ActorState<FlowTestCase411Actor> {
															#line 4173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase411Actor>::operator new;
	using FastAllocated<FlowTestCase411Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase411Actor(UnitTestParameters const& params) 
															#line 4183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase411ActorState<FlowTestCase411Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase411", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase411");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase411", reinterpret_cast<unsigned long>(this), -1);

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
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase411( UnitTestParameters const& params ) {
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase411Actor(params));
															#line 4210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase411, "/flow/flow/networked futures")

#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase447()
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase447Actor>
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase447ActorState {
															#line 4223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase447ActorState(UnitTestParameters const& params) 
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase447", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase447ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase447", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			std::vector<Promise<int>> ps(5);
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<int>> fs;
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<Void>> qs;
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for( auto& p : ps ) {
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				fs.push_back(p.getFuture());
															#line 4253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i <= ps.size();i++) {
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				qs.push_back(quorum(fs, i));
															#line 4259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < ps.size();i++) {
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(qs[i].isReady());
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(!qs[i + 1].isReady());
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ps[i].send(i);
															#line 4269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(qs[ps.size()].isReady());
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase447Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase447ActorState(); static_cast<FlowTestCase447Actor*>(this)->destroy(); return 0; }
															#line 4275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase447Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase447ActorState();
			static_cast<FlowTestCase447Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase447ActorState();
		static_cast<FlowTestCase447Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase447()
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase447Actor final : public Actor<Void>, public FastAllocated<FlowTestCase447Actor>, public FlowTestCase447ActorState<FlowTestCase447Actor> {
															#line 4304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase447Actor>::operator new;
	using FastAllocated<FlowTestCase447Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase447Actor(UnitTestParameters const& params) 
															#line 4314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase447ActorState<FlowTestCase447Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase447", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase447");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase447", reinterpret_cast<unsigned long>(this), -1);

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
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase447( UnitTestParameters const& params ) {
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase447Actor(params));
															#line 4341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase447, "/flow/flow/quorum")

#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase466()
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase466Actor>
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase466ActorState {
															#line 4354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase466ActorState(UnitTestParameters const& params) 
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase466", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase466ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase466", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<int> invalid;
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!invalid.isValid());
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<int> never = Never();
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(never.isValid() && !never.isReady());
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<int> one = 1;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(one.isValid() && one.isReady() && !one.isError());
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(one.get() == 1);
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(one.getFutureReferenceCount() == 1);
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase466Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase466ActorState(); static_cast<FlowTestCase466Actor*>(this)->destroy(); return 0; }
															#line 4392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase466Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase466ActorState();
			static_cast<FlowTestCase466Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase466ActorState();
		static_cast<FlowTestCase466Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase466()
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase466Actor final : public Actor<Void>, public FastAllocated<FlowTestCase466Actor>, public FlowTestCase466ActorState<FlowTestCase466Actor> {
															#line 4421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase466Actor>::operator new;
	using FastAllocated<FlowTestCase466Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase466Actor(UnitTestParameters const& params) 
															#line 4431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase466ActorState<FlowTestCase466Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase466", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase466");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase466", reinterpret_cast<unsigned long>(this), -1);

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
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase466( UnitTestParameters const& params ) {
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase466Actor(params));
															#line 4458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase466, "/flow/flow/trivial futures")

#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase480()
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase480Actor>
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase480ActorState {
															#line 4471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase480ActorState(UnitTestParameters const& params) 
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase480", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase480ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase480", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<int> f;
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p;
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid());
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!p.isSet());
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(1);
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isSet());
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getFuture().get() == 1);
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p2;
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p2.getFuture();
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && !f.isReady());
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p2.send(2);
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && !f.isError());
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 2);
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p3;
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p3.getFuture();
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p3.sendError(end_of_stream());
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && f.isError());
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_end_of_stream);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p4;
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p4.getFuture();
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p4 = Promise<int>();
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p4.isValid() && !p4.isSet());
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && f.isError());
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_broken_promise);
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase480Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase480ActorState(); static_cast<FlowTestCase480Actor*>(this)->destroy(); return 0; }
															#line 4541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase480Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase480ActorState();
			static_cast<FlowTestCase480Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase480ActorState();
		static_cast<FlowTestCase480Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase480()
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase480Actor final : public Actor<Void>, public FastAllocated<FlowTestCase480Actor>, public FlowTestCase480ActorState<FlowTestCase480Actor> {
															#line 4570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase480Actor>::operator new;
	using FastAllocated<FlowTestCase480Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase480Actor(UnitTestParameters const& params) 
															#line 4580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase480ActorState<FlowTestCase480Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase480", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase480");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase480", reinterpret_cast<unsigned long>(this), -1);

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
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase480( UnitTestParameters const& params ) {
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase480Actor(params));
															#line 4607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase480, "/flow/flow/trivial promises")

#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase512()
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase512Actor>
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase512ActorState {
															#line 4620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase512ActorState(UnitTestParameters const& params) 
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase512", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase512ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase512", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			FutureStream<int> f;
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(1);
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getFuture().isReady());
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.getFuture().pop() == 1);
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p2;
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p2.getFuture();
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && !f.isReady());
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p2.send(2);
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p2.send(3);
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && !f.isError());
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.pop() == 2);
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.pop() == 3);
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p3;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p3.getFuture();
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p3.send(4);
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p3.sendError(end_of_stream());
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && !f.isError());
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.pop() == 4);
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isError());
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_end_of_stream);
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p4;
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p4.getFuture();
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p4 = PromiseStream<int>();
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isValid() && f.isReady() && f.isError());
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.getError().code() == error_code_broken_promise);
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase512Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase512ActorState(); static_cast<FlowTestCase512Actor*>(this)->destroy(); return 0; }
															#line 4694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase512Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase512ActorState();
			static_cast<FlowTestCase512Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase512ActorState();
		static_cast<FlowTestCase512Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase512()
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase512Actor final : public Actor<Void>, public FastAllocated<FlowTestCase512Actor>, public FlowTestCase512ActorState<FlowTestCase512Actor> {
															#line 4723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase512Actor>::operator new;
	using FastAllocated<FlowTestCase512Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase512Actor(UnitTestParameters const& params) 
															#line 4733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase512ActorState<FlowTestCase512Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase512", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase512");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase512", reinterpret_cast<unsigned long>(this), -1);

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
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase512( UnitTestParameters const& params ) {
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase512Actor(params));
															#line 4760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase512, "/flow/flow/trivial promisestreams")

#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase546()
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase546Actor>
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase546ActorState {
															#line 4773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase546ActorState(UnitTestParameters const& params) 
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase546", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase546ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase546", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p;
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<int> f = p.getFuture();
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int result = 0;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			bool happened = false;
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( std::move(f), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&happened](int) { happened = true; }, [&happened](Error) { happened = true; });
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isValid());
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && !p.isSet() && p.getFutureReferenceCount() == 1);
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0 && !happened);
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(123);
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 123 && happened);
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && p.isSet() && p.getFutureReferenceCount() == 0 && p.getFuture().get() == 123);
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 123);
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && p.isSet() && p.getFutureReferenceCount() == 0 && p.getFuture().get() == 123);
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p = Promise<int>();
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p.getFuture();
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( std::move(f), [&result](int x) { result = x; }, [&result](Error e) { result = -e.code(); });
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isValid());
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(p.isValid() && !p.isSet() && p.getFutureReferenceCount() == 1);
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0);
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p = Promise<int>();
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == -error_code_broken_promise);
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase546Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase546ActorState(); static_cast<FlowTestCase546Actor*>(this)->destroy(); return 0; }
															#line 4845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase546Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase546ActorState();
			static_cast<FlowTestCase546Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase546ActorState();
		static_cast<FlowTestCase546Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 4869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase546()
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase546Actor final : public Actor<Void>, public FastAllocated<FlowTestCase546Actor>, public FlowTestCase546ActorState<FlowTestCase546Actor> {
															#line 4874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase546Actor>::operator new;
	using FastAllocated<FlowTestCase546Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase546Actor(UnitTestParameters const& params) 
															#line 4884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase546ActorState<FlowTestCase546Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase546", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase546");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase546", reinterpret_cast<unsigned long>(this), -1);

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
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase546( UnitTestParameters const& params ) {
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase546Actor(params));
															#line 4911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase546, "/flow/flow/callbacks")

#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 4917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase584()
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase584Actor>
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase584ActorState {
															#line 4924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase584ActorState(UnitTestParameters const& params) 
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 4931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase584", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase584ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase584", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> p;
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int result = 0;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0);
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(123);
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(456);
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 123);
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 456);
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			onReady( p.getFuture(), [&result](int x) { result = x; }, [&result](Error e) { result = -1; });
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 0);
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p = PromiseStream<int>();
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == -1);
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase584Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase584ActorState(); static_cast<FlowTestCase584Actor*>(this)->destroy(); return 0; }
															#line 4976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase584Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase584ActorState();
			static_cast<FlowTestCase584Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase584ActorState();
		static_cast<FlowTestCase584Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase584()
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase584Actor final : public Actor<Void>, public FastAllocated<FlowTestCase584Actor>, public FlowTestCase584ActorState<FlowTestCase584Actor> {
															#line 5005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase584Actor>::operator new;
	using FastAllocated<FlowTestCase584Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase584Actor(UnitTestParameters const& params) 
															#line 5015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase584ActorState<FlowTestCase584Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase584", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase584");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase584", reinterpret_cast<unsigned long>(this), -1);

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
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase584( UnitTestParameters const& params ) {
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase584Actor(params));
															#line 5042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase584, "/flow/flow/promisestream callbacks")

#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

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

															#line 5070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase639()
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase639Actor>
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase639ActorState {
															#line 5077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase639ActorState(UnitTestParameters const& params) 
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase639", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase639ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase639", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int result = 0;
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			trivialVoidActor(&result);
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 1);
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<int> f = return42Actor();
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && !f.isError() && f.get() == 42 && f.getFutureReferenceCount() == 1 && f.getPromiseReferenceCount() == 0);
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(1));
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = Future<int>();
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = templateActor(24);
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && !f.isError() && f.get() == 24 && f.getFutureReferenceCount() == 1 && f.getPromiseReferenceCount() == 0);
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(1));
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = Future<int>();
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			result = 0;
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			voidWaitActor(2, &result);
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(result == 2 && expectActorCount(0));
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<int> p;
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = addOneActor(p.getFuture());
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isReady() && expectActorCount(1));
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(100);
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.isReady() && f.get() == 101);
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(1));
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = Future<int>();
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<int> ps;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = consumeOneActor(ps.getFuture());
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!f.isReady() && expectActorCount(1));
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ps.send(101);
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 101 && ps.isEmpty());
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ps.send(102);
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!ps.isEmpty());
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = consumeOneActor(ps.getFuture());
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 102 && ps.isEmpty());
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = sumActor(ps.getFuture());
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ps.send(1);
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ps.send(10);
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ps.send(100);
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ps.sendError(end_of_stream());
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(f.get() == 111);
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(testHygeine().isReady());
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase639Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase639ActorState(); static_cast<FlowTestCase639Actor*>(this)->destroy(); return 0; }
															#line 5183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase639Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase639ActorState();
			static_cast<FlowTestCase639Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase639ActorState();
		static_cast<FlowTestCase639Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase639()
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase639Actor final : public Actor<Void>, public FastAllocated<FlowTestCase639Actor>, public FlowTestCase639ActorState<FlowTestCase639Actor> {
															#line 5212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase639Actor>::operator new;
	using FastAllocated<FlowTestCase639Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase639Actor(UnitTestParameters const& params) 
															#line 5222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase639ActorState<FlowTestCase639Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase639", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase639");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase639", reinterpret_cast<unsigned long>(this), -1);

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
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase639( UnitTestParameters const& params ) {
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase639Actor(params));
															#line 5249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase639, "/flow/flow/trivial actors")

#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 5255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase695()
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase695Actor>
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase695ActorState {
															#line 5262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase695ActorState(UnitTestParameters const& params) 
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase695", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase695ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase695", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			auto yn = makeReference<YieldMockNetwork>();
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			yn->nextYield = 0;
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> p;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> u = p.getFuture();
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> i = success(u);
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<Void>> v;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 5;i++) {
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				v.push_back(yieldedFuture(u));
															#line 5298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			auto numReady = [&v]() { return std::count_if(v.begin(), v.end(), [](Future<Void> v) { return v.isReady(); }); };
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(numReady() == 0);
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(Void());
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(u.isReady() && i.isReady() && numReady() == 0);
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 5;i++) {
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				yn->tick();
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(numReady() == i + 1);
															#line 5314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 5;i++) {
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(v[i].getPromiseReferenceCount() == 0 && v[i].getFutureReferenceCount() == 1);
															#line 5320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase695Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase695ActorState(); static_cast<FlowTestCase695Actor*>(this)->destroy(); return 0; }
															#line 5324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase695Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase695ActorState();
			static_cast<FlowTestCase695Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase695ActorState();
		static_cast<FlowTestCase695Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase695()
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase695Actor final : public Actor<Void>, public FastAllocated<FlowTestCase695Actor>, public FlowTestCase695ActorState<FlowTestCase695Actor> {
															#line 5353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase695Actor>::operator new;
	using FastAllocated<FlowTestCase695Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase695Actor(UnitTestParameters const& params) 
															#line 5363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase695ActorState<FlowTestCase695Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase695", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase695");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase695", reinterpret_cast<unsigned long>(this), -1);

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
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase695( UnitTestParameters const& params ) {
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase695Actor(params));
															#line 5390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase695, "/flow/flow/yieldedFuture/progress")

#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 5396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase728()
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase728Actor>
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase728ActorState {
															#line 5403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase728ActorState(UnitTestParameters const& params) 
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase728", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase728ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase728", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			auto yn = makeReference<YieldMockNetwork>();
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int r = 0;r < 100;r++) {
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Promise<Void> p;
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Future<Void> u = p.getFuture();
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Future<Void> i = success(u);
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> v;
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < 25;i++) {
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					v.push_back(yieldedFuture(u));
															#line 5439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				auto numReady = [&v]() { return std::count_if(v.begin(), v.end(), [](Future<Void> v) { return v.isReady(); }); };
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Future<Void> j = success(u);
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(numReady() == 0);
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				int expectYield = deterministicRandom()->randomInt(0, 4);
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				int expectReady = expectYield;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				yn->nextYield = 1 + expectYield;
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				p.send(Void());
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(u.isReady() && i.isReady() && j.isReady() && numReady() == expectReady);
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(;numReady() != v.size();) {
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					expectYield = deterministicRandom()->randomInt(0, 4);
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					yn->nextYield = 1 + expectYield;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					expectReady += 1 + expectYield;
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					yn->tick();
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(numReady() == std::min<int>(expectReady, v.size()));
															#line 5469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < v.size();i++) {
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(v[i].getPromiseReferenceCount() == 0 && v[i].getFutureReferenceCount() == 1);
															#line 5475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
			}
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase728Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase728ActorState(); static_cast<FlowTestCase728Actor*>(this)->destroy(); return 0; }
															#line 5480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase728Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase728ActorState();
			static_cast<FlowTestCase728Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase728ActorState();
		static_cast<FlowTestCase728Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase728()
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase728Actor final : public Actor<Void>, public FastAllocated<FlowTestCase728Actor>, public FlowTestCase728ActorState<FlowTestCase728Actor> {
															#line 5509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase728Actor>::operator new;
	using FastAllocated<FlowTestCase728Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase728Actor(UnitTestParameters const& params) 
															#line 5519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase728ActorState<FlowTestCase728Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase728", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase728");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase728", reinterpret_cast<unsigned long>(this), -1);

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
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase728( UnitTestParameters const& params ) {
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase728Actor(params));
															#line 5546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase728, "/flow/flow/yieldedFuture/random")

#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 5552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase774()
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase774Actor>
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase774ActorState {
															#line 5559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase774ActorState(UnitTestParameters const& params) 
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase774", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase774ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase774", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			double start;
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int N = 1000000;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			auto yn = makeReference<YieldMockNetwork>();
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			yn->nextYield = 2 * N + 100;
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> p;
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> f = p.getFuture();
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			std::vector<Future<Void>> ys;
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ys.push_back(yieldedFuture(f));
															#line 5599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("yieldedFuture(f) create: %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p.send(Void());
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("yieldedFuture(f) total: %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for( auto& y : ys ) {
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(y.isReady());
															#line 5611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			p = Promise<Void>();
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f = p.getFuture();
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				yieldedFuture(f);
															#line 5623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("yieldedFuture(f) cancel: %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase774Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase774ActorState(); static_cast<FlowTestCase774Actor*>(this)->destroy(); return 0; }
															#line 5629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase774Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase774ActorState();
			static_cast<FlowTestCase774Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase774ActorState();
		static_cast<FlowTestCase774Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase774()
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase774Actor final : public Actor<Void>, public FastAllocated<FlowTestCase774Actor>, public FlowTestCase774ActorState<FlowTestCase774Actor> {
															#line 5658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase774Actor>::operator new;
	using FastAllocated<FlowTestCase774Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase774Actor(UnitTestParameters const& params) 
															#line 5668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase774ActorState<FlowTestCase774Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase774", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase774");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase774", reinterpret_cast<unsigned long>(this), -1);

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
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase774( UnitTestParameters const& params ) {
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase774Actor(params));
															#line 5695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase774, "/flow/perf/yieldedFuture")

#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 5701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase807()
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase807Actor>
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase807ActorState {
															#line 5708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase807ActorState(UnitTestParameters const& params) 
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase807", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase807ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase807", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> a, b;
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> c = chooseTwoActor(a.getFuture(), b.getFuture());
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a.getFutureReferenceCount() == 2 && b.getFutureReferenceCount() == 2 && !c.isReady());
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			b.send(Void());
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a.getFutureReferenceCount() == 0 && b.getFutureReferenceCount() == 0 && c.isReady() && !c.isError() && expectActorCount(1));
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			c = Future<Void>();
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a.getFutureReferenceCount() == 0 && b.getFutureReferenceCount() == 0 && expectActorCount(0));
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase807Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase807ActorState(); static_cast<FlowTestCase807Actor*>(this)->destroy(); return 0; }
															#line 5746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase807Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase807ActorState();
			static_cast<FlowTestCase807Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase807ActorState();
		static_cast<FlowTestCase807Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 5770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase807()
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase807Actor final : public Actor<Void>, public FastAllocated<FlowTestCase807Actor>, public FlowTestCase807ActorState<FlowTestCase807Actor> {
															#line 5775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase807Actor>::operator new;
	using FastAllocated<FlowTestCase807Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase807Actor(UnitTestParameters const& params) 
															#line 5785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase807ActorState<FlowTestCase807Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase807", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase807");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase807", reinterpret_cast<unsigned long>(this), -1);

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
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase807( UnitTestParameters const& params ) {
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase807Actor(params));
															#line 5812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase807, "/flow/flow/chooseTwoActor")

#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 5818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase821()
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase821Actor>
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase821ActorState {
															#line 5825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase821ActorState(UnitTestParameters const& params) 
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 5832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase821", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase821ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase821", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			double start;
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int N = 1000000;
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				emptyVoidActor();
															#line 5857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("emptyVoidActor(): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				emptyActor();
															#line 5869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("emptyActor(): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Promise<Void> neverSet;
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> never = neverSet.getFuture();
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> already = Void();
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			start = timer();
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < N;i++) {
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				oneWaitVoidActor(already);
															#line 5887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("oneWaitVoidActor(already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(expectActorCount(0));
															#line 5893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(already);
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 5903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(never);
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(!f.isReady());
															#line 5918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("(cancelled) oneWaitActor(never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(expectActorCount(0));
															#line 5924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(p.getFuture());
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 5939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(after): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out[i] = oneWaitActor(pipe[i].getFuture());
															#line 5956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 5964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(fifo): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out[i] = oneWaitActor(pipe[i].getFuture());
															#line 5981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = N - 1;i >= 0;i--) {
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 5989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(lifo): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 5993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(already, already);
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(already, already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(already, never);
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(already, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(never, already);
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(never, already): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(never, never);
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(!f.isReady());
															#line 6049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("(cancelled) chooseTwoActor(never, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(p.getFuture(), never);
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(after, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out[i] = chooseTwoActor(pipe[i].getFuture(), never);
															#line 6085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(fifo, never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out[i] = chooseTwoActor(pipe[i].getFuture(), pipe[i].getFuture());
															#line 6110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor(fifo, fifo): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out[i] = chooseTwoActor(chooseTwoActor(pipe[i].getFuture(), never), never);
															#line 6135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("chooseTwoActor^2((fifo, never), never): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = oneWaitActor(chooseTwoActor(p.getFuture(), never));
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(chooseTwoActor(after, never)): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out(N);
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out[i] = oneWaitActor(chooseTwoActor(pipe[i].getFuture(), never));
															#line 6179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out[i].isReady());
															#line 6187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("oneWaitActor(chooseTwoActor(fifo, never)): %0.1f M/sec\n", N / 1e6 / (timer() - start));
															#line 6191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Promise<Void> p;
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(p.getFuture(), never);
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> a = oneWaitActor(f);
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> b = oneWaitActor(f);
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					p.send(Void());
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(f.isReady());
															#line 6210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("2xoneWaitActor(chooseTwoActor(after, never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out1(N);
															#line 1047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out2(N);
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(pipe[i].getFuture(), never);
															#line 1051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out1[i] = oneWaitActor(f);
															#line 1052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out2[i] = oneWaitActor(f);
															#line 6233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out2[i].isReady());
															#line 6241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("2xoneWaitActor(chooseTwoActor(fifo, never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out1(N);
															#line 1064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out2(N);
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(oneWaitActor(pipe[i].getFuture()), never);
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out1[i] = oneWaitActor(f);
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out2[i] = oneWaitActor(f);
															#line 6264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out2[i].isReady());
															#line 6272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("2xoneWaitActor(chooseTwoActor(oneWaitActor(fifo), never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> pipe(N);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out1(N);
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> out2(N);
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					g_cheese = pipe[i].getFuture();
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> f = chooseTwoActor(cheeseWaitActor(), never);
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					g_cheese = f;
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out1[i] = cheeseWaitActor();
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					out2[i] = cheeseWaitActor();
															#line 6299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					pipe[i].send(Void());
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ASSERT(out2[i].isReady());
															#line 6307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("2xcheeseActor(chooseTwoActor(cheeseActor(fifo), never)): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("sizeof(CheeseWaitActorActor) == %zu\n", cheeseWaitActorSize());
															#line 6313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				PromiseStream<int> data;
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				Future<int> sum = sumActor(data.getFuture());
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					data.send(1);
															#line 6326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				data.sendError(end_of_stream());
															#line 1105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				ASSERT(sum.get() == N);
															#line 1106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("sumActor: %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			{
															#line 1110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				start = timer();
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Promise<Void>> ps(3);
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				std::vector<Future<Void>> fs(3);
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				for(int i = 0;i < N;i++) {
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ps.clear();
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					ps.resize(3);
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					for(int j = 0;j < ps.size();j++) {
															#line 1118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
						fs[j] = ps[j].getFuture();
															#line 6353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
					}
															#line 1120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					Future<Void> q = quorum(fs, 2);
															#line 1121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					for( auto& p : ps ) {
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
						p.send(Void());
															#line 6361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
					}
				}
															#line 1124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("quorum(2/3): %0.2f M/sec\n", N / 1e6 / (timer() - start));
															#line 6366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase821Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase821ActorState(); static_cast<FlowTestCase821Actor*>(this)->destroy(); return 0; }
															#line 6370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase821Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase821ActorState();
			static_cast<FlowTestCase821Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase821ActorState();
		static_cast<FlowTestCase821Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 6394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase821()
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase821Actor final : public Actor<Void>, public FastAllocated<FlowTestCase821Actor>, public FlowTestCase821ActorState<FlowTestCase821Actor> {
															#line 6399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase821Actor>::operator new;
	using FastAllocated<FlowTestCase821Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase821Actor(UnitTestParameters const& params) 
															#line 6409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase821ActorState<FlowTestCase821Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase821", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase821");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase821", reinterpret_cast<unsigned long>(this), -1);

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
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase821( UnitTestParameters const& params ) {
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase821Actor(params));
															#line 6436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase821, "#flow/flow/perf/actor patterns")

#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

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

															#line 6483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1171()
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1171Actor>
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1171ActorState {
															#line 6490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1171ActorState(UnitTestParameters const& params) 
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   yamr(),
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   it()
															#line 6501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1171", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1171ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1171", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			it = 0;
															#line 6516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1171ActorState();
		static_cast<FlowTestCase1171Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1171Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1171ActorState(); static_cast<FlowTestCase1171Actor*>(this)->destroy(); return 0; }
															#line 6539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1171Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1171ActorState();
		static_cast<FlowTestCase1171Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!(it < 100000))
															#line 6558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		yamr.randomOp();
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = yield();
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1171Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase1171Actor*>(this)->actor_wait_state = 1;
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1171Actor, 0, Void >*>(static_cast<FlowTestCase1171Actor*>(this)));
															#line 6573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1171Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1171Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1171Actor*>(this)->ActorCallback< FlowTestCase1171Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1171Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1171", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1171", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1171Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1171", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1171", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1171Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1171", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1171", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	YAMRandom<YieldedAsyncMap<int, int>> yamr;
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int it;
															#line 6678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1171()
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1171Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1171Actor, 0, Void >, public FastAllocated<FlowTestCase1171Actor>, public FlowTestCase1171ActorState<FlowTestCase1171Actor> {
															#line 6683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1171Actor>::operator new;
	using FastAllocated<FlowTestCase1171Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1171Actor, 0, Void >;
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1171Actor(UnitTestParameters const& params) 
															#line 6694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1171ActorState<FlowTestCase1171Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1171", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1171");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1171", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1171Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1171( UnitTestParameters const& params ) {
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1171Actor(params));
															#line 6722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1171, "/flow/flow/YieldedAsyncMap/randomized")

#line 1180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 6728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1181()
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1181Actor>
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1181ActorState {
															#line 6735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1181ActorState(UnitTestParameters const& params) 
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   yamr(),
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   it()
															#line 6746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1181", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1181ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1181", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			it = 0;
															#line 6761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1181ActorState();
		static_cast<FlowTestCase1181Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1181Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1181ActorState(); static_cast<FlowTestCase1181Actor*>(this)->destroy(); return 0; }
															#line 6784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1181Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1181ActorState();
		static_cast<FlowTestCase1181Actor*>(this)->finishSendAndDelPromiseRef();
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
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!(it < 100000))
															#line 6803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		yamr.randomOp();
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = yield();
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1181Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase1181Actor*>(this)->actor_wait_state = 1;
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1181Actor, 0, Void >*>(static_cast<FlowTestCase1181Actor*>(this)));
															#line 6818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		it++;
															#line 6849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1181Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1181Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1181Actor*>(this)->ActorCallback< FlowTestCase1181Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1181Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1181", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1181", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1181Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1181", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1181", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1181Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1181", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1181", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	YAMRandom<AsyncMap<int, int>> yamr;
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int it;
															#line 6923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1181()
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1181Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1181Actor, 0, Void >, public FastAllocated<FlowTestCase1181Actor>, public FlowTestCase1181ActorState<FlowTestCase1181Actor> {
															#line 6928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1181Actor>::operator new;
	using FastAllocated<FlowTestCase1181Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1181Actor, 0, Void >;
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1181Actor(UnitTestParameters const& params) 
															#line 6939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1181ActorState<FlowTestCase1181Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1181", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1181");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1181", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1181Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1181( UnitTestParameters const& params ) {
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1181Actor(params));
															#line 6967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1181, "/flow/flow/AsyncMap/randomized")

#line 1190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 6973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1191()
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1191Actor>
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1191ActorState {
															#line 6980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1191ActorState(UnitTestParameters const& params) 
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   yam(),
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   y0(yam.onChange(1))
															#line 6991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1191", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1191ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1191", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			yam.setUnconditional(1, 0);
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1 = yam.onChange(1);
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1a = yam.onChange(1);
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1b = yam.onChange(1);
															#line 1198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			yam.set(1, 1);
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y2 = yam.onChange(1);
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = reportErrors(y0, "Y0");
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 1;
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 0, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1191ActorState();
		static_cast<FlowTestCase1191Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reportErrors(y1, "Y1");
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 2;
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 1, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reportErrors(y1, "Y1");
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 2;
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 1, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1191Actor*>(this)->ActorCallback< FlowTestCase1191Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1191Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_2 = reportErrors(y1a, "Y1a");
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 3;
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 2, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_2 = reportErrors(y1a, "Y1a");
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 3;
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 2, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1191Actor*>(this)->ActorCallback< FlowTestCase1191Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase1191Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_3 = reportErrors(y1b, "Y1b");
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 4;
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 3, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_3 = reportErrors(y1b, "Y1b");
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 4;
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 3, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1191Actor*>(this)->ActorCallback< FlowTestCase1191Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase1191Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_4 = reportErrors(timeout(y2, 5, Void()), "Y2");
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 5;
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 4, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_4 = reportErrors(timeout(y2, 5, Void()), "Y2");
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 5;
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1191Actor, 4, Void >*>(static_cast<FlowTestCase1191Actor*>(this)));
															#line 7356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1191Actor*>(this)->ActorCallback< FlowTestCase1191Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase1191Actor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1191Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1191ActorState(); static_cast<FlowTestCase1191Actor*>(this)->destroy(); return 0; }
															#line 7428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1191Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1191ActorState();
		static_cast<FlowTestCase1191Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1191Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1191ActorState(); static_cast<FlowTestCase1191Actor*>(this)->destroy(); return 0; }
															#line 7440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1191Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1191ActorState();
		static_cast<FlowTestCase1191Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1191Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1191Actor*>(this)->ActorCallback< FlowTestCase1191Actor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1191Actor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< FlowTestCase1191Actor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	YieldedAsyncMap<int, int> yam;
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y0;
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1;
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1a;
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1b;
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y2;
															#line 7525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1191()
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1191Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1191Actor, 0, Void >, public ActorCallback< FlowTestCase1191Actor, 1, Void >, public ActorCallback< FlowTestCase1191Actor, 2, Void >, public ActorCallback< FlowTestCase1191Actor, 3, Void >, public ActorCallback< FlowTestCase1191Actor, 4, Void >, public FastAllocated<FlowTestCase1191Actor>, public FlowTestCase1191ActorState<FlowTestCase1191Actor> {
															#line 7530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1191Actor>::operator new;
	using FastAllocated<FlowTestCase1191Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1191Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase1191Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase1191Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase1191Actor, 3, Void >;
friend struct ActorCallback< FlowTestCase1191Actor, 4, Void >;
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1191Actor(UnitTestParameters const& params) 
															#line 7545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1191ActorState<FlowTestCase1191Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1191", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1191");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1191", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1191Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase1191Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase1191Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase1191Actor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< FlowTestCase1191Actor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1191( UnitTestParameters const& params ) {
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1191Actor(params));
															#line 7577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1191, "/flow/flow/YieldedAsyncMap/basic")

#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 7583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1212()
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1212Actor>
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1212ActorState {
															#line 7590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1212ActorState(UnitTestParameters const& params) 
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   yam()
															#line 7599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1212", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1212ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1212", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 0);
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1 = yam.onChange(1);
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1a = yam.onChange(1);
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1b = yam.onChange(1);
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 1);
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1.cancel();
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!y1a.isReady());
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1a.cancel();
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!y1b.isReady());
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 1);
															#line 1229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			y1b.cancel();
															#line 1230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(y1b.getError().code() == error_code_actor_cancelled);
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(yam.count(1) == 0);
															#line 1233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1212Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1212ActorState(); static_cast<FlowTestCase1212Actor*>(this)->destroy(); return 0; }
															#line 7640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1212Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1212ActorState();
			static_cast<FlowTestCase1212Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1212ActorState();
		static_cast<FlowTestCase1212Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	YieldedAsyncMap<int, int> yam;
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1a;
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1b;
															#line 7672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1212()
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1212Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1212Actor>, public FlowTestCase1212ActorState<FlowTestCase1212Actor> {
															#line 7677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1212Actor>::operator new;
	using FastAllocated<FlowTestCase1212Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1212Actor(UnitTestParameters const& params) 
															#line 7687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1212ActorState<FlowTestCase1212Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1212", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1212");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1212", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1212( UnitTestParameters const& params ) {
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1212Actor(params));
															#line 7714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1212, "/flow/flow/YieldedAsyncMap/cancel")

#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 7720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1236()
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1236Actor>
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1236ActorState {
															#line 7727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1236ActorState(UnitTestParameters const& params) 
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   yam(),
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   y1(yam.onChange(1)),
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   y2(yam.onChange(2))
															#line 7740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1236", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1236ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1236", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			auto* pyam = &yam;
															#line 1243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			uncancellable(trigger( [pyam]() { printf("Triggered\n"); pyam->triggerAll(); }, delay(1)));
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = y1;
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1236Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1236Actor*>(this)->actor_wait_state = 1;
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1236Actor, 0, Void >*>(static_cast<FlowTestCase1236Actor*>(this)));
															#line 7766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1236ActorState();
		static_cast<FlowTestCase1236Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		printf("Got y1\n");
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		y2.cancel();
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1236Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1236ActorState(); static_cast<FlowTestCase1236Actor*>(this)->destroy(); return 0; }
															#line 7793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1236Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1236ActorState();
		static_cast<FlowTestCase1236Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		printf("Got y1\n");
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		y2.cancel();
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1236Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1236ActorState(); static_cast<FlowTestCase1236Actor*>(this)->destroy(); return 0; }
															#line 7809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1236Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1236ActorState();
		static_cast<FlowTestCase1236Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1236Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1236Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1236Actor*>(this)->ActorCallback< FlowTestCase1236Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1236Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1236", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1236", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1236Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1236", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1236", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1236Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1236", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1236", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	YieldedAsyncMap<int, int> yam;
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y1;
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> y2;
															#line 7888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1236()
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1236Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1236Actor, 0, Void >, public FastAllocated<FlowTestCase1236Actor>, public FlowTestCase1236ActorState<FlowTestCase1236Actor> {
															#line 7893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1236Actor>::operator new;
	using FastAllocated<FlowTestCase1236Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1236Actor, 0, Void >;
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1236Actor(UnitTestParameters const& params) 
															#line 7904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1236ActorState<FlowTestCase1236Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1236", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1236");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1236", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1236Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1236( UnitTestParameters const& params ) {
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1236Actor(params));
															#line 7932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1236, "/flow/flow/YieldedAsyncMap/cancel2")

#line 1256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 7938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1257()
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1257Actor>
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1257ActorState {
															#line 7945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1257ActorState(UnitTestParameters const& params) 
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 7952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1257", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1257ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1257", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			AsyncVar<int> av;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> ch = av.onChange();
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!ch.isReady());
															#line 1261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			av.set(5);
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(ch.isReady());
															#line 1263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(av.get() == 5);
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ch = av.onChange();
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(!ch.isReady());
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			av.set(6);
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(ch.isReady());
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(av.get() == 6);
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1257Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1257ActorState(); static_cast<FlowTestCase1257Actor*>(this)->destroy(); return 0; }
															#line 7989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1257Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1257ActorState();
			static_cast<FlowTestCase1257Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1257ActorState();
		static_cast<FlowTestCase1257Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 8013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1257()
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1257Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1257Actor>, public FlowTestCase1257ActorState<FlowTestCase1257Actor> {
															#line 8018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1257Actor>::operator new;
	using FastAllocated<FlowTestCase1257Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1257Actor(UnitTestParameters const& params) 
															#line 8028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1257ActorState<FlowTestCase1257Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1257", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1257");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1257", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1257( UnitTestParameters const& params ) {
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1257Actor(params));
															#line 8055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1257, "/flow/flow/AsyncVar/basic")

#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 8061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via waitAfterCancel()
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class WaitAfterCancelActor>
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class WaitAfterCancelActorState {
															#line 8068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	WaitAfterCancelActorState(int* const& output) 
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : output(output)
															#line 8075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			*output = 0;
															#line 8090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			try {
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Never();
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<WaitAfterCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 8096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WaitAfterCancelActor*>(this)->actor_wait_state = 1;
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitAfterCancelActor, 0, Void >*>(static_cast<WaitAfterCancelActor*>(this)));
															#line 8101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(false);
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<WaitAfterCancelActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAfterCancelActorState(); static_cast<WaitAfterCancelActor*>(this)->destroy(); return 0; }
															#line 8132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<WaitAfterCancelActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAfterCancelActorState();
		static_cast<WaitAfterCancelActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = (*output = 1, Future<Void>(Void()));
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<WaitAfterCancelActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_1.get(), loopDepth); };
			static_cast<WaitAfterCancelActor*>(this)->actor_wait_state = 2;
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitAfterCancelActor, 1, Void >*>(static_cast<WaitAfterCancelActor*>(this)));
															#line 8152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int* output;
															#line 8328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via waitAfterCancel()
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class WaitAfterCancelActor final : public Actor<Void>, public ActorCallback< WaitAfterCancelActor, 0, Void >, public ActorCallback< WaitAfterCancelActor, 1, Void >, public FastAllocated<WaitAfterCancelActor>, public WaitAfterCancelActorState<WaitAfterCancelActor> {
															#line 8333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<WaitAfterCancelActor>::operator new;
	using FastAllocated<WaitAfterCancelActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitAfterCancelActor, 0, Void >;
friend struct ActorCallback< WaitAfterCancelActor, 1, Void >;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	WaitAfterCancelActor(int* const& output) 
															#line 8345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> waitAfterCancel( int* const& output ) {
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new WaitAfterCancelActor(output));
															#line 8374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 8379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1285()
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1285Actor>
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1285ActorState {
															#line 8386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1285ActorState(UnitTestParameters const& params) 
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 8393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1285", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1285ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1285", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			int a = -1;
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Future<Void> f = waitAfterCancel(&a);
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a == 0);
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			f.cancel();
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(a == 1);
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1285Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1285ActorState(); static_cast<FlowTestCase1285Actor*>(this)->destroy(); return 0; }
															#line 8418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1285Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1285ActorState();
			static_cast<FlowTestCase1285Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1285ActorState();
		static_cast<FlowTestCase1285Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 8442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1285()
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1285Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1285Actor>, public FlowTestCase1285ActorState<FlowTestCase1285Actor> {
															#line 8447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1285Actor>::operator new;
	using FastAllocated<FlowTestCase1285Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1285Actor(UnitTestParameters const& params) 
															#line 8457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1285ActorState<FlowTestCase1285Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1285", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1285");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1285", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1285( UnitTestParameters const& params ) {
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1285Actor(params));
															#line 8484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1285, "/fdbrpc/flow/wait_expression_after_cancel")

#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

// Tests for https://github.com/apple/foundationdb/issues/1226

template <class>
struct ShouldNotGoIntoClassContextStack;

class Foo1 {
public:
	explicit Foo1(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 8499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo1* const& self );
template <class> friend class Foo1_FooActorActorState;

#line 1304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 8508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class Foo1_FooActorActor>
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo1_FooActorActorState {
															#line 8514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo1_FooActorActorState(Foo1* const& self) 
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 8521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo1_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo1_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo1_FooActorActor, 0, Void >*>(static_cast<Foo1_FooActorActor*>(this)));
															#line 8543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo1_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo1_FooActorActorState(); static_cast<Foo1_FooActorActor*>(this)->destroy(); return 0; }
															#line 8566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo1_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo1_FooActorActorState();
		static_cast<Foo1_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo1_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo1_FooActorActorState(); static_cast<Foo1_FooActorActor*>(this)->destroy(); return 0; }
															#line 8578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo1* self;
															#line 8651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo1_FooActorActor final : public Actor<int>, public ActorCallback< Foo1_FooActorActor, 0, Void >, public FastAllocated<Foo1_FooActorActor>, public Foo1_FooActorActorState<Foo1_FooActorActor> {
															#line 8656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo1_FooActorActor>::operator new;
	using FastAllocated<Foo1_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo1_FooActorActor, 0, Void >;
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo1_FooActorActor(Foo1* const& self) 
															#line 8667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo1::fooActor( Foo1* const& self ) {
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo1_FooActorActor(self));
															#line 8694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

class [[nodiscard]] Foo2 {
public:
	explicit Foo2(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 8703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo2* const& self );
template <class> friend class Foo2_FooActorActorState;

#line 1318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 8712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class Foo2_FooActorActor>
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo2_FooActorActorState {
															#line 8718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo2_FooActorActorState(Foo2* const& self) 
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 8725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo2_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo2_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo2_FooActorActor, 0, Void >*>(static_cast<Foo2_FooActorActor*>(this)));
															#line 8747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo2_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo2_FooActorActorState(); static_cast<Foo2_FooActorActor*>(this)->destroy(); return 0; }
															#line 8770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo2_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo2_FooActorActorState();
		static_cast<Foo2_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo2_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo2_FooActorActorState(); static_cast<Foo2_FooActorActor*>(this)->destroy(); return 0; }
															#line 8782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo2* self;
															#line 8855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo2_FooActorActor final : public Actor<int>, public ActorCallback< Foo2_FooActorActor, 0, Void >, public FastAllocated<Foo2_FooActorActor>, public Foo2_FooActorActorState<Foo2_FooActorActor> {
															#line 8860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo2_FooActorActor>::operator new;
	using FastAllocated<Foo2_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo2_FooActorActor, 0, Void >;
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo2_FooActorActor(Foo2* const& self) 
															#line 8871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo2::fooActor( Foo2* const& self ) {
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo2_FooActorActor(self));
															#line 8898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

class alignas(4) Foo3 {
public:
	explicit Foo3(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 8907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo3* const& self );
template <class> friend class Foo3_FooActorActorState;

#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 8916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class Foo3_FooActorActor>
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo3_FooActorActorState {
															#line 8922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo3_FooActorActorState(Foo3* const& self) 
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 8929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo3_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo3_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo3_FooActorActor, 0, Void >*>(static_cast<Foo3_FooActorActor*>(this)));
															#line 8951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo3_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo3_FooActorActorState(); static_cast<Foo3_FooActorActor*>(this)->destroy(); return 0; }
															#line 8974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo3_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo3_FooActorActorState();
		static_cast<Foo3_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo3_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo3_FooActorActorState(); static_cast<Foo3_FooActorActor*>(this)->destroy(); return 0; }
															#line 8986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo3* self;
															#line 9059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo3_FooActorActor final : public Actor<int>, public ActorCallback< Foo3_FooActorActor, 0, Void >, public FastAllocated<Foo3_FooActorActor>, public Foo3_FooActorActorState<Foo3_FooActorActor> {
															#line 9064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo3_FooActorActor>::operator new;
	using FastAllocated<Foo3_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo3_FooActorActor, 0, Void >;
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo3_FooActorActor(Foo3* const& self) 
															#line 9075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo3::fooActor( Foo3* const& self ) {
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo3_FooActorActor(self));
															#line 9102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

struct Super {};

class Foo4 : Super {
public:
	explicit Foo4(int x) : x(x) {}
	Future<int> foo() { return fooActor(this); }
																#line 9113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo4* const& self );
template <class> friend class Foo4_FooActorActorState;

#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

private:
	int x;
};
															#line 9122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class Foo4_FooActorActor>
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo4_FooActorActorState {
															#line 9128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo4_FooActorActorState(Foo4* const& self) 
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 9135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Foo4_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Foo4_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Foo4_FooActorActor, 0, Void >*>(static_cast<Foo4_FooActorActor*>(this)));
															#line 9157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo4_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo4_FooActorActorState(); static_cast<Foo4_FooActorActor*>(this)->destroy(); return 0; }
															#line 9180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Foo4_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Foo4_FooActorActorState();
		static_cast<Foo4_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Foo4_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Foo4_FooActorActorState(); static_cast<Foo4_FooActorActor*>(this)->destroy(); return 0; }
															#line 9192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo4* self;
															#line 9265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Foo4_FooActorActor final : public Actor<int>, public ActorCallback< Foo4_FooActorActor, 0, Void >, public FastAllocated<Foo4_FooActorActor>, public Foo4_FooActorActorState<Foo4_FooActorActor> {
															#line 9270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Foo4_FooActorActor>::operator new;
	using FastAllocated<Foo4_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Foo4_FooActorActor, 0, Void >;
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Foo4_FooActorActor(Foo4* const& self) 
															#line 9281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Foo4::fooActor( Foo4* const& self ) {
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Foo4_FooActorActor(self));
															#line 9308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

struct Outer {
	class Foo5 : Super {
	public:
		explicit Foo5(int x) : x(x) {}
		Future<int> foo() { return fooActor(this); }
																	#line 9318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
[[nodiscard]] static Future<int> fooActor( Foo5* const& self );
template <class> friend class Outer_Foo5_FooActorActorState;

#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

	private:
		int x;
	};
};
															#line 9328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via fooActor()
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class Outer_Foo5_FooActorActor>
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Outer_Foo5_FooActorActorState {
															#line 9334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Outer_Foo5_FooActorActorState(Outer::Foo5* const& self) 
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : self(self)
															#line 9341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>();
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<Outer_Foo5_FooActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Outer_Foo5_FooActorActor*>(this)->actor_wait_state = 1;
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Outer_Foo5_FooActorActor, 0, Void >*>(static_cast<Outer_Foo5_FooActorActor*>(this)));
															#line 9363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Outer_Foo5_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Outer_Foo5_FooActorActorState(); static_cast<Outer_Foo5_FooActorActor*>(this)->destroy(); return 0; }
															#line 9386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<Outer_Foo5_FooActorActor*>(this)->SAV< int >::value()) int(self->x);
		this->~Outer_Foo5_FooActorActorState();
		static_cast<Outer_Foo5_FooActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<Outer_Foo5_FooActorActor*>(this)->SAV<int>::futures) { (void)(self->x); this->~Outer_Foo5_FooActorActorState(); static_cast<Outer_Foo5_FooActorActor*>(this)->destroy(); return 0; }
															#line 9398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Outer::Foo5* self;
															#line 9471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via fooActor()
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class Outer_Foo5_FooActorActor final : public Actor<int>, public ActorCallback< Outer_Foo5_FooActorActor, 0, Void >, public FastAllocated<Outer_Foo5_FooActorActor>, public Outer_Foo5_FooActorActorState<Outer_Foo5_FooActorActor> {
															#line 9476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<Outer_Foo5_FooActorActor>::operator new;
	using FastAllocated<Outer_Foo5_FooActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Outer_Foo5_FooActorActor, 0, Void >;
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Outer_Foo5_FooActorActor(Outer::Foo5* const& self) 
															#line 9487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<int> Outer::Foo5::fooActor( Outer::Foo5* const& self ) {
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<int>(new Outer_Foo5_FooActorActor(self));
															#line 9514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

// Meant to be run with -fsanitize=undefined
															#line 9520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1374()
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1374Actor>
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1374ActorState {
															#line 9527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1374ActorState(UnitTestParameters const& params) 
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 9534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1374", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1374ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1374", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt(std::numeric_limits<int>::min(), 0);
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt(0, std::numeric_limits<int>::max());
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt(std::numeric_limits<int>::min(), std::numeric_limits<int>::min() + 1) == std::numeric_limits<int>::min());
															#line 1380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt(std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::max()) == std::numeric_limits<int>::max() - 1);
															#line 1383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::min(), 0);
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt64(0, std::numeric_limits<int64_t>::max());
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min() + 1) == std::numeric_limits<int64_t>::min());
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(deterministicRandom()->randomInt64(std::numeric_limits<int64_t>::max() - 1, std::numeric_limits<int64_t>::max()) == std::numeric_limits<int64_t>::max() - 1);
															#line 1392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (!static_cast<FlowTestCase1374Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1374ActorState(); static_cast<FlowTestCase1374Actor*>(this)->destroy(); return 0; }
															#line 9569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			new (&static_cast<FlowTestCase1374Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1374ActorState();
			static_cast<FlowTestCase1374Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1374ActorState();
		static_cast<FlowTestCase1374Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 9593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1374()
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1374Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1374Actor>, public FlowTestCase1374ActorState<FlowTestCase1374Actor> {
															#line 9598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1374Actor>::operator new;
	using FastAllocated<FlowTestCase1374Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1374Actor(UnitTestParameters const& params) 
															#line 9608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1374ActorState<FlowTestCase1374Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1374", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1374");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1374", reinterpret_cast<unsigned long>(this), -1);

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
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1374( UnitTestParameters const& params ) {
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1374Actor(params));
															#line 9635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1374, "/flow/DeterministicRandom/SignedOverflow")

#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

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

																#line 9665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
// This generated class is to be used only via listen()
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class ListenActor>
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class ListenActorState {
															#line 9671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	ListenActorState(FutureStream<Tracker> const& stream) 
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : stream(stream)
															#line 9678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_0 = stream;
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<ListenActor*>(this)->actor_wait_state = 1;
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ListenActor, 0, Tracker >*>(static_cast<ListenActor*>(this)));
															#line 9700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ListenActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ListenActorState(); static_cast<ListenActor*>(this)->destroy(); return 0; }
															#line 9727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<ListenActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ListenActorState();
		static_cast<ListenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker && movedTracker,int loopDepth) 
	{
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<ListenActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ListenActorState(); static_cast<ListenActor*>(this)->destroy(); return 0; }
															#line 9743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FutureStream<Tracker> stream;
															#line 9816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via listen()
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class ListenActor final : public Actor<Void>, public ActorSingleCallback< ListenActor, 0, Tracker >, public FastAllocated<ListenActor>, public ListenActorState<ListenActor> {
															#line 9821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<ListenActor>::operator new;
	using FastAllocated<ListenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ListenActor, 0, Tracker >;
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	ListenActor(FutureStream<Tracker> const& stream) 
															#line 9832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] static Future<Void> listen( FutureStream<Tracker> const& stream ) {
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new ListenActor(stream));
															#line 9859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
};

															#line 9865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1427()
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1427Actor>
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1427ActorState {
															#line 9872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1427ActorState(UnitTestParameters const& params) 
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   stream(),
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   listener()
															#line 9883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1427", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1427ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1427", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				listener = Tracker::listen(stream.getFuture());
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				stream.send(Tracker{});
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				StrictFuture<Void> __when_expr_0 = listener;
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 1;
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1427Actor, 0, Void >*>(static_cast<FlowTestCase1427Actor*>(this)));
															#line 9910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1427ActorState();
		static_cast<FlowTestCase1427Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			listener = Tracker::listen(stream.getFuture());
															#line 1442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Tracker namedTracker;
															#line 1443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			stream.send(namedTracker);
															#line 1444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = listener;
															#line 1444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 2;
															#line 1444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1427Actor, 1, Void >*>(static_cast<FlowTestCase1427Actor*>(this)));
															#line 9948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1427Actor*>(this)->ActorCallback< FlowTestCase1427Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1427Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1427Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1427Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
		{
															#line 1449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			stream.send(Tracker{});
															#line 1450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			stream.send(Tracker{});
															#line 10036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				FutureStream<Tracker> __when_expr_2 = stream.getFuture();
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.pop(), loopDepth); };
				static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 3;
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >*>(static_cast<FlowTestCase1427Actor*>(this)));
															#line 10047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1427Actor*>(this)->ActorCallback< FlowTestCase1427Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1427Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1427Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase1427Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
		{
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Tracker namedTracker1;
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			Tracker namedTracker2;
															#line 1467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			stream.send(namedTracker1);
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			stream.send(namedTracker2);
															#line 10140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				FutureStream<Tracker> __when_expr_4 = stream.getFuture();
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.pop(), loopDepth); };
				static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 5;
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >*>(static_cast<FlowTestCase1427Actor*>(this)));
															#line 10151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				loopDepth = 0;
			}
		}

		return loopDepth;
	}
	int a_body1cont6(int loopDepth) 
	{
		{
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_3 = stream.getFuture();
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.pop(), loopDepth); };
			static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 4;
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >*>(static_cast<FlowTestCase1427Actor*>(this)));
															#line 10170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 10182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Tracker const& __movedTracker,int loopDepth) 
	{
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		movedTracker = __movedTracker;
															#line 10191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1427Actor*>(this)->ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont9(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Tracker const& movedTracker,int loopDepth) 
	{
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 10266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Tracker && movedTracker,int loopDepth) 
	{
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 10277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1427Actor*>(this)->ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont11(int loopDepth) 
	{
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1427Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1427ActorState(); static_cast<FlowTestCase1427Actor*>(this)->destroy(); return 0; }
															#line 10349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1427Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1427ActorState();
		static_cast<FlowTestCase1427Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12(int loopDepth) 
	{
		{
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_5 = stream.getFuture();
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont12when1(__when_expr_5.pop(), loopDepth); };
			static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 6;
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >*>(static_cast<FlowTestCase1427Actor*>(this)));
															#line 10369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont13(int loopDepth) 
	{
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!copiedTracker.moved);
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(copiedTracker.copied == 1);
															#line 10381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Tracker const& __copiedTracker,int loopDepth) 
	{
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		copiedTracker = __copiedTracker;
															#line 10390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1427Actor*>(this)->ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont12cont1(int loopDepth) 
	{
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont2(Tracker const& copiedTracker,int loopDepth) 
	{
															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!copiedTracker.moved);
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(copiedTracker.copied == 1);
															#line 10465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont2(Tracker && copiedTracker,int loopDepth) 
	{
															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!copiedTracker.moved);
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(copiedTracker.copied == 1);
															#line 10476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1427Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1427Actor*>(this)->ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont12when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont12when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	PromiseStream<Tracker> stream;
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Future<Void> listener;
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Tracker movedTracker;
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Tracker copiedTracker;
															#line 10554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1427()
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1427Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1427Actor, 0, Void >, public ActorCallback< FlowTestCase1427Actor, 1, Void >, public ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >, public ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >, public ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >, public ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >, public FastAllocated<FlowTestCase1427Actor>, public FlowTestCase1427ActorState<FlowTestCase1427Actor> {
															#line 10559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1427Actor>::operator new;
	using FastAllocated<FlowTestCase1427Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1427Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase1427Actor, 1, Void >;
friend struct ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >;
friend struct ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >;
friend struct ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >;
friend struct ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >;
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1427Actor(UnitTestParameters const& params) 
															#line 10575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1427ActorState<FlowTestCase1427Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1427", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1427");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1427", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1427Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase1427Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorSingleCallback< FlowTestCase1427Actor, 2, Tracker >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorSingleCallback< FlowTestCase1427Actor, 3, Tracker >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorSingleCallback< FlowTestCase1427Actor, 4, Tracker >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorSingleCallback< FlowTestCase1427Actor, 5, Tracker >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1427( UnitTestParameters const& params ) {
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1427Actor(params));
															#line 10608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1427, "/flow/flow/PromiseStream/move")

#line 1485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 10614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1486()
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1486Actor>
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1486ActorState {
															#line 10621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1486ActorState(UnitTestParameters const& params) 
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params)
															#line 10628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1486", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1486ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1486", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			PromiseStream<Tracker> stream;
															#line 1488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			stream.send(Tracker{});
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			FutureStream<Tracker> __when_expr_0 = stream.getFuture();
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1486Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<FlowTestCase1486Actor*>(this)->actor_wait_state = 1;
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >*>(static_cast<FlowTestCase1486Actor*>(this)));
															#line 10654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1486ActorState();
		static_cast<FlowTestCase1486Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker const& tracker,int loopDepth) 
	{
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		Tracker movedTracker = std::move(tracker);
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(tracker.moved);
															#line 1492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1486Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1486ActorState(); static_cast<FlowTestCase1486Actor*>(this)->destroy(); return 0; }
															#line 10685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1486Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1486ActorState();
		static_cast<FlowTestCase1486Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Tracker && tracker,int loopDepth) 
	{
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		Tracker movedTracker = std::move(tracker);
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(tracker.moved);
															#line 1492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(!movedTracker.moved);
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(movedTracker.copied == 0);
															#line 1494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1486Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1486ActorState(); static_cast<FlowTestCase1486Actor*>(this)->destroy(); return 0; }
															#line 10705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1486Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1486ActorState();
		static_cast<FlowTestCase1486Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1486Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1486Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1486Actor*>(this)->ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >*,Tracker const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1486", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1486", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >*,Tracker && value) 
	{
		fdb_probe_actor_enter("flowTestCase1486", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1486", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1486", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1486", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 10778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1486()
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1486Actor final : public Actor<Void>, public ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >, public FastAllocated<FlowTestCase1486Actor>, public FlowTestCase1486ActorState<FlowTestCase1486Actor> {
															#line 10783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1486Actor>::operator new;
	using FastAllocated<FlowTestCase1486Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >;
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1486Actor(UnitTestParameters const& params) 
															#line 10794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1486ActorState<FlowTestCase1486Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1486", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1486");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1486", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< FlowTestCase1486Actor, 0, Tracker >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1486( UnitTestParameters const& params ) {
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1486Actor(params));
															#line 10822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1486, "/flow/flow/PromiseStream/move2")

#line 1496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

constexpr double mutexTestDelay = 0.00001;

															#line 10830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via mutexTest()
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class MutexTestActor>
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class MutexTestActorState {
															#line 10837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	MutexTestActorState(int const& id,FlowMutex* const& mutex,int const& n,bool const& allowError,bool* const& verbose) 
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : id(id),
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   mutex(mutex),
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   n(n),
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   allowError(allowError),
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   verbose(verbose)
															#line 10852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			;
															#line 10867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d Returning\n", id);
															#line 10894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<MutexTestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MutexTestActorState(); static_cast<MutexTestActor*>(this)->destroy(); return 0; }
															#line 10898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!(n-- > 0))
															#line 10917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		d = deterministicRandom()->random01() * mutexTestDelay;
															#line 1502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d wait %f while unlocked\n", id, n, d);
															#line 10929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(d);
															#line 1505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 1;
															#line 1505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 0, Void >*>(static_cast<MutexTestActor*>(this)));
															#line 10940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d locking\n", id, n);
															#line 10966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<FlowMutex::Lock> __when_expr_1 = mutex->take();
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 2;
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*>(static_cast<MutexTestActor*>(this)));
															#line 10977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 10986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d locking\n", id, n);
															#line 10990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<FlowMutex::Lock> __when_expr_1 = mutex->take();
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 2;
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 1, FlowMutex::Lock >*>(static_cast<MutexTestActor*>(this)));
															#line 11001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 11073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d locked\n", id, n);
															#line 11077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		d = deterministicRandom()->random01() * mutexTestDelay;
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (*verbose)
															#line 11083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d:%d wait %f while locked\n", id, n, d);
															#line 11087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(d);
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<MutexTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 11093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<MutexTestActor*>(this)->actor_wait_state = 3;
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MutexTestActor, 2, Void >*>(static_cast<MutexTestActor*>(this)));
															#line 11098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(FlowMutex::Lock const& __lock,int loopDepth) 
	{
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		lock = __lock;
															#line 11107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (n == 0 && allowError)
															#line 11174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (deterministicRandom()->coinflip())
															#line 11178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d sending error\n", id, n);
															#line 11186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				lock.error(end_of_stream());
															#line 11190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			else
			{
															#line 1531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d dropping promise, returning without unlock\n", id, n);
															#line 11200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (*verbose)
															#line 11208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("%d:%d unlocking\n", id, n);
															#line 11212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			lock.release();
															#line 11216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (n == 0 && allowError)
															#line 11226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (deterministicRandom()->coinflip())
															#line 11230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d sending error\n", id, n);
															#line 11238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				lock.error(end_of_stream());
															#line 11242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
			else
			{
															#line 1531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				if (*verbose)
															#line 11248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				{
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
					printf("%d:%d dropping promise, returning without unlock\n", id, n);
															#line 11252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (*verbose)
															#line 11260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("%d:%d unlocking\n", id, n);
															#line 11264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			lock.release();
															#line 11268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int id;
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowMutex* mutex;
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int n;
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	bool allowError;
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	bool* verbose;
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	double d;
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowMutex::Lock lock;
															#line 11351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via mutexTest()
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class MutexTestActor final : public Actor<Void>, public ActorCallback< MutexTestActor, 0, Void >, public ActorCallback< MutexTestActor, 1, FlowMutex::Lock >, public ActorCallback< MutexTestActor, 2, Void >, public FastAllocated<MutexTestActor>, public MutexTestActorState<MutexTestActor> {
															#line 11356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	MutexTestActor(int const& id,FlowMutex* const& mutex,int const& n,bool const& allowError,bool* const& verbose) 
															#line 11369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
[[nodiscard]] Future<Void> mutexTest( int const& id, FlowMutex* const& mutex, int const& n, bool const& allowError, bool* const& verbose ) {
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new MutexTestActor(id, mutex, n, allowError, verbose));
															#line 11399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}

#line 1548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"

															#line 11404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase1549()
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
template <class FlowTestCase1549Actor>
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1549ActorState {
															#line 11411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1549ActorState(UnitTestParameters const& params) 
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		 : params(params),
															#line 1550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   count(100000),
															#line 1553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   verboseSetting(false),
															#line 1555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		   verboseTestIteration(-1)
															#line 11424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1549", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1549ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1549", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				verbose = verboseSetting || count == verboseTestIteration;
															#line 1560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				;
															#line 11442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		this->~FlowTestCase1549ActorState();
		static_cast<FlowTestCase1549Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!static_cast<FlowTestCase1549Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1549ActorState(); static_cast<FlowTestCase1549Actor*>(this)->destroy(); return 0; }
															#line 11471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		new (&static_cast<FlowTestCase1549Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1549ActorState();
		static_cast<FlowTestCase1549Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("Error at count=%d\n", count + 1);
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			ASSERT(false);
															#line 11486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!(--count > 0))
															#line 11514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (count % 1000 == 0)
															#line 11520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("%d tests left\n", count);
															#line 11524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		mutex = FlowMutex();
															#line 1566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		tests = std::vector<Future<Void>>();
															#line 1568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		allowErrors = deterministicRandom()->coinflip();
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (verbose)
															#line 11534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("\nTesting allowErrors=%d\n", allowErrors);
															#line 11538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		error = Optional<Error>();
															#line 11542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		try {
															#line 1576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			for(int i = 0;i < 10;++i) {
															#line 1577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				tests.push_back(mutexTest(i, &mutex, 10, allowErrors, &verbose));
															#line 11548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(tests);
															#line 1579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 1;
															#line 1579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1549Actor, 0, Void >*>(static_cast<FlowTestCase1549Actor*>(this)));
															#line 11559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		ASSERT(error.present() == allowErrors);
															#line 11587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("Caught error %s\n", e.what());
															#line 11601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			error = e;
															#line 1595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			i = int();
															#line 1596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("Waiting for completions.  Future end states:\n");
															#line 11613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			i = 0;
															#line 11617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (allowErrors)
															#line 11632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("Final wait in case error was injected by the last actor to finish\n");
															#line 11640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(mutex.take());
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 2;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1549Actor, 1, Void >*>(static_cast<FlowTestCase1549Actor*>(this)));
															#line 11651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (allowErrors)
															#line 11665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (verbose)
															#line 11669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			{
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
				printf("Final wait in case error was injected by the last actor to finish\n");
															#line 11673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			}
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(mutex.take());
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 2;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1549Actor, 1, Void >*>(static_cast<FlowTestCase1549Actor*>(this)));
															#line 11684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1549Actor*>(this)->ActorCallback< FlowTestCase1549Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1549Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1549Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1549Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1549Actor*>(this)->ActorCallback< FlowTestCase1549Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1549Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1549Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase1549Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 1);

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
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (!(i < tests.size()))
															#line 11868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
			return a_body1loopBody1Catch1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		StrictFuture<ErrorOr<Void>> __when_expr_2 = errorOr(tests[i]);
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 11876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1Catch1loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 3;
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >*>(static_cast<FlowTestCase1549Actor*>(this)));
															#line 11881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (verbose)
															#line 11903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("  %d: %s\n", i, f.isError() ? f.getError().what() : "done");
															#line 11907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		++i;
															#line 11911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1Catch1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont1(ErrorOr<Void> && f,int loopDepth) 
	{
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		if (verbose)
															#line 11920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		{
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
			printf("  %d: %s\n", i, f.isError() ? f.getError().what() : "done");
															#line 11924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		}
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
		++i;
															#line 11928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
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
		if (static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1549Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1549Actor*>(this)->ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >*,ErrorOr<Void> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >*,ErrorOr<Void> && value) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), 2);

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
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	UnitTestParameters params;
															#line 1550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int count;
															#line 1553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	bool verboseSetting;
															#line 1555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int verboseTestIteration;
															#line 1558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	bool verbose;
															#line 1565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowMutex mutex;
															#line 1566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	std::vector<Future<Void>> tests;
															#line 1568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	bool allowErrors;
															#line 1573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	Optional<Error> error;
															#line 1595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	int i;
															#line 12029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1549()
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
class FlowTestCase1549Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1549Actor, 0, Void >, public ActorCallback< FlowTestCase1549Actor, 1, Void >, public ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >, public FastAllocated<FlowTestCase1549Actor>, public FlowTestCase1549ActorState<FlowTestCase1549Actor> {
															#line 12034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1549Actor>::operator new;
	using FastAllocated<FlowTestCase1549Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1549Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase1549Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >;
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	FlowTestCase1549Actor(UnitTestParameters const& params) 
															#line 12047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1549ActorState<FlowTestCase1549Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1549", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1549");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1549", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1549Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase1549Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase1549Actor, 2, ErrorOr<Void> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
static Future<Void> flowTestCase1549( UnitTestParameters const& params ) {
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
	return Future<Void>(new FlowTestCase1549Actor(params));
															#line 12077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1549, "/flow/flow/FlowMutex")

#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTests.actor.cpp"
