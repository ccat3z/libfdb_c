#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
/*
 * AsyncTaskThread.actor.cpp
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

#include <atomic>

#include "fdbclient/AsyncTaskThread.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

namespace {

class TerminateTask final : public IAsyncTask {
public:
	void operator()() override { ASSERT(false); }
	bool isTerminate() const override { return true; }
};

															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
// This generated class is to be used only via asyncTaskThreadClient()
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
template <class AsyncTaskThreadClientActor>
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
class AsyncTaskThreadClientActorState {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
public:
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	AsyncTaskThreadClientActorState(AsyncTaskThread* const& asyncTaskThread,std::atomic<int>* const& sum,int const& count,int const& clientId,double const& meanSleep) 
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		 : asyncTaskThread(asyncTaskThread),
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   sum(sum),
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   count(count),
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   clientId(clientId),
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   meanSleep(meanSleep),
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   i(0),
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   randomSleep(0.0)
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
	{
		fdb_probe_actor_create("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this));

	}
	~AsyncTaskThreadClientActorState() 
	{
		fdb_probe_actor_destroy("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
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
		this->~AsyncTaskThreadClientActorState();
		static_cast<AsyncTaskThreadClientActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (!static_cast<AsyncTaskThreadClientActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AsyncTaskThreadClientActorState(); static_cast<AsyncTaskThreadClientActor*>(this)->destroy(); return 0; }
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		new (&static_cast<AsyncTaskThreadClientActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AsyncTaskThreadClientActorState();
		static_cast<AsyncTaskThreadClientActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (!(i < count))
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		randomSleep = deterministicRandom()->random01() * 2 * meanSleep;
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(randomSleep);
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state = 1;
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncTaskThreadClientActor, 0, Void >*>(static_cast<AsyncTaskThreadClientActor*>(this)));
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
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
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		StrictFuture<Void> __when_expr_1 = asyncTaskThread->execAsync([sum = sum] { sum->fetch_add(1); return Void(); });
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state = 2;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncTaskThreadClientActor, 1, Void >*>(static_cast<AsyncTaskThreadClientActor*>(this)));
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		StrictFuture<Void> __when_expr_1 = asyncTaskThread->execAsync([sum = sum] { sum->fetch_add(1); return Void(); });
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state = 2;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncTaskThreadClientActor, 1, Void >*>(static_cast<AsyncTaskThreadClientActor*>(this)));
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
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
		if (static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state > 0) static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncTaskThreadClientActor*>(this)->ActorCallback< AsyncTaskThreadClientActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncTaskThreadClientActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AsyncTaskThreadClientActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AsyncTaskThreadClientActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		TraceEvent("AsyncTaskThreadIncrementedSum") .detail("Index", i) .detail("Sum", sum->load()) .detail("ClientId", clientId) .detail("RandomSleep", randomSleep) .detail("MeanSleep", meanSleep);
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		++i;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		TraceEvent("AsyncTaskThreadIncrementedSum") .detail("Index", i) .detail("Sum", sum->load()) .detail("ClientId", clientId) .detail("RandomSleep", randomSleep) .detail("MeanSleep", meanSleep);
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		++i;
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

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
		if (static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state > 0) static_cast<AsyncTaskThreadClientActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncTaskThreadClientActor*>(this)->ActorCallback< AsyncTaskThreadClientActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncTaskThreadClientActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AsyncTaskThreadClientActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AsyncTaskThreadClientActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	AsyncTaskThread* asyncTaskThread;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	std::atomic<int>* sum;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	int count;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	int clientId;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	double meanSleep;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	int i;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	double randomSleep;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
};
// This generated class is to be used only via asyncTaskThreadClient()
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
class AsyncTaskThreadClientActor final : public Actor<Void>, public ActorCallback< AsyncTaskThreadClientActor, 0, Void >, public ActorCallback< AsyncTaskThreadClientActor, 1, Void >, public FastAllocated<AsyncTaskThreadClientActor>, public AsyncTaskThreadClientActorState<AsyncTaskThreadClientActor> {
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
public:
	using FastAllocated<AsyncTaskThreadClientActor>::operator new;
	using FastAllocated<AsyncTaskThreadClientActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncTaskThreadClientActor, 0, Void >;
friend struct ActorCallback< AsyncTaskThreadClientActor, 1, Void >;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	AsyncTaskThreadClientActor(AsyncTaskThread* const& asyncTaskThread,std::atomic<int>* const& sum,int const& count,int const& clientId,double const& meanSleep) 
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		 : Actor<Void>(),
		   AsyncTaskThreadClientActorState<AsyncTaskThreadClientActor>(asyncTaskThread, sum, count, clientId, meanSleep)
	{
		fdb_probe_actor_enter("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("asyncTaskThreadClient");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("asyncTaskThreadClient", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AsyncTaskThreadClientActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AsyncTaskThreadClientActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
[[nodiscard]] Future<Void> asyncTaskThreadClient( AsyncTaskThread* const& asyncTaskThread, std::atomic<int>* const& sum, int const& count, int const& clientId, double const& meanSleep ) {
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	return Future<Void>(new AsyncTaskThreadClientActor(asyncTaskThread, sum, count, clientId, meanSleep));
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
}

#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"

} // namespace

const double AsyncTaskThread::meanDelay = 0.01;

AsyncTaskThread::AsyncTaskThread() : thread([this] { run(this); }) {}

AsyncTaskThread::~AsyncTaskThread() {
	bool wakeUp = false;
	{
		std::lock_guard<std::mutex> g(m);
		wakeUp = queue.push(std::make_unique<TerminateTask>());
	}
	if (wakeUp) {
		cv.notify_one();
	}
	thread.join();
}

void AsyncTaskThread::run(AsyncTaskThread* self) {
	while (true) {
		std::unique_ptr<IAsyncTask> task;
		{
			std::unique_lock<std::mutex> lk(self->m);
			self->cv.wait(lk, [self] { return !self->queue.canSleep(); });
			task = self->queue.pop().get();
			if (task->isTerminate()) {
				return;
			}
		}
		(*task)();
	}
}

															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase92()
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
template <class FlowTestCase92Actor>
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
class FlowTestCase92ActorState {
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
public:
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	FlowTestCase92ActorState(UnitTestParameters const& params) 
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		 : params(params),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   sum(0),
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   asyncTaskThread(),
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   numClients(10),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   incrementsPerClient(100)
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase92", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase92ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase92", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			std::vector<Future<Void>> clients;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			clients.reserve(numClients);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			for(int clientId = 0;clientId < numClients;++clientId) {
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
				clients.push_back(asyncTaskThreadClient( &asyncTaskThread, &sum, incrementsPerClient, clientId, deterministicRandom()->random01() * 0.01));
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
			}
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(clients);
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			if (static_cast<FlowTestCase92Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase92Actor*>(this)->actor_wait_state = 1;
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase92Actor, 0, Void >*>(static_cast<FlowTestCase92Actor*>(this)));
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
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
		this->~FlowTestCase92ActorState();
		static_cast<FlowTestCase92Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		ASSERT_EQ(sum.load(), numClients * incrementsPerClient);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (!static_cast<FlowTestCase92Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase92ActorState(); static_cast<FlowTestCase92Actor*>(this)->destroy(); return 0; }
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		new (&static_cast<FlowTestCase92Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase92ActorState();
		static_cast<FlowTestCase92Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		ASSERT_EQ(sum.load(), numClients * incrementsPerClient);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (!static_cast<FlowTestCase92Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase92ActorState(); static_cast<FlowTestCase92Actor*>(this)->destroy(); return 0; }
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		new (&static_cast<FlowTestCase92Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase92ActorState();
		static_cast<FlowTestCase92Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase92Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase92Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase92Actor*>(this)->ActorCallback< FlowTestCase92Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase92Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase92", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase92", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase92Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase92", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase92", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase92Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase92", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase92", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	UnitTestParameters params;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	std::atomic<int> sum;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	AsyncTaskThread asyncTaskThread;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	int numClients;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	int incrementsPerClient;
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase92()
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
class FlowTestCase92Actor final : public Actor<Void>, public ActorCallback< FlowTestCase92Actor, 0, Void >, public FastAllocated<FlowTestCase92Actor>, public FlowTestCase92ActorState<FlowTestCase92Actor> {
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase92Actor>::operator new;
	using FastAllocated<FlowTestCase92Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase92Actor, 0, Void >;
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	FlowTestCase92Actor(UnitTestParameters const& params) 
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase92ActorState<FlowTestCase92Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase92", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase92");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase92", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase92Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
static Future<Void> flowTestCase92( UnitTestParameters const& params ) {
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	return Future<Void>(new FlowTestCase92Actor(params));
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase92, "/asynctaskthread/add")

#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"

															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase108()
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
template <class FlowTestCase108Actor>
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
class FlowTestCase108ActorState {
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
public:
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	FlowTestCase108ActorState(UnitTestParameters const& params) 
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		 : params(params),
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		   asyncTaskThread()
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase108", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase108ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase108", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
				StrictFuture<Void> __when_expr_0 = asyncTaskThread.execAsync([] { throw operation_failed(); return Void(); });
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
				if (static_cast<FlowTestCase108Actor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase108Actor*>(this)->actor_wait_state = 1;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase108Actor, 0, Void >*>(static_cast<FlowTestCase108Actor*>(this)));
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
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
		this->~FlowTestCase108ActorState();
		static_cast<FlowTestCase108Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		if (!static_cast<FlowTestCase108Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase108ActorState(); static_cast<FlowTestCase108Actor*>(this)->destroy(); return 0; }
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		new (&static_cast<FlowTestCase108Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase108ActorState();
		static_cast<FlowTestCase108Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
			ASSERT_EQ(e.code(), error_code_operation_failed);
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
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
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		ASSERT(false);
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
		ASSERT(false);
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

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
		if (static_cast<FlowTestCase108Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase108Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase108Actor*>(this)->ActorCallback< FlowTestCase108Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase108Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase108", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase108", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase108Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase108", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase108", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase108Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase108", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase108", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
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
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	UnitTestParameters params;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	AsyncTaskThread asyncTaskThread;
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase108()
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
class FlowTestCase108Actor final : public Actor<Void>, public ActorCallback< FlowTestCase108Actor, 0, Void >, public FastAllocated<FlowTestCase108Actor>, public FlowTestCase108ActorState<FlowTestCase108Actor> {
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase108Actor>::operator new;
	using FastAllocated<FlowTestCase108Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase108Actor, 0, Void >;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	FlowTestCase108Actor(UnitTestParameters const& params) 
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase108ActorState<FlowTestCase108Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase108", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase108");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase108", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase108Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
static Future<Void> flowTestCase108( UnitTestParameters const& params ) {
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
	return Future<Void>(new FlowTestCase108Actor(params));
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase108, "/asynctaskthread/error")

#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncTaskThread.actor.cpp"
