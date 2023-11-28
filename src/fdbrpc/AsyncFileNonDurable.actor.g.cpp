#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
/*
 * AsyncFileNonDurable.actor.cpp
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

#include "fdbrpc/AsyncFileNonDurable.actor.h"
#include "flow/actorcompiler.h" // has to be last include

std::map<std::string, Future<Void>> AsyncFileNonDurable::filesBeingDeleted;

															#line 28 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
namespace {
// This generated class is to be used only via sendOnProcess()
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class SendOnProcessActor>
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendOnProcessActorState {
															#line 35 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendOnProcessActorState(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,TaskPriority const& taskID) 
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : process(process),
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   promise(promise),
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   taskID(taskID)
															#line 46 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("sendOnProcess", reinterpret_cast<unsigned long>(this));

	}
	~SendOnProcessActorState() 
	{
		fdb_probe_actor_destroy("sendOnProcess", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 27 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator.onProcess(process, taskID);
															#line 27 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<SendOnProcessActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 63 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendOnProcessActor*>(this)->actor_wait_state = 1;
															#line 27 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendOnProcessActor, 0, Void >*>(static_cast<SendOnProcessActor*>(this)));
															#line 68 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~SendOnProcessActorState();
		static_cast<SendOnProcessActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 28 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.send(Void());
															#line 29 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendOnProcessActorState(); static_cast<SendOnProcessActor*>(this)->destroy(); return 0; }
															#line 93 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<SendOnProcessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendOnProcessActorState();
		static_cast<SendOnProcessActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 28 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.send(Void());
															#line 29 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendOnProcessActorState(); static_cast<SendOnProcessActor*>(this)->destroy(); return 0; }
															#line 107 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<SendOnProcessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendOnProcessActorState();
		static_cast<SendOnProcessActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SendOnProcessActor*>(this)->actor_wait_state > 0) static_cast<SendOnProcessActor*>(this)->actor_wait_state = 0;
		static_cast<SendOnProcessActor*>(this)->ActorCallback< SendOnProcessActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendOnProcessActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendOnProcess", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendOnProcess", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendOnProcessActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendOnProcess", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendOnProcess", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendOnProcessActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendOnProcess", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendOnProcess", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* process;
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Promise<Void> promise;
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority taskID;
															#line 184 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via sendOnProcess()
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendOnProcessActor final : public Actor<Void>, public ActorCallback< SendOnProcessActor, 0, Void >, public FastAllocated<SendOnProcessActor>, public SendOnProcessActorState<SendOnProcessActor> {
															#line 189 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<SendOnProcessActor>::operator new;
	using FastAllocated<SendOnProcessActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendOnProcessActor, 0, Void >;
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendOnProcessActor(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,TaskPriority const& taskID) 
															#line 200 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<Void>(),
		   SendOnProcessActorState<SendOnProcessActor>(process, promise, taskID)
	{
		fdb_probe_actor_enter("sendOnProcess", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendOnProcess");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendOnProcess", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendOnProcessActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Void> sendOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, TaskPriority const& taskID ) {
															#line 26 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Void>(new SendOnProcessActor(process, promise, taskID));
															#line 228 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 31 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"

															#line 233 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
namespace {
// This generated class is to be used only via sendErrorOnProcess()
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class SendErrorOnProcessActor>
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendErrorOnProcessActorState {
															#line 240 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendErrorOnProcessActorState(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,Error const& e,TaskPriority const& taskID) 
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : process(process),
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   promise(promise),
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   e(e),
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   taskID(taskID)
															#line 253 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("sendErrorOnProcess", reinterpret_cast<unsigned long>(this));

	}
	~SendErrorOnProcessActorState() 
	{
		fdb_probe_actor_destroy("sendErrorOnProcess", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 36 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator.onProcess(process, taskID);
															#line 36 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<SendErrorOnProcessActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 270 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendErrorOnProcessActor*>(this)->actor_wait_state = 1;
															#line 36 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendErrorOnProcessActor, 0, Void >*>(static_cast<SendErrorOnProcessActor*>(this)));
															#line 275 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~SendErrorOnProcessActorState();
		static_cast<SendErrorOnProcessActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 37 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.sendError(e);
															#line 38 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendErrorOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendErrorOnProcessActorState(); static_cast<SendErrorOnProcessActor*>(this)->destroy(); return 0; }
															#line 300 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<SendErrorOnProcessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendErrorOnProcessActorState();
		static_cast<SendErrorOnProcessActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 37 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.sendError(e);
															#line 38 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendErrorOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendErrorOnProcessActorState(); static_cast<SendErrorOnProcessActor*>(this)->destroy(); return 0; }
															#line 314 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<SendErrorOnProcessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendErrorOnProcessActorState();
		static_cast<SendErrorOnProcessActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SendErrorOnProcessActor*>(this)->actor_wait_state > 0) static_cast<SendErrorOnProcessActor*>(this)->actor_wait_state = 0;
		static_cast<SendErrorOnProcessActor*>(this)->ActorCallback< SendErrorOnProcessActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendErrorOnProcessActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendErrorOnProcessActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendErrorOnProcessActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* process;
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Promise<Void> promise;
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Error e;
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority taskID;
															#line 393 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via sendErrorOnProcess()
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendErrorOnProcessActor final : public Actor<Void>, public ActorCallback< SendErrorOnProcessActor, 0, Void >, public FastAllocated<SendErrorOnProcessActor>, public SendErrorOnProcessActorState<SendErrorOnProcessActor> {
															#line 398 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<SendErrorOnProcessActor>::operator new;
	using FastAllocated<SendErrorOnProcessActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendErrorOnProcessActor, 0, Void >;
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendErrorOnProcessActor(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,Error const& e,TaskPriority const& taskID) 
															#line 409 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<Void>(),
		   SendErrorOnProcessActorState<SendErrorOnProcessActor>(process, promise, e, taskID)
	{
		fdb_probe_actor_enter("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendErrorOnProcess");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendErrorOnProcess", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendErrorOnProcessActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Void> sendErrorOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, Error const& e, TaskPriority const& taskID ) {
															#line 32 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Void>(new SendErrorOnProcessActor(process, promise, e, taskID));
															#line 437 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 40 "/usr/src/libfdb_c/fdbrpc/AsyncFileNonDurable.actor.cpp"
