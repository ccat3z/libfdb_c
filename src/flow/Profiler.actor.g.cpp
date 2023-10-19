#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
/*
 * Profiler.actor.cpp
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
#include "flow/network.h"

#ifdef __linux__

#include <execinfo.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <link.h>

#include "flow/Platform.h"
#include "flow/actorcompiler.h" // This must be the last include.

extern volatile thread_local int profilingEnabled;

static uint64_t sys_gettid() {
	return syscall(__NR_gettid);
}

struct SignalClosure {
	void (*func)(int, siginfo_t*, void*, void*);
	void* userdata;

	SignalClosure(void (*func)(int, siginfo_t*, void*, void*), void* userdata) : func(func), userdata(userdata) {}

	static void signal_handler(int s, siginfo_t* si, void* ucontext) {
		// async signal safe!
		// This is intended to work as a SIGPROF handler for past and future versions of the flow profiler (when
		// multiple are running in a process!) So don't change what it does without really good reason
		SignalClosure* closure = (SignalClosure*)(si->si_value.sival_ptr);
		closure->func(s, si, ucontext, closure->userdata);
	}
};

struct SyncFileForSim : ReferenceCounted<SyncFileForSim> {
	FILE* f;
	SyncFileForSim(std::string const& filename) { f = fopen(filename.c_str(), "wb"); }

	bool isOpen() const { return f != nullptr; }

	int64_t debugFD() const { return (int64_t)f; }

	Future<int> read(void* data, int length, int64_t offset) {
		ASSERT(false);
		throw internal_error();
	}

	Future<Void> write(void const* data, int length, int64_t offset) {
		ASSERT(isOpen());
		fseek(f, offset, SEEK_SET);
		if (fwrite(data, 1, length, f) != length)
			throw io_error();
		return Void();
	}

	Future<Void> truncate(int64_t size) {
		ASSERT(size == 0);
		return Void();
	}

	Future<Void> flush() {
		ASSERT(isOpen());
		fflush(f);
		return Void();
	}

	Future<Void> sync() {
		ASSERT(false);
		throw internal_error();
	}

	Future<int64_t> size() const {
		ASSERT(false);
		throw internal_error();
	}
};

struct Profiler {
	struct OutputBuffer {
		std::vector<void*> output;

		OutputBuffer() { output.reserve(100000); }
		void clear() { output.clear(); }
		void push(void* ptr) { // async signal safe!
			if (output.size() < output.capacity())
				output.push_back(ptr);
		}
		Future<Void> writeTo(Reference<SyncFileForSim> file, int64_t& offset) {
			int64_t offs = offset;
			offset += sizeof(void*) * output.size();
			return file->write(&output[0], sizeof(void*) * output.size(), offs);
		}
	};

	enum { MAX_STACK_DEPTH = 256 };

	void* addresses[MAX_STACK_DEPTH];
	SignalClosure signalClosure;
	OutputBuffer* output_buffer;
	Future<Void> actor;
	sigset_t profilingSignals;
	static Profiler* active_profiler;
	BinaryWriter environmentInfoWriter;
	INetwork* network;
	timer_t periodicTimer;
	bool timerInitialized;

	Profiler(int period, std::string const& outfn, INetwork* network)
	  : signalClosure(signal_handler_for_closure, this), environmentInfoWriter(Unversioned()), network(network),
	    timerInitialized(false) {
		actor = profile(this, period, outfn);
	}

	~Profiler() {
		enableSignal(false);

		if (timerInitialized) {
			timer_delete(periodicTimer);
		}
	}

	void signal_handler() { // async signal safe!
		static std::atomic<bool> inSigHandler = false;
		if (inSigHandler.exchange(true)) {
			return;
		}
		if (profilingEnabled) {
			double t = timer();
			output_buffer->push(*(void**)&t);
			size_t n = platform::raw_backtrace(addresses, 256);
			for (int i = 0; i < n; i++)
				output_buffer->push(addresses[i]);
			output_buffer->push((void*)-1LL);
		}
		inSigHandler.store(false);
	}

	static void signal_handler_for_closure(int, siginfo_t* si, void*, void* self) { // async signal safe!
		((Profiler*)self)->signal_handler();
	}

	void enableSignal(bool enabled) { sigprocmask(enabled ? SIG_UNBLOCK : SIG_BLOCK, &profilingSignals, nullptr); }

	void phdr(struct dl_phdr_info* info) {
		environmentInfoWriter << int64_t(1) << info->dlpi_addr
		                      << StringRef((const uint8_t*)info->dlpi_name, strlen(info->dlpi_name));
		for (int s = 0; s < info->dlpi_phnum; s++) {
			auto const& h = info->dlpi_phdr[s];
			environmentInfoWriter << int64_t(2) << h.p_type << h.p_flags // Word (uint32_t)
			                      << h.p_offset // Off (uint64_t)
			                      << h.p_vaddr << h.p_paddr // Addr (uint64_t)
			                      << h.p_filesz << h.p_memsz << h.p_align; // XWord (uint64_t)
		}
	}

	static int phdr_callback(struct dl_phdr_info* info, size_t size, void* data) {
		((Profiler*)data)->phdr(info);
		return 0;
	}

																#line 185 "/src/flow/Profiler.actor.g.cpp"
// This generated class is to be used only via profile()
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
template <class ProfileActor>
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
class ProfileActorState {
															#line 191 "/src/flow/Profiler.actor.g.cpp"
public:
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	ProfileActorState(Profiler* const& self,int const& period,std::string const& outfn) 
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		 : self(self),
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		   period(period),
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		   outfn(outfn),
															#line 185 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		   outFile(makeReference<SyncFileForSim>(outfn))
															#line 204 "/src/flow/Profiler.actor.g.cpp"
	{

	}
	~ProfileActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 186 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			if (!outFile->isOpen())
															#line 217 "/src/flow/Profiler.actor.g.cpp"
			{
															#line 187 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				TraceEvent(SevWarn, "FailedToOpenProfilingOutputFile").detail("Filename", outfn).GetLastError();
															#line 188 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				if (!static_cast<ProfileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ProfileActorState(); static_cast<ProfileActor*>(this)->destroy(); return 0; }
															#line 223 "/src/flow/Profiler.actor.g.cpp"
				new (&static_cast<ProfileActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ProfileActorState();
				static_cast<ProfileActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 193 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			platform::raw_backtrace(self->addresses, MAX_STACK_DEPTH);
															#line 198 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			self->environmentInfoWriter << int64_t(0x101) << int64_t(period * 1000);
															#line 199 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			dl_iterate_phdr(phdr_callback, self);
															#line 200 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			self->environmentInfoWriter << int64_t(0);
															#line 201 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			for(;self->environmentInfoWriter.getLength() % sizeof(void*);) {
															#line 202 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				self->environmentInfoWriter << uint8_t(0);
															#line 241 "/src/flow/Profiler.actor.g.cpp"
			}
															#line 204 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			self->output_buffer = new OutputBuffer;
															#line 205 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			otherBuffer = new OutputBuffer;
															#line 208 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sigemptyset(&self->profilingSignals);
															#line 209 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sigaddset(&self->profilingSignals, SIGPROF);
															#line 212 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			struct sigaction act;
															#line 213 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			act.sa_sigaction = SignalClosure::signal_handler;
															#line 214 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sigemptyset(&act.sa_mask);
															#line 215 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			act.sa_flags = SA_SIGINFO;
															#line 216 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sigaction(SIGPROF, &act, nullptr);
															#line 219 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			int period_ns = period * 1000;
															#line 220 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			itimerspec tv;
															#line 221 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			tv.it_interval.tv_sec = 0;
															#line 222 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			tv.it_interval.tv_nsec = period_ns;
															#line 223 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			tv.it_value.tv_sec = 0;
															#line 224 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			tv.it_value.tv_nsec = nondeterministicRandom()->randomInt(period_ns / 2, period_ns + 1);
															#line 226 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sigevent sev;
															#line 227 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sev.sigev_notify = SIGEV_THREAD_ID;
															#line 228 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sev.sigev_signo = SIGPROF;
															#line 229 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sev.sigev_value.sival_ptr = &(self->signalClosure);
															#line 230 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			sev._sigev_un._tid = sys_gettid();
															#line 231 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			if (timer_create(CLOCK_THREAD_CPUTIME_ID, &sev, &self->periodicTimer) != 0)
															#line 285 "/src/flow/Profiler.actor.g.cpp"
			{
															#line 232 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				TraceEvent(SevWarn, "FailedToCreateProfilingTimer").GetLastError();
															#line 233 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				if (!static_cast<ProfileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ProfileActorState(); static_cast<ProfileActor*>(this)->destroy(); return 0; }
															#line 291 "/src/flow/Profiler.actor.g.cpp"
				new (&static_cast<ProfileActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ProfileActorState();
				static_cast<ProfileActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 235 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			self->timerInitialized = true;
															#line 236 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			if (timer_settime(self->periodicTimer, 0, &tv, nullptr) != 0)
															#line 301 "/src/flow/Profiler.actor.g.cpp"
			{
															#line 237 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				TraceEvent(SevWarn, "FailedToSetProfilingTimer").GetLastError();
															#line 238 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
				if (!static_cast<ProfileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ProfileActorState(); static_cast<ProfileActor*>(this)->destroy(); return 0; }
															#line 307 "/src/flow/Profiler.actor.g.cpp"
				new (&static_cast<ProfileActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ProfileActorState();
				static_cast<ProfileActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 241 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			outOffset = 0;
															#line 242 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			StrictFuture<Void> __when_expr_0 = outFile->truncate(outOffset);
															#line 242 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 319 "/src/flow/Profiler.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProfileActor*>(this)->actor_wait_state = 1;
															#line 242 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 0, Void >*>(static_cast<ProfileActor*>(this)));
															#line 324 "/src/flow/Profiler.actor.g.cpp"
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
		this->~ProfileActorState();
		static_cast<ProfileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 244 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_1 = outFile->write(self->environmentInfoWriter.getData(), self->environmentInfoWriter.getLength(), outOffset);
															#line 244 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 349 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 2;
															#line 244 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 1, Void >*>(static_cast<ProfileActor*>(this)));
															#line 354 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 244 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_1 = outFile->write(self->environmentInfoWriter.getData(), self->environmentInfoWriter.getLength(), outOffset);
															#line 244 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 365 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 2;
															#line 244 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 1, Void >*>(static_cast<ProfileActor*>(this)));
															#line 370 "/src/flow/Profiler.actor.g.cpp"
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
		if (static_cast<ProfileActor*>(this)->actor_wait_state > 0) static_cast<ProfileActor*>(this)->actor_wait_state = 0;
		static_cast<ProfileActor*>(this)->ActorCallback< ProfileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProfileActor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< ProfileActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< ProfileActor, 0, Void >*,Error err) 
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
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 245 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		outOffset += self->environmentInfoWriter.getLength();
															#line 247 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		;
															#line 438 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 245 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		outOffset += self->environmentInfoWriter.getLength();
															#line 247 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		;
															#line 449 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ProfileActor*>(this)->actor_wait_state > 0) static_cast<ProfileActor*>(this)->actor_wait_state = 0;
		static_cast<ProfileActor*>(this)->ActorCallback< ProfileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProfileActor, 1, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< ProfileActor, 1, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< ProfileActor, 1, Void >*,Error err) 
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
	int a_body1cont6loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont6loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1(int loopDepth) 
	{
															#line 248 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_2 = self->network->delay(1.0, TaskPriority::Min) || self->network->delay(2.0, TaskPriority::Max);
															#line 248 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 524 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 3;
															#line 248 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 2, Void >*>(static_cast<ProfileActor*>(this)));
															#line 529 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 250 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		self->enableSignal(false);
															#line 251 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		std::swap(self->output_buffer, otherBuffer);
															#line 252 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		self->enableSignal(true);
															#line 254 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_3 = otherBuffer->writeTo(outFile, outOffset);
															#line 254 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 546 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 4;
															#line 254 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 3, Void >*>(static_cast<ProfileActor*>(this)));
															#line 551 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 250 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		self->enableSignal(false);
															#line 251 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		std::swap(self->output_buffer, otherBuffer);
															#line 252 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		self->enableSignal(true);
															#line 254 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_3 = otherBuffer->writeTo(outFile, outOffset);
															#line 254 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 568 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 4;
															#line 254 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 3, Void >*>(static_cast<ProfileActor*>(this)));
															#line 573 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ProfileActor*>(this)->actor_wait_state > 0) static_cast<ProfileActor*>(this)->actor_wait_state = 0;
		static_cast<ProfileActor*>(this)->ActorCallback< ProfileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProfileActor, 2, Void >*,Void const& value) 
	{
		a_exitChoose3();
		try {
			a_body1cont6loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ProfileActor, 2, Void >*,Void && value) 
	{
		a_exitChoose3();
		try {
			a_body1cont6loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ProfileActor, 2, Void >*,Error err) 
	{
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	int a_body1cont6loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 255 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_4 = outFile->flush();
															#line 255 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 641 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 5;
															#line 255 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 4, Void >*>(static_cast<ProfileActor*>(this)));
															#line 646 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 255 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		StrictFuture<Void> __when_expr_4 = outFile->flush();
															#line 255 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		if (static_cast<ProfileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 657 "/src/flow/Profiler.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ProfileActor*>(this)->actor_wait_state = 5;
															#line 255 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ProfileActor, 4, Void >*>(static_cast<ProfileActor*>(this)));
															#line 662 "/src/flow/Profiler.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ProfileActor*>(this)->actor_wait_state > 0) static_cast<ProfileActor*>(this)->actor_wait_state = 0;
		static_cast<ProfileActor*>(this)->ActorCallback< ProfileActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProfileActor, 3, Void >*,Void const& value) 
	{
		a_exitChoose4();
		try {
			a_body1cont6loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ProfileActor, 3, Void >*,Void && value) 
	{
		a_exitChoose4();
		try {
			a_body1cont6loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ProfileActor, 3, Void >*,Error err) 
	{
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	int a_body1cont6loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 256 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		otherBuffer->clear();
															#line 728 "/src/flow/Profiler.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 256 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
		otherBuffer->clear();
															#line 737 "/src/flow/Profiler.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ProfileActor*>(this)->actor_wait_state > 0) static_cast<ProfileActor*>(this)->actor_wait_state = 0;
		static_cast<ProfileActor*>(this)->ActorCallback< ProfileActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProfileActor, 4, Void >*,Void const& value) 
	{
		a_exitChoose5();
		try {
			a_body1cont6loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ProfileActor, 4, Void >*,Void && value) 
	{
		a_exitChoose5();
		try {
			a_body1cont6loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ProfileActor, 4, Void >*,Error err) 
	{
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	Profiler* self;
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	int period;
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	std::string outfn;
															#line 185 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	Reference<SyncFileForSim> outFile;
															#line 205 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	OutputBuffer* otherBuffer;
															#line 241 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	int64_t outOffset;
															#line 811 "/src/flow/Profiler.actor.g.cpp"
};
// This generated class is to be used only via profile()
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
class ProfileActor final : public Actor<Void>, public ActorCallback< ProfileActor, 0, Void >, public ActorCallback< ProfileActor, 1, Void >, public ActorCallback< ProfileActor, 2, Void >, public ActorCallback< ProfileActor, 3, Void >, public ActorCallback< ProfileActor, 4, Void >, public FastAllocated<ProfileActor>, public ProfileActorState<ProfileActor> {
															#line 816 "/src/flow/Profiler.actor.g.cpp"
public:
	using FastAllocated<ProfileActor>::operator new;
	using FastAllocated<ProfileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ProfileActor, 0, Void >;
friend struct ActorCallback< ProfileActor, 1, Void >;
friend struct ActorCallback< ProfileActor, 2, Void >;
friend struct ActorCallback< ProfileActor, 3, Void >;
friend struct ActorCallback< ProfileActor, 4, Void >;
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	ProfileActor(Profiler* const& self,int const& period,std::string const& outfn) 
															#line 831 "/src/flow/Profiler.actor.g.cpp"
		 : Actor<Void>(),
		   ProfileActorState<ProfileActor>(self, period, outfn)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("profile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ProfileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ProfileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ProfileActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ProfileActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ProfileActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
[[nodiscard]] static Future<Void> profile( Profiler* const& self, int const& period, std::string const& outfn ) {
															#line 183 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
	return Future<Void>(new ProfileActor(self, period, outfn));
															#line 860 "/src/flow/Profiler.actor.g.cpp"
}

#line 259 "/tmp/fdb_c/foundationdb_ep/flow/Profiler.actor.cpp"
};

// Outlives main
Profiler* Profiler::active_profiler = nullptr;

std::string findAndReplace(std::string const& fn, std::string const& symbol, std::string const& value) {
	auto i = fn.find(symbol);
	if (i == std::string::npos)
		return fn;
	return fn.substr(0, i) + value + fn.substr(i + symbol.size());
}

void startProfiling(INetwork* network,
                    Optional<int> maybePeriod /*= {}*/,
                    Optional<StringRef> maybeOutputFile /*= {}*/) {
	int period;
	if (maybePeriod.present()) {
		period = maybePeriod.get();
	} else {
		const char* periodEnv = getenv("FLOW_PROFILER_PERIOD");
		period = (periodEnv ? atoi(periodEnv) : 2000);
	}
	std::string outputFile;
	if (maybeOutputFile.present()) {
		outputFile = std::string((const char*)maybeOutputFile.get().begin(), maybeOutputFile.get().size());
	} else {
		const char* outfn = getenv("FLOW_PROFILER_OUTPUT");
		outputFile = (outfn ? outfn : "profile.bin");
	}
	outputFile = findAndReplace(
	    findAndReplace(
	        findAndReplace(outputFile, "%ADDRESS%", findAndReplace(network->getLocalAddress().toString(), ":", ".")),
	        "%PID%",
	        format("%d", getpid())),
	    "%TID%",
	    format("%llx", (long long)sys_gettid()));

	if (!Profiler::active_profiler)
		Profiler::active_profiler = new Profiler(period, outputFile, network);
}

void stopProfiling() {
	if (Profiler::active_profiler) {
		Profiler* p = Profiler::active_profiler;
		Profiler::active_profiler = nullptr;
		delete p;
	}
}

#else

void startProfiling(INetwork* network, Optional<int> period, Optional<StringRef> outputFile) {}
void stopProfiling() {}

#endif
