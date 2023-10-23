#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
/*
 * ParallelStream.actor.h
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

#pragma once

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_PARALLEL_STREAM_ACTOR_G_H)
#define FDBCLIENT_PARALLEL_STREAM_ACTOR_G_H
#include "fdbclient/ParallelStream.actor.g.h"
#elif !defined(FDBCLIENT_PARALLEL_STREAM_ACTOR_H)
#define FDBCLIENT_PARALLEL_STREAM_ACTOR_H

#include "flow/genericactors.actor.h"
#include "flow/actorcompiler.h" // must be last include

// ParallelStream is used to fetch data from multiple streams in parallel and then merge them back into a single stream
// in order.
template <class T>
class ParallelStream {
	Reference<BoundedFlowLock> semaphore;
	struct FragmentConstructorTag {
		explicit FragmentConstructorTag() = default;
	};

public:
	// A Fragment is a single stream that will get results to be merged back into the main output stream
	class Fragment : public ReferenceCounted<Fragment> {
		Reference<BoundedFlowLock> semaphore;
		PromiseStream<T> stream;
		BoundedFlowLock::Releaser releaser;
		friend class ParallelStream;

	public:
		Fragment(Reference<BoundedFlowLock> semaphore, int64_t permitNumber, FragmentConstructorTag)
		  : semaphore(semaphore), releaser(semaphore.getPtr(), permitNumber) {}
		template <class U>
		void send(U&& value) {
			stream.send(std::forward<U>(value));
		}
		void sendError(Error e) { stream.sendError(e); }
		void finish() {
			releaser.release(); // Release before destruction to free up pending fragments
			stream.sendError(end_of_stream());
		}
		Future<Void> onEmpty() { return stream.onEmpty(); }
	};

private:
	PromiseStream<Reference<Fragment>> fragments;
	size_t fragmentsProcessed{ 0 };
	PromiseStream<T> results;
	Future<Void> flusher;

public:
	// A background actor which take results from the oldest fragment and sends them to the main output stream
																#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
// This generated class is to be used only via flushToClient()
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
template <class FlushToClientActor>
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
class FlushToClientActorState {
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
public:
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	FlushToClientActorState(ParallelStream<T>* const& self) 
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		 : self(self),
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		   messagesBetweenYields(1000),
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		   messagesSinceYield(0)
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
	{
		fdb_probe_actor_create("flushToClient", reinterpret_cast<unsigned long>(this));

	}
	~FlushToClientActorState() 
	{
		fdb_probe_actor_destroy("flushToClient", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
				;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
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
		this->~FlushToClientActorState();
		static_cast<FlushToClientActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (e.code() == error_code_actor_cancelled)
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
				return a_body1Catch1(e, loopDepth);
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			}
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			self->results.sendError(e);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (!static_cast<FlushToClientActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlushToClientActorState(); static_cast<FlushToClientActor*>(this)->destroy(); return 0; }
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			new (&static_cast<FlushToClientActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlushToClientActorState();
			static_cast<FlushToClientActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		FutureStream<Reference<Fragment>> __when_expr_0 = self->fragments.getFuture();
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<FlushToClientActor*>(this)->actor_wait_state = 1;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >*>(static_cast<FlushToClientActor*>(this)));
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<Fragment> const& __fragment,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		fragment = __fragment;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<Fragment> && __fragment,int loopDepth) 
	{
		fragment = std::move(__fragment);
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state > 0) static_cast<FlushToClientActor*>(this)->actor_wait_state = 0;
		static_cast<FlushToClientActor*>(this)->ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >*,Reference<Fragment> const& value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >*,Reference<Fragment> && value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >*,Error err) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1(int loopDepth) 
	{
		try {
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			StrictFuture<Void> __when_expr_1 = self->results.onEmpty();
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (static_cast<FlushToClientActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1cont1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlushToClientActor*>(this)->actor_wait_state = 2;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlushToClientActor, 1, Void >*>(static_cast<FlushToClientActor*>(this)));
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1cont1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1cont1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (e.code() == error_code_end_of_stream)
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
				fragment.clear();
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
				return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
				return a_body1Catch2(e, std::max(0, loopDepth - 2));
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		FutureStream<T> __when_expr_2 = fragment->stream.getFuture();
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1cont1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont1loopBody1cont2when1(__when_expr_2.pop(), loopDepth); };
		static_cast<FlushToClientActor*>(this)->actor_wait_state = 3;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorSingleCallback< FlushToClientActor, 2, T >*>(static_cast<FlushToClientActor*>(this)));
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		FutureStream<T> __when_expr_2 = fragment->stream.getFuture();
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1cont1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont1loopBody1cont2when1(__when_expr_2.pop(), loopDepth); };
		static_cast<FlushToClientActor*>(this)->actor_wait_state = 3;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorSingleCallback< FlushToClientActor, 2, T >*>(static_cast<FlushToClientActor*>(this)));
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state > 0) static_cast<FlushToClientActor*>(this)->actor_wait_state = 0;
		static_cast<FlushToClientActor*>(this)->ActorCallback< FlushToClientActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlushToClientActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlushToClientActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlushToClientActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont1loopBody1cont3(T const& value,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		self->results.send(value);
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (++messagesSinceYield == messagesBetweenYields)
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		{
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			StrictFuture<Void> __when_expr_3 = yield();
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (static_cast<FlushToClientActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont1loopBody1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<FlushToClientActor*>(this)->actor_wait_state = 4;
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlushToClientActor, 3, Void >*>(static_cast<FlushToClientActor*>(this)));
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont3(T && value,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		self->results.send(value);
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (++messagesSinceYield == messagesBetweenYields)
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		{
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			StrictFuture<Void> __when_expr_3 = yield();
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (static_cast<FlushToClientActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont1loopBody1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<FlushToClientActor*>(this)->actor_wait_state = 4;
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlushToClientActor, 3, Void >*>(static_cast<FlushToClientActor*>(this)));
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont2when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont3(value, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont2when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont3(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state > 0) static_cast<FlushToClientActor*>(this)->actor_wait_state = 0;
		static_cast<FlushToClientActor*>(this)->ActorSingleCallback< FlushToClientActor, 2, T >::remove();

	}
	void a_callback_fire(ActorSingleCallback< FlushToClientActor, 2, T >*,T const& value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorSingleCallback< FlushToClientActor, 2, T >*,T && value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorSingleCallback< FlushToClientActor, 2, T >*,Error err) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont1loopBody1cont4(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		messagesSinceYield = 0;
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		messagesSinceYield = 0;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		loopDepth = a_body1loopBody1cont1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlushToClientActor*>(this)->actor_wait_state > 0) static_cast<FlushToClientActor*>(this)->actor_wait_state = 0;
		static_cast<FlushToClientActor*>(this)->ActorCallback< FlushToClientActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlushToClientActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlushToClientActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlushToClientActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont1loopBody1cont7(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	ParallelStream<T>* self;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	const int messagesBetweenYields;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	int messagesSinceYield;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	Reference<Fragment> fragment;
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
};
// This generated class is to be used only via flushToClient()
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
class FlushToClientActor final : public Actor<Void>, public ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >, public ActorCallback< FlushToClientActor, 1, Void >, public ActorSingleCallback< FlushToClientActor, 2, T >, public ActorCallback< FlushToClientActor, 3, Void >, public FastAllocated<FlushToClientActor>, public FlushToClientActorState<FlushToClientActor> {
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
public:
	using FastAllocated<FlushToClientActor>::operator new;
	using FastAllocated<FlushToClientActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >;
friend struct ActorCallback< FlushToClientActor, 1, Void >;
friend struct ActorSingleCallback< FlushToClientActor, 2, T >;
friend struct ActorCallback< FlushToClientActor, 3, Void >;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	FlushToClientActor(ParallelStream<T>* const& self) 
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		 : Actor<Void>(),
		   FlushToClientActorState<FlushToClientActor>(self)
	{
		fdb_probe_actor_enter("flushToClient", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flushToClient");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flushToClient", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< FlushToClientActor, 0, Reference<Fragment> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlushToClientActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorSingleCallback< FlushToClientActor, 2, T >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlushToClientActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
[[nodiscard]] static Future<Void> flushToClient( ParallelStream<T>* const& self ) {
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	return Future<Void>(new FlushToClientActor(self));
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
}

#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"

	ParallelStream(PromiseStream<T> results, size_t bufferLimit) : results(results) {
		semaphore = makeReference<BoundedFlowLock>(1, bufferLimit);
		flusher = flushToClient(this);
	}

	// Creates a fragment to get merged into the main output stream
																#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
// This generated class is to be used only via createFragmentImpl()
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
template <class CreateFragmentImplActor>
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
class CreateFragmentImplActorState {
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
public:
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	CreateFragmentImplActorState(ParallelStream<T>* const& self) 
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		 : self(self)
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
	{
		fdb_probe_actor_create("createFragmentImpl", reinterpret_cast<unsigned long>(this));

	}
	~CreateFragmentImplActorState() 
	{
		fdb_probe_actor_destroy("createFragmentImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			StrictFuture<int64_t> __when_expr_0 = self->semaphore->take();
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			if (static_cast<CreateFragmentImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateFragmentImplActor*>(this)->actor_wait_state = 1;
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateFragmentImplActor, 0, int64_t >*>(static_cast<CreateFragmentImplActor*>(this)));
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
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
		this->~CreateFragmentImplActorState();
		static_cast<CreateFragmentImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int64_t const& permitNumber,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		auto fragment = makeReference<Fragment>(self->semaphore, permitNumber, FragmentConstructorTag());
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		self->fragments.send(fragment);
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (!static_cast<CreateFragmentImplActor*>(this)->SAV<Fragment*>::futures) { (void)(fragment.getPtr()); this->~CreateFragmentImplActorState(); static_cast<CreateFragmentImplActor*>(this)->destroy(); return 0; }
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		new (&static_cast<CreateFragmentImplActor*>(this)->SAV< Fragment* >::value()) Fragment*(fragment.getPtr());
		this->~CreateFragmentImplActorState();
		static_cast<CreateFragmentImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int64_t && permitNumber,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		auto fragment = makeReference<Fragment>(self->semaphore, permitNumber, FragmentConstructorTag());
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		self->fragments.send(fragment);
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
		if (!static_cast<CreateFragmentImplActor*>(this)->SAV<Fragment*>::futures) { (void)(fragment.getPtr()); this->~CreateFragmentImplActorState(); static_cast<CreateFragmentImplActor*>(this)->destroy(); return 0; }
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		new (&static_cast<CreateFragmentImplActor*>(this)->SAV< Fragment* >::value()) Fragment*(fragment.getPtr());
		this->~CreateFragmentImplActorState();
		static_cast<CreateFragmentImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int64_t const& permitNumber,int loopDepth) 
	{
		loopDepth = a_body1cont1(permitNumber, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int64_t && permitNumber,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(permitNumber), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateFragmentImplActor*>(this)->actor_wait_state > 0) static_cast<CreateFragmentImplActor*>(this)->actor_wait_state = 0;
		static_cast<CreateFragmentImplActor*>(this)->ActorCallback< CreateFragmentImplActor, 0, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< CreateFragmentImplActor, 0, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("createFragmentImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createFragmentImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateFragmentImplActor, 0, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("createFragmentImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createFragmentImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateFragmentImplActor, 0, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("createFragmentImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createFragmentImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	ParallelStream<T>* self;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
};
// This generated class is to be used only via createFragmentImpl()
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
class CreateFragmentImplActor final : public Actor<Fragment*>, public ActorCallback< CreateFragmentImplActor, 0, int64_t >, public FastAllocated<CreateFragmentImplActor>, public CreateFragmentImplActorState<CreateFragmentImplActor> {
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
public:
	using FastAllocated<CreateFragmentImplActor>::operator new;
	using FastAllocated<CreateFragmentImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Fragment*>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateFragmentImplActor, 0, int64_t >;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	CreateFragmentImplActor(ParallelStream<T>* const& self) 
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
		 : Actor<Fragment*>(),
		   CreateFragmentImplActorState<CreateFragmentImplActor>(self)
	{
		fdb_probe_actor_enter("createFragmentImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createFragmentImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createFragmentImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateFragmentImplActor, 0, int64_t >*)0, actor_cancelled()); break;
		}

	}
};
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
[[nodiscard]] static Future<Fragment*> createFragmentImpl( ParallelStream<T>* const& self ) {
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"
	return Future<Fragment*>(new CreateFragmentImplActor(self));
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.g.h"
}

#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ParallelStream.actor.h"

	Future<Fragment*> createFragment() { return createFragmentImpl(this); }

	Future<Void> finish() {
		fragments.sendError(end_of_stream());
		return flusher;
	}
};

#include "flow/unactorcompiler.h"

#endif
