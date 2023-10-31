#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
/*
 * dsltest.actor.cpp
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

#include <iostream>
#include <algorithm>
#include "flow/FastRef.h"
#undef ERROR
#include "fdbrpc/simulator.h"
#include "fdbrpc/ActorFuzz.h"
#include "flow/DeterministicRandom.h"
#include "flow/ThreadHelper.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

void* allocateLargePages(int total);

bool testFuzzActor(Future<int> (*actor)(FutureStream<int> const&, PromiseStream<int> const&, Future<Void> const&),
                   const char* desc,
                   std::vector<int> const& expectedOutput) {
	// Run the test 5 times with different "timing"
	int i, outCount;
	bool ok = true;
	for (int trial = 0; trial < 5; trial++) {
		PromiseStream<int> in, out;
		Promise<Void> err;
		int before = deterministicRandom()->randomInt(0, 4);
		int errorBefore = before + deterministicRandom()->randomInt(0, 4);
		// printf("\t\tTrial #%d: %d, %d\n", trial, before, errorBefore);
		if (errorBefore <= before)
			err.sendError(operation_failed());
		for (i = 0; i < before; i++) {
			in.send((i + 1) * 1000);
		}
		Future<int> ret = (*actor)(in.getFuture(), out, err.getFuture());
		while (i < 1000000 && !ret.isReady()) {
			i++;
			if (errorBefore == i)
				err.sendError(operation_failed());
			in.send(i * 1000);
		}
		if (ret.isReady()) {
			if (ret.isError())
				out.send(ret.getError().code());
			else
				out.send(ret.get());
		} else {
			printf("\tERROR: %s did not return after consuming %d input values\n", desc, i);
			if (trial)
				printf("\t\tResult was inconsistent between runs!  (Trial %d)\n", trial);
			ok = false;
			// return false;
		}

		outCount = -1;
		while (out.getFuture().isReady()) {
			int o = out.getFuture().pop();
			outCount++;
			if (outCount < expectedOutput.size() && expectedOutput[outCount] != o) {
				printf("\tERROR: %s output #%d incorrect: %d != expected %d\n",
				       desc,
				       outCount,
				       o,
				       expectedOutput[outCount]);
				if (trial)
					printf("\t\tResult was inconsistent between runs!\n");
				ok = false;
				// return false;
			}
		}
		if (outCount + 1 != expectedOutput.size()) {
			printf(
			    "\tERROR: %s output length incorrect: %d != expected %zu\n", desc, outCount + 1, expectedOutput.size());
			if (trial)
				printf("\t\tResult was inconsistent between runs!\n");
			ok = false;
			// return false;
		}

		// We might have put in values that weren't actually consumed...
		while (in.getFuture().isReady()) {
			in.getFuture().pop();
			i--;
		}
	}
	// printf("\t%s: OK, %d input values -> %d output values\n", desc, i, outCount);
	return ok;
}

#if 0
void memoryTest2() {
	const int Size = 2000 << 20;
	const int Reads = 4 << 20;
	const int MaxThreads = 4;

	char* block = new char[Size];
	memset(block, 0, Size);

	char** random = new char*[ Reads * MaxThreads ];
	random[0] = block;
	for(int i=1; i<Reads*MaxThreads; ) {
		char *s = &block[ deterministicRandom()->randomInt(0, Size) ];
		random[i++] = s;
		/*for(int j=0; j<10 && i<Reads*MaxThreads; j++,i++) {
			random[i] = s + deterministicRandom()->randomInt(0, 4096);
			if (random[i] >= block+Size) random[i] -= Size;
		}*/
	}

	for(int threads=1; threads<=MaxThreads; threads++) {
		double tstart = timer();

		std::vector<ThreadFuture<Void>> done;
		for(int t=0; t<threads; t++) {
			char** r = random + Reads*t;
			done.push_back(
				inThread<Void>( [r,Reads] () -> Void {
					for(int i=0; i<Reads; i++)
						if ( *r[i] )
							std::cout << "Does not happen" << std::endl;
					return Void();
				}));
		}
		waitForAll(done).getBlocking();
		double duration = timer() - tstart;

		std::cout << format("%d threads: %f sec, %0.2fM/sec", threads, duration, Reads*threads/1e6/duration) << std::endl;
	}
}
#endif

enum { MaxTraversalsPerThread = 64 };

void showNumaStatus();
void* numaAllocate(size_t size);

