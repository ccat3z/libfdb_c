#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
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

															#line 27 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via allTrue()
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class AllTrueActor>
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class AllTrueActorState {
															#line 34 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	AllTrueActorState(std::vector<Future<bool>> const& all) 
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : all(all),
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   i(0)
															#line 43 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~AllTrueActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 27 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			;
															#line 56 "/src/flow/genericactors.actor.g.cpp"
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
															#line 33 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<AllTrueActor*>(this)->SAV<bool>::futures) { (void)(true); this->~AllTrueActorState(); static_cast<AllTrueActor*>(this)->destroy(); return 0; }
															#line 79 "/src/flow/genericactors.actor.g.cpp"
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
															#line 27 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!(i != all.size()))
															#line 98 "/src/flow/genericactors.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 28 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<bool> __when_expr_0 = all[i];
															#line 28 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<AllTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 106 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AllTrueActor*>(this)->actor_wait_state = 1;
															#line 28 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AllTrueActor, 0, bool >*>(static_cast<AllTrueActor*>(this)));
															#line 111 "/src/flow/genericactors.actor.g.cpp"
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
															#line 29 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!r)
															#line 133 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 30 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (!static_cast<AllTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~AllTrueActorState(); static_cast<AllTrueActor*>(this)->destroy(); return 0; }
															#line 137 "/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<AllTrueActor*>(this)->SAV< bool >::value()) bool(false);
			this->~AllTrueActorState();
			static_cast<AllTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 31 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		i++;
															#line 145 "/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(bool && r,int loopDepth) 
	{
															#line 29 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!r)
															#line 154 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 30 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (!static_cast<AllTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~AllTrueActorState(); static_cast<AllTrueActor*>(this)->destroy(); return 0; }
															#line 158 "/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<AllTrueActor*>(this)->SAV< bool >::value()) bool(false);
			this->~AllTrueActorState();
			static_cast<AllTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 31 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		i++;
															#line 166 "/src/flow/genericactors.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< AllTrueActor, 0, bool >*,bool && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< AllTrueActor, 0, bool >*,Error err) 
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
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> all;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	int i;
															#line 232 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via allTrue()
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class AllTrueActor final : public Actor<bool>, public ActorCallback< AllTrueActor, 0, bool >, public FastAllocated<AllTrueActor>, public AllTrueActorState<AllTrueActor> {
															#line 237 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<AllTrueActor>::operator new;
	using FastAllocated<AllTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AllTrueActor, 0, bool >;
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	AllTrueActor(std::vector<Future<bool>> const& all) 
															#line 248 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<bool>(),
		   AllTrueActorState<AllTrueActor>(all)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("allTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<bool> allTrue( std::vector<Future<bool>> const& all ) {
															#line 25 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<bool>(new AllTrueActor(all));
															#line 274 "/src/flow/genericactors.actor.g.cpp"
}

#line 35 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 279 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via anyTrue()
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class AnyTrueActor>
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class AnyTrueActorState {
															#line 286 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	AnyTrueActorState(std::vector<Reference<AsyncVar<bool>>> const& input,Reference<AsyncVar<bool>> const& output) 
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : input(input),
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   output(output)
															#line 295 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~AnyTrueActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			;
															#line 308 "/src/flow/genericactors.actor.g.cpp"
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
															#line 38 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		bool oneTrue = false;
															#line 39 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		std::vector<Future<Void>> changes;
															#line 40 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		for( auto it : input ) {
															#line 41 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (it->get())
															#line 344 "/src/flow/genericactors.actor.g.cpp"
			{
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
				oneTrue = true;
															#line 348 "/src/flow/genericactors.actor.g.cpp"
			}
															#line 43 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			changes.push_back(it->onChange());
															#line 352 "/src/flow/genericactors.actor.g.cpp"
		}
															#line 45 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		output->set(oneTrue);
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = waitForAny(changes);
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<AnyTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 360 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<AnyTrueActor*>(this)->actor_wait_state = 1;
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AnyTrueActor, 0, Void >*>(static_cast<AnyTrueActor*>(this)));
															#line 365 "/src/flow/genericactors.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< AnyTrueActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< AnyTrueActor, 0, Void >*,Error err) 
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
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Reference<AsyncVar<bool>>> input;
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Reference<AsyncVar<bool>> output;
															#line 443 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via anyTrue()
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class AnyTrueActor final : public Actor<Void>, public ActorCallback< AnyTrueActor, 0, Void >, public FastAllocated<AnyTrueActor>, public AnyTrueActorState<AnyTrueActor> {
															#line 448 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<AnyTrueActor>::operator new;
	using FastAllocated<AnyTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AnyTrueActor, 0, Void >;
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	AnyTrueActor(std::vector<Reference<AsyncVar<bool>>> const& input,Reference<AsyncVar<bool>> const& output) 
															#line 459 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   AnyTrueActorState<AnyTrueActor>(input, output)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("anyTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> anyTrue( std::vector<Reference<AsyncVar<bool>>> const& input, Reference<AsyncVar<bool>> const& output ) {
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new AnyTrueActor(input, output));
															#line 485 "/src/flow/genericactors.actor.g.cpp"
}

#line 49 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 490 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via cancelOnly()
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class CancelOnlyActor>
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class CancelOnlyActorState {
															#line 497 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	CancelOnlyActorState(std::vector<Future<Void>> const& futures) 
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : futures(futures)
															#line 504 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~CancelOnlyActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Never();
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (static_cast<CancelOnlyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 519 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CancelOnlyActor*>(this)->actor_wait_state = 1;
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CancelOnlyActor, 0, Void >*>(static_cast<CancelOnlyActor*>(this)));
															#line 524 "/src/flow/genericactors.actor.g.cpp"
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
															#line 54 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<CancelOnlyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancelOnlyActorState(); static_cast<CancelOnlyActor*>(this)->destroy(); return 0; }
															#line 547 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<CancelOnlyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CancelOnlyActorState();
		static_cast<CancelOnlyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 54 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<CancelOnlyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancelOnlyActorState(); static_cast<CancelOnlyActor*>(this)->destroy(); return 0; }
															#line 559 "/src/flow/genericactors.actor.g.cpp"
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
	void a_callback_fire(ActorCallback< CancelOnlyActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< CancelOnlyActor, 0, Void >*,Error err) 
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
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Future<Void>> futures;
															#line 626 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via cancelOnly()
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class CancelOnlyActor final : public Actor<Void>, public ActorCallback< CancelOnlyActor, 0, Void >, public FastAllocated<CancelOnlyActor>, public CancelOnlyActorState<CancelOnlyActor> {
															#line 631 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<CancelOnlyActor>::operator new;
	using FastAllocated<CancelOnlyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CancelOnlyActor, 0, Void >;
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	CancelOnlyActor(std::vector<Future<Void>> const& futures) 
															#line 642 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   CancelOnlyActorState<CancelOnlyActor>(futures)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cancelOnly");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> cancelOnly( std::vector<Future<Void>> const& futures ) {
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new CancelOnlyActor(futures));
															#line 668 "/src/flow/genericactors.actor.g.cpp"
}

