#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
/*
 * VersionedMap.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_VERSIONEDMAP_ACTOR_G_H)
#define FDBCLIENT_VERSIONEDMAP_ACTOR_G_H
#include "fdbclient/VersionedMap.actor.g.h"
#elif !defined(FDBCLIENT_VERSIONEDMAP_ACTOR_H)
#define FDBCLIENT_VERSIONEDMAP_ACTOR_H

#include "flow/flow.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 36 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
namespace {
// This generated class is to be used only via deferredCleanupActor()
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
template <class Tree, class DeferredCleanupActorActor>
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
class DeferredCleanupActorActorState {
															#line 43 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
public:
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
	DeferredCleanupActorActorState(std::vector<Tree> const& toFree,TaskPriority const& taskID = TaskPriority::DefaultYield) 
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		 : toFree(toFree),
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		   taskID(taskID),
															#line 36 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		   freeCount(0)
															#line 54 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
	{
		fdb_probe_actor_create("deferredCleanupActor", reinterpret_cast<unsigned long>(this));

	}
	~DeferredCleanupActorActorState() 
	{
		fdb_probe_actor_destroy("deferredCleanupActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
			;
															#line 69 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
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
		this->~DeferredCleanupActorActorState();
		static_cast<DeferredCleanupActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 50 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		if (!static_cast<DeferredCleanupActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeferredCleanupActorActorState(); static_cast<DeferredCleanupActorActor*>(this)->destroy(); return 0; }
															#line 92 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
		new (&static_cast<DeferredCleanupActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeferredCleanupActorActorState();
		static_cast<DeferredCleanupActorActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 37 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		if (!(!toFree.empty()))
															#line 111 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 38 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		Tree a = std::move(toFree.back());
															#line 39 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		toFree.pop_back();
															#line 41 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		for(int c = 0;c < 3;c++) {
															#line 42 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
			if (a->pointer[c] && a->pointer[c]->isSoleOwner())
															#line 123 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
			{
															#line 43 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
				toFree.push_back(std::move(a->pointer[c]));
															#line 127 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
			}
		}
															#line 46 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
		if (++freeCount % 100 == 0)
															#line 132 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
		{
															#line 47 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
			StrictFuture<Void> __when_expr_0 = yield(taskID);
															#line 47 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
			if (static_cast<DeferredCleanupActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 138 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeferredCleanupActorActor*>(this)->actor_wait_state = 1;
															#line 47 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeferredCleanupActorActor, 0, Void >*>(static_cast<DeferredCleanupActorActor*>(this)));
															#line 143 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
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
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

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
		if (static_cast<DeferredCleanupActorActor*>(this)->actor_wait_state > 0) static_cast<DeferredCleanupActorActor*>(this)->actor_wait_state = 0;
		static_cast<DeferredCleanupActorActor*>(this)->ActorCallback< DeferredCleanupActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeferredCleanupActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deferredCleanupActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deferredCleanupActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeferredCleanupActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deferredCleanupActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deferredCleanupActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeferredCleanupActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deferredCleanupActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deferredCleanupActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
	std::vector<Tree> toFree;
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
	TaskPriority taskID;
															#line 36 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
	int freeCount;
															#line 253 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
};
// This generated class is to be used only via deferredCleanupActor()
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
template <class Tree>
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
class DeferredCleanupActorActor final : public Actor<Void>, public ActorCallback< DeferredCleanupActorActor<Tree>, 0, Void >, public FastAllocated<DeferredCleanupActorActor<Tree>>, public DeferredCleanupActorActorState<Tree, DeferredCleanupActorActor<Tree>> {
															#line 260 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
public:
	using FastAllocated<DeferredCleanupActorActor<Tree>>::operator new;
	using FastAllocated<DeferredCleanupActorActor<Tree>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeferredCleanupActorActor<Tree>, 0, Void >;
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
	DeferredCleanupActorActor(std::vector<Tree> const& toFree,TaskPriority const& taskID = TaskPriority::DefaultYield) 
															#line 271 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
		 : Actor<Void>(),
		   DeferredCleanupActorActorState<Tree, DeferredCleanupActorActor<Tree>>(toFree, taskID)
	{
		fdb_probe_actor_enter("deferredCleanupActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deferredCleanupActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deferredCleanupActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeferredCleanupActorActor<Tree>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
template <class Tree>
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
[[nodiscard]] Future<Void> deferredCleanupActor( std::vector<Tree> const& toFree, TaskPriority const& taskID = TaskPriority::DefaultYield ) {
															#line 34 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"
	return Future<Void>(new DeferredCleanupActorActor<Tree>(toFree, taskID));
															#line 301 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.g.h"
}

#line 52 "/usr/src/libfdb_c/fdbclient/VersionedMap.actor.h"

#include "flow/unactorcompiler.h"
#endif
