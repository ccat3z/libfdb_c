#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
/*
 * PriorityMultiLock.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FLOW_PRIORITYMULTILOCK_ACTOR_G_H)
#define FLOW_PRIORITYMULTILOCK_ACTOR_G_H
#include "flow/PriorityMultiLock.actor.g.h"
#elif !defined(PRIORITYMULTILOCK_ACTOR_H)
#define PRIORITYMULTILOCK_ACTOR_H

#include "flow/flow.h"
#include <boost/intrusive/list.hpp>
#include "flow/actorcompiler.h" // This must be the last #include.

#define PRIORITYMULTILOCK_DEBUG 0

#if PRIORITYMULTILOCK_DEBUG || !defined(NO_INTELLISENSE)
#define pml_debug_printf(...)                                                                                          \
	if (now() > 0) {                                                                                                   \
		printf("pml line=%04d ", __LINE__);                                                                            \
		printf(__VA_ARGS__);                                                                                           \
	}
#else
#define pml_debug_printf(...)
#endif

// A multi user lock with a concurrent holder limit where waiters request a lock with a priority
// id and are granted locks based on a total concurrency and relative weights of the current active
// priorities.  Priority id's must start at 0 and are sequential integers.  Priority id numbers
// are not related to the importance of the priority in execution.
//
// Scheduling logic
// Let
// 	 weights[n] = configured weight for priority n
//   waiters[n] = the number of waiters for priority n
//   runnerCounts[n] = number of runners at priority n
//
//   totalPendingWeights = sum of weights for all priorities with waiters[n] > 0
//   When waiters[n] becomes == 0, totalPendingWeights -= weights[n]
//   When waiters[n] becomes  > 0, totalPendingWeights += weights[n]
//
//   The total capacity of a priority to be considered when launching tasks is
//     ceil(weights[n] / totalPendingWeights * concurrency)
//
// For improved memory locality the properties mentioned above are stored as priorities[n].<property>
// in the actual implementation.
//
// The interface is similar to FlowMutex except that lock holders can just drop the lock to release it.
//
// Usage:
//   Lock lock = wait(prioritylock.lock(priority_id));
//   lock.release();  // Explicit release, or
//   // let lock and all copies of lock go out of scope to release
class PriorityMultiLock : public ReferenceCounted<PriorityMultiLock> {
public:
	// Waiting on the lock returns a Lock, which is really just a Promise<Void>
	// Calling release() is not necessary, it exists in case the Lock holder wants to explicitly release
	// the Lock before it goes out of scope.
	struct Lock {
		void release() { promise.send(Void()); }
		bool isLocked() const { return promise.canBeSet(); }

		// This is exposed in case the caller wants to use/copy it directly
		Promise<Void> promise;
	};

	PriorityMultiLock(int concurrency, std::string weights)
	  : PriorityMultiLock(concurrency, parseStringToVector<int>(weights, ',')) {}

	PriorityMultiLock(int concurrency, std::vector<int> weightsByPriority)
	  : concurrency(concurrency), available(concurrency), waiting(0), totalPendingWeights(0), killed(false) {

		priorities.resize(weightsByPriority.size());
		for (int i = 0; i < priorities.size(); ++i) {
			priorities[i].priority = i;
			priorities[i].weight = weightsByPriority[i];
		}

		fRunner = runner(this);
	}

	~PriorityMultiLock() { kill(); }

	Future<Lock> lock(int priority = 0) {
		if (killed)
			throw broken_promise();

		Priority& p = priorities[priority];
		Queue& q = p.queue;

		// If this priority currently has no waiters
		if (q.empty()) {
			// Add this priority's weight to the total for priorities with pending work.  This must be done
			// so that currenctCapacity() below will assign capacaity to this priority.
			totalPendingWeights += p.weight;

			// If there are slots available and the priority has capacity then don't make the caller wait
			if (available > 0 && p.runners < currentCapacity(p.weight)) {
				// Remove this priority's weight from the total since it will remain empty
				totalPendingWeights -= p.weight;

				// Return a Lock to the caller
				Lock lock;
				addRunner(lock, &p);

				pml_debug_printf("lock nowait priority %d  %s\n", priority, toString().c_str());
				return lock;
			}

			// If we didn't return above then add the priority to the waitingPriorities list
			waitingPriorities.push_back(p);
		}

		Waiter& w = q.emplace_back();
		++waiting;

		pml_debug_printf("lock wait priority %d  %s\n", priority, toString().c_str());
		return w.lockPromise.getFuture();
	}

	// Halt stops the PML from handing out any new locks but leaves waiters and runners alone.
	// Existing and new waiters will not see an error, they will just never get a lock.
	// Can be safely called multiple times.
	void halt() {
		pml_debug_printf("halt %s\n", toString().c_str());
		brokenOnDestruct.reset();

		if (fRunner.isValid()) {
			fRunner.cancel();
			// Adjust available and concurrency so that if all runners finish the available
			available -= concurrency;
			concurrency = 0;
		}

		waitingPriorities.clear();
	}

	// Halt, then make existing and new waiters get a broken_promise error.
	// Can be safely called multiple times.
	void kill() {
		if (!killed) {
			// Killed must be set first because waiters which ignore exceptions could call wait again immediately.
			killed = true;
			halt();
			for (auto& p : priorities) {
				p.queue.clear();
			}
		}
	}

	std::string toString() const {
		std::string s = format("{ ptr=%p concurrency=%d available=%d running=%d waiting=%d "
		                       "pendingWeights=%d ",
		                       this,
		                       concurrency,
		                       available,
		                       concurrency - available,
		                       waiting,
		                       totalPendingWeights);

		for (auto& p : priorities) {
			s += format("{%s} ", p.toString(this).c_str());
		}

		s += "}";

		return s;
	}

	int maxPriority() const { return priorities.size() - 1; }

	int getRunnersCount() const { return concurrency - available; }
	int getWaitersCount() const { return waiting; }

	int getWaitersCount(const unsigned int priority) const {
		ASSERT(priority < priorities.size());
		return priorities[priority].queue.size();
	}

	int getRunnersCount(const unsigned int priority) const {
		ASSERT(priority < priorities.size());
		return priorities[priority].runners;
	}

private:
	struct Waiter {
		Promise<Lock> lockPromise;
	};

	// Total execution slots allowed across all priorities
	int concurrency;
	// Current available execution slots
	int available;
	// Total waiters across all priorities
	int waiting;
	// Sum of weights for all priorities with 1 or more waiters
	int totalPendingWeights;

	typedef Deque<Waiter> Queue;

	struct Priority : boost::intrusive::list_base_hook<> {
		Priority() : runners(0), weight(0), priority(-1) {}

		// Queue of waiters at this priority
		Queue queue;
		// Number of runners at this priority
		int runners;
		// Configured weight for this priority
		int weight;
		// Priority number for convenience, matches *this's index in PML priorities vector
		int priority;

		std::string toString(const PriorityMultiLock* pml) const {
			return format("priority=%d weight=%d run=%d wait=%d cap=%d",
			              priority,
			              weight,
			              runners,
			              queue.size(),
			              queue.empty() ? 0 : pml->currentCapacity(weight));
		}
	};

	std::vector<Priority> priorities;
	typedef boost::intrusive::list<Priority, boost::intrusive::constant_time_size<false>> WaitingPrioritiesList;

	// List of all priorities with 1 or more waiters.  This list exists so that the scheduling loop
	// does not have to iterage over the priorities vector checking priorities without waiters.
	WaitingPrioritiesList waitingPriorities;

	Future<Void> fRunner;
	AsyncTrigger wakeRunner;
	Promise<Void> brokenOnDestruct;
	bool killed;

																#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
// This generated class is to be used only via handleRelease()
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
template <class HandleReleaseActor>
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
class HandleReleaseActorState {
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
public:
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	HandleReleaseActorState(Reference<PriorityMultiLock> const& self,Priority* const& priority,Future<Void> const& holder) 
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		 : self(self),
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		   priority(priority),
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		   holder(holder)
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
	{
		fdb_probe_actor_create("handleRelease", reinterpret_cast<unsigned long>(this));

	}
	~HandleReleaseActorState() 
	{
		fdb_probe_actor_destroy("handleRelease", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			pml_debug_printf("%f handleRelease self=%p start\n", now(), self.getPtr());
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			try {
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				StrictFuture<Void> __when_expr_0 = holder;
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				static_cast<HandleReleaseActor*>(this)->actor_wait_state = 1;
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< HandleReleaseActor, 0, Void >*>(static_cast<HandleReleaseActor*>(this)));
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
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
		delete static_cast<HandleReleaseActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("lock release priority %d  %s\n", (int)(priority->priority), self->toString().c_str());
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("%f handleRelease self=%p releasing\n", now(), self.getPtr());
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		++self->available;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		priority->runners -= 1;
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		if (self->waiting > 0)
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		{
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			self->wakeRunner.trigger();
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			pml_debug_printf("%f handleRelease self=%p error %s\n", now(), self.getPtr(), e.what());
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
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
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("%f handleRelease self=%p success\n", now(), self.getPtr());
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("%f handleRelease self=%p success\n", now(), self.getPtr());
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
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
		if (static_cast<HandleReleaseActor*>(this)->actor_wait_state > 0) static_cast<HandleReleaseActor*>(this)->actor_wait_state = 0;
		static_cast<HandleReleaseActor*>(this)->ActorCallback< HandleReleaseActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< HandleReleaseActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("handleRelease", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("handleRelease", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< HandleReleaseActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("handleRelease", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("handleRelease", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< HandleReleaseActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("handleRelease", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("handleRelease", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont7(int loopDepth) 
	{
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		delete static_cast<HandleReleaseActor*>(this);
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	Reference<PriorityMultiLock> self;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	Priority* priority;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	Future<Void> holder;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
};
// This generated class is to be used only via handleRelease()
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
class HandleReleaseActor final : public Actor<void>, public ActorCallback< HandleReleaseActor, 0, Void >, public FastAllocated<HandleReleaseActor>, public HandleReleaseActorState<HandleReleaseActor> {
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
public:
	using FastAllocated<HandleReleaseActor>::operator new;
	using FastAllocated<HandleReleaseActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< HandleReleaseActor, 0, Void >;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	HandleReleaseActor(Reference<PriorityMultiLock> const& self,Priority* const& priority,Future<Void> const& holder) 
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		 : Actor<void>(),
		   HandleReleaseActorState<HandleReleaseActor>(self, priority, holder)
	{
		fdb_probe_actor_enter("handleRelease", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("handleRelease");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("handleRelease", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
static void handleRelease( Reference<PriorityMultiLock> const& self, Priority* const& priority, Future<Void> const& holder ) {
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	new HandleReleaseActor(self, priority, holder);
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
}

#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"

	void addRunner(Lock& lock, Priority* priority) {
		priority->runners += 1;
		--available;
		handleRelease(Reference<PriorityMultiLock>::addRef(this), priority, lock.promise.getFuture());
	}

	// Current maximum running tasks for the specified priority, which must have waiters
	// or the result is undefined
	int currentCapacity(int weight) const {
		// The total concurrency allowed for this priority at present is the total concurrency times
		// priority's weight divided by the total weights for all priorities with waiters.
		return ceil((float)weight / totalPendingWeights * concurrency);
	}

																#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
// This generated class is to be used only via runner()
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
template <class RunnerActor>
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
class RunnerActorState {
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
public:
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	RunnerActorState(PriorityMultiLock* const& self) 
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		 : self(self),
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		   error(self->brokenOnDestruct.getFuture()),
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		   p(self->waitingPriorities.end())
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
	{
		fdb_probe_actor_create("runner", reinterpret_cast<unsigned long>(this));

	}
	~RunnerActorState() 
	{
		fdb_probe_actor_destroy("runner", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			;
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
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
		this->~RunnerActorState();
		static_cast<RunnerActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("runner loop start  priority=%d  %s\n", p->priority, self->toString().c_str());
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("runner loop waitTrigger  priority=%d  %s\n", p->priority, self->toString().c_str());
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		StrictFuture<Void> __when_expr_0 = self->wakeRunner.onTrigger();
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		if (static_cast<RunnerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<RunnerActor*>(this)->actor_wait_state = 1;
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RunnerActor, 0, Void >*>(static_cast<RunnerActor*>(this)));
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("%f runner loop wake  priority=%d  %s\n", now(), p->priority, self->toString().c_str());
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		for(;self->available > 0 && self->waiting > 0;) {
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			pml_debug_printf("  launch loop start  priority=%d  %s\n", p->priority, self->toString().c_str());
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			for(;;) {
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				if (p == self->waitingPriorities.end())
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				{
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
					p = self->waitingPriorities.begin();
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				}
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				pml_debug_printf("    launch loop scan  priority=%d  %s\n", p->priority, self->toString().c_str());
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				if (!p->queue.empty() && p->runners < self->currentCapacity(p->weight))
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				{
					break;
				}
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				++p;
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Queue& queue = p->queue;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Waiter w = queue.front();
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			queue.pop_front();
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Priority* pPriority = &*p;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			if (queue.empty())
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			{
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				p = self->waitingPriorities.erase(p);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				self->totalPendingWeights -= pPriority->weight;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				pml_debug_printf( "      emptied priority  priority=%d  %s\n", pPriority->priority, self->toString().c_str());
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			--self->waiting;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Lock lock;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			w.lockPromise.send(lock);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			if (error.isReady())
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				return a_body1Catch1(error.getError(), std::max(0, loopDepth - 1));
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			if (lock.promise.canBeSet())
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				self->addRunner(lock, pPriority);
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			pml_debug_printf("    launched alreadyDone=%d priority=%d  %s\n", !lock.promise.canBeSet(), pPriority->priority, self->toString().c_str());
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		pml_debug_printf("%f runner loop wake  priority=%d  %s\n", now(), p->priority, self->toString().c_str());
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
		for(;self->available > 0 && self->waiting > 0;) {
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			pml_debug_printf("  launch loop start  priority=%d  %s\n", p->priority, self->toString().c_str());
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			for(;;) {
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				if (p == self->waitingPriorities.end())
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				{
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
					p = self->waitingPriorities.begin();
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				}
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				pml_debug_printf("    launch loop scan  priority=%d  %s\n", p->priority, self->toString().c_str());
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				if (!p->queue.empty() && p->runners < self->currentCapacity(p->weight))
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
				{
					break;
				}
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				++p;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Queue& queue = p->queue;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Waiter w = queue.front();
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			queue.pop_front();
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Priority* pPriority = &*p;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			if (queue.empty())
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			{
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				p = self->waitingPriorities.erase(p);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				self->totalPendingWeights -= pPriority->weight;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				pml_debug_printf( "      emptied priority  priority=%d  %s\n", pPriority->priority, self->toString().c_str());
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			--self->waiting;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			Lock lock;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			w.lockPromise.send(lock);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			if (error.isReady())
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				return a_body1Catch1(error.getError(), std::max(0, loopDepth - 1));
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			if (lock.promise.canBeSet())
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
				self->addRunner(lock, pPriority);
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
			}
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
			pml_debug_printf("    launched alreadyDone=%d priority=%d  %s\n", !lock.promise.canBeSet(), pPriority->priority, self->toString().c_str());
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
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
		if (static_cast<RunnerActor*>(this)->actor_wait_state > 0) static_cast<RunnerActor*>(this)->actor_wait_state = 0;
		static_cast<RunnerActor*>(this)->ActorCallback< RunnerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RunnerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("runner", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runner", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RunnerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("runner", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runner", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RunnerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("runner", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("runner", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	PriorityMultiLock* self;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	Future<Void> error;
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	WaitingPrioritiesList::iterator p;
															#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
};
// This generated class is to be used only via runner()
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
class RunnerActor final : public Actor<Void>, public ActorCallback< RunnerActor, 0, Void >, public FastAllocated<RunnerActor>, public RunnerActorState<RunnerActor> {
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
public:
	using FastAllocated<RunnerActor>::operator new;
	using FastAllocated<RunnerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RunnerActor, 0, Void >;
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	RunnerActor(PriorityMultiLock* const& self) 
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
		 : Actor<Void>(),
		   RunnerActorState<RunnerActor>(self)
	{
		fdb_probe_actor_enter("runner", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("runner");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("runner", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RunnerActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
[[nodiscard]] static Future<Void> runner( PriorityMultiLock* const& self ) {
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
	return Future<Void>(new RunnerActor(self));
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.g.h"
}

#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/PriorityMultiLock.actor.h"
};

#include "flow/unactorcompiler.h"

#endif
