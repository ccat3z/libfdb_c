#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
/*
 * HTTPServer.actor.cpp
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

#include "fdbrpc/HTTP.h"
#include "flow/IRandom.h"
#include "flow/Trace.h"
#include "fdbrpc/simulator.h"
#include "fdbrpc/SimulatorProcessInfo.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 30 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via callbackHandler()
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class CallbackHandlerActor>
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class CallbackHandlerActorState {
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	CallbackHandlerActorState(Reference<IConnection> const& conn,Future<Void> const& readRequestDone,Reference<HTTP::IRequestHandler> const& requestHandler,Reference<HTTP::IncomingRequest> const& req,FlowMutex* const& mutex) 
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : conn(conn),
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   readRequestDone(readRequestDone),
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   requestHandler(requestHandler),
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   req(req),
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   mutex(mutex),
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   response(makeReference<HTTP::OutgoingResponse>()),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   content()
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("callbackHandler", reinterpret_cast<unsigned long>(this));

	}
	~CallbackHandlerActorState() 
	{
		fdb_probe_actor_destroy("callbackHandler", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 35 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			response->data.content = &content;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			response->data.contentLen = 0;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			try {
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				StrictFuture<Void> __when_expr_0 = readRequestDone;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 1;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CallbackHandlerActor, 0, Void >*>(static_cast<CallbackHandlerActor*>(this)));
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~CallbackHandlerActorState();
		static_cast<CallbackHandlerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<FlowMutex::Lock> __when_expr_2 = mutex->take();
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 3;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >*>(static_cast<CallbackHandlerActor*>(this)));
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (e.code() == error_code_operation_cancelled)
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			}
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (e.code() == error_code_connection_failed)
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				TraceEvent(SevWarn, "HTTPServerConnHandlerFailed").error(e);
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				if (!static_cast<CallbackHandlerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CallbackHandlerActorState(); static_cast<CallbackHandlerActor*>(this)->destroy(); return 0; }
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
				new (&static_cast<CallbackHandlerActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~CallbackHandlerActorState();
				static_cast<CallbackHandlerActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (e.code() == error_code_http_request_failed || e.code() == error_code_http_bad_response)
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				TraceEvent(SevWarn, "HTTPServerConnHandlerInternalError").errorUnsuppressed(e);
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				response->reset();
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				response->code = 500;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			}
			else
			{
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				TraceEvent(SevWarn, "HTTPServerConnHandlerUnexpectedError").errorUnsuppressed(e);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			}
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
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = requestHandler->handleRequest(req, response);
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 2;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CallbackHandlerActor, 1, Void >*>(static_cast<CallbackHandlerActor*>(this)));
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = requestHandler->handleRequest(req, response);
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 2;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CallbackHandlerActor, 1, Void >*>(static_cast<CallbackHandlerActor*>(this)));
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state > 0) static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<CallbackHandlerActor*>(this)->ActorCallback< CallbackHandlerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CallbackHandlerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

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
	void a_exitChoose2() 
	{
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state > 0) static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<CallbackHandlerActor*>(this)->ActorCallback< CallbackHandlerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CallbackHandlerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 1);

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
	int a_body1cont5(int loopDepth) 
	{
		try {
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_3 = response->write(conn);
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state < 0) return a_body1cont5Catch1(actor_cancelled(), loopDepth);
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont5Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
			static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 4;
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CallbackHandlerActor, 3, Void >*>(static_cast<CallbackHandlerActor*>(this)));
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont5Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont5Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(FlowMutex::Lock const& __lock,int loopDepth) 
	{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		lock = __lock;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(FlowMutex::Lock && __lock,int loopDepth) 
	{
		lock = std::move(__lock);
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state > 0) static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<CallbackHandlerActor*>(this)->ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >::remove();

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >*,FlowMutex::Lock const& value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >*,FlowMutex::Lock && value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >*,Error err) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		lock.release();
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<CallbackHandlerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CallbackHandlerActorState(); static_cast<CallbackHandlerActor*>(this)->destroy(); return 0; }
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<CallbackHandlerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CallbackHandlerActorState();
		static_cast<CallbackHandlerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			lock.release();
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (e.code() == error_code_connection_failed)
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				TraceEvent("HTTPServerConnHandlerResponseError").errorUnsuppressed(e);
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				if (!static_cast<CallbackHandlerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CallbackHandlerActorState(); static_cast<CallbackHandlerActor*>(this)->destroy(); return 0; }
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
				new (&static_cast<CallbackHandlerActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~CallbackHandlerActorState();
				static_cast<CallbackHandlerActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			TraceEvent("HTTPServerConnHandlerResponseUnexpectedError").errorUnsuppressed(e);
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(loopDepth);

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
	void a_exitChoose4() 
	{
		if (static_cast<CallbackHandlerActor*>(this)->actor_wait_state > 0) static_cast<CallbackHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<CallbackHandlerActor*>(this)->ActorCallback< CallbackHandlerActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont5Catch1(error, 0);
		} catch (...) {
			a_body1cont5Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CallbackHandlerActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont5Catch1(error, 0);
		} catch (...) {
			a_body1cont5Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CallbackHandlerActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont5Catch1(error, 0);
		} catch (...) {
			a_body1cont5Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont8(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont6(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IConnection> conn;
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Future<Void> readRequestDone;
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IRequestHandler> requestHandler;
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IncomingRequest> req;
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowMutex* mutex;
															#line 33 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingResponse> response;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnsentPacketQueue content;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowMutex::Lock lock;
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via callbackHandler()
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class CallbackHandlerActor final : public Actor<Void>, public ActorCallback< CallbackHandlerActor, 0, Void >, public ActorCallback< CallbackHandlerActor, 1, Void >, public ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >, public ActorCallback< CallbackHandlerActor, 3, Void >, public FastAllocated<CallbackHandlerActor>, public CallbackHandlerActorState<CallbackHandlerActor> {
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<CallbackHandlerActor>::operator new;
	using FastAllocated<CallbackHandlerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CallbackHandlerActor, 0, Void >;
friend struct ActorCallback< CallbackHandlerActor, 1, Void >;
friend struct ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >;
friend struct ActorCallback< CallbackHandlerActor, 3, Void >;
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	CallbackHandlerActor(Reference<IConnection> const& conn,Future<Void> const& readRequestDone,Reference<HTTP::IRequestHandler> const& requestHandler,Reference<HTTP::IncomingRequest> const& req,FlowMutex* const& mutex) 
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   CallbackHandlerActorState<CallbackHandlerActor>(conn, readRequestDone, requestHandler, req, mutex)
	{
		fdb_probe_actor_enter("callbackHandler", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("callbackHandler");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("callbackHandler", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CallbackHandlerActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CallbackHandlerActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CallbackHandlerActor, 2, FlowMutex::Lock >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CallbackHandlerActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Void> callbackHandler( Reference<IConnection> const& conn, Future<Void> const& readRequestDone, Reference<HTTP::IRequestHandler> const& requestHandler, Reference<HTTP::IncomingRequest> const& req, FlowMutex* const& mutex ) {
															#line 28 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new CallbackHandlerActor(conn, readRequestDone, requestHandler, req, mutex));
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionHandler()
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class ConnectionHandlerActor>
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class ConnectionHandlerActorState {
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	ConnectionHandlerActorState(Reference<HTTP::SimServerContext> const& server,Reference<IConnection> const& conn,Reference<HTTP::IRequestHandler> const& requestHandler) 
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : server(server),
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   conn(conn),
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   requestHandler(requestHandler)
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("connectionHandler", reinterpret_cast<unsigned long>(this));

	}
	~ConnectionHandlerActorState() 
	{
		fdb_probe_actor_destroy("connectionHandler", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				responseMutex = FlowMutex();
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				readPrevRequest = Future<Void>(Void());
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				StrictFuture<Void> __when_expr_0 = conn->acceptHandshake();
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 1;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionHandlerActor, 0, Void >*>(static_cast<ConnectionHandlerActor*>(this)));
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~ConnectionHandlerActorState();
		static_cast<ConnectionHandlerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<ConnectionHandlerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionHandlerActorState(); static_cast<ConnectionHandlerActor*>(this)->destroy(); return 0; }
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<ConnectionHandlerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionHandlerActorState();
		static_cast<ConnectionHandlerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (e.code() != error_code_actor_cancelled)
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				TraceEvent("HTTPConnectionError", server->dbgid) .errorUnsuppressed(e) .suppressFor(1.0) .detail("ConnID", conn->getDebugID()) .detail("FromAddress", conn->getPeerAddress());
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			}
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			conn->close();
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		;
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = a_body1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = a_body1cont2loopHead1(loopDepth);

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
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state > 0) static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionHandlerActor*>(this)->ActorCallback< ConnectionHandlerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectionHandlerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1(int loopDepth) 
	{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = readPrevRequest;
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 2;
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionHandlerActor, 1, Void >*>(static_cast<ConnectionHandlerActor*>(this)));
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(0);
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 3;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionHandlerActor, 2, Void >*>(static_cast<ConnectionHandlerActor*>(this)));
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delay(0);
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 3;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionHandlerActor, 2, Void >*>(static_cast<ConnectionHandlerActor*>(this)));
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state > 0) static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionHandlerActor*>(this)->ActorCallback< ConnectionHandlerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectionHandlerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont2loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_3 = conn->onReadable();
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 4;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionHandlerActor, 3, Void >*>(static_cast<ConnectionHandlerActor*>(this)));
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_3 = conn->onReadable();
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 4;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionHandlerActor, 3, Void >*>(static_cast<ConnectionHandlerActor*>(this)));
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state > 0) static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionHandlerActor*>(this)->ActorCallback< ConnectionHandlerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectionHandlerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont2loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		req = makeReference<HTTP::IncomingRequest>();
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		readPrevRequest = req->read(conn, false);
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		server->actors.add(callbackHandler(conn, readPrevRequest, requestHandler, req, &responseMutex));
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		req = makeReference<HTTP::IncomingRequest>();
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		readPrevRequest = req->read(conn, false);
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		server->actors.add(callbackHandler(conn, readPrevRequest, requestHandler, req, &responseMutex));
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ConnectionHandlerActor*>(this)->actor_wait_state > 0) static_cast<ConnectionHandlerActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionHandlerActor*>(this)->ActorCallback< ConnectionHandlerActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionHandlerActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ConnectionHandlerActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::SimServerContext> server;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IConnection> conn;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IRequestHandler> requestHandler;
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowMutex responseMutex;
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Future<Void> readPrevRequest;
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IncomingRequest> req;
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via connectionHandler()
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class ConnectionHandlerActor final : public Actor<Void>, public ActorCallback< ConnectionHandlerActor, 0, Void >, public ActorCallback< ConnectionHandlerActor, 1, Void >, public ActorCallback< ConnectionHandlerActor, 2, Void >, public ActorCallback< ConnectionHandlerActor, 3, Void >, public FastAllocated<ConnectionHandlerActor>, public ConnectionHandlerActorState<ConnectionHandlerActor> {
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<ConnectionHandlerActor>::operator new;
	using FastAllocated<ConnectionHandlerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectionHandlerActor, 0, Void >;
friend struct ActorCallback< ConnectionHandlerActor, 1, Void >;
friend struct ActorCallback< ConnectionHandlerActor, 2, Void >;
friend struct ActorCallback< ConnectionHandlerActor, 3, Void >;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	ConnectionHandlerActor(Reference<HTTP::SimServerContext> const& server,Reference<IConnection> const& conn,Reference<HTTP::IRequestHandler> const& requestHandler) 
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectionHandlerActorState<ConnectionHandlerActor>(server, conn, requestHandler)
	{
		fdb_probe_actor_enter("connectionHandler", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectionHandler");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectionHandler", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectionHandlerActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectionHandlerActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ConnectionHandlerActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ConnectionHandlerActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Void> connectionHandler( Reference<HTTP::SimServerContext> const& server, Reference<IConnection> const& conn, Reference<HTTP::IRequestHandler> const& requestHandler ) {
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new ConnectionHandlerActor(server, conn, requestHandler));
															#line 1210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via listenActor()
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class ListenActorActor>
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class ListenActorActorState {
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	ListenActorActorState(Reference<HTTP::SimServerContext> const& server,Reference<HTTP::IRequestHandler> const& requestHandler,NetworkAddress const& addr,Reference<IListener> const& listener) 
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : server(server),
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   requestHandler(requestHandler),
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   addr(addr),
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   listener(listener)
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("listenActor", reinterpret_cast<unsigned long>(this));

	}
	~ListenActorActorState() 
	{
		fdb_probe_actor_destroy("listenActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			TraceEvent(SevDebug, "HTTPServerListenStart", server->dbgid).detail("ListenAddress", addr.toString());
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = requestHandler->init();
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<ListenActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListenActorActor*>(this)->actor_wait_state = 1;
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListenActorActor, 0, Void >*>(static_cast<ListenActorActor*>(this)));
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~ListenActorActorState();
		static_cast<ListenActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		TraceEvent(SevDebug, "HTTPServerListenInitialized", server->dbgid).detail("ListenAddress", addr.toString());
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		try {
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			;
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			loopDepth = a_body1cont1loopHead1(loopDepth);
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
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		TraceEvent(SevDebug, "HTTPServerListenInitialized", server->dbgid).detail("ListenAddress", addr.toString());
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		try {
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			;
															#line 1305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			loopDepth = a_body1cont1loopHead1(loopDepth);
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
		if (static_cast<ListenActorActor*>(this)->actor_wait_state > 0) static_cast<ListenActorActor*>(this)->actor_wait_state = 0;
		static_cast<ListenActorActor*>(this)->ActorCallback< ListenActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ListenActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListenActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListenActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<ListenActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ListenActorActorState(); static_cast<ListenActorActor*>(this)->destroy(); return 0; }
															#line 1383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<ListenActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ListenActorActorState();
		static_cast<ListenActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			TraceEvent(SevError, "HTTPListenError", server->dbgid).error(e);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_1 = listener->accept();
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<ListenActorActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ListenActorActor*>(this)->actor_wait_state = 2;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListenActorActor, 1, Reference<IConnection> >*>(static_cast<ListenActorActor*>(this)));
															#line 1432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Reference<IConnection> const& conn,int loopDepth) 
	{
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!server->running)
															#line 1454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			TraceEvent("HTTPServerExitedAfterAccept", server->dbgid);
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (conn)
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			server->actors.add(connectionHandler(server, conn, requestHandler));
															#line 1467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Reference<IConnection> && conn,int loopDepth) 
	{
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!server->running)
															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			TraceEvent("HTTPServerExitedAfterAccept", server->dbgid);
															#line 1481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (conn)
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			server->actors.add(connectionHandler(server, conn, requestHandler));
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Reference<IConnection> const& conn,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(conn, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Reference<IConnection> && conn,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(conn), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ListenActorActor*>(this)->actor_wait_state > 0) static_cast<ListenActorActor*>(this)->actor_wait_state = 0;
		static_cast<ListenActorActor*>(this)->ActorCallback< ListenActorActor, 1, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< ListenActorActor, 1, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ListenActorActor, 1, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ListenActorActor, 1, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
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
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::SimServerContext> server;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IRequestHandler> requestHandler;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	NetworkAddress addr;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IListener> listener;
															#line 1580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via listenActor()
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class ListenActorActor final : public Actor<Void>, public ActorCallback< ListenActorActor, 0, Void >, public ActorCallback< ListenActorActor, 1, Reference<IConnection> >, public FastAllocated<ListenActorActor>, public ListenActorActorState<ListenActorActor> {
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<ListenActorActor>::operator new;
	using FastAllocated<ListenActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListenActorActor, 0, Void >;
friend struct ActorCallback< ListenActorActor, 1, Reference<IConnection> >;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	ListenActorActor(Reference<HTTP::SimServerContext> const& server,Reference<HTTP::IRequestHandler> const& requestHandler,NetworkAddress const& addr,Reference<IListener> const& listener) 
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   ListenActorActorState<ListenActorActor>(server, requestHandler, addr, listener)
	{
		fdb_probe_actor_enter("listenActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listenActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listenActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListenActorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ListenActorActor, 1, Reference<IConnection> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Void> listenActor( Reference<HTTP::SimServerContext> const& server, Reference<HTTP::IRequestHandler> const& requestHandler, NetworkAddress const& addr, Reference<IListener> const& listener ) {
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new ListenActorActor(server, requestHandler, addr, listener));
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

NetworkAddress HTTP::SimServerContext::newAddress() {
	// allocate new addr, assert we have enough addr space
	ASSERT(listenAddresses.size() < 1000);
	return NetworkAddress(
	    g_simulator->getCurrentProcess()->address.ip, nextPort++, true /* isPublic*/, false /*isTLS*/);
}

