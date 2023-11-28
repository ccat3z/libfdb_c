#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
/*
 * WriteOnlySet.actor.cpp
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

#include "flow/DeterministicRandom.h"
#include "flow/WriteOnlySet.h"
#include "flow/flow.h"
#include "flow/UnitTest.h"

#include <chrono>
#include <random>
#include <thread>

#include "flow/actorcompiler.h" // has to be last include

#ifdef ENABLE_SAMPLING
template <class T, class IndexType, IndexType CAPACITY>
auto WriteOnlySet<T, IndexType, CAPACITY>::insert(const Reference<T>& lineage) -> Index {
	Index res;
	if (!freeQueue.pop(res)) {
		TraceEvent(SevWarnAlways, "NoCapacityInWriteOnlySet");
		return npos;
	}
	ASSERT(_set[res].load() == 0);
	auto ptr = reinterpret_cast<uintptr_t>(lineage.getPtr());
	ASSERT((ptr % 2) == 0); // this needs to be at least 2-byte aligned
	ASSERT(ptr != 0);
	lineage->addref();
	_set[res].store(ptr);
	return res;
}

template <class T, class IndexType, IndexType CAPACITY>
bool WriteOnlySet<T, IndexType, CAPACITY>::eraseImpl(Index idx) {
	while (true) {
		auto ptr = _set[idx].load();
		if (ptr & LOCK) {
			_set[idx].store(0);
			freeList.push(reinterpret_cast<T*>(ptr ^ LOCK));
			return false;
		} else {
			if (_set[idx].compare_exchange_strong(ptr, 0)) {
				reinterpret_cast<T*>(ptr)->delref();
				return true;
			}
		}
	}
}

template <class T, class IndexType, IndexType CAPACITY>
bool WriteOnlySet<T, IndexType, CAPACITY>::erase(Index idx) {
	ASSERT(idx >= 0 && idx < CAPACITY);
	auto res = eraseImpl(idx);
	ASSERT(freeQueue.push(idx));
	return res;
}

template <class T, class IndexType, IndexType CAPACITY>
bool WriteOnlySet<T, IndexType, CAPACITY>::replace(Index idx, const Reference<T>& lineage) {
	auto lineagePtr = reinterpret_cast<uintptr_t>(lineage.getPtr());
	if (lineage.isValid()) {
		lineage->addref();
	}
	ASSERT((lineagePtr % 2) == 0); // this needs to be at least 2-byte aligned

	while (true) {
		auto ptr = _set[idx].load();
		if (ptr & LOCK) {
			_set[idx].store(lineagePtr);
			ASSERT(freeList.push(reinterpret_cast<T*>(ptr ^ LOCK)));
			return false;
		} else {
			if (_set[idx].compare_exchange_strong(ptr, lineagePtr)) {
				if (ptr) {
					reinterpret_cast<T*>(ptr)->delref();
				}
				return ptr != 0;
			}
		}
	}
}

template <class T, class IndexType, IndexType CAPACITY>
WriteOnlySet<T, IndexType, CAPACITY>::WriteOnlySet() : _set(CAPACITY) {
	// insert the free indexes in reverse order
	for (unsigned i = CAPACITY; i > 0; --i) {
		freeQueue.push(i - 1);
		std::atomic_init(&_set[i - 1], uintptr_t(0));
	}
}

template <class T, class IndexType, IndexType CAPACITY>
std::vector<Reference<T>> WriteOnlySet<T, IndexType, CAPACITY>::copy() {
	std::vector<Reference<T>> result;
	for (int i = 0; i < CAPACITY; ++i) {
		auto ptr = _set[i].load();
		if (ptr) {
			ASSERT((ptr & LOCK) == 0); // if we lock something we need to immediately unlock after we're done copying
			// We attempt lock so this won't get deleted. We will try this only once, if the other thread removed the
			// object from the set between the previews lines and now, we just won't make it part of the result.
			if (_set[i].compare_exchange_strong(ptr, ptr | LOCK)) {
				T* entry = reinterpret_cast<T*>(ptr);
				ptr |= LOCK;
				entry->addref();
				// we try to unlock now. If this element was removed while we incremented the refcount, the element will
				// end up in the freeList, so we will decrement later.
				_set[i].compare_exchange_strong(ptr, ptr ^ LOCK);
				result.push_back(Reference(entry));
			}
		}
	}
	// after we're done we need to clean up all objects that contented on a lock. This won't be perfect (as some thread
	// might not yet added the object to the free list), but whatever we don't get now we'll clean up in the next
	// iteration
	freeList.consume_all([](auto toClean) { toClean->delref(); });
	return result;
}

template <class T, class IndexType>
WriteOnlyVariable<T, IndexType>::WriteOnlyVariable() : WriteOnlySet<T, IndexType, 1>() {}

template <class T, class IndexType>
Reference<T> WriteOnlyVariable<T, IndexType>::get() {
	auto result = WriteOnlySet<T, IndexType, 1>::copy();
	return result.size() ? result.at(0) : Reference<T>();
}

template <class T, class IndexType>
bool WriteOnlyVariable<T, IndexType>::replace(const Reference<T>& element) {
	return WriteOnlySet<T, IndexType, 1>::replace(0, element);
}

// Explicit instantiation
template class WriteOnlySet<ActorLineage, unsigned, 1024>;
template class WriteOnlyVariable<ActorLineage, unsigned>;

// testing code
namespace {

// Some statistics
std::atomic<unsigned long> instanceCounter = 0;
std::atomic<unsigned long> numInserts = 0;
std::atomic<unsigned long> numErase = 0;
std::atomic<unsigned long> numLockedErase = 0;
std::atomic<unsigned long> numCopied = 0;

// A simple object that counts the number of its instances. This is used to detect memory leaks.
struct TestObject {
	mutable std::atomic<unsigned> _refCount = 1;
	TestObject() { instanceCounter.fetch_add(1); }
	void delref() const {
		if (--_refCount == 0) {
			delete this;
			--instanceCounter;
		}
	}
	void addref() const { ++_refCount; }
};

using TestSet = WriteOnlySet<TestObject, unsigned, 128>;
using Clock = std::chrono::steady_clock;

// An actor that can join a set of threads in an async way.
															#line 182 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
// This generated class is to be used only via threadjoiner()
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
template <class ThreadjoinerActor>
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
class ThreadjoinerActorState {
															#line 188 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
public:
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	ThreadjoinerActorState(std::shared_ptr<std::vector<std::thread>> const& threads,std::shared_ptr<TestSet> const& set) 
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		 : threads(threads),
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		   set(set)
															#line 197 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
	{
		fdb_probe_actor_create("threadjoiner", reinterpret_cast<unsigned long>(this));

	}
	~ThreadjoinerActorState() 
	{
		fdb_probe_actor_destroy("threadjoiner", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 181 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			;
															#line 212 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
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
		this->~ThreadjoinerActorState();
		static_cast<ThreadjoinerActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 182 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(0.1);
															#line 182 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		if (static_cast<ThreadjoinerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 244 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ThreadjoinerActor*>(this)->actor_wait_state = 1;
															#line 182 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ThreadjoinerActor, 0, Void >*>(static_cast<ThreadjoinerActor*>(this)));
															#line 249 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 183 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		for(unsigned i = 0;;) {
															#line 184 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (threads->size() == i)
															#line 260 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			{
				break;
			}
															#line 187 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			auto& t = (*threads)[i];
															#line 188 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (t.joinable())
															#line 268 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			{
															#line 189 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				t.join();
															#line 190 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				if (i + 1 < threads->size())
															#line 274 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
				{
															#line 191 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
					std::swap(*threads->rbegin(), (*threads)[i]);
															#line 278 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
				}
															#line 193 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				threads->pop_back();
															#line 282 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			}
			else
			{
															#line 195 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				++i;
															#line 288 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			}
		}
															#line 198 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		if (threads->empty())
															#line 293 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		{
															#line 199 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			set->copy();
															#line 200 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			ASSERT(instanceCounter.load() == 0);
															#line 201 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (!static_cast<ThreadjoinerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ThreadjoinerActorState(); static_cast<ThreadjoinerActor*>(this)->destroy(); return 0; }
															#line 301 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			new (&static_cast<ThreadjoinerActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ThreadjoinerActorState();
			static_cast<ThreadjoinerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 183 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		for(unsigned i = 0;;) {
															#line 184 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (threads->size() == i)
															#line 317 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			{
				break;
			}
															#line 187 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			auto& t = (*threads)[i];
															#line 188 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (t.joinable())
															#line 325 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			{
															#line 189 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				t.join();
															#line 190 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				if (i + 1 < threads->size())
															#line 331 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
				{
															#line 191 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
					std::swap(*threads->rbegin(), (*threads)[i]);
															#line 335 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
				}
															#line 193 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				threads->pop_back();
															#line 339 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			}
			else
			{
															#line 195 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				++i;
															#line 345 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			}
		}
															#line 198 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		if (threads->empty())
															#line 350 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		{
															#line 199 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			set->copy();
															#line 200 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			ASSERT(instanceCounter.load() == 0);
															#line 201 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (!static_cast<ThreadjoinerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ThreadjoinerActorState(); static_cast<ThreadjoinerActor*>(this)->destroy(); return 0; }
															#line 358 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			new (&static_cast<ThreadjoinerActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ThreadjoinerActorState();
			static_cast<ThreadjoinerActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ThreadjoinerActor*>(this)->actor_wait_state > 0) static_cast<ThreadjoinerActor*>(this)->actor_wait_state = 0;
		static_cast<ThreadjoinerActor*>(this)->ActorCallback< ThreadjoinerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ThreadjoinerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("threadjoiner", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadjoiner", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ThreadjoinerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("threadjoiner", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadjoiner", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ThreadjoinerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("threadjoiner", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadjoiner", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	std::shared_ptr<std::vector<std::thread>> threads;
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	std::shared_ptr<TestSet> set;
															#line 435 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
};
// This generated class is to be used only via threadjoiner()
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
class ThreadjoinerActor final : public Actor<Void>, public ActorCallback< ThreadjoinerActor, 0, Void >, public FastAllocated<ThreadjoinerActor>, public ThreadjoinerActorState<ThreadjoinerActor> {
															#line 440 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
public:
	using FastAllocated<ThreadjoinerActor>::operator new;
	using FastAllocated<ThreadjoinerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ThreadjoinerActor, 0, Void >;
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	ThreadjoinerActor(std::shared_ptr<std::vector<std::thread>> const& threads,std::shared_ptr<TestSet> const& set) 
															#line 451 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		 : Actor<Void>(),
		   ThreadjoinerActorState<ThreadjoinerActor>(threads, set)
	{
		fdb_probe_actor_enter("threadjoiner", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("threadjoiner");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("threadjoiner", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ThreadjoinerActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
[[nodiscard]] Future<Void> threadjoiner( std::shared_ptr<std::vector<std::thread>> const& threads, std::shared_ptr<TestSet> const& set ) {
															#line 180 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	return Future<Void>(new ThreadjoinerActor(threads, set));
															#line 478 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
}

#line 205 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"

// occasionally copy the contents of the past set.
void testCopier(std::shared_ptr<TestSet> set, std::chrono::seconds runFor) {
	auto start = Clock::now();
	while (true) {
		if (Clock::now() - start > runFor) {
			return;
		}
		auto copy = set->copy();
		numCopied.fetch_add(copy.size());
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

// In a loop adds and removes a set of objects to the set
void writer(std::shared_ptr<TestSet> set, std::chrono::seconds runFor) {
	auto start = Clock::now();
	std::random_device rDev;
	DeterministicRandom rnd(rDev());
	while (true) {
		unsigned inserts = 0, erases = 0;
		if (Clock::now() - start > runFor) {
			return;
		}
		std::vector<TestSet::Index> positions;
		for (int i = 0; i < rnd.randomInt(1, 101); ++i) {
			Reference<TestObject> o(new TestObject());
			auto pos = set->insert(o);
			if (pos == TestSet::npos) {
				// could not insert -- ignore
				break;
			}
			++inserts;
			ASSERT(pos < TestSet::capacity);
			positions.push_back(pos);
		}
		rnd.randomShuffle(positions);
		for (auto p : positions) {
			if (!set->erase(p)) {
				++numLockedErase;
			}
			++erases;
		}
		numInserts.fetch_add(inserts);
		numErase.fetch_add(erases);
		ASSERT(inserts == erases);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

// This unit test creates 5 writer threads and one copier thread.
															#line 533 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
// This generated class is to be used only via flowTestCase256()
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
template <class FlowTestCase256Actor>
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
class FlowTestCase256ActorState {
															#line 539 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
public:
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	FlowTestCase256ActorState(UnitTestParameters const& params) 
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		 : params(params)
															#line 546 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase256", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase256ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase256", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 257 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (g_network->isSimulated())
															#line 561 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			{
															#line 259 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				if (!static_cast<FlowTestCase256Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase256ActorState(); static_cast<FlowTestCase256Actor*>(this)->destroy(); return 0; }
															#line 565 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
				new (&static_cast<FlowTestCase256Actor*>(this)->SAV< Void >::value()) Void(Void());
				this->~FlowTestCase256ActorState();
				static_cast<FlowTestCase256Actor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 261 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			auto set = std::make_shared<TestSet>();
															#line 262 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			auto threads = std::make_shared<std::vector<std::thread>>();
															#line 263 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			std::chrono::seconds runFor(10);
															#line 264 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			for(int i = 0;i < 5;++i) {
															#line 265 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
				threads->emplace_back([set, runFor]() { writer(set, runFor); });
															#line 581 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			}
															#line 267 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			threads->emplace_back([set, runFor]() { testCopier(set, runFor); });
															#line 268 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			StrictFuture<Void> __when_expr_0 = threadjoiner(threads, set);
															#line 268 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			if (static_cast<FlowTestCase256Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 589 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase256Actor*>(this)->actor_wait_state = 1;
															#line 268 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase256Actor, 0, Void >*>(static_cast<FlowTestCase256Actor*>(this)));
															#line 594 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
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
		this->~FlowTestCase256ActorState();
		static_cast<FlowTestCase256Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 269 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		TraceEvent("WriteOnlySetTestResult") .detail("Inserts", numInserts.load()) .detail("Erases", numErase.load()) .detail("Copies", numCopied.load()) .detail("LockedErase", numLockedErase.load());
															#line 274 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		if (!static_cast<FlowTestCase256Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase256ActorState(); static_cast<FlowTestCase256Actor*>(this)->destroy(); return 0; }
															#line 619 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		new (&static_cast<FlowTestCase256Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase256ActorState();
		static_cast<FlowTestCase256Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 269 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		TraceEvent("WriteOnlySetTestResult") .detail("Inserts", numInserts.load()) .detail("Erases", numErase.load()) .detail("Copies", numCopied.load()) .detail("LockedErase", numLockedErase.load());
															#line 274 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
		if (!static_cast<FlowTestCase256Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase256ActorState(); static_cast<FlowTestCase256Actor*>(this)->destroy(); return 0; }
															#line 633 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		new (&static_cast<FlowTestCase256Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase256ActorState();
		static_cast<FlowTestCase256Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase256Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase256Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase256Actor*>(this)->ActorCallback< FlowTestCase256Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase256Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase256", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase256", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase256Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase256", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase256", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase256Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase256", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase256", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	UnitTestParameters params;
															#line 706 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase256()
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
class FlowTestCase256Actor final : public Actor<Void>, public ActorCallback< FlowTestCase256Actor, 0, Void >, public FastAllocated<FlowTestCase256Actor>, public FlowTestCase256ActorState<FlowTestCase256Actor> {
															#line 711 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase256Actor>::operator new;
	using FastAllocated<FlowTestCase256Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase256Actor, 0, Void >;
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	FlowTestCase256Actor(UnitTestParameters const& params) 
															#line 722 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase256ActorState<FlowTestCase256Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase256", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase256");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase256", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase256Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
static Future<Void> flowTestCase256( UnitTestParameters const& params ) {
															#line 256 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
	return Future<Void>(new FlowTestCase256Actor(params));
															#line 749 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase256, "/flow/WriteOnlySet")

#line 276 "/usr/src/libfdb_c/flow/WriteOnlySet.actor.cpp"
} // namespace
#endif
