#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
/*
 * ParallelStreamCorrectness.actor.cpp
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

#include <vector>

#include "fdbclient/ParallelStream.actor.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

namespace ParallelStreamTest {

struct TestValue {
	int x;
	TestValue(int x) : x(x) {}
	int expectedSize() const { return sizeof(int); }
};

															#line 37 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
// This generated class is to be used only via produce()
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
template <class ProduceActor>
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
class ProduceActorState {
															#line 43 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
public:
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ProduceActorState(ParallelStream<ParallelStreamTest::TestValue>::Fragment* const& fragment,ParallelStreamTest::TestValue const& value) 
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		 : fragment(fragment),
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   value(value)
															#line 52 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
	{
		fdb_probe_actor_create("produce", reinterpret_cast<unsigned long>(this));

	}
	~ProduceActorState() 
	{
		fdb_probe_actor_destroy("produce", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(deterministicRandom()->random01());
															#line 37 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			if (static_cast<ProduceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 69 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProduceActor*>(this)->actor_wait_state = 1;
															#line 37 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProduceActor, 0, Void >*>(static_cast<ProduceActor*>(this)));
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
		this->~ProduceActorState();
		static_cast<ProduceActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 38 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		fragment->send(value);
															#line 39 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(deterministicRandom()->random01());
															#line 39 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<ProduceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 101 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProduceActor*>(this)->actor_wait_state = 2;
															#line 39 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProduceActor, 1, Void >*>(static_cast<ProduceActor*>(this)));
															#line 106 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 38 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		fragment->send(value);
															#line 39 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(deterministicRandom()->random01());
															#line 39 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<ProduceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 119 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProduceActor*>(this)->actor_wait_state = 2;
															#line 39 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProduceActor, 1, Void >*>(static_cast<ProduceActor*>(this)));
															#line 124 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
		if (static_cast<ProduceActor*>(this)->actor_wait_state > 0) static_cast<ProduceActor*>(this)->actor_wait_state = 0;
		static_cast<ProduceActor*>(this)->ActorCallback< ProduceActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProduceActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ProduceActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ProduceActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 40 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		fragment->finish();
															#line 41 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (!static_cast<ProduceActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ProduceActorState(); static_cast<ProduceActor*>(this)->destroy(); return 0; }
															#line 198 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		new (&static_cast<ProduceActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ProduceActorState();
		static_cast<ProduceActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 40 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		fragment->finish();
															#line 41 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (!static_cast<ProduceActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ProduceActorState(); static_cast<ProduceActor*>(this)->destroy(); return 0; }
															#line 212 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		new (&static_cast<ProduceActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ProduceActorState();
		static_cast<ProduceActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ProduceActor*>(this)->actor_wait_state > 0) static_cast<ProduceActor*>(this)->actor_wait_state = 0;
		static_cast<ProduceActor*>(this)->ActorCallback< ProduceActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProduceActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ProduceActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ProduceActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ParallelStream<ParallelStreamTest::TestValue>::Fragment* fragment;
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ParallelStreamTest::TestValue value;
															#line 287 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
};
// This generated class is to be used only via produce()
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
class ProduceActor final : public Actor<Void>, public ActorCallback< ProduceActor, 0, Void >, public ActorCallback< ProduceActor, 1, Void >, public FastAllocated<ProduceActor>, public ProduceActorState<ProduceActor> {
															#line 292 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
public:
	using FastAllocated<ProduceActor>::operator new;
	using FastAllocated<ProduceActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ProduceActor, 0, Void >;
friend struct ActorCallback< ProduceActor, 1, Void >;
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ProduceActor(ParallelStream<ParallelStreamTest::TestValue>::Fragment* const& fragment,ParallelStreamTest::TestValue const& value) 
															#line 304 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		 : Actor<Void>(),
		   ProduceActorState<ProduceActor>(fragment, value)
	{
		fdb_probe_actor_enter("produce", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("produce");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("produce", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ProduceActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ProduceActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
[[nodiscard]] static Future<Void> produce( ParallelStream<ParallelStreamTest::TestValue>::Fragment* const& fragment, ParallelStreamTest::TestValue const& value ) {
															#line 35 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	return Future<Void>(new ProduceActor(fragment, value));
															#line 332 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
}

#line 43 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"

															#line 337 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
// This generated class is to be used only via consume()
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
template <class ConsumeActor>
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
class ConsumeActorState {
															#line 343 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
public:
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ConsumeActorState(FutureStream<ParallelStreamTest::TestValue> const& stream,int const& expected) 
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		 : stream(stream),
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   expected(expected),
															#line 45 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   next()
															#line 354 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
	{
		fdb_probe_actor_create("consume", reinterpret_cast<unsigned long>(this));

	}
	~ConsumeActorState() 
	{
		fdb_probe_actor_destroy("consume", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 47 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
				;
															#line 370 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
		this->~ConsumeActorState();
		static_cast<ConsumeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 52 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			ASSERT(e.code() == error_code_end_of_stream);
															#line 53 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			ASSERT(next == expected);
															#line 54 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			if (!static_cast<ConsumeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConsumeActorState(); static_cast<ConsumeActor*>(this)->destroy(); return 0; }
															#line 404 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
			new (&static_cast<ConsumeActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ConsumeActorState();
			static_cast<ConsumeActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 48 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		FutureStream<ParallelStreamTest::TestValue> __when_expr_0 = stream;
															#line 48 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<ConsumeActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 431 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<ConsumeActor*>(this)->actor_wait_state = 1;
															#line 48 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >*>(static_cast<ConsumeActor*>(this)));
															#line 436 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(ParallelStreamTest::TestValue const& value,int loopDepth) 
	{
															#line 49 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		ASSERT(value.x == next++);
															#line 445 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(ParallelStreamTest::TestValue && value,int loopDepth) 
	{
															#line 49 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		ASSERT(value.x == next++);
															#line 454 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(ParallelStreamTest::TestValue const& value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(value, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ParallelStreamTest::TestValue && value,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConsumeActor*>(this)->actor_wait_state > 0) static_cast<ConsumeActor*>(this)->actor_wait_state = 0;
		static_cast<ConsumeActor*>(this)->ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >*,ParallelStreamTest::TestValue const& value) 
	{
		fdb_probe_actor_enter("consume", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("consume", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >*,ParallelStreamTest::TestValue && value) 
	{
		fdb_probe_actor_enter("consume", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("consume", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >*,Error err) 
	{
		fdb_probe_actor_enter("consume", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("consume", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	FutureStream<ParallelStreamTest::TestValue> stream;
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	int expected;
															#line 45 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	int next;
															#line 528 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
};
// This generated class is to be used only via consume()
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
class ConsumeActor final : public Actor<Void>, public ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >, public FastAllocated<ConsumeActor>, public ConsumeActorState<ConsumeActor> {
															#line 533 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
public:
	using FastAllocated<ConsumeActor>::operator new;
	using FastAllocated<ConsumeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >;
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ConsumeActor(FutureStream<ParallelStreamTest::TestValue> const& stream,int const& expected) 
															#line 544 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		 : Actor<Void>(),
		   ConsumeActorState<ConsumeActor>(stream, expected)
	{
		fdb_probe_actor_enter("consume", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("consume");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("consume", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< ConsumeActor, 0, ParallelStreamTest::TestValue >*)0, actor_cancelled()); break;
		}

	}
};
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
[[nodiscard]] static Future<Void> consume( FutureStream<ParallelStreamTest::TestValue> const& stream, int const& expected ) {
															#line 44 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	return Future<Void>(new ConsumeActor(stream, expected));
															#line 571 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
}

#line 57 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"

} // namespace ParallelStreamTest

															#line 578 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase60()
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
template <class FlowTestCase60Actor>
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
class FlowTestCase60ActorState {
															#line 585 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
public:
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	FlowTestCase60ActorState(UnitTestParameters const& params) 
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		 : params(params),
															#line 61 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   results(),
															#line 62 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   bufferLimit(deterministicRandom()->randomInt(0, 21)),
															#line 63 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   numProducers(deterministicRandom()->randomInt(1, 1001)),
															#line 64 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   parallelStream(results, bufferLimit),
															#line 65 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   consumer(ParallelStreamTest::consume(results.getFuture(), numProducers)),
															#line 66 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		   producers()
															#line 604 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
															#line 67 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			TraceEvent("StartingParallelStreamTest").detail("BufferLimit", bufferLimit).detail("NumProducers", numProducers);
															#line 68 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			i = 0;
															#line 69 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
			;
															#line 623 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
		this->~FlowTestCase60ActorState();
		static_cast<FlowTestCase60Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 73 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		StrictFuture<Void> __when_expr_1 = parallelStream.finish();
															#line 73 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 648 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 2;
															#line 73 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 1, Void >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 653 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
															#line 69 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (!(i < numProducers))
															#line 669 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 70 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		StrictFuture<ParallelStream<ParallelStreamTest::TestValue>::Fragment*> __when_expr_0 = parallelStream.createFragment();
															#line 70 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 677 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 1;
															#line 70 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 682 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
	int a_body1loopBody1cont1(ParallelStream<ParallelStreamTest::TestValue>::Fragment* const& fragment,int loopDepth) 
	{
															#line 71 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		producers.push_back(ParallelStreamTest::produce(fragment, ParallelStreamTest::TestValue(i)));
															#line 69 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		++i;
															#line 706 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(ParallelStream<ParallelStreamTest::TestValue>::Fragment* && fragment,int loopDepth) 
	{
															#line 71 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		producers.push_back(ParallelStreamTest::produce(fragment, ParallelStreamTest::TestValue(i)));
															#line 69 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		++i;
															#line 717 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(ParallelStream<ParallelStreamTest::TestValue>::Fragment* const& fragment,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(fragment, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ParallelStream<ParallelStreamTest::TestValue>::Fragment* && fragment,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(fragment), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase60Actor*>(this)->ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >*,ParallelStream<ParallelStreamTest::TestValue>::Fragment* const& value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >*,ParallelStream<ParallelStreamTest::TestValue>::Fragment* && value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >*,Error err) 
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
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		StrictFuture<Void> __when_expr_2 = consumer;
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 791 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 3;
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 2, Void >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 796 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		StrictFuture<Void> __when_expr_2 = consumer;
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 807 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 3;
															#line 74 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase60Actor, 2, Void >*>(static_cast<FlowTestCase60Actor*>(this)));
															#line 812 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 75 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (!static_cast<FlowTestCase60Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase60ActorState(); static_cast<FlowTestCase60Actor*>(this)->destroy(); return 0; }
															#line 884 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		new (&static_cast<FlowTestCase60Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase60ActorState();
		static_cast<FlowTestCase60Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 75 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
		if (!static_cast<FlowTestCase60Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase60ActorState(); static_cast<FlowTestCase60Actor*>(this)->destroy(); return 0; }
															#line 896 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
		new (&static_cast<FlowTestCase60Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase60ActorState();
		static_cast<FlowTestCase60Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<FlowTestCase60Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase60Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase60Actor*>(this)->ActorCallback< FlowTestCase60Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase60Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase60Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase60", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase60", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	UnitTestParameters params;
															#line 61 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	PromiseStream<ParallelStreamTest::TestValue> results;
															#line 62 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	size_t bufferLimit;
															#line 63 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	size_t numProducers;
															#line 64 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	ParallelStream<ParallelStreamTest::TestValue> parallelStream;
															#line 65 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	Future<Void> consumer;
															#line 66 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	std::vector<Future<Void>> producers;
															#line 68 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	int i;
															#line 983 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase60()
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
class FlowTestCase60Actor final : public Actor<Void>, public ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >, public ActorCallback< FlowTestCase60Actor, 1, Void >, public ActorCallback< FlowTestCase60Actor, 2, Void >, public FastAllocated<FlowTestCase60Actor>, public FlowTestCase60ActorState<FlowTestCase60Actor> {
															#line 988 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase60Actor>::operator new;
	using FastAllocated<FlowTestCase60Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >;
friend struct ActorCallback< FlowTestCase60Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase60Actor, 2, Void >;
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	FlowTestCase60Actor(UnitTestParameters const& params) 
															#line 1001 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
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
		case 1: this->a_callback_error((ActorCallback< FlowTestCase60Actor, 0, ParallelStream<ParallelStreamTest::TestValue>::Fragment* >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase60Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase60Actor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
static Future<Void> flowTestCase60( UnitTestParameters const& params ) {
															#line 60 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"
	return Future<Void>(new FlowTestCase60Actor(params));
															#line 1031 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase60, "/fdbclient/ParallelStream")

#line 77 "/usr/src/libfdb_c/fdbclient/ParallelStream.actor.cpp"

void forceLinkParallelStreamTests() {}