#if 0
void memoryTest() {
	//memoryTest2();
	//return;

	showNumaStatus();

	const int N = 128<<20;	// 128 = 1GB
	const int N2 = 8<<20;
	std::cout << "Preparing memory test with " << N / 1e6 * sizeof(void*) << " MB" << std::endl;
	void **x;
	if (0) {
		std::cout << "  NUMA large pages" << std::endl;
		x = (void**)numaAllocate(size_t(N)*sizeof(void*));
	} else if (1) {
		std::cout << "  Normal pages" << std::endl;
		x = new void*[ N ];
		printf("  at %p\n", x);
	} else {
		std::cout << "  Large pages" << std::endl;
		x = (void**)allocate(N*sizeof(void*), true);
	}
	memset(x, 0, ((int64_t)N) * sizeof(void*));

	showNumaStatus();

	if (1) {
		std::cout <<"  Random permutation" << std::endl;
		// Random cyclic permutation
		for(int i=0; i<N; i++)
			x[i] = &x[i];
		// Sattolo's algorithm
		for(int n = N-1; n >= 1; n--) {
			int k = deterministicRandom()->randomInt(0, n);  //random.IRandomX(0, n-1);
			std::swap( x[k], x[n] );
		}
	} else {
		std::cout <<"  Sequential permutation" << std::endl;
		// Sequential
		for(int i=0; i<N-1; i++)
			x[i] = &x[i+1];
		x[N-1] = &x[0];
	}
	void **p = x;
	for(int i=0; i<N; i++) {
		p = (void**)*p;
		if (p == x) {
			std::cout << "Cycle " << i << std::endl;
			if (i != N-1) terminate();
		}
	}

	const int MT = 16;
	for(int TraversalsPerThread = 1; TraversalsPerThread <= MaxTraversalsPerThread; TraversalsPerThread *= 2)
	{
		const int PseudoThreads = MT * TraversalsPerThread;
		void **starts[MT*MaxTraversalsPerThread];
		for(int t=0; t<PseudoThreads; t++) {
			starts[t] = &x[ N/PseudoThreads * t ];
			//starts[t] = &x[ deterministicRandom()->randomInt(0,N) ];
		}
		for(int T=1; T<=MT; T+=T) {
			double start = timer();
			std::vector< Future<double> > done;
			for(int t=0; t<T; t++) {
				void*** start = starts + t*TraversalsPerThread;
				done.push_back(
					inThread<double>( [start,N2,TraversalsPerThread] () -> double {
						void **p[MaxTraversalsPerThread];
						for(int j=0; j<TraversalsPerThread; j++)
							p[j] = start[j];
						for(int i=0; i<N2; i++)
							for(int j=0; j<TraversalsPerThread; j++) {
								p[j] = (void**)*p[j];
								if (TraversalsPerThread > 1)
									_mm_prefetch( (const char*)p[j], _MM_HINT_T0 );
							}
						for(int j=0; j<TraversalsPerThread; j++)
							if (p[j] == p[(j+1)%TraversalsPerThread])
								std::cout << "N";
						return timer();
					}));
			}
			double firstEnd = 1e30;
			for(int t=0; t<T; t++) {
				done[t].getBlocking();
				firstEnd = std::min(firstEnd, done[t].get());
			}
			double end = timer();
			printf("  %2dx%2d traversals: %5.3fs, %6.1f M/sec, %4.1f%%\n", T, (int)TraversalsPerThread, end-start,
				N2 / 1e6 * (T*TraversalsPerThread) / (end-start),
				(firstEnd-start)/(end-start)*100.0);
		}
	}

	//delete[] x;	// TODO: Free large pages
}
#endif

															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via addN()
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <int N, class X, class AddNActor>
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class AddNActorState {
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	AddNActorState(Future<X> const& in) 
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : in(in)
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("addN", reinterpret_cast<unsigned long>(this));

	}
	~AddNActorState() 
	{
		fdb_probe_actor_destroy("addN", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<X> __when_expr_0 = in;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<AddNActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AddNActor*>(this)->actor_wait_state = 1;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddNActor, 0, X >*>(static_cast<AddNActor*>(this)));
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~AddNActorState();
		static_cast<AddNActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(X const& i,int loopDepth) 
	{
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<AddNActor*>(this)->SAV<X>::futures) { (void)(i + N); this->~AddNActorState(); static_cast<AddNActor*>(this)->destroy(); return 0; }
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<AddNActor*>(this)->SAV< X >::value()) X(i + N);
		this->~AddNActorState();
		static_cast<AddNActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(X && i,int loopDepth) 
	{
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<AddNActor*>(this)->SAV<X>::futures) { (void)(i + N); this->~AddNActorState(); static_cast<AddNActor*>(this)->destroy(); return 0; }
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<AddNActor*>(this)->SAV< X >::value()) X(i + N);
		this->~AddNActorState();
		static_cast<AddNActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(X const& i,int loopDepth) 
	{
		loopDepth = a_body1cont1(i, loopDepth);

		return loopDepth;
	}
	int a_body1when1(X && i,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(i), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AddNActor*>(this)->actor_wait_state > 0) static_cast<AddNActor*>(this)->actor_wait_state = 0;
		static_cast<AddNActor*>(this)->ActorCallback< AddNActor, 0, X >::remove();

	}
	void a_callback_fire(ActorCallback< AddNActor, 0, X >*,X const& value) 
	{
		fdb_probe_actor_enter("addN", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addN", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AddNActor, 0, X >*,X && value) 
	{
		fdb_probe_actor_enter("addN", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addN", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AddNActor, 0, X >*,Error err) 
	{
		fdb_probe_actor_enter("addN", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addN", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<X> in;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via addN()
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <int N, class X>
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class AddNActor final : public Actor<X>, public ActorCallback< AddNActor<N, X>, 0, X >, public FastAllocated<AddNActor<N, X>>, public AddNActorState<N, X, AddNActor<N, X>> {
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<AddNActor<N, X>>::operator new;
	using FastAllocated<AddNActor<N, X>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<X>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddNActor<N, X>, 0, X >;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	AddNActor(Future<X> const& in) 
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<X>(),
		   AddNActorState<N, X, AddNActor<N, X>>(in)
	{
		fdb_probe_actor_enter("addN", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("addN");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("addN", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AddNActor<N, X>, 0, X >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <int N, class X>
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<X> addN( Future<X> const& in ) {
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<X>(new AddNActor<N, X>(in));
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via switchTest()
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class A, class B, class SwitchTestActor>
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SwitchTestActorState {
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SwitchTestActorState(FutureStream<A> const& as,Future<B> const& oneb) 
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : as(as),
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   oneb(oneb)
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("switchTest", reinterpret_cast<unsigned long>(this));

	}
	~SwitchTestActorState() 
	{
		fdb_probe_actor_destroy("switchTest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~SwitchTestActorState();
		static_cast<SwitchTestActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		for(;;) {
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			std::cout << "Done!" << std::endl;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<SwitchTestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SwitchTestActorState(); static_cast<SwitchTestActor*>(this)->destroy(); return 0; }
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<SwitchTestActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~SwitchTestActorState();
			static_cast<SwitchTestActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<A> __when_expr_0 = as;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<SwitchTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		StrictFuture<B> __when_expr_1 = oneb;
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<SwitchTestActor*>(this)->actor_wait_state = 1;
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< SwitchTestActor, 0, A >*>(static_cast<SwitchTestActor*>(this)));
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SwitchTestActor, 1, B >*>(static_cast<SwitchTestActor*>(this)));
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(A const& a,int loopDepth) 
	{
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		std::cout << "A " << a << std::endl;
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(A && a,int loopDepth) 
	{
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		std::cout << "A " << a << std::endl;
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(B const& b,int loopDepth) 
	{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		std::cout << "B " << b << std::endl;
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when2(B && b,int loopDepth) 
	{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		std::cout << "B " << b << std::endl;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SwitchTestActor*>(this)->actor_wait_state > 0) static_cast<SwitchTestActor*>(this)->actor_wait_state = 0;
		static_cast<SwitchTestActor*>(this)->ActorSingleCallback< SwitchTestActor, 0, A >::remove();
		static_cast<SwitchTestActor*>(this)->ActorCallback< SwitchTestActor, 1, B >::remove();

	}
	void a_callback_fire(ActorSingleCallback< SwitchTestActor, 0, A >*,A const& value) 
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< SwitchTestActor, 0, A >*,A && value) 
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< SwitchTestActor, 0, A >*,Error err) 
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SwitchTestActor, 1, B >*,B const& value) 
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SwitchTestActor, 1, B >*,B && value) 
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SwitchTestActor, 1, B >*,Error err) 
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	FutureStream<A> as;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<B> oneb;
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via switchTest()
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class A, class B>
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SwitchTestActor final : public Actor<Void>, public ActorSingleCallback< SwitchTestActor<A, B>, 0, A >, public ActorCallback< SwitchTestActor<A, B>, 1, B >, public FastAllocated<SwitchTestActor<A, B>>, public SwitchTestActorState<A, B, SwitchTestActor<A, B>> {
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<SwitchTestActor<A, B>>::operator new;
	using FastAllocated<SwitchTestActor<A, B>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< SwitchTestActor<A, B>, 0, A >;
friend struct ActorCallback< SwitchTestActor<A, B>, 1, B >;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SwitchTestActor(FutureStream<A> const& as,Future<B> const& oneb) 
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   SwitchTestActorState<A, B, SwitchTestActor<A, B>>(as, oneb)
	{
		fdb_probe_actor_enter("switchTest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("switchTest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("switchTest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< SwitchTestActor<A, B>, 0, A >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class A, class B>
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> switchTest( FutureStream<A> const& as, Future<B> const& oneb ) {
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new SwitchTestActor<A, B>(as, oneb));
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

class TestBuffer : public ReferenceCounted<TestBuffer> {
public:
	static TestBuffer* create(int length) {
#if defined(__INTEL_COMPILER)
		return new TestBuffer(length);
#else
		auto b = (TestBuffer*)new int[(length + 7) / 4];
		new (b) TestBuffer(length);
		return b;
#endif
	}
#if !defined(__INTEL_COMPILER)
	void operator delete(void* buf) {
		std::cout << "Freeing buffer" << std::endl;
		delete[] (int*)buf;
	}
#endif

	int size() const { return length; }
	uint8_t* begin() { return data; }
	uint8_t* end() { return data + length; }
	const uint8_t* begin() const { return data; }
	const uint8_t* end() const { return data + length; }

private:
	TestBuffer(int length) noexcept : length(length) {}
	int length;
	uint8_t data[1];
};

int fastKeyCount = 0;

class FastKey : public FastAllocated<FastKey>, public ReferenceCounted<FastKey> {
public:
	FastKey() : length(0) {}
	FastKey(char* b, int length) : length(length) {
		ASSERT(length <= sizeof(data));
		memcpy(data, b, length);
	}
	~FastKey() { fastKeyCount++; }
	int size() const { return length; }
	uint8_t* begin() { return data; }
	uint8_t* end() { return data + length; }
	const uint8_t* begin() const { return data; }
	const uint8_t* end() const { return data + length; }

private:
	int length;
	uint8_t data[252];
};

struct TestB : FastAllocated<TestB> {
	char x[65];
};

void fastAllocTest() {
	double t;

	std::vector<void*> d;
	for (int i = 0; i < 1000000; i++) {
		d.push_back(FastAllocator<64>::allocate());
		int r = deterministicRandom()->randomInt(0, 1000000);
		if (r < d.size()) {
			FastAllocator<64>::release(d[r]);
			d[r] = d.back();
			d.pop_back();
		}
	}
	std::sort(d.begin(), d.end());
	if (std::unique(d.begin(), d.end()) != d.end())
		std::cout << "Pointer returned twice!?" << std::endl;

	for (int i = 0; i < 2; i++) {
		void* p = FastAllocator<64>::allocate();
		void* q = FastAllocator<64>::allocate();
		std::cout << (intptr_t)p << " " << (intptr_t)q << std::endl;
		FastAllocator<64>::release(p);
		FastAllocator<64>::release(q);
	}

	t = timer();
	for (int i = 0; i < 1000000; i++)
		(void)FastAllocator<64>::allocate();
	t = timer() - t;
	std::cout << "Allocations: " << (1 / t) << "M/sec" << std::endl;

	t = timer();
	for (int i = 0; i < 1000000; i++)
		FastAllocator<64>::release(FastAllocator<64>::allocate());
	t = timer() - t;
	std::cout << "Allocate/Release pairs: " << (1 / t) << "M/sec" << std::endl;

	t = timer();
	void* pp[100];
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 100; j++)
			pp[j] = FastAllocator<64>::allocate();
		for (int j = 0; j < 100; j++)
			FastAllocator<64>::release(pp[j]);
	}
	t = timer() - t;
	std::cout << "Allocate/Release interleaved(100): " << (1 / t) << "M/sec" << std::endl;

	t = timer();
	for (int i = 0; i < 1000000; i++)
		delete new TestB;
	t = timer() - t;
	std::cout << "Allocate/Release TestB pairs: " << (1 / t) << "M/sec" << std::endl;

#if FLOW_THREAD_SAFE
	t = timer();
	std::vector<Future<bool>> results;
	for (int i = 0; i < 4; i++)
		results.push_back(inThread<bool>([]() -> bool {
			TestB* pp[100];
			for (int i = 0; i < 10000; i++) {
				for (int j = 0; j < 100; j++)
					pp[j] = new TestB;
				for (int j = 0; j < 100; j++)
					delete pp[j];
			}
			return true;
		}));
	waitForAll(results).getBlocking();
	t = timer() - t;
	std::cout << "Threaded Allocate/Release TestB interleaved (100): " << results.size() << " x " << (1 / t) << "M/sec"
	          << std::endl;
#endif

	volatile int32_t v = 0;

	t = timer();
	for (int i = 0; i < 10000000; i++)
		interlockedIncrement(&v);
	t = timer() - t;
	std::cout << "interlocked increment: " << 10.0 / t << "M/sec " << v << std::endl;

	v = 5;
	t = timer();
	for (int i = 0; i < 10000000; i++) {
		interlockedCompareExchange(&v, 5, 5);
	}
	t = timer() - t;
	std::cout << "1 state machine: " << 10.0 / t << "M/sec " << v << std::endl;

	v = 0;
	t = timer();
	for (int i = 0; i < 10000000; i++)
		v++;
	t = timer() - t;
	std::cout << "volatile increment: " << 10.0 / t << "M/sec " << v << std::endl;

	{
		Reference<TestBuffer> b(TestBuffer::create(1000));
		memcpy(b->begin(), "Hello, world!", 14);

		t = timer();
		for (int i = 0; i < 10000000; i++) {
			Reference<TestBuffer> r = std::move(b);
			b = std::move(r);
		}
		t = timer() - t;
		std::cout << "move Reference<Buffer>: " << 10.0 / t << "M/sec " << std::endl;

		t = timer();
		for (int i = 0; i < 10000000; i++) {
			Reference<TestBuffer> r = b;
		}
		t = timer() - t;
		std::cout << "copy (1) Reference<Buffer>: " << 10.0 / t << "M/sec " << std::endl;

		Reference<TestBuffer> c = b;
		t = timer();
		for (int i = 0; i < 10000000; i++) {
			Reference<TestBuffer> r = b;
		}
		t = timer() - t;
		std::cout << "copy (2) Reference<Buffer>: " << 10.0 / t << "M/sec " << std::endl;

		std::cout << (const char*)b->begin() << std::endl;
	}
	t = timer();
	for (int i = 0; i < 10000000; i++) {
		delete new FastKey;
	}
	t = timer() - t;
	std::cout << "delete new FastKey: " << 10.0 / t << "M/sec " << fastKeyCount << std::endl;

	t = timer();
	for (int i = 0; i < 10000000; i++) {
		Reference<FastKey> r(new FastKey);
	}
	t = timer() - t;
	std::cout << "new Reference<FastKey>: " << 10.0 / t << "M/sec " << fastKeyCount << std::endl;
}

template <class PromiseT>
Future<Void> threadSafetySender(std::vector<PromiseT>& v, Event& start, Event& ready, int iterations) {
	for (int i = 0; i < iterations; i++) {
		start.block();
		if (v.size() == 0)
			return Void();
		for (int i = 0; i < v.size(); i++)
			v[i].send(Void());
		ready.set();
	}
	return Void();
}

															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via threadSafetyWaiter()
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ThreadSafetyWaiterActor>
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ThreadSafetyWaiterActorState {
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ThreadSafetyWaiterActorState(Future<Void> const& f,int32_t* const& count) 
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : f(f),
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   count(count)
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("threadSafetyWaiter", reinterpret_cast<unsigned long>(this));

	}
	~ThreadSafetyWaiterActorState() 
	{
		fdb_probe_actor_destroy("threadSafetyWaiter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f;
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			static_cast<ThreadSafetyWaiterActor*>(this)->actor_wait_state = 1;
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ThreadSafetyWaiterActor, 0, Void >*>(static_cast<ThreadSafetyWaiterActor*>(this)));
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		delete static_cast<ThreadSafetyWaiterActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		interlockedIncrement(count);
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		interlockedIncrement(count);
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<ThreadSafetyWaiterActor*>(this)->actor_wait_state > 0) static_cast<ThreadSafetyWaiterActor*>(this)->actor_wait_state = 0;
		static_cast<ThreadSafetyWaiterActor*>(this)->ActorCallback< ThreadSafetyWaiterActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ThreadSafetyWaiterActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ThreadSafetyWaiterActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ThreadSafetyWaiterActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		delete static_cast<ThreadSafetyWaiterActor*>(this);
															#line 1105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<Void> f;
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int32_t* count;
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via threadSafetyWaiter()
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ThreadSafetyWaiterActor final : public Actor<void>, public ActorCallback< ThreadSafetyWaiterActor, 0, Void >, public FastAllocated<ThreadSafetyWaiterActor>, public ThreadSafetyWaiterActorState<ThreadSafetyWaiterActor> {
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ThreadSafetyWaiterActor>::operator new;
	using FastAllocated<ThreadSafetyWaiterActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< ThreadSafetyWaiterActor, 0, Void >;
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ThreadSafetyWaiterActor(Future<Void> const& f,int32_t* const& count) 
															#line 1130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<void>(),
		   ThreadSafetyWaiterActorState<ThreadSafetyWaiterActor>(f, count)
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("threadSafetyWaiter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
void threadSafetyWaiter( Future<Void> const& f, int32_t* const& count ) {
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	new ThreadSafetyWaiterActor(f, count);
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via threadSafetyWaiter()
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ThreadSafetyWaiterActor1>
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ThreadSafetyWaiterActor1State {
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ThreadSafetyWaiterActor1State(FutureStream<Void> const& f,int const& n,int32_t* const& count) 
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : f(f),
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   n(n),
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   count(count)
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("threadSafetyWaiter", reinterpret_cast<unsigned long>(this));

	}
	~ThreadSafetyWaiterActor1State() 
	{
		fdb_probe_actor_destroy("threadSafetyWaiter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		delete static_cast<ThreadSafetyWaiterActor1*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

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
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!(n--))
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<Void> __when_expr_0 = f;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 1229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		static_cast<ThreadSafetyWaiterActor1*>(this)->actor_wait_state = 1;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >*>(static_cast<ThreadSafetyWaiterActor1*>(this)));
															#line 1233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		interlockedIncrement(count);
															#line 1255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		interlockedIncrement(count);
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		if (static_cast<ThreadSafetyWaiterActor1*>(this)->actor_wait_state > 0) static_cast<ThreadSafetyWaiterActor1*>(this)->actor_wait_state = 0;
		static_cast<ThreadSafetyWaiterActor1*>(this)->ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		delete static_cast<ThreadSafetyWaiterActor1*>(this);
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	FutureStream<Void> f;
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int n;
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int32_t* count;
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via threadSafetyWaiter()
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ThreadSafetyWaiterActor1 final : public Actor<void>, public ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >, public FastAllocated<ThreadSafetyWaiterActor1>, public ThreadSafetyWaiterActor1State<ThreadSafetyWaiterActor1> {
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ThreadSafetyWaiterActor1>::operator new;
	using FastAllocated<ThreadSafetyWaiterActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorSingleCallback< ThreadSafetyWaiterActor1, 0, Void >;
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ThreadSafetyWaiterActor1(FutureStream<Void> const& f,int const& n,int32_t* const& count) 
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<void>(),
		   ThreadSafetyWaiterActor1State<ThreadSafetyWaiterActor1>(f, n, count)
	{
		fdb_probe_actor_enter("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("threadSafetyWaiter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("threadSafetyWaiter", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
void threadSafetyWaiter( FutureStream<Void> const& f, int const& n, int32_t* const& count ) {
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	new ThreadSafetyWaiterActor1(f, n, count);
															#line 1382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

#if 0
void threadSafetyTest() {
	double t = timer();

	int N = 10000, V = 100;

	std::vector<Promise<Void>> v;
	Event start, ready;
	Future<Void> sender = inThread<Void>( [&] { return threadSafetySender( v, start, ready, N ); } );

	for(int i=0; i<N; i++) {
		v.clear();
		for (int j = 0; j < V; j++)
			v.push_back(Promise<Void>());
		std::vector<Future<Void>> f( v.size() );
		for(int i=0; i<v.size(); i++)
			f[i] = v[i].getFuture();
		std::random_shuffle( f.begin(), f.end() );

		start.set();
		int32_t count = 0;
		for(int i=0; i<f.size(); i++)
			threadSafetyWaiter( f[i], &count );
		ready.block();

		if (count != V)
			std::cout << "Thread safety error: " << count << std::endl;
	}

	t = timer()-t;
	std::cout << "Thread safety test (2t): " << (V*N/1e6/t) << "M/sec" << std::endl;
}

void threadSafetyTest2() {
	double t = timer();

	int N = 1000, V = 100;

	// std::vector<PromiseStream<Void>> streams( 100 );
	std::vector<PromiseStream<Void>> streams;
	for (int i = 0; i < 100; i++)
		streams.push_back(PromiseStream<Void>());
	std::vector<PromiseStream<Void>> v;
	Event start, ready;
	Future<Void> sender = inThread<Void>( [&] { return threadSafetySender( v, start, ready, N ); } );

	for(int i=0; i<N; i++) {
		std::vector<int> counts( streams.size() );
		v.clear();
		for(int k=0; k<V; k++) {
			int i = deterministicRandom()->randomInt(0, (int)streams.size());
			counts[i]++;
			v.push_back( streams[i] );
		}

		start.set();
		int32_t count = 0;
		for(int i=0; i<streams.size(); i++)
			threadSafetyWaiter( streams[i].getFuture(), counts[i], &count );
		ready.block();

		if (count != V)
			std::cout << "Thread safety error: " << count << std::endl;
	}

	t = timer()-t;
	std::cout << "Thread safety test 2 (2t): " << (V*N/1e6/t) << "M/sec" << std::endl;
}

volatile int32_t cancelled = 0, returned = 0;

															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via returnCancelRacer()
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ReturnCancelRacerActor>
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ReturnCancelRacerActorState {
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ReturnCancelRacerActorState(Future<Void> const& f) 
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : f(f)
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("returnCancelRacer", reinterpret_cast<unsigned long>(this));

	}
	~ReturnCancelRacerActorState() 
	{
		fdb_probe_actor_destroy("returnCancelRacer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				StrictFuture<Void> __when_expr_0 = f;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (static_cast<ReturnCancelRacerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReturnCancelRacerActor*>(this)->actor_wait_state = 1;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReturnCancelRacerActor, 0, Void >*>(static_cast<ReturnCancelRacerActor*>(this)));
															#line 1495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ReturnCancelRacerActorState();
		static_cast<ReturnCancelRacerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		interlockedIncrement( &returned );
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ReturnCancelRacerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReturnCancelRacerActorState(); static_cast<ReturnCancelRacerActor*>(this)->destroy(); return 0; }
															#line 1526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ReturnCancelRacerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReturnCancelRacerActorState();
		static_cast<ReturnCancelRacerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			interlockedIncrement( &cancelled );
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			return a_body1Catch1(__current_error, loopDepth);
															#line 1541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<ReturnCancelRacerActor*>(this)->actor_wait_state > 0) static_cast<ReturnCancelRacerActor*>(this)->actor_wait_state = 0;
		static_cast<ReturnCancelRacerActor*>(this)->ActorCallback< ReturnCancelRacerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReturnCancelRacerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("returnCancelRacer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnCancelRacer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReturnCancelRacerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("returnCancelRacer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnCancelRacer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReturnCancelRacerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("returnCancelRacer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("returnCancelRacer", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
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
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<Void> f;
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via returnCancelRacer()
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ReturnCancelRacerActor final : public Actor<Void>, public ActorCallback< ReturnCancelRacerActor, 0, Void >, public FastAllocated<ReturnCancelRacerActor>, public ReturnCancelRacerActorState<ReturnCancelRacerActor> {
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ReturnCancelRacerActor>::operator new;
	using FastAllocated<ReturnCancelRacerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReturnCancelRacerActor, 0, Void >;
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ReturnCancelRacerActor(Future<Void> const& f) 
															#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   ReturnCancelRacerActorState<ReturnCancelRacerActor>(f)
	{
		fdb_probe_actor_enter("returnCancelRacer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("returnCancelRacer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("returnCancelRacer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReturnCancelRacerActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> returnCancelRacer( Future<Void> const& f ) {
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new ReturnCancelRacerActor(f));
															#line 1685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

void returnCancelRaceTest() {
	int N = 100, M = 100;
	for(int i=0; i<N; i++) {
		std::vector< Promise<Void> > promises;
		std::vector< Future<Void> > futures;
		for(int i=0; i < M; i++) {
			promises.push_back( Promise<Void>() );
			futures.push_back( returnCancelRacer( promises.back().getFuture() ) );
		}
		std::random_shuffle( futures.begin(), futures.end() );

		// FIXME: Doesn't work as written with auto-reset
		// events. Probably not particularly racy as written. Test may
		// FAIL or PASS at whim.

		Event ev1, ev2;
		ThreadFuture<Void> b = inThread<Void>( [&] ()->Void {
			ev1.block();
			for(int i=0; i<promises.size(); i++)
				futures[i] = Future<Void>();
			return Void();
		} );
		ThreadFuture<Void> a = inThread<Void>([&]()->Void {
			ev2.block();
			for(int i=0; i<promises.size(); i++) {
				promises[i].send(Void());
				for( volatile int32_t dummy = 0; dummy < 10; dummy ++ );
			}
			return Void();
		} );
		ev1.set(); ev2.set();
		a.getBlocking();
		b.getBlocking();
	}

	bool ok = cancelled && returned && cancelled+returned == N*M;
	printf("ReturnCancelRaceTest: %s\n", ok ? "PASS" : "FAIL");
	printf("  %d cancelled, %d returned\n", cancelled, returned);
}
#endif

															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via chooseTest()
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ChooseTestActor>
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ChooseTestActorState {
															#line 1738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ChooseTestActorState(Future<int> const& a,Future<int> const& b) 
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : a(a),
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   b(b)
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("chooseTest", reinterpret_cast<unsigned long>(this));

	}
	~ChooseTestActorState() 
	{
		fdb_probe_actor_destroy("chooseTest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_0 = a;
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<ChooseTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_1 = b;
															#line 1768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ChooseTestActor*>(this)->actor_wait_state = 1;
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChooseTestActor, 0, int >*>(static_cast<ChooseTestActor*>(this)));
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChooseTestActor, 1, int >*>(static_cast<ChooseTestActor*>(this)));
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ChooseTestActorState();
		static_cast<ChooseTestActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(int const& A,int loopDepth) 
	{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ChooseTestActor*>(this)->SAV<int>::futures) { (void)(A); this->~ChooseTestActorState(); static_cast<ChooseTestActor*>(this)->destroy(); return 0; }
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ChooseTestActor*>(this)->SAV< int >::value()) int(A);
		this->~ChooseTestActorState();
		static_cast<ChooseTestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int && A,int loopDepth) 
	{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ChooseTestActor*>(this)->SAV<int>::futures) { (void)(A); this->~ChooseTestActorState(); static_cast<ChooseTestActor*>(this)->destroy(); return 0; }
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ChooseTestActor*>(this)->SAV< int >::value()) int(A);
		this->~ChooseTestActorState();
		static_cast<ChooseTestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(int const& B,int loopDepth) 
	{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ChooseTestActor*>(this)->SAV<int>::futures) { (void)(B); this->~ChooseTestActorState(); static_cast<ChooseTestActor*>(this)->destroy(); return 0; }
															#line 1822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ChooseTestActor*>(this)->SAV< int >::value()) int(B);
		this->~ChooseTestActorState();
		static_cast<ChooseTestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(int && B,int loopDepth) 
	{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ChooseTestActor*>(this)->SAV<int>::futures) { (void)(B); this->~ChooseTestActorState(); static_cast<ChooseTestActor*>(this)->destroy(); return 0; }
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ChooseTestActor*>(this)->SAV< int >::value()) int(B);
		this->~ChooseTestActorState();
		static_cast<ChooseTestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChooseTestActor*>(this)->actor_wait_state > 0) static_cast<ChooseTestActor*>(this)->actor_wait_state = 0;
		static_cast<ChooseTestActor*>(this)->ActorCallback< ChooseTestActor, 0, int >::remove();
		static_cast<ChooseTestActor*>(this)->ActorCallback< ChooseTestActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< ChooseTestActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChooseTestActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChooseTestActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChooseTestActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChooseTestActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChooseTestActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> a;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> b;
															#line 1943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via chooseTest()
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ChooseTestActor final : public Actor<int>, public ActorCallback< ChooseTestActor, 0, int >, public ActorCallback< ChooseTestActor, 1, int >, public FastAllocated<ChooseTestActor>, public ChooseTestActorState<ChooseTestActor> {
															#line 1948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ChooseTestActor>::operator new;
	using FastAllocated<ChooseTestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChooseTestActor, 0, int >;
friend struct ActorCallback< ChooseTestActor, 1, int >;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ChooseTestActor(Future<int> const& a,Future<int> const& b) 
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   ChooseTestActorState<ChooseTestActor>(a, b)
	{
		fdb_probe_actor_enter("chooseTest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("chooseTest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("chooseTest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChooseTestActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> chooseTest( Future<int> const& a, Future<int> const& b ) {
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new ChooseTestActor(a, b));
															#line 1988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

void showArena(ArenaBlock* a, ArenaBlock* parent) {
	printf("ArenaBlock %p (<-%p): %d bytes, %d refs\n", a, parent, a->size(), a->debugGetReferenceCount());
	if (!a->isTiny()) {
		int o = a->nextBlockOffset;
		while (o) {
			ArenaBlockRef* r = (ArenaBlockRef*)((char*)a->getData() + o);

			// If alignedBuffer is valid then print its pointer and size, else recurse
			if (r->aligned4kBufferSize != 0) {
				printf("AlignedBuffer %p (<-%p) %u bytes\n", r->aligned4kBuffer, a, r->aligned4kBufferSize);
			} else {
				showArena(r->next, a);
			}

			o = r->nextBlockOffset;
		}
	}
}

void arenaTest() {
	BinaryWriter wr(AssumeVersion(g_network->protocolVersion()));
	{
		Arena arena;
		VectorRef<StringRef> test;
		test.push_back(arena, StringRef(arena, "Hello"_sr));
		test.push_back(arena, StringRef(arena, ", "_sr));
		test.push_back(arena, StringRef(arena, "World!"_sr));

		for (auto i = test.begin(); i != test.end(); ++i)
			for (auto j = i->begin(); j != i->end(); ++j)
				std::cout << *j;
		std::cout << std::endl;

		wr << test;
	}
	{
		Arena arena2;
		VectorRef<StringRef> test2;
		BinaryReader reader(wr.getData(), wr.getLength(), AssumeVersion(g_network->protocolVersion()));
		reader >> test2 >> arena2;

		for (auto i = test2.begin(); i != test2.end(); ++i)
			for (auto j = i->begin(); j != i->end(); ++j)
				std::cout << *j;
		std::cout << std::endl;
	}

	double t = timer();
	for (int i = 0; i < 100; i++) {
		Arena ar;
		for (int i = 0; i < 10000000; i++)
			new (ar) char[10];
	}
	printf("100 x 10M x 10B allocated+freed from Arenas: %f sec\n", timer() - t);

	// printf("100M x 8bytes allocations: %d bytes used\n", 0);//ar.getSize());
	// showArena( ar.impl.getPtr(), 0 );
};

															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via testStream()
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class TestStreamActor>
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class TestStreamActorState {
															#line 2059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	TestStreamActorState(FutureStream<int> const& xs) 
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : xs(xs)
															#line 2066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("testStream", reinterpret_cast<unsigned long>(this));

	}
	~TestStreamActorState() 
	{
		fdb_probe_actor_destroy("testStream", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 2081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		delete static_cast<TestStreamActor*>(this);
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
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<int> __when_expr_0 = xs;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 2112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		static_cast<TestStreamActor*>(this)->actor_wait_state = 1;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< TestStreamActor, 0, int >*>(static_cast<TestStreamActor*>(this)));
															#line 2116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int const& x,int loopDepth) 
	{
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		std::cout << x << std::endl;
															#line 2125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int && x,int loopDepth) 
	{
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		std::cout << x << std::endl;
															#line 2134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(int const& x,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(x, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(int && x,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(x), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TestStreamActor*>(this)->actor_wait_state > 0) static_cast<TestStreamActor*>(this)->actor_wait_state = 0;
		static_cast<TestStreamActor*>(this)->ActorSingleCallback< TestStreamActor, 0, int >::remove();

	}
	void a_callback_fire(ActorSingleCallback< TestStreamActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("testStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testStream", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< TestStreamActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("testStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testStream", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< TestStreamActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("testStream", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testStream", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	FutureStream<int> xs;
															#line 2204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via testStream()
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class TestStreamActor final : public Actor<void>, public ActorSingleCallback< TestStreamActor, 0, int >, public FastAllocated<TestStreamActor>, public TestStreamActorState<TestStreamActor> {
															#line 2209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<TestStreamActor>::operator new;
	using FastAllocated<TestStreamActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorSingleCallback< TestStreamActor, 0, int >;
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	TestStreamActor(FutureStream<int> const& xs) 
															#line 2220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<void>(),
		   TestStreamActorState<TestStreamActor>(xs)
	{
		fdb_probe_actor_enter("testStream", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testStream");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testStream", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
void testStream( FutureStream<int> const& xs ) {
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	new TestStreamActor(xs);
															#line 2239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 2244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest1()
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest1Actor>
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest1ActorState {
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest1ActorState(bool const& b) 
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : b(b)
															#line 2258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest1", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest1ActorState() 
	{
		fdb_probe_actor_destroy("actorTest1", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("1");
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (b)
															#line 2275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			{
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				return a_body1Catch1(future_version(), loopDepth);
															#line 2279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			}
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest1Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorTest1ActorState(); static_cast<ActorTest1Actor*>(this)->destroy(); return 0; }
															#line 2283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest1Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ActorTest1ActorState();
			static_cast<ActorTest1Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~ActorTest1ActorState();
		static_cast<ActorTest1Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool b;
															#line 2307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest1()
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest1Actor final : public Actor<Void>, public FastAllocated<ActorTest1Actor>, public ActorTest1ActorState<ActorTest1Actor> {
															#line 2312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest1Actor>::operator new;
	using FastAllocated<ActorTest1Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest1Actor(bool const& b) 
															#line 2322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   ActorTest1ActorState<ActorTest1Actor>(b)
	{
		fdb_probe_actor_enter("actorTest1", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest1");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest1", reinterpret_cast<unsigned long>(this), -1);

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
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> actorTest1( bool const& b ) {
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new ActorTest1Actor(b));
															#line 2349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 2354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest2()
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest2Actor>
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest2ActorState {
															#line 2361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest2ActorState(bool const& b) 
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : b(b)
															#line 2368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest2", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest2ActorState() 
	{
		fdb_probe_actor_destroy("actorTest2", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("2");
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (b)
															#line 2385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			{
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				return a_body1Catch1(future_version(), loopDepth);
															#line 2389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			}
			loopDepth = a_body1cont3(loopDepth);
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
		delete static_cast<ActorTest2Actor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		delete static_cast<ActorTest2Actor*>(this);
															#line 2412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool b;
															#line 2419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest2()
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest2Actor final : public Actor<void>, public FastAllocated<ActorTest2Actor>, public ActorTest2ActorState<ActorTest2Actor> {
															#line 2424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest2Actor>::operator new;
	using FastAllocated<ActorTest2Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest2Actor(bool const& b) 
															#line 2434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<void>(),
		   ActorTest2ActorState<ActorTest2Actor>(b)
	{
		fdb_probe_actor_enter("actorTest2", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest2");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest2", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
void actorTest2( bool const& b ) {
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	new ActorTest2Actor(b);
															#line 2453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 2458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest3()
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest3Actor>
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest3ActorState {
															#line 2465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest3ActorState(bool const& b) 
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : b(b)
															#line 2472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest3", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest3ActorState() 
	{
		fdb_probe_actor_destroy("actorTest3", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (b)
															#line 2488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					return a_body1Catch2(future_version(), loopDepth);
															#line 2492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				}
				loopDepth = a_body1cont4(loopDepth);
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
		this->~ActorTest3ActorState();
		static_cast<ActorTest3Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		printf("\nactorTest3 failed\n");
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ActorTest3Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorTest3ActorState(); static_cast<ActorTest3Actor*>(this)->destroy(); return 0; }
															#line 2524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ActorTest3Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ActorTest3ActorState();
		static_cast<ActorTest3Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("3");
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest3Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorTest3ActorState(); static_cast<ActorTest3Actor*>(this)->destroy(); return 0; }
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest3Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ActorTest3ActorState();
			static_cast<ActorTest3Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
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
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool b;
															#line 2568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest3()
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest3Actor final : public Actor<Void>, public FastAllocated<ActorTest3Actor>, public ActorTest3ActorState<ActorTest3Actor> {
															#line 2573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest3Actor>::operator new;
	using FastAllocated<ActorTest3Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest3Actor(bool const& b) 
															#line 2583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   ActorTest3ActorState<ActorTest3Actor>(b)
	{
		fdb_probe_actor_enter("actorTest3", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest3");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest3", reinterpret_cast<unsigned long>(this), -1);

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
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> actorTest3( bool const& b ) {
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new ActorTest3Actor(b));
															#line 2610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 2615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest4()
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest4Actor>
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest4ActorState {
															#line 2622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest4ActorState(bool const& b) 
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : b(b),
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   tstart(now())
															#line 2631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest4", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest4ActorState() 
	{
		fdb_probe_actor_destroy("actorTest4", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (b)
															#line 2647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				{
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					return a_body1Catch2(operation_failed(), loopDepth);
															#line 2651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				}
				loopDepth = a_body1cont4(loopDepth);
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
		this->~ActorTest4ActorState();
		static_cast<ActorTest4Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (now() < tstart + 1)
															#line 2681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("actorTest4 failed");
															#line 2685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		}
		else
		{
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("4");
															#line 2691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		}
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ActorTest4Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorTest4ActorState(); static_cast<ActorTest4Actor*>(this)->destroy(); return 0; }
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ActorTest4Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ActorTest4ActorState();
		static_cast<ActorTest4Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(1);
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<ActorTest4Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1Catch2when1(__when_expr_0.get(), loopDepth); };
			static_cast<ActorTest4Actor*>(this)->actor_wait_state = 1;
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorTest4Actor, 0, Void >*>(static_cast<ActorTest4Actor*>(this)));
															#line 2715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
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
	int a_body1Catch2cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1Catch2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1Catch2cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorTest4Actor*>(this)->actor_wait_state > 0) static_cast<ActorTest4Actor*>(this)->actor_wait_state = 0;
		static_cast<ActorTest4Actor*>(this)->ActorCallback< ActorTest4Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ActorTest4Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("actorTest4", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest4", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ActorTest4Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("actorTest4", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest4", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ActorTest4Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("actorTest4", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest4", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool b;
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	double tstart;
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest4()
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest4Actor final : public Actor<Void>, public ActorCallback< ActorTest4Actor, 0, Void >, public FastAllocated<ActorTest4Actor>, public ActorTest4ActorState<ActorTest4Actor> {
															#line 2823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest4Actor>::operator new;
	using FastAllocated<ActorTest4Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorTest4Actor, 0, Void >;
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest4Actor(bool const& b) 
															#line 2834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   ActorTest4ActorState<ActorTest4Actor>(b)
	{
		fdb_probe_actor_enter("actorTest4", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest4");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest4", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ActorTest4Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> actorTest4( bool const& b ) {
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new ActorTest4Actor(b));
															#line 2862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 2867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest5()
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest5Actor>
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest5ActorState {
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest5ActorState() 
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : caught(false)
															#line 2881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest5", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest5ActorState() 
	{
		fdb_probe_actor_destroy("actorTest5", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 2896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ActorTest5ActorState();
		static_cast<ActorTest5Actor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		;
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		inloop = false;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (caught)
															#line 2944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("5");
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest5Actor*>(this)->SAV<bool>::futures) { (void)(true); this->~ActorTest5ActorState(); static_cast<ActorTest5Actor*>(this)->destroy(); return 0; }
															#line 2950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest5Actor*>(this)->SAV< bool >::value()) bool(true);
			this->~ActorTest5ActorState();
			static_cast<ActorTest5Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			for(;;) {
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (inloop)
															#line 2961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				{
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					printf("\nactorTest5 failed\n");
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					if (!static_cast<ActorTest5Actor*>(this)->SAV<bool>::futures) { (void)(false); this->~ActorTest5ActorState(); static_cast<ActorTest5Actor*>(this)->destroy(); return 0; }
															#line 2967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
					new (&static_cast<ActorTest5Actor*>(this)->SAV< bool >::value()) bool(false);
					this->~ActorTest5ActorState();
					static_cast<ActorTest5Actor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				inloop = true;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (1)
															#line 2977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				{
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					return a_body1loopBody1loopBody1Catch1(operation_failed(), loopDepth);
															#line 2981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				}
			}
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			caught = true;
															#line 3004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool caught;
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool inloop;
															#line 3019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest5()
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest5Actor final : public Actor<bool>, public FastAllocated<ActorTest5Actor>, public ActorTest5ActorState<ActorTest5Actor> {
															#line 3024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest5Actor>::operator new;
	using FastAllocated<ActorTest5Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest5Actor() 
															#line 3034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<bool>(),
		   ActorTest5ActorState<ActorTest5Actor>()
	{
		fdb_probe_actor_enter("actorTest5", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest5");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest5", reinterpret_cast<unsigned long>(this), -1);

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
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<bool> actorTest5(  ) {
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<bool>(new ActorTest5Actor());
															#line 3061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 3066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest6()
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest6Actor>
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest6ActorState {
															#line 3073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest6ActorState() 
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : caught(false)
															#line 3080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest6", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest6ActorState() 
	{
		fdb_probe_actor_destroy("actorTest6", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 3095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ActorTest6ActorState();
		static_cast<ActorTest6Actor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (caught)
															#line 3125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("6");
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest6Actor*>(this)->SAV<bool>::futures) { (void)(true); this->~ActorTest6ActorState(); static_cast<ActorTest6Actor*>(this)->destroy(); return 0; }
															#line 3131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest6Actor*>(this)->SAV< bool >::value()) bool(true);
			this->~ActorTest6ActorState();
			static_cast<ActorTest6Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (1)
															#line 3140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			{
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				return a_body1loopBody1Catch1(operation_failed(), loopDepth);
															#line 3144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			caught = true;
															#line 3167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool caught;
															#line 3193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest6()
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest6Actor final : public Actor<bool>, public FastAllocated<ActorTest6Actor>, public ActorTest6ActorState<ActorTest6Actor> {
															#line 3198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest6Actor>::operator new;
	using FastAllocated<ActorTest6Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest6Actor() 
															#line 3208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<bool>(),
		   ActorTest6ActorState<ActorTest6Actor>()
	{
		fdb_probe_actor_enter("actorTest6", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest6");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest6", reinterpret_cast<unsigned long>(this), -1);

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
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<bool> actorTest6(  ) {
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<bool>(new ActorTest6Actor());
															#line 3235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 3240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest7()
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest7Actor>
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest7ActorState {
															#line 3247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest7ActorState() 
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	{
															#line 3253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		fdb_probe_actor_create("actorTest7", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest7ActorState() 
	{
		fdb_probe_actor_destroy("actorTest7", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				for(;;) {
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					for(;;) {
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
						if (1)
															#line 3272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
						{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
							return a_body1Catch2(operation_failed(), loopDepth);
															#line 3276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
						}
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
						if (1)
															#line 3280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
						{
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
							printf("actorTest7 failed (1)\n");
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
							if (!static_cast<ActorTest7Actor*>(this)->SAV<bool>::futures) { (void)(false); this->~ActorTest7ActorState(); static_cast<ActorTest7Actor*>(this)->destroy(); return 0; }
															#line 3286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
							new (&static_cast<ActorTest7Actor*>(this)->SAV< bool >::value()) bool(false);
							this->~ActorTest7ActorState();
							static_cast<ActorTest7Actor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
						if (0)
															#line 3294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
						{
							break;
						}
					}
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
					if (1)
															#line 3301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
					{
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
						printf("actorTest7 failed (2)\n");
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
						if (!static_cast<ActorTest7Actor*>(this)->SAV<bool>::futures) { (void)(false); this->~ActorTest7ActorState(); static_cast<ActorTest7Actor*>(this)->destroy(); return 0; }
															#line 3307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
						new (&static_cast<ActorTest7Actor*>(this)->SAV< bool >::value()) bool(false);
						this->~ActorTest7ActorState();
						static_cast<ActorTest7Actor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
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
		this->~ActorTest7ActorState();
		static_cast<ActorTest7Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("7");
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest7Actor*>(this)->SAV<bool>::futures) { (void)(true); this->~ActorTest7ActorState(); static_cast<ActorTest7Actor*>(this)->destroy(); return 0; }
															#line 3344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest7Actor*>(this)->SAV< bool >::value()) bool(true);
			this->~ActorTest7ActorState();
			static_cast<ActorTest7Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
};
// This generated class is to be used only via actorTest7()
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest7Actor final : public Actor<bool>, public FastAllocated<ActorTest7Actor>, public ActorTest7ActorState<ActorTest7Actor> {
															#line 3362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest7Actor>::operator new;
	using FastAllocated<ActorTest7Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest7Actor() 
															#line 3372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<bool>(),
		   ActorTest7ActorState<ActorTest7Actor>()
	{
		fdb_probe_actor_enter("actorTest7", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest7");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest7", reinterpret_cast<unsigned long>(this), -1);

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
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<bool> actorTest7(  ) {
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<bool>(new ActorTest7Actor());
															#line 3399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 3404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest8()
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest8Actor>
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest8ActorState {
															#line 3411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest8ActorState() 
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : caught(false),
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   set(true)
															#line 3420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest8", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest8ActorState() 
	{
		fdb_probe_actor_destroy("actorTest8", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 3435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ActorTest8ActorState();
		static_cast<ActorTest8Actor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		inloop = false;
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (caught)
															#line 3467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("8");
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest8Actor*>(this)->SAV<bool>::futures) { (void)(true); this->~ActorTest8ActorState(); static_cast<ActorTest8Actor*>(this)->destroy(); return 0; }
															#line 3473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest8Actor*>(this)->SAV< bool >::value()) bool(true);
			this->~ActorTest8ActorState();
			static_cast<ActorTest8Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 3482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			loopDepth = a_body1loopBody1loopHead1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			caught = true;
															#line 3504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (inloop)
															#line 3526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("\nactorTest8 failed\n");
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest8Actor*>(this)->SAV<bool>::futures) { (void)(false); this->~ActorTest8ActorState(); static_cast<ActorTest8Actor*>(this)->destroy(); return 0; }
															#line 3532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest8Actor*>(this)->SAV< bool >::value()) bool(false);
			this->~ActorTest8ActorState();
			static_cast<ActorTest8Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		StrictFuture<bool> __when_expr_0 = set;
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<ActorTest8Actor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ActorTest8Actor*>(this)->actor_wait_state = 1;
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorTest8Actor, 0, bool >*>(static_cast<ActorTest8Actor*>(this)));
															#line 3547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(bool const& b,int loopDepth) 
	{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		inloop = true;
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (1)
															#line 3558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			return a_body1loopBody1Catch1(operation_failed(), std::max(0, loopDepth - 1));
															#line 3562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(bool && b,int loopDepth) 
	{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		inloop = true;
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (1)
															#line 3574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			return a_body1loopBody1Catch1(operation_failed(), std::max(0, loopDepth - 1));
															#line 3578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(bool const& b,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(b, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(bool && b,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(std::move(b), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorTest8Actor*>(this)->actor_wait_state > 0) static_cast<ActorTest8Actor*>(this)->actor_wait_state = 0;
		static_cast<ActorTest8Actor*>(this)->ActorCallback< ActorTest8Actor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< ActorTest8Actor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("actorTest8", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest8", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ActorTest8Actor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("actorTest8", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest8", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ActorTest8Actor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("actorTest8", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest8", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool caught;
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<bool> set;
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	bool inloop;
															#line 3653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest8()
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest8Actor final : public Actor<bool>, public ActorCallback< ActorTest8Actor, 0, bool >, public FastAllocated<ActorTest8Actor>, public ActorTest8ActorState<ActorTest8Actor> {
															#line 3658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest8Actor>::operator new;
	using FastAllocated<ActorTest8Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorTest8Actor, 0, bool >;
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest8Actor() 
															#line 3669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<bool>(),
		   ActorTest8ActorState<ActorTest8Actor>()
	{
		fdb_probe_actor_enter("actorTest8", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest8");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest8", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ActorTest8Actor, 0, bool >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<bool> actorTest8(  ) {
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<bool>(new ActorTest8Actor());
															#line 3697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 3702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest9A()
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest9AActor>
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest9AActorState {
															#line 3709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest9AActorState(Future<Void> const& setAfterCalling) 
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : setAfterCalling(setAfterCalling),
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   count(0)
															#line 3718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest9A", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest9AActorState() 
	{
		fdb_probe_actor_destroy("actorTest9A", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 3733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ActorTest9AActorState();
		static_cast<ActorTest9AActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (count == 4)
															#line 3763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("9");
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest9AActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ActorTest9AActorState(); static_cast<ActorTest9AActor*>(this)->destroy(); return 0; }
															#line 3769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest9AActor*>(this)->SAV< bool >::value()) bool(true);
			this->~ActorTest9AActorState();
			static_cast<ActorTest9AActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (count && count != 4)
															#line 3777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			printf("\nactorTest9 failed\n");
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<ActorTest9AActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ActorTest9AActorState(); static_cast<ActorTest9AActor*>(this)->destroy(); return 0; }
															#line 3783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<ActorTest9AActor*>(this)->SAV< bool >::value()) bool(false);
			this->~ActorTest9AActorState();
			static_cast<ActorTest9AActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		;
															#line 3791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		;
															#line 3813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		count++;
															#line 3835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1(int loopDepth) 
	{
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		StrictFuture<Void> __when_expr_0 = setAfterCalling;
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<ActorTest9AActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 3));
															#line 3853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 3)); else return a_body1loopBody1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ActorTest9AActor*>(this)->actor_wait_state = 1;
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorTest9AActor, 0, Void >*>(static_cast<ActorTest9AActor*>(this)));
															#line 3858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		;
															#line 3880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		;
															#line 3889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorTest9AActor*>(this)->actor_wait_state > 0) static_cast<ActorTest9AActor*>(this)->actor_wait_state = 0;
		static_cast<ActorTest9AActor*>(this)->ActorCallback< ActorTest9AActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ActorTest9AActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ActorTest9AActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ActorTest9AActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1loopBody1loopBody1cont2(int loopDepth) 
	{
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		count++;
															#line 3961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return a_body1loopBody1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1loopBody1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1loopBody1(int loopDepth) 
	{
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		for(;;) {
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			count++;
															#line 3979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			break;
		}
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		StrictFuture<Void> __when_expr_1 = Future<Void>(Void());
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<ActorTest9AActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 4));
															#line 3986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 4)); else return a_body1loopBody1loopBody1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ActorTest9AActor*>(this)->actor_wait_state = 2;
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ActorTest9AActor, 1, Void >*>(static_cast<ActorTest9AActor*>(this)));
															#line 3991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1loopBody1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 3));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 3));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		count++;
															#line 4013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return a_body1loopBody1loopBody1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		count++;
															#line 4022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		return a_body1loopBody1loopBody1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ActorTest9AActor*>(this)->actor_wait_state > 0) static_cast<ActorTest9AActor*>(this)->actor_wait_state = 0;
		static_cast<ActorTest9AActor*>(this)->ActorCallback< ActorTest9AActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ActorTest9AActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ActorTest9AActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ActorTest9AActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<Void> setAfterCalling;
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int count;
															#line 4094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest9A()
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest9AActor final : public Actor<bool>, public ActorCallback< ActorTest9AActor, 0, Void >, public ActorCallback< ActorTest9AActor, 1, Void >, public FastAllocated<ActorTest9AActor>, public ActorTest9AActorState<ActorTest9AActor> {
															#line 4099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest9AActor>::operator new;
	using FastAllocated<ActorTest9AActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorTest9AActor, 0, Void >;
friend struct ActorCallback< ActorTest9AActor, 1, Void >;
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest9AActor(Future<Void> const& setAfterCalling) 
															#line 4111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<bool>(),
		   ActorTest9AActorState<ActorTest9AActor>(setAfterCalling)
	{
		fdb_probe_actor_enter("actorTest9A", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest9A");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest9A", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ActorTest9AActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ActorTest9AActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<bool> actorTest9A( Future<Void> const& setAfterCalling ) {
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<bool>(new ActorTest9AActor(setAfterCalling));
															#line 4140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

Future<bool> actorTest9() {
	Promise<Void> p;
	Future<bool> f = actorTest9A(p.getFuture());
	p.send(Void());
	return f;
}

															#line 4152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via actorTest10A()
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class ActorTest10AActor>
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest10AActorState {
															#line 4159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest10AActorState(FutureStream<int> const& inputStream,Future<Void> const& go) 
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : inputStream(inputStream),
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   go(go),
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   i()
															#line 4170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("actorTest10A", reinterpret_cast<unsigned long>(this));

	}
	~ActorTest10AActorState() 
	{
		fdb_probe_actor_destroy("actorTest10A", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			i = 0;
															#line 4185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~ActorTest10AActorState();
		static_cast<ActorTest10AActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<ActorTest10AActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ActorTest10AActorState(); static_cast<ActorTest10AActor*>(this)->destroy(); return 0; }
															#line 4208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<ActorTest10AActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ActorTest10AActorState();
		static_cast<ActorTest10AActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!(i < 5))
															#line 4227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		StrictFuture<Void> __when_expr_0 = go;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<ActorTest10AActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ActorTest10AActor*>(this)->actor_wait_state = 1;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorTest10AActor, 0, Void >*>(static_cast<ActorTest10AActor*>(this)));
															#line 4240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<int> __when_expr_1 = inputStream;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<ActorTest10AActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.pop(), loopDepth); };
		static_cast<ActorTest10AActor*>(this)->actor_wait_state = 2;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< ActorTest10AActor, 1, int >*>(static_cast<ActorTest10AActor*>(this)));
															#line 4269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<int> __when_expr_1 = inputStream;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<ActorTest10AActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.pop(), loopDepth); };
		static_cast<ActorTest10AActor*>(this)->actor_wait_state = 2;
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< ActorTest10AActor, 1, int >*>(static_cast<ActorTest10AActor*>(this)));
															#line 4285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		if (static_cast<ActorTest10AActor*>(this)->actor_wait_state > 0) static_cast<ActorTest10AActor*>(this)->actor_wait_state = 0;
		static_cast<ActorTest10AActor*>(this)->ActorCallback< ActorTest10AActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ActorTest10AActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ActorTest10AActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ActorTest10AActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int const& input,int loopDepth) 
	{
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		i++;
															#line 4357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(int && input,int loopDepth) 
	{
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		i++;
															#line 4366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(int const& input,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(input, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(int && input,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(input), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ActorTest10AActor*>(this)->actor_wait_state > 0) static_cast<ActorTest10AActor*>(this)->actor_wait_state = 0;
		static_cast<ActorTest10AActor*>(this)->ActorSingleCallback< ActorTest10AActor, 1, int >::remove();

	}
	void a_callback_fire(ActorSingleCallback< ActorTest10AActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< ActorTest10AActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorSingleCallback< ActorTest10AActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	FutureStream<int> inputStream;
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<Void> go;
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int i;
															#line 4440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via actorTest10A()
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class ActorTest10AActor final : public Actor<Void>, public ActorCallback< ActorTest10AActor, 0, Void >, public ActorSingleCallback< ActorTest10AActor, 1, int >, public FastAllocated<ActorTest10AActor>, public ActorTest10AActorState<ActorTest10AActor> {
															#line 4445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<ActorTest10AActor>::operator new;
	using FastAllocated<ActorTest10AActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorTest10AActor, 0, Void >;
friend struct ActorSingleCallback< ActorTest10AActor, 1, int >;
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	ActorTest10AActor(FutureStream<int> const& inputStream,Future<Void> const& go) 
															#line 4457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   ActorTest10AActorState<ActorTest10AActor>(inputStream, go)
	{
		fdb_probe_actor_enter("actorTest10A", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorTest10A");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorTest10A", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ActorTest10AActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorSingleCallback< ActorTest10AActor, 1, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> actorTest10A( FutureStream<int> const& inputStream, Future<Void> const& go ) {
															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new ActorTest10AActor(inputStream, go));
															#line 4486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

void actorTest10() {
	PromiseStream<int> ins;
	Promise<Void> go;
	for (int x = 0; x < 2; x++)
		ins.send(x);
	Future<Void> a = actorTest10A(ins.getFuture(), go.getFuture());
	go.send(Void());
	for (int x = 0; x < 3; x++)
		ins.send(x);
	if (!a.isReady())
		printf("\nactorTest10 failed\n");
	else
		printf("10");
}

															#line 4506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via cancellable()
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class CancellableActor>
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class CancellableActorState {
															#line 4513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	CancellableActorState() 
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	{
															#line 4519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		fdb_probe_actor_create("cancellable", reinterpret_cast<unsigned long>(this));

	}
	~CancellableActorState() 
	{
		fdb_probe_actor_destroy("cancellable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Never();
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<CancellableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CancellableActor*>(this)->actor_wait_state = 1;
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CancellableActor, 0, Void >*>(static_cast<CancellableActor*>(this)));
															#line 4540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~CancellableActorState();
		static_cast<CancellableActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<CancellableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancellableActorState(); static_cast<CancellableActor*>(this)->destroy(); return 0; }
															#line 4563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<CancellableActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CancellableActorState();
		static_cast<CancellableActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<CancellableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancellableActorState(); static_cast<CancellableActor*>(this)->destroy(); return 0; }
															#line 4575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<CancellableActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CancellableActorState();
		static_cast<CancellableActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<CancellableActor*>(this)->actor_wait_state > 0) static_cast<CancellableActor*>(this)->actor_wait_state = 0;
		static_cast<CancellableActor*>(this)->ActorCallback< CancellableActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CancellableActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancellable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CancellableActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancellable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CancellableActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cancellable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancellable", reinterpret_cast<unsigned long>(this), 0);

	}
};
// This generated class is to be used only via cancellable()
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class CancellableActor final : public Actor<Void>, public ActorCallback< CancellableActor, 0, Void >, public FastAllocated<CancellableActor>, public CancellableActorState<CancellableActor> {
															#line 4650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<CancellableActor>::operator new;
	using FastAllocated<CancellableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CancellableActor, 0, Void >;
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	CancellableActor() 
															#line 4661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   CancellableActorState<CancellableActor>()
	{
		fdb_probe_actor_enter("cancellable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cancellable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cancellable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CancellableActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> cancellable(  ) {
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new CancellableActor());
															#line 4689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 4694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via simple()
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class SimpleActor>
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SimpleActorState {
															#line 4701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SimpleActorState() 
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	{
															#line 4707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		fdb_probe_actor_create("simple", reinterpret_cast<unsigned long>(this));

	}
	~SimpleActorState() 
	{
		fdb_probe_actor_destroy("simple", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<SimpleActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SimpleActorState(); static_cast<SimpleActor*>(this)->destroy(); return 0; }
															#line 4721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<SimpleActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~SimpleActorState();
			static_cast<SimpleActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~SimpleActorState();
		static_cast<SimpleActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
};
// This generated class is to be used only via simple()
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SimpleActor final : public Actor<Void>, public FastAllocated<SimpleActor>, public SimpleActorState<SimpleActor> {
															#line 4747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<SimpleActor>::operator new;
	using FastAllocated<SimpleActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SimpleActor() 
															#line 4757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   SimpleActorState<SimpleActor>()
	{
		fdb_probe_actor_enter("simple", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("simple");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("simple", reinterpret_cast<unsigned long>(this), -1);

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
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> simple(  ) {
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new SimpleActor());
															#line 4784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 4789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via simpleWait()
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class SimpleWaitActor>
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SimpleWaitActorState {
															#line 4796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SimpleWaitActorState() 
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	{
															#line 4802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		fdb_probe_actor_create("simpleWait", reinterpret_cast<unsigned long>(this));

	}
	~SimpleWaitActorState() 
	{
		fdb_probe_actor_destroy("simpleWait", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>(Void());
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<SimpleWaitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SimpleWaitActor*>(this)->actor_wait_state = 1;
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SimpleWaitActor, 0, Void >*>(static_cast<SimpleWaitActor*>(this)));
															#line 4823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~SimpleWaitActorState();
		static_cast<SimpleWaitActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<SimpleWaitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SimpleWaitActorState(); static_cast<SimpleWaitActor*>(this)->destroy(); return 0; }
															#line 4846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<SimpleWaitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SimpleWaitActorState();
		static_cast<SimpleWaitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<SimpleWaitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SimpleWaitActorState(); static_cast<SimpleWaitActor*>(this)->destroy(); return 0; }
															#line 4858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<SimpleWaitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SimpleWaitActorState();
		static_cast<SimpleWaitActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SimpleWaitActor*>(this)->actor_wait_state > 0) static_cast<SimpleWaitActor*>(this)->actor_wait_state = 0;
		static_cast<SimpleWaitActor*>(this)->ActorCallback< SimpleWaitActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SimpleWaitActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("simpleWait", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simpleWait", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SimpleWaitActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("simpleWait", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simpleWait", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SimpleWaitActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("simpleWait", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simpleWait", reinterpret_cast<unsigned long>(this), 0);

	}
};
// This generated class is to be used only via simpleWait()
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SimpleWaitActor final : public Actor<Void>, public ActorCallback< SimpleWaitActor, 0, Void >, public FastAllocated<SimpleWaitActor>, public SimpleWaitActorState<SimpleWaitActor> {
															#line 4933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<SimpleWaitActor>::operator new;
	using FastAllocated<SimpleWaitActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SimpleWaitActor, 0, Void >;
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SimpleWaitActor() 
															#line 4944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   SimpleWaitActorState<SimpleWaitActor>()
	{
		fdb_probe_actor_enter("simpleWait", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("simpleWait");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("simpleWait", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SimpleWaitActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> simpleWait(  ) {
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new SimpleWaitActor());
															#line 4972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 4977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via simpleRet()
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class SimpleRetActor>
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SimpleRetActorState {
															#line 4984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SimpleRetActorState(Future<int> const& x) 
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : x(x)
															#line 4991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("simpleRet", reinterpret_cast<unsigned long>(this));

	}
	~SimpleRetActorState() 
	{
		fdb_probe_actor_destroy("simpleRet", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_0 = x;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<SimpleRetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SimpleRetActor*>(this)->actor_wait_state = 1;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SimpleRetActor, 0, int >*>(static_cast<SimpleRetActor*>(this)));
															#line 5013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~SimpleRetActorState();
		static_cast<SimpleRetActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int const& i,int loopDepth) 
	{
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<SimpleRetActor*>(this)->SAV<int>::futures) { (void)(i); this->~SimpleRetActorState(); static_cast<SimpleRetActor*>(this)->destroy(); return 0; }
															#line 5036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<SimpleRetActor*>(this)->SAV< int >::value()) int(i);
		this->~SimpleRetActorState();
		static_cast<SimpleRetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && i,int loopDepth) 
	{
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<SimpleRetActor*>(this)->SAV<int>::futures) { (void)(i); this->~SimpleRetActorState(); static_cast<SimpleRetActor*>(this)->destroy(); return 0; }
															#line 5048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<SimpleRetActor*>(this)->SAV< int >::value()) int(i);
		this->~SimpleRetActorState();
		static_cast<SimpleRetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int const& i,int loopDepth) 
	{
		loopDepth = a_body1cont1(i, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && i,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(i), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SimpleRetActor*>(this)->actor_wait_state > 0) static_cast<SimpleRetActor*>(this)->actor_wait_state = 0;
		static_cast<SimpleRetActor*>(this)->ActorCallback< SimpleRetActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< SimpleRetActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("simpleRet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simpleRet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SimpleRetActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("simpleRet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simpleRet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SimpleRetActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("simpleRet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simpleRet", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> x;
															#line 5121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via simpleRet()
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class SimpleRetActor final : public Actor<int>, public ActorCallback< SimpleRetActor, 0, int >, public FastAllocated<SimpleRetActor>, public SimpleRetActorState<SimpleRetActor> {
															#line 5126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<SimpleRetActor>::operator new;
	using FastAllocated<SimpleRetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SimpleRetActor, 0, int >;
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	SimpleRetActor(Future<int> const& x) 
															#line 5137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   SimpleRetActorState<SimpleRetActor>(x)
	{
		fdb_probe_actor_enter("simpleRet", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("simpleRet");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("simpleRet", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SimpleRetActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> simpleRet( Future<int> const& x ) {
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new SimpleRetActor(x));
															#line 5165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

template <int i>
Future<int> chain(Future<int> const& x);

															#line 5173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via achain()
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <int i, class AchainActor>
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class AchainActorState {
															#line 5180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	AchainActorState(Future<int> const& x) 
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : x(x)
															#line 5187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("achain", reinterpret_cast<unsigned long>(this));

	}
	~AchainActorState() 
	{
		fdb_probe_actor_destroy("achain", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_0 = chain<i>(x);
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<AchainActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AchainActor*>(this)->actor_wait_state = 1;
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AchainActor, 0, int >*>(static_cast<AchainActor*>(this)));
															#line 5209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~AchainActorState();
		static_cast<AchainActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int const& k,int loopDepth) 
	{
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<AchainActor*>(this)->SAV<int>::futures) { (void)(k + 1); this->~AchainActorState(); static_cast<AchainActor*>(this)->destroy(); return 0; }
															#line 5232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<AchainActor*>(this)->SAV< int >::value()) int(k + 1);
		this->~AchainActorState();
		static_cast<AchainActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(int && k,int loopDepth) 
	{
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<AchainActor*>(this)->SAV<int>::futures) { (void)(k + 1); this->~AchainActorState(); static_cast<AchainActor*>(this)->destroy(); return 0; }
															#line 5244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<AchainActor*>(this)->SAV< int >::value()) int(k + 1);
		this->~AchainActorState();
		static_cast<AchainActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int const& k,int loopDepth) 
	{
		loopDepth = a_body1cont1(k, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && k,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(k), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AchainActor*>(this)->actor_wait_state > 0) static_cast<AchainActor*>(this)->actor_wait_state = 0;
		static_cast<AchainActor*>(this)->ActorCallback< AchainActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< AchainActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("achain", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("achain", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AchainActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("achain", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("achain", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AchainActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("achain", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("achain", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> x;
															#line 5317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via achain()
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <int i>
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class AchainActor final : public Actor<int>, public ActorCallback< AchainActor<i>, 0, int >, public FastAllocated<AchainActor<i>>, public AchainActorState<i, AchainActor<i>> {
															#line 5324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<AchainActor<i>>::operator new;
	using FastAllocated<AchainActor<i>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AchainActor<i>, 0, int >;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	AchainActor(Future<int> const& x) 
															#line 5335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   AchainActorState<i, AchainActor<i>>(x)
	{
		fdb_probe_actor_enter("achain", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("achain");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("achain", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AchainActor<i>, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <int i>
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> achain( Future<int> const& x ) {
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new AchainActor<i>(x));
															#line 5365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

template <int i>
Future<int> chain(Future<int> const& x) {
	return achain<i - 1>(x);
}

template <>
Future<int> chain<0>(Future<int> const& x) {
	return x;
}

															#line 5380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
Future<int> chain2( Future<int> const& x, int const& i );

#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 5385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via chain2()
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class Chain2Actor>
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class Chain2ActorState {
															#line 5392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Chain2ActorState(Future<int> const& x,int const& i) 
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : x(x),
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   i(i)
															#line 5401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("chain2", reinterpret_cast<unsigned long>(this));

	}
	~Chain2ActorState() 
	{
		fdb_probe_actor_destroy("chain2", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (i > 1)
															#line 5416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			{
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				StrictFuture<int> __when_expr_0 = chain2(x, i - 1);
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (static_cast<Chain2Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Chain2Actor*>(this)->actor_wait_state = 1;
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Chain2Actor, 0, int >*>(static_cast<Chain2Actor*>(this)));
															#line 5427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				StrictFuture<int> __when_expr_1 = x;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (static_cast<Chain2Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<Chain2Actor*>(this)->actor_wait_state = 2;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Chain2Actor, 1, int >*>(static_cast<Chain2Actor*>(this)));
															#line 5441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~Chain2ActorState();
		static_cast<Chain2Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int const& k,int loopDepth) 
	{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<Chain2Actor*>(this)->SAV<int>::futures) { (void)(k + 1); this->~Chain2ActorState(); static_cast<Chain2Actor*>(this)->destroy(); return 0; }
															#line 5465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<Chain2Actor*>(this)->SAV< int >::value()) int(k + 1);
		this->~Chain2ActorState();
		static_cast<Chain2Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(int && k,int loopDepth) 
	{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<Chain2Actor*>(this)->SAV<int>::futures) { (void)(k + 1); this->~Chain2ActorState(); static_cast<Chain2Actor*>(this)->destroy(); return 0; }
															#line 5477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<Chain2Actor*>(this)->SAV< int >::value()) int(k + 1);
		this->~Chain2ActorState();
		static_cast<Chain2Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int const& k,int loopDepth) 
	{
		loopDepth = a_body1cont2(k, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && k,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(k), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Chain2Actor*>(this)->actor_wait_state > 0) static_cast<Chain2Actor*>(this)->actor_wait_state = 0;
		static_cast<Chain2Actor*>(this)->ActorCallback< Chain2Actor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< Chain2Actor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Chain2Actor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Chain2Actor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int const& k,int loopDepth) 
	{
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<Chain2Actor*>(this)->SAV<int>::futures) { (void)(k + i); this->~Chain2ActorState(); static_cast<Chain2Actor*>(this)->destroy(); return 0; }
															#line 5552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<Chain2Actor*>(this)->SAV< int >::value()) int(k + i);
		this->~Chain2ActorState();
		static_cast<Chain2Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(int && k,int loopDepth) 
	{
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<Chain2Actor*>(this)->SAV<int>::futures) { (void)(k + i); this->~Chain2ActorState(); static_cast<Chain2Actor*>(this)->destroy(); return 0; }
															#line 5564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<Chain2Actor*>(this)->SAV< int >::value()) int(k + i);
		this->~Chain2ActorState();
		static_cast<Chain2Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(int const& k,int loopDepth) 
	{
		loopDepth = a_body1cont4(k, loopDepth);

		return loopDepth;
	}
	int a_body1when2(int && k,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(k), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<Chain2Actor*>(this)->actor_wait_state > 0) static_cast<Chain2Actor*>(this)->actor_wait_state = 0;
		static_cast<Chain2Actor*>(this)->ActorCallback< Chain2Actor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< Chain2Actor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Chain2Actor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Chain2Actor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> x;
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int i;
															#line 5639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via chain2()
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class Chain2Actor final : public Actor<int>, public ActorCallback< Chain2Actor, 0, int >, public ActorCallback< Chain2Actor, 1, int >, public FastAllocated<Chain2Actor>, public Chain2ActorState<Chain2Actor> {
															#line 5644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<Chain2Actor>::operator new;
	using FastAllocated<Chain2Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Chain2Actor, 0, int >;
friend struct ActorCallback< Chain2Actor, 1, int >;
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Chain2Actor(Future<int> const& x,int const& i) 
															#line 5656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   Chain2ActorState<Chain2Actor>(x, i)
	{
		fdb_probe_actor_enter("chain2", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("chain2");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("chain2", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Chain2Actor, 0, int >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Chain2Actor, 1, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> chain2( Future<int> const& x, int const& i ) {
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new Chain2Actor(x, i));
															#line 5685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 5690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via cancellable2()
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class Cancellable2Actor>
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class Cancellable2ActorState {
															#line 5697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Cancellable2ActorState() 
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	{
															#line 5703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		fdb_probe_actor_create("cancellable2", reinterpret_cast<unsigned long>(this));

	}
	~Cancellable2ActorState() 
	{
		fdb_probe_actor_destroy("cancellable2", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				StrictFuture<Void> __when_expr_0 = Never();
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				if (static_cast<Cancellable2Actor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Cancellable2Actor*>(this)->actor_wait_state = 1;
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Cancellable2Actor, 0, Void >*>(static_cast<Cancellable2Actor*>(this)));
															#line 5725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~Cancellable2ActorState();
		static_cast<Cancellable2Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 5755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<Cancellable2Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Cancellable2ActorState(); static_cast<Cancellable2Actor*>(this)->destroy(); return 0; }
															#line 5769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<Cancellable2Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Cancellable2ActorState();
		static_cast<Cancellable2Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<Cancellable2Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Cancellable2ActorState(); static_cast<Cancellable2Actor*>(this)->destroy(); return 0; }
															#line 5781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<Cancellable2Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Cancellable2ActorState();
		static_cast<Cancellable2Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Cancellable2Actor*>(this)->actor_wait_state > 0) static_cast<Cancellable2Actor*>(this)->actor_wait_state = 0;
		static_cast<Cancellable2Actor*>(this)->ActorCallback< Cancellable2Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Cancellable2Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cancellable2", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancellable2", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Cancellable2Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cancellable2", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancellable2", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Cancellable2Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cancellable2", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancellable2", reinterpret_cast<unsigned long>(this), 0);

	}
};
// This generated class is to be used only via cancellable2()
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class Cancellable2Actor final : public Actor<Void>, public ActorCallback< Cancellable2Actor, 0, Void >, public FastAllocated<Cancellable2Actor>, public Cancellable2ActorState<Cancellable2Actor> {
															#line 5856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<Cancellable2Actor>::operator new;
	using FastAllocated<Cancellable2Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Cancellable2Actor, 0, Void >;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Cancellable2Actor() 
															#line 5867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   Cancellable2ActorState<Cancellable2Actor>()
	{
		fdb_probe_actor_enter("cancellable2", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cancellable2");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cancellable2", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Cancellable2Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> cancellable2(  ) {
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new Cancellable2Actor());
															#line 5895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 5900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via introLoadValueFromDisk()
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class IntroLoadValueFromDiskActor>
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroLoadValueFromDiskActorState {
															#line 5907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroLoadValueFromDiskActorState(Future<std::string> const& filename) 
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : filename(filename)
															#line 5914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this));

	}
	~IntroLoadValueFromDiskActorState() 
	{
		fdb_probe_actor_destroy("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<std::string> __when_expr_0 = filename;
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<IntroLoadValueFromDiskActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IntroLoadValueFromDiskActor*>(this)->actor_wait_state = 1;
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >*>(static_cast<IntroLoadValueFromDiskActor*>(this)));
															#line 5936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~IntroLoadValueFromDiskActorState();
		static_cast<IntroLoadValueFromDiskActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::string const& file,int loopDepth) 
	{
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (file == "/dev/threes")
															#line 5959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<IntroLoadValueFromDiskActor*>(this)->SAV<int>::futures) { (void)(3); this->~IntroLoadValueFromDiskActorState(); static_cast<IntroLoadValueFromDiskActor*>(this)->destroy(); return 0; }
															#line 5963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<IntroLoadValueFromDiskActor*>(this)->SAV< int >::value()) int(3);
			this->~IntroLoadValueFromDiskActorState();
			static_cast<IntroLoadValueFromDiskActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			ASSERT(false);
															#line 5973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		}
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroLoadValueFromDiskActor*>(this)->SAV<int>::futures) { (void)(0); this->~IntroLoadValueFromDiskActorState(); static_cast<IntroLoadValueFromDiskActor*>(this)->destroy(); return 0; }
															#line 5977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroLoadValueFromDiskActor*>(this)->SAV< int >::value()) int(0);
		this->~IntroLoadValueFromDiskActorState();
		static_cast<IntroLoadValueFromDiskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::string && file,int loopDepth) 
	{
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (file == "/dev/threes")
															#line 5989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (!static_cast<IntroLoadValueFromDiskActor*>(this)->SAV<int>::futures) { (void)(3); this->~IntroLoadValueFromDiskActorState(); static_cast<IntroLoadValueFromDiskActor*>(this)->destroy(); return 0; }
															#line 5993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			new (&static_cast<IntroLoadValueFromDiskActor*>(this)->SAV< int >::value()) int(3);
			this->~IntroLoadValueFromDiskActorState();
			static_cast<IntroLoadValueFromDiskActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			ASSERT(false);
															#line 6003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		}
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroLoadValueFromDiskActor*>(this)->SAV<int>::futures) { (void)(0); this->~IntroLoadValueFromDiskActorState(); static_cast<IntroLoadValueFromDiskActor*>(this)->destroy(); return 0; }
															#line 6007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroLoadValueFromDiskActor*>(this)->SAV< int >::value()) int(0);
		this->~IntroLoadValueFromDiskActorState();
		static_cast<IntroLoadValueFromDiskActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::string const& file,int loopDepth) 
	{
		loopDepth = a_body1cont1(file, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::string && file,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(file), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IntroLoadValueFromDiskActor*>(this)->actor_wait_state > 0) static_cast<IntroLoadValueFromDiskActor*>(this)->actor_wait_state = 0;
		static_cast<IntroLoadValueFromDiskActor*>(this)->ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >::remove();

	}
	void a_callback_fire(ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<std::string> filename;
															#line 6080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via introLoadValueFromDisk()
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroLoadValueFromDiskActor final : public Actor<int>, public ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >, public FastAllocated<IntroLoadValueFromDiskActor>, public IntroLoadValueFromDiskActorState<IntroLoadValueFromDiskActor> {
															#line 6085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<IntroLoadValueFromDiskActor>::operator new;
	using FastAllocated<IntroLoadValueFromDiskActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >;
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroLoadValueFromDiskActor(Future<std::string> const& filename) 
															#line 6096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   IntroLoadValueFromDiskActorState<IntroLoadValueFromDiskActor>(filename)
	{
		fdb_probe_actor_enter("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("introLoadValueFromDisk");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("introLoadValueFromDisk", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IntroLoadValueFromDiskActor, 0, std::string >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> introLoadValueFromDisk( Future<std::string> const& filename ) {
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new IntroLoadValueFromDiskActor(filename));
															#line 6124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 6129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via introAdd()
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class IntroAddActor>
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroAddActorState {
															#line 6136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroAddActorState(Future<int> const& a,Future<int> const& b) 
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : a(a),
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   b(b)
															#line 6145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("introAdd", reinterpret_cast<unsigned long>(this));

	}
	~IntroAddActorState() 
	{
		fdb_probe_actor_destroy("introAdd", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_0 = a;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<IntroAddActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IntroAddActor*>(this)->actor_wait_state = 1;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IntroAddActor, 0, int >*>(static_cast<IntroAddActor*>(this)));
															#line 6167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~IntroAddActorState();
		static_cast<IntroAddActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		StrictFuture<int> __when_expr_1 = b;
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<IntroAddActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<IntroAddActor*>(this)->actor_wait_state = 2;
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IntroAddActor, 1, int >*>(static_cast<IntroAddActor*>(this)));
															#line 6197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(int const& __x,int loopDepth) 
	{
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		x = __x;
															#line 6206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && __x,int loopDepth) 
	{
		x = std::move(__x);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IntroAddActor*>(this)->actor_wait_state > 0) static_cast<IntroAddActor*>(this)->actor_wait_state = 0;
		static_cast<IntroAddActor*>(this)->ActorCallback< IntroAddActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< IntroAddActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IntroAddActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IntroAddActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int const& y,int loopDepth) 
	{
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroAddActor*>(this)->SAV<int>::futures) { (void)(x + y); this->~IntroAddActorState(); static_cast<IntroAddActor*>(this)->destroy(); return 0; }
															#line 6273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroAddActor*>(this)->SAV< int >::value()) int(x + y);
		this->~IntroAddActorState();
		static_cast<IntroAddActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(int && y,int loopDepth) 
	{
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroAddActor*>(this)->SAV<int>::futures) { (void)(x + y); this->~IntroAddActorState(); static_cast<IntroAddActor*>(this)->destroy(); return 0; }
															#line 6285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroAddActor*>(this)->SAV< int >::value()) int(x + y);
		this->~IntroAddActorState();
		static_cast<IntroAddActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(int const& y,int loopDepth) 
	{
		loopDepth = a_body1cont2(y, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int && y,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(y), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<IntroAddActor*>(this)->actor_wait_state > 0) static_cast<IntroAddActor*>(this)->actor_wait_state = 0;
		static_cast<IntroAddActor*>(this)->ActorCallback< IntroAddActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< IntroAddActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IntroAddActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IntroAddActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> a;
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> b;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int x;
															#line 6362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via introAdd()
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroAddActor final : public Actor<int>, public ActorCallback< IntroAddActor, 0, int >, public ActorCallback< IntroAddActor, 1, int >, public FastAllocated<IntroAddActor>, public IntroAddActorState<IntroAddActor> {
															#line 6367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<IntroAddActor>::operator new;
	using FastAllocated<IntroAddActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IntroAddActor, 0, int >;
friend struct ActorCallback< IntroAddActor, 1, int >;
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroAddActor(Future<int> const& a,Future<int> const& b) 
															#line 6379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   IntroAddActorState<IntroAddActor>(a, b)
	{
		fdb_probe_actor_enter("introAdd", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("introAdd");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("introAdd", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IntroAddActor, 0, int >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IntroAddActor, 1, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> introAdd( Future<int> const& a, Future<int> const& b ) {
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new IntroAddActor(a, b));
															#line 6408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 6413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via introFirst()
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class IntroFirstActor>
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroFirstActorState {
															#line 6420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroFirstActorState(Future<int> const& a,Future<int> const& b) 
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : a(a),
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   b(b)
															#line 6429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("introFirst", reinterpret_cast<unsigned long>(this));

	}
	~IntroFirstActorState() 
	{
		fdb_probe_actor_destroy("introFirst", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_0 = a;
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			if (static_cast<IntroFirstActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			StrictFuture<int> __when_expr_1 = b;
															#line 6450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<IntroFirstActor*>(this)->actor_wait_state = 1;
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IntroFirstActor, 0, int >*>(static_cast<IntroFirstActor*>(this)));
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IntroFirstActor, 1, int >*>(static_cast<IntroFirstActor*>(this)));
															#line 6457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~IntroFirstActorState();
		static_cast<IntroFirstActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(int const& x,int loopDepth) 
	{
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroFirstActor*>(this)->SAV<int>::futures) { (void)(x); this->~IntroFirstActorState(); static_cast<IntroFirstActor*>(this)->destroy(); return 0; }
															#line 6480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroFirstActor*>(this)->SAV< int >::value()) int(x);
		this->~IntroFirstActorState();
		static_cast<IntroFirstActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(int && x,int loopDepth) 
	{
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroFirstActor*>(this)->SAV<int>::futures) { (void)(x); this->~IntroFirstActorState(); static_cast<IntroFirstActor*>(this)->destroy(); return 0; }
															#line 6492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroFirstActor*>(this)->SAV< int >::value()) int(x);
		this->~IntroFirstActorState();
		static_cast<IntroFirstActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(int const& x,int loopDepth) 
	{
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroFirstActor*>(this)->SAV<int>::futures) { (void)(x); this->~IntroFirstActorState(); static_cast<IntroFirstActor*>(this)->destroy(); return 0; }
															#line 6504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroFirstActor*>(this)->SAV< int >::value()) int(x);
		this->~IntroFirstActorState();
		static_cast<IntroFirstActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(int && x,int loopDepth) 
	{
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<IntroFirstActor*>(this)->SAV<int>::futures) { (void)(x); this->~IntroFirstActorState(); static_cast<IntroFirstActor*>(this)->destroy(); return 0; }
															#line 6516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<IntroFirstActor*>(this)->SAV< int >::value()) int(x);
		this->~IntroFirstActorState();
		static_cast<IntroFirstActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IntroFirstActor*>(this)->actor_wait_state > 0) static_cast<IntroFirstActor*>(this)->actor_wait_state = 0;
		static_cast<IntroFirstActor*>(this)->ActorCallback< IntroFirstActor, 0, int >::remove();
		static_cast<IntroFirstActor*>(this)->ActorCallback< IntroFirstActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< IntroFirstActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IntroFirstActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IntroFirstActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IntroFirstActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IntroFirstActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IntroFirstActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> a;
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	Future<int> b;
															#line 6625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via introFirst()
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroFirstActor final : public Actor<int>, public ActorCallback< IntroFirstActor, 0, int >, public ActorCallback< IntroFirstActor, 1, int >, public FastAllocated<IntroFirstActor>, public IntroFirstActorState<IntroFirstActor> {
															#line 6630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<IntroFirstActor>::operator new;
	using FastAllocated<IntroFirstActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IntroFirstActor, 0, int >;
friend struct ActorCallback< IntroFirstActor, 1, int >;
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroFirstActor(Future<int> const& a,Future<int> const& b) 
															#line 6642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<int>(),
		   IntroFirstActorState<IntroFirstActor>(a, b)
	{
		fdb_probe_actor_enter("introFirst", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("introFirst");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("introFirst", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IntroFirstActor, 0, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<int> introFirst( Future<int> const& a, Future<int> const& b ) {
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<int>(new IntroFirstActor(a, b));
															#line 6670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

struct AddReply {
	int sum;
	AddReply() {}
	AddReply(int x) : sum(x) {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, sum);
	}
};

struct AddRequest {
	int a, b;
	Promise<AddReply> reply; // Self-addressed envelope

	AddRequest() {}
	AddRequest(int a, int b) : a(a), b(b) {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, a, b, reply);
	}
};

															#line 6699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via introAddServer()
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class IntroAddServerActor>
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroAddServerActorState {
															#line 6706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroAddServerActorState(PromiseStream<AddRequest> const& add) 
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : add(add)
															#line 6713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("introAddServer", reinterpret_cast<unsigned long>(this));

	}
	~IntroAddServerActorState() 
	{
		fdb_probe_actor_destroy("introAddServer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 6728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		delete static_cast<IntroAddServerActor*>(this);
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
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<AddRequest> __when_expr_0 = add.getFuture();
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 6759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		static_cast<IntroAddServerActor*>(this)->actor_wait_state = 1;
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< IntroAddServerActor, 0, AddRequest >*>(static_cast<IntroAddServerActor*>(this)));
															#line 6763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(AddRequest const& req,int loopDepth) 
	{
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		printf("%d + %d = %d\n", req.a, req.b, req.a + req.b);
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		req.reply.send(req.a + req.b);
															#line 6780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(AddRequest && req,int loopDepth) 
	{
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		printf("%d + %d = %d\n", req.a, req.b, req.a + req.b);
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		req.reply.send(req.a + req.b);
															#line 6791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IntroAddServerActor*>(this)->actor_wait_state > 0) static_cast<IntroAddServerActor*>(this)->actor_wait_state = 0;
		static_cast<IntroAddServerActor*>(this)->ActorSingleCallback< IntroAddServerActor, 0, AddRequest >::remove();

	}
	void a_callback_fire(ActorSingleCallback< IntroAddServerActor, 0, AddRequest >*,AddRequest const& value) 
	{
		fdb_probe_actor_enter("introAddServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAddServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< IntroAddServerActor, 0, AddRequest >*,AddRequest && value) 
	{
		fdb_probe_actor_enter("introAddServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAddServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< IntroAddServerActor, 0, AddRequest >*,Error err) 
	{
		fdb_probe_actor_enter("introAddServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("introAddServer", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	PromiseStream<AddRequest> add;
															#line 6849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via introAddServer()
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class IntroAddServerActor final : public Actor<void>, public ActorSingleCallback< IntroAddServerActor, 0, AddRequest >, public FastAllocated<IntroAddServerActor>, public IntroAddServerActorState<IntroAddServerActor> {
															#line 6854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<IntroAddServerActor>::operator new;
	using FastAllocated<IntroAddServerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorSingleCallback< IntroAddServerActor, 0, AddRequest >;
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	IntroAddServerActor(PromiseStream<AddRequest> const& add) 
															#line 6865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<void>(),
		   IntroAddServerActorState<IntroAddServerActor>(add)
	{
		fdb_probe_actor_enter("introAddServer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("introAddServer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("introAddServer", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
void introAddServer( PromiseStream<AddRequest> const& add ) {
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	new IntroAddServerActor(add);
															#line 6884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

void introPromiseFuture() {
	Promise<int> myPromise;

	Future<int> myFuture = myPromise.getFuture();

	myPromise.send(12345);

	ASSERT(myFuture.isReady() && myFuture.get() == 12345);
}

void introActor() {
	Future<int> f = introLoadValueFromDisk(std::string("/dev/threes"));
	ASSERT(f.get() == 3);

	Promise<int> a, b;
	Future<int> sum = introAdd(a.getFuture(), b.getFuture());
	b.send(3);
	ASSERT(!sum.isReady());
	a.send(2);
	ASSERT(sum.get() == 5);

	Promise<int> c, d;
	Future<int> first = introFirst(c.getFuture(), d.getFuture());
	ASSERT(!first.isReady());
	// d.send(100);
	d.sendError(operation_failed());
	ASSERT(first.isError() && first.getError().code() == error_code_operation_failed);
	// ASSERT( first.getBlocking() == 100 );

	PromiseStream<AddRequest> addInterface;
	introAddServer(addInterface);

	Future<AddReply> reply = addInterface.getReply(AddRequest(5, 2));
	ASSERT(reply.get().sum == 7);

	printf("OK\n");
}

template <int N>
void chainTest() {
	auto startt = timer();
	for (int i = 0; i < 100000; i++) {
		Promise<int> p;
		Future<int> f = chain<N>(p.getFuture());
		p.send(i);
		ASSERT(f.get() == i + N);
	}
	auto endt = timer();
	printf("chain<%d>: %0.3f M/sec\n", N, 0.1 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 100000; i++) {
		Promise<int> p;
		Future<int> f = chain2(p.getFuture(), N);
		p.send(i);
		ASSERT(f.get() == i + N);
	}
	endt = timer();
	printf("chain2<%d>: %0.3f M/sec\n", N, 0.1 / (endt - startt));
}

															#line 6950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via cycle()
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class CycleActor>
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class CycleActorState {
															#line 6957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	CycleActorState(FutureStream<Void> const& in,PromiseStream<Void> const& out,int* const& ptotal) 
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : in(in),
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   out(out),
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   ptotal(ptotal)
															#line 6968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("cycle", reinterpret_cast<unsigned long>(this));

	}
	~CycleActorState() 
	{
		fdb_probe_actor_destroy("cycle", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 6983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		delete static_cast<CycleActor*>(this);
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
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<Void> __when_expr_0 = in;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
															#line 7014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		static_cast<CycleActor*>(this)->actor_wait_state = 1;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< CycleActor, 0, Void >*>(static_cast<CycleActor*>(this)));
															#line 7018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		(*ptotal)++;
															#line 1076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		out.send(Void());
															#line 7029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		(*ptotal)++;
															#line 1076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		out.send(Void());
															#line 7040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		if (static_cast<CycleActor*>(this)->actor_wait_state > 0) static_cast<CycleActor*>(this)->actor_wait_state = 0;
		static_cast<CycleActor*>(this)->ActorSingleCallback< CycleActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorSingleCallback< CycleActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cycle", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cycle", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< CycleActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cycle", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cycle", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< CycleActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cycle", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cycle", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	FutureStream<Void> in;
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	PromiseStream<Void> out;
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int* ptotal;
															#line 7114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via cycle()
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class CycleActor final : public Actor<void>, public ActorSingleCallback< CycleActor, 0, Void >, public FastAllocated<CycleActor>, public CycleActorState<CycleActor> {
															#line 7119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<CycleActor>::operator new;
	using FastAllocated<CycleActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorSingleCallback< CycleActor, 0, Void >;
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	CycleActor(FutureStream<Void> const& in,PromiseStream<Void> const& out,int* const& ptotal) 
															#line 7130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<void>(),
		   CycleActorState<CycleActor>(in, out, ptotal)
	{
		fdb_probe_actor_enter("cycle", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cycle");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cycle", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
void cycle( FutureStream<Void> const& in, PromiseStream<Void> const& out, int* const& ptotal ) {
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	new CycleActor(in, out, ptotal);
															#line 7149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

															#line 7154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
namespace {
// This generated class is to be used only via cycleTime()
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
template <class CycleTimeActor>
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class CycleTimeActorState {
															#line 7161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	CycleTimeActorState(int const& nodes,int const& times) 
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		 : nodes(nodes),
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   times(times),
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   n(nodes),
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		   total(0)
															#line 7174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
	{
		fdb_probe_actor_create("cycleTime", reinterpret_cast<unsigned long>(this));

	}
	~CycleTimeActorState() 
	{
		fdb_probe_actor_destroy("cycleTime", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			for(int i = 1;i < nodes;i++) {
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
				cycle(n[i].getFuture(), n[(i + 1) % nodes], &total);
															#line 7191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
			}
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			startT = timer();
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			n[1].send(Void());
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
			;
															#line 7199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		this->~CycleTimeActorState();
		static_cast<CycleTimeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		printf("Ring test: %d nodes, %d total ops, %.3f seconds\n", nodes, total, timer() - startT);
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!static_cast<CycleTimeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CycleTimeActorState(); static_cast<CycleTimeActor*>(this)->destroy(); return 0; }
															#line 7224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		new (&static_cast<CycleTimeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CycleTimeActorState();
		static_cast<CycleTimeActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		FutureStream<Void> __when_expr_0 = n[0].getFuture();
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (static_cast<CycleTimeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
		static_cast<CycleTimeActor*>(this)->actor_wait_state = 1;
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< CycleTimeActor, 0, Void >*>(static_cast<CycleTimeActor*>(this)));
															#line 7250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!--times)
															#line 7272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		n[1].send(Void());
															#line 7278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		if (!--times)
															#line 7287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
		n[1].send(Void());
															#line 7293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
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
		if (static_cast<CycleTimeActor*>(this)->actor_wait_state > 0) static_cast<CycleTimeActor*>(this)->actor_wait_state = 0;
		static_cast<CycleTimeActor*>(this)->ActorSingleCallback< CycleTimeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorSingleCallback< CycleTimeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cycleTime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cycleTime", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< CycleTimeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cycleTime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cycleTime", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< CycleTimeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cycleTime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cycleTime", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int nodes;
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int times;
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	std::vector<PromiseStream<Void>> n;
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	int total;
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	double startT;
															#line 7371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
};
// This generated class is to be used only via cycleTime()
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
class CycleTimeActor final : public Actor<Void>, public ActorSingleCallback< CycleTimeActor, 0, Void >, public FastAllocated<CycleTimeActor>, public CycleTimeActorState<CycleTimeActor> {
															#line 7376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
public:
	using FastAllocated<CycleTimeActor>::operator new;
	using FastAllocated<CycleTimeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< CycleTimeActor, 0, Void >;
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	CycleTimeActor(int const& nodes,int const& times) 
															#line 7387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
		 : Actor<Void>(),
		   CycleTimeActorState<CycleTimeActor>(nodes, times)
	{
		fdb_probe_actor_enter("cycleTime", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cycleTime");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cycleTime", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< CycleTimeActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
Future<Void> cycleTime( int const& nodes, int const& times ) {
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"
	return Future<Void>(new CycleTimeActor(nodes, times));
															#line 7415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.g.cpp"
}

#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/dsltest.actor.cpp"

void sleeptest() {
#ifdef __linux__
	int times[] = { 0, 100, 500, 1000, 5000, 100000, 500000, 1000000 };
	for (int j = 0; j < 8; j++) {
		double b = timer();
		int n = std::min(100, 4000000 / (1 + times[j]));
		for (int i = 0; i < n; i++) {
			timespec ts;
			ts.tv_sec = times[j] / 1000000;
			ts.tv_nsec = (times[j] % 1000000) * 1000;
			clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, nullptr);
			// nanosleep(&ts, nullptr);
		}
		double t = timer() - b;
		printf("Sleep test (%dus x %d): %0.1f\n", times[j], n, double(t) / n * 1e6);
	}
#endif
}

void asyncMapTest() {
	Future<Void> c;

	{
		AsyncMap<int, int> m1;
		m1.set(10, 1);
		ASSERT(m1.get(10) == 1);
		ASSERT(m1.get(20) == 0);
		Future<Void> a = m1.onChange(10);
		Future<Void> b = m1.onChange(20);
		c = m1.onChange(30);
		ASSERT(!a.isReady() && !b.isReady());
		m1.set(10, 0);
		ASSERT(a.isReady() && !a.isError() && !b.isReady() && m1.get(10) == 0);
		m1.set(20, 5);
		ASSERT(b.isReady() && !b.isError() && m1.get(20) == 5);

		a = m1.onChange(10);
		b = m1.onChange(20);
		m1.triggerRange(15, 25);
		ASSERT(!a.isReady() && b.isReady() && !b.isError() && m1.get(20) == 5);
	}
	ASSERT(c.isReady() && c.isError() && c.getError().code() == error_code_broken_promise);

	printf("AsyncMap: OK\n");

	double startt;
	AsyncMap<int, int> m2;
	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		m2.set(5, 0);
		m2.set(5, 1);
	}
	printf("  set(not present/present): %0.1fM/sec\n", 2.0 / (timer() - startt));
	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		m2.set(5, 1);
		m2.set(5, 2);
	}
	printf("  set(present/present): %0.1fM/sec\n", 2.0 / (timer() - startt));
	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		m2.set(5, 1);
	}
	printf("  set(no change): %0.1fM/sec\n", 1.0 / (timer() - startt));

	m2.set(5, 5);
	startt = timer();
	for (int i = 0; i < 1000000; i++)
		m2.onChange(5);
	printf("  onChange(present, cancelled): %0.1fM/sec\n", 1.0 / (timer() - startt));
	startt = timer();
	for (int i = 0; i < 1000000; i++)
		m2.onChange(10);
	printf("  onChange(not present, cancelled): %0.1fM/sec\n", 1.0 / (timer() - startt));
	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		auto f = m2.onChange(10);
		m2.set(10, 1);
		m2.set(10, 0);
	}
	printf("  onChange(not present, set): %0.1fM/sec\n", 1.0 / (timer() - startt));
	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		auto f = m2.onChange(5);
		m2.set(5, i + 1);
	}
	printf("  onChange(present, set): %0.1fM/sec\n", 1.0 / (timer() - startt));
}

extern void net2_test();

void dsltest() {
	double startt, endt;

	setThreadLocalDeterministicRandomSeed(40);

	asyncMapTest();

	net2_test();
	// sleeptest();

	Future<Void> ctf = cycleTime(1000, 1000);
	ctf.get();

	introPromiseFuture();
	introActor();
	// return;

	printf("Actor control flow tests: ");
	actorTest1(true);
	actorTest2(true);
	actorTest3(true);
	// if (g_network == g_simulator)
	// g_simulator->run( actorTest4(true) );
	actorTest5();
	actorTest6();
	actorTest7();
	actorTest8();
	actorTest9();
	actorTest10();

	printf("\n");

	printf("Running actor fuzz tests:\n");
	// Only include this test outside of Windows because of MSVC compiler bug
#ifndef WIN32
	auto afResults = actorFuzzTests();
#else
	std::pair<int, int> afResults(0, 0);
#endif
	printf("Actor fuzz tests: %d/%d passed\n", afResults.first, afResults.second);
	startt = timer();
	for (int i = 0; i < 1000000; i++)
		deterministicRandom()->random01();
	endt = timer();
	printf("Random01: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		Promise<Void>();
	endt = timer();
	printf("Promises: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		Promise<Void>().send(Void());
	endt = timer();
	printf("Promises (with send): %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		Promise<Void> p;
		Future<Void> f = p.getFuture();
		p.send(Void());
		f.get();
	}
	endt = timer();
	printf("Promise/Future/send roundtrip: %0.2f M/sec\n", 1.0 / (endt - startt));

	Promise<Void> p;

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		p.getFuture();
	endt = timer();
	printf("Futures: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		PromiseStream<Void>();
	endt = timer();
	printf("PromiseStreams: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		PromiseStream<Void>().send(Void());
	endt = timer();
	printf("PromiseStreams (with send): %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		PromiseStream<Void> p;
		FutureStream<Void> f = p.getFuture();
		p.send(Void());
		f.pop();
	}
	endt = timer();
	printf("PromiseStream/FutureStream/send/popBlocking roundtrip: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	{
		PromiseStream<int> ps;
		for (int i = 0; i < 1000000; i++) {
			ps.send(i);
		}
	}
	endt = timer();
	printf("PromiseStream queued send: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		cancellable();
	endt = timer();
	printf("Cancellations: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		cancellable2();
	endt = timer();
	printf("Cancellations with catch: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		simple();
	endt = timer();
	printf("Actor creation: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++)
		simpleWait();
	endt = timer();
	printf("With trivial wait: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		Promise<int> p;
		Future<int> f = simpleRet(p.getFuture());
		p.send(i);
		ASSERT(f.get() == i);
	}
	endt = timer();
	printf("Bounce int through actor: %0.2f M/sec\n", 1.0 / (endt - startt));

	startt = timer();
	for (int i = 0; i < 1000000; i++) {
		Promise<int> p;
		Future<int> f = simpleRet(p.getFuture());
		Future<int> g = simpleRet(p.getFuture());
		p.send(i);
		ASSERT(f.get() == i);
		ASSERT(g.get() == i);
	}
	endt = timer();
	printf("Bounce int through two actors in parallel: %0.2f M/sec\n", 1.0 / (endt - startt));

	/*chainTest<1>();
	chainTest<4>();
	chainTest<16>();
	chainTest<64>();

	startt = timer();
	for(int i=0; i<1000000; i++)
	    try {
	        throw success();
	    } catch (Error&) {
	    }
	endt = timer();
	printf("C++ exception: %0.2f M/sec\n", 1.0/(endt-startt));*/

	arenaTest();

	{
		Promise<int> a, b;
		Future<int> c = chooseTest(a.getFuture(), b.getFuture());
		a.send(1);
		b.send(2);
		std::cout << "c=" << c.get() << std::endl;
	}

	{
		Promise<double> i;
		Future<double> d = addN<20>(i.getFuture());
		i.send(1.1);
		std::cout << d.get() << std::endl;
	}

	{
		Promise<double> i;
		i.sendError(operation_failed());
		Future<double> d = addN<20>(i.getFuture());
		if (d.isError() && d.getError().code() == error_code_operation_failed)
			std::cout << "Error transmitted OK" << std::endl;
		else
			std::cout << "Error not transmitted!" << std::endl;
	}

	/*{
	    int na = Actor::allActors.size();
	    PromiseStream<int> t;
	    testStream(t.getFuture());
	    if (Actor::allActors.size() != na+1)
	        std::cout << "Actor not created!" << std::endl;
	    t = PromiseStream<int>();
	    if (Actor::allActors.size() != na)
	        std::cout << "Actor not cleaned up!" << std::endl;
	}*/

	PromiseStream<int> as;
	Promise<double> bs;
	as.send(4);
	Future<Void> sT = switchTest(as.getFuture(), bs.getFuture());
	as.send(5);
	// sT = move(Future<Void>());
	as.send(6);
	bs.send(10.1);
	as.send(7);

	fastAllocTest();

#if FLOW_THREAD_SAFE
	returnCancelRaceTest();
	threadSafetyTest();
	threadSafetyTest2();
#else
	printf("Thread safety disabled.\n");
#endif
}

/*ACTOR Future<Void> pingServer( FutureStream<Promise<bool>> requests, int rate ) {
    state int count = 0;
    loop {
        Promise<bool> req = waitNext( requests );
        req.send( (++count)%rate != 0 );
    }
}

ACTOR Future<int> ping( PromiseStream<Promise<bool>> server ) {
    state int count = 0;
    loop {
        bool result = wait( server.getReply<bool>() );

        count++;
        if (!result)
            break;
    }
    return count;
}

void pingtest() {
    double start = timer();
    PromiseStream<Promise<bool>> serverInterface;
    Future<Void> pS = pingServer( serverInterface.getFuture(), 5000000 );
    Future<int> count = ping( serverInterface );
    double end = timer();
    std::cout << count.get() << " pings completed in " << (end-start) << " sec" << std::endl;
}*/

void copyTest() {
	double start, elapsed;

	Arena arena;
	StringRef s(new (arena) uint8_t[10 << 20], 10 << 20);

	{
		start = timer();
		for (int i = 0; i < 100; i++) {
			StringRef k = s;
			(void)k;
		}
		elapsed = timer() - start;

		printf("StringRef->StringRef: %fs/GB\n", elapsed);
	}

	{
		start = timer();
		for (int i = 0; i < 100; i++)
			Standalone<StringRef> a = s;
		elapsed = timer() - start;

		printf("StringRef->Standalone: %fs/GB\n", elapsed);
	}

	{
		Standalone<StringRef> sa = s;
		start = timer();
		for (int i = 0; i < 100; i++)
			Standalone<StringRef> a = sa;
		elapsed = timer() - start;

		printf("Standalone->Standalone: %fs/GB\n", elapsed);
	}

	{
		Standalone<StringRef> sa = s, sb;
		start = timer();
		for (int i = 0; i < 50; i++) {
			sb = std::move(sa);
			sa = std::move(sb);
		}
		elapsed = timer() - start;
		printf("move(Standalone)->Standalone: %fs/GB\n", elapsed);
	}
}

/*ACTOR void badTest( FutureStream<int> is ) {
    state PromiseStream<int> js;

    loop choose {
        when( int j = waitNext( js.getFuture() ) ) {
            std::cout << "J" << j << std::endl;
        }
        when( int i = waitNext( is ) ) {
            std::cout << "I" << i << std::endl;
            js.send( i );
            std::cout << "-I" << i << std::endl;
        }
    }
}

void dsltest() {
    PromiseStream<int> is;
    badTest( is.getFuture() );
    is.send(1);
    is.send(2);
    is.send(3);
    throw not_implemented();
}
void pingtest() {}*/
