#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
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

#define BOOST_SYSTEM_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
#include <boost/asio.hpp>
#include <boost/range.hpp>
#include <thread>

#include "fdbclient/FDBTypes.h"
#include "fdbrpc/SimExternalConnection.h"
#include "flow/Net2Packet.h"
#include "flow/Platform.h"
#include "flow/SendBufferIterator.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

using namespace boost::asio;

static io_service ios;

class SimExternalConnectionImpl {
public:
																#line 44 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
// This generated class is to be used only via onReadable()
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
template <class OnReadableActor>
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class OnReadableActorState {
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	OnReadableActorState(SimExternalConnection* const& self) 
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		 : self(self)
															#line 57 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 43 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(0.1);
															#line 43 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<OnReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 74 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnReadableActor*>(this)->actor_wait_state = 1;
															#line 43 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnReadableActor, 0, Void >*>(static_cast<OnReadableActor*>(this)));
															#line 79 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 44 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (self->readBuffer.empty())
															#line 102 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 45 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_1 = self->onReadableTrigger.onTrigger();
															#line 45 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<OnReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 108 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnReadableActor*>(this)->actor_wait_state = 2;
															#line 45 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadableActor, 1, Void >*>(static_cast<OnReadableActor*>(this)));
															#line 113 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 44 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (self->readBuffer.empty())
															#line 127 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 45 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_1 = self->onReadableTrigger.onTrigger();
															#line 45 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<OnReadableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 133 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<OnReadableActor*>(this)->actor_wait_state = 2;
															#line 45 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadableActor, 1, Void >*>(static_cast<OnReadableActor*>(this)));
															#line 138 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 47 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<OnReadableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnReadableActorState(); static_cast<OnReadableActor*>(this)->destroy(); return 0; }
															#line 215 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	SimExternalConnection* self;
															#line 300 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via onReadable()
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class OnReadableActor final : public Actor<Void>, public ActorCallback< OnReadableActor, 0, Void >, public ActorCallback< OnReadableActor, 1, Void >, public FastAllocated<OnReadableActor>, public OnReadableActorState<OnReadableActor> {
															#line 305 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<OnReadableActor>::operator new;
	using FastAllocated<OnReadableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnReadableActor, 0, Void >;
friend struct ActorCallback< OnReadableActor, 1, Void >;
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	OnReadableActor(SimExternalConnection* const& self) 
															#line 317 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
[[nodiscard]] static Future<Void> onReadable( SimExternalConnection* const& self ) {
															#line 42 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Void>(new OnReadableActor(self));
															#line 345 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
}

#line 49 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"

																#line 350 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
// This generated class is to be used only via connect()
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
template <class ConnectActor>
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class ConnectActorState {
															#line 356 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	ConnectActorState(NetworkAddress const& toAddr) 
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		 : toAddr(toAddr)
															#line 363 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 51 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(0.1);
															#line 51 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<ConnectActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 380 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ConnectActor*>(this)->actor_wait_state = 1;
															#line 51 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectActor, 0, Void >*>(static_cast<ConnectActor*>(this)));
															#line 385 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 52 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ip::tcp::socket socket(ios);
															#line 53 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		auto ip = toAddr.ip;
															#line 54 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ip::address address;
															#line 55 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (ip.isV6())
															#line 414 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 56 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v6(ip.toV6());
															#line 418 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
		else
		{
															#line 58 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v4(ip.toV4());
															#line 424 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
															#line 60 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		boost::system::error_code err;
															#line 61 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		socket.connect(ip::tcp::endpoint(address, toAddr.port), err);
															#line 62 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (err)
															#line 432 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 63 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>()); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 436 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>());
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 65 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>(new SimExternalConnection(std::move(socket)))); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 446 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>(new SimExternalConnection(std::move(socket))));
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 52 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ip::tcp::socket socket(ios);
															#line 53 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		auto ip = toAddr.ip;
															#line 54 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ip::address address;
															#line 55 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (ip.isV6())
															#line 465 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 56 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v6(ip.toV6());
															#line 469 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
		else
		{
															#line 58 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			address = boost::asio::ip::address_v4(ip.toV4());
															#line 475 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		}
															#line 60 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		boost::system::error_code err;
															#line 61 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		socket.connect(ip::tcp::endpoint(address, toAddr.port), err);
															#line 62 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (err)
															#line 483 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 63 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>()); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 487 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			new (&static_cast<ConnectActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(Reference<IConnection>());
			this->~ConnectActorState();
			static_cast<ConnectActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 65 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (!static_cast<ConnectActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(Reference<IConnection>(new SimExternalConnection(std::move(socket)))); this->~ConnectActorState(); static_cast<ConnectActor*>(this)->destroy(); return 0; }
															#line 497 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	NetworkAddress toAddr;
															#line 571 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via connect()
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class ConnectActor final : public Actor<Reference<IConnection>>, public ActorCallback< ConnectActor, 0, Void >, public FastAllocated<ConnectActor>, public ConnectActorState<ConnectActor> {
															#line 576 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<ConnectActor>::operator new;
	using FastAllocated<ConnectActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectActor, 0, Void >;
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	ConnectActor(NetworkAddress const& toAddr) 
															#line 587 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
[[nodiscard]] static Future<Reference<IConnection>> connect( NetworkAddress const& toAddr ) {
															#line 50 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Reference<IConnection>>(new ConnectActor(toAddr));
															#line 614 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
}

#line 68 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
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
			if (addr.is_v6()) {
				addrs.emplace_back(IPAddress(addr.to_v6().to_bytes()), endpoint.port());
			} else {
				addrs.emplace_back(addr.to_v4().to_ulong(), endpoint.port());
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

															#line 712 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
namespace {
// This generated class is to be used only via resolveTCPEndpointImpl()
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
template <class ResolveTCPEndpointImplActor>
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class ResolveTCPEndpointImplActorState {
															#line 719 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	ResolveTCPEndpointImplActorState(std::string const& host,std::string const& service,DNSCache* const& dnsCache) 
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		 : host(host),
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   service(service),
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   dnsCache(dnsCache)
															#line 730 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 165 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(0.1);
															#line 165 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<ResolveTCPEndpointImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 747 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ResolveTCPEndpointImplActor*>(this)->actor_wait_state = 1;
															#line 165 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveTCPEndpointImplActor, 0, Void >*>(static_cast<ResolveTCPEndpointImplActor*>(this)));
															#line 752 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 166 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<ResolveTCPEndpointImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache)); this->~ResolveTCPEndpointImplActorState(); static_cast<ResolveTCPEndpointImplActor*>(this)->destroy(); return 0; }
															#line 775 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<ResolveTCPEndpointImplActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache));
		this->~ResolveTCPEndpointImplActorState();
		static_cast<ResolveTCPEndpointImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 166 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<ResolveTCPEndpointImplActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(SimExternalConnection::resolveTCPEndpointBlocking(host, service, dnsCache)); this->~ResolveTCPEndpointImplActorState(); static_cast<ResolveTCPEndpointImplActor*>(this)->destroy(); return 0; }
															#line 787 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	std::string host;
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	std::string service;
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	DNSCache* dnsCache;
															#line 864 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via resolveTCPEndpointImpl()
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class ResolveTCPEndpointImplActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< ResolveTCPEndpointImplActor, 0, Void >, public FastAllocated<ResolveTCPEndpointImplActor>, public ResolveTCPEndpointImplActorState<ResolveTCPEndpointImplActor> {
															#line 869 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<ResolveTCPEndpointImplActor>::operator new;
	using FastAllocated<ResolveTCPEndpointImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveTCPEndpointImplActor, 0, Void >;
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	ResolveTCPEndpointImplActor(std::string const& host,std::string const& service,DNSCache* const& dnsCache) 
															#line 880 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
[[nodiscard]] static Future<std::vector<NetworkAddress>> resolveTCPEndpointImpl( std::string const& host, std::string const& service, DNSCache* const& dnsCache ) {
															#line 162 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new ResolveTCPEndpointImplActor(host, service, dnsCache));
															#line 908 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
}

#line 168 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"

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

															#line 946 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase202()
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
template <class FlowTestCase202Actor>
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase202ActorState {
															#line 953 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase202ActorState(UnitTestParameters const& params) 
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		 : params(params),
															#line 203 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   maxDataLength(10000),
															#line 204 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   serverThread([] { return testEchoServer(); }),
															#line 205 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   packetQueue(),
															#line 206 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   externalConn()
															#line 968 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase202", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase202ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase202", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 207 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			;
															#line 983 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~FlowTestCase202ActorState();
		static_cast<FlowTestCase202Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 217 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		data = deterministicRandom()->randomAlphaNumeric(deterministicRandom()->randomInt(0, maxDataLength + 1));
															#line 218 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		PacketWriter packetWriter(packetQueue.getWriteBuffer(data.size()), nullptr, Unversioned());
															#line 219 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		packetWriter.serializeBytes(data);
															#line 220 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_1 = externalConn->onWritable();
															#line 220 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1014 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 2;
															#line 220 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase202Actor, 1, Void >*>(static_cast<FlowTestCase202Actor*>(this)));
															#line 1019 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 208 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_0 = INetworkConnections::net()->connect("localhost", std::to_string(testEchoServerPort));
															#line 208 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1037 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 1;
															#line 208 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >*>(static_cast<FlowTestCase202Actor*>(this)));
															#line 1042 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
															#line 210 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (_externalConn.isValid())
															#line 1064 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 211 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			externalConn = std::move(_externalConn);
															#line 1068 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 215 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		threadSleep(0.01);
															#line 1073 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Reference<IConnection> && _externalConn,int loopDepth) 
	{
															#line 210 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (_externalConn.isValid())
															#line 1082 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		{
															#line 211 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			externalConn = std::move(_externalConn);
															#line 1086 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 215 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		threadSleep(0.01);
															#line 1091 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase202Actor*>(this)->ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 221 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->write(packetQueue.getUnsent());
															#line 222 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_2 = externalConn->onReadable();
															#line 222 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1167 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 3;
															#line 222 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase202Actor, 2, Void >*>(static_cast<FlowTestCase202Actor*>(this)));
															#line 1172 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 221 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->write(packetQueue.getUnsent());
															#line 222 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_2 = externalConn->onReadable();
															#line 222 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1185 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 3;
															#line 222 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase202Actor, 2, Void >*>(static_cast<FlowTestCase202Actor*>(this)));
															#line 1190 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase202Actor*>(this)->ActorCallback< FlowTestCase202Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase202Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase202Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase202Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 223 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		std::vector<uint8_t> vec(data.size());
															#line 224 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->read(&vec[0], &vec[0] + vec.size());
															#line 225 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->close();
															#line 226 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StringRef echo(&vec[0], vec.size());
															#line 227 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(echo.toString() == data.toString());
															#line 228 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		serverThread.join();
															#line 229 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase202Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase202ActorState(); static_cast<FlowTestCase202Actor*>(this)->destroy(); return 0; }
															#line 1274 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase202Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase202ActorState();
		static_cast<FlowTestCase202Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 223 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		std::vector<uint8_t> vec(data.size());
															#line 224 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->read(&vec[0], &vec[0] + vec.size());
															#line 225 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		externalConn->close();
															#line 226 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StringRef echo(&vec[0], vec.size());
															#line 227 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(echo.toString() == data.toString());
															#line 228 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		serverThread.join();
															#line 229 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase202Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase202ActorState(); static_cast<FlowTestCase202Actor*>(this)->destroy(); return 0; }
															#line 1298 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase202Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase202ActorState();
		static_cast<FlowTestCase202Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase202Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase202Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase202Actor*>(this)->ActorCallback< FlowTestCase202Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase202Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase202Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase202Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	UnitTestParameters params;
															#line 203 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	const size_t maxDataLength;
															#line 204 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	std::thread serverThread;
															#line 205 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	UnsentPacketQueue packetQueue;
															#line 206 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	Reference<IConnection> externalConn;
															#line 217 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	Key data;
															#line 1381 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase202()
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase202Actor final : public Actor<Void>, public ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >, public ActorCallback< FlowTestCase202Actor, 1, Void >, public ActorCallback< FlowTestCase202Actor, 2, Void >, public FastAllocated<FlowTestCase202Actor>, public FlowTestCase202ActorState<FlowTestCase202Actor> {
															#line 1386 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase202Actor>::operator new;
	using FastAllocated<FlowTestCase202Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >;
friend struct ActorCallback< FlowTestCase202Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase202Actor, 2, Void >;
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase202Actor(UnitTestParameters const& params) 
															#line 1399 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase202ActorState<FlowTestCase202Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase202", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase202");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase202", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase202Actor, 0, Reference<IConnection> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase202Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase202Actor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
static Future<Void> flowTestCase202( UnitTestParameters const& params ) {
															#line 202 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Void>(new FlowTestCase202Actor(params));
															#line 1429 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase202, "fdbrpc/SimExternalClient")

#line 231 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"

															#line 1435 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase232()
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
template <class FlowTestCase232Actor>
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase232ActorState {
															#line 1442 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase232ActorState(UnitTestParameters const& params) 
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		 : params(params),
															#line 233 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   networkAddresses(),
															#line 234 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		   address1(IPAddress(0x13131313), 1)
															#line 1453 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase232", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase232ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase232", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 235 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			networkAddresses.push_back(address1);
															#line 236 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			INetworkConnections::net()->addMockTCPEndpoint("testhost1", "port1", networkAddresses);
															#line 237 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = INetworkConnections::net()->resolveTCPEndpoint("testhost1", "port1");
															#line 237 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			if (static_cast<FlowTestCase232Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1474 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase232Actor*>(this)->actor_wait_state = 1;
															#line 237 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >*>(static_cast<FlowTestCase232Actor*>(this)));
															#line 1479 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		this->~FlowTestCase232ActorState();
		static_cast<FlowTestCase232Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 239 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(resolvedNetworkAddresses.size() == 1);
															#line 240 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(std::find(resolvedNetworkAddresses.begin(), resolvedNetworkAddresses.end(), address1) != resolvedNetworkAddresses.end());
															#line 242 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		INetworkConnections::net()->removeMockTCPEndpoint("testhost1", "port1");
															#line 243 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		address2 = NetworkAddress(IPAddress(0x14141414), 2);
															#line 244 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		networkAddresses.push_back(address2);
															#line 245 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		INetworkConnections::net()->addMockTCPEndpoint("testhost1", "port1", networkAddresses);
															#line 246 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		StrictFuture<Void> __when_expr_1 = store(resolvedNetworkAddresses, INetworkConnections::net()->resolveTCPEndpoint("testhost1", "port1"));
															#line 246 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (static_cast<FlowTestCase232Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1516 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<FlowTestCase232Actor*>(this)->actor_wait_state = 2;
															#line 246 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase232Actor, 1, Void >*>(static_cast<FlowTestCase232Actor*>(this)));
															#line 1521 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> const& __resolvedNetworkAddresses,int loopDepth) 
	{
															#line 237 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		resolvedNetworkAddresses = __resolvedNetworkAddresses;
															#line 1530 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
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
		if (static_cast<FlowTestCase232Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase232Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase232Actor*>(this)->ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 247 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(resolvedNetworkAddresses.size() == 2);
															#line 248 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(std::find(resolvedNetworkAddresses.begin(), resolvedNetworkAddresses.end(), address2) != resolvedNetworkAddresses.end());
															#line 251 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase232Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase232ActorState(); static_cast<FlowTestCase232Actor*>(this)->destroy(); return 0; }
															#line 1601 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase232Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase232ActorState();
		static_cast<FlowTestCase232Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 247 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(resolvedNetworkAddresses.size() == 2);
															#line 248 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		ASSERT(std::find(resolvedNetworkAddresses.begin(), resolvedNetworkAddresses.end(), address2) != resolvedNetworkAddresses.end());
															#line 251 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
		if (!static_cast<FlowTestCase232Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase232ActorState(); static_cast<FlowTestCase232Actor*>(this)->destroy(); return 0; }
															#line 1617 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		new (&static_cast<FlowTestCase232Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase232ActorState();
		static_cast<FlowTestCase232Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase232Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase232Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase232Actor*>(this)->ActorCallback< FlowTestCase232Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase232Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase232Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase232Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	UnitTestParameters params;
															#line 233 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	std::vector<NetworkAddress> networkAddresses;
															#line 234 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	NetworkAddress address1;
															#line 237 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	std::vector<NetworkAddress> resolvedNetworkAddresses;
															#line 243 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	NetworkAddress address2;
															#line 1698 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase232()
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
class FlowTestCase232Actor final : public Actor<Void>, public ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >, public ActorCallback< FlowTestCase232Actor, 1, Void >, public FastAllocated<FlowTestCase232Actor>, public FlowTestCase232ActorState<FlowTestCase232Actor> {
															#line 1703 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase232Actor>::operator new;
	using FastAllocated<FlowTestCase232Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >;
friend struct ActorCallback< FlowTestCase232Actor, 1, Void >;
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	FlowTestCase232Actor(UnitTestParameters const& params) 
															#line 1715 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase232ActorState<FlowTestCase232Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase232", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase232");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase232", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase232Actor, 0, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase232Actor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
static Future<Void> flowTestCase232( UnitTestParameters const& params ) {
															#line 232 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"
	return Future<Void>(new FlowTestCase232Actor(params));
															#line 1744 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase232, "fdbrpc/MockDNS")

#line 253 "/usr/src/libfdb_c/fdbrpc/SimExternalConnection.actor.cpp"

void forceLinkSimExternalConnectionTests() {}
