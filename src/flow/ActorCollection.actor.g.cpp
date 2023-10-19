#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
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
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 28 "/src/flow/ActorCollection.actor.g.cpp"
namespace {
// This generated class is to be used only via actorCollection()
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
template <class ActorCollectionActor>
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
class ActorCollectionActorState {
															#line 35 "/src/flow/ActorCollection.actor.g.cpp"
public:
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	ActorCollectionActorState(FutureStream<Future<Void>> const& addActor,int* const& pCount,double* const& lastChangeTime,double* const& idleTime,double* const& allTime,bool const& returnWhenEmptied) 
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		 : addActor(addActor),
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   pCount(pCount),
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   lastChangeTime(lastChangeTime),
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   idleTime(idleTime),
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   allTime(allTime),
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   returnWhenEmptied(returnWhenEmptied),
															#line 32 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   nextTag(0),
															#line 33 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   tag_streamHelper(),
															#line 34 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   complete(),
															#line 35 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   errors(),
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		   count(0)
															#line 62 "/src/flow/ActorCollection.actor.g.cpp"
	{

	}
	~ActorCollectionActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 37 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			if (!pCount)
															#line 75 "/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 38 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				pCount = &count;
															#line 79 "/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 40 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			;
															#line 83 "/src/flow/ActorCollection.actor.g.cpp"
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
															#line 41 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		FutureStream<Future<Void>> __when_expr_0 = addActor;
															#line 40 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		if (static_cast<ActorCollectionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 115 "/src/flow/ActorCollection.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 52 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		FutureStream<int64_t> __when_expr_1 = complete.getFuture();
															#line 119 "/src/flow/ActorCollection.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.pop(), loopDepth); };
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		FutureStream<Error> __when_expr_2 = errors.getFuture();
															#line 123 "/src/flow/ActorCollection.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when3(__when_expr_2.pop(), loopDepth); };
		static_cast<ActorCollectionActor*>(this)->actor_wait_state = 1;
															#line 41 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*>(static_cast<ActorCollectionActor*>(this)));
															#line 52 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< ActorCollectionActor, 1, int64_t >*>(static_cast<ActorCollectionActor*>(this)));
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorSingleCallback< ActorCollectionActor, 2, Error >*>(static_cast<ActorCollectionActor*>(this)));
															#line 132 "/src/flow/ActorCollection.actor.g.cpp"
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
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		int64_t t = nextTag++;
															#line 43 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		tag_streamHelper[t] = streamHelper(complete, errors, tag(f, t));
															#line 44 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		++*pCount;
															#line 45 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		if (*pCount == 1 && lastChangeTime && idleTime && allTime)
															#line 153 "/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			double currentTime = now();
															#line 47 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			*idleTime += currentTime - *lastChangeTime;
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			*allTime += currentTime - *lastChangeTime;
															#line 49 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			*lastChangeTime = currentTime;
															#line 163 "/src/flow/ActorCollection.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Future<Void> && f,int loopDepth) 
	{
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		int64_t t = nextTag++;
															#line 43 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		tag_streamHelper[t] = streamHelper(complete, errors, tag(f, t));
															#line 44 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		++*pCount;
															#line 45 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		if (*pCount == 1 && lastChangeTime && idleTime && allTime)
															#line 179 "/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			double currentTime = now();
															#line 47 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			*idleTime += currentTime - *lastChangeTime;
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			*allTime += currentTime - *lastChangeTime;
															#line 49 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			*lastChangeTime = currentTime;
															#line 189 "/src/flow/ActorCollection.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(int64_t const& t,int loopDepth) 
	{
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		if (!--*pCount)
															#line 199 "/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 54 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			if (lastChangeTime && idleTime && allTime)
															#line 203 "/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 55 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				double currentTime = now();
															#line 56 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				*allTime += currentTime - *lastChangeTime;
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				*lastChangeTime = currentTime;
															#line 211 "/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 59 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			if (returnWhenEmptied)
															#line 215 "/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 60 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				if (!static_cast<ActorCollectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorCollectionActorState(); static_cast<ActorCollectionActor*>(this)->destroy(); return 0; }
															#line 219 "/src/flow/ActorCollection.actor.g.cpp"
				new (&static_cast<ActorCollectionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ActorCollectionActorState();
				static_cast<ActorCollectionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		tag_streamHelper.erase(t);
															#line 228 "/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(int64_t && t,int loopDepth) 
	{
															#line 53 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		if (!--*pCount)
															#line 237 "/src/flow/ActorCollection.actor.g.cpp"
		{
															#line 54 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			if (lastChangeTime && idleTime && allTime)
															#line 241 "/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 55 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				double currentTime = now();
															#line 56 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				*allTime += currentTime - *lastChangeTime;
															#line 57 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				*lastChangeTime = currentTime;
															#line 249 "/src/flow/ActorCollection.actor.g.cpp"
			}
															#line 59 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
			if (returnWhenEmptied)
															#line 253 "/src/flow/ActorCollection.actor.g.cpp"
			{
															#line 60 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
				if (!static_cast<ActorCollectionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorCollectionActorState(); static_cast<ActorCollectionActor*>(this)->destroy(); return 0; }
															#line 257 "/src/flow/ActorCollection.actor.g.cpp"
				new (&static_cast<ActorCollectionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ActorCollectionActorState();
				static_cast<ActorCollectionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		tag_streamHelper.erase(t);
															#line 266 "/src/flow/ActorCollection.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(Error const& e,int loopDepth) 
	{
															#line 65 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 275 "/src/flow/ActorCollection.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1when3(Error && e,int loopDepth) 
	{
															#line 65 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
		return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 283 "/src/flow/ActorCollection.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorCollectionActor*>(this)->actor_wait_state > 0) static_cast<ActorCollectionActor*>(this)->actor_wait_state = 0;
		static_cast<ActorCollectionActor*>(this)->ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >::remove();
		static_cast<ActorCollectionActor*>(this)->ActorSingleCallback< ActorCollectionActor, 1, int64_t >::remove();
		static_cast<ActorCollectionActor*>(this)->ActorSingleCallback< ActorCollectionActor, 2, Error >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*,Future<Void> const& value) 
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
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*,Future<Void> && value) 
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
	void a_callback_error(ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >*,Error err) 
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
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 1, int64_t >*,int64_t const& value) 
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
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 1, int64_t >*,int64_t && value) 
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
	void a_callback_error(ActorSingleCallback< ActorCollectionActor, 1, int64_t >*,Error err) 
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
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 2, Error >*,Error const& value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorSingleCallback< ActorCollectionActor, 2, Error >*,Error && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorSingleCallback< ActorCollectionActor, 2, Error >*,Error err) 
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
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	FutureStream<Future<Void>> addActor;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	int* pCount;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	double* lastChangeTime;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	double* idleTime;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	double* allTime;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	bool returnWhenEmptied;
															#line 32 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	int64_t nextTag;
															#line 33 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	Map<int64_t, Future<Void>> tag_streamHelper;
															#line 34 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	PromiseStream<int64_t> complete;
															#line 35 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	PromiseStream<Error> errors;
															#line 36 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	int count;
															#line 434 "/src/flow/ActorCollection.actor.g.cpp"
};
// This generated class is to be used only via actorCollection()
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
class ActorCollectionActor final : public Actor<Void>, public ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >, public ActorSingleCallback< ActorCollectionActor, 1, int64_t >, public ActorSingleCallback< ActorCollectionActor, 2, Error >, public FastAllocated<ActorCollectionActor>, public ActorCollectionActorState<ActorCollectionActor> {
															#line 439 "/src/flow/ActorCollection.actor.g.cpp"
public:
	using FastAllocated<ActorCollectionActor>::operator new;
	using FastAllocated<ActorCollectionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ActorCollectionActor, 0, Future<Void> >;
friend struct ActorSingleCallback< ActorCollectionActor, 1, int64_t >;
friend struct ActorSingleCallback< ActorCollectionActor, 2, Error >;
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	ActorCollectionActor(FutureStream<Future<Void>> const& addActor,int* const& pCount,double* const& lastChangeTime,double* const& idleTime,double* const& allTime,bool const& returnWhenEmptied) 
															#line 452 "/src/flow/ActorCollection.actor.g.cpp"
		 : Actor<Void>(),
		   ActorCollectionActorState<ActorCollectionActor>(addActor, pCount, lastChangeTime, idleTime, allTime, returnWhenEmptied)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorCollection");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
[[nodiscard]] Future<Void> actorCollection( FutureStream<Future<Void>> const& addActor, int* const& pCount, double* const& lastChangeTime, double* const& idleTime, double* const& allTime, bool const& returnWhenEmptied ) {
															#line 26 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"
	return Future<Void>(new ActorCollectionActor(addActor, pCount, lastChangeTime, idleTime, allTime, returnWhenEmptied));
															#line 478 "/src/flow/ActorCollection.actor.g.cpp"
}

#line 69 "/tmp/fdb_c/foundationdb_ep/flow/ActorCollection.actor.cpp"

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
