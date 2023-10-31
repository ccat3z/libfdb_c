#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
/*
 * RESTUtils.actor.cpp
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

#include "fdbclient/RESTUtils.h"
#include "fdbclient/Knobs.h"

#include "flow/flat_buffers.h"
#include "flow/UnitTest.h"
#include "flow/IConnection.h"

#include <boost/algorithm/string.hpp>
#include <queue>

#include "flow/actorcompiler.h" // always the last include

const std::unordered_map<std::string, RESTConnectionType> RESTConnectionType::supportedConnTypes = {
	{ "http", RESTConnectionType("http", RESTConnectionType::NOT_SECURE_CONNECTION) },
	{ "https", RESTConnectionType("https", RESTConnectionType::SECURE_CONNECTION) }
};

RESTConnectionType RESTConnectionType::getConnectionType(const std::string& protocol) {
	auto itr = RESTConnectionType::supportedConnTypes.find(protocol);
	if (itr == RESTConnectionType::supportedConnTypes.end()) {
		TraceEvent("RESTConnectionTypeUnsupportedPrototocol").detail("Protocol", protocol);
		CODE_PROBE(true, "REST URI unsupported protocol");
		throw rest_unsupported_protocol();
	}
	return itr->second;
}

bool RESTConnectionType::isProtocolSupported(const std::string& protocol) {
	auto itr = RESTConnectionType::supportedConnTypes.find(protocol);
	return itr != RESTConnectionType::supportedConnTypes.end();
}

bool RESTConnectionType::isSecure(const std::string& protocol) {
	auto itr = RESTConnectionType::supportedConnTypes.find(protocol);
	if (itr == RESTConnectionType::supportedConnTypes.end()) {
		TraceEvent("RESTConnectionTypeUnsupportedPrototocol").detail("Protocol", protocol);
		throw rest_unsupported_protocol();
	}
	return itr->second.secure == RESTConnectionType::SECURE_CONNECTION;
}

RESTClientKnobs::RESTClientKnobs() {
	connection_pool_size = FLOW_KNOBS->RESTCLIENT_MAX_CONNECTIONPOOL_SIZE;
	connect_tries = FLOW_KNOBS->RESTCLIENT_CONNECT_TRIES;
	connect_timeout = FLOW_KNOBS->RESTCLIENT_CONNECT_TIMEOUT;
	max_connection_life = FLOW_KNOBS->RESTCLIENT_MAX_CONNECTION_LIFE;
	request_tries = FLOW_KNOBS->RESTCLIENT_REQUEST_TRIES;
	request_timeout_secs = FLOW_KNOBS->RESTCLIENT_REQUEST_TIMEOUT_SEC;

	knobMap["connection_pool_size"] = std::addressof(connection_pool_size);
	knobMap["pz"] = std::addressof(connection_pool_size);
	knobMap["connect_tries"] = std::addressof(connect_tries);
	knobMap["ct"] = std::addressof(connect_tries);
	knobMap["connect_timeout"] = std::addressof(connect_timeout);
	knobMap["cto"] = std::addressof(connect_timeout);
	knobMap["max_connection_life"] = std::addressof(max_connection_life);
	knobMap["mcl"] = std::addressof(max_connection_life);
	knobMap["request_tries"] = std::addressof(request_tries);
	knobMap["rt"] = std::addressof(request_tries);
	knobMap["request_timeout_secs"] = std::addressof(request_timeout_secs);
	knobMap["rtom"] = std::addressof(request_timeout_secs);
}

void RESTClientKnobs::set(const std::unordered_map<std::string, int>& knobSettings) {
	TraceEvent trace = TraceEvent("RESTClientSetKnobs");

	for (const auto& itr : knobSettings) {
		const auto& kItr = RESTClientKnobs::knobMap.find(itr.first);
		if (kItr == RESTClientKnobs::knobMap.end()) {
			trace.detail("RESTClientInvalidKnobName", itr.first);
			throw rest_invalid_rest_client_knob();
		}
		ASSERT_EQ(itr.first.compare(kItr->first), 0);
		*(kItr->second) = itr.second;
		trace.detail(itr.first.c_str(), itr.second);
	}
}

std::unordered_map<std::string, int> RESTClientKnobs::get() const {
	std::unordered_map<std::string, int> details = {
		{ "connection_pool_size", connection_pool_size },
		{ "connect_tries", connect_tries },
		{ "connect_timeout", connect_timeout },
		{ "max_connection_life", max_connection_life },
		{ "request_tries", request_tries },
		{ "request_timeout_secs", request_timeout_secs },
	};

	return details;
}

															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via connect_impl()
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class Connect_implActor>
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class Connect_implActorState {
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	Connect_implActorState(Reference<RESTConnectionPool> const& connectionPool,RESTConnectionPoolKey const& connectKey,bool const& isSecure,int const& maxConnLife) 
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : connectionPool(connectionPool),
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		   connectKey(connectKey),
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		   isSecure(isSecure),
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		   maxConnLife(maxConnLife)
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("connect_impl", reinterpret_cast<unsigned long>(this));

	}
	~Connect_implActorState() 
	{
		fdb_probe_actor_destroy("connect_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::VERBOSE)
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			{
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				TraceEvent("RESTUtilConnectStart") .detail("Host", connectKey.first) .detail("Service", connectKey.second) .detail("IsSecure", isSecure) .detail("ConnectPoolNumKeys", connectionPool->connectionPoolMap.size());
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			}
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			auto poolItr = connectionPool->connectionPoolMap.find(connectKey);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			for(;poolItr != connectionPool->connectionPoolMap.end() && !poolItr->second.empty();) {
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				RESTConnectionPool::ReusableConnection rconn = poolItr->second.front();
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				poolItr->second.pop();
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				if (rconn.expirationTime > now())
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
				{
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
					if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::DEBUG)
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
					{
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
						TraceEvent("RESTUtilReuseConn") .detail("Host", connectKey.first) .detail("Service", connectKey.second) .detail("RemoteEndpoint", rconn.conn->getPeerAddress()) .detail("ExpireIn", rconn.expirationTime - now()) .detail("NumConnsInPool", poolItr->second.size());
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
					}
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
					if (!static_cast<Connect_implActor*>(this)->SAV<RESTConnectionPool::ReusableConnection>::futures) { (void)(rconn); this->~Connect_implActorState(); static_cast<Connect_implActor*>(this)->destroy(); return 0; }
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
					new (&static_cast<Connect_implActor*>(this)->SAV< RESTConnectionPool::ReusableConnection >::value()) RESTConnectionPool::ReusableConnection(rconn);
					this->~Connect_implActorState();
					static_cast<Connect_implActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT(poolItr == connectionPool->connectionPoolMap.end() || poolItr->second.empty());
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_0 = INetworkConnections::net()->connect(connectKey.first, connectKey.second, isSecure);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (static_cast<Connect_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Connect_implActor*>(this)->actor_wait_state = 1;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Connect_implActor, 0, Reference<IConnection> >*>(static_cast<Connect_implActor*>(this)));
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
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
		this->~Connect_implActorState();
		static_cast<Connect_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->connectHandshake();
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		if (static_cast<Connect_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Connect_implActor*>(this)->actor_wait_state = 2;
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Connect_implActor, 1, Void >*>(static_cast<Connect_implActor*>(this)));
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IConnection> const& __conn,int loopDepth) 
	{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		conn = __conn;
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IConnection> && __conn,int loopDepth) 
	{
		conn = std::move(__conn);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Connect_implActor*>(this)->actor_wait_state > 0) static_cast<Connect_implActor*>(this)->actor_wait_state = 0;
		static_cast<Connect_implActor*>(this)->ActorCallback< Connect_implActor, 0, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< Connect_implActor, 0, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Connect_implActor, 0, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Connect_implActor, 0, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		TraceEvent("RESTTUilCreateNewConn") .suppressFor(60) .detail("Host", connectKey.first) .detail("Service", connectKey.second) .detail("RemoteEndpoint", conn->getPeerAddress()) .detail("ConnPoolSize", connectionPool->connectionPoolMap.size());
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		if (!static_cast<Connect_implActor*>(this)->SAV<RESTConnectionPool::ReusableConnection>::futures) { (void)(RESTConnectionPool::ReusableConnection({ conn, now() + maxConnLife })); this->~Connect_implActorState(); static_cast<Connect_implActor*>(this)->destroy(); return 0; }
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		new (&static_cast<Connect_implActor*>(this)->SAV< RESTConnectionPool::ReusableConnection >::value()) RESTConnectionPool::ReusableConnection(RESTConnectionPool::ReusableConnection({ conn, now() + maxConnLife }));
		this->~Connect_implActorState();
		static_cast<Connect_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		TraceEvent("RESTTUilCreateNewConn") .suppressFor(60) .detail("Host", connectKey.first) .detail("Service", connectKey.second) .detail("RemoteEndpoint", conn->getPeerAddress()) .detail("ConnPoolSize", connectionPool->connectionPoolMap.size());
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		if (!static_cast<Connect_implActor*>(this)->SAV<RESTConnectionPool::ReusableConnection>::futures) { (void)(RESTConnectionPool::ReusableConnection({ conn, now() + maxConnLife })); this->~Connect_implActorState(); static_cast<Connect_implActor*>(this)->destroy(); return 0; }
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		new (&static_cast<Connect_implActor*>(this)->SAV< RESTConnectionPool::ReusableConnection >::value()) RESTConnectionPool::ReusableConnection(RESTConnectionPool::ReusableConnection({ conn, now() + maxConnLife }));
		this->~Connect_implActorState();
		static_cast<Connect_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Connect_implActor*>(this)->actor_wait_state > 0) static_cast<Connect_implActor*>(this)->actor_wait_state = 0;
		static_cast<Connect_implActor*>(this)->ActorCallback< Connect_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Connect_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Connect_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Connect_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	Reference<RESTConnectionPool> connectionPool;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	RESTConnectionPoolKey connectKey;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	bool isSecure;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	int maxConnLife;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	Reference<IConnection> conn;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via connect_impl()
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class Connect_implActor final : public Actor<RESTConnectionPool::ReusableConnection>, public ActorCallback< Connect_implActor, 0, Reference<IConnection> >, public ActorCallback< Connect_implActor, 1, Void >, public FastAllocated<Connect_implActor>, public Connect_implActorState<Connect_implActor> {
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<Connect_implActor>::operator new;
	using FastAllocated<Connect_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RESTConnectionPool::ReusableConnection>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Connect_implActor, 0, Reference<IConnection> >;
friend struct ActorCallback< Connect_implActor, 1, Void >;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	Connect_implActor(Reference<RESTConnectionPool> const& connectionPool,RESTConnectionPoolKey const& connectKey,bool const& isSecure,int const& maxConnLife) 
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<RESTConnectionPool::ReusableConnection>(),
		   Connect_implActorState<Connect_implActor>(connectionPool, connectKey, isSecure, maxConnLife)
	{
		fdb_probe_actor_enter("connect_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connect_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connect_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Connect_implActor, 0, Reference<IConnection> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Connect_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
[[nodiscard]] Future<RESTConnectionPool::ReusableConnection> connect_impl( Reference<RESTConnectionPool> const& connectionPool, RESTConnectionPoolKey const& connectKey, bool const& isSecure, int const& maxConnLife ) {
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<RESTConnectionPool::ReusableConnection>(new Connect_implActor(connectionPool, connectKey, isSecure, maxConnLife));
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}

#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

Future<RESTConnectionPool::ReusableConnection> RESTConnectionPool::connect(RESTConnectionPoolKey connectKey,
                                                                           const bool isSecure,
                                                                           const int maxConnLife) {
	return connect_impl(Reference<RESTConnectionPool>::addRef(this), connectKey, isSecure, maxConnLife);
}

void RESTConnectionPool::returnConnection(RESTConnectionPoolKey connectKey,
                                          ReusableConnection& rconn,
                                          const int maxConnections) {
	if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::VERBOSE) {
		TraceEvent("RESTUtilReturnConnStart")
		    .detail("Host", connectKey.first)
		    .detail("Service", connectKey.second)
		    .detail("ConnectPoolNumKeys", connectionPoolMap.size());
	}

	auto poolItr = connectionPoolMap.find(connectKey);
	// If it expires in the future then add it to the pool in the front iff connection pool size is not maxed
	if (rconn.expirationTime > now()) {
		bool returned = true;
		if (poolItr == connectionPoolMap.end()) {
			connectionPoolMap.insert({ connectKey, std::queue<RESTConnectionPool::ReusableConnection>({ rconn }) });
		} else if (poolItr->second.size() < maxConnections) {
			poolItr->second.push(rconn);
		} else {
			// Connection pool at its capacity; do nothing
			returned = false;
		}

		if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::DEBUG && returned) {
			poolItr = connectionPoolMap.find(connectKey);
			TraceEvent("RESTUtilReturnConnToPool")
			    .detail("Host", connectKey.first)
			    .detail("Service", connectKey.second)
			    .detail("ConnPoolSize", connectionPoolMap.size())
			    .detail("CachedConns", poolItr->second.size())
			    .detail("TimeToExpire", rconn.expirationTime - now());
		}
	}
	rconn.conn = Reference<IConnection>();
}

RESTUrl::RESTUrl(const std::string& fUrl) {
	parseUrl(fUrl);
}

RESTUrl::RESTUrl(const std::string& fullUrl, const std::string& b) : body(b) {
	parseUrl(fullUrl);
}

void RESTUrl::parseUrl(const std::string& fullUrl) {
	// Sample valid URIs
	// 1. With 'host' & 'resource' := '<protocol>://<host>/<resource>'
	// 2. With 'host', 'service' & 'resource' := '<protocol>://<host>:port/<resource>'
	// 3. With 'host', 'service', 'resource' & 'reqParameters' := '<protocol>://<host>:port/<resource>?<parameter-list>'

	if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::VERBOSE) {
		TraceEvent("RESTParseURI").detail("URI", fullUrl);
	}

	try {
		StringRef t(fullUrl);
		StringRef p = t.eat("://");
		std::string protocol = p.toString();
		boost::algorithm::to_lower(protocol);
		this->connType = RESTConnectionType::getConnectionType(protocol);
		if (!this->connType.secure && !CLIENT_KNOBS->REST_KMS_ALLOW_NOT_SECURE_CONNECTION) {
			TraceEvent(SevWarnAlways, "RESTUtilsUnSupportedNotSecureConn").detail("Protocol", protocol);
			CODE_PROBE(true, "REST URI not-secure connection not supported");
			throw rest_unsupported_protocol();
		}

		// extract 'resource' and optional 'parameter list' if supplied in the URL
		uint8_t foundSeparator = 0;
		StringRef hostPort = t.eatAny("/?", &foundSeparator);
		this->resource = "/";
		if (foundSeparator == '/') {
			this->resource += t.eat("?").toString();
			this->reqParameters = t.eat().toString();
		}

		// hostPort is at least a host or IP address, optionally followed by :portNumber or :serviceName
		StringRef hRef(hostPort);
		StringRef h = hRef.eat(":");
		if (h.size() == 0) {
			CODE_PROBE(true, "REST URI empty host");
			throw std::string("host cannot be empty");
		}
		this->host = h.toString();
		this->service = hRef.eat().toString();

		if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::DEBUG) {
			TraceEvent("RESTUtilParseURI")
			    .detail("URI", fullUrl)
			    .detail("Host", this->host)
			    .detail("Service", this->service)
			    .detail("Resource", this->resource)
			    .detail("ReqParameters", this->reqParameters)
			    .detail("ConnectionType", this->connType.toString());
		}
	} catch (std::string& err) {
		TraceEvent(SevWarnAlways, "RESTUtilParseError").detail("URI", fullUrl).detail("Error", err);
		throw rest_invalid_uri();
	}
}

// Only used to link unit tests
void forceLinkRESTUtilsTests() {}

															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase269()
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase269Actor>
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase269ActorState {
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase269ActorState(UnitTestParameters const& params) 
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase269", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase269ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase269", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				std::string uri("httpx://foo/bar");
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				RESTUrl r(uri);
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				ASSERT(false);
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
				loopDepth = a_body1cont3(loopDepth);
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
		this->~FlowTestCase269ActorState();
		static_cast<FlowTestCase269Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		if (!static_cast<FlowTestCase269Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase269ActorState(); static_cast<FlowTestCase269Actor*>(this)->destroy(); return 0; }
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		new (&static_cast<FlowTestCase269Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase269ActorState();
		static_cast<FlowTestCase269Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (e.code() != error_code_rest_unsupported_protocol)
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			{
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
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
	int a_body1cont3(int loopDepth) 
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
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase269()
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase269Actor final : public Actor<Void>, public FastAllocated<FlowTestCase269Actor>, public FlowTestCase269ActorState<FlowTestCase269Actor> {
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase269Actor>::operator new;
	using FastAllocated<FlowTestCase269Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase269Actor(UnitTestParameters const& params) 
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase269ActorState<FlowTestCase269Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase269", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase269");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase269", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase269( UnitTestParameters const& params ) {
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase269Actor(params));
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase269, "/RESTUtils/InvalidProtocol")

#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase282()
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase282Actor>
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase282ActorState {
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase282ActorState(UnitTestParameters const& params) 
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase282", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase282ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase282", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				std::string uri("https://:/bar");
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				RESTUrl r(uri);
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				ASSERT(false);
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
				loopDepth = a_body1cont3(loopDepth);
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
		this->~FlowTestCase282ActorState();
		static_cast<FlowTestCase282Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		if (!static_cast<FlowTestCase282Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase282ActorState(); static_cast<FlowTestCase282Actor*>(this)->destroy(); return 0; }
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		new (&static_cast<FlowTestCase282Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase282ActorState();
		static_cast<FlowTestCase282Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (e.code() != error_code_rest_invalid_uri)
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			{
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
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
	int a_body1cont3(int loopDepth) 
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
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase282()
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase282Actor final : public Actor<Void>, public FastAllocated<FlowTestCase282Actor>, public FlowTestCase282ActorState<FlowTestCase282Actor> {
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase282Actor>::operator new;
	using FastAllocated<FlowTestCase282Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase282Actor(UnitTestParameters const& params) 
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase282ActorState<FlowTestCase282Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase282", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase282");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase282", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase282( UnitTestParameters const& params ) {
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase282Actor(params));
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase282, "/RESTUtils/MissingHost")

#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase295()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase295Actor>
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase295ActorState {
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase295ActorState(UnitTestParameters const& params) 
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase295", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase295ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase295", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			std::string uri("https://host:80/foo/bar");
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			RESTUrl r(uri);
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.connType.secure, RESTConnectionType::SECURE_CONNECTION);
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.host.compare("host"), 0);
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.service.compare("80"), 0);
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.resource.compare("/foo/bar"), 0);
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (!static_cast<FlowTestCase295Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase295ActorState(); static_cast<FlowTestCase295Actor*>(this)->destroy(); return 0; }
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			new (&static_cast<FlowTestCase295Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase295ActorState();
			static_cast<FlowTestCase295Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase295ActorState();
		static_cast<FlowTestCase295Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase295()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase295Actor final : public Actor<Void>, public FastAllocated<FlowTestCase295Actor>, public FlowTestCase295ActorState<FlowTestCase295Actor> {
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase295Actor>::operator new;
	using FastAllocated<FlowTestCase295Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase295Actor(UnitTestParameters const& params) 
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase295ActorState<FlowTestCase295Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase295", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase295");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase295", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase295( UnitTestParameters const& params ) {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase295Actor(params));
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase295, "/RESTUtils/ValidURIWithService")

#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase305()
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase305Actor>
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase305ActorState {
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase305ActorState(UnitTestParameters const& params) 
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase305", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase305ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase305", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			std::string uri("https://host/foo/bar");
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			RESTUrl r(uri);
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.connType.secure, RESTConnectionType::SECURE_CONNECTION);
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.host.compare("host"), 0);
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT(r.service.empty());
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.resource.compare("/foo/bar"), 0);
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (!static_cast<FlowTestCase305Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase305ActorState(); static_cast<FlowTestCase305Actor*>(this)->destroy(); return 0; }
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			new (&static_cast<FlowTestCase305Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase305ActorState();
			static_cast<FlowTestCase305Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase305ActorState();
		static_cast<FlowTestCase305Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase305()
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase305Actor final : public Actor<Void>, public FastAllocated<FlowTestCase305Actor>, public FlowTestCase305ActorState<FlowTestCase305Actor> {
															#line 1051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase305Actor>::operator new;
	using FastAllocated<FlowTestCase305Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase305Actor(UnitTestParameters const& params) 
															#line 1061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase305ActorState<FlowTestCase305Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase305", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase305");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase305", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase305( UnitTestParameters const& params ) {
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase305Actor(params));
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase305, "/RESTUtils/ValidURIWithoutService")

#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase315()
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase315Actor>
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase315ActorState {
															#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase315ActorState(UnitTestParameters const& params) 
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase315", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase315ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase315", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			std::string uri("https://host//foo/bar");
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			RESTUrl r(uri);
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.connType.secure, RESTConnectionType::SECURE_CONNECTION);
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.host.compare("host"), 0);
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT(r.service.empty());
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.resource.compare("//foo/bar"), 0);
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (!static_cast<FlowTestCase315Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase315ActorState(); static_cast<FlowTestCase315Actor*>(this)->destroy(); return 0; }
															#line 1135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			new (&static_cast<FlowTestCase315Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase315ActorState();
			static_cast<FlowTestCase315Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase315ActorState();
		static_cast<FlowTestCase315Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase315()
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase315Actor final : public Actor<Void>, public FastAllocated<FlowTestCase315Actor>, public FlowTestCase315ActorState<FlowTestCase315Actor> {
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase315Actor>::operator new;
	using FastAllocated<FlowTestCase315Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase315Actor(UnitTestParameters const& params) 
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase315ActorState<FlowTestCase315Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase315", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase315");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase315", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase315( UnitTestParameters const& params ) {
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase315Actor(params));
															#line 1201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase315, "/RESTUtils/ValidURIWithExtraForwardSlash")

#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase325()
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase325Actor>
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase325ActorState {
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase325ActorState(UnitTestParameters const& params) 
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase325", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase325ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase325", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			std::string uri("https://host/foo/bar?param1,param2");
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			RESTUrl r(uri);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.connType.secure, RESTConnectionType::SECURE_CONNECTION);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.host.compare("host"), 0);
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT(r.service.empty());
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.resource.compare("/foo/bar"), 0);
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.reqParameters.compare("param1,param2"), 0);
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (!static_cast<FlowTestCase325Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase325ActorState(); static_cast<FlowTestCase325Actor*>(this)->destroy(); return 0; }
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			new (&static_cast<FlowTestCase325Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase325ActorState();
			static_cast<FlowTestCase325Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase325ActorState();
		static_cast<FlowTestCase325Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase325()
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase325Actor final : public Actor<Void>, public FastAllocated<FlowTestCase325Actor>, public FlowTestCase325ActorState<FlowTestCase325Actor> {
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase325Actor>::operator new;
	using FastAllocated<FlowTestCase325Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase325Actor(UnitTestParameters const& params) 
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase325ActorState<FlowTestCase325Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase325", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase325");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase325", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase325( UnitTestParameters const& params ) {
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase325Actor(params));
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase325, "/RESTUtils/ValidURIWithParamsSecure")

#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase336()
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase336Actor>
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase336ActorState {
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase336ActorState(UnitTestParameters const& params) 
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase336", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase336ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase336", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			auto& g_knobs = IKnobCollection::getMutableGlobalKnobCollection();
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			g_knobs.setKnob("rest_kms_allow_not_secure_connection", KnobValueRef::create(bool{ false }));
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			std::string uri("http://host/foo/bar?param1,param2");
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			try {
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				RESTUrl r(uri);
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
				ASSERT(false);
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
				loopDepth = a_body1cont3(loopDepth);
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
		this->~FlowTestCase336ActorState();
		static_cast<FlowTestCase336Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		if (!static_cast<FlowTestCase336Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase336ActorState(); static_cast<FlowTestCase336Actor*>(this)->destroy(); return 0; }
															#line 1390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		new (&static_cast<FlowTestCase336Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase336ActorState();
		static_cast<FlowTestCase336Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(e.code(), error_code_rest_unsupported_protocol);
															#line 1403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
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
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase336()
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase336Actor final : public Actor<Void>, public FastAllocated<FlowTestCase336Actor>, public FlowTestCase336ActorState<FlowTestCase336Actor> {
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase336Actor>::operator new;
	using FastAllocated<FlowTestCase336Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase336Actor(UnitTestParameters const& params) 
															#line 1444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase336ActorState<FlowTestCase336Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase336", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase336");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase336", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase336( UnitTestParameters const& params ) {
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase336Actor(params));
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase336, "/RESTUtils/ValidURIWithParamsKnobNotEnabled")

#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"

															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase349()
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
template <class FlowTestCase349Actor>
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase349ActorState {
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase349ActorState(UnitTestParameters const& params) 
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
		 : params(params)
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase349", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase349ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase349", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			auto& g_knobs = IKnobCollection::getMutableGlobalKnobCollection();
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			g_knobs.setKnob("rest_kms_allow_not_secure_connection", KnobValueRef::create(bool{ true }));
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			std::string uri("http://host/foo/bar?param1,param2");
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			RESTUrl r(uri);
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.connType.secure, RESTConnectionType::NOT_SECURE_CONNECTION);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.host.compare("host"), 0);
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT(r.service.empty());
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.resource.compare("/foo/bar"), 0);
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			ASSERT_EQ(r.reqParameters.compare("param1,param2"), 0);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
			if (!static_cast<FlowTestCase349Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase349ActorState(); static_cast<FlowTestCase349Actor*>(this)->destroy(); return 0; }
															#line 1524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
			new (&static_cast<FlowTestCase349Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase349ActorState();
			static_cast<FlowTestCase349Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase349ActorState();
		static_cast<FlowTestCase349Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	UnitTestParameters params;
															#line 1548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase349()
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
class FlowTestCase349Actor final : public Actor<Void>, public FastAllocated<FlowTestCase349Actor>, public FlowTestCase349ActorState<FlowTestCase349Actor> {
															#line 1553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase349Actor>::operator new;
	using FastAllocated<FlowTestCase349Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	FlowTestCase349Actor(UnitTestParameters const& params) 
															#line 1563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase349ActorState<FlowTestCase349Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase349", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase349");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase349", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
static Future<Void> flowTestCase349( UnitTestParameters const& params ) {
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.cpp"
	return Future<Void>(new FlowTestCase349Actor(params));
															#line 1590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTUtils.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase349, "/RESTUtils/ValidURIWithParams")

