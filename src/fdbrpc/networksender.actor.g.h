#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
/*
 * networksender.actor.h
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

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FDBRPC_NETWORKSENDER_ACTOR_G_H)
#define FDBRPC_NETWORKSENDER_ACTOR_G_H
#include "fdbrpc/networksender.actor.g.h"
#elif !defined(RPCNETWORKSENDER_ACTOR_H)
#define RPCNETWORKSENDER_ACTOR_H

#include "fdbrpc/FlowTransport.h"
#include "flow/flow.h"
#include "flow/actorcompiler.h" // This must be the last #include.

// This actor is used by FlowTransport to serialize the response to a ReplyPromise across the network
															#line 36 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
namespace {
// This generated class is to be used only via networkSender()
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
template <class T, class NetworkSenderActor>
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
class NetworkSenderActorState {
															#line 43 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
public:
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
	NetworkSenderActorState(Future<T> const& input,Endpoint const& endpoint) 
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
		 : input(input),
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
		   endpoint(endpoint)
															#line 52 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
	{
		fdb_probe_actor_create("networkSender", reinterpret_cast<unsigned long>(this));

	}
	~NetworkSenderActorState() 
	{
		fdb_probe_actor_destroy("networkSender", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 37 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
				StrictFuture<T> __when_expr_0 = input;
															#line 37 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 70 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
				static_cast<NetworkSenderActor*>(this)->actor_wait_state = 1;
															#line 37 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< NetworkSenderActor, 0, T >*>(static_cast<NetworkSenderActor*>(this)));
															#line 74 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
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
		delete static_cast<NetworkSenderActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& err,int loopDepth=0) 
	{
		try {
															#line 41 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
			if (err.code() == error_code_never_reply)
															#line 109 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
			{
															#line 42 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
				delete static_cast<NetworkSenderActor*>(this);
															#line 113 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
				return 0;
			}
															#line 44 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
			ASSERT(err.code() != error_code_actor_cancelled);
															#line 45 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
			FlowTransport::transport().sendUnreliable(SerializeSource<ErrorOr<EnsureTable<T>>>(err), endpoint, false);
															#line 120 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(T const& value,int loopDepth) 
	{
															#line 38 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
		FlowTransport::transport().sendUnreliable(SerializeSource<ErrorOr<EnsureTable<T>>>(value), endpoint, false);
															#line 135 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(T && value,int loopDepth) 
	{
															#line 38 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
		FlowTransport::transport().sendUnreliable(SerializeSource<ErrorOr<EnsureTable<T>>>(value), endpoint, false);
															#line 144 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(T const& value,int loopDepth) 
	{
		loopDepth = a_body1cont2(value, loopDepth);

		return loopDepth;
	}
	int a_body1when1(T && value,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<NetworkSenderActor*>(this)->actor_wait_state > 0) static_cast<NetworkSenderActor*>(this)->actor_wait_state = 0;
		static_cast<NetworkSenderActor*>(this)->ActorCallback< NetworkSenderActor, 0, T >::remove();

	}
	void a_callback_fire(ActorCallback< NetworkSenderActor, 0, T >*,T const& value) 
	{
		fdb_probe_actor_enter("networkSender", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("networkSender", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< NetworkSenderActor, 0, T >*,T && value) 
	{
		fdb_probe_actor_enter("networkSender", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("networkSender", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< NetworkSenderActor, 0, T >*,Error err) 
	{
		fdb_probe_actor_enter("networkSender", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("networkSender", reinterpret_cast<unsigned long>(this), 0);

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
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
		delete static_cast<NetworkSenderActor*>(this);
															#line 229 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
	Future<T> input;
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
	Endpoint endpoint;
															#line 238 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
};
// This generated class is to be used only via networkSender()
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
template <class T>
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
class NetworkSenderActor final : public Actor<void>, public ActorCallback< NetworkSenderActor<T>, 0, T >, public FastAllocated<NetworkSenderActor<T>>, public NetworkSenderActorState<T, NetworkSenderActor<T>> {
															#line 245 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
public:
	using FastAllocated<NetworkSenderActor<T>>::operator new;
	using FastAllocated<NetworkSenderActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< NetworkSenderActor<T>, 0, T >;
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
	NetworkSenderActor(Future<T> const& input,Endpoint const& endpoint) 
															#line 256 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
		 : Actor<void>(),
		   NetworkSenderActorState<T, NetworkSenderActor<T>>(input, endpoint)
	{
		fdb_probe_actor_enter("networkSender", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("networkSender");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("networkSender", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
template <class T>
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
void networkSender( Future<T> const& input, Endpoint const& endpoint ) {
															#line 34 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
	new NetworkSenderActor<T>(input, endpoint);
															#line 277 "/usr/src/libfdb_c/fdbrpc/networksender.actor.g.h"
}

#line 48 "/usr/src/libfdb_c/fdbrpc/networksender.actor.h"
#include "flow/unactorcompiler.h"

#endif
