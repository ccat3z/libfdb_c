#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
/*
 * AsyncFileNonDurable.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FLOW_ASYNCFILENONDURABLE_ACTOR_G_H)
#define FLOW_ASYNCFILENONDURABLE_ACTOR_G_H
#include "fdbrpc/AsyncFileNonDurable.actor.g.h"
#elif !defined(FLOW_ASYNCFILENONDURABLE_ACTOR_H)
#define FLOW_ASYNCFILENONDURABLE_ACTOR_H

#include "flow/flow.h"
#include "fdbrpc/IAsyncFile.h"
#include "flow/ActorCollection.h"
#include "fdbrpc/simulator.h"
#include "fdbrpc/TraceFileIO.h"
#include "fdbrpc/RangeMap.h"
#include "flow/actorcompiler.h" // This must be the last #include.

#undef max
#undef min

															#line 44 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<Void> sendOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, TaskPriority const& taskID );

#line 43 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<Void> sendErrorOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, Error const& e, TaskPriority const& taskID );

#line 47 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

															#line 53 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
namespace {
// This generated class is to be used only via sendErrorOnShutdown()
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class T, class SendErrorOnShutdownActor>
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class SendErrorOnShutdownActorState {
															#line 60 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	SendErrorOnShutdownActorState(Future<T> const& in,bool const& assertOnCancel = false) 
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : in(in),
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   assertOnCancel(assertOnCancel)
															#line 69 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this));

	}
	~SendErrorOnShutdownActorState() 
	{
		fdb_probe_actor_destroy("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 52 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_0 = success(g_simulator.getCurrentProcess()->shutdownSignal.getFuture());
															#line 51 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 87 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 55 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<T> __when_expr_1 = in;
															#line 91 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state = 1;
															#line 52 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendErrorOnShutdownActor, 0, Void >*>(static_cast<SendErrorOnShutdownActor*>(this)));
															#line 55 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SendErrorOnShutdownActor, 1, T >*>(static_cast<SendErrorOnShutdownActor*>(this)));
															#line 98 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~SendErrorOnShutdownActorState();
		static_cast<SendErrorOnShutdownActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 60 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			ASSERT(e.code() != error_code_actor_cancelled || !assertOnCancel);
															#line 61 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 130 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 53 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch2(io_error().asInjectedFault(), loopDepth);
															#line 144 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 53 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch2(io_error().asInjectedFault(), loopDepth);
															#line 152 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1when2(T const& rep,int loopDepth) 
	{
															#line 56 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SendErrorOnShutdownActor*>(this)->SAV<T>::futures) { (void)(rep); this->~SendErrorOnShutdownActorState(); static_cast<SendErrorOnShutdownActor*>(this)->destroy(); return 0; }
															#line 160 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SendErrorOnShutdownActor*>(this)->SAV< T >::value()) T(rep);
		this->~SendErrorOnShutdownActorState();
		static_cast<SendErrorOnShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(T && rep,int loopDepth) 
	{
															#line 56 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SendErrorOnShutdownActor*>(this)->SAV<T>::futures) { (void)(rep); this->~SendErrorOnShutdownActorState(); static_cast<SendErrorOnShutdownActor*>(this)->destroy(); return 0; }
															#line 172 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SendErrorOnShutdownActor*>(this)->SAV< T >::value()) T(rep);
		this->~SendErrorOnShutdownActorState();
		static_cast<SendErrorOnShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state > 0) static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state = 0;
		static_cast<SendErrorOnShutdownActor*>(this)->ActorCallback< SendErrorOnShutdownActor, 0, Void >::remove();
		static_cast<SendErrorOnShutdownActor*>(this)->ActorCallback< SendErrorOnShutdownActor, 1, T >::remove();

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendErrorOnShutdownActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SendErrorOnShutdownActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<T> in;
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool assertOnCancel;
															#line 281 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via sendErrorOnShutdown()
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class T>
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class SendErrorOnShutdownActor final : public Actor<T>, public ActorCallback< SendErrorOnShutdownActor<T>, 0, Void >, public ActorCallback< SendErrorOnShutdownActor<T>, 1, T >, public FastAllocated<SendErrorOnShutdownActor<T>>, public SendErrorOnShutdownActorState<T, SendErrorOnShutdownActor<T>> {
															#line 288 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<SendErrorOnShutdownActor<T>>::operator new;
	using FastAllocated<SendErrorOnShutdownActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendErrorOnShutdownActor<T>, 0, Void >;
friend struct ActorCallback< SendErrorOnShutdownActor<T>, 1, T >;
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	SendErrorOnShutdownActor(Future<T> const& in,bool const& assertOnCancel = false) 
															#line 300 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<T>(),
		   SendErrorOnShutdownActorState<T, SendErrorOnShutdownActor<T>>(in, assertOnCancel)
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendErrorOnShutdown");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendErrorOnShutdownActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class T>
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<T> sendErrorOnShutdown( Future<T> const& in, bool const& assertOnCancel = false ) {
															#line 48 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<T>(new SendErrorOnShutdownActor<T>(in, assertOnCancel));
															#line 330 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 64 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

class AsyncFileDetachable final : public IAsyncFile, public ReferenceCounted<AsyncFileDetachable> {
private:
	Reference<IAsyncFile> file;
	Future<Void> shutdown;
	bool assertOnReadWriteCancel;

public:
	explicit AsyncFileDetachable(Reference<IAsyncFile> file) : file(file), assertOnReadWriteCancel(true) {
		shutdown = doShutdown(this);
	}

																#line 346 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via doShutdown()
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class DoShutdownActor>
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class DoShutdownActorState {
															#line 352 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	DoShutdownActorState(AsyncFileDetachable* const& self) 
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self)
															#line 359 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("doShutdown", reinterpret_cast<unsigned long>(this));

	}
	~DoShutdownActorState() 
	{
		fdb_probe_actor_destroy("doShutdown", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 77 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = success(g_simulator.getCurrentProcess()->shutdownSignal.getFuture());
															#line 77 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<DoShutdownActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 376 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoShutdownActor*>(this)->actor_wait_state = 1;
															#line 77 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoShutdownActor, 0, Void >*>(static_cast<DoShutdownActor*>(this)));
															#line 381 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~DoShutdownActorState();
		static_cast<DoShutdownActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->file = Reference<IAsyncFile>();
															#line 79 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<DoShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoShutdownActorState(); static_cast<DoShutdownActor*>(this)->destroy(); return 0; }
															#line 406 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<DoShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoShutdownActorState();
		static_cast<DoShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 78 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->file = Reference<IAsyncFile>();
															#line 79 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<DoShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoShutdownActorState(); static_cast<DoShutdownActor*>(this)->destroy(); return 0; }
															#line 420 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<DoShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoShutdownActorState();
		static_cast<DoShutdownActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<DoShutdownActor*>(this)->actor_wait_state > 0) static_cast<DoShutdownActor*>(this)->actor_wait_state = 0;
		static_cast<DoShutdownActor*>(this)->ActorCallback< DoShutdownActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoShutdownActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoShutdownActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoShutdownActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileDetachable* self;
															#line 493 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via doShutdown()
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class DoShutdownActor final : public Actor<Void>, public ActorCallback< DoShutdownActor, 0, Void >, public FastAllocated<DoShutdownActor>, public DoShutdownActorState<DoShutdownActor> {
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<DoShutdownActor>::operator new;
	using FastAllocated<DoShutdownActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoShutdownActor, 0, Void >;
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	DoShutdownActor(AsyncFileDetachable* const& self) 
															#line 509 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   DoShutdownActorState<DoShutdownActor>(self)
	{
		fdb_probe_actor_enter("doShutdown", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doShutdown");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doShutdown", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoShutdownActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> doShutdown( AsyncFileDetachable* const& self ) {
															#line 76 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new DoShutdownActor(self));
															#line 536 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 81 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 541 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via open()
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OpenActor>
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OpenActorState {
															#line 547 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OpenActorState(Future<Reference<IAsyncFile>> const& wrappedFile) 
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : wrappedFile(wrappedFile)
															#line 554 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("open", reinterpret_cast<unsigned long>(this));

	}
	~OpenActorState() 
	{
		fdb_probe_actor_destroy("open", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 84 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = success(g_simulator.getCurrentProcess()->shutdownSignal.getFuture());
															#line 83 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 571 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 87 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Reference<IAsyncFile>> __when_expr_1 = wrappedFile;
															#line 575 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor*>(this)->actor_wait_state = 1;
															#line 84 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 0, Void >*>(static_cast<OpenActor*>(this)));
															#line 87 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 1, Reference<IAsyncFile> >*>(static_cast<OpenActor*>(this)));
															#line 582 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 85 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 605 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 85 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 613 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1when2(Reference<IAsyncFile> const& f,int loopDepth) 
	{
															#line 88 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(makeReference<AsyncFileDetachable>(f)); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 621 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(makeReference<AsyncFileDetachable>(f));
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Reference<IAsyncFile> && f,int loopDepth) 
	{
															#line 88 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(makeReference<AsyncFileDetachable>(f)); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 633 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(makeReference<AsyncFileDetachable>(f));
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OpenActor*>(this)->actor_wait_state > 0) static_cast<OpenActor*>(this)->actor_wait_state = 0;
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 0, Void >::remove();
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 1, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OpenActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OpenActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OpenActor, 1, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OpenActor, 1, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OpenActor, 1, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Reference<IAsyncFile>> wrappedFile;
															#line 740 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via open()
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OpenActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< OpenActor, 0, Void >, public ActorCallback< OpenActor, 1, Reference<IAsyncFile> >, public FastAllocated<OpenActor>, public OpenActorState<OpenActor> {
															#line 745 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OpenActor>::operator new;
	using FastAllocated<OpenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OpenActor, 0, Void >;
friend struct ActorCallback< OpenActor, 1, Reference<IAsyncFile> >;
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OpenActor(Future<Reference<IAsyncFile>> const& wrappedFile) 
															#line 757 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Reference<IAsyncFile>>(),
		   OpenActorState<OpenActor>(wrappedFile)
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("open");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OpenActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( Future<Reference<IAsyncFile>> const& wrappedFile ) {
															#line 82 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Reference<IAsyncFile>>(new OpenActor(wrappedFile));
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 92 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	void addref() override { ReferenceCounted<AsyncFileDetachable>::addref(); }
	void delref() override { ReferenceCounted<AsyncFileDetachable>::delref(); }

	Future<int> read(void* data, int length, int64_t offset) override {
		if (!file.getPtr() || g_simulator.getCurrentProcess()->shutdownSignal.getFuture().isReady())
			return io_error().asInjectedFault();
		return sendErrorOnShutdown(file->read(data, length, offset), assertOnReadWriteCancel);
	}

	Future<Void> write(void const* data, int length, int64_t offset) override {
		if (!file.getPtr() || g_simulator.getCurrentProcess()->shutdownSignal.getFuture().isReady())
			return io_error().asInjectedFault();
		return sendErrorOnShutdown(file->write(data, length, offset), assertOnReadWriteCancel);
	}

	Future<Void> truncate(int64_t size) override {
		if (!file.getPtr() || g_simulator.getCurrentProcess()->shutdownSignal.getFuture().isReady())
			return io_error().asInjectedFault();
		return sendErrorOnShutdown(file->truncate(size));
	}

	Future<Void> sync() override {
		if (!file.getPtr() || g_simulator.getCurrentProcess()->shutdownSignal.getFuture().isReady())
			return io_error().asInjectedFault();
		return sendErrorOnShutdown(file->sync());
	}

	Future<int64_t> size() const override {
		if (!file.getPtr() || g_simulator.getCurrentProcess()->shutdownSignal.getFuture().isReady())
			return io_error().asInjectedFault();
		return sendErrorOnShutdown(file->size());
	}

	int64_t debugFD() const override {
		if (!file.getPtr())
			throw io_error().asInjectedFault();
		return file->debugFD();
	}
	std::string getFilename() const override {
		if (!file.getPtr())
			throw io_error().asInjectedFault();
		return file->getFilename();
	}
};

// An async file implementation which wraps another async file and will randomly destroy sectors that it is writing when
// killed This is used to simulate a power failure which prevents all written data from being persisted to disk
class AsyncFileNonDurable final : public IAsyncFile, public ReferenceCounted<AsyncFileNonDurable> {
public:
	UID id;
	std::string filename;

	// For files that use atomic write and create, they are initially created with an extra suffix
	std::string initialFilename;

	// An approximation of the size of the file; .size() should be used instead of this variable in most cases
	mutable int64_t approximateSize;

	// The address of the machine that opened the file
	NetworkAddress openedAddress;

	bool aio;

private:
	// The wrapped IAsyncFile
	Reference<IAsyncFile> file;

	// The maximum amount of time a write is delayed before being passed along to the underlying file
	double maxWriteDelay;

	// Modifications which haven't been pushed to file, mapped by the location in the file that is being modified.
	// Be sure to update minSizeAfterPendingModifications when modifying pendingModifications.
	RangeMap<uint64_t, Future<Void>> pendingModifications;
	// The size of the file after the set of pendingModifications completes,
	// (the set pending at the time of reading this member). Must be updated in
	// lockstep with any inserts into the pendingModifications map. Tracking
	// this variable is necessary so that we can know the range of the file a
	// truncate is modifying, so we can insert it into the pendingModifications
	// map. Until minSizeAfterPendingModificationsIsExact is true, this is only a lower bound.
	mutable int64_t minSizeAfterPendingModifications = 0;
	mutable bool minSizeAfterPendingModificationsIsExact = false;

	// Will be blocked whenever kill is running
	Promise<Void> killed;
	Promise<Void> killComplete;

	// Used by sync (and kill) to force writes which have not yet been passed along.
	// If true is sent, then writes will be durable.  If false, then they may not be durable.
	Promise<bool> startSyncPromise;

	// The performance parameters of the simulated disk
	Reference<DiskParameters> diskParameters;

	// Set to true the first time sync is called on the file
	bool hasBeenSynced;

	// Used to describe what corruption is allowed by the file as well as the type of corruption being used on a
	// particular page
	enum KillMode { NO_CORRUPTION = 0, DROP_ONLY = 1, FULL_CORRUPTION = 2 };

	// Limits what types of corruption are applied to writes from this file
	KillMode killMode;

	ActorCollection
	    reponses; // cannot call getResult on this actor collection, since the actors will be on different processes

	AsyncFileNonDurable(const std::string& filename,
	                    const std::string& initialFilename,
	                    Reference<IAsyncFile> file,
	                    Reference<DiskParameters> diskParameters,
	                    NetworkAddress openedAddress,
	                    bool aio)
	  : filename(filename), initialFilename(initialFilename), approximateSize(0), openedAddress(openedAddress),
	    aio(aio), file(file), pendingModifications(uint64_t(-1)), diskParameters(diskParameters), reponses(false) {

		// This is only designed to work in simulation
		ASSERT(g_network->isSimulated());
		this->id = deterministicRandom()->randomUniqueID();

		//TraceEvent("AsyncFileNonDurable_Create", id).detail("Filename", filename);
		maxWriteDelay = FLOW_KNOBS->NON_DURABLE_MAX_WRITE_DELAY;
		hasBeenSynced = false;

		killMode = (KillMode)deterministicRandom()->randomInt(1, 3);
		//TraceEvent("AsyncFileNonDurable_CreateEnd", id).detail("Filename", filename).backtrace();
	}

public:
	static std::map<std::string, Future<Void>> filesBeingDeleted;

	// Creates a new AsyncFileNonDurable which wraps the provided IAsyncFile
																#line 920 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via open()
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OpenActor1>
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OpenActor1State {
															#line 926 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OpenActor1State(std::string const& filename,std::string const& actualFilename,Future<Reference<IAsyncFile>> const& wrappedFile,Reference<DiskParameters> const& diskParameters,bool const& aio) 
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : filename(filename),
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   actualFilename(actualFilename),
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   wrappedFile(wrappedFile),
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   diskParameters(diskParameters),
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   aio(aio),
															#line 229 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 230 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask()),
															#line 231 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   shutdown(success(currentProcess->shutdownSignal.getFuture()))
															#line 947 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("open", reinterpret_cast<unsigned long>(this));

	}
	~OpenActor1State() 
	{
		fdb_probe_actor_destroy("open", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 234 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 234 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 964 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OpenActor1*>(this)->actor_wait_state = 1;
															#line 234 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 0, Void >*>(static_cast<OpenActor1*>(this)));
															#line 969 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OpenActor1State();
		static_cast<OpenActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 236 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = success(wrappedFile) || shutdown;
															#line 236 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 995 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor1*>(this)->actor_wait_state = 2;
															#line 236 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 1, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1000 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 236 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = success(wrappedFile) || shutdown;
															#line 236 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1018 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OpenActor1*>(this)->actor_wait_state = 2;
															#line 236 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 1, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1023 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OpenActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 271 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 272 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			std::string currentFilename = (wrappedFile.isReady() && !wrappedFile.isError()) ? wrappedFile.get()->getFilename() : actualFilename;
															#line 274 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			currentProcess->machine->openFiles.erase(currentFilename);
															#line 276 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_6 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 276 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1110 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_6.get(), loopDepth); };
			static_cast<OpenActor1*>(this)->actor_wait_state = 7;
															#line 276 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 6, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1115 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 238 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (shutdown.isReady())
															#line 1130 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 239 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1134 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 241 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		file = wrappedFile.get();
															#line 245 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		deletedFile = filesBeingDeleted.find(filename);
															#line 246 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (deletedFile != filesBeingDeleted.end())
															#line 1142 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 248 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = deletedFile->second || shutdown;
															#line 248 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1148 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<OpenActor1*>(this)->actor_wait_state = 3;
															#line 248 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 2, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1153 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 238 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (shutdown.isReady())
															#line 1167 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 239 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1171 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 241 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		file = wrappedFile.get();
															#line 245 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		deletedFile = filesBeingDeleted.find(filename);
															#line 246 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (deletedFile != filesBeingDeleted.end())
															#line 1179 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 248 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = deletedFile->second || shutdown;
															#line 248 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1185 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<OpenActor1*>(this)->actor_wait_state = 3;
															#line 248 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 2, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1190 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont4(loopDepth);
		}

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
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OpenActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 255 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		nonDurableFile = Reference<AsyncFileNonDurable>(new AsyncFileNonDurable(filename, actualFilename, file, diskParameters, currentProcess->address, aio));
															#line 259 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		sizeFuture = nonDurableFile->size();
															#line 260 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_4 = success(sizeFuture) || shutdown;
															#line 260 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1273 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<OpenActor1*>(this)->actor_wait_state = 5;
															#line 260 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 4, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1278 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 250 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (shutdown.isReady())
															#line 1287 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 251 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1291 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 252 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 252 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1297 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<OpenActor1*>(this)->actor_wait_state = 4;
															#line 252 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 3, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1302 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 250 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (shutdown.isReady())
															#line 1311 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 251 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1315 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 252 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 252 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1321 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<OpenActor1*>(this)->actor_wait_state = 4;
															#line 252 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 3, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1326 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OpenActor1, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< OpenActor1, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 262 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (shutdown.isReady())
															#line 1473 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 263 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1477 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_5 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1cont1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont9when1(__when_expr_5.get(), loopDepth); };
		static_cast<OpenActor1*>(this)->actor_wait_state = 6;
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 5, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1488 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 262 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (shutdown.isReady())
															#line 1497 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 263 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1501 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_5 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OpenActor1*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1507 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1cont1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont9when1(__when_expr_5.get(), loopDepth); };
		static_cast<OpenActor1*>(this)->actor_wait_state = 6;
															#line 267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< OpenActor1, 5, Void >*>(static_cast<OpenActor1*>(this)));
															#line 1512 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< OpenActor1, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont10(Void const& _,int loopDepth) 
	{
															#line 269 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OpenActor1*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(nonDurableFile); this->~OpenActor1State(); static_cast<OpenActor1*>(this)->destroy(); return 0; }
															#line 1584 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OpenActor1*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(nonDurableFile)); // state_var_RVO
		this->~OpenActor1State();
		static_cast<OpenActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(Void && _,int loopDepth) 
	{
															#line 269 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OpenActor1*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(nonDurableFile); this->~OpenActor1State(); static_cast<OpenActor1*>(this)->destroy(); return 0; }
															#line 1596 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OpenActor1*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(nonDurableFile)); // state_var_RVO
		this->~OpenActor1State();
		static_cast<OpenActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont9when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont9when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont9when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< OpenActor1, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 277 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 1671 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 277 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 1679 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<OpenActor1*>(this)->actor_wait_state > 0) static_cast<OpenActor1*>(this)->actor_wait_state = 0;
		static_cast<OpenActor1*>(this)->ActorCallback< OpenActor1, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor1, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< OpenActor1, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< OpenActor1, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	std::string filename;
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	std::string actualFilename;
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Reference<IAsyncFile>> wrappedFile;
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Reference<DiskParameters> diskParameters;
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool aio;
															#line 229 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 230 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 231 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Void> shutdown;
															#line 241 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Reference<IAsyncFile> file;
															#line 245 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	std::map<std::string, Future<Void>>::iterator deletedFile;
															#line 255 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Reference<AsyncFileNonDurable> nonDurableFile;
															#line 259 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<int64_t> sizeFuture;
															#line 271 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 1772 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via open()
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OpenActor1 final : public Actor<Reference<IAsyncFile>>, public ActorCallback< OpenActor1, 0, Void >, public ActorCallback< OpenActor1, 1, Void >, public ActorCallback< OpenActor1, 2, Void >, public ActorCallback< OpenActor1, 3, Void >, public ActorCallback< OpenActor1, 4, Void >, public ActorCallback< OpenActor1, 5, Void >, public ActorCallback< OpenActor1, 6, Void >, public FastAllocated<OpenActor1>, public OpenActor1State<OpenActor1> {
															#line 1777 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OpenActor1>::operator new;
	using FastAllocated<OpenActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OpenActor1, 0, Void >;
friend struct ActorCallback< OpenActor1, 1, Void >;
friend struct ActorCallback< OpenActor1, 2, Void >;
friend struct ActorCallback< OpenActor1, 3, Void >;
friend struct ActorCallback< OpenActor1, 4, Void >;
friend struct ActorCallback< OpenActor1, 5, Void >;
friend struct ActorCallback< OpenActor1, 6, Void >;
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OpenActor1(std::string const& filename,std::string const& actualFilename,Future<Reference<IAsyncFile>> const& wrappedFile,Reference<DiskParameters> const& diskParameters,bool const& aio) 
															#line 1794 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Reference<IAsyncFile>>(),
		   OpenActor1State<OpenActor1>(filename, actualFilename, wrappedFile, diskParameters, aio)
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("open");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OpenActor1, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OpenActor1, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OpenActor1, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< OpenActor1, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< OpenActor1, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< OpenActor1, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< OpenActor1, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( std::string const& filename, std::string const& actualFilename, Future<Reference<IAsyncFile>> const& wrappedFile, Reference<DiskParameters> const& diskParameters, bool const& aio ) {
															#line 224 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Reference<IAsyncFile>>(new OpenActor1(filename, actualFilename, wrappedFile, diskParameters, aio));
															#line 1827 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 280 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	~AsyncFileNonDurable() override {
		//TraceEvent("AsyncFileNonDurable_Destroy", id).detail("Filename", filename);
	}

	void addref() override { ReferenceCounted<AsyncFileNonDurable>::addref(); }

	void delref() override {
		if (delref_no_destroy()) {
			if (filesBeingDeleted.count(filename) == 0) {
				//TraceEvent("AsyncFileNonDurable_StartDelete", id).detail("Filename", filename);
				Future<Void> deleteFuture = closeFile(this);
				if (!deleteFuture.isReady())
					filesBeingDeleted[filename] = deleteFuture;
			}

			removeOpenFile(filename, this);
			if (initialFilename != filename) {
				removeOpenFile(initialFilename, this);
			}
		}
	}

	// Removes a file from the openFiles map
	static void removeOpenFile(std::string filename, AsyncFileNonDurable* file) {
		auto& openFiles = g_simulator.getCurrentProcess()->machine->openFiles;

		auto iter = openFiles.find(filename);

		// Various actions (e.g. simulated delete) can remove a file from openFiles prematurely, so it may already
		// be gone. Renamed files (from atomic write and create) will also be present under only one of the two
		// names.
		if (iter != openFiles.end()) {
			// even if the filename exists, it doesn't mean that it references the same file. It could be that the
			// file was renamed and later a file with the same name was opened.
			if (iter->second.getPtrIfReady().orDefault(nullptr) == file) {
				openFiles.erase(iter);
			}
		}
	}

	// Passes along reads straight to the underlying file, waiting for any outstanding changes that could affect the
	// results
	Future<int> read(void* data, int length, int64_t offset) override { return read(this, data, length, offset); }

	// Writes data to the file.  Writes are delayed a random amount of time before being
	// passed to the underlying file
	Future<Void> write(void const* data, int length, int64_t offset) override {
		//TraceEvent("AsyncFileNonDurable_Write", id).detail("Filename", filename).detail("Offset", offset).detail("Length", length);
		if (length == 0) {
			TraceEvent(SevWarnAlways, "AsyncFileNonDurable_EmptyModification", id).detail("Filename", filename);
			return Void();
		}

		debugFileSet("AsyncFileNonDurableWrite", filename, data, offset, length);

		Promise<Void> writeStarted;
		Promise<Future<Void>> writeEnded;
		writeEnded.send(write(this, writeStarted, writeEnded.getFuture(), data, length, offset));
		return writeStarted.getFuture();
	}

	// Truncates the file.  Truncates are delayed a random amount of time before being
	// passed to the underlying file
	Future<Void> truncate(int64_t size) override {
		//TraceEvent("AsyncFileNonDurable_Truncate", id).detail("Filename", filename).detail("Offset", size);
		debugFileTruncate("AsyncFileNonDurableTruncate", filename, size);

		Promise<Void> truncateStarted;
		Promise<Future<Void>> truncateEnded;
		truncateEnded.send(truncate(this, truncateStarted, truncateEnded.getFuture(), size));
		return truncateStarted.getFuture();
	}

	// Fsyncs the file.  This allows all delayed modifications to the file to complete before
	// syncing the underlying file
	Future<Void> sync() override {
		//TraceEvent("AsyncFileNonDurable_Sync", id).detail("Filename", filename);
		Future<Void> syncFuture = sync(this, true);
		reponses.add(syncFuture);
		return syncFuture;
	}

	// Passes along size requests to the underlying file, augmenting with any writes past the end of the file
	Future<int64_t> size() const override { return size(this); }

	int64_t debugFD() const override { return file->debugFD(); }

	std::string getFilename() const override { return file->getFilename(); }

	// Forces a non-durable sync (some writes are not made or made incorrectly)
	// This is used when the file should 'die' without first completing its operations
	//(e.g. to simulate power failure)
	Future<Void> kill() {
		TraceEvent("AsyncFileNonDurable_Kill", id).detail("Filename", filename);
		TEST(true); // AsyncFileNonDurable was killed
		return sync(this, false);
	}

private:
	// Returns a future that is used to ensure the waiter ends up on the main thread
	Future<Void> returnToMainThread() {
		Promise<Void> p;
		Future<Void> f = p.getFuture();
		g_network->onMainThread(std::move(p), g_network->getCurrentTask());
		return f;
	}

	// Gets existing modifications that overlap the specified range.  Optionally inserts a new modification into the map
	std::vector<Future<Void>> getModificationsAndInsert(int64_t offset,
	                                                    int64_t length,
	                                                    bool insertModification = false,
	                                                    Future<Void> value = Void()) {
		auto modification = RangeMapRange<uint64_t>(offset, length >= 0 ? offset + length : uint64_t(-1));
		auto priorModifications = pendingModifications.intersectingRanges(modification);

		// Aggregate existing modifications in this range
		std::vector<Future<Void>> modificationFutures;
		for (auto itr = priorModifications.begin(); itr != priorModifications.end(); ++itr) {
			if (itr.value().isValid() && (!itr.value().isReady() || itr.value().isError())) {
				modificationFutures.push_back(itr.value());
			}
		}

		// Add the modification if we are doing a write or truncate
		if (insertModification)
			pendingModifications.insert(modification, value);

		return modificationFutures;
	}

	// Checks if the file is killed.  If so, then the current sync is completed if running and then an error is thrown
																#line 1963 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via checkKilled()
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class CheckKilledActor>
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class CheckKilledActorState {
															#line 1969 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	CheckKilledActorState(AsyncFileNonDurable const* const& self,std::string const& context) 
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   context(context)
															#line 1978 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("checkKilled", reinterpret_cast<unsigned long>(this));

	}
	~CheckKilledActorState() 
	{
		fdb_probe_actor_destroy("checkKilled", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 413 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (self->killed.isSet())
															#line 1993 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 415 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_0 = self->killComplete.getFuture();
															#line 415 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<CheckKilledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1999 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CheckKilledActor*>(this)->actor_wait_state = 1;
															#line 415 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckKilledActor, 0, Void >*>(static_cast<CheckKilledActor*>(this)));
															#line 2004 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont1(loopDepth);
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
		this->~CheckKilledActorState();
		static_cast<CheckKilledActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 423 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<CheckKilledActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckKilledActorState(); static_cast<CheckKilledActor*>(this)->destroy(); return 0; }
															#line 2032 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<CheckKilledActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckKilledActorState();
		static_cast<CheckKilledActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 416 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		TraceEvent("AsyncFileNonDurable_KilledFileOperation", self->id) .detail("In", context) .detail("Filename", self->filename);
															#line 419 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		TEST(true);
															#line 420 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 2048 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 416 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		TraceEvent("AsyncFileNonDurable_KilledFileOperation", self->id) .detail("In", context) .detail("Filename", self->filename);
															#line 419 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		TEST(true);
															#line 420 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 2060 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

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
		if (static_cast<CheckKilledActor*>(this)->actor_wait_state > 0) static_cast<CheckKilledActor*>(this)->actor_wait_state = 0;
		static_cast<CheckKilledActor*>(this)->ActorCallback< CheckKilledActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckKilledActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckKilledActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckKilledActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable const* self;
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	std::string context;
															#line 2131 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via checkKilled()
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class CheckKilledActor final : public Actor<Void>, public ActorCallback< CheckKilledActor, 0, Void >, public FastAllocated<CheckKilledActor>, public CheckKilledActorState<CheckKilledActor> {
															#line 2136 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<CheckKilledActor>::operator new;
	using FastAllocated<CheckKilledActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckKilledActor, 0, Void >;
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	CheckKilledActor(AsyncFileNonDurable const* const& self,std::string const& context) 
															#line 2147 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   CheckKilledActorState<CheckKilledActor>(self, context)
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkKilled");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckKilledActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<Void> checkKilled( AsyncFileNonDurable const* const& self, std::string const& context ) {
															#line 412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new CheckKilledActor(self, context));
															#line 2174 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 425 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	// Passes along reads straight to the underlying file, waiting for any outstanding changes that could affect the
	// results
																#line 2181 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via onRead()
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OnReadActor>
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OnReadActorState {
															#line 2187 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OnReadActorState(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   data(data),
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   length(length),
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   offset(offset)
															#line 2200 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("onRead", reinterpret_cast<unsigned long>(this));

	}
	~OnReadActorState() 
	{
		fdb_probe_actor_destroy("onRead", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 429 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = checkKilled(self, "Read");
															#line 429 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2217 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnReadActor*>(this)->actor_wait_state = 1;
															#line 429 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 0, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2222 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OnReadActorState();
		static_cast<OnReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 430 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length);
															#line 431 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(priorModifications);
															#line 431 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2249 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 2;
															#line 431 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 1, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2254 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 430 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length);
															#line 431 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(priorModifications);
															#line 431 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 2;
															#line 431 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 1, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2272 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnReadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 432 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		readFuture = self->file->read(data, length, offset);
															#line 433 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = success(readFuture) || self->killed.getFuture();
															#line 433 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2348 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 3;
															#line 433 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 2, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2353 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 432 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		readFuture = self->file->read(data, length, offset);
															#line 433 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = success(readFuture) || self->killed.getFuture();
															#line 433 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2366 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 3;
															#line 433 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 2, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2371 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnReadActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 436 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = checkKilled(self, "ReadEnd");
															#line 436 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2445 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 4;
															#line 436 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 3, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2450 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 436 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = checkKilled(self, "ReadEnd");
															#line 436 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2461 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 4;
															#line 436 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 3, Void >*>(static_cast<OnReadActor*>(this)));
															#line 2466 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OnReadActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 438 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		debugFileCheck("AsyncFileNonDurableRead", self->filename, data, offset, length);
															#line 443 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnReadActor*>(this)->SAV<int>::futures) { (void)(readFuture.get()); this->~OnReadActorState(); static_cast<OnReadActor*>(this)->destroy(); return 0; }
															#line 2540 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnReadActor*>(this)->SAV< int >::value()) int(readFuture.get());
		this->~OnReadActorState();
		static_cast<OnReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 438 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		debugFileCheck("AsyncFileNonDurableRead", self->filename, data, offset, length);
															#line 443 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnReadActor*>(this)->SAV<int>::futures) { (void)(readFuture.get()); this->~OnReadActorState(); static_cast<OnReadActor*>(this)->destroy(); return 0; }
															#line 2554 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnReadActor*>(this)->SAV< int >::value()) int(readFuture.get());
		this->~OnReadActorState();
		static_cast<OnReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< OnReadActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	void* data;
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int length;
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t offset;
															#line 432 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<int> readFuture;
															#line 2635 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via onRead()
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OnReadActor final : public Actor<int>, public ActorCallback< OnReadActor, 0, Void >, public ActorCallback< OnReadActor, 1, Void >, public ActorCallback< OnReadActor, 2, Void >, public ActorCallback< OnReadActor, 3, Void >, public FastAllocated<OnReadActor>, public OnReadActorState<OnReadActor> {
															#line 2640 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OnReadActor>::operator new;
	using FastAllocated<OnReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnReadActor, 0, Void >;
friend struct ActorCallback< OnReadActor, 1, Void >;
friend struct ActorCallback< OnReadActor, 2, Void >;
friend struct ActorCallback< OnReadActor, 3, Void >;
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OnReadActor(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 2654 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int>(),
		   OnReadActorState<OnReadActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onRead");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnReadActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnReadActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OnReadActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< OnReadActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<int> onRead( AsyncFileNonDurable* const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 428 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int>(new OnReadActor(self, data, length, offset));
															#line 2684 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 445 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 2689 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via read()
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class ReadActor>
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class ReadActorState {
															#line 2695 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ReadActorState(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   data(data),
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   length(length),
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   offset(offset),
															#line 447 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 448 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 2712 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("read", reinterpret_cast<unsigned long>(this));

	}
	~ReadActorState() 
	{
		fdb_probe_actor_destroy("read", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 449 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 449 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2729 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadActor*>(this)->actor_wait_state = 1;
															#line 449 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 0, Void >*>(static_cast<ReadActor*>(this)));
															#line 2734 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~ReadActorState();
		static_cast<ReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<int> __when_expr_1 = self->onRead(self, data, length, offset);
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2760 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ReadActor*>(this)->actor_wait_state = 2;
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 1, int >*>(static_cast<ReadActor*>(this)));
															#line 2765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<int> __when_expr_1 = self->onRead(self, data, length, offset);
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2783 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ReadActor*>(this)->actor_wait_state = 2;
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 1, int >*>(static_cast<ReadActor*>(this)));
															#line 2788 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<ReadActor*>(this)->actor_wait_state > 0) static_cast<ReadActor*>(this)->actor_wait_state = 0;
		static_cast<ReadActor*>(this)->ActorCallback< ReadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 456 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 457 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 457 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2871 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ReadActor*>(this)->actor_wait_state = 4;
															#line 457 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 3, Void >*>(static_cast<ReadActor*>(this)));
															#line 2876 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
															#line 453 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 453 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2893 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadActor*>(this)->actor_wait_state = 3;
															#line 453 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 2, Void >*>(static_cast<ReadActor*>(this)));
															#line 2898 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(int const& __rep,int loopDepth) 
	{
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		rep = __rep;
															#line 2907 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int && __rep,int loopDepth) 
	{
		rep = std::move(__rep);
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadActor*>(this)->actor_wait_state > 0) static_cast<ReadActor*>(this)->actor_wait_state = 0;
		static_cast<ReadActor*>(this)->ActorCallback< ReadActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 454 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<ReadActor*>(this)->SAV<int>::futures) { (void)(rep); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 2974 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<ReadActor*>(this)->SAV< int >::value()) int(std::move(rep)); // state_var_RVO
		this->~ReadActorState();
		static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 454 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<ReadActor*>(this)->SAV<int>::futures) { (void)(rep); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 2986 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<ReadActor*>(this)->SAV< int >::value()) int(std::move(rep)); // state_var_RVO
		this->~ReadActorState();
		static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadActor*>(this)->actor_wait_state > 0) static_cast<ReadActor*>(this)->actor_wait_state = 0;
		static_cast<ReadActor*>(this)->ActorCallback< ReadActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 458 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 3061 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 458 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 3069 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ReadActor*>(this)->actor_wait_state > 0) static_cast<ReadActor*>(this)->actor_wait_state = 0;
		static_cast<ReadActor*>(this)->ActorCallback< ReadActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ReadActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ReadActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	void* data;
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int length;
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t offset;
															#line 447 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 448 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 452 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int rep;
															#line 456 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 3152 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via read()
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class ReadActor final : public Actor<int>, public ActorCallback< ReadActor, 0, Void >, public ActorCallback< ReadActor, 1, int >, public ActorCallback< ReadActor, 2, Void >, public ActorCallback< ReadActor, 3, Void >, public FastAllocated<ReadActor>, public ReadActorState<ReadActor> {
															#line 3157 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<ReadActor>::operator new;
	using FastAllocated<ReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadActor, 0, Void >;
friend struct ActorCallback< ReadActor, 1, int >;
friend struct ActorCallback< ReadActor, 2, Void >;
friend struct ActorCallback< ReadActor, 3, Void >;
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ReadActor(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 3171 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int>(),
		   ReadActorState<ReadActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadActor, 1, int >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ReadActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<int> read( AsyncFileNonDurable* const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 446 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int>(new ReadActor(self, data, length, offset));
															#line 3201 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 461 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	// Delays writes a random amount of time before passing them through to the underlying file.
	// If a kill interrupts the delay, then the output could be the correct write, part of the write,
	// or none of the write.  It may also corrupt parts of sectors which have not been written correctly
																#line 3209 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via write()
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class WriteActor>
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class WriteActorState {
															#line 3215 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	WriteActorState(AsyncFileNonDurable* const& self,Promise<Void> const& writeStarted,Future<Future<Void>> const& ownFuture,void const* const& data,int const& length,int64_t const& offset) 
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   writeStarted(writeStarted),
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   ownFuture(ownFuture),
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   data(data),
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   length(length),
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   offset(offset),
															#line 471 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   dataCopy(StringRef((uint8_t*)data, length)),
															#line 472 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 473 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 3238 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("write", reinterpret_cast<unsigned long>(this));

	}
	~WriteActorState() 
	{
		fdb_probe_actor_destroy("write", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 474 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 474 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3255 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 1;
															#line 474 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 0, Void >*>(static_cast<WriteActor*>(this)));
															#line 3260 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 476 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator.speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 479 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 3285 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Write");
															#line 483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3291 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 2;
															#line 483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 1, Void >*>(static_cast<WriteActor*>(this)));
															#line 3296 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 476 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator.speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 479 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 3313 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Write");
															#line 483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3319 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 2;
															#line 483 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 1, Void >*>(static_cast<WriteActor*>(this)));
															#line 3324 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 511 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = true;
															#line 513 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_4 = delay(delayDuration);
															#line 512 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3406 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont2when1(__when_expr_4.get(), loopDepth); };
															#line 514 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<bool> __when_expr_5 = startSyncFuture;
															#line 3410 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont2when2(__when_expr_5.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 5;
															#line 513 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 4, Void >*>(static_cast<WriteActor*>(this)));
															#line 514 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 5, bool >*>(static_cast<WriteActor*>(this)));
															#line 3417 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 504 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->reponses.add(sendErrorOnProcess(currentProcess, writeStarted, e, currentTaskID));
															#line 505 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 3429 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 485 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 485 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3445 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 3;
															#line 485 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 2, Future<Void> >*>(static_cast<WriteActor*>(this)));
															#line 3450 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 485 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 485 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3461 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 3;
															#line 485 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 2, Future<Void> >*>(static_cast<WriteActor*>(this)));
															#line 3466 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Future<Void> const& writeEnded,int loopDepth) 
	{
															#line 486 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length, true, writeEnded);
															#line 488 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModifications = std::max(self->minSizeAfterPendingModifications, offset + length);
															#line 490 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (BUGGIFY_WITH_PROB(0.001) && !g_simulator.speedUpSimulation)
															#line 3542 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 491 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back( delay(deterministicRandom()->random01() * FLOW_KNOBS->MAX_PRIOR_MODIFICATION_DELAY) || self->killed.getFuture());
															#line 3546 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		else
		{
															#line 495 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back(waitUntilDiskReady(self->diskParameters, length) || self->killed.getFuture());
															#line 3552 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = waitForAll(priorModifications);
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3558 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 4;
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 3, Void >*>(static_cast<WriteActor*>(this)));
															#line 3563 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Future<Void> && writeEnded,int loopDepth) 
	{
															#line 486 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length, true, writeEnded);
															#line 488 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModifications = std::max(self->minSizeAfterPendingModifications, offset + length);
															#line 490 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (BUGGIFY_WITH_PROB(0.001) && !g_simulator.speedUpSimulation)
															#line 3576 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 491 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back( delay(deterministicRandom()->random01() * FLOW_KNOBS->MAX_PRIOR_MODIFICATION_DELAY) || self->killed.getFuture());
															#line 3580 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		else
		{
															#line 495 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back(waitUntilDiskReady(self->diskParameters, length) || self->killed.getFuture());
															#line 3586 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = waitForAll(priorModifications);
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3592 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 4;
															#line 498 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 3, Void >*>(static_cast<WriteActor*>(this)));
															#line 3597 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> const& writeEnded,int loopDepth) 
	{
		loopDepth = a_body1cont4(writeEnded, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> && writeEnded,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(writeEnded), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 2, Future<Void> >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 2, Future<Void> >*,Future<Void> const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteActor, 2, Future<Void> >*,Future<Void> && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteActor, 2, Future<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 500 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = std::max(self->approximateSize, length + offset);
															#line 502 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, writeStarted, currentTaskID));
															#line 3671 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 500 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = std::max(self->approximateSize, length + offset);
															#line 502 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, writeStarted, currentTaskID));
															#line 3682 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WriteActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WriteActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont9(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10(int loopDepth) 
	{
															#line 519 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		debugFileCheck("AsyncFileNonDurableWriteAfterWait", self->filename, dataCopy.begin(), offset, length);
															#line 522 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		ASSERT(!self->aio || (offset % 4096 == 0 && length % 4096 == 0));
															#line 526 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		int diskPageLength = saveDurable ? length : 4096;
															#line 527 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		int diskSectorLength = saveDurable ? length : 512;
															#line 529 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> writeFutures;
															#line 530 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		for(int writeOffset = 0;writeOffset < length;) {
															#line 532 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			int pageLength = diskPageLength;
															#line 533 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (!self->aio && !saveDurable)
															#line 3781 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 536 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				pageLength = std::min<int64_t>((int64_t)length - writeOffset, diskPageLength - ((offset + writeOffset) % diskPageLength));
															#line 3785 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			}
															#line 541 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			KillMode pageKillMode = (KillMode)deterministicRandom()->randomInt(0, self->killMode + 1);
															#line 543 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			for(int pageOffset = 0;pageOffset < pageLength;) {
															#line 545 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				int sectorLength = diskSectorLength;
															#line 546 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (!self->aio && !saveDurable)
															#line 3795 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 549 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					sectorLength = std::min<int64_t>((int64_t)length - (writeOffset + pageOffset), diskSectorLength - ((offset + writeOffset + pageOffset) % diskSectorLength));
															#line 3799 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
															#line 557 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (saveDurable || pageKillMode == NO_CORRUPTION || (pageKillMode == FULL_CORRUPTION && deterministicRandom()->random01() < 0.25))
															#line 3803 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 561 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					writeFutures.push_back(self->file->write( dataCopy.begin() + writeOffset + pageOffset, sectorLength, offset + writeOffset + pageOffset));
															#line 3807 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
				else
				{
															#line 567 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					if (pageKillMode == FULL_CORRUPTION && deterministicRandom()->random01() < 0.66667)
															#line 3813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
					{
															#line 570 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						int side = deterministicRandom()->randomInt(0, 3);
															#line 574 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						bool garbage = side == 2 || deterministicRandom()->random01() < 0.5;
															#line 576 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t goodStart = 0;
															#line 577 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t goodEnd = sectorLength;
															#line 578 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t badStart = 0;
															#line 579 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t badEnd = sectorLength;
															#line 581 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						if (side == 0)
															#line 3829 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
						{
															#line 582 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							goodEnd = deterministicRandom()->randomInt(0, sectorLength);
															#line 583 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							badStart = goodEnd;
															#line 3835 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
						}
						else
						{
															#line 584 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							if (side == 1)
															#line 3841 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
							{
															#line 585 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
								badEnd = deterministicRandom()->randomInt(0, sectorLength);
															#line 586 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
								goodStart = badEnd;
															#line 3847 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
							else
							{
															#line 588 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
								goodEnd = 0;
															#line 3853 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
						}
															#line 591 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						if (garbage && badStart != badEnd)
															#line 3858 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
						{
															#line 592 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							uint8_t* badData = const_cast<uint8_t*>(&dataCopy.begin()[badStart + writeOffset + pageOffset]);
															#line 593 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							for(int i = 0;i < badEnd - badStart;i += sizeof(uint32_t)) {
															#line 594 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
								uint32_t val = deterministicRandom()->randomUInt32();
															#line 595 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
								memcpy(&badData[i], &val, std::min(badEnd - badStart - i, (int64_t)sizeof(uint32_t)));
															#line 3868 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
															#line 598 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							writeFutures.push_back(self->file->write(dataCopy.begin() + writeOffset + pageOffset, sectorLength, offset + writeOffset + pageOffset));
															#line 601 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							debugFileSet("AsyncFileNonDurableBadWrite", self->filename, dataCopy.begin() + writeOffset + pageOffset, offset + writeOffset + pageOffset, sectorLength);
															#line 3874 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
						}
						else
						{
															#line 606 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
							if (goodStart != goodEnd)
															#line 3880 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
							{
															#line 607 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
								writeFutures.push_back( self->file->write(dataCopy.begin() + goodStart + writeOffset + pageOffset, goodEnd - goodStart, goodStart + offset + writeOffset + pageOffset));
															#line 3884 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
						}
															#line 612 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						TraceEvent("AsyncFileNonDurable_BadWrite", self->id) .detail("Offset", offset + writeOffset + pageOffset) .detail("Length", sectorLength) .detail("GoodStart", goodStart) .detail("GoodEnd", goodEnd) .detail("HasGarbage", garbage) .detail("Side", side) .detail("Filename", self->filename);
															#line 620 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						TEST(true);
															#line 3891 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
					}
					else
					{
															#line 622 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						TraceEvent("AsyncFileNonDurable_DroppedWrite", self->id) .detail("Offset", offset + writeOffset + pageOffset) .detail("Length", sectorLength) .detail("Filename", self->filename);
															#line 626 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
						TEST(true);
															#line 3899 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
					}
				}
															#line 629 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				pageOffset += sectorLength;
															#line 3904 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			}
															#line 632 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			writeOffset += pageLength;
															#line 3908 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 635 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_6 = waitForAll(writeFutures);
															#line 635 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3914 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont10when1(__when_expr_6.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 6;
															#line 635 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 6, Void >*>(static_cast<WriteActor*>(this)));
															#line 3919 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool const& durable,int loopDepth) 
	{
															#line 515 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 3940 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool && durable,int loopDepth) 
	{
															#line 515 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 3949 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 4, Void >::remove();
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 5, bool >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WriteActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WriteActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WriteActor, 5, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< WriteActor, 5, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< WriteActor, 5, bool >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont10cont1(Void const& _,int loopDepth) 
	{
															#line 637 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<WriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteActorState(); static_cast<WriteActor*>(this)->destroy(); return 0; }
															#line 4055 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<WriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont1(Void && _,int loopDepth) 
	{
															#line 637 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<WriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteActorState(); static_cast<WriteActor*>(this)->destroy(); return 0; }
															#line 4067 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<WriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< WriteActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< WriteActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Promise<Void> writeStarted;
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Future<Void>> ownFuture;
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	void const* data;
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int length;
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t offset;
															#line 471 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Standalone<StringRef> dataCopy;
															#line 472 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 473 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 476 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	double delayDuration;
															#line 479 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<bool> startSyncFuture;
															#line 511 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool saveDurable;
															#line 4162 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via write()
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class WriteActor final : public Actor<Void>, public ActorCallback< WriteActor, 0, Void >, public ActorCallback< WriteActor, 1, Void >, public ActorCallback< WriteActor, 2, Future<Void> >, public ActorCallback< WriteActor, 3, Void >, public ActorCallback< WriteActor, 4, Void >, public ActorCallback< WriteActor, 5, bool >, public ActorCallback< WriteActor, 6, Void >, public FastAllocated<WriteActor>, public WriteActorState<WriteActor> {
															#line 4167 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<WriteActor>::operator new;
	using FastAllocated<WriteActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteActor, 0, Void >;
friend struct ActorCallback< WriteActor, 1, Void >;
friend struct ActorCallback< WriteActor, 2, Future<Void> >;
friend struct ActorCallback< WriteActor, 3, Void >;
friend struct ActorCallback< WriteActor, 4, Void >;
friend struct ActorCallback< WriteActor, 5, bool >;
friend struct ActorCallback< WriteActor, 6, Void >;
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	WriteActor(AsyncFileNonDurable* const& self,Promise<Void> const& writeStarted,Future<Future<Void>> const& ownFuture,void const* const& data,int const& length,int64_t const& offset) 
															#line 4184 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   WriteActorState<WriteActor>(self, writeStarted, ownFuture, data, length, offset)
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("write");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteActor, 2, Future<Void> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WriteActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WriteActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< WriteActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> write( AsyncFileNonDurable* const& self, Promise<Void> const& writeStarted, Future<Future<Void>> const& ownFuture, void const* const& data, int const& length, int64_t const& offset ) {
															#line 465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new WriteActor(self, writeStarted, ownFuture, data, length, offset));
															#line 4216 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 639 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	// Delays truncates a random amount of time before passing them through to the underlying file.
	// If a kill interrupts the delay, then the truncate may or may not be performed
																#line 4223 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via truncate()
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class TruncateActor>
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class TruncateActorState {
															#line 4229 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TruncateActorState(AsyncFileNonDurable* const& self,Promise<Void> const& truncateStarted,Future<Future<Void>> const& ownFuture,int64_t const& size) 
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   truncateStarted(truncateStarted),
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   ownFuture(ownFuture),
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   size(size),
															#line 646 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 647 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 4246 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("truncate", reinterpret_cast<unsigned long>(this));

	}
	~TruncateActorState() 
	{
		fdb_probe_actor_destroy("truncate", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 648 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 648 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4263 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 1;
															#line 648 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 0, Void >*>(static_cast<TruncateActor*>(this)));
															#line 4268 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~TruncateActorState();
		static_cast<TruncateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 650 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator.speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 652 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 4293 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Truncate");
															#line 656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4299 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 2;
															#line 656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 1, Void >*>(static_cast<TruncateActor*>(this)));
															#line 4304 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 650 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator.speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 652 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 4321 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Truncate");
															#line 656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4327 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 2;
															#line 656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 1, Void >*>(static_cast<TruncateActor*>(this)));
															#line 4332 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TruncateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 690 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = true;
															#line 692 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_5 = delay(delayDuration);
															#line 691 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4414 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont2when1(__when_expr_5.get(), loopDepth); };
															#line 693 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<bool> __when_expr_6 = startSyncFuture;
															#line 4418 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont2when2(__when_expr_6.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 6;
															#line 692 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 5, Void >*>(static_cast<TruncateActor*>(this)));
															#line 693 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 6, bool >*>(static_cast<TruncateActor*>(this)));
															#line 4425 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 685 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->reponses.add(sendErrorOnProcess(currentProcess, truncateStarted, e, currentTaskID));
															#line 686 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 4437 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4453 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 3;
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 2, Future<Void> >*>(static_cast<TruncateActor*>(this)));
															#line 4458 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4469 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 3;
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 2, Future<Void> >*>(static_cast<TruncateActor*>(this)));
															#line 4474 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TruncateActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 662 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!self->minSizeAfterPendingModificationsIsExact)
															#line 4546 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 663 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = success(self->size());
															#line 663 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4552 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 4;
															#line 663 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 3, Void >*>(static_cast<TruncateActor*>(this)));
															#line 4557 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> const& __truncateEnded,int loopDepth) 
	{
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		truncateEnded = __truncateEnded;
															#line 4571 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> && __truncateEnded,int loopDepth) 
	{
		truncateEnded = std::move(__truncateEnded);
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 2, Future<Void> >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 2, Future<Void> >*,Future<Void> const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 2, Future<Void> >*,Future<Void> && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TruncateActor, 2, Future<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 665 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		ASSERT(self->minSizeAfterPendingModificationsIsExact);
															#line 666 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		int64_t beginModifiedRange = std::min(size, self->minSizeAfterPendingModifications);
															#line 667 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModifications = size;
															#line 669 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(beginModifiedRange, -1, true, truncateEnded);
															#line 672 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (BUGGIFY_WITH_PROB(0.001))
															#line 4646 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 673 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back( delay(deterministicRandom()->random01() * FLOW_KNOBS->MAX_PRIOR_MODIFICATION_DELAY) || self->killed.getFuture());
															#line 4650 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		else
		{
															#line 677 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back(waitUntilDiskReady(self->diskParameters, 0) || self->killed.getFuture());
															#line 4656 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 679 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_4 = waitForAll(priorModifications);
															#line 679 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4662 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 5;
															#line 679 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 4, Void >*>(static_cast<TruncateActor*>(this)));
															#line 4667 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< TruncateActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 681 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = size;
															#line 683 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, truncateStarted, currentTaskID));
															#line 4753 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 681 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = size;
															#line 683 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, truncateStarted, currentTaskID));
															#line 4764 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< TruncateActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont11(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont12(int loopDepth) 
	{
															#line 698 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (g_network->check_yield(TaskPriority::DefaultYield))
															#line 4849 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 699 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_7 = delay(0, TaskPriority::DefaultYield);
															#line 699 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4855 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont12when1(__when_expr_7.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 7;
															#line 699 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 7, Void >*>(static_cast<TruncateActor*>(this)));
															#line 4860 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont12cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool const& durable,int loopDepth) 
	{
															#line 694 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 4886 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool && durable,int loopDepth) 
	{
															#line 694 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 4895 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 5, Void >::remove();
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 6, bool >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< TruncateActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 6, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 6, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< TruncateActor, 6, bool >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont12cont1(int loopDepth) 
	{
															#line 704 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (saveDurable || self->killMode == NO_CORRUPTION || deterministicRandom()->random01() < 0.5)
															#line 5001 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 705 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_8 = self->file->truncate(size);
															#line 705 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5007 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1cont12cont1when1(__when_expr_8.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 8;
															#line 705 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 8, Void >*>(static_cast<TruncateActor*>(this)));
															#line 5012 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 707 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			TraceEvent("AsyncFileNonDurable_DroppedTruncate", self->id).detail("Size", size);
															#line 708 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			TEST(true);
															#line 5021 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = a_body1cont12cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont12cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1cont12when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1cont12when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< TruncateActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1cont12cont3(int loopDepth) 
	{
															#line 711 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<TruncateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TruncateActorState(); static_cast<TruncateActor*>(this)->destroy(); return 0; }
															#line 5106 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<TruncateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TruncateActorState();
		static_cast<TruncateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1cont12cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1cont12cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< TruncateActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 8);

	}
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Promise<Void> truncateStarted;
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Future<Void>> ownFuture;
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t size;
															#line 646 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 647 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 650 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	double delayDuration;
															#line 652 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<bool> startSyncFuture;
															#line 658 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Void> truncateEnded;
															#line 690 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool saveDurable;
															#line 5209 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via truncate()
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class TruncateActor final : public Actor<Void>, public ActorCallback< TruncateActor, 0, Void >, public ActorCallback< TruncateActor, 1, Void >, public ActorCallback< TruncateActor, 2, Future<Void> >, public ActorCallback< TruncateActor, 3, Void >, public ActorCallback< TruncateActor, 4, Void >, public ActorCallback< TruncateActor, 5, Void >, public ActorCallback< TruncateActor, 6, bool >, public ActorCallback< TruncateActor, 7, Void >, public ActorCallback< TruncateActor, 8, Void >, public FastAllocated<TruncateActor>, public TruncateActorState<TruncateActor> {
															#line 5214 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<TruncateActor>::operator new;
	using FastAllocated<TruncateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TruncateActor, 0, Void >;
friend struct ActorCallback< TruncateActor, 1, Void >;
friend struct ActorCallback< TruncateActor, 2, Future<Void> >;
friend struct ActorCallback< TruncateActor, 3, Void >;
friend struct ActorCallback< TruncateActor, 4, Void >;
friend struct ActorCallback< TruncateActor, 5, Void >;
friend struct ActorCallback< TruncateActor, 6, bool >;
friend struct ActorCallback< TruncateActor, 7, Void >;
friend struct ActorCallback< TruncateActor, 8, Void >;
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TruncateActor(AsyncFileNonDurable* const& self,Promise<Void> const& truncateStarted,Future<Future<Void>> const& ownFuture,int64_t const& size) 
															#line 5233 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   TruncateActorState<TruncateActor>(self, truncateStarted, ownFuture, size)
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("truncate");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TruncateActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TruncateActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TruncateActor, 2, Future<Void> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< TruncateActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< TruncateActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< TruncateActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< TruncateActor, 7, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< TruncateActor, 8, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> truncate( AsyncFileNonDurable* const& self, Promise<Void> const& truncateStarted, Future<Future<Void>> const& ownFuture, int64_t const& size ) {
															#line 642 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new TruncateActor(self, truncateStarted, ownFuture, size));
															#line 5267 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 713 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	// Waits for delayed modifications to the file to complete and then syncs the underlying file
	// If durable is false, then some of the delayed modifications will not be applied or will be
	// applied incorrectly
																#line 5275 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via onSync()
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OnSyncActor>
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSyncActorState {
															#line 5281 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSyncActorState(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   durable(durable)
															#line 5290 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("onSync", reinterpret_cast<unsigned long>(this));

	}
	~OnSyncActorState() 
	{
		fdb_probe_actor_destroy("onSync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 719 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			ASSERT(durable || !self->killed.isSet());
															#line 721 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (durable)
															#line 5307 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 722 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				self->hasBeenSynced = true;
															#line 723 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0, true) || self->killed.getFuture();
															#line 723 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5315 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OnSyncActor*>(this)->actor_wait_state = 1;
															#line 723 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 0, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5320 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont1(loopDepth);
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
		this->~OnSyncActorState();
		static_cast<OnSyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 726 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = checkKilled(self, durable ? "Sync" : "Kill");
															#line 726 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5350 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSyncActor*>(this)->actor_wait_state = 2;
															#line 726 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 1, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5355 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
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
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 728 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!durable)
															#line 5439 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 729 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->killed.send(Void());
															#line 5443 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 732 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> outstandingModifications;
															#line 733 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<RangeMapRange<uint64_t>> stillPendingModifications;
															#line 735 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		auto rangeItr = self->pendingModifications.ranges();
															#line 736 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = rangeItr.begin();itr != rangeItr.end();++itr) {
															#line 737 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (itr.value().isValid() && (!itr->value().isReady() || itr->value().isError()))
															#line 5455 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 738 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				outstandingModifications.push_back(itr->value());
															#line 740 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (!itr.value().isReady())
															#line 5461 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 741 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					stillPendingModifications.push_back(itr->range());
															#line 5465 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
			}
		}
															#line 745 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		Future<Void> allModifications = waitForAll(outstandingModifications);
															#line 747 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->pendingModifications.insert(RangeMapRange<uint64_t>(0, -1), Void());
															#line 748 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = stillPendingModifications.begin();itr != stillPendingModifications.end();++itr) {
															#line 749 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->pendingModifications.insert( *itr, success(allModifications));
															#line 5477 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 754 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 755 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->startSyncPromise = Promise<bool>();
															#line 758 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		writeDurable = durable || deterministicRandom()->random01() < 0.1;
															#line 759 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncPromise.send(writeDurable);
															#line 762 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (durable)
															#line 5489 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 763 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = allModifications;
															#line 763 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5495 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 3;
															#line 763 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 2, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5500 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = success(errorOr(allModifications));
															#line 765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5509 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when2(__when_expr_3.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 4;
															#line 765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 3, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5514 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 728 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!durable)
															#line 5524 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 729 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->killed.send(Void());
															#line 5528 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 732 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> outstandingModifications;
															#line 733 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<RangeMapRange<uint64_t>> stillPendingModifications;
															#line 735 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		auto rangeItr = self->pendingModifications.ranges();
															#line 736 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = rangeItr.begin();itr != rangeItr.end();++itr) {
															#line 737 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (itr.value().isValid() && (!itr->value().isReady() || itr->value().isError()))
															#line 5540 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 738 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				outstandingModifications.push_back(itr->value());
															#line 740 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (!itr.value().isReady())
															#line 5546 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 741 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					stillPendingModifications.push_back(itr->range());
															#line 5550 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
			}
		}
															#line 745 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		Future<Void> allModifications = waitForAll(outstandingModifications);
															#line 747 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->pendingModifications.insert(RangeMapRange<uint64_t>(0, -1), Void());
															#line 748 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = stillPendingModifications.begin();itr != stillPendingModifications.end();++itr) {
															#line 749 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->pendingModifications.insert( *itr, success(allModifications));
															#line 5562 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 754 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 755 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->startSyncPromise = Promise<bool>();
															#line 758 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		writeDurable = durable || deterministicRandom()->random01() < 0.1;
															#line 759 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncPromise.send(writeDurable);
															#line 762 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (durable)
															#line 5574 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 763 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = allModifications;
															#line 763 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5580 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 3;
															#line 763 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 2, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5585 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = success(errorOr(allModifications));
															#line 765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5594 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when2(__when_expr_3.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 4;
															#line 765 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 3, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5599 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}

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
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 767 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!durable)
															#line 5672 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 771 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (self->hasBeenSynced && writeDurable && deterministicRandom()->random01() < 0.5)
															#line 5676 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 772 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				TEST(true);
															#line 773 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_4 = success(errorOr(self->file->sync()));
															#line 773 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5684 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
				static_cast<OnSyncActor*>(this)->actor_wait_state = 5;
															#line 773 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 4, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5689 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont13(loopDepth);
			}
		}
		else
		{
															#line 783 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_5 = checkKilled(self, "SyncEnd");
															#line 783 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5703 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont4when2(__when_expr_5.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 6;
															#line 783 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 5, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5708 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont10(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont11(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont11(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont11(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont11(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont12(int loopDepth) 
	{
															#line 788 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnSyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSyncActorState(); static_cast<OnSyncActor*>(this)->destroy(); return 0; }
															#line 5868 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnSyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSyncActorState();
		static_cast<OnSyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont13(int loopDepth) 
	{
															#line 779 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->killComplete.send(Void());
															#line 5880 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont14(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont14(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont14(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont14(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont15(Void const& _,int loopDepth) 
	{
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_6 = self->file->sync();
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5966 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont15when1(__when_expr_6.get(), loopDepth); };
		static_cast<OnSyncActor*>(this)->actor_wait_state = 7;
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 6, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5971 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont15(Void && _,int loopDepth) 
	{
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_6 = self->file->sync();
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5982 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont15when1(__when_expr_6.get(), loopDepth); };
		static_cast<OnSyncActor*>(this)->actor_wait_state = 7;
															#line 784 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 6, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 5987 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont15(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont15(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont4when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont4when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont15cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont15cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont15when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont15cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont15when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont15cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont15when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont15when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool durable;
															#line 758 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool writeDurable;
															#line 6136 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via onSync()
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSyncActor final : public Actor<Void>, public ActorCallback< OnSyncActor, 0, Void >, public ActorCallback< OnSyncActor, 1, Void >, public ActorCallback< OnSyncActor, 2, Void >, public ActorCallback< OnSyncActor, 3, Void >, public ActorCallback< OnSyncActor, 4, Void >, public ActorCallback< OnSyncActor, 5, Void >, public ActorCallback< OnSyncActor, 6, Void >, public FastAllocated<OnSyncActor>, public OnSyncActorState<OnSyncActor> {
															#line 6141 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OnSyncActor>::operator new;
	using FastAllocated<OnSyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSyncActor, 0, Void >;
friend struct ActorCallback< OnSyncActor, 1, Void >;
friend struct ActorCallback< OnSyncActor, 2, Void >;
friend struct ActorCallback< OnSyncActor, 3, Void >;
friend struct ActorCallback< OnSyncActor, 4, Void >;
friend struct ActorCallback< OnSyncActor, 5, Void >;
friend struct ActorCallback< OnSyncActor, 6, Void >;
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSyncActor(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 6158 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   OnSyncActorState<OnSyncActor>(self, durable)
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSyncActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnSyncActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OnSyncActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< OnSyncActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< OnSyncActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< OnSyncActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< OnSyncActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> onSync( AsyncFileNonDurable* const& self, bool const& durable ) {
															#line 717 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new OnSyncActor(self, durable));
															#line 6191 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 790 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 6196 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via sync()
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class SyncActor>
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class SyncActorState {
															#line 6202 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	SyncActorState(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   durable(durable),
															#line 792 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 793 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 6215 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("sync", reinterpret_cast<unsigned long>(this));

	}
	~SyncActorState() 
	{
		fdb_probe_actor_destroy("sync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 794 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 794 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6232 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 1;
															#line 794 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 0, Void >*>(static_cast<SyncActor*>(this)));
															#line 6237 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 797 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = self->onSync(self, durable);
															#line 797 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 6263 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 2;
															#line 797 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 1, Void >*>(static_cast<SyncActor*>(this)));
															#line 6268 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 797 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = self->onSync(self, durable);
															#line 797 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 6286 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 2;
															#line 797 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 1, Void >*>(static_cast<SyncActor*>(this)));
															#line 6291 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 802 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 803 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 803 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6374 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 4;
															#line 803 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 3, Void >*>(static_cast<SyncActor*>(this)));
															#line 6379 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 798 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 798 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 6396 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SyncActor*>(this)->actor_wait_state = 3;
															#line 798 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 2, Void >*>(static_cast<SyncActor*>(this)));
															#line 6401 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 798 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 798 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 6412 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SyncActor*>(this)->actor_wait_state = 3;
															#line 798 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 2, Void >*>(static_cast<SyncActor*>(this)));
															#line 6417 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SyncActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SyncActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 800 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SyncActorState(); static_cast<SyncActor*>(this)->destroy(); return 0; }
															#line 6489 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 800 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SyncActorState(); static_cast<SyncActor*>(this)->destroy(); return 0; }
															#line 6501 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SyncActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SyncActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 804 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 6576 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 804 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 6584 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SyncActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SyncActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	bool durable;
															#line 792 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 793 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 802 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 6661 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via sync()
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class SyncActor final : public Actor<Void>, public ActorCallback< SyncActor, 0, Void >, public ActorCallback< SyncActor, 1, Void >, public ActorCallback< SyncActor, 2, Void >, public ActorCallback< SyncActor, 3, Void >, public FastAllocated<SyncActor>, public SyncActorState<SyncActor> {
															#line 6666 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<SyncActor>::operator new;
	using FastAllocated<SyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SyncActor, 0, Void >;
friend struct ActorCallback< SyncActor, 1, Void >;
friend struct ActorCallback< SyncActor, 2, Void >;
friend struct ActorCallback< SyncActor, 3, Void >;
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	SyncActor(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 6680 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   SyncActorState<SyncActor>(self, durable)
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SyncActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SyncActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SyncActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SyncActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> sync( AsyncFileNonDurable* const& self, bool const& durable ) {
															#line 791 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new SyncActor(self, durable));
															#line 6710 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 807 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	// Passes along size requests to the underlying file, augmenting with any writes past the end of the file
																#line 6716 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via onSize()
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OnSizeActor>
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSizeActorState {
															#line 6722 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSizeActorState(AsyncFileNonDurable const* const& self) 
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self)
															#line 6729 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("onSize", reinterpret_cast<unsigned long>(this));

	}
	~OnSizeActorState() 
	{
		fdb_probe_actor_destroy("onSize", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 811 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = checkKilled(self, "Size");
															#line 811 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6746 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSizeActor*>(this)->actor_wait_state = 1;
															#line 811 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 0, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 6751 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OnSizeActorState();
		static_cast<OnSizeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 812 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		sizeFuture = self->file->size();
															#line 813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = success(sizeFuture) || self->killed.getFuture();
															#line 813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6778 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 2;
															#line 813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 1, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 6783 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 812 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		sizeFuture = self->file->size();
															#line 813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = success(sizeFuture) || self->killed.getFuture();
															#line 813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6796 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 2;
															#line 813 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 1, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 6801 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnSizeActor*>(this)->actor_wait_state > 0) static_cast<OnSizeActor*>(this)->actor_wait_state = 0;
		static_cast<OnSizeActor*>(this)->ActorCallback< OnSizeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSizeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 815 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = checkKilled(self, "SizeEnd");
															#line 815 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6875 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 3;
															#line 815 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 2, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 6880 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 815 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = checkKilled(self, "SizeEnd");
															#line 815 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6891 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 3;
															#line 815 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 2, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 6896 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnSizeActor*>(this)->actor_wait_state > 0) static_cast<OnSizeActor*>(this)->actor_wait_state = 0;
		static_cast<OnSizeActor*>(this)->ActorCallback< OnSizeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnSizeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 818 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = self->minSizeAfterPendingModifications = std::max<int64_t>(sizeFuture.get(), self->minSizeAfterPendingModifications);
															#line 820 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModificationsIsExact = true;
															#line 821 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnSizeActor*>(this)->SAV<int64_t>::futures) { (void)(self->approximateSize); this->~OnSizeActorState(); static_cast<OnSizeActor*>(this)->destroy(); return 0; }
															#line 6972 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnSizeActor*>(this)->SAV< int64_t >::value()) int64_t(self->approximateSize);
		this->~OnSizeActorState();
		static_cast<OnSizeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 818 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = self->minSizeAfterPendingModifications = std::max<int64_t>(sizeFuture.get(), self->minSizeAfterPendingModifications);
															#line 820 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModificationsIsExact = true;
															#line 821 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnSizeActor*>(this)->SAV<int64_t>::futures) { (void)(self->approximateSize); this->~OnSizeActorState(); static_cast<OnSizeActor*>(this)->destroy(); return 0; }
															#line 6988 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnSizeActor*>(this)->SAV< int64_t >::value()) int64_t(self->approximateSize);
		this->~OnSizeActorState();
		static_cast<OnSizeActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<OnSizeActor*>(this)->actor_wait_state > 0) static_cast<OnSizeActor*>(this)->actor_wait_state = 0;
		static_cast<OnSizeActor*>(this)->ActorCallback< OnSizeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OnSizeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable const* self;
															#line 812 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<int64_t> sizeFuture;
															#line 7063 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via onSize()
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSizeActor final : public Actor<int64_t>, public ActorCallback< OnSizeActor, 0, Void >, public ActorCallback< OnSizeActor, 1, Void >, public ActorCallback< OnSizeActor, 2, Void >, public FastAllocated<OnSizeActor>, public OnSizeActorState<OnSizeActor> {
															#line 7068 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OnSizeActor>::operator new;
	using FastAllocated<OnSizeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSizeActor, 0, Void >;
friend struct ActorCallback< OnSizeActor, 1, Void >;
friend struct ActorCallback< OnSizeActor, 2, Void >;
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSizeActor(AsyncFileNonDurable const* const& self) 
															#line 7081 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int64_t>(),
		   OnSizeActorState<OnSizeActor>(self)
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSize");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSizeActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnSizeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OnSizeActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<int64_t> onSize( AsyncFileNonDurable const* const& self ) {
															#line 809 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int64_t>(new OnSizeActor(self));
															#line 7110 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 823 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 7115 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via size()
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class SizeActor>
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class SizeActorState {
															#line 7121 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	SizeActorState(AsyncFileNonDurable const* const& self) 
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 825 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 826 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 7132 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("size", reinterpret_cast<unsigned long>(this));

	}
	~SizeActorState() 
	{
		fdb_probe_actor_destroy("size", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 828 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 828 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7149 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 1;
															#line 828 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 0, Void >*>(static_cast<SizeActor*>(this)));
															#line 7154 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~SizeActorState();
		static_cast<SizeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<int64_t> __when_expr_1 = onSize(self);
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7180 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 2;
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 1, int64_t >*>(static_cast<SizeActor*>(this)));
															#line 7185 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<int64_t> __when_expr_1 = onSize(self);
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7203 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 2;
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 1, int64_t >*>(static_cast<SizeActor*>(this)));
															#line 7208 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SizeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SizeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 836 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 837 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 837 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7291 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 4;
															#line 837 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 3, Void >*>(static_cast<SizeActor*>(this)));
															#line 7296 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
															#line 832 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator.onProcess(currentProcess, currentTaskID);
															#line 832 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7313 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SizeActor*>(this)->actor_wait_state = 3;
															#line 832 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 2, Void >*>(static_cast<SizeActor*>(this)));
															#line 7318 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(int64_t const& __rep,int loopDepth) 
	{
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		rep = __rep;
															#line 7327 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int64_t && __rep,int loopDepth) 
	{
		rep = std::move(__rep);
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SizeActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SizeActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 834 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SizeActor*>(this)->SAV<int64_t>::futures) { (void)(rep); this->~SizeActorState(); static_cast<SizeActor*>(this)->destroy(); return 0; }
															#line 7394 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SizeActor*>(this)->SAV< int64_t >::value()) int64_t(std::move(rep)); // state_var_RVO
		this->~SizeActorState();
		static_cast<SizeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 834 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SizeActor*>(this)->SAV<int64_t>::futures) { (void)(rep); this->~SizeActorState(); static_cast<SizeActor*>(this)->destroy(); return 0; }
															#line 7406 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SizeActor*>(this)->SAV< int64_t >::value()) int64_t(std::move(rep)); // state_var_RVO
		this->~SizeActorState();
		static_cast<SizeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SizeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SizeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 838 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 7481 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 838 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 7489 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SizeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SizeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable const* self;
															#line 825 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 826 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 831 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t rep;
															#line 836 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 7566 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via size()
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class SizeActor final : public Actor<int64_t>, public ActorCallback< SizeActor, 0, Void >, public ActorCallback< SizeActor, 1, int64_t >, public ActorCallback< SizeActor, 2, Void >, public ActorCallback< SizeActor, 3, Void >, public FastAllocated<SizeActor>, public SizeActorState<SizeActor> {
															#line 7571 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<SizeActor>::operator new;
	using FastAllocated<SizeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SizeActor, 0, Void >;
friend struct ActorCallback< SizeActor, 1, int64_t >;
friend struct ActorCallback< SizeActor, 2, Void >;
friend struct ActorCallback< SizeActor, 3, Void >;
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	SizeActor(AsyncFileNonDurable const* const& self) 
															#line 7585 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int64_t>(),
		   SizeActorState<SizeActor>(self)
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("size");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SizeActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SizeActor, 1, int64_t >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SizeActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SizeActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<int64_t> size( AsyncFileNonDurable const* const& self ) {
															#line 824 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int64_t>(new SizeActor(self));
															#line 7615 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 841 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"

	// Finishes all outstanding actors on an AsyncFileNonDurable and then deletes it
																#line 7621 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via closeFile()
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
template <class CloseFileActor>
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class CloseFileActorState {
															#line 7627 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	CloseFileActorState(AsyncFileNonDurable* const& self) 
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 844 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator.getCurrentProcess()),
															#line 845 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask()),
															#line 846 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		   filename(self->filename)
															#line 7640 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("closeFile", reinterpret_cast<unsigned long>(this));

	}
	~CloseFileActorState() 
	{
		fdb_probe_actor_destroy("closeFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 848 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			g_simulator.getMachineByNetworkAddress(self->openedAddress)->deletingOrClosingFiles.insert(self->getFilename());
															#line 850 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator.onMachine(currentProcess);
															#line 850 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<CloseFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7659 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CloseFileActor*>(this)->actor_wait_state = 1;
															#line 850 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CloseFileActor, 0, Void >*>(static_cast<CloseFileActor*>(this)));
															#line 7664 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~CloseFileActorState();
		static_cast<CloseFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 853 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 854 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->startSyncPromise = Promise<bool>();
															#line 855 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			startSyncPromise.send(true);
															#line 857 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			std::vector<Future<Void>> outstandingModifications;
															#line 859 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			for(auto itr = self->pendingModifications.ranges().begin();itr != self->pendingModifications.ranges().end();++itr) {
															#line 862 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (itr->value().isValid() && !itr->value().isReady())
															#line 7698 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 863 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					outstandingModifications.push_back(itr->value());
															#line 7702 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
			}
															#line 866 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = waitForAllReady(outstandingModifications);
															#line 866 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7709 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CloseFileActor*>(this)->actor_wait_state = 2;
															#line 866 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CloseFileActor, 1, Void >*>(static_cast<CloseFileActor*>(this)));
															#line 7714 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 853 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 854 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			self->startSyncPromise = Promise<bool>();
															#line 855 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			startSyncPromise.send(true);
															#line 857 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			std::vector<Future<Void>> outstandingModifications;
															#line 859 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			for(auto itr = self->pendingModifications.ranges().begin();itr != self->pendingModifications.ranges().end();++itr) {
															#line 862 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
				if (itr->value().isValid() && !itr->value().isReady())
															#line 7740 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 863 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
					outstandingModifications.push_back(itr->value());
															#line 7744 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
			}
															#line 866 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = waitForAllReady(outstandingModifications);
															#line 866 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7751 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CloseFileActor*>(this)->actor_wait_state = 2;
															#line 866 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CloseFileActor, 1, Void >*>(static_cast<CloseFileActor*>(this)));
															#line 7756 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<CloseFileActor*>(this)->actor_wait_state > 0) static_cast<CloseFileActor*>(this)->actor_wait_state = 0;
		static_cast<CloseFileActor*>(this)->ActorCallback< CloseFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CloseFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CloseFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CloseFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 882 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 883 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(err, loopDepth);
															#line 7837 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 869 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (self->killed.isSet())
															#line 7851 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 870 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = self->killComplete.getFuture();
															#line 870 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7857 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<CloseFileActor*>(this)->actor_wait_state = 3;
															#line 870 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CloseFileActor, 2, Void >*>(static_cast<CloseFileActor*>(this)));
															#line 7862 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 869 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (self->killed.isSet())
															#line 7876 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 870 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = self->killComplete.getFuture();
															#line 870 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 7882 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<CloseFileActor*>(this)->actor_wait_state = 3;
															#line 870 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CloseFileActor, 2, Void >*>(static_cast<CloseFileActor*>(this)));
															#line 7887 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

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
		if (static_cast<CloseFileActor*>(this)->actor_wait_state > 0) static_cast<CloseFileActor*>(this)->actor_wait_state = 0;
		static_cast<CloseFileActor*>(this)->ActorCallback< CloseFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CloseFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CloseFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CloseFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 873 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		g_simulator.getMachineByNetworkAddress(self->openedAddress)->closingFiles.erase(self->getFilename());
															#line 874 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		g_simulator.getMachineByNetworkAddress(self->openedAddress) ->deletingOrClosingFiles.erase(self->getFilename());
															#line 876 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		AsyncFileNonDurable::filesBeingDeleted.erase(self->filename);
															#line 879 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		delete self;
															#line 880 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<CloseFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CloseFileActorState(); static_cast<CloseFileActor*>(this)->destroy(); return 0; }
															#line 7972 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<CloseFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CloseFileActorState();
		static_cast<CloseFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CloseFileActor*>(this)->actor_wait_state > 0) static_cast<CloseFileActor*>(this)->actor_wait_state = 0;
		static_cast<CloseFileActor*>(this)->ActorCallback< CloseFileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CloseFileActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CloseFileActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CloseFileActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 844 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 845 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 846 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	std::string filename;
															#line 882 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 8065 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via closeFile()
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
class CloseFileActor final : public Actor<Void>, public ActorCallback< CloseFileActor, 0, Void >, public ActorCallback< CloseFileActor, 1, Void >, public ActorCallback< CloseFileActor, 2, Void >, public FastAllocated<CloseFileActor>, public CloseFileActorState<CloseFileActor> {
															#line 8070 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<CloseFileActor>::operator new;
	using FastAllocated<CloseFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CloseFileActor, 0, Void >;
friend struct ActorCallback< CloseFileActor, 1, Void >;
friend struct ActorCallback< CloseFileActor, 2, Void >;
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	CloseFileActor(AsyncFileNonDurable* const& self) 
															#line 8083 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   CloseFileActorState<CloseFileActor>(self)
	{
		fdb_probe_actor_enter("closeFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("closeFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("closeFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CloseFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CloseFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CloseFileActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> closeFile( AsyncFileNonDurable* const& self ) {
															#line 843 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new CloseFileActor(self));
															#line 8112 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 886 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.h"
};

#include "flow/unactorcompiler.h"
#endif
