#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
/*
 * genericactors.actor.cpp
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

#include "flow/flow.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 27 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via allTrue()
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class AllTrueActor>
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class AllTrueActorState {
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	AllTrueActorState(std::vector<Future<bool>> const& all) 
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : all(all),
															#line 26 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   i(0)
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("allTrue", reinterpret_cast<unsigned long>(this));

	}
	~AllTrueActorState() 
	{
		fdb_probe_actor_destroy("allTrue", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 27 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~AllTrueActorState();
		static_cast<AllTrueActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<AllTrueActor*>(this)->SAV<bool>::futures) { (void)(true); this->~AllTrueActorState(); static_cast<AllTrueActor*>(this)->destroy(); return 0; }
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<AllTrueActor*>(this)->SAV< bool >::value()) bool(true);
		this->~AllTrueActorState();
		static_cast<AllTrueActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 27 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!(i != all.size()))
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<bool> __when_expr_0 = all[i];
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<AllTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AllTrueActor*>(this)->actor_wait_state = 1;
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AllTrueActor, 0, bool >*>(static_cast<AllTrueActor*>(this)));
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
	int a_body1loopBody1cont1(bool const& r,int loopDepth) 
	{
															#line 29 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!r)
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 30 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (!static_cast<AllTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~AllTrueActorState(); static_cast<AllTrueActor*>(this)->destroy(); return 0; }
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<AllTrueActor*>(this)->SAV< bool >::value()) bool(false);
			this->~AllTrueActorState();
			static_cast<AllTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 31 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		i++;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(bool && r,int loopDepth) 
	{
															#line 29 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!r)
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 30 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (!static_cast<AllTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~AllTrueActorState(); static_cast<AllTrueActor*>(this)->destroy(); return 0; }
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<AllTrueActor*>(this)->SAV< bool >::value()) bool(false);
			this->~AllTrueActorState();
			static_cast<AllTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 31 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		i++;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(r, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(r), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AllTrueActor*>(this)->actor_wait_state > 0) static_cast<AllTrueActor*>(this)->actor_wait_state = 0;
		static_cast<AllTrueActor*>(this)->ActorCallback< AllTrueActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< AllTrueActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("allTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AllTrueActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("allTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AllTrueActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("allTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allTrue", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> all;
															#line 26 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int i;
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via allTrue()
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class AllTrueActor final : public Actor<bool>, public ActorCallback< AllTrueActor, 0, bool >, public FastAllocated<AllTrueActor>, public AllTrueActorState<AllTrueActor> {
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<AllTrueActor>::operator new;
	using FastAllocated<AllTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AllTrueActor, 0, bool >;
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	AllTrueActor(std::vector<Future<bool>> const& all) 
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<bool>(),
		   AllTrueActorState<AllTrueActor>(all)
	{
		fdb_probe_actor_enter("allTrue", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("allTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("allTrue", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AllTrueActor, 0, bool >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<bool> allTrue( std::vector<Future<bool>> const& all ) {
															#line 25 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<bool>(new AllTrueActor(all));
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via anyTrue()
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class AnyTrueActor>
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class AnyTrueActorState {
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	AnyTrueActorState(std::vector<Reference<AsyncVar<bool>>> const& input,Reference<AsyncVar<bool>> const& output) 
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : input(input),
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   output(output)
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("anyTrue", reinterpret_cast<unsigned long>(this));

	}
	~AnyTrueActorState() 
	{
		fdb_probe_actor_destroy("anyTrue", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			;
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~AnyTrueActorState();
		static_cast<AnyTrueActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		bool oneTrue = false;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		std::vector<Future<Void>> changes;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		for( auto it : input ) {
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (it->get())
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			{
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				oneTrue = true;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			changes.push_back(it->onChange());
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		}
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		output->set(oneTrue);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = waitForAny(changes);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<AnyTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AnyTrueActor*>(this)->actor_wait_state = 1;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AnyTrueActor, 0, Void >*>(static_cast<AnyTrueActor*>(this)));
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<AnyTrueActor*>(this)->actor_wait_state > 0) static_cast<AnyTrueActor*>(this)->actor_wait_state = 0;
		static_cast<AnyTrueActor*>(this)->ActorCallback< AnyTrueActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AnyTrueActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("anyTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("anyTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AnyTrueActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("anyTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("anyTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AnyTrueActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("anyTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("anyTrue", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Reference<AsyncVar<bool>>> input;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Reference<AsyncVar<bool>> output;
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via anyTrue()
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class AnyTrueActor final : public Actor<Void>, public ActorCallback< AnyTrueActor, 0, Void >, public FastAllocated<AnyTrueActor>, public AnyTrueActorState<AnyTrueActor> {
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<AnyTrueActor>::operator new;
	using FastAllocated<AnyTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AnyTrueActor, 0, Void >;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	AnyTrueActor(std::vector<Reference<AsyncVar<bool>>> const& input,Reference<AsyncVar<bool>> const& output) 
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   AnyTrueActorState<AnyTrueActor>(input, output)
	{
		fdb_probe_actor_enter("anyTrue", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("anyTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("anyTrue", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AnyTrueActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> anyTrue( std::vector<Reference<AsyncVar<bool>>> const& input, Reference<AsyncVar<bool>> const& output ) {
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new AnyTrueActor(input, output));
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via cancelOnly()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class CancelOnlyActor>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class CancelOnlyActorState {
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	CancelOnlyActorState(std::vector<Future<Void>> const& futures) 
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : futures(futures)
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("cancelOnly", reinterpret_cast<unsigned long>(this));

	}
	~CancelOnlyActorState() 
	{
		fdb_probe_actor_destroy("cancelOnly", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Never();
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<CancelOnlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CancelOnlyActor*>(this)->actor_wait_state = 1;
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CancelOnlyActor, 0, Void >*>(static_cast<CancelOnlyActor*>(this)));
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~CancelOnlyActorState();
		static_cast<CancelOnlyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<CancelOnlyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancelOnlyActorState(); static_cast<CancelOnlyActor*>(this)->destroy(); return 0; }
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<CancelOnlyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CancelOnlyActorState();
		static_cast<CancelOnlyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<CancelOnlyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancelOnlyActorState(); static_cast<CancelOnlyActor*>(this)->destroy(); return 0; }
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<CancelOnlyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CancelOnlyActorState();
		static_cast<CancelOnlyActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<CancelOnlyActor*>(this)->actor_wait_state > 0) static_cast<CancelOnlyActor*>(this)->actor_wait_state = 0;
		static_cast<CancelOnlyActor*>(this)->ActorCallback< CancelOnlyActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CancelOnlyActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cancelOnly", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelOnly", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CancelOnlyActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cancelOnly", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelOnly", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CancelOnlyActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cancelOnly", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelOnly", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<Void>> futures;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via cancelOnly()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class CancelOnlyActor final : public Actor<Void>, public ActorCallback< CancelOnlyActor, 0, Void >, public FastAllocated<CancelOnlyActor>, public CancelOnlyActorState<CancelOnlyActor> {
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<CancelOnlyActor>::operator new;
	using FastAllocated<CancelOnlyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CancelOnlyActor, 0, Void >;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	CancelOnlyActor(std::vector<Future<Void>> const& futures) 
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   CancelOnlyActorState<CancelOnlyActor>(futures)
	{
		fdb_probe_actor_enter("cancelOnly", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cancelOnly");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cancelOnly", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CancelOnlyActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> cancelOnly( std::vector<Future<Void>> const& futures ) {
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new CancelOnlyActor(futures));
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via timeoutWarningCollector()
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class TimeoutWarningCollectorActor>
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class TimeoutWarningCollectorActorState {
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	TimeoutWarningCollectorActorState(FutureStream<Void> const& input,double const& logDelay,const char* const& context,UID const& id) 
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : input(input),
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   logDelay(logDelay),
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   context(context),
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   id(id),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   counter(0),
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   end(delay(logDelay))
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("timeoutWarningCollector", reinterpret_cast<unsigned long>(this));

	}
	~TimeoutWarningCollectorActorState() 
	{
		fdb_probe_actor_destroy("timeoutWarningCollector", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			;
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~TimeoutWarningCollectorActorState();
		static_cast<TimeoutWarningCollectorActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		FutureStream<Void> __when_expr_0 = input;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<TimeoutWarningCollectorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = end;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<TimeoutWarningCollectorActor*>(this)->actor_wait_state = 1;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*>(static_cast<TimeoutWarningCollectorActor*>(this)));
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutWarningCollectorActor, 1, Void >*>(static_cast<TimeoutWarningCollectorActor*>(this)));
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		counter++;
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		counter++;
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (counter)
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			TraceEvent(SevWarn, context, id).detail("LateProcessCount", counter).detail("LoggingDelay", logDelay);
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		}
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		end = delay(logDelay);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		counter = 0;
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (counter)
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			TraceEvent(SevWarn, context, id).detail("LateProcessCount", counter).detail("LoggingDelay", logDelay);
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		}
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		end = delay(logDelay);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		counter = 0;
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeoutWarningCollectorActor*>(this)->actor_wait_state > 0) static_cast<TimeoutWarningCollectorActor*>(this)->actor_wait_state = 0;
		static_cast<TimeoutWarningCollectorActor*>(this)->ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >::remove();
		static_cast<TimeoutWarningCollectorActor*>(this)->ActorCallback< TimeoutWarningCollectorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutWarningCollectorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeoutWarningCollectorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeoutWarningCollectorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FutureStream<Void> input;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	double logDelay;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	const char* context;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	UID id;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	uint64_t counter;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Future<Void> end;
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via timeoutWarningCollector()
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class TimeoutWarningCollectorActor final : public Actor<Void>, public ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >, public ActorCallback< TimeoutWarningCollectorActor, 1, Void >, public FastAllocated<TimeoutWarningCollectorActor>, public TimeoutWarningCollectorActorState<TimeoutWarningCollectorActor> {
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<TimeoutWarningCollectorActor>::operator new;
	using FastAllocated<TimeoutWarningCollectorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >;
friend struct ActorCallback< TimeoutWarningCollectorActor, 1, Void >;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	TimeoutWarningCollectorActor(FutureStream<Void> const& input,double const& logDelay,const char* const& context,UID const& id) 
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   TimeoutWarningCollectorActorState<TimeoutWarningCollectorActor>(input, logDelay, context, id)
	{
		fdb_probe_actor_enter("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeoutWarningCollector");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeoutWarningCollector", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> timeoutWarningCollector( FutureStream<Void> const& input, double const& logDelay, const char* const& context, UID const& id ) {
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new TimeoutWarningCollectorActor(input, logDelay, context, id));
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via waitForMost()
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class WaitForMostActor>
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class WaitForMostActorState {
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	WaitForMostActorState(std::vector<Future<ErrorOr<Void>>> const& futures,int const& faultTolerance,Error const& e,double const& waitMultiplierForSlowFutures) 
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : futures(futures),
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   faultTolerance(faultTolerance),
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   e(e),
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   waitMultiplierForSlowFutures(waitMultiplierForSlowFutures),
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   successFutures(),
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   startTime(now())
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("waitForMost", reinterpret_cast<unsigned long>(this));

	}
	~WaitForMostActorState() 
	{
		fdb_probe_actor_destroy("waitForMost", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			successFutures.reserve(futures.size());
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			for( const auto& future : futures ) {
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				successFutures.push_back(fmap([](auto const& result) { return result.present(); }, future));
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			}
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<bool> __when_expr_0 = quorumEqualsTrue(successFutures, successFutures.size() - faultTolerance);
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<WaitForMostActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitForMostActor*>(this)->actor_wait_state = 1;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForMostActor, 0, bool >*>(static_cast<WaitForMostActor*>(this)));
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~WaitForMostActorState();
		static_cast<WaitForMostActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& success,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!success)
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		}
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay((now() - startTime) * waitMultiplierForSlowFutures) || waitForAll(successFutures);
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<WaitForMostActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitForMostActor*>(this)->actor_wait_state = 2;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForMostActor, 1, Void >*>(static_cast<WaitForMostActor*>(this)));
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool && success,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!success)
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		}
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay((now() - startTime) * waitMultiplierForSlowFutures) || waitForAll(successFutures);
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<WaitForMostActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitForMostActor*>(this)->actor_wait_state = 2;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForMostActor, 1, Void >*>(static_cast<WaitForMostActor*>(this)));
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(bool const& success,int loopDepth) 
	{
		loopDepth = a_body1cont1(success, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && success,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(success), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitForMostActor*>(this)->actor_wait_state > 0) static_cast<WaitForMostActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForMostActor*>(this)->ActorCallback< WaitForMostActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForMostActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForMostActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForMostActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<WaitForMostActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForMostActorState(); static_cast<WaitForMostActor*>(this)->destroy(); return 0; }
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<WaitForMostActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitForMostActorState();
		static_cast<WaitForMostActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<WaitForMostActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForMostActorState(); static_cast<WaitForMostActor*>(this)->destroy(); return 0; }
															#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<WaitForMostActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitForMostActorState();
		static_cast<WaitForMostActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WaitForMostActor*>(this)->actor_wait_state > 0) static_cast<WaitForMostActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForMostActor*>(this)->ActorCallback< WaitForMostActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForMostActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitForMostActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitForMostActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<ErrorOr<Void>>> futures;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int faultTolerance;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Error e;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	double waitMultiplierForSlowFutures;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> successFutures;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	double startTime;
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via waitForMost()
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class WaitForMostActor final : public Actor<Void>, public ActorCallback< WaitForMostActor, 0, bool >, public ActorCallback< WaitForMostActor, 1, Void >, public FastAllocated<WaitForMostActor>, public WaitForMostActorState<WaitForMostActor> {
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<WaitForMostActor>::operator new;
	using FastAllocated<WaitForMostActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForMostActor, 0, bool >;
friend struct ActorCallback< WaitForMostActor, 1, Void >;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	WaitForMostActor(std::vector<Future<ErrorOr<Void>>> const& futures,int const& faultTolerance,Error const& e,double const& waitMultiplierForSlowFutures) 
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   WaitForMostActorState<WaitForMostActor>(futures, faultTolerance, e, waitMultiplierForSlowFutures)
	{
		fdb_probe_actor_enter("waitForMost", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForMost");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForMost", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForMostActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitForMostActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> waitForMost( std::vector<Future<ErrorOr<Void>>> const& futures, int const& faultTolerance, Error const& e, double const& waitMultiplierForSlowFutures ) {
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new WaitForMostActor(futures, faultTolerance, e, waitMultiplierForSlowFutures));
															#line 1340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via quorumEqualsTrue()
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class QuorumEqualsTrueActor>
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class QuorumEqualsTrueActorState {
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	QuorumEqualsTrueActorState(std::vector<Future<bool>> const& futures,int const& required) 
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : futures(futures),
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   required(required),
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   true_futures(),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   false_futures()
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("quorumEqualsTrue", reinterpret_cast<unsigned long>(this));

	}
	~QuorumEqualsTrueActorState() 
	{
		fdb_probe_actor_destroy("quorumEqualsTrue", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			true_futures.reserve(futures.size());
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			false_futures.reserve(futures.size());
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			for(int i = 0;i < futures.size();i++) {
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				true_futures.push_back(onEqual(futures[i], true));
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				false_futures.push_back(onEqual(futures[i], false));
															#line 1388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			}
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = quorum(true_futures, required);
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<QuorumEqualsTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_1 = quorum(false_futures, futures.size() - required + 1);
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<QuorumEqualsTrueActor*>(this)->actor_wait_state = 1;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< QuorumEqualsTrueActor, 0, Void >*>(static_cast<QuorumEqualsTrueActor*>(this)));
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< QuorumEqualsTrueActor, 1, Void >*>(static_cast<QuorumEqualsTrueActor*>(this)));
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(true); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(true);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(true); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(true);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(false);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(false);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<QuorumEqualsTrueActor*>(this)->actor_wait_state > 0) static_cast<QuorumEqualsTrueActor*>(this)->actor_wait_state = 0;
		static_cast<QuorumEqualsTrueActor*>(this)->ActorCallback< QuorumEqualsTrueActor, 0, Void >::remove();
		static_cast<QuorumEqualsTrueActor*>(this)->ActorCallback< QuorumEqualsTrueActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< QuorumEqualsTrueActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< QuorumEqualsTrueActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> futures;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int required;
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<Void>> true_futures;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<Void>> false_futures;
															#line 1577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via quorumEqualsTrue()
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class QuorumEqualsTrueActor final : public Actor<bool>, public ActorCallback< QuorumEqualsTrueActor, 0, Void >, public ActorCallback< QuorumEqualsTrueActor, 1, Void >, public FastAllocated<QuorumEqualsTrueActor>, public QuorumEqualsTrueActorState<QuorumEqualsTrueActor> {
															#line 1582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<QuorumEqualsTrueActor>::operator new;
	using FastAllocated<QuorumEqualsTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< QuorumEqualsTrueActor, 0, Void >;
friend struct ActorCallback< QuorumEqualsTrueActor, 1, Void >;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	QuorumEqualsTrueActor(std::vector<Future<bool>> const& futures,int const& required) 
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<bool>(),
		   QuorumEqualsTrueActorState<QuorumEqualsTrueActor>(futures, required)
	{
		fdb_probe_actor_enter("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("quorumEqualsTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("quorumEqualsTrue", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< QuorumEqualsTrueActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<bool> quorumEqualsTrue( std::vector<Future<bool>> const& futures, int const& required ) {
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<bool>(new QuorumEqualsTrueActor(futures, required));
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 1627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via shortCircuitAny()
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class ShortCircuitAnyActor>
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class ShortCircuitAnyActorState {
															#line 1634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	ShortCircuitAnyActorState(std::vector<Future<bool>> const& f) 
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : f(f)
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("shortCircuitAny", reinterpret_cast<unsigned long>(this));

	}
	~ShortCircuitAnyActorState() 
	{
		fdb_probe_actor_destroy("shortCircuitAny", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			std::vector<Future<Void>> sc;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			sc.reserve(f.size());
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			for( Future<bool> fut : f ) {
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				sc.push_back(returnIfTrue(fut));
															#line 1662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(f);
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<ShortCircuitAnyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_1 = waitForAny(sc);
															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ShortCircuitAnyActor*>(this)->actor_wait_state = 1;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ShortCircuitAnyActor, 0, Void >*>(static_cast<ShortCircuitAnyActor*>(this)));
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ShortCircuitAnyActor, 1, Void >*>(static_cast<ShortCircuitAnyActor*>(this)));
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		for( const auto& fut : f ) {
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (fut.get())
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			{
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
				new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
				this->~ShortCircuitAnyActorState();
				static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		for( const auto& fut : f ) {
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (fut.get())
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			{
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
				new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
				this->~ShortCircuitAnyActorState();
				static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ShortCircuitAnyActor*>(this)->actor_wait_state > 0) static_cast<ShortCircuitAnyActor*>(this)->actor_wait_state = 0;
		static_cast<ShortCircuitAnyActor*>(this)->ActorCallback< ShortCircuitAnyActor, 0, Void >::remove();
		static_cast<ShortCircuitAnyActor*>(this)->ActorCallback< ShortCircuitAnyActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ShortCircuitAnyActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ShortCircuitAnyActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> f;
															#line 1875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via shortCircuitAny()
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class ShortCircuitAnyActor final : public Actor<bool>, public ActorCallback< ShortCircuitAnyActor, 0, Void >, public ActorCallback< ShortCircuitAnyActor, 1, Void >, public FastAllocated<ShortCircuitAnyActor>, public ShortCircuitAnyActorState<ShortCircuitAnyActor> {
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<ShortCircuitAnyActor>::operator new;
	using FastAllocated<ShortCircuitAnyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ShortCircuitAnyActor, 0, Void >;
friend struct ActorCallback< ShortCircuitAnyActor, 1, Void >;
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	ShortCircuitAnyActor(std::vector<Future<bool>> const& f) 
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<bool>(),
		   ShortCircuitAnyActorState<ShortCircuitAnyActor>(f)
	{
		fdb_probe_actor_enter("shortCircuitAny", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("shortCircuitAny");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("shortCircuitAny", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ShortCircuitAnyActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<bool> shortCircuitAny( std::vector<Future<bool>> const& f ) {
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<bool>(new ShortCircuitAnyActor(f));
															#line 1920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

Future<Void> orYield(Future<Void> f) {
	if (f.isReady()) {
		if (f.isError())
			return tagError<Void>(yield(), f.getError());
		else
			return yield();
	} else
		return f;
}

															#line 1935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via returnIfTrue()
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class ReturnIfTrueActor>
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class ReturnIfTrueActorState {
															#line 1942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	ReturnIfTrueActorState(Future<bool> const& f) 
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : f(f)
															#line 1949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("returnIfTrue", reinterpret_cast<unsigned long>(this));

	}
	~ReturnIfTrueActorState() 
	{
		fdb_probe_actor_destroy("returnIfTrue", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<bool> __when_expr_0 = f;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 1;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReturnIfTrueActor, 0, bool >*>(static_cast<ReturnIfTrueActor*>(this)));
															#line 1971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~ReturnIfTrueActorState();
		static_cast<ReturnIfTrueActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& b,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (b)
															#line 1994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (!static_cast<ReturnIfTrueActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReturnIfTrueActorState(); static_cast<ReturnIfTrueActor*>(this)->destroy(); return 0; }
															#line 1998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<ReturnIfTrueActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReturnIfTrueActorState();
			static_cast<ReturnIfTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = Never();
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 2;
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReturnIfTrueActor, 1, Void >*>(static_cast<ReturnIfTrueActor*>(this)));
															#line 2013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool && b,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (b)
															#line 2022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (!static_cast<ReturnIfTrueActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReturnIfTrueActorState(); static_cast<ReturnIfTrueActor*>(this)->destroy(); return 0; }
															#line 2026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<ReturnIfTrueActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReturnIfTrueActorState();
			static_cast<ReturnIfTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = Never();
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 2;
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReturnIfTrueActor, 1, Void >*>(static_cast<ReturnIfTrueActor*>(this)));
															#line 2041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(bool const& b,int loopDepth) 
	{
		loopDepth = a_body1cont1(b, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && b,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(b), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state > 0) static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 0;
		static_cast<ReturnIfTrueActor*>(this)->ActorCallback< ReturnIfTrueActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< ReturnIfTrueActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReturnIfTrueActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReturnIfTrueActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 2113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 2121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"

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
		if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state > 0) static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 0;
		static_cast<ReturnIfTrueActor*>(this)->ActorCallback< ReturnIfTrueActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReturnIfTrueActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReturnIfTrueActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReturnIfTrueActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Future<bool> f;
															#line 2190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via returnIfTrue()
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class ReturnIfTrueActor final : public Actor<Void>, public ActorCallback< ReturnIfTrueActor, 0, bool >, public ActorCallback< ReturnIfTrueActor, 1, Void >, public FastAllocated<ReturnIfTrueActor>, public ReturnIfTrueActorState<ReturnIfTrueActor> {
															#line 2195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<ReturnIfTrueActor>::operator new;
	using FastAllocated<ReturnIfTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReturnIfTrueActor, 0, bool >;
friend struct ActorCallback< ReturnIfTrueActor, 1, Void >;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	ReturnIfTrueActor(Future<bool> const& f) 
															#line 2207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   ReturnIfTrueActorState<ReturnIfTrueActor>(f)
	{
		fdb_probe_actor_enter("returnIfTrue", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("returnIfTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("returnIfTrue", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReturnIfTrueActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReturnIfTrueActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> returnIfTrue( Future<bool> const& f ) {
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new ReturnIfTrueActor(f));
															#line 2236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 2241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via lowPriorityDelay()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class LowPriorityDelayActor>
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class LowPriorityDelayActorState {
															#line 2248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	LowPriorityDelayActorState(double const& waitTime) 
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : waitTime(waitTime),
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   loopCount(0),
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   totalLoops(std::max<int>(waitTime / FLOW_KNOBS->LOW_PRIORITY_MAX_DELAY, FLOW_KNOBS->LOW_PRIORITY_DELAY_COUNT))
															#line 2259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("lowPriorityDelay", reinterpret_cast<unsigned long>(this));

	}
	~LowPriorityDelayActorState() 
	{
		fdb_probe_actor_destroy("lowPriorityDelay", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			;
															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~LowPriorityDelayActorState();
		static_cast<LowPriorityDelayActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<LowPriorityDelayActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LowPriorityDelayActorState(); static_cast<LowPriorityDelayActor*>(this)->destroy(); return 0; }
															#line 2297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<LowPriorityDelayActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~LowPriorityDelayActorState();
		static_cast<LowPriorityDelayActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!(loopCount < totalLoops))
															#line 2316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(waitTime / totalLoops, TaskPriority::Low);
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<LowPriorityDelayActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<LowPriorityDelayActor*>(this)->actor_wait_state = 1;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LowPriorityDelayActor, 0, Void >*>(static_cast<LowPriorityDelayActor*>(this)));
															#line 2329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		loopCount++;
															#line 2351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		loopCount++;
															#line 2360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		if (static_cast<LowPriorityDelayActor*>(this)->actor_wait_state > 0) static_cast<LowPriorityDelayActor*>(this)->actor_wait_state = 0;
		static_cast<LowPriorityDelayActor*>(this)->ActorCallback< LowPriorityDelayActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LowPriorityDelayActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lowPriorityDelay", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lowPriorityDelay", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LowPriorityDelayActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lowPriorityDelay", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lowPriorityDelay", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LowPriorityDelayActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lowPriorityDelay", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lowPriorityDelay", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	double waitTime;
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int loopCount;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int totalLoops;
															#line 2434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via lowPriorityDelay()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class LowPriorityDelayActor final : public Actor<Void>, public ActorCallback< LowPriorityDelayActor, 0, Void >, public FastAllocated<LowPriorityDelayActor>, public LowPriorityDelayActorState<LowPriorityDelayActor> {
															#line 2439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<LowPriorityDelayActor>::operator new;
	using FastAllocated<LowPriorityDelayActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LowPriorityDelayActor, 0, Void >;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	LowPriorityDelayActor(double const& waitTime) 
															#line 2450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   LowPriorityDelayActorState<LowPriorityDelayActor>(waitTime)
	{
		fdb_probe_actor_enter("lowPriorityDelay", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lowPriorityDelay");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lowPriorityDelay", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LowPriorityDelayActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> lowPriorityDelay( double const& waitTime ) {
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new LowPriorityDelayActor(waitTime));
															#line 2478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

namespace {

struct DummyState {
	int changed{ 0 };
	int unchanged{ 0 };
	bool operator==(DummyState const& rhs) const { return changed == rhs.changed && unchanged == rhs.unchanged; }
	bool operator!=(DummyState const& rhs) const { return !(*this == rhs); }
};

															#line 2492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
// This generated class is to be used only via testPublisher()
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class TestPublisherActor>
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class TestPublisherActorState {
															#line 2498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	TestPublisherActorState(Reference<AsyncVar<DummyState>> const& input) 
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : input(input),
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   i(0)
															#line 2507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("testPublisher", reinterpret_cast<unsigned long>(this));

	}
	~TestPublisherActorState() 
	{
		fdb_probe_actor_destroy("testPublisher", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			;
															#line 2522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~TestPublisherActorState();
		static_cast<TestPublisherActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<TestPublisherActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestPublisherActorState(); static_cast<TestPublisherActor*>(this)->destroy(); return 0; }
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<TestPublisherActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestPublisherActorState();
		static_cast<TestPublisherActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!(i < 100))
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(deterministicRandom()->random01());
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<TestPublisherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TestPublisherActor*>(this)->actor_wait_state = 1;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestPublisherActor, 0, Void >*>(static_cast<TestPublisherActor*>(this)));
															#line 2577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		auto var = input->get();
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		++var.changed;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		input->set(var);
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		++i;
															#line 2605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		auto var = input->get();
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		++var.changed;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		input->set(var);
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		++i;
															#line 2620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		if (static_cast<TestPublisherActor*>(this)->actor_wait_state > 0) static_cast<TestPublisherActor*>(this)->actor_wait_state = 0;
		static_cast<TestPublisherActor*>(this)->ActorCallback< TestPublisherActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestPublisherActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testPublisher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testPublisher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TestPublisherActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testPublisher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testPublisher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TestPublisherActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testPublisher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testPublisher", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Reference<AsyncVar<DummyState>> input;
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int i;
															#line 2692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via testPublisher()
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class TestPublisherActor final : public Actor<Void>, public ActorCallback< TestPublisherActor, 0, Void >, public FastAllocated<TestPublisherActor>, public TestPublisherActorState<TestPublisherActor> {
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<TestPublisherActor>::operator new;
	using FastAllocated<TestPublisherActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestPublisherActor, 0, Void >;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	TestPublisherActor(Reference<AsyncVar<DummyState>> const& input) 
															#line 2708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   TestPublisherActorState<TestPublisherActor>(input)
	{
		fdb_probe_actor_enter("testPublisher", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testPublisher");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testPublisher", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TestPublisherActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> testPublisher( Reference<AsyncVar<DummyState>> const& input ) {
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new TestPublisherActor(input));
															#line 2735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 2740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
// This generated class is to be used only via testSubscriber()
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class TestSubscriberActor>
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class TestSubscriberActorState {
															#line 2746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	TestSubscriberActorState(Reference<IAsyncListener<int>> const& output,Optional<int> const& expected) 
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : output(output),
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   expected(expected)
															#line 2755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("testSubscriber", reinterpret_cast<unsigned long>(this));

	}
	~TestSubscriberActorState() 
	{
		fdb_probe_actor_destroy("testSubscriber", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			;
															#line 2770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~TestSubscriberActorState();
		static_cast<TestSubscriberActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = output->onChange();
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (static_cast<TestSubscriberActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TestSubscriberActor*>(this)->actor_wait_state = 1;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestSubscriberActor, 0, Void >*>(static_cast<TestSubscriberActor*>(this)));
															#line 2807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(expected.present());
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (output->get() == expected.get())
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (!static_cast<TestSubscriberActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestSubscriberActorState(); static_cast<TestSubscriberActor*>(this)->destroy(); return 0; }
															#line 2822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<TestSubscriberActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TestSubscriberActorState();
			static_cast<TestSubscriberActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(expected.present());
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (output->get() == expected.get())
															#line 2838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (!static_cast<TestSubscriberActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestSubscriberActorState(); static_cast<TestSubscriberActor*>(this)->destroy(); return 0; }
															#line 2842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<TestSubscriberActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TestSubscriberActorState();
			static_cast<TestSubscriberActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<TestSubscriberActor*>(this)->actor_wait_state > 0) static_cast<TestSubscriberActor*>(this)->actor_wait_state = 0;
		static_cast<TestSubscriberActor*>(this)->ActorCallback< TestSubscriberActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestSubscriberActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testSubscriber", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testSubscriber", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TestSubscriberActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testSubscriber", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testSubscriber", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TestSubscriberActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testSubscriber", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testSubscriber", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Reference<IAsyncListener<int>> output;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Optional<int> expected;
															#line 2919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via testSubscriber()
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class TestSubscriberActor final : public Actor<Void>, public ActorCallback< TestSubscriberActor, 0, Void >, public FastAllocated<TestSubscriberActor>, public TestSubscriberActorState<TestSubscriberActor> {
															#line 2924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<TestSubscriberActor>::operator new;
	using FastAllocated<TestSubscriberActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestSubscriberActor, 0, Void >;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	TestSubscriberActor(Reference<IAsyncListener<int>> const& output,Optional<int> const& expected) 
															#line 2935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   TestSubscriberActorState<TestSubscriberActor>(output, expected)
	{
		fdb_probe_actor_enter("testSubscriber", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testSubscriber");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testSubscriber", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TestSubscriberActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> testSubscriber( Reference<IAsyncListener<int>> const& output, Optional<int> const& expected ) {
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new TestSubscriberActor(output, expected));
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}

#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

static Future<ErrorOr<Void>> goodTestFuture(double duration) {
	return tag(delay(duration), ErrorOr<Void>(Void()));
}

static Future<ErrorOr<Void>> badTestFuture(double duration, Error e) {
	return tag(delay(duration), ErrorOr<Void>(e));
}

} // namespace

															#line 2977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase207()
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class FlowTestCase207Actor>
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class FlowTestCase207ActorState {
															#line 2984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FlowTestCase207ActorState(UnitTestParameters const& params) 
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : params(params)
															#line 2991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase207", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase207ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase207", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			auto input = makeReference<AsyncVar<DummyState>>();
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			subscriber1 = testSubscriber(IAsyncListener<int>::create(input, [](auto const& var) { return var.changed; }), 100);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			subscriber2 = testSubscriber(IAsyncListener<int>::create(input, [](auto const& var) { return var.unchanged; }), {});
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = subscriber1 && testPublisher(input);
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<FlowTestCase207Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase207Actor*>(this)->actor_wait_state = 1;
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase207Actor, 0, Void >*>(static_cast<FlowTestCase207Actor*>(this)));
															#line 3019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~FlowTestCase207ActorState();
		static_cast<FlowTestCase207Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(!subscriber2.isReady());
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase207Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase207ActorState(); static_cast<FlowTestCase207Actor*>(this)->destroy(); return 0; }
															#line 3044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase207Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase207ActorState();
		static_cast<FlowTestCase207Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(!subscriber2.isReady());
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase207Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase207ActorState(); static_cast<FlowTestCase207Actor*>(this)->destroy(); return 0; }
															#line 3058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase207Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase207ActorState();
		static_cast<FlowTestCase207Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase207Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase207Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase207Actor*>(this)->ActorCallback< FlowTestCase207Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase207Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase207", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase207", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase207Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase207", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase207", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase207Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase207", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase207", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	UnitTestParameters params;
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Future<Void> subscriber1;
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Future<Void> subscriber2;
															#line 3135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase207()
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class FlowTestCase207Actor final : public Actor<Void>, public ActorCallback< FlowTestCase207Actor, 0, Void >, public FastAllocated<FlowTestCase207Actor>, public FlowTestCase207ActorState<FlowTestCase207Actor> {
															#line 3140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase207Actor>::operator new;
	using FastAllocated<FlowTestCase207Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase207Actor, 0, Void >;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FlowTestCase207Actor(UnitTestParameters const& params) 
															#line 3151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase207ActorState<FlowTestCase207Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase207", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase207");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase207", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase207Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
static Future<Void> flowTestCase207( UnitTestParameters const& params ) {
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new FlowTestCase207Actor(params));
															#line 3179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase207, "/flow/genericactors/AsyncListener")

#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

															#line 3185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase218()
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class FlowTestCase218Actor>
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class FlowTestCase218ActorState {
															#line 3192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FlowTestCase218ActorState(UnitTestParameters const& params) 
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : params(params),
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   futures()
															#line 3201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase218", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase218ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase218", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				futures = { goodTestFuture(1), goodTestFuture(2), goodTestFuture(3) };
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitForMost(futures, 1, operation_failed(), 0.0);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 1;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase218Actor, 0, Void >*>(static_cast<FlowTestCase218Actor*>(this)));
															#line 3226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~FlowTestCase218ActorState();
		static_cast<FlowTestCase218Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		{
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			futures = { goodTestFuture(1), goodTestFuture(2), goodTestFuture(3) };
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_1 = waitForMost(futures, 0, operation_failed(), 0.0);
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 2;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase218Actor, 1, Void >*>(static_cast<FlowTestCase218Actor*>(this)));
															#line 3260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(!futures[2].isReady());
															#line 3270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(!futures[2].isReady());
															#line 3279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase218Actor*>(this)->ActorCallback< FlowTestCase218Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase218Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
	{
		{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			futures = { goodTestFuture(1), goodTestFuture(2), goodTestFuture(3) };
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_2 = waitForMost(futures, 1, operation_failed(), 1.0);
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 3;
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase218Actor, 2, Void >*>(static_cast<FlowTestCase218Actor*>(this)));
															#line 3361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(futures[2].isReady());
															#line 3371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(futures[2].isReady());
															#line 3380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase218Actor*>(this)->ActorCallback< FlowTestCase218Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase218Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(int loopDepth) 
	{
		{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			futures = { goodTestFuture(1), goodTestFuture(2), badTestFuture(1, success()) };
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_3 = waitForMost(futures, 1, operation_failed(), 1.0);
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont7when1(__when_expr_3.get(), loopDepth); };
			static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 4;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase218Actor, 3, Void >*>(static_cast<FlowTestCase218Actor*>(this)));
															#line 3462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(futures[2].isReady());
															#line 3472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(futures[2].isReady());
															#line 3481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase218Actor*>(this)->ActorCallback< FlowTestCase218Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase218Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont10(int loopDepth) 
	{
		{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			futures = { goodTestFuture(1), goodTestFuture(2), badTestFuture(1, success()) };
															#line 3554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			try {
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				StrictFuture<Void> __when_expr_4 = waitForMost(futures, 0, operation_failed(), 1.0);
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state < 0) return a_body1cont10Catch1(actor_cancelled(), loopDepth);
															#line 3560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont10Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont10when1(__when_expr_4.get(), loopDepth); };
				static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 5;
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase218Actor, 4, Void >*>(static_cast<FlowTestCase218Actor*>(this)));
															#line 3565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1cont10Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1cont10Catch1(unknown_error(), loopDepth);
			}
		}

		return loopDepth;
	}
	int a_body1cont11(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont11(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont11(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont11(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase218Actor*>(this)->ActorCallback< FlowTestCase218Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase218Actor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont10cont1(int loopDepth) 
	{
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase218Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase218ActorState(); static_cast<FlowTestCase218Actor*>(this)->destroy(); return 0; }
															#line 3656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase218Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase218ActorState();
		static_cast<FlowTestCase218Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont2(int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			ASSERT_EQ(e.code(), error_code_operation_failed);
															#line 3675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			loopDepth = a_body1cont10cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont3(Void const& _,int loopDepth) 
	{
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(false);
															#line 3690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont10cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont3(Void && _,int loopDepth) 
	{
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(false);
															#line 3699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1cont10cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<FlowTestCase218Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase218Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase218Actor*>(this)->ActorCallback< FlowTestCase218Actor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont10Catch1(error, 0);
		} catch (...) {
			a_body1cont10Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< FlowTestCase218Actor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont10Catch1(error, 0);
		} catch (...) {
			a_body1cont10Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< FlowTestCase218Actor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont10Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont10Catch1(error, 0);
		} catch (...) {
			a_body1cont10Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont10cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont10cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	UnitTestParameters params;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<Future<ErrorOr<Void>>> futures;
															#line 3784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase218()
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class FlowTestCase218Actor final : public Actor<Void>, public ActorCallback< FlowTestCase218Actor, 0, Void >, public ActorCallback< FlowTestCase218Actor, 1, Void >, public ActorCallback< FlowTestCase218Actor, 2, Void >, public ActorCallback< FlowTestCase218Actor, 3, Void >, public ActorCallback< FlowTestCase218Actor, 4, Void >, public FastAllocated<FlowTestCase218Actor>, public FlowTestCase218ActorState<FlowTestCase218Actor> {
															#line 3789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase218Actor>::operator new;
	using FastAllocated<FlowTestCase218Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase218Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase218Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase218Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase218Actor, 3, Void >;
friend struct ActorCallback< FlowTestCase218Actor, 4, Void >;
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FlowTestCase218Actor(UnitTestParameters const& params) 
															#line 3804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase218ActorState<FlowTestCase218Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase218", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase218");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase218", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase218Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase218Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase218Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase218Actor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< FlowTestCase218Actor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
static Future<Void> flowTestCase218( UnitTestParameters const& params ) {
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new FlowTestCase218Actor(params));
															#line 3836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase218, "/flow/genericactors/WaitForMost")

#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"

#if false
															#line 3843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase252()
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
template <class FlowTestCase252Actor>
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class FlowTestCase252ActorState {
															#line 3850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FlowTestCase252ActorState(UnitTestParameters const& params) 
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		 : params(params),
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   resA(),
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   resB(),
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   resC(),
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		   promise()
															#line 3865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase252", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase252ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase252", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			auto future = storeTuple(promise.getFuture(), resA, resB, resC);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			promise.send(std::make_tuple(std::vector<UID>(10), 15, 2.0));
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ready(future);
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			if (static_cast<FlowTestCase252Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase252Actor*>(this)->actor_wait_state = 1;
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase252Actor, 0, Void >*>(static_cast<FlowTestCase252Actor*>(this)));
															#line 3891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
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
		this->~FlowTestCase252ActorState();
		static_cast<FlowTestCase252Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(resA.size() == 10);
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(resB == 15);
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(resC == 2.0);
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase252Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase252ActorState(); static_cast<FlowTestCase252Actor*>(this)->destroy(); return 0; }
															#line 3920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase252Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase252ActorState();
		static_cast<FlowTestCase252Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(resA.size() == 10);
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(resB == 15);
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		ASSERT(resC == 2.0);
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase252Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase252ActorState(); static_cast<FlowTestCase252Actor*>(this)->destroy(); return 0; }
															#line 3938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase252Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase252ActorState();
		static_cast<FlowTestCase252Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase252Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase252Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase252Actor*>(this)->ActorCallback< FlowTestCase252Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase252Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase252", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase252", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase252Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase252", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase252", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase252Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase252", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase252", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	UnitTestParameters params;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	std::vector<UID> resA;
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	int resB;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	double resC;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	Promise<std::tuple<std::vector<UID>, int, double>> promise;
															#line 4019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase252()
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
class FlowTestCase252Actor final : public Actor<Void>, public ActorCallback< FlowTestCase252Actor, 0, Void >, public FastAllocated<FlowTestCase252Actor>, public FlowTestCase252ActorState<FlowTestCase252Actor> {
															#line 4024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase252Actor>::operator new;
	using FastAllocated<FlowTestCase252Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase252Actor, 0, Void >;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	FlowTestCase252Actor(UnitTestParameters const& params) 
															#line 4035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase252ActorState<FlowTestCase252Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase252", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase252");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase252", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase252Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
static Future<Void> flowTestCase252( UnitTestParameters const& params ) {
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
	return Future<Void>(new FlowTestCase252Actor(params));
															#line 4063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase252, "/flow/genericactors/generic/storeTuple")

#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/genericactors.actor.cpp"
#endif
