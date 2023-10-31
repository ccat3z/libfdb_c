#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
/*
 * ActorCollection.actor.cpp
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

#include "flow/ActorCollection.h"
#include "flow/IndexedSet.h"
#include "flow/UnitTest.h"
#include <boost/intrusive/list.hpp>
#include "flow/actorcompiler.h" // This must be the last #include.

struct Runner : public boost::intrusive::list_base_hook<>, FastAllocated<Runner>, NonCopyable {
	Future<Void> handler;
};

// An intrusive list of Runners, which are FastAllocated.  Each runner holds a handler future
typedef boost::intrusive::list<Runner, boost::intrusive::constant_time_size<false>> RunnerList;

// The runners list in the ActorCollection must be destroyed when the actor is destructed rather
// than before returning or throwing
struct RunnerListDestroyer : NonCopyable {
	RunnerListDestroyer(RunnerList* list) : list(list) {}

	~RunnerListDestroyer() {
		list->clear_and_dispose([](Runner* r) { delete r; });
	}

	RunnerList* list;
};

															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via runnerHandler()
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class RunnerHandlerActor>
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class RunnerHandlerActorState {
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	RunnerHandlerActorState(PromiseStream<RunnerList::iterator> const& output,PromiseStream<Error> const& errors,Future<Void> const& task,RunnerList::iterator const& runner) 
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		 : output(output),
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   errors(errors),
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   task(task),
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   runner(runner)
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
	{
		fdb_probe_actor_create("runnerHandler", reinterpret_cast<unsigned long>(this));

	}
	~RunnerHandlerActorState() 
	{
		fdb_probe_actor_destroy("runnerHandler", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				StrictFuture<Void> __when_expr_0 = task;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				if (static_cast<RunnerHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<RunnerHandlerActor*>(this)->actor_wait_state = 1;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunnerHandlerActor, 0, Void >*>(static_cast<RunnerHandlerActor*>(this)));
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~RunnerHandlerActorState();
		static_cast<RunnerHandlerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<RunnerHandlerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RunnerHandlerActorState(); static_cast<RunnerHandlerActor*>(this)->destroy(); return 0; }
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<RunnerHandlerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RunnerHandlerActorState();
		static_cast<RunnerHandlerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			errors.send(e);
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		output.send(runner);
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		output.send(runner);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		if (static_cast<RunnerHandlerActor*>(this)->actor_wait_state > 0) static_cast<RunnerHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<RunnerHandlerActor*>(this)->ActorCallback< RunnerHandlerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunnerHandlerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runnerHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runnerHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunnerHandlerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runnerHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runnerHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunnerHandlerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runnerHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runnerHandler", reinterpret_cast<unsigned long>(this), 0);

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
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	PromiseStream<RunnerList::iterator> output;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	PromiseStream<Error> errors;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	Future<Void> task;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	RunnerList::iterator runner;
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
};
// This generated class is to be used only via runnerHandler()
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class RunnerHandlerActor final : public Actor<Void>, public ActorCallback< RunnerHandlerActor, 0, Void >, public FastAllocated<RunnerHandlerActor>, public RunnerHandlerActorState<RunnerHandlerActor> {
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<RunnerHandlerActor>::operator new;
	using FastAllocated<RunnerHandlerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunnerHandlerActor, 0, Void >;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	RunnerHandlerActor(PromiseStream<RunnerList::iterator> const& output,PromiseStream<Error> const& errors,Future<Void> const& task,RunnerList::iterator const& runner) 
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   RunnerHandlerActorState<RunnerHandlerActor>(output, errors, task, runner)
	{
		fdb_probe_actor_enter("runnerHandler", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runnerHandler");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runnerHandler", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunnerHandlerActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
[[nodiscard]] Future<Void> runnerHandler( PromiseStream<RunnerList::iterator> const& output, PromiseStream<Error> const& errors, Future<Void> const& task, RunnerList::iterator const& runner ) {
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new RunnerHandlerActor(output, errors, task, runner));
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}

#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"

															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via actorCollection()
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class ActorCollectionActor>
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class ActorCollectionActorState {
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	ActorCollectionActorState(FutureStream<Future<Void>> const& addActor,int* const& pCount,double* const& lastChangeTime,double* const& idleTime,double* const& allTime,bool const& returnWhenEmptied) 
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		 : addActor(addActor),
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   pCount(pCount),
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   lastChangeTime(lastChangeTime),
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   idleTime(idleTime),
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   allTime(allTime),
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   returnWhenEmptied(returnWhenEmptied),
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   runners(),
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   runnersDestroyer(&runners),
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   complete(),
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   errors(),
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   count(0)
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
	{
		fdb_probe_actor_create("actorCollection", reinterpret_cast<unsigned long>(this));

	}
	~ActorCollectionActorState() 
	{
		fdb_probe_actor_destroy("actorCollection", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (!pCount)
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				pCount = &count;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~ActorCollectionActorState();
		static_cast<ActorCollectionActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		FutureStream<Future<Void>> __when_expr_0 = addActor;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (static_cast<ActorCollectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		FutureStream<RunnerList::iterator> __when_expr_1 = complete.getFuture();
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.pop(), loopDepth); };
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		FutureStream<Error> __when_expr_2 = errors.getFuture();
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when3(__when_expr_2.pop(), loopDepth); };
		static_cast<ActorCollectionActor*>(this)->actor_wait_state = 1;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*>(static_cast<ActorCollectionActor*>(this)));
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >*>(static_cast<ActorCollectionActor*>(this)));
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorSingleCallback< ActorCollectionActor, 2, Error >*>(static_cast<ActorCollectionActor*>(this)));
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Future<Void> const& f,int loopDepth) 
	{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		auto i = runners.insert(runners.end(), *new Runner());
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		Future<Void> handler = runnerHandler(complete, errors, f, i);
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		i->handler = handler;
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		++*pCount;
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (*pCount == 1 && lastChangeTime && idleTime && allTime)
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			double currentTime = now();
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			*idleTime += currentTime - *lastChangeTime;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			*allTime += currentTime - *lastChangeTime;
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			*lastChangeTime = currentTime;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Future<Void> && f,int loopDepth) 
	{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		auto i = runners.insert(runners.end(), *new Runner());
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		Future<Void> handler = runnerHandler(complete, errors, f, i);
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		i->handler = handler;
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		++*pCount;
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (*pCount == 1 && lastChangeTime && idleTime && allTime)
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			double currentTime = now();
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			*idleTime += currentTime - *lastChangeTime;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			*allTime += currentTime - *lastChangeTime;
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			*lastChangeTime = currentTime;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(RunnerList::iterator const& i,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!--*pCount)
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (lastChangeTime && idleTime && allTime)
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				double currentTime = now();
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				*allTime += currentTime - *lastChangeTime;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				*lastChangeTime = currentTime;
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (returnWhenEmptied)
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				if (!static_cast<ActorCollectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorCollectionActorState(); static_cast<ActorCollectionActor*>(this)->destroy(); return 0; }
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
				new (&static_cast<ActorCollectionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ActorCollectionActorState();
				static_cast<ActorCollectionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		runners.erase_and_dispose(i, [](Runner* r) { delete r; });
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(RunnerList::iterator && i,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!--*pCount)
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (lastChangeTime && idleTime && allTime)
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				double currentTime = now();
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				*allTime += currentTime - *lastChangeTime;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				*lastChangeTime = currentTime;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (returnWhenEmptied)
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				if (!static_cast<ActorCollectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorCollectionActorState(); static_cast<ActorCollectionActor*>(this)->destroy(); return 0; }
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
				new (&static_cast<ActorCollectionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ActorCollectionActorState();
				static_cast<ActorCollectionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		runners.erase_and_dispose(i, [](Runner* r) { delete r; });
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(Error const& e,int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1when3(Error && e,int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorCollectionActor*>(this)->actor_wait_state > 0) static_cast<ActorCollectionActor*>(this)->actor_wait_state = 0;
		static_cast<ActorCollectionActor*>(this)->ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >::remove();
		static_cast<ActorCollectionActor*>(this)->ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >::remove();
		static_cast<ActorCollectionActor*>(this)->ActorSingleCallback< ActorCollectionActor, 2, Error >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*,Future<Void> const& value) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*,Future<Void> && value) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >*,RunnerList::iterator const& value) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >*,RunnerList::iterator && value) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >*,Error err) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 2, Error >*,Error const& value) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose1();
		try {
			a_body1loopBody1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 2, Error >*,Error && value) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose1();
		try {
			a_body1loopBody1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorSingleCallback< ActorCollectionActor, 2, Error >*,Error err) 
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FutureStream<Future<Void>> addActor;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	int* pCount;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	double* lastChangeTime;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	double* idleTime;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	double* allTime;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	bool returnWhenEmptied;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	RunnerList runners;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	RunnerListDestroyer runnersDestroyer;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	PromiseStream<RunnerList::iterator> complete;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	PromiseStream<Error> errors;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	int count;
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
};
// This generated class is to be used only via actorCollection()
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class ActorCollectionActor final : public Actor<Void>, public ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >, public ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >, public ActorSingleCallback< ActorCollectionActor, 2, Error >, public FastAllocated<ActorCollectionActor>, public ActorCollectionActorState<ActorCollectionActor> {
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<ActorCollectionActor>::operator new;
	using FastAllocated<ActorCollectionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >;
friend struct ActorSingleCallback< ActorCollectionActor, 1, RunnerList::iterator >;
friend struct ActorSingleCallback< ActorCollectionActor, 2, Error >;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	ActorCollectionActor(FutureStream<Future<Void>> const& addActor,int* const& pCount,double* const& lastChangeTime,double* const& idleTime,double* const& allTime,bool const& returnWhenEmptied) 
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   ActorCollectionActorState<ActorCollectionActor>(addActor, pCount, lastChangeTime, idleTime, allTime, returnWhenEmptied)
	{
		fdb_probe_actor_enter("actorCollection", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorCollection");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorCollection", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
[[nodiscard]] Future<Void> actorCollection( FutureStream<Future<Void>> const& addActor, int* const& pCount, double* const& lastChangeTime, double* const& idleTime, double* const& allTime, bool const& returnWhenEmptied ) {
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new ActorCollectionActor(addActor, pCount, lastChangeTime, idleTime, allTime, returnWhenEmptied));
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}

#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"

template <class T, class U>
struct Traceable<std::pair<T, U>> {
	static constexpr bool value = Traceable<T>::value && Traceable<U>::value;
	static std::string toString(const std::pair<T, U>& p) {
		auto tStr = Traceable<T>::toString(p.first);
		auto uStr = Traceable<U>::toString(p.second);
		std::string result(tStr.size() + uStr.size() + 3, 'x');
		std::copy(tStr.begin(), tStr.end(), result.begin());
		auto iter = result.begin() + tStr.size();
		*(iter++) = ' ';
		*(iter++) = '-';
		*(iter++) = ' ';
		std::copy(uStr.begin(), uStr.end(), iter);
		return result;
	}
};

void forceLinkActorCollectionTests() {}

// The above implementation relies on the behavior that fulfilling a promise
// that another when clause in the same choose block is waiting on is not fired synchronously.
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase132()
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class FlowTestCase132Actor>
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FlowTestCase132ActorState {
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FlowTestCase132ActorState(UnitTestParameters const& params) 
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		 : params(params),
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   promise()
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase132", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase132ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase132", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (static_cast<FlowTestCase132Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			StrictFuture<Void> __when_expr_1 = promise.getFuture();
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase132Actor*>(this)->actor_wait_state = 1;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase132Actor, 0, Void >*>(static_cast<FlowTestCase132Actor*>(this)));
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase132Actor, 1, Void >*>(static_cast<FlowTestCase132Actor*>(this)));
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~FlowTestCase132ActorState();
		static_cast<FlowTestCase132Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FlowTestCase132Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase132ActorState(); static_cast<FlowTestCase132Actor*>(this)->destroy(); return 0; }
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FlowTestCase132Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase132ActorState();
		static_cast<FlowTestCase132Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		promise.send(Void());
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		promise.send(Void());
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		ASSERT(false);
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		ASSERT(false);
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase132Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase132Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase132Actor*>(this)->ActorCallback< FlowTestCase132Actor, 0, Void >::remove();
		static_cast<FlowTestCase132Actor*>(this)->ActorCallback< FlowTestCase132Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase132Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase132Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase132Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase132Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase132Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase132Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	UnitTestParameters params;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	Promise<Void> promise;
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase132()
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FlowTestCase132Actor final : public Actor<Void>, public ActorCallback< FlowTestCase132Actor, 0, Void >, public ActorCallback< FlowTestCase132Actor, 1, Void >, public FastAllocated<FlowTestCase132Actor>, public FlowTestCase132ActorState<FlowTestCase132Actor> {
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase132Actor>::operator new;
	using FastAllocated<FlowTestCase132Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase132Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase132Actor, 1, Void >;
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FlowTestCase132Actor(UnitTestParameters const& params) 
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase132ActorState<FlowTestCase132Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase132", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase132");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase132", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase132Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
static Future<Void> flowTestCase132( UnitTestParameters const& params ) {
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new FlowTestCase132Actor(params));
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase132, "/flow/actorCollection/chooseWhen")

#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"

															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via failIfNotCancelled()
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class FailIfNotCancelledActor>
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FailIfNotCancelledActorState {
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FailIfNotCancelledActorState() 
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	{
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		fdb_probe_actor_create("failIfNotCancelled", reinterpret_cast<unsigned long>(this));

	}
	~FailIfNotCancelledActorState() 
	{
		fdb_probe_actor_destroy("failIfNotCancelled", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (static_cast<FailIfNotCancelledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FailIfNotCancelledActor*>(this)->actor_wait_state = 1;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FailIfNotCancelledActor, 0, Void >*>(static_cast<FailIfNotCancelledActor*>(this)));
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~FailIfNotCancelledActorState();
		static_cast<FailIfNotCancelledActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		ASSERT(false);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FailIfNotCancelledActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FailIfNotCancelledActorState(); static_cast<FailIfNotCancelledActor*>(this)->destroy(); return 0; }
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FailIfNotCancelledActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FailIfNotCancelledActorState();
		static_cast<FailIfNotCancelledActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		ASSERT(false);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FailIfNotCancelledActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FailIfNotCancelledActorState(); static_cast<FailIfNotCancelledActor*>(this)->destroy(); return 0; }
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FailIfNotCancelledActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FailIfNotCancelledActorState();
		static_cast<FailIfNotCancelledActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FailIfNotCancelledActor*>(this)->actor_wait_state > 0) static_cast<FailIfNotCancelledActor*>(this)->actor_wait_state = 0;
		static_cast<FailIfNotCancelledActor*>(this)->ActorCallback< FailIfNotCancelledActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FailIfNotCancelledActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("failIfNotCancelled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("failIfNotCancelled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FailIfNotCancelledActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("failIfNotCancelled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("failIfNotCancelled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FailIfNotCancelledActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("failIfNotCancelled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("failIfNotCancelled", reinterpret_cast<unsigned long>(this), 0);

	}
};
// This generated class is to be used only via failIfNotCancelled()
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FailIfNotCancelledActor final : public Actor<Void>, public ActorCallback< FailIfNotCancelledActor, 0, Void >, public FastAllocated<FailIfNotCancelledActor>, public FailIfNotCancelledActorState<FailIfNotCancelledActor> {
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<FailIfNotCancelledActor>::operator new;
	using FastAllocated<FailIfNotCancelledActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FailIfNotCancelledActor, 0, Void >;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FailIfNotCancelledActor() 
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   FailIfNotCancelledActorState<FailIfNotCancelledActor>()
	{
		fdb_probe_actor_enter("failIfNotCancelled", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("failIfNotCancelled");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("failIfNotCancelled", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FailIfNotCancelledActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
[[nodiscard]] Future<Void> failIfNotCancelled(  ) {
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new FailIfNotCancelledActor());
															#line 1255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}

#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"

// test contract that actors are cancelled when the actor collection is cleared
															#line 1261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase153()
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class FlowTestCase153Actor>
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FlowTestCase153ActorState {
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FlowTestCase153ActorState(UnitTestParameters const& params) 
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		 : params(params),
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   actorCollection(false)
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase153", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase153ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase153", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			int actors = deterministicRandom()->randomInt(1, 1000);
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			for(int i = 0;i < actors;i++) {
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				actorCollection.add(failIfNotCancelled());
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			actorCollection.clear(false);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (static_cast<FlowTestCase153Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase153Actor*>(this)->actor_wait_state = 1;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase153Actor, 0, Void >*>(static_cast<FlowTestCase153Actor*>(this)));
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~FlowTestCase153ActorState();
		static_cast<FlowTestCase153Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FlowTestCase153Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase153ActorState(); static_cast<FlowTestCase153Actor*>(this)->destroy(); return 0; }
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FlowTestCase153Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase153ActorState();
		static_cast<FlowTestCase153Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FlowTestCase153Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase153ActorState(); static_cast<FlowTestCase153Actor*>(this)->destroy(); return 0; }
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FlowTestCase153Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase153ActorState();
		static_cast<FlowTestCase153Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase153Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase153Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase153Actor*>(this)->ActorCallback< FlowTestCase153Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase153Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase153", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase153", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase153Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase153", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase153", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase153Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase153", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase153", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	UnitTestParameters params;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	ActorCollection actorCollection;
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase153()
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FlowTestCase153Actor final : public Actor<Void>, public ActorCallback< FlowTestCase153Actor, 0, Void >, public FastAllocated<FlowTestCase153Actor>, public FlowTestCase153ActorState<FlowTestCase153Actor> {
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase153Actor>::operator new;
	using FastAllocated<FlowTestCase153Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase153Actor, 0, Void >;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FlowTestCase153Actor(UnitTestParameters const& params) 
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase153ActorState<FlowTestCase153Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase153", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase153");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase153", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase153Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
static Future<Void> flowTestCase153( UnitTestParameters const& params ) {
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new FlowTestCase153Actor(params));
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase153, "/flow/actorCollection/testCancel")

#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"

															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via failedActor()
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class FailedActorActor>
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FailedActorActorState {
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FailedActorActorState() 
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	{
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		fdb_probe_actor_create("failedActor", reinterpret_cast<unsigned long>(this));

	}
	~FailedActorActorState() 
	{
		fdb_probe_actor_destroy("failedActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			return a_body1Catch1(operation_failed(), loopDepth);
															#line 1496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~FailedActorActorState();
		static_cast<FailedActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
};
// This generated class is to be used only via failedActor()
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FailedActorActor final : public Actor<Void>, public FastAllocated<FailedActorActor>, public FailedActorActorState<FailedActorActor> {
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<FailedActorActor>::operator new;
	using FastAllocated<FailedActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FailedActorActor() 
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   FailedActorActorState<FailedActorActor>()
	{
		fdb_probe_actor_enter("failedActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("failedActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("failedActor", reinterpret_cast<unsigned long>(this), -1);

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
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
[[nodiscard]] Future<Void> failedActor(  ) {
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new FailedActorActor());
															#line 1555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}

#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"

// test contract that even if the actor collection has stopped and new actors are added to the promise stream, they are
// all cancelled when resetting actor
															#line 1562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase170()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
template <class FlowTestCase170Actor>
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FlowTestCase170ActorState {
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FlowTestCase170ActorState(UnitTestParameters const& params) 
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		 : params(params),
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		   actorCollection(false)
															#line 1578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase170", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase170ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase170", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			int actors = deterministicRandom()->randomInt(1, 500);
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			for(int i = 0;i < actors;i++) {
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				actorCollection.add(failIfNotCancelled());
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			actorCollection.add(failedActor());
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			for(int i = 0;i < actors;i++) {
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
				actorCollection.add(failIfNotCancelled());
															#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			actorCollection = ActorCollection(false);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			if (static_cast<FlowTestCase170Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase170Actor*>(this)->actor_wait_state = 1;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase170Actor, 0, Void >*>(static_cast<FlowTestCase170Actor*>(this)));
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
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
		this->~FlowTestCase170ActorState();
		static_cast<FlowTestCase170Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FlowTestCase170Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase170ActorState(); static_cast<FlowTestCase170Actor*>(this)->destroy(); return 0; }
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FlowTestCase170Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase170ActorState();
		static_cast<FlowTestCase170Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
		if (!static_cast<FlowTestCase170Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase170ActorState(); static_cast<FlowTestCase170Actor*>(this)->destroy(); return 0; }
															#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		new (&static_cast<FlowTestCase170Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase170ActorState();
		static_cast<FlowTestCase170Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase170Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase170Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase170Actor*>(this)->ActorCallback< FlowTestCase170Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase170Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase170", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase170", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase170Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase170", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase170", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase170Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase170", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase170", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	UnitTestParameters params;
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	ActorCollection actorCollection;
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase170()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
class FlowTestCase170Actor final : public Actor<Void>, public ActorCallback< FlowTestCase170Actor, 0, Void >, public FastAllocated<FlowTestCase170Actor>, public FlowTestCase170ActorState<FlowTestCase170Actor> {
															#line 1733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase170Actor>::operator new;
	using FastAllocated<FlowTestCase170Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase170Actor, 0, Void >;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	FlowTestCase170Actor(UnitTestParameters const& params) 
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase170ActorState<FlowTestCase170Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase170", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase170");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase170", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase170Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
static Future<Void> flowTestCase170( UnitTestParameters const& params ) {
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
	return Future<Void>(new FlowTestCase170Actor(params));
															#line 1772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase170, "/flow/actorCollection/testCancelPromiseStream")

#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/ActorCollection.actor.cpp"
