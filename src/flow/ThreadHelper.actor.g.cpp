#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
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
#include <thread>

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
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase49()
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
template <class FlowTestCase49Actor>
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
class FlowTestCase49ActorState {
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
public:
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	FlowTestCase49ActorState(UnitTestParameters const& params) 
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		 : params(params)
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase49", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase49ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase49", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			if (g_network->isSimulated())
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
			{
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
				if (!static_cast<FlowTestCase49Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase49ActorState(); static_cast<FlowTestCase49Actor*>(this)->destroy(); return 0; }
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
				new (&static_cast<FlowTestCase49Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase49ActorState();
				static_cast<FlowTestCase49Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			auto* tsav = new ThreadSingleAssignmentVar<Void>;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			thread = std::thread{ ThreadFutureSendObj{ tsav } };
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			ThreadFuture<Void> f(tsav);
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			StrictFuture<Void> __when_expr_0 = safeThreadFutureToFuture(f);
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			if (static_cast<FlowTestCase49Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase49Actor*>(this)->actor_wait_state = 1;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase49Actor, 0, Void >*>(static_cast<FlowTestCase49Actor*>(this)));
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
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
		this->~FlowTestCase49ActorState();
		static_cast<FlowTestCase49Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		thread.join();
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		if (!static_cast<FlowTestCase49Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase49ActorState(); static_cast<FlowTestCase49Actor*>(this)->destroy(); return 0; }
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
		new (&static_cast<FlowTestCase49Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase49ActorState();
		static_cast<FlowTestCase49Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		thread.join();
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		if (!static_cast<FlowTestCase49Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase49ActorState(); static_cast<FlowTestCase49Actor*>(this)->destroy(); return 0; }
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
		new (&static_cast<FlowTestCase49Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase49ActorState();
		static_cast<FlowTestCase49Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase49Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase49Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase49Actor*>(this)->ActorCallback< FlowTestCase49Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase49Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase49", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase49", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase49Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase49", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase49", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase49Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase49", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase49", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	UnitTestParameters params;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	std::thread thread;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase49()
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
class FlowTestCase49Actor final : public Actor<Void>, public ActorCallback< FlowTestCase49Actor, 0, Void >, public FastAllocated<FlowTestCase49Actor>, public FlowTestCase49ActorState<FlowTestCase49Actor> {
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase49Actor>::operator new;
	using FastAllocated<FlowTestCase49Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase49Actor, 0, Void >;
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	FlowTestCase49Actor(UnitTestParameters const& params) 
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase49ActorState<FlowTestCase49Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase49", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase49");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase49", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase49Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
static Future<Void> flowTestCase49( UnitTestParameters const& params ) {
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	return Future<Void>(new FlowTestCase49Actor(params));
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase49, "/flow/safeThreadFutureToFuture/Send")

#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"

// Test the case where the underlying threadFuture is cancelled
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase63()
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
template <class FlowTestCase63Actor>
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
class FlowTestCase63ActorState {
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
public:
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	FlowTestCase63ActorState(UnitTestParameters const& params) 
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		 : params(params)
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase63", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase63ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase63", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			if (g_network->isSimulated())
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
			{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
				if (!static_cast<FlowTestCase63Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase63ActorState(); static_cast<FlowTestCase63Actor*>(this)->destroy(); return 0; }
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
				new (&static_cast<FlowTestCase63Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase63ActorState();
				static_cast<FlowTestCase63Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			ThreadFuture<Void> f = onMainThread([]() -> Future<Void> { return Never(); });
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			thread = std::thread{ ThreadFutureCancelObj(f) };
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
			try {
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
				StrictFuture<Void> __when_expr_0 = safeThreadFutureToFuture(f);
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
				if (static_cast<FlowTestCase63Actor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase63Actor*>(this)->actor_wait_state = 1;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase63Actor, 0, Void >*>(static_cast<FlowTestCase63Actor*>(this)));
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
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
		this->~FlowTestCase63ActorState();
		static_cast<FlowTestCase63Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		thread.join();
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		if (!static_cast<FlowTestCase63Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase63ActorState(); static_cast<FlowTestCase63Actor*>(this)->destroy(); return 0; }
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
		new (&static_cast<FlowTestCase63Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase63ActorState();
		static_cast<FlowTestCase63Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
			ASSERT(e.code() == error_code_actor_cancelled);
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
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
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		ASSERT(false);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
		ASSERT(false);
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
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
		if (static_cast<FlowTestCase63Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase63Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase63Actor*>(this)->ActorCallback< FlowTestCase63Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase63Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase63", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase63", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase63Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase63", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase63", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase63Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase63", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase63", reinterpret_cast<unsigned long>(this), 0);

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
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	UnitTestParameters params;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	std::thread thread;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase63()
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
class FlowTestCase63Actor final : public Actor<Void>, public ActorCallback< FlowTestCase63Actor, 0, Void >, public FastAllocated<FlowTestCase63Actor>, public FlowTestCase63ActorState<FlowTestCase63Actor> {
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase63Actor>::operator new;
	using FastAllocated<FlowTestCase63Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase63Actor, 0, Void >;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	FlowTestCase63Actor(UnitTestParameters const& params) 
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase63ActorState<FlowTestCase63Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase63", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase63");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase63", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase63Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
static Future<Void> flowTestCase63( UnitTestParameters const& params ) {
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
	return Future<Void>(new FlowTestCase63Actor(params));
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase63, "/flow/safeThreadFutureToFuture/Cancel")

#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ThreadHelper.actor.cpp"
