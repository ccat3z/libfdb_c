#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
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

#include <regex>

#include "flow/IConnection.h"
#include "flow/UnitTest.h"

#include "flow/actorcompiler.h" // has to be last include

namespace {

const static std::regex validation("^([\\w\\-]+\\.?)+:([\\d]+){1,}(:tls)?$");
const static std::regex ipv4Validation("^([\\d]{1,3}\\.?){4,}:([\\d]+){1,}(:tls)?$");

} // anonymous namespace

bool Hostname::isHostname(const std::string& str) {
	try {
		return !std::regex_match(str, ipv4Validation) && std::regex_match(str, validation);
	} catch (std::exception e) {
		TraceEvent(SevWarn, "AddressParseError").detail("StdException", e.what()).detail("String", str);
		throw address_parse_error();
	}
}

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

															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
namespace {
// This generated class is to be used only via resolveImpl()
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
template <class ResolveImplActor>
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
class ResolveImplActorState {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
public:
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	ResolveImplActorState(const Hostname* const& self) 
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		 : self(self)
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
	{
		fdb_probe_actor_create("resolveImpl", reinterpret_cast<unsigned long>(this));

	}
	~ResolveImplActorState() 
	{
		fdb_probe_actor_destroy("resolveImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
				StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = INetworkConnections::net()->resolveTCPEndpointWithDNSCache(self->host, self->service);
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
				if (static_cast<ResolveImplActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ResolveImplActor*>(this)->actor_wait_state = 1;
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*>(static_cast<ResolveImplActor*>(this)));
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			if (!static_cast<ResolveImplActor*>(this)->SAV<Optional<NetworkAddress>>::futures) { (void)(Optional<NetworkAddress>()); this->~ResolveImplActorState(); static_cast<ResolveImplActor*>(this)->destroy(); return 0; }
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		NetworkAddress address = INetworkConnections::pickOneAddress(addresses);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		address.flags = 0;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		address.fromHostname = NetworkAddressFromHostname::True;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (self->isTLS)
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			address.flags |= NetworkAddress::FLAG_TLS;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		}
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (!static_cast<ResolveImplActor*>(this)->SAV<Optional<NetworkAddress>>::futures) { (void)(address); this->~ResolveImplActorState(); static_cast<ResolveImplActor*>(this)->destroy(); return 0; }
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<ResolveImplActor*>(this)->SAV< Optional<NetworkAddress> >::value()) Optional<NetworkAddress>(address);
		this->~ResolveImplActorState();
		static_cast<ResolveImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<NetworkAddress> && addresses,int loopDepth) 
	{
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		NetworkAddress address = INetworkConnections::pickOneAddress(addresses);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		address.flags = 0;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		address.fromHostname = NetworkAddressFromHostname::True;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (self->isTLS)
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			address.flags |= NetworkAddress::FLAG_TLS;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		}
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (!static_cast<ResolveImplActor*>(this)->SAV<Optional<NetworkAddress>>::futures) { (void)(address); this->~ResolveImplActorState(); static_cast<ResolveImplActor*>(this)->destroy(); return 0; }
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		fdb_probe_actor_enter("resolveImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("resolveImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("resolveImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	const Hostname* self;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
};
// This generated class is to be used only via resolveImpl()
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
class ResolveImplActor final : public Actor<Optional<NetworkAddress>>, public ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >, public FastAllocated<ResolveImplActor>, public ResolveImplActorState<ResolveImplActor> {
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
public:
	using FastAllocated<ResolveImplActor>::operator new;
	using FastAllocated<ResolveImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveImplActor, 0, std::vector<NetworkAddress> >;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	ResolveImplActor(const Hostname* const& self) 
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		 : Actor<Optional<NetworkAddress>>(),
		   ResolveImplActorState<ResolveImplActor>(self)
	{
		fdb_probe_actor_enter("resolveImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resolveImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("resolveImpl", reinterpret_cast<unsigned long>(this), -1);

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
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
[[nodiscard]] Future<Optional<NetworkAddress>> resolveImpl( const Hostname* const& self ) {
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	return Future<Optional<NetworkAddress>>(new ResolveImplActor(self));
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
}

#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"

															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
namespace {
// This generated class is to be used only via resolveWithRetryImpl()
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
template <class ResolveWithRetryImplActor>
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
class ResolveWithRetryImplActorState {
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
public:
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	ResolveWithRetryImplActorState(const Hostname* const& self) 
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		 : self(self),
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		   resolveInterval(FLOW_KNOBS->HOSTNAME_RESOLVE_INIT_INTERVAL)
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
	{
		fdb_probe_actor_create("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this));

	}
	~ResolveWithRetryImplActorState() 
	{
		fdb_probe_actor_destroy("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = resolveImpl(self);
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 1;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*>(static_cast<ResolveWithRetryImplActor*>(this)));
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(e.code() == error_code_actor_cancelled);
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (address.present())
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			if (!static_cast<ResolveWithRetryImplActor*>(this)->SAV<NetworkAddress>::futures) { (void)(address.get()); this->~ResolveWithRetryImplActorState(); static_cast<ResolveWithRetryImplActor*>(this)->destroy(); return 0; }
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
			new (&static_cast<ResolveWithRetryImplActor*>(this)->SAV< NetworkAddress >::value()) NetworkAddress(address.get());
			this->~ResolveWithRetryImplActorState();
			static_cast<ResolveWithRetryImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(resolveInterval);
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 2;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ResolveWithRetryImplActor, 1, Void >*>(static_cast<ResolveWithRetryImplActor*>(this)));
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<NetworkAddress> && address,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (address.present())
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			if (!static_cast<ResolveWithRetryImplActor*>(this)->SAV<NetworkAddress>::futures) { (void)(address.get()); this->~ResolveWithRetryImplActorState(); static_cast<ResolveWithRetryImplActor*>(this)->destroy(); return 0; }
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
			new (&static_cast<ResolveWithRetryImplActor*>(this)->SAV< NetworkAddress >::value()) NetworkAddress(address.get());
			this->~ResolveWithRetryImplActorState();
			static_cast<ResolveWithRetryImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(resolveInterval);
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ResolveWithRetryImplActor*>(this)->actor_wait_state = 2;
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ResolveWithRetryImplActor, 1, Void >*>(static_cast<ResolveWithRetryImplActor*>(this)));
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		resolveInterval = std::min(2 * resolveInterval, FLOW_KNOBS->HOSTNAME_RESOLVE_MAX_INTERVAL);
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		resolveInterval = std::min(2 * resolveInterval, FLOW_KNOBS->HOSTNAME_RESOLVE_MAX_INTERVAL);
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ResolveWithRetryImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ResolveWithRetryImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), 1);

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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	const Hostname* self;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	double resolveInterval;
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
};
// This generated class is to be used only via resolveWithRetryImpl()
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
class ResolveWithRetryImplActor final : public Actor<NetworkAddress>, public ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >, public ActorCallback< ResolveWithRetryImplActor, 1, Void >, public FastAllocated<ResolveWithRetryImplActor>, public ResolveWithRetryImplActorState<ResolveWithRetryImplActor> {
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
public:
	using FastAllocated<ResolveWithRetryImplActor>::operator new;
	using FastAllocated<ResolveWithRetryImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<NetworkAddress>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ResolveWithRetryImplActor, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< ResolveWithRetryImplActor, 1, Void >;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	ResolveWithRetryImplActor(const Hostname* const& self) 
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		 : Actor<NetworkAddress>(),
		   ResolveWithRetryImplActorState<ResolveWithRetryImplActor>(self)
	{
		fdb_probe_actor_enter("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("resolveWithRetryImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("resolveWithRetryImpl", reinterpret_cast<unsigned long>(this), -1);

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
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
[[nodiscard]] Future<NetworkAddress> resolveWithRetryImpl( const Hostname* const& self ) {
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	return Future<NetworkAddress>(new ResolveWithRetryImplActor(self));
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
}

#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"

Future<Optional<NetworkAddress>> Hostname::resolve() {
	return resolveImpl(this);
}

Future<NetworkAddress> Hostname::resolveWithRetry() const {
	return resolveWithRetryImpl(this);
}

Optional<NetworkAddress> Hostname::resolveBlocking() const {
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

															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase120()
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
template <class FlowTestCase120Actor>
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
class FlowTestCase120ActorState {
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
public:
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	FlowTestCase120ActorState(UnitTestParameters const& params) 
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		 : params(params)
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase120", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase120ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase120", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn1s = "localhost:1234";
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn2s = "host-name:1234";
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn3s = "host.name:1234";
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn4s = "host-name_part1.host-name_part2:1234:tls";
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn5s = "127.0.0.1:1234";
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn6s = "127.0.0.1:1234:tls";
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn7s = "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:4800";
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn8s = "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:4800:tls";
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn9s = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn10s = "2001:0db8:85a3:0000:0000:8a2e:0370:7334:tls";
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn11s = "[::1]:4800";
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn12s = "[::1]:4800:tls";
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			std::string hn13s = "1234";
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			auto hn1 = Hostname::parse(hn1s);
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn1.toString() == hn1s);
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn1.host == "localhost");
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn1.service == "1234");
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!hn1.isTLS);
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			hn2 = Hostname::parse(hn2s);
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn2.toString() == hn2s);
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn2.host == "host-name");
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn2.service == "1234");
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!hn2.isTLS);
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			auto hn3 = Hostname::parse(hn3s);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn3.toString() == hn3s);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn3.host == "host.name");
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn3.service == "1234");
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!hn3.isTLS);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			auto hn4 = Hostname::parse(hn4s);
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn4.toString() == hn4s);
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn4.host == "host-name_part1.host-name_part2");
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn4.service == "1234");
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(hn4.isTLS);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn5s));
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn6s));
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn7s));
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn8s));
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn9s));
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn10s));
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn11s));
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn12s));
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(!Hostname::isHostname(hn13s));
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			StrictFuture<Optional<NetworkAddress>> __when_expr_0 = hn2.resolve();
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 1;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >*>(static_cast<FlowTestCase120Actor*>(this)));
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		this->~FlowTestCase120ActorState();
		static_cast<FlowTestCase120Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(!optionalAddress.present());
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = hn2.resolveBlocking();
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(!optionalAddress.present());
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		address = NetworkAddress();
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		try {
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			StrictFuture<Void> __when_expr_1 = timeoutError(store(address, hn2.resolveWithRetry()), 1);
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 2;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase120Actor, 1, Void >*>(static_cast<FlowTestCase120Actor*>(this)));
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = __optionalAddress;
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase120Actor*>(this)->ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >*,Optional<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(address == NetworkAddress());
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		addressSource = NetworkAddress::parse("127.0.0.0:1234");
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		INetworkConnections::net()->addMockTCPEndpoint("host-name", "1234", { addressSource });
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_2 = store(optionalAddress, hn2.resolve());
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 3;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase120Actor, 2, Void >*>(static_cast<FlowTestCase120Actor*>(this)));
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
			ASSERT(e.code() == error_code_timed_out);
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase120Actor*>(this)->ActorCallback< FlowTestCase120Actor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< FlowTestCase120Actor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 1);

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
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = hn2.resolveBlocking();
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(address, hn2.resolveWithRetry());
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 4;
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase120Actor, 3, Void >*>(static_cast<FlowTestCase120Actor*>(this)));
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = hn2.resolveBlocking();
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(optionalAddress.present() && optionalAddress.get() == addressSource);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		optionalAddress = Optional<NetworkAddress>();
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(address, hn2.resolveWithRetry());
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 4;
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase120Actor, 3, Void >*>(static_cast<FlowTestCase120Actor*>(this)));
															#line 1118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
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
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase120Actor*>(this)->ActorCallback< FlowTestCase120Actor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< FlowTestCase120Actor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(address == addressSource);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (!static_cast<FlowTestCase120Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase120ActorState(); static_cast<FlowTestCase120Actor*>(this)->destroy(); return 0; }
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<FlowTestCase120Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase120ActorState();
		static_cast<FlowTestCase120Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		ASSERT(address == addressSource);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
		if (!static_cast<FlowTestCase120Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase120ActorState(); static_cast<FlowTestCase120Actor*>(this)->destroy(); return 0; }
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		new (&static_cast<FlowTestCase120Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase120ActorState();
		static_cast<FlowTestCase120Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase120Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase120Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase120Actor*>(this)->ActorCallback< FlowTestCase120Actor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< FlowTestCase120Actor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< FlowTestCase120Actor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	UnitTestParameters params;
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	Hostname hn2;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	Optional<NetworkAddress> optionalAddress;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	NetworkAddress address;
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	NetworkAddress addressSource;
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase120()
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
class FlowTestCase120Actor final : public Actor<Void>, public ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >, public ActorCallback< FlowTestCase120Actor, 1, Void >, public ActorCallback< FlowTestCase120Actor, 2, Void >, public ActorCallback< FlowTestCase120Actor, 3, Void >, public FastAllocated<FlowTestCase120Actor>, public FlowTestCase120ActorState<FlowTestCase120Actor> {
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase120Actor>::operator new;
	using FastAllocated<FlowTestCase120Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >;
friend struct ActorCallback< FlowTestCase120Actor, 1, Void >;
friend struct ActorCallback< FlowTestCase120Actor, 2, Void >;
friend struct ActorCallback< FlowTestCase120Actor, 3, Void >;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	FlowTestCase120Actor(UnitTestParameters const& params) 
															#line 1306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase120ActorState<FlowTestCase120Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase120", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase120");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase120", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase120Actor, 0, Optional<NetworkAddress> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< FlowTestCase120Actor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< FlowTestCase120Actor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< FlowTestCase120Actor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
static Future<Void> flowTestCase120( UnitTestParameters const& params ) {
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
	return Future<Void>(new FlowTestCase120Actor(params));
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase120, "/flow/Hostname/hostname")

#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/Hostname.actor.cpp"
