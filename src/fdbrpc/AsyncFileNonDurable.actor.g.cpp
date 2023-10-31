#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
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

#include "fdbrpc/SimulatorMachineInfo.h"
#include "fdbrpc/SimulatorProcessInfo.h"

#include "flow/actorcompiler.h" // has to be last include

std::map<std::string, Future<Void>> AsyncFileNonDurable::filesBeingDeleted;

Future<Void> waitShutdownSignal() {
	return success(g_simulator->getCurrentProcess()->shutdownSignal.getFuture());
}

															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
namespace {
// This generated class is to be used only via sendOnProcess()
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class SendOnProcessActor>
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendOnProcessActorState {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendOnProcessActorState(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,TaskPriority const& taskID) 
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : process(process),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   promise(promise),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   taskID(taskID)
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onProcess(process, taskID);
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<SendOnProcessActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendOnProcessActor*>(this)->actor_wait_state = 1;
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendOnProcessActor, 0, Void >*>(static_cast<SendOnProcessActor*>(this)));
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.send(Void());
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendOnProcessActorState(); static_cast<SendOnProcessActor*>(this)->destroy(); return 0; }
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<SendOnProcessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendOnProcessActorState();
		static_cast<SendOnProcessActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.send(Void());
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendOnProcessActorState(); static_cast<SendOnProcessActor*>(this)->destroy(); return 0; }
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* process;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Promise<Void> promise;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority taskID;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via sendOnProcess()
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendOnProcessActor final : public Actor<Void>, public ActorCallback< SendOnProcessActor, 0, Void >, public FastAllocated<SendOnProcessActor>, public SendOnProcessActorState<SendOnProcessActor> {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<SendOnProcessActor>::operator new;
	using FastAllocated<SendOnProcessActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendOnProcessActor, 0, Void >;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendOnProcessActor(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,TaskPriority const& taskID) 
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Void> sendOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, TaskPriority const& taskID ) {
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Void>(new SendOnProcessActor(process, promise, taskID));
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
namespace {
// This generated class is to be used only via sendErrorOnProcess()
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class SendErrorOnProcessActor>
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendErrorOnProcessActorState {
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendErrorOnProcessActorState(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,Error const& e,TaskPriority const& taskID) 
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : process(process),
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   promise(promise),
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   e(e),
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   taskID(taskID)
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onProcess(process, taskID);
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<SendErrorOnProcessActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendErrorOnProcessActor*>(this)->actor_wait_state = 1;
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendErrorOnProcessActor, 0, Void >*>(static_cast<SendErrorOnProcessActor*>(this)));
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.sendError(e);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendErrorOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendErrorOnProcessActorState(); static_cast<SendErrorOnProcessActor*>(this)->destroy(); return 0; }
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<SendErrorOnProcessActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendErrorOnProcessActorState();
		static_cast<SendErrorOnProcessActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		promise.sendError(e);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<SendErrorOnProcessActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendErrorOnProcessActorState(); static_cast<SendErrorOnProcessActor*>(this)->destroy(); return 0; }
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* process;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Promise<Void> promise;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Error e;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority taskID;
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via sendErrorOnProcess()
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class SendErrorOnProcessActor final : public Actor<Void>, public ActorCallback< SendErrorOnProcessActor, 0, Void >, public FastAllocated<SendErrorOnProcessActor>, public SendErrorOnProcessActorState<SendErrorOnProcessActor> {
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<SendErrorOnProcessActor>::operator new;
	using FastAllocated<SendErrorOnProcessActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendErrorOnProcessActor, 0, Void >;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	SendErrorOnProcessActor(ISimulator::ProcessInfo* const& process,Promise<Void> const& promise,Error const& e,TaskPriority const& taskID) 
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Void> sendErrorOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, Error const& e, TaskPriority const& taskID ) {
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Void>(new SendErrorOnProcessActor(process, promise, e, taskID));
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
// This generated class is to be used only via doShutdown()
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class AsyncFileDetachable_DoShutdownActor>
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileDetachable_DoShutdownActorState {
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileDetachable_DoShutdownActorState(AsyncFileDetachable* const& self) 
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : self(self)
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("doShutdown", reinterpret_cast<unsigned long>(this));

	}
	~AsyncFileDetachable_DoShutdownActorState() 
	{
		fdb_probe_actor_destroy("doShutdown", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(g_simulator->getCurrentProcess()->shutdownSignal.getFuture());
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->actor_wait_state = 1;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >*>(static_cast<AsyncFileDetachable_DoShutdownActor*>(this)));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~AsyncFileDetachable_DoShutdownActorState();
		static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		self->file = Reference<IAsyncFile>();
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AsyncFileDetachable_DoShutdownActorState(); static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->destroy(); return 0; }
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AsyncFileDetachable_DoShutdownActorState();
		static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		self->file = Reference<IAsyncFile>();
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AsyncFileDetachable_DoShutdownActorState(); static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->destroy(); return 0; }
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AsyncFileDetachable_DoShutdownActorState();
		static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileDetachable_DoShutdownActor*>(this)->ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >*,Error err) 
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
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileDetachable* self;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via doShutdown()
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileDetachable_DoShutdownActor final : public Actor<Void>, public ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >, public FastAllocated<AsyncFileDetachable_DoShutdownActor>, public AsyncFileDetachable_DoShutdownActorState<AsyncFileDetachable_DoShutdownActor> {
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<AsyncFileDetachable_DoShutdownActor>::operator new;
	using FastAllocated<AsyncFileDetachable_DoShutdownActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >;
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileDetachable_DoShutdownActor(AsyncFileDetachable* const& self) 
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<Void>(),
		   AsyncFileDetachable_DoShutdownActorState<AsyncFileDetachable_DoShutdownActor>(self)
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
		case 1: this->a_callback_error((ActorCallback< AsyncFileDetachable_DoShutdownActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Void> AsyncFileDetachable::doShutdown( AsyncFileDetachable* const& self ) {
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Void>(new AsyncFileDetachable_DoShutdownActor(self));
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
// This generated class is to be used only via open()
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class AsyncFileDetachable_OpenActor>
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileDetachable_OpenActorState {
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileDetachable_OpenActorState(Future<Reference<IAsyncFile>> const& wrappedFile) 
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : wrappedFile(wrappedFile)
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("open", reinterpret_cast<unsigned long>(this));

	}
	~AsyncFileDetachable_OpenActorState() 
	{
		fdb_probe_actor_destroy("open", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(g_simulator->getCurrentProcess()->shutdownSignal.getFuture());
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileDetachable_OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_1 = wrappedFile;
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileDetachable_OpenActor*>(this)->actor_wait_state = 1;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >*>(static_cast<AsyncFileDetachable_OpenActor*>(this)));
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >*>(static_cast<AsyncFileDetachable_OpenActor*>(this)));
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~AsyncFileDetachable_OpenActorState();
		static_cast<AsyncFileDetachable_OpenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Reference<IAsyncFile> const& f,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileDetachable_OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(makeReference<AsyncFileDetachable>(f)); this->~AsyncFileDetachable_OpenActorState(); static_cast<AsyncFileDetachable_OpenActor*>(this)->destroy(); return 0; }
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileDetachable_OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(makeReference<AsyncFileDetachable>(f));
		this->~AsyncFileDetachable_OpenActorState();
		static_cast<AsyncFileDetachable_OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Reference<IAsyncFile> && f,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileDetachable_OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(makeReference<AsyncFileDetachable>(f)); this->~AsyncFileDetachable_OpenActorState(); static_cast<AsyncFileDetachable_OpenActor*>(this)->destroy(); return 0; }
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileDetachable_OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(makeReference<AsyncFileDetachable>(f));
		this->~AsyncFileDetachable_OpenActorState();
		static_cast<AsyncFileDetachable_OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AsyncFileDetachable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileDetachable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileDetachable_OpenActor*>(this)->ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >::remove();
		static_cast<AsyncFileDetachable_OpenActor*>(this)->ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >*,Error err) 
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
	void a_callback_fire(ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
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
	void a_callback_error(ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >*,Error err) 
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
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Future<Reference<IAsyncFile>> wrappedFile;
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via open()
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileDetachable_OpenActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >, public ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >, public FastAllocated<AsyncFileDetachable_OpenActor>, public AsyncFileDetachable_OpenActorState<AsyncFileDetachable_OpenActor> {
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<AsyncFileDetachable_OpenActor>::operator new;
	using FastAllocated<AsyncFileDetachable_OpenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >;
friend struct ActorCallback< AsyncFileDetachable_OpenActor, 1, Reference<IAsyncFile> >;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileDetachable_OpenActor(Future<Reference<IAsyncFile>> const& wrappedFile) 
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<Reference<IAsyncFile>>(),
		   AsyncFileDetachable_OpenActorState<AsyncFileDetachable_OpenActor>(wrappedFile)
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
		case 1: this->a_callback_error((ActorCallback< AsyncFileDetachable_OpenActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Reference<IAsyncFile>> AsyncFileDetachable::open( Future<Reference<IAsyncFile>> const& wrappedFile ) {
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Reference<IAsyncFile>>(new AsyncFileDetachable_OpenActor(wrappedFile));
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

Future<int> AsyncFileDetachable::read(void* data, int length, int64_t offset) {
	if (!file.getPtr() || g_simulator->getCurrentProcess()->shutdownSignal.getFuture().isReady())
		return io_error().asInjectedFault();
	return sendErrorOnShutdown(file->read(data, length, offset), assertOnReadWriteCancel);
}

Future<Void> AsyncFileDetachable::write(void const* data, int length, int64_t offset) {
	if (!file.getPtr() || g_simulator->getCurrentProcess()->shutdownSignal.getFuture().isReady())
		return io_error().asInjectedFault();
	return sendErrorOnShutdown(file->write(data, length, offset), assertOnReadWriteCancel);
}

Future<Void> AsyncFileDetachable::truncate(int64_t size) {
	if (!file.getPtr() || g_simulator->getCurrentProcess()->shutdownSignal.getFuture().isReady())
		return io_error().asInjectedFault();
	return sendErrorOnShutdown(file->truncate(size));
}

Future<Void> AsyncFileDetachable::sync() {
	if (!file.getPtr() || g_simulator->getCurrentProcess()->shutdownSignal.getFuture().isReady())
		return io_error().asInjectedFault();
	return sendErrorOnShutdown(file->sync());
}

Future<int64_t> AsyncFileDetachable::size() const {
	if (!file.getPtr() || g_simulator->getCurrentProcess()->shutdownSignal.getFuture().isReady())
		return io_error().asInjectedFault();
	return sendErrorOnShutdown(file->size());
}

															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
// This generated class is to be used only via open()
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class AsyncFileNonDurable_OpenActor>
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileNonDurable_OpenActorState {
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable_OpenActorState(std::string const& filename,std::string const& actualFilename,Future<Reference<IAsyncFile>> const& wrappedFile,Reference<DiskParameters> const& diskParameters,bool const& aio) 
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : filename(filename),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   actualFilename(actualFilename),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   wrappedFile(wrappedFile),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   diskParameters(diskParameters),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   aio(aio),
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   currentTaskID(g_network->getCurrentTask()),
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   shutdown(success(currentProcess->shutdownSignal.getFuture()))
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("open", reinterpret_cast<unsigned long>(this));

	}
	~AsyncFileNonDurable_OpenActorState() 
	{
		fdb_probe_actor_destroy("open", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 1;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~AsyncFileNonDurable_OpenActorState();
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(wrappedFile) || shutdown;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 2;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(wrappedFile) || shutdown;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 2;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >*,Error err) 
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
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			err = e;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			std::string currentFilename = (wrappedFile.isReady() && !wrappedFile.isError()) ? wrappedFile.get()->getFilename() : actualFilename;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			currentProcess->machine->openFiles.erase(currentFilename);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_6 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_6.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 7;
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (shutdown.isReady())
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		}
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		file = wrappedFile.get();
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		deletedFile = AsyncFileNonDurable::filesBeingDeleted.find(filename);
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (deletedFile != AsyncFileNonDurable::filesBeingDeleted.end())
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_2 = deletedFile->second || shutdown;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 3;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (shutdown.isReady())
															#line 1170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		}
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		file = wrappedFile.get();
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		deletedFile = AsyncFileNonDurable::filesBeingDeleted.find(filename);
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (deletedFile != AsyncFileNonDurable::filesBeingDeleted.end())
															#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_2 = deletedFile->second || shutdown;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 3;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >*,Error err) 
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
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		nonDurableFile = Reference<AsyncFileNonDurable>(new AsyncFileNonDurable(filename, actualFilename, file, diskParameters, currentProcess->address, aio));
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		sizeFuture = nonDurableFile->size();
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		StrictFuture<Void> __when_expr_4 = success(sizeFuture) || shutdown;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 5;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (shutdown.isReady())
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		}
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 4;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (shutdown.isReady())
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		}
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 4;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >*,Error err) 
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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >*,Error err) 
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
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (shutdown.isReady())
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		StrictFuture<Void> __when_expr_5 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1cont1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont9when1(__when_expr_5.get(), loopDepth); };
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 6;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (shutdown.isReady())
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1cont1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		StrictFuture<Void> __when_expr_5 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1cont1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont9when1(__when_expr_5.get(), loopDepth); };
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 6;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >*>(static_cast<AsyncFileNonDurable_OpenActor*>(this)));
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >*,Error err) 
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
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileNonDurable_OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(nonDurableFile); this->~AsyncFileNonDurable_OpenActorState(); static_cast<AsyncFileNonDurable_OpenActor*>(this)->destroy(); return 0; }
															#line 1587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileNonDurable_OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(nonDurableFile)); // state_var_RVO
		this->~AsyncFileNonDurable_OpenActorState();
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(Void && _,int loopDepth) 
	{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileNonDurable_OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(nonDurableFile); this->~AsyncFileNonDurable_OpenActorState(); static_cast<AsyncFileNonDurable_OpenActor*>(this)->destroy(); return 0; }
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileNonDurable_OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(std::move(nonDurableFile)); // state_var_RVO
		this->~AsyncFileNonDurable_OpenActorState();
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >*,Error err) 
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
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 1674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"

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
		if (static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_OpenActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_OpenActor*>(this)->ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >*,Error err) 
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
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	std::string filename;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	std::string actualFilename;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Future<Reference<IAsyncFile>> wrappedFile;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Reference<DiskParameters> diskParameters;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	bool aio;
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority currentTaskID;
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Future<Void> shutdown;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Reference<IAsyncFile> file;
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	std::map<std::string, Future<Void>>::iterator deletedFile;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Reference<AsyncFileNonDurable> nonDurableFile;
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Future<int64_t> sizeFuture;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Error err;
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via open()
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileNonDurable_OpenActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >, public ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >, public ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >, public ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >, public ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >, public ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >, public ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >, public FastAllocated<AsyncFileNonDurable_OpenActor>, public AsyncFileNonDurable_OpenActorState<AsyncFileNonDurable_OpenActor> {
															#line 1780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<AsyncFileNonDurable_OpenActor>::operator new;
	using FastAllocated<AsyncFileNonDurable_OpenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >;
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >;
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >;
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >;
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >;
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >;
friend struct ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable_OpenActor(std::string const& filename,std::string const& actualFilename,Future<Reference<IAsyncFile>> const& wrappedFile,Reference<DiskParameters> const& diskParameters,bool const& aio) 
															#line 1797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<Reference<IAsyncFile>>(),
		   AsyncFileNonDurable_OpenActorState<AsyncFileNonDurable_OpenActor>(filename, actualFilename, wrappedFile, diskParameters, aio)
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
		case 1: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< AsyncFileNonDurable_OpenActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Reference<IAsyncFile>> AsyncFileNonDurable::open( std::string const& filename, std::string const& actualFilename, Future<Reference<IAsyncFile>> const& wrappedFile, Reference<DiskParameters> const& diskParameters, bool const& aio ) {
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Reference<IAsyncFile>>(new AsyncFileNonDurable_OpenActor(filename, actualFilename, wrappedFile, diskParameters, aio));
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

															#line 1835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
// This generated class is to be used only via read()
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class AsyncFileNonDurable_ReadActor>
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileNonDurable_ReadActorState {
															#line 1841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable_ReadActorState(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : self(self),
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   data(data),
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   length(length),
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   offset(offset),
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   currentTaskID(g_network->getCurrentTask())
															#line 1858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("read", reinterpret_cast<unsigned long>(this));

	}
	~AsyncFileNonDurable_ReadActorState() 
	{
		fdb_probe_actor_destroy("read", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 1;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >*>(static_cast<AsyncFileNonDurable_ReadActor*>(this)));
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~AsyncFileNonDurable_ReadActorState();
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<int> __when_expr_1 = self->onRead(self, data, length, offset);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 2;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >*>(static_cast<AsyncFileNonDurable_ReadActor*>(this)));
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<int> __when_expr_1 = self->onRead(self, data, length, offset);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 2;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >*>(static_cast<AsyncFileNonDurable_ReadActor*>(this)));
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >*,Error err) 
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
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			err = e;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 4;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >*>(static_cast<AsyncFileNonDurable_ReadActor*>(this)));
															#line 2022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		StrictFuture<Void> __when_expr_2 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 3;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >*>(static_cast<AsyncFileNonDurable_ReadActor*>(this)));
															#line 2044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(int const& __rep,int loopDepth) 
	{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		rep = __rep;
															#line 2053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >*,int const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >*,int && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >*,Error err) 
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
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileNonDurable_ReadActor*>(this)->SAV<int>::futures) { (void)(rep); this->~AsyncFileNonDurable_ReadActorState(); static_cast<AsyncFileNonDurable_ReadActor*>(this)->destroy(); return 0; }
															#line 2120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileNonDurable_ReadActor*>(this)->SAV< int >::value()) int(std::move(rep)); // state_var_RVO
		this->~AsyncFileNonDurable_ReadActorState();
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileNonDurable_ReadActor*>(this)->SAV<int>::futures) { (void)(rep); this->~AsyncFileNonDurable_ReadActorState(); static_cast<AsyncFileNonDurable_ReadActor*>(this)->destroy(); return 0; }
															#line 2132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileNonDurable_ReadActor*>(this)->SAV< int >::value()) int(std::move(rep)); // state_var_RVO
		this->~AsyncFileNonDurable_ReadActorState();
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >*,Error err) 
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
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 2207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		return a_body1Catch1(err, loopDepth);
															#line 2215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"

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
		if (static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_ReadActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_ReadActor*>(this)->ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >*,Error err) 
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
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable* self;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	void* data;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	int length;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	int64_t offset;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority currentTaskID;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	int rep;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Error err;
															#line 2298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via read()
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileNonDurable_ReadActor final : public Actor<int>, public ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >, public ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >, public ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >, public ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >, public FastAllocated<AsyncFileNonDurable_ReadActor>, public AsyncFileNonDurable_ReadActorState<AsyncFileNonDurable_ReadActor> {
															#line 2303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<AsyncFileNonDurable_ReadActor>::operator new;
	using FastAllocated<AsyncFileNonDurable_ReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >;
friend struct ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >;
friend struct ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >;
friend struct ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable_ReadActor(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 2317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<int>(),
		   AsyncFileNonDurable_ReadActorState<AsyncFileNonDurable_ReadActor>(self, data, length, offset)
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
		case 1: this->a_callback_error((ActorCallback< AsyncFileNonDurable_ReadActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AsyncFileNonDurable_ReadActor, 1, int >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AsyncFileNonDurable_ReadActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< AsyncFileNonDurable_ReadActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<int> AsyncFileNonDurable::read( AsyncFileNonDurable* const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<int>(new AsyncFileNonDurable_ReadActor(self, data, length, offset));
															#line 2347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

															#line 2352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
// This generated class is to be used only via closeFile()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
template <class AsyncFileNonDurable_CloseFileActor>
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileNonDurable_CloseFileActorState {
															#line 2358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable_CloseFileActorState(AsyncFileNonDurable* const& self) 
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		 : self(self),
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   currentTaskID(g_network->getCurrentTask()),
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		   filename(self->filename)
															#line 2371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
	{
		fdb_probe_actor_create("closeFile", reinterpret_cast<unsigned long>(this));

	}
	~AsyncFileNonDurable_CloseFileActorState() 
	{
		fdb_probe_actor_destroy("closeFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			g_simulator->getMachineByNetworkAddress(self->openedAddress)->deletingOrClosingFiles.insert(self->getFilename());
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 1;
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >*>(static_cast<AsyncFileNonDurable_CloseFileActor*>(this)));
															#line 2395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		this->~AsyncFileNonDurable_CloseFileActorState();
		static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			self->startSyncPromise = Promise<bool>();
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			startSyncPromise.send(true);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			std::vector<Future<Void>> outstandingModifications;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			for(auto itr = self->pendingModifications.ranges().begin();itr != self->pendingModifications.ranges().end();++itr) {
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
				if (itr->value().isValid() && !itr->value().isReady())
															#line 2429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
				{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
					outstandingModifications.push_back(itr->value());
															#line 2433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
				}
			}
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_1 = waitForAllReady(outstandingModifications);
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 2;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >*>(static_cast<AsyncFileNonDurable_CloseFileActor*>(this)));
															#line 2445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			self->startSyncPromise = Promise<bool>();
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			startSyncPromise.send(true);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			std::vector<Future<Void>> outstandingModifications;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			for(auto itr = self->pendingModifications.ranges().begin();itr != self->pendingModifications.ranges().end();++itr) {
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
				if (itr->value().isValid() && !itr->value().isReady())
															#line 2471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
				{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
					outstandingModifications.push_back(itr->value());
															#line 2475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
				}
			}
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_1 = waitForAllReady(outstandingModifications);
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 2;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >*>(static_cast<AsyncFileNonDurable_CloseFileActor*>(this)));
															#line 2487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >*,Error err) 
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
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			err = e;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			return a_body1Catch1(err, loopDepth);
															#line 2568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (self->killed.isSet())
															#line 2582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_2 = self->killComplete.getFuture();
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 3;
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >*>(static_cast<AsyncFileNonDurable_CloseFileActor*>(this)));
															#line 2593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (self->killed.isSet())
															#line 2607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			StrictFuture<Void> __when_expr_2 = self->killComplete.getFuture();
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 3;
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >*>(static_cast<AsyncFileNonDurable_CloseFileActor*>(this)));
															#line 2618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
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
		if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >*,Error err) 
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
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		g_simulator->getMachineByNetworkAddress(self->openedAddress)->closingFiles.erase(self->getFilename());
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		g_simulator->getMachineByNetworkAddress(self->openedAddress)->deletingOrClosingFiles.erase(self->getFilename());
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		AsyncFileNonDurable::filesBeingDeleted.erase(self->filename);
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		delete self;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
		if (!static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AsyncFileNonDurable_CloseFileActorState(); static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->destroy(); return 0; }
															#line 2703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		new (&static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AsyncFileNonDurable_CloseFileActorState();
		static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state > 0) static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->actor_wait_state = 0;
		static_cast<AsyncFileNonDurable_CloseFileActor*>(this)->ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >*,Void const& value) 
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
	void a_callback_fire(ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >*,Void && value) 
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
	void a_callback_error(ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >*,Error err) 
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
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable* self;
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	ISimulator::ProcessInfo* currentProcess;
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	TaskPriority currentTaskID;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	std::string filename;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	Error err;
															#line 2796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
};
// This generated class is to be used only via closeFile()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
class AsyncFileNonDurable_CloseFileActor final : public Actor<Void>, public ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >, public ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >, public ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >, public FastAllocated<AsyncFileNonDurable_CloseFileActor>, public AsyncFileNonDurable_CloseFileActorState<AsyncFileNonDurable_CloseFileActor> {
															#line 2801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
public:
	using FastAllocated<AsyncFileNonDurable_CloseFileActor>::operator new;
	using FastAllocated<AsyncFileNonDurable_CloseFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >;
friend struct ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >;
friend struct ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	AsyncFileNonDurable_CloseFileActor(AsyncFileNonDurable* const& self) 
															#line 2814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
		 : Actor<Void>(),
		   AsyncFileNonDurable_CloseFileActorState<AsyncFileNonDurable_CloseFileActor>(self)
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
		case 1: this->a_callback_error((ActorCallback< AsyncFileNonDurable_CloseFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AsyncFileNonDurable_CloseFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AsyncFileNonDurable_CloseFileActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
[[nodiscard]] Future<Void> AsyncFileNonDurable::closeFile( AsyncFileNonDurable* const& self ) {
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"
	return Future<Void>(new AsyncFileNonDurable_CloseFileActor(self));
															#line 2843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.g.cpp"
}

#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileNonDurable.actor.cpp"

void AsyncFileNonDurable::removeOpenFile(std::string filename, AsyncFileNonDurable* file) {
	auto& openFiles = g_simulator->getCurrentProcess()->machine->openFiles;

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