void HTTP::SimServerContext::registerNewServer(NetworkAddress addr, Reference<HTTP::IRequestHandler> requestHandler) {
	listenAddresses.push_back(addr);
	listeners.push_back(INetworkConnections::net()->listen(addr));
	actors.add(listenActor(Reference<HTTP::SimServerContext>::addRef(this), requestHandler, addr, listeners.back()));
}

void HTTP::SimRegisteredHandlerContext::updateDNS() {
	// if addresses is empty, that violates the assumption that there is at least one address when doing resolution.
	// Only update dns if we have at least one address
	if (!addresses.empty()) {
		INetworkConnections::net()->addMockTCPEndpoint(hostname, service, addresses);
	}
}

void HTTP::SimRegisteredHandlerContext::addAddress(NetworkAddress addr) {
	addresses.push_back(addr);
	fmt::print("HTTP: adding address {0} for {1}:{2}\n", addr.toString(), hostname, service);
	updateDNS();
}

void HTTP::SimRegisteredHandlerContext::removeIp(IPAddress ip) {
	fmt::print("HTTP: removing ip {0} for {1}:{2}\n", ip.toString(), hostname, service);
	for (int i = 0; i < addresses.size(); i++) {
		if (addresses[i].ip == ip) {
			swapAndPop(&addresses, i);
			i--;
		}
	}
	updateDNS();
}

