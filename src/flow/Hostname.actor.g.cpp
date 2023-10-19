#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
/*
 * Hostname.actor.cpp
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

#include "flow/Hostname.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // has to be last include

Hostname Hostname::parse(const std::string& s) {
	if (s.empty() || !Hostname::isHostname(s)) {
		throw connection_string_invalid();
	}

	bool isTLS = false;
	std::string f;
	if (s.size() > 4 && strcmp(s.c_str() + s.size() - 4, ":tls") == 0) {
		isTLS = true;
		f = s.substr(0, s.size() - 4);
	} else {
		f = s;
	}
	auto colonPos = f.find_first_of(":");
	return Hostname(f.substr(0, colonPos), f.substr(colonPos + 1), isTLS);
}

															#line 44 "/src/flow/Hostname.actor.g.cpp"
namespace {
// This generated class is to be used only via resolveImpl()
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
template <class ResolveImplActor>
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
class ResolveImplActorState {
															#line 51 "/src/flow/Hostname.actor.g.cpp"
public:
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	ResolveImplActorState(Hostname* const& self) 
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		 : self(self)
															#line 58 "/src/flow/Hostname.actor.g.cpp"
	{

	}
	~ResolveImplActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 44 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
				StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = INetworkConnections::net()->resolveTCPEndpointWithDNSCache(self->host, self->service);
															#line 44 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
				if (static_cast<ResolveImplActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 74 "/src/flow/Hostname.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ResolveImplActor*>(this)->actor_wait_state = 1;
															#line 44 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*>(static_cast<ResolveImplActor*>(this)));
															#line 79 "/src/flow/Hostname.actor.g.cpp"
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
		this->~ResolveImplActorState();
		static_cast<ResolveImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 54 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			if (!static_cast<ResolveImplActor*>(this)->SAV<Optional<NetworkAddress>>::futures) { (void)(Optional<NetworkAddress>()); this->~ResolveImplActorState(); static_cast<ResolveImplActor*>(this)->destroy(); return 0; }
															#line 109 "/src/flow/Hostname.actor.g.cpp"
			new (&static_cast<ResolveImplActor*>(this)->SAV< Optional<NetworkAddress> >::value()) Optional<NetworkAddress>(Optional<NetworkAddress>());
			this->~ResolveImplActorState();
			static_cast<ResolveImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(std::vector<NetworkAddress> const& addresses,int loopDepth) 
	{
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		NetworkAddress address = INetworkConnections::pickOneAddress(addresses);
															#line 47 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		address.flags = 0;
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		address.fromHostname = NetworkAddressFromHostname::True;
															#line 49 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (self->isTLS)
															#line 133 "/src/flow/Hostname.actor.g.cpp"
		{
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			address.flags |= NetworkAddress::FLAG_TLS;
															#line 137 "/src/flow/Hostname.actor.g.cpp"
		}
															#line 52 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (!static_cast<ResolveImplActor*>(this)->SAV<Optional<NetworkAddress>>::futures) { (void)(address); this->~ResolveImplActorState(); static_cast<ResolveImplActor*>(this)->destroy(); return 0; }
															#line 141 "/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<ResolveImplActor*>(this)->SAV< Optional<NetworkAddress> >::value()) Optional<NetworkAddress>(address);
		this->~ResolveImplActorState();
		static_cast<ResolveImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<NetworkAddress> && addresses,int loopDepth) 
	{
															#line 46 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		NetworkAddress address = INetworkConnections::pickOneAddress(addresses);
															#line 47 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		address.flags = 0;
															#line 48 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		address.fromHostname = NetworkAddressFromHostname::True;
															#line 49 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (self->isTLS)
															#line 159 "/src/flow/Hostname.actor.g.cpp"
		{
															#line 50 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			address.flags |= NetworkAddress::FLAG_TLS;
															#line 163 "/src/flow/Hostname.actor.g.cpp"
		}
															#line 52 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (!static_cast<ResolveImplActor*>(this)->SAV<Optional<NetworkAddress>>::futures) { (void)(address); this->~ResolveImplActorState(); static_cast<ResolveImplActor*>(this)->destroy(); return 0; }
															#line 167 "/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<ResolveImplActor*>(this)->SAV< Optional<NetworkAddress> >::value()) Optional<NetworkAddress>(address);
		this->~ResolveImplActorState();
		static_cast<ResolveImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> const& addresses,int loopDepth) 
	{
		loopDepth = a_body1cont2(addresses, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> && addresses,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(addresses), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ResolveImplActor*>(this)->actor_wait_state > 0) static_cast<ResolveImplActor*>(this)->actor_wait_state = 0;
		static_cast<ResolveImplActor*>(this)->ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}

	}
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	Hostname* self;
															#line 234 "/src/flow/Hostname.actor.g.cpp"
};
// This generated class is to be used only via resolveImpl()
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
class ResolveImplActor final : public Actor<Optional<NetworkAddress>>, public ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >, public FastAllocated<ResolveImplActor>, public ResolveImplActorState<ResolveImplActor> {
															#line 239 "/src/flow/Hostname.actor.g.cpp"
public:
	using FastAllocated<ResolveImplActor>::operator new;
	using FastAllocated<ResolveImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >;
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	ResolveImplActor(Hostname* const& self) 
															#line 250 "/src/flow/Hostname.actor.g.cpp"
		 : Actor<Optional<NetworkAddress>>(),
		   ResolveImplActorState<ResolveImplActor>(self)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resolveImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
[[nodiscard]] Future<Optional<NetworkAddress>> resolveImpl( Hostname* const& self ) {
															#line 42 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	return Future<Optional<NetworkAddress>>(new ResolveImplActor(self));
															#line 276 "/src/flow/Hostname.actor.g.cpp"
}

#line 57 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"

															#line 281 "/src/flow/Hostname.actor.g.cpp"
namespace {
// This generated class is to be used only via resolveWithRetryImpl()
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
template <class ResolveWithRetryImplActor>
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
class ResolveWithRetryImplActorState {
															#line 288 "/src/flow/Hostname.actor.g.cpp"
public:
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	ResolveWithRetryImplActorState(Hostname* const& self) 
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		 : self(self),
															#line 59 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		   resolveInterval(FLOW_KNOBS->HOSTNAME_RESOLVE_INIT_INTERVAL)
															#line 297 "/src/flow/Hostname.actor.g.cpp"
	{

	}
	~ResolveWithRetryImplActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 60 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			;
															#line 310 "/src/flow/Hostname.actor.g.cpp"
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
		this->~ResolveWithRetryImplActorState();
		static_cast<ResolveWithRetryImplActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = resolveImpl(self);
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 343 "/src/flow/Hostname.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 1;
															#line 62 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*>(static_cast<ResolveWithRetryImplActor*>(this)));
															#line 348 "/src/flow/Hostname.actor.g.cpp"
			loopDepth = 0;
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
															#line 69 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(e.code() == error_code_actor_cancelled);
															#line 70 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 372 "/src/flow/Hostname.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<NetworkAddress> const& address,int loopDepth) 
	{
															#line 63 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (address.present())
															#line 386 "/src/flow/Hostname.actor.g.cpp"
		{
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			if (!static_cast<ResolveWithRetryImplActor*>(this)->SAV<NetworkAddress>::futures) { (void)(address.get()); this->~ResolveWithRetryImplActorState(); static_cast<ResolveWithRetryImplActor*>(this)->destroy(); return 0; }
															#line 390 "/src/flow/Hostname.actor.g.cpp"
			new (&static_cast<ResolveWithRetryImplActor*>(this)->SAV< NetworkAddress >::value()) NetworkAddress(address.get());
			this->~ResolveWithRetryImplActorState();
			static_cast<ResolveWithRetryImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(resolveInterval);
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 400 "/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 2;
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ResolveWithRetryImplActor, 1, Void >*>(static_cast<ResolveWithRetryImplActor*>(this)));
															#line 405 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 63 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (address.present())
															#line 414 "/src/flow/Hostname.actor.g.cpp"
		{
															#line 64 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			if (!static_cast<ResolveWithRetryImplActor*>(this)->SAV<NetworkAddress>::futures) { (void)(address.get()); this->~ResolveWithRetryImplActorState(); static_cast<ResolveWithRetryImplActor*>(this)->destroy(); return 0; }
															#line 418 "/src/flow/Hostname.actor.g.cpp"
			new (&static_cast<ResolveWithRetryImplActor*>(this)->SAV< NetworkAddress >::value()) NetworkAddress(address.get());
			this->~ResolveWithRetryImplActorState();
			static_cast<ResolveWithRetryImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(resolveInterval);
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 428 "/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 2;
															#line 66 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ResolveWithRetryImplActor, 1, Void >*>(static_cast<ResolveWithRetryImplActor*>(this)));
															#line 433 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<NetworkAddress> const& address,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(address, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<NetworkAddress> && address,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(address), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state > 0) static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 0;
		static_cast<ResolveWithRetryImplActor*>(this)->ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> const& value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*,Error err) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 67 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		resolveInterval = std::min(2 * resolveInterval, FLOW_KNOBS->HOSTNAME_RESOLVE_MAX_INTERVAL);
															#line 499 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 67 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		resolveInterval = std::min(2 * resolveInterval, FLOW_KNOBS->HOSTNAME_RESOLVE_MAX_INTERVAL);
															#line 508 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state > 0) static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 0;
		static_cast<ResolveWithRetryImplActor*>(this)->ActorCallback< ResolveWithRetryImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ResolveWithRetryImplActor, 1, Void >*,Void const& value) 
	{
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< ResolveWithRetryImplActor, 1, Void >*,Void && value) 
	{
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< ResolveWithRetryImplActor, 1, Void >*,Error err) 
	{
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}

	}
	int a_body1loopBody1cont6(int loopDepth) 
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
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	Hostname* self;
															#line 59 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	double resolveInterval;
															#line 587 "/src/flow/Hostname.actor.g.cpp"
};
// This generated class is to be used only via resolveWithRetryImpl()
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
class ResolveWithRetryImplActor final : public Actor<NetworkAddress>, public ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >, public ActorCallback< ResolveWithRetryImplActor, 1, Void >, public FastAllocated<ResolveWithRetryImplActor>, public ResolveWithRetryImplActorState<ResolveWithRetryImplActor> {
															#line 592 "/src/flow/Hostname.actor.g.cpp"
public:
	using FastAllocated<ResolveWithRetryImplActor>::operator new;
	using FastAllocated<ResolveWithRetryImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<NetworkAddress>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< ResolveWithRetryImplActor, 1, Void >;
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	ResolveWithRetryImplActor(Hostname* const& self) 
															#line 604 "/src/flow/Hostname.actor.g.cpp"
		 : Actor<NetworkAddress>(),
		   ResolveWithRetryImplActorState<ResolveWithRetryImplActor>(self)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resolveWithRetryImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ResolveWithRetryImplActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
[[nodiscard]] Future<NetworkAddress> resolveWithRetryImpl( Hostname* const& self ) {
															#line 58 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	return Future<NetworkAddress>(new ResolveWithRetryImplActor(self));
															#line 631 "/src/flow/Hostname.actor.g.cpp"
}

#line 74 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"

Future<Optional<NetworkAddress>> Hostname::resolve() {
	return resolveImpl(this);
}

Future<NetworkAddress> Hostname::resolveWithRetry() {
	return resolveWithRetryImpl(this);
}

Optional<NetworkAddress> Hostname::resolveBlocking() {
	try {
		std::vector<NetworkAddress> addresses =
		    INetworkConnections::net()->resolveTCPEndpointBlockingWithDNSCache(host, service);
		NetworkAddress address = INetworkConnections::pickOneAddress(addresses);
		address.flags = 0; // Reset the parsed address to public
		address.fromHostname = NetworkAddressFromHostname::True;
		if (isTLS) {
			address.flags |= NetworkAddress::FLAG_TLS;
		}
		return address;
	} catch (...) {
		return Optional<NetworkAddress>();
	}
}

															#line 660 "/src/flow/Hostname.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase99()
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
template <class FlowTestCase99Actor>
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
class FlowTestCase99ActorState {
															#line 667 "/src/flow/Hostname.actor.g.cpp"
public:
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	FlowTestCase99ActorState(UnitTestParameters const& params) 
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		 : params(params)
															#line 674 "/src/flow/Hostname.actor.g.cpp"
	{

	}
	~FlowTestCase99ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 100 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn1s = "localhost:1234";
															#line 101 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn2s = "host-name:1234";
															#line 102 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn3s = "host.name:1234";
															#line 103 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn4s = "host-name_part1.host-name_part2:1234:tls";
															#line 105 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn5s = "127.0.0.1:1234";
															#line 106 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn6s = "127.0.0.1:1234:tls";
															#line 107 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn7s = "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:4800";
															#line 108 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn8s = "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:4800:tls";
															#line 109 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn9s = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
															#line 110 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn10s = "2001:0db8:85a3:0000:0000:8a2e:0370:7334:tls";
															#line 111 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn11s = "[::1]:4800";
															#line 112 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn12s = "[::1]:4800:tls";
															#line 113 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			std::string hn13s = "1234";
															#line 115 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			auto hn1 = Hostname::parse(hn1s);
															#line 116 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn1.toString() == hn1s);
															#line 117 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn1.host == "localhost");
															#line 118 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn1.service == "1234");
															#line 119 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!hn1.isTLS);
															#line 121 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			hn2 = Hostname::parse(hn2s);
															#line 122 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn2.toString() == hn2s);
															#line 123 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn2.host == "host-name");
															#line 124 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn2.service == "1234");
															#line 125 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!hn2.isTLS);
															#line 127 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			auto hn3 = Hostname::parse(hn3s);
															#line 128 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn3.toString() == hn3s);
															#line 129 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn3.host == "host.name");
															#line 130 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn3.service == "1234");
															#line 131 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!hn3.isTLS);
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			auto hn4 = Hostname::parse(hn4s);
															#line 134 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn4.toString() == hn4s);
															#line 135 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn4.host == "host-name_part1.host-name_part2");
															#line 136 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn4.service == "1234");
															#line 137 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(hn4.isTLS);
															#line 139 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn5s));
															#line 140 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn6s));
															#line 141 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn7s));
															#line 142 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn8s));
															#line 143 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn9s));
															#line 144 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn10s));
															#line 145 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn11s));
															#line 146 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn12s));
															#line 147 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn13s));
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hn2.resolve();
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 773 "/src/flow/Hostname.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 1;
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >*>(static_cast<FlowTestCase99Actor*>(this)));
															#line 778 "/src/flow/Hostname.actor.g.cpp"
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
		this->~FlowTestCase99ActorState();
		static_cast<FlowTestCase99Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 150 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(!optionalAddress.present());
															#line 152 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = hn2.resolveBlocking();
															#line 153 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(!optionalAddress.present());
															#line 155 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		address = NetworkAddress();
															#line 807 "/src/flow/Hostname.actor.g.cpp"
		try {
															#line 157 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			StrictFuture<Void> __when_expr_1 = timeoutError(store(address, hn2.resolveWithRetry()), 1);
															#line 157 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 813 "/src/flow/Hostname.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 2;
															#line 157 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase99Actor, 1, Void >*>(static_cast<FlowTestCase99Actor*>(this)));
															#line 818 "/src/flow/Hostname.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> const& __optionalAddress,int loopDepth) 
	{
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = __optionalAddress;
															#line 833 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<NetworkAddress> && __optionalAddress,int loopDepth) 
	{
		optionalAddress = std::move(__optionalAddress);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase99Actor*>(this)->ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> const& value) 
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
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
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
	void a_callback_error(ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >*,Error err) 
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
	int a_body1cont2(int loopDepth) 
	{
															#line 161 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(address == NetworkAddress());
															#line 163 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		addressSource = NetworkAddress::parse("127.0.0.0:1234");
															#line 164 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		INetworkConnections::net()->addMockTCPEndpoint("host-name", "1234", { addressSource });
															#line 167 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_2 = store(optionalAddress, hn2.resolve());
															#line 167 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 902 "/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 3;
															#line 167 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase99Actor, 2, Void >*>(static_cast<FlowTestCase99Actor*>(this)));
															#line 907 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 159 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
			ASSERT(e.code() == error_code_timed_out);
															#line 917 "/src/flow/Hostname.actor.g.cpp"
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
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

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
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase99Actor*>(this)->ActorCallback< FlowTestCase99Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 1, Void >*,Void const& value) 
	{
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 1, Void >*,Void && value) 
	{
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< FlowTestCase99Actor, 1, Void >*,Error err) 
	{
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}

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
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 168 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 172 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = hn2.resolveBlocking();
															#line 173 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 174 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 177 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(address, hn2.resolveWithRetry());
															#line 177 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1026 "/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 4;
															#line 177 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase99Actor, 3, Void >*>(static_cast<FlowTestCase99Actor*>(this)));
															#line 1031 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 168 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 169 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 172 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = hn2.resolveBlocking();
															#line 173 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 174 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 177 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(address, hn2.resolveWithRetry());
															#line 177 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1052 "/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 4;
															#line 177 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase99Actor, 3, Void >*>(static_cast<FlowTestCase99Actor*>(this)));
															#line 1057 "/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase99Actor*>(this)->ActorCallback< FlowTestCase99Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 2, Void >*,Void const& value) 
	{
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 2, Void >*,Void && value) 
	{
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< FlowTestCase99Actor, 2, Void >*,Error err) 
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
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 178 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(address == addressSource);
															#line 180 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (!static_cast<FlowTestCase99Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase99ActorState(); static_cast<FlowTestCase99Actor*>(this)->destroy(); return 0; }
															#line 1125 "/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<FlowTestCase99Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase99ActorState();
		static_cast<FlowTestCase99Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 178 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		ASSERT(address == addressSource);
															#line 180 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
		if (!static_cast<FlowTestCase99Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase99ActorState(); static_cast<FlowTestCase99Actor*>(this)->destroy(); return 0; }
															#line 1139 "/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<FlowTestCase99Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase99ActorState();
		static_cast<FlowTestCase99Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<FlowTestCase99Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase99Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase99Actor*>(this)->ActorCallback< FlowTestCase99Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 3, Void >*,Void const& value) 
	{
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< FlowTestCase99Actor, 3, Void >*,Void && value) 
	{
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< FlowTestCase99Actor, 3, Void >*,Error err) 
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
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	UnitTestParameters params;
															#line 121 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	Hostname hn2;
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	Optional<NetworkAddress> optionalAddress;
															#line 155 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	NetworkAddress address;
															#line 163 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	NetworkAddress addressSource;
															#line 1214 "/src/flow/Hostname.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase99()
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
class FlowTestCase99Actor final : public Actor<Void>, public ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >, public ActorCallback< FlowTestCase99Actor, 1, Void >, public ActorCallback< FlowTestCase99Actor, 2, Void >, public ActorCallback< FlowTestCase99Actor, 3, Void >, public FastAllocated<FlowTestCase99Actor>, public FlowTestCase99ActorState<FlowTestCase99Actor> {
															#line 1219 "/src/flow/Hostname.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase99Actor>::operator new;
	using FastAllocated<FlowTestCase99Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< FlowTestCase99Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase99Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase99Actor, 3, Void >;
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	FlowTestCase99Actor(UnitTestParameters const& params) 
															#line 1233 "/src/flow/Hostname.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase99ActorState<FlowTestCase99Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase99");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase99Actor, 0, Optional<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase99Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase99Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase99Actor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
static Future<Void> flowTestCase99( UnitTestParameters const& params ) {
															#line 99 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
	return Future<Void>(new FlowTestCase99Actor(params));
															#line 1262 "/src/flow/Hostname.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase99, "/flow/Hostname/hostname")

#line 182 "/tmp/fdb_c/foundationdb_ep/flow/Hostname.actor.cpp"