#line 56 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 673 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via timeoutWarningCollector()
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class TimeoutWarningCollectorActor>
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class TimeoutWarningCollectorActorState {
															#line 680 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	TimeoutWarningCollectorActorState(FutureStream<Void> const& input,double const& logDelay,const char* const& context,UID const& id) 
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : input(input),
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   logDelay(logDelay),
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   context(context),
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   id(id),
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   counter(0),
															#line 59 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   end(delay(logDelay))
															#line 697 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~TimeoutWarningCollectorActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 60 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			;
															#line 710 "/src/flow/genericactors.actor.g.cpp"
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
															#line 61 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		FutureStream<Void> __when_expr_0 = input;
															#line 60 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<TimeoutWarningCollectorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 742 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = end;
															#line 746 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<TimeoutWarningCollectorActor*>(this)->actor_wait_state = 1;
															#line 61 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*>(static_cast<TimeoutWarningCollectorActor*>(this)));
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeoutWarningCollectorActor, 1, Void >*>(static_cast<TimeoutWarningCollectorActor*>(this)));
															#line 753 "/src/flow/genericactors.actor.g.cpp"
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
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		counter++;
															#line 768 "/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		counter++;
															#line 777 "/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 65 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (counter)
															#line 786 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			TraceEvent(SevWarn, context, id).detail("LateProcessCount", counter).detail("LoggingDelay", logDelay);
															#line 790 "/src/flow/genericactors.actor.g.cpp"
		}
															#line 67 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		end = delay(logDelay);
															#line 68 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		counter = 0;
															#line 796 "/src/flow/genericactors.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 65 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (counter)
															#line 805 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			TraceEvent(SevWarn, context, id).detail("LateProcessCount", counter).detail("LoggingDelay", logDelay);
															#line 809 "/src/flow/genericactors.actor.g.cpp"
		}
															#line 67 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		end = delay(logDelay);
															#line 68 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		counter = 0;
															#line 815 "/src/flow/genericactors.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >*,Error err) 
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
	void a_callback_fire(ActorCallback< TimeoutWarningCollectorActor, 1, Void >*,Void const& value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< TimeoutWarningCollectorActor, 1, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< TimeoutWarningCollectorActor, 1, Void >*,Error err) 
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
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	FutureStream<Void> input;
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	double logDelay;
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	const char* context;
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	UID id;
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	uint64_t counter;
															#line 59 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Future<Void> end;
															#line 917 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via timeoutWarningCollector()
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class TimeoutWarningCollectorActor final : public Actor<Void>, public ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >, public ActorCallback< TimeoutWarningCollectorActor, 1, Void >, public FastAllocated<TimeoutWarningCollectorActor>, public TimeoutWarningCollectorActorState<TimeoutWarningCollectorActor> {
															#line 922 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<TimeoutWarningCollectorActor>::operator new;
	using FastAllocated<TimeoutWarningCollectorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< TimeoutWarningCollectorActor, 0, Void >;
friend struct ActorCallback< TimeoutWarningCollectorActor, 1, Void >;
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	TimeoutWarningCollectorActor(FutureStream<Void> const& input,double const& logDelay,const char* const& context,UID const& id) 
															#line 934 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   TimeoutWarningCollectorActorState<TimeoutWarningCollectorActor>(input, logDelay, context, id)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeoutWarningCollector");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> timeoutWarningCollector( FutureStream<Void> const& input, double const& logDelay, const char* const& context, UID const& id ) {
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new TimeoutWarningCollectorActor(input, logDelay, context, id));
															#line 960 "/src/flow/genericactors.actor.g.cpp"
}

