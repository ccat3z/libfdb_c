#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
/*
 * IThreadPoolTest.actor.cpp
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

// Thread naming only works on Linux.
#if defined(__linux__)

#include "flow/IThreadPool.h"

#include <pthread.h>
#include <iostream>

#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // has to be last include

void forceLinkIThreadPoolTests() {}

struct ThreadNameReceiver final : IThreadPoolReceiver {
	void init() override {}

	struct GetNameAction final : TypedAction<ThreadNameReceiver, GetNameAction> {
		ThreadReturnPromise<std::string> name;

		double getTimeEstimate() const override { return 3.; }
	};

	void action(GetNameAction& a) {
		pthread_t t = pthread_self();
		const size_t arrayLen = 16;
		char name[arrayLen];
		int err = pthread_getname_np(t, name, arrayLen);
		if (err != 0) {
			std::cout << "Get name failed with error code: " << err << std::endl;
			a.name.sendError(platform_error());
			return;
		}
		std::string s = name;
		ASSERT(a.name.isValid());
		a.name.send(std::move(s));
		ASSERT(!a.name.isValid());
	}
};

															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase60()
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
template <class FlowTestCase60Actor>
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
class FlowTestCase60ActorState {
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
public:
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	FlowTestCase60ActorState(UnitTestParameters const& params) 
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		 : params(params)
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase60", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase60ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase60", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			noUnseed = true;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			pool = createGenericThreadPool();
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			pool->addThread(new ThreadNameReceiver(), "thread-foo");
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			auto* a = new ThreadNameReceiver::GetNameAction();
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			auto fut = a->name.getFuture();
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			pool->post(a);
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			StrictFuture<std::string> __when_expr_0 = fut;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 1;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 0, std::string >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
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
		this->~FlowTestCase60ActorState();
		static_cast<FlowTestCase60Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::string const& name,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (name != "thread-foo")
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			std::cout << "Incorrect thread name: " << name << std::endl;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			ASSERT(false);
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		}
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		StrictFuture<Void> __when_expr_1 = pool->stop();
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 2;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 1, Void >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::string && name,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (name != "thread-foo")
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			std::cout << "Incorrect thread name: " << name << std::endl;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			ASSERT(false);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		}
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		StrictFuture<Void> __when_expr_1 = pool->stop();
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 2;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 1, Void >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::string const& name,int loopDepth) 
	{
		loopDepth = a_body1cont1(name, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::string && name,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(name), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase60Actor*>(this)->ActorCallback< FlowTestCase60Actor, 0, std::string >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 0, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 0, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase60Actor, 0, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (!static_cast<FlowTestCase60Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase60ActorState(); static_cast<FlowTestCase60Actor*>(this)->destroy(); return 0; }
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		new (&static_cast<FlowTestCase60Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase60ActorState();
		static_cast<FlowTestCase60Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (!static_cast<FlowTestCase60Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase60ActorState(); static_cast<FlowTestCase60Actor*>(this)->destroy(); return 0; }
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		new (&static_cast<FlowTestCase60Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase60ActorState();
		static_cast<FlowTestCase60Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase60Actor*>(this)->ActorCallback< FlowTestCase60Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase60Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	UnitTestParameters params;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	Reference<IThreadPool> pool;
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase60()
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
class FlowTestCase60Actor final : public Actor<Void>, public ActorCallback< FlowTestCase60Actor, 0, std::string >, public ActorCallback< FlowTestCase60Actor, 1, Void >, public FastAllocated<FlowTestCase60Actor>, public FlowTestCase60ActorState<FlowTestCase60Actor> {
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase60Actor>::operator new;
	using FastAllocated<FlowTestCase60Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase60Actor, 0, std::string >;
friend struct ActorCallback< FlowTestCase60Actor, 1, Void >;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	FlowTestCase60Actor(UnitTestParameters const& params) 
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase60ActorState<FlowTestCase60Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase60");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase60Actor, 0, std::string >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase60Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
static Future<Void> flowTestCase60( UnitTestParameters const& params ) {
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	return Future<Void>(new FlowTestCase60Actor(params));
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase60, "/flow/IThreadPool/NamedThread")

#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"

struct ThreadSafePromiseStreamSender final : IThreadPoolReceiver {
	ThreadSafePromiseStreamSender(ThreadReturnPromiseStream<std::string>* notifications)
	  : notifications(notifications) {}
	void init() override {}

	struct GetNameAction final : TypedAction<ThreadSafePromiseStreamSender, GetNameAction> {
		double getTimeEstimate() const override { return 3.; }
	};

	void action(GetNameAction& a) {
		pthread_t t = pthread_self();
		const size_t arrayLen = 16;
		char name[arrayLen];
		int err = pthread_getname_np(t, name, arrayLen);
		if (err != 0) {
			std::cout << "Get name failed with error code: " << err << std::endl;
			notifications->sendError(platform_error());
			return;
		}
		notifications->send(name);
	}

	struct FaultyAction final : TypedAction<ThreadSafePromiseStreamSender, FaultyAction> {
		double getTimeEstimate() const override { return 3.; }
	};

	void action(FaultyAction& a) { notifications->sendError(platform_error().asInjectedFault()); }

private:
	ThreadReturnPromiseStream<std::string>* notifications;
};

															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase117()
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
template <class FlowTestCase117Actor>
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
class FlowTestCase117ActorState {
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
public:
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	FlowTestCase117ActorState(UnitTestParameters const& params) 
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		 : params(params)
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase117", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase117ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase117", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			noUnseed = true;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			notifications = std::unique_ptr<ThreadReturnPromiseStream<std::string>>(new ThreadReturnPromiseStream<std::string>());
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			pool = createGenericThreadPool();
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			pool->addThread(new ThreadSafePromiseStreamSender(notifications.get()), "thread-foo");
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			num = 3;
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			for(int i = 0;i < num;++i) {
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
				auto* a = new ThreadSafePromiseStreamSender::GetNameAction();
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
				pool->post(a);
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
			}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			futs = notifications->getFuture();
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			n = 0;
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
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
		this->~FlowTestCase117ActorState();
		static_cast<FlowTestCase117Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		ASSERT(n == num);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		auto* faultyAction = new ThreadSafePromiseStreamSender::FaultyAction();
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		pool->post(faultyAction);
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		try {
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			FutureStream<std::string> __when_expr_1 = futs;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			if (static_cast<FlowTestCase117Actor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.pop(), loopDepth); };
			static_cast<FlowTestCase117Actor*>(this)->actor_wait_state = 2;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase117Actor, 1, std::string >*>(static_cast<FlowTestCase117Actor*>(this)));
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
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
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (!(n < num))
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		FutureStream<std::string> __when_expr_0 = futs;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (static_cast<FlowTestCase117Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<FlowTestCase117Actor*>(this)->actor_wait_state = 1;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< FlowTestCase117Actor, 0, std::string >*>(static_cast<FlowTestCase117Actor*>(this)));
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
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
	int a_body1loopBody1cont1(std::string const& name,int loopDepth) 
	{
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (name != "thread-foo")
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			std::cout << "Incorrect thread name: " << name << std::endl;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			ASSERT(false);
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		}
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		++n;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(std::string && name,int loopDepth) 
	{
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (name != "thread-foo")
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			std::cout << "Incorrect thread name: " << name << std::endl;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			ASSERT(false);
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		}
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		++n;
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::string const& name,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(name, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::string && name,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(name), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase117Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase117Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase117Actor*>(this)->ActorSingleCallback< FlowTestCase117Actor, 0, std::string >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase117Actor, 0, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase117Actor, 0, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase117Actor, 0, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		StrictFuture<Void> __when_expr_2 = pool->stop();
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (static_cast<FlowTestCase117Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase117Actor*>(this)->actor_wait_state = 3;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase117Actor, 2, Void >*>(static_cast<FlowTestCase117Actor*>(this)));
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
			ASSERT(e.isInjectedFault());
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(std::string const& name,int loopDepth) 
	{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		ASSERT(false);
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(std::string && name,int loopDepth) 
	{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		ASSERT(false);
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::string const& name,int loopDepth) 
	{
		loopDepth = a_body1cont4(name, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::string && name,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(name), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase117Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase117Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase117Actor*>(this)->ActorSingleCallback< FlowTestCase117Actor, 1, std::string >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase117Actor, 1, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< FlowTestCase117Actor, 1, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorSingleCallback< FlowTestCase117Actor, 1, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (!static_cast<FlowTestCase117Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase117ActorState(); static_cast<FlowTestCase117Actor*>(this)->destroy(); return 0; }
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		new (&static_cast<FlowTestCase117Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase117ActorState();
		static_cast<FlowTestCase117Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
		if (!static_cast<FlowTestCase117Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase117ActorState(); static_cast<FlowTestCase117Actor*>(this)->destroy(); return 0; }
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		new (&static_cast<FlowTestCase117Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase117ActorState();
		static_cast<FlowTestCase117Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase117Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase117Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase117Actor*>(this)->ActorCallback< FlowTestCase117Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase117Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase117Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase117Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	UnitTestParameters params;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	std::unique_ptr<ThreadReturnPromiseStream<std::string>> notifications;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	Reference<IThreadPool> pool;
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	int num;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	FutureStream<std::string> futs;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	int n;
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase117()
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
class FlowTestCase117Actor final : public Actor<Void>, public ActorSingleCallback< FlowTestCase117Actor, 0, std::string >, public ActorSingleCallback< FlowTestCase117Actor, 1, std::string >, public ActorCallback< FlowTestCase117Actor, 2, Void >, public FastAllocated<FlowTestCase117Actor>, public FlowTestCase117ActorState<FlowTestCase117Actor> {
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase117Actor>::operator new;
	using FastAllocated<FlowTestCase117Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< FlowTestCase117Actor, 0, std::string >;
friend struct ActorSingleCallback< FlowTestCase117Actor, 1, std::string >;
friend struct ActorCallback< FlowTestCase117Actor, 2, Void >;
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	FlowTestCase117Actor(UnitTestParameters const& params) 
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase117ActorState<FlowTestCase117Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase117", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase117");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase117", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< FlowTestCase117Actor, 0, std::string >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorSingleCallback< FlowTestCase117Actor, 1, std::string >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase117Actor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
static Future<Void> flowTestCase117( UnitTestParameters const& params ) {
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"
	return Future<Void>(new FlowTestCase117Actor(params));
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase117, "/flow/IThreadPool/ThreadReturnPromiseStream")

#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/IThreadPoolTest.actor.cpp"

#else
void forceLinkIThreadPoolTests() {}
#endif