// unit test stuff

															#line 1671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via helloWorldServerCallback()
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class HelloWorldServerCallbackActor>
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class HelloWorldServerCallbackActorState {
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	HelloWorldServerCallbackActorState(Reference<HTTP::IncomingRequest> const& req,Reference<HTTP::OutgoingResponse> const& response) 
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : req(req),
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   response(response)
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("helloWorldServerCallback", reinterpret_cast<unsigned long>(this));

	}
	~HelloWorldServerCallbackActorState() 
	{
		fdb_probe_actor_destroy("helloWorldServerCallback", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<HelloWorldServerCallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<HelloWorldServerCallbackActor*>(this)->actor_wait_state = 1;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< HelloWorldServerCallbackActor, 0, Void >*>(static_cast<HelloWorldServerCallbackActor*>(this)));
															#line 1709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~HelloWorldServerCallbackActorState();
		static_cast<HelloWorldServerCallbackActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->verb, HTTP::HTTP_VERB_POST);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->resource, "/hello-world");
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers.size(), 2);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(req->data.headers.count("Hello"));
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers["Hello"], "World");
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(req->data.headers.count("Content-Length"));
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers["Content-Length"], std::to_string(req->data.content.size()));
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.contentLen, req->data.content.size());
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.content, "Hello World Request!");
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->code = 200;
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Hello"] = "World";
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		std::string hello = "Hello World Response!";
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Content-MD5"] = HTTP::computeMD5Sum(hello);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		PacketWriter pw(response->data.content->getWriteBuffer(hello.size()), nullptr, Unversioned());
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		pw.serializeBytes(hello);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.contentLen = hello.size();
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<HelloWorldServerCallbackActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~HelloWorldServerCallbackActorState(); static_cast<HelloWorldServerCallbackActor*>(this)->destroy(); return 0; }
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<HelloWorldServerCallbackActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~HelloWorldServerCallbackActorState();
		static_cast<HelloWorldServerCallbackActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->verb, HTTP::HTTP_VERB_POST);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->resource, "/hello-world");
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers.size(), 2);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(req->data.headers.count("Hello"));
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers["Hello"], "World");
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(req->data.headers.count("Content-Length"));
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers["Content-Length"], std::to_string(req->data.content.size()));
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.contentLen, req->data.content.size());
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.content, "Hello World Request!");
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->code = 200;
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Hello"] = "World";
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		std::string hello = "Hello World Response!";
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Content-MD5"] = HTTP::computeMD5Sum(hello);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		PacketWriter pw(response->data.content->getWriteBuffer(hello.size()), nullptr, Unversioned());
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		pw.serializeBytes(hello);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.contentLen = hello.size();
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<HelloWorldServerCallbackActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~HelloWorldServerCallbackActorState(); static_cast<HelloWorldServerCallbackActor*>(this)->destroy(); return 0; }
															#line 1808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<HelloWorldServerCallbackActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~HelloWorldServerCallbackActorState();
		static_cast<HelloWorldServerCallbackActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<HelloWorldServerCallbackActor*>(this)->actor_wait_state > 0) static_cast<HelloWorldServerCallbackActor*>(this)->actor_wait_state = 0;
		static_cast<HelloWorldServerCallbackActor*>(this)->ActorCallback< HelloWorldServerCallbackActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< HelloWorldServerCallbackActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< HelloWorldServerCallbackActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< HelloWorldServerCallbackActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IncomingRequest> req;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingResponse> response;
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via helloWorldServerCallback()
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class HelloWorldServerCallbackActor final : public Actor<Void>, public ActorCallback< HelloWorldServerCallbackActor, 0, Void >, public FastAllocated<HelloWorldServerCallbackActor>, public HelloWorldServerCallbackActorState<HelloWorldServerCallbackActor> {
															#line 1888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<HelloWorldServerCallbackActor>::operator new;
	using FastAllocated<HelloWorldServerCallbackActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< HelloWorldServerCallbackActor, 0, Void >;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	HelloWorldServerCallbackActor(Reference<HTTP::IncomingRequest> const& req,Reference<HTTP::OutgoingResponse> const& response) 
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   HelloWorldServerCallbackActorState<HelloWorldServerCallbackActor>(req, response)
	{
		fdb_probe_actor_enter("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("helloWorldServerCallback");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("helloWorldServerCallback", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< HelloWorldServerCallbackActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Void> helloWorldServerCallback( Reference<HTTP::IncomingRequest> const& req, Reference<HTTP::OutgoingResponse> const& response ) {
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new HelloWorldServerCallbackActor(req, response));
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

struct HelloWorldRequestHandler : HTTP::IRequestHandler, ReferenceCounted<HelloWorldRequestHandler> {
	Future<Void> handleRequest(Reference<HTTP::IncomingRequest> req,
	                           Reference<HTTP::OutgoingResponse> response) override {
		return helloWorldServerCallback(req, response);
	}
	Reference<HTTP::IRequestHandler> clone() override { return makeReference<HelloWorldRequestHandler>(); }

	void addref() override { ReferenceCounted<HelloWorldRequestHandler>::addref(); }
	void delref() override { ReferenceCounted<HelloWorldRequestHandler>::delref(); }
};

															#line 1943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via helloErrorServerCallback()
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class HelloErrorServerCallbackActor>
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class HelloErrorServerCallbackActorState {
															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	HelloErrorServerCallbackActorState(Reference<HTTP::IncomingRequest> const& req,Reference<HTTP::OutgoingResponse> const& response) 
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : req(req),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   response(response)
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("helloErrorServerCallback", reinterpret_cast<unsigned long>(this));

	}
	~HelloErrorServerCallbackActorState() 
	{
		fdb_probe_actor_destroy("helloErrorServerCallback", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<HelloErrorServerCallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<HelloErrorServerCallbackActor*>(this)->actor_wait_state = 1;
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< HelloErrorServerCallbackActor, 0, Void >*>(static_cast<HelloErrorServerCallbackActor*>(this)));
															#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~HelloErrorServerCallbackActorState();
		static_cast<HelloErrorServerCallbackActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (deterministicRandom()->coinflip())
															#line 2004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		}
		else
		{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			return a_body1Catch1(http_request_failed(), loopDepth);
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (deterministicRandom()->coinflip())
															#line 2023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		{
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		}
		else
		{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			return a_body1Catch1(http_request_failed(), loopDepth);
															#line 2033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		if (static_cast<HelloErrorServerCallbackActor*>(this)->actor_wait_state > 0) static_cast<HelloErrorServerCallbackActor*>(this)->actor_wait_state = 0;
		static_cast<HelloErrorServerCallbackActor*>(this)->ActorCallback< HelloErrorServerCallbackActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< HelloErrorServerCallbackActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< HelloErrorServerCallbackActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< HelloErrorServerCallbackActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IncomingRequest> req;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingResponse> response;
															#line 2105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via helloErrorServerCallback()
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class HelloErrorServerCallbackActor final : public Actor<Void>, public ActorCallback< HelloErrorServerCallbackActor, 0, Void >, public FastAllocated<HelloErrorServerCallbackActor>, public HelloErrorServerCallbackActorState<HelloErrorServerCallbackActor> {
															#line 2110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<HelloErrorServerCallbackActor>::operator new;
	using FastAllocated<HelloErrorServerCallbackActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< HelloErrorServerCallbackActor, 0, Void >;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	HelloErrorServerCallbackActor(Reference<HTTP::IncomingRequest> const& req,Reference<HTTP::OutgoingResponse> const& response) 
															#line 2121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   HelloErrorServerCallbackActorState<HelloErrorServerCallbackActor>(req, response)
	{
		fdb_probe_actor_enter("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("helloErrorServerCallback");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("helloErrorServerCallback", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< HelloErrorServerCallbackActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Void> helloErrorServerCallback( Reference<HTTP::IncomingRequest> const& req, Reference<HTTP::OutgoingResponse> const& response ) {
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new HelloErrorServerCallbackActor(req, response));
															#line 2149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

struct HelloErrorRequestHandler : HTTP::IRequestHandler, ReferenceCounted<HelloErrorRequestHandler> {
	Future<Void> handleRequest(Reference<HTTP::IncomingRequest> req,
	                           Reference<HTTP::OutgoingResponse> response) override {
		return helloErrorServerCallback(req, response);
	}
	Reference<HTTP::IRequestHandler> clone() override { return makeReference<HelloErrorRequestHandler>(); }

	void addref() override { ReferenceCounted<HelloErrorRequestHandler>::addref(); }
	void delref() override { ReferenceCounted<HelloErrorRequestHandler>::delref(); }
};

															#line 2165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via helloBadMD5ServerCallback()
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class HelloBadMD5ServerCallbackActor>
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class HelloBadMD5ServerCallbackActorState {
															#line 2172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	HelloBadMD5ServerCallbackActorState(Reference<HTTP::IncomingRequest> const& req,Reference<HTTP::OutgoingResponse> const& response) 
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : req(req),
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   response(response)
															#line 2181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this));

	}
	~HelloBadMD5ServerCallbackActorState() 
	{
		fdb_probe_actor_destroy("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0);
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<HelloBadMD5ServerCallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<HelloBadMD5ServerCallbackActor*>(this)->actor_wait_state = 1;
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >*>(static_cast<HelloBadMD5ServerCallbackActor*>(this)));
															#line 2203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~HelloBadMD5ServerCallbackActorState();
		static_cast<HelloBadMD5ServerCallbackActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->verb, HTTP::HTTP_VERB_GET);
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->resource, "/hello-world");
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers.size(), 1);
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(req->data.headers.count("Content-Length"));
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers["Content-Length"], std::to_string(req->data.content.size()));
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.contentLen, req->data.content.size());
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.content, "Hello Bad MD5 Request!");
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->code = 200;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Hello"] = "World";
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		std::string hello = "Hello World Response!";
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Content-MD5"] = HTTP::computeMD5Sum(hello);
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		hello = "Hello Bad MD5 Response";
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		PacketWriter pw(response->data.content->getWriteBuffer(hello.size()), nullptr, Unversioned());
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		pw.serializeBytes(hello);
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.contentLen = hello.size();
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<HelloBadMD5ServerCallbackActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~HelloBadMD5ServerCallbackActorState(); static_cast<HelloBadMD5ServerCallbackActor*>(this)->destroy(); return 0; }
															#line 2256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<HelloBadMD5ServerCallbackActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~HelloBadMD5ServerCallbackActorState();
		static_cast<HelloBadMD5ServerCallbackActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->verb, HTTP::HTTP_VERB_GET);
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->resource, "/hello-world");
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers.size(), 1);
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(req->data.headers.count("Content-Length"));
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.headers["Content-Length"], std::to_string(req->data.content.size()));
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.contentLen, req->data.content.size());
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(req->data.content, "Hello Bad MD5 Request!");
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->code = 200;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Hello"] = "World";
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		std::string hello = "Hello World Response!";
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.headers["Content-MD5"] = HTTP::computeMD5Sum(hello);
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		hello = "Hello Bad MD5 Response";
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		PacketWriter pw(response->data.content->getWriteBuffer(hello.size()), nullptr, Unversioned());
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		pw.serializeBytes(hello);
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		response->data.contentLen = hello.size();
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<HelloBadMD5ServerCallbackActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~HelloBadMD5ServerCallbackActorState(); static_cast<HelloBadMD5ServerCallbackActor*>(this)->destroy(); return 0; }
															#line 2298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<HelloBadMD5ServerCallbackActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~HelloBadMD5ServerCallbackActorState();
		static_cast<HelloBadMD5ServerCallbackActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<HelloBadMD5ServerCallbackActor*>(this)->actor_wait_state > 0) static_cast<HelloBadMD5ServerCallbackActor*>(this)->actor_wait_state = 0;
		static_cast<HelloBadMD5ServerCallbackActor*>(this)->ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::IncomingRequest> req;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingResponse> response;
															#line 2373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via helloBadMD5ServerCallback()
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class HelloBadMD5ServerCallbackActor final : public Actor<Void>, public ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >, public FastAllocated<HelloBadMD5ServerCallbackActor>, public HelloBadMD5ServerCallbackActorState<HelloBadMD5ServerCallbackActor> {
															#line 2378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<HelloBadMD5ServerCallbackActor>::operator new;
	using FastAllocated<HelloBadMD5ServerCallbackActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	HelloBadMD5ServerCallbackActor(Reference<HTTP::IncomingRequest> const& req,Reference<HTTP::OutgoingResponse> const& response) 
															#line 2389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   HelloBadMD5ServerCallbackActorState<HelloBadMD5ServerCallbackActor>(req, response)
	{
		fdb_probe_actor_enter("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("helloBadMD5ServerCallback");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("helloBadMD5ServerCallback", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< HelloBadMD5ServerCallbackActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Void> helloBadMD5ServerCallback( Reference<HTTP::IncomingRequest> const& req, Reference<HTTP::OutgoingResponse> const& response ) {
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new HelloBadMD5ServerCallbackActor(req, response));
															#line 2417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

struct HelloBadMD5RequestHandler : HTTP::IRequestHandler, ReferenceCounted<HelloBadMD5RequestHandler> {
	Future<Void> handleRequest(Reference<HTTP::IncomingRequest> req,
	                           Reference<HTTP::OutgoingResponse> response) override {
		return helloBadMD5ServerCallback(req, response);
	}
	Reference<HTTP::IRequestHandler> clone() override { return makeReference<HelloBadMD5RequestHandler>(); }

	void addref() override { ReferenceCounted<HelloBadMD5RequestHandler>::addref(); }
	void delref() override { ReferenceCounted<HelloBadMD5RequestHandler>::delref(); }
};

typedef std::function<Future<Reference<HTTP::IncomingResponse>>(Reference<IConnection> conn)> DoRequestFunction;

// handles retrying on timeout and reinitializing connection like other users of HTTP (S3BlobStore, RestClient)
															#line 2436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via doRequestTest()
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class DoRequestTestActor>
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoRequestTestActorState {
															#line 2443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoRequestTestActorState(std::string const& hostname,std::string const& service,DoRequestFunction const& reqFunction) 
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : hostname(hostname),
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   service(service),
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   reqFunction(reqFunction),
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   conn()
															#line 2456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("doRequestTest", reinterpret_cast<unsigned long>(this));

	}
	~DoRequestTestActorState() 
	{
		fdb_probe_actor_destroy("doRequestTest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			;
															#line 2471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~DoRequestTestActorState();
		static_cast<DoRequestTestActor*>(this)->sendErrorAndDelPromiseRef(error);
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
		try {
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (!conn)
															#line 2502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				StrictFuture<Void> __when_expr_0 = store(conn, INetworkConnections::net()->connect(hostname, service, false));
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				if (static_cast<DoRequestTestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoRequestTestActor*>(this)->actor_wait_state = 1;
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRequestTestActor, 0, Void >*>(static_cast<DoRequestTestActor*>(this)));
															#line 2513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
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
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (conn)
															#line 2540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				conn->close();
															#line 2544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			}
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (e.code() != error_code_timed_out && e.code() != error_code_connection_failed && e.code() != error_code_lookup_failed)
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			{
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 2552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			}
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			conn.clear();
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(0.1);
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<DoRequestTestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<DoRequestTestActor*>(this)->actor_wait_state = 4;
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoRequestTestActor, 3, Void >*>(static_cast<DoRequestTestActor*>(this)));
															#line 2565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		Future<Reference<HTTP::IncomingResponse>> f = reqFunction(conn);
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Reference<HTTP::IncomingResponse>> __when_expr_2 = f;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestTestActor*>(this)->actor_wait_state = 3;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >*>(static_cast<DoRequestTestActor*>(this)));
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(conn.isValid());
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->connectHandshake();
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestTestActor*>(this)->actor_wait_state = 2;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestTestActor, 1, Void >*>(static_cast<DoRequestTestActor*>(this)));
															#line 2607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(conn.isValid());
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->connectHandshake();
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestTestActor*>(this)->actor_wait_state = 2;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestTestActor, 1, Void >*>(static_cast<DoRequestTestActor*>(this)));
															#line 2625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestTestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestTestActor*>(this)->ActorCallback< DoRequestTestActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoRequestTestActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestTestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestTestActor*>(this)->ActorCallback< DoRequestTestActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoRequestTestActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		conn->close();
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoRequestTestActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoRequestTestActorState(); static_cast<DoRequestTestActor*>(this)->destroy(); return 0; }
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoRequestTestActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoRequestTestActorState();
		static_cast<DoRequestTestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		conn->close();
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoRequestTestActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoRequestTestActorState(); static_cast<DoRequestTestActor*>(this)->destroy(); return 0; }
															#line 2788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoRequestTestActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoRequestTestActorState();
		static_cast<DoRequestTestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(response, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(response), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestTestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestTestActor*>(this)->ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> const& value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> && value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DoRequestTestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestTestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestTestActor*>(this)->ActorCallback< DoRequestTestActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DoRequestTestActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DoRequestTestActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	std::string hostname;
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	std::string service;
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoRequestFunction reqFunction;
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IConnection> conn;
															#line 2942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via doRequestTest()
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoRequestTestActor final : public Actor<Reference<HTTP::IncomingResponse>>, public ActorCallback< DoRequestTestActor, 0, Void >, public ActorCallback< DoRequestTestActor, 1, Void >, public ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >, public ActorCallback< DoRequestTestActor, 3, Void >, public FastAllocated<DoRequestTestActor>, public DoRequestTestActorState<DoRequestTestActor> {
															#line 2947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<DoRequestTestActor>::operator new;
	using FastAllocated<DoRequestTestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::IncomingResponse>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoRequestTestActor, 0, Void >;
friend struct ActorCallback< DoRequestTestActor, 1, Void >;
friend struct ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >;
friend struct ActorCallback< DoRequestTestActor, 3, Void >;
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoRequestTestActor(std::string const& hostname,std::string const& service,DoRequestFunction const& reqFunction) 
															#line 2961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Reference<HTTP::IncomingResponse>>(),
		   DoRequestTestActorState<DoRequestTestActor>(hostname, service, reqFunction)
	{
		fdb_probe_actor_enter("doRequestTest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doRequestTest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doRequestTest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoRequestTestActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoRequestTestActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DoRequestTestActor, 2, Reference<HTTP::IncomingResponse> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DoRequestTestActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::IncomingResponse>> doRequestTest( std::string const& hostname, std::string const& service, DoRequestFunction const& reqFunction ) {
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Reference<HTTP::IncomingResponse>>(new DoRequestTestActor(hostname, service, reqFunction));
															#line 2992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 2997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via doHelloWorldReq()
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class DoHelloWorldReqActor>
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoHelloWorldReqActorState {
															#line 3004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoHelloWorldReqActorState(Reference<IConnection> const& conn) 
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : conn(conn),
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   content(),
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   req(makeReference<HTTP::OutgoingRequest>()),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   sendReceiveRate(makeReference<Unlimited>()),
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   bytes_sent(0)
															#line 3019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("doHelloWorldReq", reinterpret_cast<unsigned long>(this));

	}
	~DoHelloWorldReqActorState() 
	{
		fdb_probe_actor_destroy("doHelloWorldReq", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->verb = HTTP::HTTP_VERB_POST;
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->resource = "/hello-world";
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.headers["Hello"] = "World";
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			std::string hello = "Hello World Request!";
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.content = &content;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.contentLen = hello.size();
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			PacketWriter pw(req->data.content->getWriteBuffer(hello.size()), nullptr, Unversioned());
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			pw.serializeBytes(hello);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Reference<HTTP::IncomingResponse>> __when_expr_0 = timeoutError(HTTP::doRequest(conn, req, sendReceiveRate, &bytes_sent, sendReceiveRate), 30.0);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<DoHelloWorldReqActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoHelloWorldReqActor*>(this)->actor_wait_state = 1;
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >*>(static_cast<DoHelloWorldReqActor*>(this)));
															#line 3057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~DoHelloWorldReqActorState();
		static_cast<DoHelloWorldReqActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		std::string expectedContent = "Hello World Response!";
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->code, 200);
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers.size(), 3);
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->data.headers.count("Hello"));
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers["Hello"], "World");
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->data.headers.count("Content-Length"));
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers["Content-Length"], std::to_string(response->data.content.size()));
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->data.headers.count("Content-MD5"));
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers["Content-MD5"], HTTP::computeMD5Sum(expectedContent));
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.contentLen, response->data.content.size());
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.content, expectedContent);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoHelloWorldReqActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoHelloWorldReqActorState(); static_cast<DoHelloWorldReqActor*>(this)->destroy(); return 0; }
															#line 3102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoHelloWorldReqActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoHelloWorldReqActorState();
		static_cast<DoHelloWorldReqActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		std::string expectedContent = "Hello World Response!";
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->code, 200);
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers.size(), 3);
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->data.headers.count("Hello"));
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers["Hello"], "World");
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->data.headers.count("Content-Length"));
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers["Content-Length"], std::to_string(response->data.content.size()));
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->data.headers.count("Content-MD5"));
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.headers["Content-MD5"], HTTP::computeMD5Sum(expectedContent));
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.contentLen, response->data.content.size());
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT_EQ(response->data.content, expectedContent);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoHelloWorldReqActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoHelloWorldReqActorState(); static_cast<DoHelloWorldReqActor*>(this)->destroy(); return 0; }
															#line 3136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoHelloWorldReqActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoHelloWorldReqActorState();
		static_cast<DoHelloWorldReqActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
		loopDepth = a_body1cont1(response, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(response), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoHelloWorldReqActor*>(this)->actor_wait_state > 0) static_cast<DoHelloWorldReqActor*>(this)->actor_wait_state = 0;
		static_cast<DoHelloWorldReqActor*>(this)->ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >::remove();

	}
	void a_callback_fire(ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> const& value) 
	{
		fdb_probe_actor_enter("doHelloWorldReq", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloWorldReq", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> && value) 
	{
		fdb_probe_actor_enter("doHelloWorldReq", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloWorldReq", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >*,Error err) 
	{
		fdb_probe_actor_enter("doHelloWorldReq", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloWorldReq", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IConnection> conn;
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnsentPacketQueue content;
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingRequest> req;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	int64_t bytes_sent;
															#line 3217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via doHelloWorldReq()
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoHelloWorldReqActor final : public Actor<Reference<HTTP::IncomingResponse>>, public ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >, public FastAllocated<DoHelloWorldReqActor>, public DoHelloWorldReqActorState<DoHelloWorldReqActor> {
															#line 3222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<DoHelloWorldReqActor>::operator new;
	using FastAllocated<DoHelloWorldReqActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::IncomingResponse>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoHelloWorldReqActor(Reference<IConnection> const& conn) 
															#line 3233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Reference<HTTP::IncomingResponse>>(),
		   DoHelloWorldReqActorState<DoHelloWorldReqActor>(conn)
	{
		fdb_probe_actor_enter("doHelloWorldReq", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doHelloWorldReq");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doHelloWorldReq", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoHelloWorldReqActor, 0, Reference<HTTP::IncomingResponse> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::IncomingResponse>> doHelloWorldReq( Reference<IConnection> const& conn ) {
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Reference<HTTP::IncomingResponse>>(new DoHelloWorldReqActor(conn));
															#line 3261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 3266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via doHelloWorldErrorReq()
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class DoHelloWorldErrorReqActor>
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoHelloWorldErrorReqActorState {
															#line 3273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoHelloWorldErrorReqActorState(Reference<IConnection> const& conn) 
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : conn(conn),
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   content(),
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   req(makeReference<HTTP::OutgoingRequest>()),
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   sendReceiveRate(makeReference<Unlimited>()),
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   bytes_sent(0)
															#line 3288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this));

	}
	~DoHelloWorldErrorReqActorState() 
	{
		fdb_probe_actor_destroy("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->verb = HTTP::HTTP_VERB_GET;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->resource = "/hello-error";
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.content = &content;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.contentLen = 0;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Reference<HTTP::IncomingResponse>> __when_expr_0 = timeoutError(HTTP::doRequest(conn, req, sendReceiveRate, &bytes_sent, sendReceiveRate), 30.0);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<DoHelloWorldErrorReqActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoHelloWorldErrorReqActor*>(this)->actor_wait_state = 1;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >*>(static_cast<DoHelloWorldErrorReqActor*>(this)));
															#line 3318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~DoHelloWorldErrorReqActorState();
		static_cast<DoHelloWorldErrorReqActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->code == 500);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoHelloWorldErrorReqActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoHelloWorldErrorReqActorState(); static_cast<DoHelloWorldErrorReqActor*>(this)->destroy(); return 0; }
															#line 3343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoHelloWorldErrorReqActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoHelloWorldErrorReqActorState();
		static_cast<DoHelloWorldErrorReqActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(response->code == 500);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoHelloWorldErrorReqActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoHelloWorldErrorReqActorState(); static_cast<DoHelloWorldErrorReqActor*>(this)->destroy(); return 0; }
															#line 3357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoHelloWorldErrorReqActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoHelloWorldErrorReqActorState();
		static_cast<DoHelloWorldErrorReqActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
		loopDepth = a_body1cont1(response, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(response), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoHelloWorldErrorReqActor*>(this)->actor_wait_state > 0) static_cast<DoHelloWorldErrorReqActor*>(this)->actor_wait_state = 0;
		static_cast<DoHelloWorldErrorReqActor*>(this)->ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >::remove();

	}
	void a_callback_fire(ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> const& value) 
	{
		fdb_probe_actor_enter("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> && value) 
	{
		fdb_probe_actor_enter("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >*,Error err) 
	{
		fdb_probe_actor_enter("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IConnection> conn;
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnsentPacketQueue content;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingRequest> req;
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	int64_t bytes_sent;
															#line 3438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via doHelloWorldErrorReq()
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoHelloWorldErrorReqActor final : public Actor<Reference<HTTP::IncomingResponse>>, public ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >, public FastAllocated<DoHelloWorldErrorReqActor>, public DoHelloWorldErrorReqActorState<DoHelloWorldErrorReqActor> {
															#line 3443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<DoHelloWorldErrorReqActor>::operator new;
	using FastAllocated<DoHelloWorldErrorReqActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::IncomingResponse>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoHelloWorldErrorReqActor(Reference<IConnection> const& conn) 
															#line 3454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Reference<HTTP::IncomingResponse>>(),
		   DoHelloWorldErrorReqActorState<DoHelloWorldErrorReqActor>(conn)
	{
		fdb_probe_actor_enter("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doHelloWorldErrorReq");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doHelloWorldErrorReq", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoHelloWorldErrorReqActor, 0, Reference<HTTP::IncomingResponse> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::IncomingResponse>> doHelloWorldErrorReq( Reference<IConnection> const& conn ) {
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Reference<HTTP::IncomingResponse>>(new DoHelloWorldErrorReqActor(conn));
															#line 3482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 3487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via doHelloBadMD5Req()
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class DoHelloBadMD5ReqActor>
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoHelloBadMD5ReqActorState {
															#line 3494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoHelloBadMD5ReqActorState(Reference<IConnection> const& conn) 
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : conn(conn),
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   content(),
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   req(makeReference<HTTP::OutgoingRequest>()),
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   sendReceiveRate(makeReference<Unlimited>()),
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		   bytes_sent(0)
															#line 3509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this));

	}
	~DoHelloBadMD5ReqActorState() 
	{
		fdb_probe_actor_destroy("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->verb = HTTP::HTTP_VERB_GET;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->resource = "/hello-world";
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			std::string hello = "Hello Bad MD5 Request!";
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.content = &content;
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			req->data.contentLen = hello.size();
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			PacketWriter pw(req->data.content->getWriteBuffer(hello.size()), nullptr, Unversioned());
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			pw.serializeBytes(hello);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Reference<HTTP::IncomingResponse>> __when_expr_0 = timeoutError(HTTP::doRequest(conn, req, sendReceiveRate, &bytes_sent, sendReceiveRate), 30.0);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<DoHelloBadMD5ReqActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoHelloBadMD5ReqActor*>(this)->actor_wait_state = 1;
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >*>(static_cast<DoHelloBadMD5ReqActor*>(this)));
															#line 3545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~DoHelloBadMD5ReqActorState();
		static_cast<DoHelloBadMD5ReqActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(false);
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoHelloBadMD5ReqActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoHelloBadMD5ReqActorState(); static_cast<DoHelloBadMD5ReqActor*>(this)->destroy(); return 0; }
															#line 3570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoHelloBadMD5ReqActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoHelloBadMD5ReqActorState();
		static_cast<DoHelloBadMD5ReqActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(false);
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<DoHelloBadMD5ReqActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(response); this->~DoHelloBadMD5ReqActorState(); static_cast<DoHelloBadMD5ReqActor*>(this)->destroy(); return 0; }
															#line 3584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<DoHelloBadMD5ReqActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(response);
		this->~DoHelloBadMD5ReqActorState();
		static_cast<DoHelloBadMD5ReqActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<HTTP::IncomingResponse> const& response,int loopDepth) 
	{
		loopDepth = a_body1cont1(response, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<HTTP::IncomingResponse> && response,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(response), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoHelloBadMD5ReqActor*>(this)->actor_wait_state > 0) static_cast<DoHelloBadMD5ReqActor*>(this)->actor_wait_state = 0;
		static_cast<DoHelloBadMD5ReqActor*>(this)->ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >::remove();

	}
	void a_callback_fire(ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> const& value) 
	{
		fdb_probe_actor_enter("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> && value) 
	{
		fdb_probe_actor_enter("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >*,Error err) 
	{
		fdb_probe_actor_enter("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IConnection> conn;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnsentPacketQueue content;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<HTTP::OutgoingRequest> req;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	int64_t bytes_sent;
															#line 3665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via doHelloBadMD5Req()
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class DoHelloBadMD5ReqActor final : public Actor<Reference<HTTP::IncomingResponse>>, public ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >, public FastAllocated<DoHelloBadMD5ReqActor>, public DoHelloBadMD5ReqActorState<DoHelloBadMD5ReqActor> {
															#line 3670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<DoHelloBadMD5ReqActor>::operator new;
	using FastAllocated<DoHelloBadMD5ReqActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::IncomingResponse>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	DoHelloBadMD5ReqActor(Reference<IConnection> const& conn) 
															#line 3681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Reference<HTTP::IncomingResponse>>(),
		   DoHelloBadMD5ReqActorState<DoHelloBadMD5ReqActor>(conn)
	{
		fdb_probe_actor_enter("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doHelloBadMD5Req");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doHelloBadMD5Req", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoHelloBadMD5ReqActor, 0, Reference<HTTP::IncomingResponse> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::IncomingResponse>> doHelloBadMD5Req( Reference<IConnection> const& conn ) {
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Reference<HTTP::IncomingResponse>>(new DoHelloBadMD5ReqActor(conn));
															#line 3709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}

#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

// can't run as regular unit test right now because it needs special setup
															#line 3715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase396()
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class FlowTestCase396Actor>
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class FlowTestCase396ActorState {
															#line 3722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowTestCase396ActorState(UnitTestParameters const& params) 
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : params(params)
															#line 3729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase396", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase396ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase396", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			ASSERT(g_network->isSimulated());
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			fmt::print("Registering sim server\n");
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			hostname = "helloworld-" + deterministicRandom()->randomUniqueID().toString();
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->registerSimHTTPServer(hostname, "80", makeReference<HelloWorldRequestHandler>());
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<FlowTestCase396Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase396Actor*>(this)->actor_wait_state = 1;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase396Actor, 0, Void >*>(static_cast<FlowTestCase396Actor*>(this)));
															#line 3757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~FlowTestCase396ActorState();
		static_cast<FlowTestCase396Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Registered sim server\n");
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = success(doRequestTest(hostname, "80", doHelloWorldReq));
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<FlowTestCase396Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase396Actor*>(this)->actor_wait_state = 2;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase396Actor, 1, Void >*>(static_cast<FlowTestCase396Actor*>(this)));
															#line 3789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Registered sim server\n");
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = success(doRequestTest(hostname, "80", doHelloWorldReq));
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<FlowTestCase396Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase396Actor*>(this)->actor_wait_state = 2;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase396Actor, 1, Void >*>(static_cast<FlowTestCase396Actor*>(this)));
															#line 3807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		if (static_cast<FlowTestCase396Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase396Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase396Actor*>(this)->ActorCallback< FlowTestCase396Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase396Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase396Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase396Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Done Hello\n");
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<FlowTestCase396Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase396ActorState(); static_cast<FlowTestCase396Actor*>(this)->destroy(); return 0; }
															#line 3881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<FlowTestCase396Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase396ActorState();
		static_cast<FlowTestCase396Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Done Hello\n");
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<FlowTestCase396Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase396ActorState(); static_cast<FlowTestCase396Actor*>(this)->destroy(); return 0; }
															#line 3895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<FlowTestCase396Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase396ActorState();
		static_cast<FlowTestCase396Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase396Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase396Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase396Actor*>(this)->ActorCallback< FlowTestCase396Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase396Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase396Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase396Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnitTestParameters params;
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	std::string hostname;
															#line 3970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase396()
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class FlowTestCase396Actor final : public Actor<Void>, public ActorCallback< FlowTestCase396Actor, 0, Void >, public ActorCallback< FlowTestCase396Actor, 1, Void >, public FastAllocated<FlowTestCase396Actor>, public FlowTestCase396ActorState<FlowTestCase396Actor> {
															#line 3975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase396Actor>::operator new;
	using FastAllocated<FlowTestCase396Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase396Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase396Actor, 1, Void >;
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowTestCase396Actor(UnitTestParameters const& params) 
															#line 3987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase396ActorState<FlowTestCase396Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase396", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase396");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase396", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase396Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase396Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
static Future<Void> flowTestCase396( UnitTestParameters const& params ) {
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new FlowTestCase396Actor(params));
															#line 4016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase396, "/HTTP/Server/HelloWorld")

#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 4022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase409()
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class FlowTestCase409Actor>
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class FlowTestCase409ActorState {
															#line 4029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowTestCase409ActorState(UnitTestParameters const& params) 
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : params(params)
															#line 4036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase409", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase409ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase409", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			ASSERT(g_network->isSimulated());
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			fmt::print("Registering sim server\n");
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			hostname = "helloerror-" + deterministicRandom()->randomUniqueID().toString();
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->registerSimHTTPServer(hostname, "80", makeReference<HelloErrorRequestHandler>());
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<FlowTestCase409Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase409Actor*>(this)->actor_wait_state = 1;
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase409Actor, 0, Void >*>(static_cast<FlowTestCase409Actor*>(this)));
															#line 4064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~FlowTestCase409ActorState();
		static_cast<FlowTestCase409Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Registered sim server\n");
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = success(doRequestTest(hostname, "80", doHelloWorldErrorReq));
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<FlowTestCase409Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase409Actor*>(this)->actor_wait_state = 2;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase409Actor, 1, Void >*>(static_cast<FlowTestCase409Actor*>(this)));
															#line 4096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Registered sim server\n");
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = success(doRequestTest(hostname, "80", doHelloWorldErrorReq));
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (static_cast<FlowTestCase409Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase409Actor*>(this)->actor_wait_state = 2;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase409Actor, 1, Void >*>(static_cast<FlowTestCase409Actor*>(this)));
															#line 4114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		if (static_cast<FlowTestCase409Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase409Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase409Actor*>(this)->ActorCallback< FlowTestCase409Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase409Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase409Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase409Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Done Error\n");
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<FlowTestCase409Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase409ActorState(); static_cast<FlowTestCase409Actor*>(this)->destroy(); return 0; }
															#line 4188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<FlowTestCase409Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase409ActorState();
		static_cast<FlowTestCase409Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Done Error\n");
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<FlowTestCase409Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase409ActorState(); static_cast<FlowTestCase409Actor*>(this)->destroy(); return 0; }
															#line 4202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<FlowTestCase409Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase409ActorState();
		static_cast<FlowTestCase409Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase409Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase409Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase409Actor*>(this)->ActorCallback< FlowTestCase409Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase409Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase409Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase409Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnitTestParameters params;
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	std::string hostname;
															#line 4277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase409()
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class FlowTestCase409Actor final : public Actor<Void>, public ActorCallback< FlowTestCase409Actor, 0, Void >, public ActorCallback< FlowTestCase409Actor, 1, Void >, public FastAllocated<FlowTestCase409Actor>, public FlowTestCase409ActorState<FlowTestCase409Actor> {
															#line 4282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase409Actor>::operator new;
	using FastAllocated<FlowTestCase409Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase409Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase409Actor, 1, Void >;
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowTestCase409Actor(UnitTestParameters const& params) 
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase409ActorState<FlowTestCase409Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase409", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase409");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase409", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase409Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase409Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
static Future<Void> flowTestCase409( UnitTestParameters const& params ) {
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new FlowTestCase409Actor(params));
															#line 4323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase409, "/HTTP/Server/HelloError")

#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"

															#line 4329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase422()
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
template <class FlowTestCase422Actor>
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class FlowTestCase422ActorState {
															#line 4336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowTestCase422ActorState(UnitTestParameters const& params) 
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		 : params(params)
															#line 4343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase422", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase422ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase422", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			ASSERT(g_network->isSimulated());
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			fmt::print("Registering sim server\n");
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			hostname = "hellobadmd5-" + deterministicRandom()->randomUniqueID().toString();
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = g_simulator->registerSimHTTPServer(hostname, "80", makeReference<HelloBadMD5RequestHandler>());
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<FlowTestCase422Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase422Actor*>(this)->actor_wait_state = 1;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase422Actor, 0, Void >*>(static_cast<FlowTestCase422Actor*>(this)));
															#line 4371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		this->~FlowTestCase422ActorState();
		static_cast<FlowTestCase422Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Registered sim server\n");
															#line 4394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		try {
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(doRequestTest(hostname, "80", doHelloBadMD5Req));
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<FlowTestCase422Actor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase422Actor*>(this)->actor_wait_state = 2;
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase422Actor, 1, Void >*>(static_cast<FlowTestCase422Actor*>(this)));
															#line 4405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Registered sim server\n");
															#line 4420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		try {
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(doRequestTest(hostname, "80", doHelloBadMD5Req));
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			if (static_cast<FlowTestCase422Actor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase422Actor*>(this)->actor_wait_state = 2;
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase422Actor, 1, Void >*>(static_cast<FlowTestCase422Actor*>(this)));
															#line 4431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
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
		if (static_cast<FlowTestCase422Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase422Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase422Actor*>(this)->ActorCallback< FlowTestCase422Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase422Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase422Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase422Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		fmt::print("Done Bad MD5\n");
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		if (!static_cast<FlowTestCase422Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase422ActorState(); static_cast<FlowTestCase422Actor*>(this)->destroy(); return 0; }
															#line 4511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		new (&static_cast<FlowTestCase422Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase422ActorState();
		static_cast<FlowTestCase422Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
			ASSERT(e.code() == error_code_http_bad_response);
															#line 4524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
			loopDepth = a_body1cont2(loopDepth);
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
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(false);
															#line 4539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
		ASSERT(false);
															#line 4548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

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
		if (static_cast<FlowTestCase422Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase422Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase422Actor*>(this)->ActorCallback< FlowTestCase422Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase422Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase422Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase422Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
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
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	UnitTestParameters params;
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	std::string hostname;
															#line 4633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase422()
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
class FlowTestCase422Actor final : public Actor<Void>, public ActorCallback< FlowTestCase422Actor, 0, Void >, public ActorCallback< FlowTestCase422Actor, 1, Void >, public FastAllocated<FlowTestCase422Actor>, public FlowTestCase422ActorState<FlowTestCase422Actor> {
															#line 4638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase422Actor>::operator new;
	using FastAllocated<FlowTestCase422Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase422Actor, 0, Void >;
friend struct ActorCallback< FlowTestCase422Actor, 1, Void >;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	FlowTestCase422Actor(UnitTestParameters const& params) 
															#line 4650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase422ActorState<FlowTestCase422Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase422", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase422");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase422", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase422Actor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase422Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
static Future<Void> flowTestCase422( UnitTestParameters const& params ) {
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.cpp"
	return Future<Void>(new FlowTestCase422Actor(params));
															#line 4679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTPServer.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase422, "/HTTP/Server/HelloBadMD5")