#line 72 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 965 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via quorumEqualsTrue()
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class QuorumEqualsTrueActor>
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class QuorumEqualsTrueActorState {
															#line 972 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	QuorumEqualsTrueActorState(std::vector<Future<bool>> const& futures,int const& required) 
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : futures(futures),
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   required(required),
															#line 74 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   true_futures(),
															#line 75 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   false_futures()
															#line 985 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~QuorumEqualsTrueActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 76 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			true_futures.reserve(futures.size());
															#line 77 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			false_futures.reserve(futures.size());
															#line 78 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			for(int i = 0;i < futures.size();i++) {
															#line 79 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
				true_futures.push_back(onEqual(futures[i], true));
															#line 80 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
				false_futures.push_back(onEqual(futures[i], false));
															#line 1006 "/src/flow/genericactors.actor.g.cpp"
			}
															#line 84 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = quorum(true_futures, required);
															#line 83 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (static_cast<QuorumEqualsTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1012 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 87 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_1 = quorum(false_futures, futures.size() - required + 1);
															#line 1016 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<QuorumEqualsTrueActor*>(this)->actor_wait_state = 1;
															#line 84 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< QuorumEqualsTrueActor, 0, Void >*>(static_cast<QuorumEqualsTrueActor*>(this)));
															#line 87 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< QuorumEqualsTrueActor, 1, Void >*>(static_cast<QuorumEqualsTrueActor*>(this)));
															#line 1023 "/src/flow/genericactors.actor.g.cpp"
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
															#line 85 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(true); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1046 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(true);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 85 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(true); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1058 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(true);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1070 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<QuorumEqualsTrueActor*>(this)->SAV< bool >::value()) bool(false);
		this->~QuorumEqualsTrueActorState();
		static_cast<QuorumEqualsTrueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 88 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<QuorumEqualsTrueActor*>(this)->SAV<bool>::futures) { (void)(false); this->~QuorumEqualsTrueActorState(); static_cast<QuorumEqualsTrueActor*>(this)->destroy(); return 0; }
															#line 1082 "/src/flow/genericactors.actor.g.cpp"
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
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< QuorumEqualsTrueActor, 0, Void >*,Error err) 
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
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 1, Void >*,Void const& value) 
	{
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< QuorumEqualsTrueActor, 1, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< QuorumEqualsTrueActor, 1, Void >*,Error err) 
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
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> futures;
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	int required;
															#line 74 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Future<Void>> true_futures;
															#line 75 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Future<Void>> false_futures;
															#line 1183 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via quorumEqualsTrue()
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class QuorumEqualsTrueActor final : public Actor<bool>, public ActorCallback< QuorumEqualsTrueActor, 0, Void >, public ActorCallback< QuorumEqualsTrueActor, 1, Void >, public FastAllocated<QuorumEqualsTrueActor>, public QuorumEqualsTrueActorState<QuorumEqualsTrueActor> {
															#line 1188 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<QuorumEqualsTrueActor>::operator new;
	using FastAllocated<QuorumEqualsTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< QuorumEqualsTrueActor, 0, Void >;
friend struct ActorCallback< QuorumEqualsTrueActor, 1, Void >;
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	QuorumEqualsTrueActor(std::vector<Future<bool>> const& futures,int const& required) 
															#line 1200 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<bool>(),
		   QuorumEqualsTrueActorState<QuorumEqualsTrueActor>(futures, required)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("quorumEqualsTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<bool> quorumEqualsTrue( std::vector<Future<bool>> const& futures, int const& required ) {
															#line 73 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<bool>(new QuorumEqualsTrueActor(futures, required));
															#line 1226 "/src/flow/genericactors.actor.g.cpp"
}

#line 92 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 1231 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via shortCircuitAny()
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class ShortCircuitAnyActor>
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class ShortCircuitAnyActorState {
															#line 1238 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	ShortCircuitAnyActorState(std::vector<Future<bool>> const& f) 
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : f(f)
															#line 1245 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~ShortCircuitAnyActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 94 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			std::vector<Future<Void>> sc;
															#line 95 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			sc.reserve(f.size());
															#line 96 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			for( Future<bool> fut : f ) {
															#line 97 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
				sc.push_back(returnIfTrue(fut));
															#line 1264 "/src/flow/genericactors.actor.g.cpp"
			}
															#line 101 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = waitForAll(f);
															#line 100 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (static_cast<ShortCircuitAnyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1270 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 112 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_1 = waitForAny(sc);
															#line 1274 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ShortCircuitAnyActor*>(this)->actor_wait_state = 1;
															#line 101 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ShortCircuitAnyActor, 0, Void >*>(static_cast<ShortCircuitAnyActor*>(this)));
															#line 112 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ShortCircuitAnyActor, 1, Void >*>(static_cast<ShortCircuitAnyActor*>(this)));
															#line 1281 "/src/flow/genericactors.actor.g.cpp"
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
															#line 105 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		for( const auto& fut : f ) {
															#line 106 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (fut.get())
															#line 1306 "/src/flow/genericactors.actor.g.cpp"
			{
															#line 107 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
				if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1310 "/src/flow/genericactors.actor.g.cpp"
				new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
				this->~ShortCircuitAnyActorState();
				static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 110 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1319 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 105 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		for( const auto& fut : f ) {
															#line 106 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (fut.get())
															#line 1333 "/src/flow/genericactors.actor.g.cpp"
			{
															#line 107 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
				if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1337 "/src/flow/genericactors.actor.g.cpp"
				new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
				this->~ShortCircuitAnyActorState();
				static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 110 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1346 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(false);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 113 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1358 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<ShortCircuitAnyActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ShortCircuitAnyActorState();
		static_cast<ShortCircuitAnyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 113 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<ShortCircuitAnyActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ShortCircuitAnyActorState(); static_cast<ShortCircuitAnyActor*>(this)->destroy(); return 0; }
															#line 1370 "/src/flow/genericactors.actor.g.cpp"
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
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< ShortCircuitAnyActor, 0, Void >*,Error err) 
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
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 1, Void >*,Void const& value) 
	{
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ShortCircuitAnyActor, 1, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ShortCircuitAnyActor, 1, Void >*,Error err) 
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
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	std::vector<Future<bool>> f;
															#line 1465 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via shortCircuitAny()
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class ShortCircuitAnyActor final : public Actor<bool>, public ActorCallback< ShortCircuitAnyActor, 0, Void >, public ActorCallback< ShortCircuitAnyActor, 1, Void >, public FastAllocated<ShortCircuitAnyActor>, public ShortCircuitAnyActorState<ShortCircuitAnyActor> {
															#line 1470 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<ShortCircuitAnyActor>::operator new;
	using FastAllocated<ShortCircuitAnyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ShortCircuitAnyActor, 0, Void >;
friend struct ActorCallback< ShortCircuitAnyActor, 1, Void >;
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	ShortCircuitAnyActor(std::vector<Future<bool>> const& f) 
															#line 1482 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<bool>(),
		   ShortCircuitAnyActorState<ShortCircuitAnyActor>(f)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("shortCircuitAny");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<bool> shortCircuitAny( std::vector<Future<bool>> const& f ) {
															#line 93 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<bool>(new ShortCircuitAnyActor(f));
															#line 1508 "/src/flow/genericactors.actor.g.cpp"
}

#line 117 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

Future<Void> orYield(Future<Void> f) {
	if (f.isReady()) {
		if (f.isError())
			return tagError<Void>(yield(), f.getError());
		else
			return yield();
	} else
		return f;
}

															#line 1523 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via returnIfTrue()
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class ReturnIfTrueActor>
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class ReturnIfTrueActorState {
															#line 1530 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	ReturnIfTrueActorState(Future<bool> const& f) 
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : f(f)
															#line 1537 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~ReturnIfTrueActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 129 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<bool> __when_expr_0 = f;
															#line 129 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1552 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 1;
															#line 129 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReturnIfTrueActor, 0, bool >*>(static_cast<ReturnIfTrueActor*>(this)));
															#line 1557 "/src/flow/genericactors.actor.g.cpp"
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
															#line 130 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (b)
															#line 1580 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 131 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (!static_cast<ReturnIfTrueActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReturnIfTrueActorState(); static_cast<ReturnIfTrueActor*>(this)->destroy(); return 0; }
															#line 1584 "/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<ReturnIfTrueActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReturnIfTrueActorState();
			static_cast<ReturnIfTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = Never();
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1594 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 2;
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReturnIfTrueActor, 1, Void >*>(static_cast<ReturnIfTrueActor*>(this)));
															#line 1599 "/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool && b,int loopDepth) 
	{
															#line 130 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (b)
															#line 1608 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 131 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (!static_cast<ReturnIfTrueActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReturnIfTrueActorState(); static_cast<ReturnIfTrueActor*>(this)->destroy(); return 0; }
															#line 1612 "/src/flow/genericactors.actor.g.cpp"
			new (&static_cast<ReturnIfTrueActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReturnIfTrueActorState();
			static_cast<ReturnIfTrueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_1 = Never();
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<ReturnIfTrueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1622 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReturnIfTrueActor*>(this)->actor_wait_state = 2;
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReturnIfTrueActor, 1, Void >*>(static_cast<ReturnIfTrueActor*>(this)));
															#line 1627 "/src/flow/genericactors.actor.g.cpp"
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
	void a_callback_fire(ActorCallback< ReturnIfTrueActor, 0, bool >*,bool && value) 
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
	void a_callback_error(ActorCallback< ReturnIfTrueActor, 0, bool >*,Error err) 
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
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 134 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1693 "/src/flow/genericactors.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 134 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1701 "/src/flow/genericactors.actor.g.cpp"

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
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ReturnIfTrueActor, 1, Void >*,Void && value) 
	{
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ReturnIfTrueActor, 1, Void >*,Error err) 
	{
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Future<bool> f;
															#line 1764 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via returnIfTrue()
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class ReturnIfTrueActor final : public Actor<Void>, public ActorCallback< ReturnIfTrueActor, 0, bool >, public ActorCallback< ReturnIfTrueActor, 1, Void >, public FastAllocated<ReturnIfTrueActor>, public ReturnIfTrueActorState<ReturnIfTrueActor> {
															#line 1769 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<ReturnIfTrueActor>::operator new;
	using FastAllocated<ReturnIfTrueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReturnIfTrueActor, 0, bool >;
friend struct ActorCallback< ReturnIfTrueActor, 1, Void >;
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	ReturnIfTrueActor(Future<bool> const& f) 
															#line 1781 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   ReturnIfTrueActorState<ReturnIfTrueActor>(f)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("returnIfTrue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> returnIfTrue( Future<bool> const& f ) {
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new ReturnIfTrueActor(f));
															#line 1808 "/src/flow/genericactors.actor.g.cpp"
}

#line 136 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 1813 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via lowPriorityDelay()
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class LowPriorityDelayActor>
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class LowPriorityDelayActorState {
															#line 1820 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	LowPriorityDelayActorState(double const& waitTime) 
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : waitTime(waitTime),
															#line 138 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   loopCount(0),
															#line 139 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   totalLoops(std::max<int>(waitTime / FLOW_KNOBS->LOW_PRIORITY_MAX_DELAY, FLOW_KNOBS->LOW_PRIORITY_DELAY_COUNT))
															#line 1831 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~LowPriorityDelayActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 142 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			;
															#line 1844 "/src/flow/genericactors.actor.g.cpp"
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
															#line 146 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<LowPriorityDelayActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LowPriorityDelayActorState(); static_cast<LowPriorityDelayActor*>(this)->destroy(); return 0; }
															#line 1867 "/src/flow/genericactors.actor.g.cpp"
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
															#line 142 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!(loopCount < totalLoops))
															#line 1886 "/src/flow/genericactors.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 143 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(waitTime / totalLoops, TaskPriority::Low);
															#line 143 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<LowPriorityDelayActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1894 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<LowPriorityDelayActor*>(this)->actor_wait_state = 1;
															#line 143 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LowPriorityDelayActor, 0, Void >*>(static_cast<LowPriorityDelayActor*>(this)));
															#line 1899 "/src/flow/genericactors.actor.g.cpp"
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
															#line 144 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		loopCount++;
															#line 1921 "/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 144 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		loopCount++;
															#line 1930 "/src/flow/genericactors.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< LowPriorityDelayActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< LowPriorityDelayActor, 0, Void >*,Error err) 
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
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	double waitTime;
															#line 138 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	int loopCount;
															#line 139 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	int totalLoops;
															#line 1998 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via lowPriorityDelay()
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class LowPriorityDelayActor final : public Actor<Void>, public ActorCallback< LowPriorityDelayActor, 0, Void >, public FastAllocated<LowPriorityDelayActor>, public LowPriorityDelayActorState<LowPriorityDelayActor> {
															#line 2003 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<LowPriorityDelayActor>::operator new;
	using FastAllocated<LowPriorityDelayActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LowPriorityDelayActor, 0, Void >;
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	LowPriorityDelayActor(double const& waitTime) 
															#line 2014 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   LowPriorityDelayActorState<LowPriorityDelayActor>(waitTime)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lowPriorityDelay");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> lowPriorityDelay( double const& waitTime ) {
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new LowPriorityDelayActor(waitTime));
															#line 2040 "/src/flow/genericactors.actor.g.cpp"
}

#line 148 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

namespace {

struct DummyState {
	int changed{ 0 };
	int unchanged{ 0 };
	bool operator==(DummyState const& rhs) const { return changed == rhs.changed && unchanged == rhs.unchanged; }
	bool operator!=(DummyState const& rhs) const { return !(*this == rhs); }
};

															#line 2054 "/src/flow/genericactors.actor.g.cpp"
// This generated class is to be used only via testPublisher()
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class TestPublisherActor>
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class TestPublisherActorState {
															#line 2060 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	TestPublisherActorState(Reference<AsyncVar<DummyState>> const& input) 
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : input(input),
															#line 159 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   i(0)
															#line 2069 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~TestPublisherActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 160 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			;
															#line 2082 "/src/flow/genericactors.actor.g.cpp"
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
															#line 166 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<TestPublisherActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestPublisherActorState(); static_cast<TestPublisherActor*>(this)->destroy(); return 0; }
															#line 2105 "/src/flow/genericactors.actor.g.cpp"
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
															#line 160 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!(i < 100))
															#line 2124 "/src/flow/genericactors.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 161 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(deterministicRandom()->random01());
															#line 161 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<TestPublisherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2132 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TestPublisherActor*>(this)->actor_wait_state = 1;
															#line 161 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestPublisherActor, 0, Void >*>(static_cast<TestPublisherActor*>(this)));
															#line 2137 "/src/flow/genericactors.actor.g.cpp"
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
															#line 162 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		auto var = input->get();
															#line 163 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		++var.changed;
															#line 164 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		input->set(var);
															#line 160 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		++i;
															#line 2165 "/src/flow/genericactors.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 162 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		auto var = input->get();
															#line 163 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		++var.changed;
															#line 164 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		input->set(var);
															#line 160 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		++i;
															#line 2180 "/src/flow/genericactors.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< TestPublisherActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< TestPublisherActor, 0, Void >*,Error err) 
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
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Reference<AsyncVar<DummyState>> input;
															#line 159 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	int i;
															#line 2246 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via testPublisher()
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class TestPublisherActor final : public Actor<Void>, public ActorCallback< TestPublisherActor, 0, Void >, public FastAllocated<TestPublisherActor>, public TestPublisherActorState<TestPublisherActor> {
															#line 2251 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<TestPublisherActor>::operator new;
	using FastAllocated<TestPublisherActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestPublisherActor, 0, Void >;
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	TestPublisherActor(Reference<AsyncVar<DummyState>> const& input) 
															#line 2262 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   TestPublisherActorState<TestPublisherActor>(input)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testPublisher");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> testPublisher( Reference<AsyncVar<DummyState>> const& input ) {
															#line 158 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new TestPublisherActor(input));
															#line 2287 "/src/flow/genericactors.actor.g.cpp"
}

#line 168 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

															#line 2292 "/src/flow/genericactors.actor.g.cpp"
// This generated class is to be used only via testSubscriber()
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class TestSubscriberActor>
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class TestSubscriberActorState {
															#line 2298 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	TestSubscriberActorState(Reference<IAsyncListener<int>> const& output,Optional<int> const& expected) 
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : output(output),
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		   expected(expected)
															#line 2307 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~TestSubscriberActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 170 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			;
															#line 2320 "/src/flow/genericactors.actor.g.cpp"
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
															#line 171 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		StrictFuture<Void> __when_expr_0 = output->onChange();
															#line 171 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (static_cast<TestSubscriberActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2352 "/src/flow/genericactors.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TestSubscriberActor*>(this)->actor_wait_state = 1;
															#line 171 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestSubscriberActor, 0, Void >*>(static_cast<TestSubscriberActor*>(this)));
															#line 2357 "/src/flow/genericactors.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 172 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		ASSERT(expected.present());
															#line 173 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (output->get() == expected.get())
															#line 2368 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 174 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (!static_cast<TestSubscriberActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestSubscriberActorState(); static_cast<TestSubscriberActor*>(this)->destroy(); return 0; }
															#line 2372 "/src/flow/genericactors.actor.g.cpp"
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
															#line 172 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		ASSERT(expected.present());
															#line 173 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (output->get() == expected.get())
															#line 2388 "/src/flow/genericactors.actor.g.cpp"
		{
															#line 174 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (!static_cast<TestSubscriberActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestSubscriberActorState(); static_cast<TestSubscriberActor*>(this)->destroy(); return 0; }
															#line 2392 "/src/flow/genericactors.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< TestSubscriberActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< TestSubscriberActor, 0, Void >*,Error err) 
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
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Reference<IAsyncListener<int>> output;
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Optional<int> expected;
															#line 2463 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via testSubscriber()
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class TestSubscriberActor final : public Actor<Void>, public ActorCallback< TestSubscriberActor, 0, Void >, public FastAllocated<TestSubscriberActor>, public TestSubscriberActorState<TestSubscriberActor> {
															#line 2468 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<TestSubscriberActor>::operator new;
	using FastAllocated<TestSubscriberActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestSubscriberActor, 0, Void >;
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	TestSubscriberActor(Reference<IAsyncListener<int>> const& output,Optional<int> const& expected) 
															#line 2479 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   TestSubscriberActorState<TestSubscriberActor>(output, expected)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testSubscriber");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
[[nodiscard]] Future<Void> testSubscriber( Reference<IAsyncListener<int>> const& output, Optional<int> const& expected ) {
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new TestSubscriberActor(output, expected));
															#line 2504 "/src/flow/genericactors.actor.g.cpp"
}

#line 178 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"

} // namespace

															#line 2511 "/src/flow/genericactors.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase181()
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
template <class FlowTestCase181Actor>
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class FlowTestCase181ActorState {
															#line 2518 "/src/flow/genericactors.actor.g.cpp"
public:
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	FlowTestCase181ActorState(UnitTestParameters const& params) 
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		 : params(params)
															#line 2525 "/src/flow/genericactors.actor.g.cpp"
	{

	}
	~FlowTestCase181ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 182 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			auto input = makeReference<AsyncVar<DummyState>>();
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			subscriber1 = testSubscriber(IAsyncListener<int>::create(input, [](auto const& var) { return var.changed; }), 100);
															#line 185 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			subscriber2 = testSubscriber(IAsyncListener<int>::create(input, [](auto const& var) { return var.unchanged; }), {});
															#line 187 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			StrictFuture<Void> __when_expr_0 = subscriber1 && testPublisher(input);
															#line 187 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			if (static_cast<FlowTestCase181Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2546 "/src/flow/genericactors.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase181Actor*>(this)->actor_wait_state = 1;
															#line 187 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase181Actor, 0, Void >*>(static_cast<FlowTestCase181Actor*>(this)));
															#line 2551 "/src/flow/genericactors.actor.g.cpp"
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
		this->~FlowTestCase181ActorState();
		static_cast<FlowTestCase181Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 188 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		ASSERT(!subscriber2.isReady());
															#line 189 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase181Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase181ActorState(); static_cast<FlowTestCase181Actor*>(this)->destroy(); return 0; }
															#line 2576 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase181Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase181ActorState();
		static_cast<FlowTestCase181Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 188 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		ASSERT(!subscriber2.isReady());
															#line 189 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
		if (!static_cast<FlowTestCase181Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase181ActorState(); static_cast<FlowTestCase181Actor*>(this)->destroy(); return 0; }
															#line 2590 "/src/flow/genericactors.actor.g.cpp"
		new (&static_cast<FlowTestCase181Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase181ActorState();
		static_cast<FlowTestCase181Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase181Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase181Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase181Actor*>(this)->ActorCallback< FlowTestCase181Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase181Actor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< FlowTestCase181Actor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< FlowTestCase181Actor, 0, Void >*,Error err) 
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
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	UnitTestParameters params;
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Future<Void> subscriber1;
															#line 185 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	Future<Void> subscriber2;
															#line 2661 "/src/flow/genericactors.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase181()
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
class FlowTestCase181Actor final : public Actor<Void>, public ActorCallback< FlowTestCase181Actor, 0, Void >, public FastAllocated<FlowTestCase181Actor>, public FlowTestCase181ActorState<FlowTestCase181Actor> {
															#line 2666 "/src/flow/genericactors.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase181Actor>::operator new;
	using FastAllocated<FlowTestCase181Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase181Actor, 0, Void >;
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	FlowTestCase181Actor(UnitTestParameters const& params) 
															#line 2677 "/src/flow/genericactors.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase181ActorState<FlowTestCase181Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase181");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase181Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
static Future<Void> flowTestCase181( UnitTestParameters const& params ) {
															#line 181 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
	return Future<Void>(new FlowTestCase181Actor(params));
															#line 2703 "/src/flow/genericactors.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase181, "/flow/genericactors/AsyncListener")

#line 191 "/tmp/fdb_c/foundationdb_ep/flow/genericactors.actor.cpp"
