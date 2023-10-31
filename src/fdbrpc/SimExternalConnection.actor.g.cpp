#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
/*
 * SimExternalConnection.actor.cpp
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

#ifndef BOOST_SYSTEM_NO_LIB
#define BOOST_SYSTEM_NO_LIB
#endif
#ifndef BOOST_DATE_TIME_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#endif
#ifndef BOOST_REGEX_NO_LIB
#define BOOST_REGEX_NO_LIB
#endif
#include <boost/asio.hpp>
#include <boost/range.hpp>
#include <thread>

#include "fdbrpc/SimExternalConnection.h"
#include "flow/Net2Packet.h"
#include "flow/Platform.h"
#include "flow/SendBufferIterator.h"
#include "flow/UnitTest.h"
#include "flow/IConnection.h"
#include "flow/actorcompiler.h" // This must be the last #include.

using namespace boost::asio;

static io_service ios;

class SimExternalConnectionImpl {
public:
																#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
// This generated class is to be used only via onReadable()
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
template <class OnReadableActor>
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class OnReadableActorState {
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	OnReadableActorState(SimExternalConnection* const& self) 
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		 : self(self)
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("onReadable", reinterpret_cast<unsigned long>(this));

	}
	~OnReadableActorState() 
	{
		fdb_probe_actor_destroy("onReadable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(0.1);
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<OnReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnReadableActor*>(this)->actor_wait_state = 1;
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnReadableActor, 0, Void >*>(static_cast<OnReadableActor*>(this)));
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~OnReadableActorState();
		static_cast<OnReadableActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (self->readBuffer.empty())
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_1 = self->onReadableTrigger.onTrigger();
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<OnReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnReadableActor*>(this)->actor_wait_state = 2;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadableActor, 1, Void >*>(static_cast<OnReadableActor*>(this)));
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (self->readBuffer.empty())
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_1 = self->onReadableTrigger.onTrigger();
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<OnReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnReadableActor*>(this)->actor_wait_state = 2;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadableActor, 1, Void >*>(static_cast<OnReadableActor*>(this)));
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
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
		if (static_cast<OnReadableActor*>(this)->actor_wait_state > 0) static_cast<OnReadableActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadableActor*>(this)->ActorCallback< OnReadableActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadableActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnReadableActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnReadableActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<OnReadableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnReadableActorState(); static_cast<OnReadableActor*>(this)->destroy(); return 0; }
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<OnReadableActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnReadableActorState();
		static_cast<OnReadableActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

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
		if (static_cast<OnReadableActor*>(this)->actor_wait_state > 0) static_cast<OnReadableActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadableActor*>(this)->ActorCallback< OnReadableActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadableActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnReadableActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnReadableActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	SimExternalConnection* self;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via onReadable()
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class OnReadableActor final : public Actor<Void>, public ActorCallback< OnReadableActor, 0, Void >, public ActorCallback< OnReadableActor, 1, Void >, public FastAllocated<OnReadableActor>, public OnReadableActorState<OnReadableActor> {
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<OnReadableActor>::operator new;
	using FastAllocated<OnReadableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnReadableActor, 0, Void >;
friend struct ActorCallback< OnReadableActor, 1, Void >;
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	OnReadableActor(SimExternalConnection* const& self) 
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<Void>(),
		   OnReadableActorState<OnReadableActor>(self)
	{
		fdb_probe_actor_enter("onReadable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onReadable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onReadable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnReadableActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnReadableActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
[[nodiscard]] static Future<Void> onReadable( SimExternalConnection* const& self ) {
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Void>(new OnReadableActor(self));
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
}

#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"

																#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
template <class ConnectActor>
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class ConnectActorState {
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	ConnectActorState(NetworkAddress const& toAddr) 
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		 : toAddr(toAddr)
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("connect", reinterpret_cast<unsigned long>(this));

	}
	~ConnectActorState() 
	{
		fdb_probe_actor_destroy("connect", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(0.1);
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<ConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ConnectActor*>(this)->actor_wait_state = 1;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor, 0, Void >*>(static_cast<ConnectActor*>(this)));
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~ConnectActorState();
		static_cast<ConnectActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ip::tcp::socket socket(ios);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		auto ip = toAddr.ip;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ip::address address;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (ip.isV6())
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v6(ip.toV6());
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
		else
		{
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v4(ip.toV4());
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		boost::system::error_code err;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		socket.connect(ip::tcp::endpoint(address, toAddr.port), err);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (err)
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>()); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>());
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>(new SimExternalConnection(std::move(socket)))); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>(new SimExternalConnection(std::move(socket))));
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ip::tcp::socket socket(ios);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		auto ip = toAddr.ip;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ip::address address;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (ip.isV6())
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v6(ip.toV6());
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
		else
		{
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v4(ip.toV4());
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		boost::system::error_code err;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		socket.connect(ip::tcp::endpoint(address, toAddr.port), err);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (err)
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>()); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>());
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>(new SimExternalConnection(std::move(socket)))); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>(new SimExternalConnection(std::move(socket))));
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		if (static_cast<ConnectActor*>(this)->actor_wait_state > 0) static_cast<ConnectActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectActor*>(this)->ActorCallback< ConnectActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	NetworkAddress toAddr;
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class ConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< ConnectActor, 0, Void >, public FastAllocated<ConnectActor>, public ConnectActorState<ConnectActor> {
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<ConnectActor>::operator new;
	using FastAllocated<ConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor, 0, Void >;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	ConnectActor(NetworkAddress const& toAddr) 
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   ConnectActorState<ConnectActor>(toAddr)
	{
		fdb_probe_actor_enter("connect", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connect");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connect", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> connect( NetworkAddress const& toAddr ) {
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Reference<IConnection>>(new ConnectActor(toAddr));
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
}

#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
};

void SimExternalConnection::close() {
	socket.close();
}

Future<Void> SimExternalConnection::acceptHandshake() {
	return Void();
}

Future<Void> SimExternalConnection::connectHandshake() {
	return Void();
}

Future<Void> SimExternalConnection::onWritable() {
	return Void();
}

Future<Void> SimExternalConnection::onReadable() {
	return SimExternalConnectionImpl::onReadable(this);
}

int SimExternalConnection::read(uint8_t* begin, uint8_t* end) {
	auto toRead = std::min<int>(end - begin, readBuffer.size());
	std::copy(readBuffer.begin(), readBuffer.begin() + toRead, begin);
	readBuffer.erase(readBuffer.begin(), readBuffer.begin() + toRead);
	return toRead;
}

int SimExternalConnection::write(SendBuffer const* buffer, int limit) {
	boost::system::error_code err;
	bool triggerReaders = (socket.available() == 0);
	int bytesSent = socket.write_some(
	    boost::iterator_range<SendBufferIterator>(SendBufferIterator(buffer, limit), SendBufferIterator()), err);
	ASSERT(!err);
	ASSERT(bytesSent > 0);
	threadSleep(0.1);
	const auto bytesReadable = socket.available();
	std::vector<uint8_t> tempReadBuffer(bytesReadable);
	for (int index = 0; index < bytesReadable;) {
		index += socket.read_some(mutable_buffers_1(&tempReadBuffer[index], bytesReadable), err);
	}
	std::copy(tempReadBuffer.begin(), tempReadBuffer.end(), std::inserter(readBuffer, readBuffer.end()));
	ASSERT(!err);
	ASSERT(socket.available() == 0);
	if (triggerReaders) {
		onReadableTrigger.trigger();
	}
	return bytesSent;
}

NetworkAddress SimExternalConnection::getPeerAddress() const {
	auto endpoint = socket.remote_endpoint();
	auto addr = endpoint.address();
	if (addr.is_v6()) {
		return NetworkAddress(IPAddress(addr.to_v6().to_bytes()), endpoint.port());
	} else {
		return NetworkAddress(addr.to_v4().to_ulong(), endpoint.port());
	}
}

bool SimExternalConnection::hasTrustedPeer() const {
	return true;
}

UID SimExternalConnection::getDebugID() const {
	return dbgid;
}

std::vector<NetworkAddress> SimExternalConnection::resolveTCPEndpointBlocking(const std::string& host,
                                                                              const std::string& service,
                                                                              DNSCache* dnsCache) {
	ip::tcp::resolver resolver(ios);
	try {
		auto iter = resolver.resolve(host, service);
		decltype(iter) end;
		std::vector<NetworkAddress> addrs;
		while (iter != end) {
			auto endpoint = iter->endpoint();
			auto addr = endpoint.address();
			// register the endpoint as public so that if it does happen to be an fdb process, we can connect to it
			// successfully
			if (addr.is_v6()) {
				addrs.emplace_back(IPAddress(addr.to_v6().to_bytes()), endpoint.port(), true, false);
			} else {
				addrs.emplace_back(addr.to_v4().to_ulong(), endpoint.port(), true, false);
			}
			++iter;
		}
		if (addrs.empty()) {
			throw lookup_failed();
		}
		dnsCache->add(host, service, addrs);
		return addrs;
	} catch (...) {
		dnsCache->remove(host, service);
		throw lookup_failed();
	}
}

															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
namespace {
// This generated class is to be used only via resolveTCPEndpointImpl()
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
template <class ResolveTCPEndpointImplActor>
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class ResolveTCPEndpointImplActorState {
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	ResolveTCPEndpointImplActorState(std::string const& host,std::string const& service,DNSCache* const& dnsCache) 
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		 : host(host),
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   service(service),
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   dnsCache(dnsCache)
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this));

	}
	~ResolveTCPEndpointImplActorState() 
	{
		fdb_probe_actor_destroy("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(0.1);
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<ResolveTCPEndpointImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ResolveTCPEndpointImplActor*>(this)->actor_wait_state = 1;
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveTCPEndpointImplActor, 0, Void >*>(static_cast<ResolveTCPEndpointImplActor*>(this)));
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~ResolveTCPEndpointImplActorState();
		static_cast<ResolveTCPEndpointImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<ResolveTCPEndpointImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache)); this->~ResolveTCPEndpointImplActorState(); static_cast<ResolveTCPEndpointImplActor*>(this)->destroy(); return 0; }
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<ResolveTCPEndpointImplActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache));
		this->~ResolveTCPEndpointImplActorState();
		static_cast<ResolveTCPEndpointImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<ResolveTCPEndpointImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache)); this->~ResolveTCPEndpointImplActorState(); static_cast<ResolveTCPEndpointImplActor*>(this)->destroy(); return 0; }
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<ResolveTCPEndpointImplActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache));
		this->~ResolveTCPEndpointImplActorState();
		static_cast<ResolveTCPEndpointImplActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ResolveTCPEndpointImplActor*>(this)->actor_wait_state > 0) static_cast<ResolveTCPEndpointImplActor*>(this)->actor_wait_state = 0;
		static_cast<ResolveTCPEndpointImplActor*>(this)->ActorCallback< ResolveTCPEndpointImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ResolveTCPEndpointImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResolveTCPEndpointImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ResolveTCPEndpointImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	std::string host;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	std::string service;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	DNSCache* dnsCache;
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via resolveTCPEndpointImpl()
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class ResolveTCPEndpointImplActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< ResolveTCPEndpointImplActor, 0, Void >, public FastAllocated<ResolveTCPEndpointImplActor>, public ResolveTCPEndpointImplActorState<ResolveTCPEndpointImplActor> {
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<ResolveTCPEndpointImplActor>::operator new;
	using FastAllocated<ResolveTCPEndpointImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveTCPEndpointImplActor, 0, Void >;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	ResolveTCPEndpointImplActor(std::string const& host,std::string const& service,DNSCache* const& dnsCache) 
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<std::vector<NetworkAddress>>(),
		   ResolveTCPEndpointImplActorState<ResolveTCPEndpointImplActor>(host, service, dnsCache)
	{
		fdb_probe_actor_enter("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resolveTCPEndpointImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("resolveTCPEndpointImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ResolveTCPEndpointImplActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
[[nodiscard]] static Future<std::vector<NetworkAddress>> resolveTCPEndpointImpl( std::string const& host, std::string const& service, DNSCache* const& dnsCache ) {
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new ResolveTCPEndpointImplActor(host, service, dnsCache));
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
}

#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"

Future<std::vector<NetworkAddress>> SimExternalConnection::resolveTCPEndpoint(const std::string& host,
                                                                              const std::string& service,
                                                                              DNSCache* dnsCache) {
	return resolveTCPEndpointImpl(host, service, dnsCache);
}

Future<Reference<IConnection>> SimExternalConnection::connect(NetworkAddress toAddr) {
	return SimExternalConnectionImpl::connect(toAddr);
}

SimExternalConnection::SimExternalConnection(ip::tcp::socket&& socket)
  : socket(std::move(socket)), dbgid(deterministicRandom()->randomUniqueID()) {}

static constexpr auto testEchoServerPort = 8000;

static void testEchoServer() {
	static constexpr auto readBufferSize = 1000;
	io_service ios;
	ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), testEchoServerPort));
	ip::tcp::socket socket(ios);
	acceptor.accept(socket);
	loop {
		char readBuffer[readBufferSize];
		boost::system::error_code err;
		auto length = socket.read_some(mutable_buffers_1(readBuffer, readBufferSize), err);
		if (err == boost::asio::error::eof) {
			return;
		}
		ASSERT(!err);
		write(socket, buffer(readBuffer, length));
	}
}

															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase214()
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
template <class FlowTestCase214Actor>
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase214ActorState {
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase214ActorState(UnitTestParameters const& params) 
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		 : params(params),
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   maxDataLength(10000),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   serverThread([] { return testEchoServer(); }),
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   packetQueue(),
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   externalConn()
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase214", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase214ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase214", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			;
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~FlowTestCase214ActorState();
		static_cast<FlowTestCase214Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		data = Standalone<StringRef>(deterministicRandom()->randomAlphaNumeric(deterministicRandom()->randomInt(0, maxDataLength + 1)));
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		PacketWriter packetWriter(packetQueue.getWriteBuffer(data.size()), nullptr, Unversioned());
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		packetWriter.serializeBytes(data);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_1 = externalConn->onWritable();
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 2;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase214Actor, 1, Void >*>(static_cast<FlowTestCase214Actor*>(this)));
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_0 = INetworkConnections::net()->connect("localhost", std::to_string(testEchoServerPort));
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 1;
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >*>(static_cast<FlowTestCase214Actor*>(this)));
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
	int a_body1loopBody1cont1(Reference<IConnection> const& _externalConn,int loopDepth) 
	{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (_externalConn.isValid())
															#line 1076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			externalConn = std::move(_externalConn);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		threadSleep(0.01);
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Reference<IConnection> && _externalConn,int loopDepth) 
	{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (_externalConn.isValid())
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			externalConn = std::move(_externalConn);
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		threadSleep(0.01);
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IConnection> const& _externalConn,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_externalConn, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IConnection> && _externalConn,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_externalConn), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase214Actor*>(this)->ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->write(packetQueue.getUnsent());
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_2 = externalConn->onReadable();
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 3;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase214Actor, 2, Void >*>(static_cast<FlowTestCase214Actor*>(this)));
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->write(packetQueue.getUnsent());
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_2 = externalConn->onReadable();
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 3;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase214Actor, 2, Void >*>(static_cast<FlowTestCase214Actor*>(this)));
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase214Actor*>(this)->ActorCallback< FlowTestCase214Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase214Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase214Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase214Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		std::vector<uint8_t> vec(data.size());
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->read(&vec[0], &vec[0] + vec.size());
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->close();
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StringRef echo(&vec[0], vec.size());
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(echo.toString() == data.toString());
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		serverThread.join();
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase214Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase214ActorState(); static_cast<FlowTestCase214Actor*>(this)->destroy(); return 0; }
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase214Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase214ActorState();
		static_cast<FlowTestCase214Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		std::vector<uint8_t> vec(data.size());
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->read(&vec[0], &vec[0] + vec.size());
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->close();
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StringRef echo(&vec[0], vec.size());
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(echo.toString() == data.toString());
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		serverThread.join();
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase214Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase214ActorState(); static_cast<FlowTestCase214Actor*>(this)->destroy(); return 0; }
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase214Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase214ActorState();
		static_cast<FlowTestCase214Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase214Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase214Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase214Actor*>(this)->ActorCallback< FlowTestCase214Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase214Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase214Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase214Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	UnitTestParameters params;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	const size_t maxDataLength;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	std::thread serverThread;
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	UnsentPacketQueue packetQueue;
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	Reference<IConnection> externalConn;
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	Standalone<StringRef> data;
															#line 1393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase214()
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase214Actor final : public Actor<Void>, public ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >, public ActorCallback< FlowTestCase214Actor, 1, Void >, public ActorCallback< FlowTestCase214Actor, 2, Void >, public FastAllocated<FlowTestCase214Actor>, public FlowTestCase214ActorState<FlowTestCase214Actor> {
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase214Actor>::operator new;
	using FastAllocated<FlowTestCase214Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >;
friend struct ActorCallback< FlowTestCase214Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase214Actor, 2, Void >;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase214Actor(UnitTestParameters const& params) 
															#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase214ActorState<FlowTestCase214Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase214", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase214");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase214", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase214Actor, 0, Reference<IConnection> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase214Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase214Actor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
static Future<Void> flowTestCase214( UnitTestParameters const& params ) {
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Void>(new FlowTestCase214Actor(params));
															#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase214, "fdbrpc/SimExternalClient")

#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"

															#line 1447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase245()
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
template <class FlowTestCase245Actor>
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase245ActorState {
															#line 1454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase245ActorState(UnitTestParameters const& params) 
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		 : params(params),
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   networkAddresses(),
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		   address1(IPAddress(0x13131313), 1)
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase245", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase245ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase245", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			networkAddresses.push_back(address1);
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			INetworkConnections::net()->addMockTCPEndpoint("testhost1", "port1", networkAddresses);
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = INetworkConnections::net()->resolveTCPEndpoint("testhost1", "port1");
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<FlowTestCase245Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase245Actor*>(this)->actor_wait_state = 1;
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >*>(static_cast<FlowTestCase245Actor*>(this)));
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~FlowTestCase245ActorState();
		static_cast<FlowTestCase245Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(resolvedNetworkAddresses.size() == 1);
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(std::find(resolvedNetworkAddresses.begin(), resolvedNetworkAddresses.end(), address1) != resolvedNetworkAddresses.end());
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		INetworkConnections::net()->removeMockTCPEndpoint("testhost1", "port1");
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		address2 = NetworkAddress(IPAddress(0x14141414), 2);
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		networkAddresses.push_back(address2);
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		INetworkConnections::net()->addMockTCPEndpoint("testhost1", "port1", networkAddresses);
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_1 = store(resolvedNetworkAddresses, INetworkConnections::net()->resolveTCPEndpoint("testhost1", "port1"));
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase245Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase245Actor*>(this)->actor_wait_state = 2;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase245Actor, 1, Void >*>(static_cast<FlowTestCase245Actor*>(this)));
															#line 1533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> const& __resolvedNetworkAddresses,int loopDepth) 
	{
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		resolvedNetworkAddresses = __resolvedNetworkAddresses;
															#line 1542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> && __resolvedNetworkAddresses,int loopDepth) 
	{
		resolvedNetworkAddresses = std::move(__resolvedNetworkAddresses);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase245Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase245Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase245Actor*>(this)->ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(resolvedNetworkAddresses.size() == 2);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(std::find(resolvedNetworkAddresses.begin(), resolvedNetworkAddresses.end(), address2) != resolvedNetworkAddresses.end());
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase245Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase245ActorState(); static_cast<FlowTestCase245Actor*>(this)->destroy(); return 0; }
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase245Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase245ActorState();
		static_cast<FlowTestCase245Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(resolvedNetworkAddresses.size() == 2);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(std::find(resolvedNetworkAddresses.begin(), resolvedNetworkAddresses.end(), address2) != resolvedNetworkAddresses.end());
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase245Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase245ActorState(); static_cast<FlowTestCase245Actor*>(this)->destroy(); return 0; }
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase245Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase245ActorState();
		static_cast<FlowTestCase245Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase245Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase245Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase245Actor*>(this)->ActorCallback< FlowTestCase245Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase245Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase245Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase245Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	UnitTestParameters params;
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	std::vector<NetworkAddress> networkAddresses;
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	NetworkAddress address1;
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	std::vector<NetworkAddress> resolvedNetworkAddresses;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	NetworkAddress address2;
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase245()
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase245Actor final : public Actor<Void>, public ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >, public ActorCallback< FlowTestCase245Actor, 1, Void >, public FastAllocated<FlowTestCase245Actor>, public FlowTestCase245ActorState<FlowTestCase245Actor> {
															#line 1715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase245Actor>::operator new;
	using FastAllocated<FlowTestCase245Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >;
friend struct ActorCallback< FlowTestCase245Actor, 1, Void >;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase245Actor(UnitTestParameters const& params) 
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase245ActorState<FlowTestCase245Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase245", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase245");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase245", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase245Actor, 0, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase245Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
static Future<Void> flowTestCase245( UnitTestParameters const& params ) {
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Void>(new FlowTestCase245Actor(params));
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase245, "fdbrpc/MockDNS")

#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/SimExternalConnection.actor.cpp"

void forceLinkSimExternalConnectionTests() {}
