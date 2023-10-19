#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
/*
 * ThreadHelper.actor.cpp
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

#include <string>

#include "flow/flow.h"
#include "flow/network.h"
#include "flow/ThreadHelper.actor.h"
#include "flow/Error.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

ThreadCallback* ThreadCallback::addCallback(ThreadCallback* cb) {
	return (new ThreadMultiCallback())->addCallback(this)->addCallback(cb);
}

// A simple thread object that sends the result
struct ThreadFutureSendObj {
	void operator()() { tsav->send(Void()); }
	ThreadSingleAssignmentVar<Void>* tsav;
};

// A simple thread object that cancels the threadFuture
struct ThreadFutureCancelObj {
	ThreadFutureCancelObj(ThreadFuture<Void> f) : f(f) {}
	void operator()() { f.cancel(); }
	ThreadFuture<Void> f;
};

// This unit test should be running with TSAN enabled binary
															#line 50 "/src/flow/ThreadHelper.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase48()
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
template <class FlowTestCase48Actor>
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
class FlowTestCase48ActorState {
															#line 57 "/src/flow/ThreadHelper.actor.g.cpp"
public:
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	FlowTestCase48ActorState(UnitTestParameters const& params) 
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		 : params(params)
															#line 64 "/src/flow/ThreadHelper.actor.g.cpp"
	{

	}
	~FlowTestCase48ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			if (g_network->isSimulated())
															#line 77 "/src/flow/ThreadHelper.actor.g.cpp"
			{
															#line 51 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
				if (!static_cast<FlowTestCase48Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase48ActorState(); static_cast<FlowTestCase48Actor*>(this)->destroy(); return 0; }
															#line 81 "/src/flow/ThreadHelper.actor.g.cpp"
				new (&static_cast<FlowTestCase48Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase48ActorState();
				static_cast<FlowTestCase48Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 52 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			auto* tsav = new ThreadSingleAssignmentVar<Void>;
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			thread = std::thread{ ThreadFutureSendObj{ tsav } };
															#line 54 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			ThreadFuture<Void> f(tsav);
															#line 56 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			StrictFuture<Void> __when_expr_0 = safeThreadFutureToFuture(f);
															#line 56 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			if (static_cast<FlowTestCase48Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 97 "/src/flow/ThreadHelper.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase48Actor*>(this)->actor_wait_state = 1;
															#line 56 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase48Actor, 0, Void >*>(static_cast<FlowTestCase48Actor*>(this)));
															#line 102 "/src/flow/ThreadHelper.actor.g.cpp"
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
		this->~FlowTestCase48ActorState();
		static_cast<FlowTestCase48Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		thread.join();
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		if (!static_cast<FlowTestCase48Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase48ActorState(); static_cast<FlowTestCase48Actor*>(this)->destroy(); return 0; }
															#line 127 "/src/flow/ThreadHelper.actor.g.cpp"
		new (&static_cast<FlowTestCase48Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase48ActorState();
		static_cast<FlowTestCase48Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		thread.join();
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		if (!static_cast<FlowTestCase48Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase48ActorState(); static_cast<FlowTestCase48Actor*>(this)->destroy(); return 0; }
															#line 141 "/src/flow/ThreadHelper.actor.g.cpp"
		new (&static_cast<FlowTestCase48Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase48ActorState();
		static_cast<FlowTestCase48Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase48Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase48Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase48Actor*>(this)->ActorCallback< FlowTestCase48Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase48Actor, 0, Void >*,Void const& value) 
	{
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< FlowTestCase48Actor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< FlowTestCase48Actor, 0, Void >*,Error err) 
	{
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	UnitTestParameters params;
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	std::thread thread;
															#line 210 "/src/flow/ThreadHelper.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase48()
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
class FlowTestCase48Actor final : public Actor<Void>, public ActorCallback< FlowTestCase48Actor, 0, Void >, public FastAllocated<FlowTestCase48Actor>, public FlowTestCase48ActorState<FlowTestCase48Actor> {
															#line 215 "/src/flow/ThreadHelper.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase48Actor>::operator new;
	using FastAllocated<FlowTestCase48Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase48Actor, 0, Void >;
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	FlowTestCase48Actor(UnitTestParameters const& params) 
															#line 226 "/src/flow/ThreadHelper.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase48ActorState<FlowTestCase48Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase48");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase48Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
static Future<Void> flowTestCase48( UnitTestParameters const& params ) {
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	return Future<Void>(new FlowTestCase48Actor(params));
															#line 252 "/src/flow/ThreadHelper.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase48, "/flow/safeThreadFutureToFuture/Send")

#line 60 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"

// Test the case where the underlying threadFuture is cancelled
															#line 259 "/src/flow/ThreadHelper.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase62()
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
template <class FlowTestCase62Actor>
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
class FlowTestCase62ActorState {
															#line 266 "/src/flow/ThreadHelper.actor.g.cpp"
public:
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	FlowTestCase62ActorState(UnitTestParameters const& params) 
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		 : params(params)
															#line 273 "/src/flow/ThreadHelper.actor.g.cpp"
	{

	}
	~FlowTestCase62ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			if (g_network->isSimulated())
															#line 286 "/src/flow/ThreadHelper.actor.g.cpp"
			{
															#line 65 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
				if (!static_cast<FlowTestCase62Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase62ActorState(); static_cast<FlowTestCase62Actor*>(this)->destroy(); return 0; }
															#line 290 "/src/flow/ThreadHelper.actor.g.cpp"
				new (&static_cast<FlowTestCase62Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase62ActorState();
				static_cast<FlowTestCase62Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			ThreadFuture<Void> f = onMainThread([]() -> Future<Void> { return Never(); });
															#line 67 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			thread = std::thread{ ThreadFutureCancelObj(f) };
															#line 300 "/src/flow/ThreadHelper.actor.g.cpp"
			try {
															#line 69 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
				StrictFuture<Void> __when_expr_0 = safeThreadFutureToFuture(f);
															#line 69 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
				if (static_cast<FlowTestCase62Actor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 306 "/src/flow/ThreadHelper.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase62Actor*>(this)->actor_wait_state = 1;
															#line 69 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase62Actor, 0, Void >*>(static_cast<FlowTestCase62Actor*>(this)));
															#line 311 "/src/flow/ThreadHelper.actor.g.cpp"
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
		this->~FlowTestCase62ActorState();
		static_cast<FlowTestCase62Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 74 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		thread.join();
															#line 75 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		if (!static_cast<FlowTestCase62Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase62ActorState(); static_cast<FlowTestCase62Actor*>(this)->destroy(); return 0; }
															#line 342 "/src/flow/ThreadHelper.actor.g.cpp"
		new (&static_cast<FlowTestCase62Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase62ActorState();
		static_cast<FlowTestCase62Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 72 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
			ASSERT(e.code() == error_code_actor_cancelled);
															#line 355 "/src/flow/ThreadHelper.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 70 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		ASSERT(false);
															#line 370 "/src/flow/ThreadHelper.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 70 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
		ASSERT(false);
															#line 379 "/src/flow/ThreadHelper.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase62Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase62Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase62Actor*>(this)->ActorCallback< FlowTestCase62Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase62Actor, 0, Void >*,Void const& value) 
	{
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< FlowTestCase62Actor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< FlowTestCase62Actor, 0, Void >*,Error err) 
	{
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}

	}
	int a_body1cont5(int loopDepth) 
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
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	UnitTestParameters params;
															#line 67 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	std::thread thread;
															#line 458 "/src/flow/ThreadHelper.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase62()
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
class FlowTestCase62Actor final : public Actor<Void>, public ActorCallback< FlowTestCase62Actor, 0, Void >, public FastAllocated<FlowTestCase62Actor>, public FlowTestCase62ActorState<FlowTestCase62Actor> {
															#line 463 "/src/flow/ThreadHelper.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase62Actor>::operator new;
	using FastAllocated<FlowTestCase62Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase62Actor, 0, Void >;
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	FlowTestCase62Actor(UnitTestParameters const& params) 
															#line 474 "/src/flow/ThreadHelper.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase62ActorState<FlowTestCase62Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase62");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase62Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
static Future<Void> flowTestCase62( UnitTestParameters const& params ) {
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
	return Future<Void>(new FlowTestCase62Actor(params));
															#line 500 "/src/flow/ThreadHelper.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase62, "/flow/safeThreadFutureToFuture/Cancel")

#line 77 "/tmp/fdb_c/foundationdb_ep/flow/ThreadHelper.actor.cpp"
