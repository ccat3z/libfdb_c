#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
/*
 * FlowTransport.actor.cpp
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

#include "fdbrpc/FlowTransport.h"
#include "flow/Arena.h"
#include "flow/network.h"

#include <cstdint>
#include <unordered_map>
#if VALGRIND
#include <memcheck.h>
#endif

#include <boost/unordered_map.hpp>

#include "fdbrpc/TokenSign.h"
#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/FailureMonitor.h"
#include "fdbrpc/HealthMonitor.h"
#include "fdbrpc/JsonWebKeySet.h"
#include "fdbrpc/genericactors.actor.h"
#include "fdbrpc/IPAllowList.h"
#include "fdbrpc/TokenCache.h"
#include "fdbrpc/simulator.h"
#include "flow/ActorCollection.h"
#include "flow/Error.h"
#include "flow/flow.h"
#include "flow/Net2Packet.h"
#include "flow/TDMetric.actor.h"
#include "flow/ObjectSerializer.h"
#include "flow/Platform.h"
#include "flow/ProtocolVersion.h"
#include "flow/UnitTest.h"
#include "flow/WatchFile.actor.h"
#include "flow/IConnection.h"
#define XXH_INLINE_ALL
#include "flow/xxhash.h"
#include "flow/actorcompiler.h" // This must be the last #include.

void removeCachedDNS(const std::string& host, const std::string& service) {
	INetworkConnections::net()->removeCachedDNS(host, service);
}

namespace {

NetworkAddressList g_currentDeliveryPeerAddress = NetworkAddressList();
bool g_currentDeliverPeerAddressTrusted = false;
Future<Void> g_currentDeliveryPeerDisconnect;

} // namespace

constexpr int PACKET_LEN_WIDTH = sizeof(uint32_t);
const uint64_t TOKEN_STREAM_FLAG = 1;

FDB_BOOLEAN_PARAM(InReadSocket);
FDB_BOOLEAN_PARAM(IsStableConnection);

class EndpointMap : NonCopyable {
public:
	// Reserve space for this many wellKnownEndpoints
	explicit EndpointMap(int wellKnownEndpointCount);
	void insertWellKnown(NetworkMessageReceiver* r, const Endpoint::Token& token, TaskPriority priority);
	void insert(NetworkMessageReceiver* r, Endpoint::Token& token, TaskPriority priority);
	const Endpoint& insert(NetworkAddressList localAddresses,
	                       std::vector<std::pair<FlowReceiver*, TaskPriority>> const& streams);
	NetworkMessageReceiver* get(Endpoint::Token const& token);
	TaskPriority getPriority(Endpoint::Token const& token);
	void remove(Endpoint::Token const& token, NetworkMessageReceiver* r);

private:
	void realloc();

	struct Entry {
		union {
			uint64_t
			    uid[2]; // priority packed into lower 32 bits; actual lower 32 bits of token are the index in data[]
			uint32_t nextFree;
		};
		NetworkMessageReceiver* receiver = nullptr;
		Endpoint::Token& token() { return *(Endpoint::Token*)uid; }
	};
	int wellKnownEndpointCount;
	std::vector<Entry> data;
	uint32_t firstFree;
};

EndpointMap::EndpointMap(int wellKnownEndpointCount)
  : wellKnownEndpointCount(wellKnownEndpointCount), data(wellKnownEndpointCount), firstFree(-1) {}

void EndpointMap::realloc() {
	int oldSize = data.size();
	data.resize(std::max(128, oldSize * 2));
	for (int i = oldSize; i < data.size(); i++) {
		data[i].receiver = 0;
		data[i].nextFree = i + 1;
	}
	data[data.size() - 1].nextFree = firstFree;
	firstFree = oldSize;
}

void EndpointMap::insertWellKnown(NetworkMessageReceiver* r, const Endpoint::Token& token, TaskPriority priority) {
	int index = token.second();
	ASSERT(index <= wellKnownEndpointCount);
	ASSERT(data[index].receiver == nullptr);
	data[index].receiver = r;
	data[index].token() =
	    Endpoint::Token(token.first(), (token.second() & 0xffffffff00000000LL) | static_cast<uint32_t>(priority));
}

void EndpointMap::insert(NetworkMessageReceiver* r, Endpoint::Token& token, TaskPriority priority) {
	if (firstFree == uint32_t(-1))
		realloc();
	int index = firstFree;
	firstFree = data[index].nextFree;
	token = Endpoint::Token(token.first(), (token.second() & 0xffffffff00000000LL) | index);
	data[index].token() =
	    Endpoint::Token(token.first(), (token.second() & 0xffffffff00000000LL) | static_cast<uint32_t>(priority));
	data[index].receiver = r;
}

const Endpoint& EndpointMap::insert(NetworkAddressList localAddresses,
                                    std::vector<std::pair<FlowReceiver*, TaskPriority>> const& streams) {
	int adjacentFree = 0;
	int adjacentStart = -1;
	firstFree = -1;
	for (int i = wellKnownEndpointCount; i < data.size(); i++) {
		if (data[i].receiver) {
			adjacentFree = 0;
		} else {
			data[i].nextFree = firstFree;
			firstFree = i;
			if (adjacentStart == -1 && ++adjacentFree == streams.size()) {
				adjacentStart = i + 1 - adjacentFree;
				firstFree = data[adjacentStart].nextFree;
			}
		}
	}
	if (adjacentStart == -1) {
		data.resize(data.size() + streams.size() - adjacentFree);
		adjacentStart = data.size() - streams.size();
		if (adjacentFree > 0) {
			firstFree = data[adjacentStart].nextFree;
		}
	}

	UID base = deterministicRandom()->randomUniqueID();
	for (uint64_t i = 0; i < streams.size(); i++) {
		int index = adjacentStart + i;
		uint64_t first = (base.first() + (i << 32)) | TOKEN_STREAM_FLAG;
		streams[i].first->setEndpoint(
		    Endpoint(localAddresses, UID(first, (base.second() & 0xffffffff00000000LL) | index)));
		data[index].token() =
		    Endpoint::Token(first, (base.second() & 0xffffffff00000000LL) | static_cast<uint32_t>(streams[i].second));
		data[index].receiver = (NetworkMessageReceiver*)streams[i].first;
	}

	return streams[0].first->getEndpoint(TaskPriority::DefaultEndpoint);
}

NetworkMessageReceiver* EndpointMap::get(Endpoint::Token const& token) {
	uint32_t index = token.second();
	if (index < wellKnownEndpointCount && data[index].receiver == nullptr) {
		TraceEvent(SevWarnAlways, "WellKnownEndpointNotAdded")
		    .detail("Token", token)
		    .detail("Index", index)
		    .backtrace();
	}
	if (index < data.size() && data[index].token().first() == token.first() &&
	    ((data[index].token().second() & 0xffffffff00000000LL) | index) == token.second())
		return data[index].receiver;
	return 0;
}

TaskPriority EndpointMap::getPriority(Endpoint::Token const& token) {
	uint32_t index = token.second();
	if (index < data.size() && data[index].token().first() == token.first() &&
	    ((data[index].token().second() & 0xffffffff00000000LL) | index) == token.second()) {
		auto res = static_cast<TaskPriority>(data[index].token().second());
		// we don't allow this priority to be "misused" for other stuff as we won't even
		// attempt to find an endpoint if UnknownEndpoint is returned here
		ASSERT(res != TaskPriority::UnknownEndpoint);
		return res;
	}
	return TaskPriority::UnknownEndpoint;
}

void EndpointMap::remove(Endpoint::Token const& token, NetworkMessageReceiver* r) {
	uint32_t index = token.second();
	if (index < wellKnownEndpointCount) {
		data[index].receiver = nullptr;
	} else if (index < data.size() && data[index].token().first() == token.first() &&
	           ((data[index].token().second() & 0xffffffff00000000LL) | index) == token.second() &&
	           data[index].receiver == r) {
		data[index].receiver = 0;
		data[index].nextFree = firstFree;
		firstFree = index;
	}
}

struct EndpointNotFoundReceiver final : NetworkMessageReceiver {
	EndpointNotFoundReceiver(EndpointMap& endpoints) {
		endpoints.insertWellKnown(
		    this, Endpoint::wellKnownToken(WLTOKEN_ENDPOINT_NOT_FOUND), TaskPriority::DefaultEndpoint);
	}

	void receive(ArenaObjectReader& reader) override {
		// Remote machine tells us it doesn't have endpoint e
		UID token;
		reader.deserialize(token);
		Endpoint e = FlowTransport::transport().loadedEndpoint(token);
		IFailureMonitor::failureMonitor().endpointNotFound(e);
	}
	bool isPublic() const override { return true; }
};

struct PingRequest {
	constexpr static FileIdentifier file_identifier = 4707015;
	ReplyPromise<Void> reply{ PeerCompatibilityPolicy{ RequirePeer::AtLeast,
		                                               ProtocolVersion::withStableInterfaces() } };
	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, reply);
	}
};

struct PingReceiver final : NetworkMessageReceiver {
	PingReceiver(EndpointMap& endpoints) {
		endpoints.insertWellKnown(this, Endpoint::wellKnownToken(WLTOKEN_PING_PACKET), TaskPriority::ReadSocket);
	}
	void receive(ArenaObjectReader& reader) override {
		PingRequest req;
		reader.deserialize(req);
		req.reply.send(Void());
	}
	PeerCompatibilityPolicy peerCompatibilityPolicy() const override {
		return PeerCompatibilityPolicy{ RequirePeer::AtLeast, ProtocolVersion::withStableInterfaces() };
	}
	bool isPublic() const override { return true; }
};

struct UnauthorizedEndpointReceiver final : NetworkMessageReceiver {
	UnauthorizedEndpointReceiver(EndpointMap& endpoints) {
		endpoints.insertWellKnown(
		    this, Endpoint::wellKnownToken(WLTOKEN_UNAUTHORIZED_ENDPOINT), TaskPriority::ReadSocket);
	}

	void receive(ArenaObjectReader& reader) override {
		UID token;
		reader.deserialize(token);
		Endpoint e = FlowTransport::transport().loadedEndpoint(token);
		IFailureMonitor::failureMonitor().unauthorizedEndpoint(e);
	}
	bool isPublic() const override { return true; }
};

// NetworkAddressCachedString retains a cached Standalone<StringRef> of
// a NetworkAddressList.address.toString() value. This cached value is useful
// for features in the hot path (i.e. Tracing), which need the String formatted value
// frequently and do not wish to pay the formatting cost. If the underlying NetworkAddressList
// needs to change, do not attempt to update it directly, use the setNetworkAddress API as it
// will ensure the new toString() cached value is updated.
class NetworkAddressCachedString {
public:
	NetworkAddressCachedString() { setAddressList(NetworkAddressList()); }
	NetworkAddressCachedString(NetworkAddressList const& list) { setAddressList(list); }
	NetworkAddressList const& getAddressList() const { return addressList; }
	void setAddressList(NetworkAddressList const& list) {
		cachedStr = Standalone<StringRef>(StringRef(list.address.toString()));
		addressList = list;
	}
	void setNetworkAddress(NetworkAddress const& addr) {
		addressList.address = addr;
		setAddressList(addressList); // force the recaching of the string.
	}
	Standalone<StringRef> getLocalAddressAsString() const { return cachedStr; }
	operator NetworkAddressList const&() { return addressList; }

private:
	NetworkAddressList addressList;
	Standalone<StringRef> cachedStr;
};

class TransportData {
public:
	TransportData(uint64_t transportId, int maxWellKnownEndpoints, IPAllowList const* allowList);

	~TransportData();

	void initMetrics() {
		bytesSent.init("Net2.BytesSent"_sr);
		countPacketsReceived.init("Net2.CountPacketsReceived"_sr);
		countPacketsGenerated.init("Net2.CountPacketsGenerated"_sr);
		countConnEstablished.init("Net2.CountConnEstablished"_sr);
		countConnClosedWithError.init("Net2.CountConnClosedWithError"_sr);
		countConnClosedWithoutError.init("Net2.CountConnClosedWithoutError"_sr);
	}

	Reference<struct Peer> getPeer(NetworkAddress const& address);
	Reference<struct Peer> getOrOpenPeer(NetworkAddress const& address, bool startConnectionKeeper = true);

	// Returns true if given network address 'address' is one of the address we are listening on.
	bool isLocalAddress(const NetworkAddress& address) const;
	void applyPublicKeySet(StringRef jwkSetString);

	NetworkAddressCachedString localAddresses;
	std::vector<Future<Void>> listeners;
	std::unordered_map<NetworkAddress, Reference<struct Peer>> peers;
	std::unordered_map<NetworkAddress, std::pair<double, double>> closedPeers;
	HealthMonitor healthMonitor;
	std::set<NetworkAddress> orderedAddresses;
	Reference<AsyncVar<bool>> degraded;

	EndpointMap endpoints;
	EndpointNotFoundReceiver endpointNotFoundReceiver{ endpoints };
	PingReceiver pingReceiver{ endpoints };
	UnauthorizedEndpointReceiver unauthorizedEndpointReceiver{ endpoints };

	Int64MetricHandle bytesSent;
	Int64MetricHandle countPacketsReceived;
	Int64MetricHandle countPacketsGenerated;
	Int64MetricHandle countConnEstablished;
	Int64MetricHandle countConnClosedWithError;
	Int64MetricHandle countConnClosedWithoutError;

	std::map<NetworkAddress, std::pair<uint64_t, double>> incompatiblePeers;
	AsyncTrigger incompatiblePeersChanged;
	uint32_t numIncompatibleConnections;
	std::map<uint64_t, double> multiVersionConnections;
	double lastIncompatibleMessage;
	uint64_t transportId;
	IPAllowList allowList;

	Future<Void> multiVersionCleanup;
	Future<Void> pingLogger;
	Future<Void> publicKeyFileWatch;

	std::unordered_map<Standalone<StringRef>, PublicKey> publicKeys;
};

															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via pingLatencyLogger()
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class PingLatencyLoggerActor>
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class PingLatencyLoggerActorState {
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	PingLatencyLoggerActorState(TransportData* const& self) 
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   lastAddress(NetworkAddress())
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("pingLatencyLogger", reinterpret_cast<unsigned long>(this));

	}
	~PingLatencyLoggerActorState() 
	{
		fdb_probe_actor_destroy("pingLatencyLogger", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~PingLatencyLoggerActorState();
		static_cast<PingLatencyLoggerActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (self->orderedAddresses.size())
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			auto it = self->orderedAddresses.upper_bound(lastAddress);
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (it == self->orderedAddresses.end())
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it = self->orderedAddresses.begin();
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastAddress = *it;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			auto peer = self->getPeer(lastAddress);
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!peer)
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent(SevWarnAlways, "MissingNetworkAddress").suppressFor(10.0).detail("PeerAddr", lastAddress);
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (peer->lastLoggedTime <= 0.0)
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedTime = peer->lastConnectTime;
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (peer && (peer->pingLatencies.getPopulationSize() >= 10 || peer->connectFailedCount > 0 || peer->timeoutCount > 0))
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("PingLatency") .detail("Elapsed", now() - peer->lastLoggedTime) .detail("PeerAddr", lastAddress) .detail("MinLatency", peer->pingLatencies.min()) .detail("MaxLatency", peer->pingLatencies.max()) .detail("MeanLatency", peer->pingLatencies.mean()) .detail("MedianLatency", peer->pingLatencies.median()) .detail("P90Latency", peer->pingLatencies.percentile(0.90)) .detail("Count", peer->pingLatencies.getPopulationSize()) .detail("BytesReceived", peer->bytesReceived - peer->lastLoggedBytesReceived) .detail("BytesSent", peer->bytesSent - peer->lastLoggedBytesSent) .detail("TimeoutCount", peer->timeoutCount) .detail("ConnectOutgoingCount", peer->connectOutgoingCount) .detail("ConnectIncomingCount", peer->connectIncomingCount) .detail("ConnectFailedCount", peer->connectFailedCount) .detail("ConnectMinLatency", peer->connectLatencies.min()) .detail("ConnectMaxLatency", peer->connectLatencies.max()) .detail("ConnectMeanLatency", peer->connectLatencies.mean()) .detail("ConnectMedianLatency", peer->connectLatencies.median()) .detail("ConnectP90Latency", peer->connectLatencies.percentile(0.90));
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedTime = now();
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->connectOutgoingCount = 0;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->connectIncomingCount = 0;
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->connectFailedCount = 0;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->pingLatencies.clear();
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->connectLatencies.clear();
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedBytesReceived = peer->bytesReceived;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedBytesSent = peer->bytesSent;
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->timeoutCount = 0;
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->PING_LOGGING_INTERVAL);
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 1;
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PingLatencyLoggerActor, 0, Void >*>(static_cast<PingLatencyLoggerActor*>(this)));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (it == self->orderedAddresses.begin())
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->PING_LOGGING_INTERVAL);
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
					static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 2;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PingLatencyLoggerActor, 1, Void >*>(static_cast<PingLatencyLoggerActor*>(this)));
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1loopBody1cont7(loopDepth);
				}
			}
		}
		else
		{
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->PING_LOGGING_INTERVAL);
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when3(__when_expr_2.get(), loopDepth); };
			static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 3;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PingLatencyLoggerActor, 2, Void >*>(static_cast<PingLatencyLoggerActor*>(this)));
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state > 0) static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 0;
		static_cast<PingLatencyLoggerActor*>(this)->ActorCallback< PingLatencyLoggerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PingLatencyLoggerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PingLatencyLoggerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PingLatencyLoggerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state > 0) static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 0;
		static_cast<PingLatencyLoggerActor*>(this)->ActorCallback< PingLatencyLoggerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PingLatencyLoggerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PingLatencyLoggerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PingLatencyLoggerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state > 0) static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 0;
		static_cast<PingLatencyLoggerActor*>(this)->ActorCallback< PingLatencyLoggerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PingLatencyLoggerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< PingLatencyLoggerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< PingLatencyLoggerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress lastAddress;
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via pingLatencyLogger()
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class PingLatencyLoggerActor final : public Actor<Void>, public ActorCallback< PingLatencyLoggerActor, 0, Void >, public ActorCallback< PingLatencyLoggerActor, 1, Void >, public ActorCallback< PingLatencyLoggerActor, 2, Void >, public FastAllocated<PingLatencyLoggerActor>, public PingLatencyLoggerActorState<PingLatencyLoggerActor> {
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<PingLatencyLoggerActor>::operator new;
	using FastAllocated<PingLatencyLoggerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PingLatencyLoggerActor, 0, Void >;
friend struct ActorCallback< PingLatencyLoggerActor, 1, Void >;
friend struct ActorCallback< PingLatencyLoggerActor, 2, Void >;
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	PingLatencyLoggerActor(TransportData* const& self) 
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   PingLatencyLoggerActorState<PingLatencyLoggerActor>(self)
	{
		fdb_probe_actor_enter("pingLatencyLogger", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("pingLatencyLogger");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("pingLatencyLogger", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PingLatencyLoggerActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PingLatencyLoggerActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< PingLatencyLoggerActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> pingLatencyLogger( TransportData* const& self ) {
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new PingLatencyLoggerActor(self));
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

TransportData::TransportData(uint64_t transportId, int maxWellKnownEndpoints, IPAllowList const* allowList)
  : endpoints(maxWellKnownEndpoints), endpointNotFoundReceiver(endpoints), pingReceiver(endpoints),
    numIncompatibleConnections(0), lastIncompatibleMessage(0), transportId(transportId),
    allowList(allowList == nullptr ? IPAllowList() : *allowList) {
	degraded = makeReference<AsyncVar<bool>>(false);
	pingLogger = pingLatencyLogger(this);
}

#define CONNECT_PACKET_V0 0x0FDB00A444020001LL
#define CONNECT_PACKET_V0_SIZE 14

#pragma pack(push, 1)
struct ConnectPacket {
	// The value does not include the size of `connectPacketLength` itself,
	// but only the other fields of this structure.
	uint32_t connectPacketLength = 0;
	ProtocolVersion protocolVersion; // Expect currentProtocolVersion

	uint16_t canonicalRemotePort = 0; // Port number to reconnect to the originating process
	uint64_t connectionId = 0; // Multi-version clients will use the same Id for both connections, other connections
	                           // will set this to zero. Added at protocol Version 0x0FDB00A444020001.

	// IP Address to reconnect to the originating process. Only one of these must be populated.
	uint32_t canonicalRemoteIp4 = 0;

	enum ConnectPacketFlags { FLAG_IPV6 = 1 };
	uint16_t flags = 0;
	uint8_t canonicalRemoteIp6[16] = { 0 };

	ConnectPacket() = default;

	IPAddress canonicalRemoteIp() const {
		if (isIPv6()) {
			IPAddress::IPAddressStore store;
			memcpy(store.data(), canonicalRemoteIp6, sizeof(canonicalRemoteIp6));
			return IPAddress(store);
		} else {
			return IPAddress(canonicalRemoteIp4);
		}
	}

	void setCanonicalRemoteIp(const IPAddress& ip) {
		if (ip.isV6()) {
			flags = flags | FLAG_IPV6;
			memcpy(&canonicalRemoteIp6, ip.toV6().data(), 16);
		} else {
			flags = flags & ~FLAG_IPV6;
			canonicalRemoteIp4 = ip.toV4();
		}
	}

	bool isIPv6() const { return flags & FLAG_IPV6; }

	uint32_t totalPacketSize() const { return connectPacketLength + sizeof(connectPacketLength); }

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, connectPacketLength);
		if (connectPacketLength > sizeof(ConnectPacket) - sizeof(connectPacketLength)) {
			ASSERT(!g_network->isSimulated());
			TraceEvent("SerializationFailed").backtrace();
			throw serialization_failed();
		}

		serializer(ar, protocolVersion, canonicalRemotePort, connectionId, canonicalRemoteIp4);
		if (ar.isDeserializing && !ar.protocolVersion().hasIPv6()) {
			flags = 0;
		} else {
			// We can send everything in serialized packet, since the current version of ConnectPacket
			// is backward compatible with CONNECT_PACKET_V0.
			serializer(ar, flags);
			ar.serializeBytes(&canonicalRemoteIp6, sizeof(canonicalRemoteIp6));
		}
	}
};

#pragma pack(pop)

															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
[[nodiscard]] static Future<Void> connectionReader( TransportData* const& transport, Reference<IConnection> const& conn, Reference<struct Peer> const& peer, Promise<Reference<struct Peer>> const& onConnected );

#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

static void sendLocal(TransportData* self, ISerializeSource const& what, const Endpoint& destination);
static ReliablePacket* sendPacket(TransportData* self,
                                  Reference<Peer> peer,
                                  ISerializeSource const& what,
                                  const Endpoint& destination,
                                  bool reliable);

															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionMonitor()
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionMonitorActor>
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionMonitorActorState {
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionMonitorActorState(Reference<Peer> const& peer) 
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : peer(peer),
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   remotePingEndpoint({ peer->destination }, Endpoint::wellKnownToken(WLTOKEN_PING_PACKET))
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("connectionMonitor", reinterpret_cast<unsigned long>(this));

	}
	~ConnectionMonitorActorState() 
	{
		fdb_probe_actor_destroy("connectionMonitor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~ConnectionMonitorActorState();
		static_cast<ConnectionMonitorActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!FlowTransport::isClient() && !peer->destination.isPublic() && peer->compatible)
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastRefreshed = now();
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastBytesReceived = peer->bytesReceived;
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = a_body1loopBody1loopHead1(loopDepth);
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 2;
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 1, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME, TaskPriority::ReadSocket);
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 1;
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 0, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (lastBytesReceived < peer->bytesReceived)
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastRefreshed = now();
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastBytesReceived = peer->bytesReceived;
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		else
		{
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (lastRefreshed < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_INCOMING_IDLE_MULTIPLIER)
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
				return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
			}
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (lastBytesReceived < peer->bytesReceived)
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastRefreshed = now();
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			lastBytesReceived = peer->bytesReceived;
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		else
		{
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (lastRefreshed < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_INCOMING_IDLE_MULTIPLIER)
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
				return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
			}
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state > 0) static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionMonitorActor*>(this)->ActorCallback< ConnectionMonitorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectionMonitorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (peer->reliable.empty() && peer->unsent.empty() && peer->outstandingReplies == 0)
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (peer->peerReferences == 0 && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_UNREFERENCED_CLOSE_DELAY))
															#line 1177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(connection_unreferenced(), std::max(0, loopDepth - 1));
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (FlowTransport::isClient() && peer->compatible && peer->destination.isPublic() && (peer->lastConnectTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT) && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT))
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					return a_body1Catch1(connection_idle(), std::max(0, loopDepth - 1));
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
		}
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME, TaskPriority::ReadSocket);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 3;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 2, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (peer->reliable.empty() && peer->unsent.empty() && peer->outstandingReplies == 0)
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (peer->peerReferences == 0 && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_UNREFERENCED_CLOSE_DELAY))
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(connection_unreferenced(), std::max(0, loopDepth - 1));
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (FlowTransport::isClient() && peer->compatible && peer->destination.isPublic() && (peer->lastConnectTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT) && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT))
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					return a_body1Catch1(connection_idle(), std::max(0, loopDepth - 1));
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
		}
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME, TaskPriority::ReadSocket);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 3;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 2, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state > 0) static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionMonitorActor*>(this)->ActorCallback< ConnectionMonitorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectionMonitorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		pingRequest = PingRequest();
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		FlowTransport::transport().sendUnreliable(SerializeSource<PingRequest>(pingRequest), remotePingEndpoint, true);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		timeouts = 0;
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		startTime = now();
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		pingRequest = PingRequest();
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		FlowTransport::transport().sendUnreliable(SerializeSource<PingRequest>(pingRequest), remotePingEndpoint, true);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		timeouts = 0;
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		startTime = now();
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopHead1(loopDepth);

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
	void a_exitChoose3() 
	{
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state > 0) static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionMonitorActor*>(this)->ActorCallback< ConnectionMonitorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectionMonitorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont9(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont4loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1(int loopDepth) 
	{
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont4loopBody1when1(__when_expr_3.get(), loopDepth); };
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = pingRequest.reply.getFuture();
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont4loopBody1when2(__when_expr_4.get(), loopDepth); };
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_5 = peer->resetPing.onTrigger();
															#line 1440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont4loopBody1when3(__when_expr_5.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 4;
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 3, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 4, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 5, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont9(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont4loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		peer->timeoutCount++;
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (startingBytes == peer->bytesReceived)
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (peer->destination.isPublic())
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->pingLatencies.addSample(now() - startTime);
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionTimeout").suppressFor(1.0).detail("WithAddr", peer->destination);
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 2));
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (timeouts > 1)
															#line 1497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevWarnAlways, "ConnectionSlowPing") .suppressFor(1.0) .detail("WithAddr", peer->destination) .detail("Timeouts", timeouts);
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		timeouts++;
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when1(Void && _,int loopDepth) 
	{
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		peer->timeoutCount++;
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (startingBytes == peer->bytesReceived)
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (peer->destination.isPublic())
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->pingLatencies.addSample(now() - startTime);
															#line 1526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionTimeout").suppressFor(1.0).detail("WithAddr", peer->destination);
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 2));
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (timeouts > 1)
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevWarnAlways, "ConnectionSlowPing") .suppressFor(1.0) .detail("WithAddr", peer->destination) .detail("Timeouts", timeouts);
															#line 1540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		timeouts++;
															#line 1546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (peer->destination.isPublic())
															#line 1555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			peer->pingLatencies.addSample(now() - startTime);
															#line 1559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		return a_body1loopBody1cont4break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when2(Void && _,int loopDepth) 
	{
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (peer->destination.isPublic())
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			peer->pingLatencies.addSample(now() - startTime);
															#line 1573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		return a_body1loopBody1cont4break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when3(Void const& _,int loopDepth) 
	{
		return a_body1loopBody1cont4break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when3(Void && _,int loopDepth) 
	{
		return a_body1loopBody1cont4break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state > 0) static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionMonitorActor*>(this)->ActorCallback< ConnectionMonitorActor, 3, Void >::remove();
		static_cast<ConnectionMonitorActor*>(this)->ActorCallback< ConnectionMonitorActor, 4, Void >::remove();
		static_cast<ConnectionMonitorActor*>(this)->ActorCallback< ConnectionMonitorActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ConnectionMonitorActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ConnectionMonitorActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4loopBody1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ConnectionMonitorActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4loopBody1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ConnectionMonitorActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> peer;
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Endpoint remotePingEndpoint;
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	double lastRefreshed;
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int64_t lastBytesReceived;
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	PingRequest pingRequest;
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int64_t startingBytes;
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int timeouts;
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	double startTime;
															#line 1750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionMonitor()
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionMonitorActor final : public Actor<Void>, public ActorCallback< ConnectionMonitorActor, 0, Void >, public ActorCallback< ConnectionMonitorActor, 1, Void >, public ActorCallback< ConnectionMonitorActor, 2, Void >, public ActorCallback< ConnectionMonitorActor, 3, Void >, public ActorCallback< ConnectionMonitorActor, 4, Void >, public ActorCallback< ConnectionMonitorActor, 5, Void >, public FastAllocated<ConnectionMonitorActor>, public ConnectionMonitorActorState<ConnectionMonitorActor> {
															#line 1755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ConnectionMonitorActor>::operator new;
	using FastAllocated<ConnectionMonitorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectionMonitorActor, 0, Void >;
friend struct ActorCallback< ConnectionMonitorActor, 1, Void >;
friend struct ActorCallback< ConnectionMonitorActor, 2, Void >;
friend struct ActorCallback< ConnectionMonitorActor, 3, Void >;
friend struct ActorCallback< ConnectionMonitorActor, 4, Void >;
friend struct ActorCallback< ConnectionMonitorActor, 5, Void >;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionMonitorActor(Reference<Peer> const& peer) 
															#line 1771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectionMonitorActorState<ConnectionMonitorActor>(peer)
	{
		fdb_probe_actor_enter("connectionMonitor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectionMonitor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectionMonitor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectionMonitorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectionMonitorActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ConnectionMonitorActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ConnectionMonitorActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> connectionMonitor( Reference<Peer> const& peer ) {
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionMonitorActor(peer));
															#line 1802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

															#line 1807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionWriter()
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionWriterActor>
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionWriterActorState {
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionWriterActorState(Reference<Peer> const& self,Reference<IConnection> const& conn) 
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn),
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   lastWriteTime(now())
															#line 1825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("connectionWriter", reinterpret_cast<unsigned long>(this));

	}
	~ConnectionWriterActorState() 
	{
		fdb_probe_actor_destroy("connectionWriter", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 1840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~ConnectionWriterActorState();
		static_cast<ConnectionWriterActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delayJittered( std::max<double>(FLOW_KNOBS->MIN_COALESCE_DELAY, FLOW_KNOBS->MAX_COALESCE_DELAY - (now() - lastWriteTime)), TaskPriority::WriteSocket);
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 1;
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 0, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state > 0) static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionWriterActor*>(this)->ActorCallback< ConnectionWriterActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectionWriterActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1(int loopDepth) 
	{
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		lastWriteTime = now();
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		int sent = conn->write(self->unsent.getUnsent(), FLOW_KNOBS->MAX_PACKET_SEND_BYTES);
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (sent)
															#line 1987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->bytesSent += sent;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->transport->bytesSent += sent;
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->unsent.sent(sent);
															#line 1995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (self->unsent.empty())
															#line 1999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		CODE_PROBE( true, "We didn't write everything, so apparently the write buffer is full.  Wait for it to be nonfull");
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 2;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 1, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 3;
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 2, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 2043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 3;
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 2, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 2059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state > 0) static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionWriterActor*>(this)->ActorCallback< ConnectionWriterActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectionWriterActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont1loopBody1cont4(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont4(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state > 0) static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionWriterActor*>(this)->ActorCallback< ConnectionWriterActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectionWriterActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1(int loopDepth) 
	{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!(self->unsent.empty()))
															#line 2219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = self->dataToSend.onTrigger();
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 4;
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 3, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state > 0) static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionWriterActor*>(this)->ActorCallback< ConnectionWriterActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionWriterActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ConnectionWriterActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> self;
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	double lastWriteTime;
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionWriter()
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionWriterActor final : public Actor<Void>, public ActorCallback< ConnectionWriterActor, 0, Void >, public ActorCallback< ConnectionWriterActor, 1, Void >, public ActorCallback< ConnectionWriterActor, 2, Void >, public ActorCallback< ConnectionWriterActor, 3, Void >, public FastAllocated<ConnectionWriterActor>, public ConnectionWriterActorState<ConnectionWriterActor> {
															#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ConnectionWriterActor>::operator new;
	using FastAllocated<ConnectionWriterActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectionWriterActor, 0, Void >;
friend struct ActorCallback< ConnectionWriterActor, 1, Void >;
friend struct ActorCallback< ConnectionWriterActor, 2, Void >;
friend struct ActorCallback< ConnectionWriterActor, 3, Void >;
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionWriterActor(Reference<Peer> const& self,Reference<IConnection> const& conn) 
															#line 2350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectionWriterActorState<ConnectionWriterActor>(self, conn)
	{
		fdb_probe_actor_enter("connectionWriter", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectionWriter");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectionWriter", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectionWriterActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectionWriterActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ConnectionWriterActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ConnectionWriterActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> connectionWriter( Reference<Peer> const& self, Reference<IConnection> const& conn ) {
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionWriterActor(self, conn));
															#line 2381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via delayedHealthUpdate()
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class DelayedHealthUpdateActor>
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class DelayedHealthUpdateActorState {
															#line 2393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	DelayedHealthUpdateActorState(NetworkAddress const& address,bool* const& tooManyConnectionsClosed) 
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : address(address),
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   tooManyConnectionsClosed(tooManyConnectionsClosed),
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   start(now())
															#line 2404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("delayedHealthUpdate", reinterpret_cast<unsigned long>(this));

	}
	~DelayedHealthUpdateActorState() 
	{
		fdb_probe_actor_destroy("delayedHealthUpdate", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 2419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~DelayedHealthUpdateActorState();
		static_cast<DelayedHealthUpdateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<DelayedHealthUpdateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DelayedHealthUpdateActorState(); static_cast<DelayedHealthUpdateActor*>(this)->destroy(); return 0; }
															#line 2442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<DelayedHealthUpdateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DelayedHealthUpdateActorState();
		static_cast<DelayedHealthUpdateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (FLOW_KNOBS->HEALTH_MONITOR_MARK_FAILED_UNSTABLE_CONNECTIONS && FlowTransport::transport().healthMonitor()->tooManyConnectionsClosed(address) && address.isPublic())
															#line 2461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(FLOW_KNOBS->MAX_RECONNECTION_TIME * 2.0);
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<DelayedHealthUpdateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DelayedHealthUpdateActor*>(this)->actor_wait_state = 1;
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DelayedHealthUpdateActor, 0, Void >*>(static_cast<DelayedHealthUpdateActor*>(this)));
															#line 2472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (*tooManyConnectionsClosed)
															#line 2479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("TooManyConnectionsClosedMarkAvailable") .detail("Dest", address) .detail("StartTime", start) .detail("TimeElapsed", now() - start) .detail("ClosedCount", FlowTransport::transport().healthMonitor()->closedConnectionsCount(address));
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				*tooManyConnectionsClosed = false;
															#line 2485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().setStatus(address, FailureStatus(false));
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}

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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DelayedHealthUpdateActor*>(this)->actor_wait_state > 0) static_cast<DelayedHealthUpdateActor*>(this)->actor_wait_state = 0;
		static_cast<DelayedHealthUpdateActor*>(this)->ActorCallback< DelayedHealthUpdateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DelayedHealthUpdateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DelayedHealthUpdateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DelayedHealthUpdateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress address;
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool* tooManyConnectionsClosed;
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	double start;
															#line 2595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via delayedHealthUpdate()
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class DelayedHealthUpdateActor final : public Actor<Void>, public ActorCallback< DelayedHealthUpdateActor, 0, Void >, public FastAllocated<DelayedHealthUpdateActor>, public DelayedHealthUpdateActorState<DelayedHealthUpdateActor> {
															#line 2600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<DelayedHealthUpdateActor>::operator new;
	using FastAllocated<DelayedHealthUpdateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DelayedHealthUpdateActor, 0, Void >;
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	DelayedHealthUpdateActor(NetworkAddress const& address,bool* const& tooManyConnectionsClosed) 
															#line 2611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   DelayedHealthUpdateActorState<DelayedHealthUpdateActor>(address, tooManyConnectionsClosed)
	{
		fdb_probe_actor_enter("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("delayedHealthUpdate");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("delayedHealthUpdate", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DelayedHealthUpdateActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> delayedHealthUpdate( NetworkAddress const& address, bool* const& tooManyConnectionsClosed ) {
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new DelayedHealthUpdateActor(address, tooManyConnectionsClosed));
															#line 2639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

															#line 2644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionKeeper()
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionKeeperActor>
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionKeeperActorState {
															#line 2651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionKeeperActorState(Reference<Peer> const& self,Reference<IConnection> const& conn = Reference<IConnection>(),Future<Void> const& reader = Void()) 
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn),
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   reader(reader)
															#line 2662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("connectionKeeper", reinterpret_cast<unsigned long>(this));

	}
	~ConnectionKeeperActorState() 
	{
		fdb_probe_actor_destroy("connectionKeeper", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevDebug, "ConnectionKeeper", conn ? conn->getDebugID() : UID()) .detail("PeerAddr", self->destination) .detail("ConnSet", (bool)conn);
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			ASSERT_WE_THINK(FlowTransport::transport().getLocalAddress() != self->destination);
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = Future<Void>();
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			firstConnFailedTime = Optional<double>();
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			retryConnect = false;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			tooManyConnectionsClosed = false;
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 2689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~ConnectionKeeperActorState();
		static_cast<ConnectionKeeperActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = Future<Void>();
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!conn)
															#line 2722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->outgoingConnectionIdle = true;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				;
															#line 2728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = a_body1loopBody1loopHead1(loopDepth);
			}
			else
			{
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->outgoingConnectionIdle = false;
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->lastConnectTime = now();
															#line 2737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->connected = false;
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF.cancel();
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (now() - self->lastConnectTime > FLOW_KNOBS->RECONNECTION_RESET_TIME)
															#line 2764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->reconnectionDelay = FLOW_KNOBS->INITIAL_RECONNECTION_TIME;
															#line 2768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->reconnectionDelay = std::min(FLOW_KNOBS->MAX_RECONNECTION_TIME, self->reconnectionDelay * FLOW_KNOBS->RECONNECTION_TIME_GROWTH_RATE);
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (firstConnFailedTime.present())
															#line 2778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (now() - firstConnFailedTime.get() > FLOW_KNOBS->PEER_UNAVAILABLE_FOR_LONG_TIME_TIMEOUT)
															#line 2782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent(SevWarnAlways, "PeerUnavailableForLongTime", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					firstConnFailedTime = now() - FLOW_KNOBS->PEER_UNAVAILABLE_FOR_LONG_TIME_TIMEOUT / 2.0;
															#line 2788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
			else
			{
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				firstConnFailedTime = now();
															#line 2795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			retryConnect = true;
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_connection_failed)
															#line 2801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (!self->destination.isPublic())
															#line 2805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(true));
															#line 2809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
				else
				{
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (now() - firstConnFailedTime.get() > FLOW_KNOBS->FAILURE_DETECTION_DELAY)
															#line 2815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(true));
															#line 2819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
			}
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->discardUnreliablePackets();
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			reader = Future<Void>();
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			bool ok = e.code() == error_code_connection_failed || e.code() == error_code_actor_cancelled || e.code() == error_code_connection_unreferenced || e.code() == error_code_connection_idle || (g_network->isSimulated() && e.code() == error_code_checksum_failed);
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (self->compatible)
															#line 2831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent(ok ? SevInfo : SevWarnAlways, "ConnectionClosed", conn ? conn->getDebugID() : UID()) .errorUnsuppressed(e) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 2835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent( ok ? SevInfo : SevWarnAlways, "IncompatibleConnectionClosed", conn ? conn->getDebugID() : UID()) .errorUnsuppressed(e) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->compatible = true;
															#line 2843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (self->destination.isPublic() && IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() && !FlowTransport::isClient())
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				auto& it = self->transport->closedPeers[self->destination];
															#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (now() - it.second > FLOW_KNOBS->TOO_MANY_CONNECTIONS_CLOSED_RESET_DELAY)
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					it.first = now();
															#line 2857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
				else
				{
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (now() - it.first > FLOW_KNOBS->TOO_MANY_CONNECTIONS_CLOSED_TIMEOUT)
															#line 2863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						TraceEvent(SevWarnAlways, "TooManyConnectionsClosed", conn ? conn->getDebugID() : UID()) .suppressFor(5.0) .detail("PeerAddr", self->destination);
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						self->transport->degraded->set(true);
															#line 2869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it.second = now();
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (conn)
															#line 2878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (self->destination.isPublic() && e.code() == error_code_connection_failed)
															#line 2882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					FlowTransport::transport().healthMonitor()->reportPeerClosed(self->destination);
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (FLOW_KNOBS->HEALTH_MONITOR_MARK_FAILED_UNSTABLE_CONNECTIONS && FlowTransport::transport().healthMonitor()->tooManyConnectionsClosed(self->destination) && self->destination.isPublic())
															#line 2888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						TraceEvent("TooManyConnectionsClosedMarkFailed") .detail("Dest", self->destination) .detail( "ClosedCount", FlowTransport::transport().healthMonitor()->closedConnectionsCount(self->destination));
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						tooManyConnectionsClosed = true;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(true));
															#line 2896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				conn->close();
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				conn = Reference<IConnection>();
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (e.code() != error_code_incompatible_protocol_version)
															#line 2905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					self->protocolVersion->set(Optional<ProtocolVersion>());
															#line 2909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().notifyDisconnect(self->destination);
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			Promise<Void> disconnect = self->disconnect;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->disconnect = Promise<Void>();
															#line 872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			disconnect.send(Void());
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 2922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (self->peerReferences <= 0 && self->reliable.empty() && self->unsent.empty() && self->outstandingReplies == 0)
															#line 2930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("PeerDestroy").errorUnsuppressed(e).suppressFor(1.0).detail("PeerAddr", self->destination);
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->connect.cancel();
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->transport->peers.erase(self->destination);
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->transport->orderedAddresses.erase(self->destination);
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (!static_cast<ConnectionKeeperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionKeeperActorState(); static_cast<ConnectionKeeperActor*>(this)->destroy(); return 0; }
															#line 2942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				new (&static_cast<ConnectionKeeperActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ConnectionKeeperActorState();
				static_cast<ConnectionKeeperActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			loopDepth = a_body1loopBody1cont1(loopDepth);
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
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		firstConnFailedTime.reset();
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		try {
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->transport->countConnEstablished++;
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!delayedHealthUpdateF.isValid())
															#line 2968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				delayedHealthUpdateF = delayedHealthUpdate(self->destination, &tooManyConnectionsClosed);
															#line 2972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->connected = true;
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_8 = connectionWriter(self, conn) || reader || connectionMonitor(self) || self->resetConnection.onTrigger();
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2Catch1(actor_cancelled(), loopDepth);
															#line 2980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1loopBody1cont2Catch1(__when_expr_8.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_8.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 6;
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 8, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 2985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1cont2Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1cont2Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		ASSERT(self->destination.isPublic());
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->outgoingConnectionIdle = false;
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delayJittered(std::max(0.0, self->lastConnectTime + self->reconnectionDelay - now()));
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 2;
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 2, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!(self->unsent.empty()))
															#line 3027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		Future<Void> retryConnectF = Never();
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (retryConnect)
															#line 3035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			retryConnectF = IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() ? delay(FLOW_KNOBS->FAILURE_DETECTION_DELAY) : delay(FLOW_KNOBS->SERVER_REQUEST_INTERVAL);
															#line 3039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->dataToSend.onTrigger();
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = retryConnectF;
															#line 3049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 1;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 0, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 1, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when2(Void const& _,int loopDepth) 
	{
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when2(Void && _,int loopDepth) 
	{
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state > 0) static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 0, Void >::remove();
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->lastConnectTime = now();
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectingTo", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination) .detail("PeerReferences", self->peerReferences) .detail("FailureStatus", IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() ? "OK" : "FAILED");
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		++self->connectOutgoingCount;
															#line 3209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		try {
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(self->destination);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont4Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_4 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1cont4Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont4when2(__when_expr_4.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 3;
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 4, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1cont4Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1cont4Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->lastConnectTime = now();
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectingTo", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination) .detail("PeerReferences", self->peerReferences) .detail("FailureStatus", IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() ? "OK" : "FAILED");
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		++self->connectOutgoingCount;
															#line 3245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		try {
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(self->destination);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont4Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_4 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1cont4Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont4when2(__when_expr_4.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 3;
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 4, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1cont4Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1cont4Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state > 0) static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			++self->connectFailedCount;
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() != error_code_connection_failed)
															#line 3349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				return a_body1loopBody1Catch1(e, loopDepth);
															#line 3353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionTimedOut", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1loopBody1Catch1(e, loopDepth);
															#line 3359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Reference<IConnection> const& _conn,int loopDepth) 
	{
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		conn = _conn;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_5 = conn->connectHandshake();
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1cont4Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont4when1when1(__when_expr_5.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 4;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 5, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Reference<IConnection> && _conn,int loopDepth) 
	{
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		conn = _conn;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_5 = conn->connectHandshake();
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1cont4Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont4when1when1(__when_expr_5.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 4;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 5, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4when2(Void const& _,int loopDepth) 
	{
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont4Catch1(connection_failed(), loopDepth);
															#line 3415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont4when2(Void && _,int loopDepth) 
	{
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont4Catch1(connection_failed(), loopDepth);
															#line 3423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state > 0) static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >::remove();
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 4, Void >::remove();

	}
	int a_body1loopBody1cont4when1cont1(Void const& _,int loopDepth) 
	{
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->connectLatencies.addSample(now() - self->lastConnectTime);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (FlowTransport::isClient())
															#line 3440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(false));
															#line 3444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (self->unsent.empty())
															#line 3448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = delayedHealthUpdate(self->destination, &tooManyConnectionsClosed);
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delayedHealthUpdateF;
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1cont4Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when1(__when_expr_6.get(), loopDepth); };
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_7 = self->dataToSend.onTrigger();
															#line 3460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1cont4Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when2(__when_expr_7.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 5;
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 6, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 7, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont4when1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont1(Void && _,int loopDepth) 
	{
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->connectLatencies.addSample(now() - self->lastConnectTime);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (FlowTransport::isClient())
															#line 3483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(false));
															#line 3487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (self->unsent.empty())
															#line 3491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = delayedHealthUpdate(self->destination, &tooManyConnectionsClosed);
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delayedHealthUpdateF;
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1cont4Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when1(__when_expr_6.get(), loopDepth); };
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_7 = self->dataToSend.onTrigger();
															#line 3503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1cont4Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when2(__when_expr_7.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 5;
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 6, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 7, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont4when1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4when1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4when1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4when1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state > 0) static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4when1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4when1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose4();
		try {
			a_body1loopBody1cont4Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1loopBody1cont4when1cont2(int loopDepth) 
	{
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectionExchangingConnectPacket", conn->getDebugID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->prependConnectPacket();
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		reader = connectionReader(self->transport, conn, self, Promise<Reference<Peer>>());
															#line 3591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont4(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4when1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont1when1(Void const& _,int loopDepth) 
	{
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		conn->close();
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		conn = Reference<IConnection>();
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		retryConnect = false;
															#line 3610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont1when1(Void && _,int loopDepth) 
	{
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		conn->close();
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		conn = Reference<IConnection>();
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		retryConnect = false;
															#line 3623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4when1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4when1cont4(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state > 0) static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 6, Void >::remove();
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1cont4when1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1cont4when1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1cont4Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose5();
		try {
			a_body1loopBody1cont4when1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose5();
		try {
			a_body1loopBody1cont4when1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose5();
		try {
			a_body1loopBody1cont4Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont4Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont4Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_connection_failed || e.code() == error_code_actor_cancelled || e.code() == error_code_connection_unreferenced || (g_network->isSimulated() && e.code() == error_code_checksum_failed))
															#line 3845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->transport->countConnClosedWithoutError++;
															#line 3849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->transport->countConnClosedWithError++;
															#line 3855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1loopBody1Catch1(e, loopDepth);
															#line 3859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectionReset", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont2Catch1(connection_failed(), loopDepth);
															#line 3875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectionReset", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont2Catch1(connection_failed(), loopDepth);
															#line 3885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state > 0) static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionKeeperActor*>(this)->ActorCallback< ConnectionKeeperActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose6();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< ConnectionKeeperActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose6();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< ConnectionKeeperActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose6();
		try {
			a_body1loopBody1cont2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), 8);

	}
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> self;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> reader;
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> delayedHealthUpdateF;
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Optional<double> firstConnFailedTime;
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int retryConnect;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool tooManyConnectionsClosed;
															#line 3966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionKeeper()
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionKeeperActor final : public Actor<Void>, public ActorCallback< ConnectionKeeperActor, 0, Void >, public ActorCallback< ConnectionKeeperActor, 1, Void >, public ActorCallback< ConnectionKeeperActor, 2, Void >, public ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >, public ActorCallback< ConnectionKeeperActor, 5, Void >, public ActorCallback< ConnectionKeeperActor, 6, Void >, public ActorCallback< ConnectionKeeperActor, 7, Void >, public ActorCallback< ConnectionKeeperActor, 4, Void >, public ActorCallback< ConnectionKeeperActor, 8, Void >, public FastAllocated<ConnectionKeeperActor>, public ConnectionKeeperActorState<ConnectionKeeperActor> {
															#line 3971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ConnectionKeeperActor>::operator new;
	using FastAllocated<ConnectionKeeperActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectionKeeperActor, 0, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 1, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 2, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >;
friend struct ActorCallback< ConnectionKeeperActor, 5, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 6, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 7, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 4, Void >;
friend struct ActorCallback< ConnectionKeeperActor, 8, Void >;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionKeeperActor(Reference<Peer> const& self,Reference<IConnection> const& conn = Reference<IConnection>(),Future<Void> const& reader = Void()) 
															#line 3990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectionKeeperActorState<ConnectionKeeperActor>(self, conn, reader)
	{
		fdb_probe_actor_enter("connectionKeeper", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectionKeeper");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectionKeeper", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectionKeeperActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectionKeeperActor, 2, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ConnectionKeeperActor, 5, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ConnectionKeeperActor, 6, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ConnectionKeeperActor, 8, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> connectionKeeper( Reference<Peer> const& self, Reference<IConnection> const& conn = Reference<IConnection>(), Future<Void> const& reader = Void() ) {
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionKeeperActor(self, conn, reader));
															#line 4023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

Peer::Peer(TransportData* transport, NetworkAddress const& destination)
  : transport(transport), destination(destination), compatible(true), connected(false), outgoingConnectionIdle(true),
    lastConnectTime(0.0), reconnectionDelay(FLOW_KNOBS->INITIAL_RECONNECTION_TIME), peerReferences(-1),
    bytesReceived(0), bytesSent(0), lastDataPacketSentTime(now()), outstandingReplies(0),
    pingLatencies(destination.isPublic() ? FLOW_KNOBS->PING_SKETCH_ACCURACY : 0.1), lastLoggedTime(0.0),
    lastLoggedBytesReceived(0), lastLoggedBytesSent(0), timeoutCount(0),
    protocolVersion(Reference<AsyncVar<Optional<ProtocolVersion>>>(new AsyncVar<Optional<ProtocolVersion>>())),
    connectOutgoingCount(0), connectIncomingCount(0), connectFailedCount(0),
    connectLatencies(destination.isPublic() ? FLOW_KNOBS->PING_SKETCH_ACCURACY : 0.1) {
	IFailureMonitor::failureMonitor().setStatus(destination, FailureStatus(false));
}

void Peer::send(PacketBuffer* pb, ReliablePacket* rp, bool firstUnsent) {
	unsent.setWriteBuffer(pb);
	if (rp)
		reliable.insert(rp);
	if (firstUnsent)
		dataToSend.trigger();
}

void Peer::prependConnectPacket() {
	// Send the ConnectPacket expected at the beginning of a new connection
	ConnectPacket pkt;
	if (transport->localAddresses.getAddressList().address.isTLS() == destination.isTLS()) {
		pkt.canonicalRemotePort = transport->localAddresses.getAddressList().address.port;
		pkt.setCanonicalRemoteIp(transport->localAddresses.getAddressList().address.ip);
	} else if (transport->localAddresses.getAddressList().secondaryAddress.present()) {
		pkt.canonicalRemotePort = transport->localAddresses.getAddressList().secondaryAddress.get().port;
		pkt.setCanonicalRemoteIp(transport->localAddresses.getAddressList().secondaryAddress.get().ip);
	} else {
		// a "mixed" TLS/non-TLS connection is like a client/server connection - there's no way to reverse it
		pkt.canonicalRemotePort = 0;
		pkt.setCanonicalRemoteIp(IPAddress(0));
	}

	pkt.connectPacketLength = sizeof(pkt) - sizeof(pkt.connectPacketLength);
	pkt.protocolVersion = g_network->protocolVersion();
	pkt.protocolVersion.addObjectSerializerFlag();
	pkt.connectionId = transport->transportId;

	PacketBuffer *pb_first = PacketBuffer::create(), *pb_end = nullptr;
	PacketWriter wr(pb_first, nullptr, Unversioned());
	pkt.serialize(wr);
	pb_end = wr.finish();
#if VALGRIND
	SendBuffer* checkbuf = pb_first;
	while (checkbuf) {
		int size = checkbuf->bytes_written;
		const uint8_t* data = checkbuf->data();
		VALGRIND_CHECK_MEM_IS_DEFINED(data, size);
		checkbuf = checkbuf->next;
	}
#endif
	unsent.prependWriteBuffer(pb_first, pb_end);
}

void Peer::discardUnreliablePackets() {
	// Throw away the current unsent list, dropping the reference count on each PacketBuffer that accounts for presence
	// in the unsent list
	unsent.discardAll();

	// If there are reliable packets, compact reliable packets into a new unsent range
	if (!reliable.empty()) {
		PacketBuffer* pb = unsent.getWriteBuffer();
		pb = reliable.compact(pb, nullptr);
		unsent.setWriteBuffer(pb);
	}
}

void Peer::onIncomingConnection(Reference<Peer> self, Reference<IConnection> conn, Future<Void> reader) {
	// In case two processes are trying to connect to each other simultaneously, the process with the larger canonical
	// NetworkAddress gets to keep its outgoing connection.
	++self->connectIncomingCount;
	if (!destination.isPublic() && !outgoingConnectionIdle)
		throw address_in_use();
	NetworkAddress compatibleAddr = transport->localAddresses.getAddressList().address;
	if (transport->localAddresses.getAddressList().secondaryAddress.present() &&
	    transport->localAddresses.getAddressList().secondaryAddress.get().isTLS() == destination.isTLS()) {
		compatibleAddr = transport->localAddresses.getAddressList().secondaryAddress.get();
	}

	if (!destination.isPublic() || outgoingConnectionIdle || destination > compatibleAddr ||
	    (lastConnectTime > 1.0 && now() - lastConnectTime > FLOW_KNOBS->ALWAYS_ACCEPT_DELAY)) {
		// Keep the new connection
		TraceEvent("IncomingConnection"_audit, conn->getDebugID())
		    .suppressFor(1.0)
		    .detail("FromAddr", conn->getPeerAddress())
		    .detail("CanonicalAddr", destination)
		    .detail("IsPublic", destination.isPublic())
		    .detail("Trusted", self->transport->allowList(conn->getPeerAddress().ip) && conn->hasTrustedPeer());

		connect.cancel();
		prependConnectPacket();
		connect = connectionKeeper(self, conn, reader);
	} else {
		TraceEvent("RedundantConnection", conn->getDebugID())
		    .suppressFor(1.0)
		    .detail("FromAddr", conn->getPeerAddress().toString())
		    .detail("CanonicalAddr", destination)
		    .detail("LocalAddr", compatibleAddr);

		// Keep our prior connection
		reader.cancel();
		conn->close();

		// Send an (ignored) packet to make sure that, if our outgoing connection died before the peer made this
		// connection attempt, we eventually find out that our connection is dead, close it, and then respond to the
		// next connection reattempt from peer.
	}
}

TransportData::~TransportData() {
	for (auto& p : peers) {
		p.second->connect.cancel();
	}
}

static bool checkCompatible(const PeerCompatibilityPolicy& policy, ProtocolVersion version) {
	switch (policy.requirement) {
	case RequirePeer::Exactly:
		return version.version() == policy.version.version();
	case RequirePeer::AtLeast:
		return version.version() >= policy.version.version();
	default:
		ASSERT(false);
		return false;
	}
}

// This actor looks up the task associated with an endpoint
// and sends the message to it. The actual deserialization will
// be done by that task (see NetworkMessageReceiver).
															#line 4160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via deliver()
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class DeliverActor>
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class DeliverActorState {
															#line 4167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	DeliverActorState(TransportData* const& self,Endpoint const& destination,TaskPriority const& priority,ArenaReader const& reader,NetworkAddress const& peerAddress,bool const& isTrustedPeer,InReadSocket const& inReadSocket,Future<Void> const& disconnect) 
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   destination(destination),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   priority(priority),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   reader(reader),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   peerAddress(peerAddress),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   isTrustedPeer(isTrustedPeer),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   inReadSocket(inReadSocket),
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   disconnect(disconnect)
															#line 4188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("deliver", reinterpret_cast<unsigned long>(this));

	}
	~DeliverActorState() 
	{
		fdb_probe_actor_destroy("deliver", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (priority < TaskPriority::ReadSocket || !inReadSocket)
															#line 4203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				StrictFuture<Void> __when_expr_0 = orderedDelay(0, priority);
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 4209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				static_cast<DeliverActor*>(this)->actor_wait_state = 1;
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeliverActor, 0, Void >*>(static_cast<DeliverActor*>(this)));
															#line 4213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_network->setCurrentTask(priority);
															#line 4220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		delete static_cast<DeliverActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		auto receiver = self->endpoints.get(destination.token);
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (receiver && (isTrustedPeer || receiver->isPublic()))
															#line 4245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!checkCompatible(receiver->peerCompatibilityPolicy(), reader.protocolVersion()))
															#line 4249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				delete static_cast<DeliverActor*>(this);
															#line 4253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				return 0;
			}
			try {
															#line 1047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(g_currentDeliveryPeerAddress == NetworkAddressList());
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(!g_currentDeliverPeerAddressTrusted);
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerAddress = destination.addresses;
															#line 1050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliverPeerAddressTrusted = isTrustedPeer;
															#line 1051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerDisconnect = disconnect;
															#line 1052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				StringRef data = reader.arenaReadAll();
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(data.size() > 8);
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ArenaObjectReader objReader(reader.arena(), reader.arenaReadAll(), AssumeVersion(reader.protocolVersion()));
															#line 1055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				receiver->receive(objReader);
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerAddress = NetworkAddressList();
															#line 1057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliverPeerAddressTrusted = false;
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerDisconnect = Future<Void>();
															#line 4281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = a_body1cont8(loopDepth);
			}
			catch (Error& error) {
				loopDepth = a_body1cont1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (destination.token.first() & TOKEN_STREAM_FLAG)
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (receiver)
															#line 4298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent(SevWarnAlways, "AttemptedRPCToPrivatePrevented"_audit) .detail("From", peerAddress) .detail("Token", destination.token) .detail("Receiver", typeid(*receiver).name());
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					ASSERT(!self->isLocalAddress(destination.getPrimaryAddress()));
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					Reference<Peer> peer = self->getOrOpenPeer(destination.getPrimaryAddress());
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					sendPacket(self, peer, SerializeSource<UID>(destination.token), Endpoint::wellKnown(destination.addresses, WLTOKEN_UNAUTHORIZED_ENDPOINT), false);
															#line 4308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
				else
				{
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (destination.token.first() != -1)
															#line 4314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						if (self->isLocalAddress(destination.getPrimaryAddress()))
															#line 4318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							sendLocal(self, SerializeSource<UID>(destination.token), Endpoint::wellKnown(destination.addresses, WLTOKEN_ENDPOINT_NOT_FOUND));
															#line 4322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						}
						else
						{
															#line 1093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							Reference<Peer> peer = self->getOrOpenPeer(destination.getPrimaryAddress());
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							sendPacket(self, peer, SerializeSource<UID>(destination.token), Endpoint::wellKnown(destination.addresses, WLTOKEN_ENDPOINT_NOT_FOUND), false);
															#line 4330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						}
					}
				}
			}
			loopDepth = a_body1cont4(loopDepth);
		}

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
		if (static_cast<DeliverActor*>(this)->actor_wait_state > 0) static_cast<DeliverActor*>(this)->actor_wait_state = 0;
		static_cast<DeliverActor*>(this)->ActorCallback< DeliverActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeliverActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deliver", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deliver", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeliverActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deliver", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deliver", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeliverActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deliver", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deliver", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
	{
		loopDepth = a_body1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			g_currentDeliveryPeerAddress = NetworkAddressList();
															#line 1061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			g_currentDeliverPeerAddressTrusted = false;
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			g_currentDeliveryPeerDisconnect = Future<Void>();
															#line 1063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevError, "ReceiverError") .error(e) .detail("Token", destination.token.toString()) .detail("Peer", destination.getPrimaryAddress());
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!FlowTransport::isClient())
															#line 4440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				flushAndExit(FDB_EXIT_ERROR);
															#line 4444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 4448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont16(int loopDepth) 
	{
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		delete static_cast<DeliverActor*>(this);
															#line 4475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Endpoint destination;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TaskPriority priority;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ArenaReader reader;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress peerAddress;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool isTrustedPeer;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	InReadSocket inReadSocket;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> disconnect;
															#line 4496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via deliver()
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class DeliverActor final : public Actor<void>, public ActorCallback< DeliverActor, 0, Void >, public FastAllocated<DeliverActor>, public DeliverActorState<DeliverActor> {
															#line 4501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<DeliverActor>::operator new;
	using FastAllocated<DeliverActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DeliverActor, 0, Void >;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	DeliverActor(TransportData* const& self,Endpoint const& destination,TaskPriority const& priority,ArenaReader const& reader,NetworkAddress const& peerAddress,bool const& isTrustedPeer,InReadSocket const& inReadSocket,Future<Void> const& disconnect) 
															#line 4512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<void>(),
		   DeliverActorState<DeliverActor>(self, destination, priority, reader, peerAddress, isTrustedPeer, inReadSocket, disconnect)
	{
		fdb_probe_actor_enter("deliver", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deliver");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deliver", reinterpret_cast<unsigned long>(this), -1);

	}
};
}
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
static void deliver( TransportData* const& self, Endpoint const& destination, TaskPriority const& priority, ArenaReader const& reader, NetworkAddress const& peerAddress, bool const& isTrustedPeer, InReadSocket const& inReadSocket, Future<Void> const& disconnect ) {
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	new DeliverActor(self, destination, priority, reader, peerAddress, isTrustedPeer, inReadSocket, disconnect);
															#line 4531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

static void scanPackets(TransportData* transport,
                        uint8_t*& unprocessed_begin,
                        const uint8_t* e,
                        Arena& arena,
                        NetworkAddress const& peerAddress,
                        bool isTrustedPeer,
                        ProtocolVersion peerProtocolVersion,
                        Future<Void> disconnect,
                        IsStableConnection isStableConnection) {
	// Find each complete packet in the given byte range and queue a ready task to deliver it.
	// Remove the complete packets from the range by increasing unprocessed_begin.
	// There won't be more than 64K of data plus one packet, so this shouldn't take a long time.
	uint8_t* p = unprocessed_begin;

	const bool checksumEnabled = !peerAddress.isTLS();
	loop {
		uint32_t packetLen;
		XXH64_hash_t packetChecksum;

		// Read packet length if size is sufficient or stop
		if (e - p < PACKET_LEN_WIDTH)
			break;
		packetLen = *(uint32_t*)p;
		p += PACKET_LEN_WIDTH;

		// Read checksum if present
		if (checksumEnabled) {
			// Read checksum if size is sufficient or stop
			if (e - p < sizeof(packetChecksum))
				break;
			packetChecksum = *(XXH64_hash_t*)p;
			p += sizeof(packetChecksum);
		}

		if (packetLen > FLOW_KNOBS->PACKET_LIMIT) {
			TraceEvent(SevError, "PacketLimitExceeded")
			    .detail("FromPeer", peerAddress.toString())
			    .detail("Length", (int)packetLen);
			throw platform_error();
		}

		if (e - p < packetLen)
			break;

		if (packetLen < sizeof(UID)) {
			if (g_network->isSimulated()) {
				// Same as ASSERT(false), but prints packet length:
				ASSERT_GE(packetLen, sizeof(UID));
			} else {
				TraceEvent(SevError, "PacketTooSmall")
				    .detail("FromPeer", peerAddress.toString())
				    .detail("Length", packetLen);
				throw platform_error();
			}
		}

		if (checksumEnabled) {
			bool isBuggifyEnabled = false;
			if (g_network->isSimulated() && !isStableConnection &&
			    g_network->now() - g_simulator->lastConnectionFailure >
			        g_simulator->connectionFailuresDisableDuration &&
			    BUGGIFY_WITH_PROB(0.0001)) {
				g_simulator->lastConnectionFailure = g_network->now();
				isBuggifyEnabled = true;
				TraceEvent(SevInfo, "BitsFlip").log();
				int flipBits = 32 - (int)floor(log2(deterministicRandom()->randomUInt32()));

				uint32_t firstFlipByteLocation = deterministicRandom()->randomUInt32() % packetLen;
				int firstFlipBitLocation = deterministicRandom()->randomInt(0, 8);
				*(p + firstFlipByteLocation) ^= 1 << firstFlipBitLocation;
				flipBits--;

				for (int i = 0; i < flipBits; i++) {
					uint32_t byteLocation = deterministicRandom()->randomUInt32() % packetLen;
					int bitLocation = deterministicRandom()->randomInt(0, 8);
					if (byteLocation != firstFlipByteLocation || bitLocation != firstFlipBitLocation) {
						*(p + byteLocation) ^= 1 << bitLocation;
					}
				}
			}

			XXH64_hash_t calculatedChecksum = XXH3_64bits(p, packetLen);
			if (calculatedChecksum != packetChecksum) {
				if (isBuggifyEnabled) {
					TraceEvent(SevInfo, "ChecksumMismatchExp")
					    .detail("PacketChecksum", packetChecksum)
					    .detail("CalculatedChecksum", calculatedChecksum)
					    .detail("PeerAddress", peerAddress.toString());
				} else {
					TraceEvent(SevWarnAlways, "ChecksumMismatchUnexp")
					    .detail("PacketChecksum", packetChecksum)
					    .detail("CalculatedChecksum", calculatedChecksum);
				}
				throw checksum_failed();
			} else {
				if (isBuggifyEnabled) {
					TraceEvent(SevError, "ChecksumMatchUnexp")
					    .detail("PacketChecksum", packetChecksum)
					    .detail("CalculatedChecksum", calculatedChecksum);
				}
			}
		}

#if VALGRIND
		VALGRIND_CHECK_MEM_IS_DEFINED(p, packetLen);
#endif
		// remove object serializer flag to account for flat buffer
		peerProtocolVersion.removeObjectSerializerFlag();
		ArenaReader reader(arena, StringRef(p, packetLen), AssumeVersion(peerProtocolVersion));
		UID token;
		reader >> token;

		++transport->countPacketsReceived;

		if (packetLen > FLOW_KNOBS->PACKET_WARNING) {
			TraceEvent(SevWarn, "LargePacketReceived")
			    .suppressFor(1.0)
			    .detail("FromPeer", peerAddress.toString())
			    .detail("Length", (int)packetLen)
			    .detail("Token", token);
		}

		ASSERT(!reader.empty());
		TaskPriority priority = transport->endpoints.getPriority(token);
		// we ignore packets to unknown endpoints if they're not going to a stream anyways, so we can just
		// return here. The main place where this seems to happen is if a ReplyPromise is not waited on
		// long enough.
		// It would be slightly more elegant/readable to put this if-block into the deliver actor, but if
		// we have many messages to UnknownEndpoint we want to optimize earlier. As deliver is an actor it
		// will allocate some state on the heap and this prevents it from doing that.
		if (priority != TaskPriority::UnknownEndpoint || (token.first() & TOKEN_STREAM_FLAG) != 0) {
			deliver(transport,
			        Endpoint({ peerAddress }, token),
			        priority,
			        std::move(reader),
			        peerAddress,
			        isTrustedPeer,
			        InReadSocket::True,
			        disconnect);
		}

		unprocessed_begin = p = p + packetLen;
	}
}

// Given unprocessed buffer [begin, end), check if next packet size is known and return
// enough size for the next packet, whose format is: {size, optional_checksum, data} +
// next_packet_size.
static int getNewBufferSize(const uint8_t* begin,
                            const uint8_t* end,
                            const NetworkAddress& peerAddress,
                            ProtocolVersion peerProtocolVersion) {
	const int len = end - begin;
	if (len < PACKET_LEN_WIDTH) {
		return FLOW_KNOBS->MIN_PACKET_BUFFER_BYTES;
	}
	const uint32_t packetLen = *(uint32_t*)begin;
	if (packetLen > FLOW_KNOBS->PACKET_LIMIT) {
		TraceEvent(SevError, "PacketLimitExceeded")
		    .detail("FromPeer", peerAddress.toString())
		    .detail("Length", (int)packetLen);
		throw platform_error();
	}
	return std::max<uint32_t>(FLOW_KNOBS->MIN_PACKET_BUFFER_BYTES,
	                          packetLen + sizeof(uint32_t) * (peerAddress.isTLS() ? 2 : 3));
}

// This actor exists whenever there is an open or opening connection, whether incoming or outgoing
// For incoming connections conn is set and peer is initially nullptr; for outgoing connections it is the reverse
															#line 4705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionReader()
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionReaderActor>
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionReaderActorState {
															#line 4712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionReaderActorState(TransportData* const& transport,Reference<IConnection> const& conn,Reference<Peer> const& peer,Promise<Reference<Peer>> const& onConnected) 
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : transport(transport),
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn),
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   peer(peer),
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   onConnected(onConnected),
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   arena(),
															#line 1280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   unprocessed_begin(nullptr),
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   unprocessed_end(nullptr),
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   buffer_end(nullptr),
															#line 1283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   expectConnectPacket(true),
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   compatible(false),
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   incompatiblePeerCounted(false),
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   peerAddress(),
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   peerProtocolVersion(),
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   trusted(transport->allowList(conn->getPeerAddress().ip) && conn->hasTrustedPeer())
															#line 4745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("connectionReader", reinterpret_cast<unsigned long>(this));

	}
	~ConnectionReaderActorState() 
	{
		fdb_probe_actor_destroy("connectionReader", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			peerAddress = conn->getPeerAddress();
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!peer)
															#line 4762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(!peerAddress.isPublic());
															#line 4766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			try {
															#line 1295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				;
															#line 4771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
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
		this->~ConnectionReaderActorState();
		static_cast<ConnectionReaderActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (incompatiblePeerCounted)
															#line 4801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(peer && peer->transport->numIncompatibleConnections > 0);
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->transport->numIncompatibleConnections--;
															#line 4807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 4811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

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
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 4832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = conn->onReadable();
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 4;
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 3, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 4848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		readAllBytes = buffer_end - unprocessed_end;
															#line 1298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (readAllBytes < FLOW_KNOBS->MIN_PACKET_BUFFER_FREE_BYTES)
															#line 4866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			Arena newArena;
															#line 1300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			const int unproc_len = unprocessed_end - unprocessed_begin;
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			const int len = getNewBufferSize(unprocessed_begin, unprocessed_end, peerAddress, peerProtocolVersion);
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			uint8_t* const newBuffer = new (newArena) uint8_t[len];
															#line 1304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (unproc_len > 0)
															#line 4878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				memcpy(newBuffer, unprocessed_begin, unproc_len);
															#line 4882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			arena = newArena;
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			unprocessed_begin = newBuffer;
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			unprocessed_end = newBuffer + unproc_len;
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			buffer_end = newBuffer + len;
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			readAllBytes = buffer_end - unprocessed_end;
															#line 4894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		totalReadBytes = 0;
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 4900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
															#line 1326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (peer)
															#line 4922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			peer->bytesReceived += totalReadBytes;
															#line 4926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (totalReadBytes == 0)
															#line 4930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		readWillBlock = totalReadBytes != readAllBytes;
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (expectConnectPacket && unprocessed_end - unprocessed_begin >= CONNECT_PACKET_V0_SIZE)
															#line 4938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			int32_t connectPacketSize = ((ConnectPacket*)unprocessed_begin)->totalPacketSize();
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (unprocessed_end - unprocessed_begin >= connectPacketSize)
															#line 4944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				auto protocolVersion = ((ConnectPacket*)unprocessed_begin)->protocolVersion;
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				BinaryReader pktReader(unprocessed_begin, connectPacketSize, AssumeVersion(protocolVersion));
															#line 1340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				ConnectPacket pkt;
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				serializer(pktReader, pkt);
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				uint64_t connectionId = pkt.connectionId;
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (!pkt.protocolVersion.hasObjectSerializerFlag() || !pkt.protocolVersion.isCompatible(g_network->protocolVersion()))
															#line 4958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					NetworkAddress addr = pkt.canonicalRemotePort ? NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort) : conn->getPeerAddress();
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (connectionId != 1)
															#line 4964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						addr.port = 0;
															#line 4968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (!transport->multiVersionConnections.count(connectionId))
															#line 4972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						if (now() - transport->lastIncompatibleMessage > FLOW_KNOBS->CONNECTION_REJECTED_MESSAGE_DELAY)
															#line 4976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							TraceEvent(SevWarn, "ConnectionRejected", conn->getDebugID()) .detail("Reason", "IncompatibleProtocolVersion") .detail("LocalVersion", g_network->protocolVersion()) .detail("RejectedVersion", pkt.protocolVersion) .detail("Peer", pkt.canonicalRemotePort ? NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort) : conn->getPeerAddress()) .detail("ConnectionId", connectionId);
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							transport->lastIncompatibleMessage = now();
															#line 4982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						}
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						if (!transport->incompatiblePeers.count(addr))
															#line 4986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							transport->incompatiblePeers[addr] = std::make_pair(connectionId, now());
															#line 4990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						}
					}
					else
					{
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						if (connectionId > 1)
															#line 4997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							transport->multiVersionConnections[connectionId] = now() + FLOW_KNOBS->CONNECTION_ID_TIMEOUT;
															#line 5001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						}
					}
															#line 1373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					compatible = false;
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (!protocolVersion.hasInexpensiveMultiVersionClient())
															#line 5008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						if (peer)
															#line 5012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
							peer->protocolVersion->set(protocolVersion);
															#line 5016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
						}
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						return a_body1Catch2(incompatible_protocol_version(), std::max(0, loopDepth - 2));
															#line 5020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
				else
				{
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					compatible = true;
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent("ConnectionEstablished", conn->getDebugID()) .suppressFor(1.0) .detail("Peer", conn->getPeerAddress()) .detail("ConnectionId", connectionId);
															#line 5029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (connectionId > 1)
															#line 5033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					transport->multiVersionConnections[connectionId] = now() + FLOW_KNOBS->CONNECTION_ID_TIMEOUT;
															#line 5037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				unprocessed_begin += connectPacketSize;
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				expectConnectPacket = false;
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (peer)
															#line 5045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					peerProtocolVersion = protocolVersion;
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent("ConnectedOutgoing") .suppressFor(1.0) .detail("PeerAddr", NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort));
															#line 1404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					peer->compatible = compatible;
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (!compatible)
															#line 5055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						peer->transport->numIncompatibleConnections++;
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						incompatiblePeerCounted = true;
															#line 5061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					ASSERT(pkt.canonicalRemotePort == peerAddress.port);
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					onConnected.send(peer);
															#line 5067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					loopDepth = a_body1loopBody1loopBody1cont8(loopDepth);
				}
				else
				{
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					peerProtocolVersion = protocolVersion;
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (pkt.canonicalRemotePort)
															#line 5076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						peerAddress = NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort, true, peerAddress.isTLS(), NetworkAddressFromHostname(peerAddress.fromHostname));
															#line 5080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					peer = transport->getOrOpenPeer(peerAddress, false);
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					peer->compatible = compatible;
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (!compatible)
															#line 5088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						peer->transport->numIncompatibleConnections++;
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
						incompatiblePeerCounted = true;
															#line 5094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					onConnected.send(peer);
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					StrictFuture<Void> __when_expr_1 = delay(0);
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 5102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
					static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 2;
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 1, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
					loopDepth = 0;
				}
			}
			else
			{
				loopDepth = a_body1loopBody1loopBody1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1(int loopDepth) 
	{
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		const int len = std::min<int>(buffer_end - unprocessed_end, FLOW_KNOBS->MAX_PACKET_SEND_BYTES);
															#line 1317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (len == 0)
															#line 5136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		readBytes = conn->read(unprocessed_end, unprocessed_end + len);
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (readBytes == 0)
															#line 5144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = yield(TaskPriority::ReadSocket);
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 3));
															#line 5152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 3)); else return a_body1loopBody1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 1;
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 0, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		totalReadBytes += readBytes;
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		unprocessed_end += readBytes;
															#line 5181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		totalReadBytes += readBytes;
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		unprocessed_end += readBytes;
															#line 5192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state > 0) static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionReaderActor*>(this)->ActorCallback< ConnectionReaderActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectionReaderActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1loopBody1cont4(int loopDepth) 
	{
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!expectConnectPacket)
															#line 5264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (compatible || peerProtocolVersion.hasStableInterfaces())
															#line 5268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				scanPackets(transport, unprocessed_begin, unprocessed_end, arena, peerAddress, trusted, peerProtocolVersion, peer->disconnect.getFuture(), IsStableConnection(g_network->isSimulated() && conn->isStableConnection()));
															#line 5272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				unprocessed_begin = unprocessed_end;
															#line 1446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				peer->resetPing.trigger();
															#line 5280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (readWillBlock)
															#line 5285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::ReadSocket);
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 5293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 3;
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 2, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont8(int loopDepth) 
	{
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		peer->protocolVersion->set(peerProtocolVersion);
															#line 5313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont22(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont22(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont22(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont22(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state > 0) static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionReaderActor*>(this)->ActorCallback< ConnectionReaderActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectionReaderActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1loopBody1cont26(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont26(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont26(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont26(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state > 0) static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionReaderActor*>(this)->ActorCallback< ConnectionReaderActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectionReaderActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(0, TaskPriority::ReadSocket);
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 5;
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 4, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(0, TaskPriority::ReadSocket);
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 5;
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 4, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state > 0) static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionReaderActor*>(this)->ActorCallback< ConnectionReaderActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ConnectionReaderActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

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
	void a_exitChoose5() 
	{
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state > 0) static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionReaderActor*>(this)->ActorCallback< ConnectionReaderActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ConnectionReaderActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ConnectionReaderActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* transport;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> peer;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Promise<Reference<Peer>> onConnected;
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Arena arena;
															#line 1280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	uint8_t* unprocessed_begin;
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	uint8_t* unprocessed_end;
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	uint8_t* buffer_end;
															#line 1283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool expectConnectPacket;
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool compatible;
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool incompatiblePeerCounted;
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress peerAddress;
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ProtocolVersion peerProtocolVersion;
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool trusted;
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int readAllBytes;
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int totalReadBytes;
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int readBytes;
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	bool readWillBlock;
															#line 5674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionReader()
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionReaderActor final : public Actor<Void>, public ActorCallback< ConnectionReaderActor, 0, Void >, public ActorCallback< ConnectionReaderActor, 1, Void >, public ActorCallback< ConnectionReaderActor, 2, Void >, public ActorCallback< ConnectionReaderActor, 3, Void >, public ActorCallback< ConnectionReaderActor, 4, Void >, public FastAllocated<ConnectionReaderActor>, public ConnectionReaderActorState<ConnectionReaderActor> {
															#line 5679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ConnectionReaderActor>::operator new;
	using FastAllocated<ConnectionReaderActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectionReaderActor, 0, Void >;
friend struct ActorCallback< ConnectionReaderActor, 1, Void >;
friend struct ActorCallback< ConnectionReaderActor, 2, Void >;
friend struct ActorCallback< ConnectionReaderActor, 3, Void >;
friend struct ActorCallback< ConnectionReaderActor, 4, Void >;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionReaderActor(TransportData* const& transport,Reference<IConnection> const& conn,Reference<Peer> const& peer,Promise<Reference<Peer>> const& onConnected) 
															#line 5694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectionReaderActorState<ConnectionReaderActor>(transport, conn, peer, onConnected)
	{
		fdb_probe_actor_enter("connectionReader", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectionReader");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectionReader", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectionReaderActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectionReaderActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ConnectionReaderActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ConnectionReaderActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ConnectionReaderActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> connectionReader( TransportData* const& transport, Reference<IConnection> const& conn, Reference<Peer> const& peer, Promise<Reference<Peer>> const& onConnected ) {
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionReaderActor(transport, conn, peer, onConnected));
															#line 5726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

															#line 5731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionIncoming()
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionIncomingActor>
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionIncomingActorState {
															#line 5738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionIncomingActorState(TransportData* const& self,Reference<IConnection> const& conn) 
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn)
															#line 5747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("connectionIncoming", reinterpret_cast<unsigned long>(this));

	}
	~ConnectionIncomingActorState() 
	{
		fdb_probe_actor_destroy("connectionIncoming", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				StrictFuture<Void> __when_expr_0 = conn->acceptHandshake();
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 1;
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 0, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 5770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() != error_code_actor_cancelled)
															#line 5800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("IncomingConnectionError", conn->getDebugID()) .errorUnsuppressed(e) .suppressFor(1.0) .detail("FromAddress", conn->getPeerAddress());
															#line 5804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			conn->close();
															#line 1496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ConnectionIncomingActorState();
			static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		onConnected = Promise<Reference<Peer>>();
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		reader = connectionReader(self, conn, Reference<Peer>(), onConnected);
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reader;
															#line 1474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<Peer>> __when_expr_2 = onConnected.getFuture();
															#line 5838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont2when2(__when_expr_2.get(), loopDepth); };
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 5842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont2when3(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 2;
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 1, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 3, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 5851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		onConnected = Promise<Reference<Peer>>();
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		reader = connectionReader(self, conn, Reference<Peer>(), onConnected);
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reader;
															#line 1474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<Peer>> __when_expr_2 = onConnected.getFuture();
															#line 5870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont2when2(__when_expr_2.get(), loopDepth); };
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 5874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont2when3(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 2;
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 1, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 3, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 5883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state > 0) static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionIncomingActor*>(this)->ActorCallback< ConnectionIncomingActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConnectionIncomingActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		ASSERT(false);
															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		ASSERT(false);
															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when2(Reference<Peer> const& p,int loopDepth) 
	{
															#line 1480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		p->onIncomingConnection(p, conn, reader);
															#line 5995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(Reference<Peer> && p,int loopDepth) 
	{
															#line 1480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		p->onIncomingConnection(p, conn, reader);
															#line 6004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when3(Void const& _,int loopDepth) 
	{
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		CODE_PROBE(true, "Incoming connection timed out");
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		return a_body1Catch2(timed_out(), loopDepth);
															#line 6015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2when3(Void && _,int loopDepth) 
	{
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		CODE_PROBE(true, "Incoming connection timed out");
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		return a_body1Catch2(timed_out(), loopDepth);
															#line 6025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state > 0) static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 0;
		static_cast<ConnectionIncomingActor*>(this)->ActorCallback< ConnectionIncomingActor, 1, Void >::remove();
		static_cast<ConnectionIncomingActor*>(this)->ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >::remove();
		static_cast<ConnectionIncomingActor*>(this)->ActorCallback< ConnectionIncomingActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConnectionIncomingActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*,Reference<Peer> const& value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*,Reference<Peer> && value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*,Error err) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose2();
		try {
			a_body1cont2when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ConnectionIncomingActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose2();
		try {
			a_body1cont2when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ConnectionIncomingActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Promise<Reference<Peer>> onConnected;
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> reader;
															#line 6180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionIncoming()
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ConnectionIncomingActor final : public Actor<Void>, public ActorCallback< ConnectionIncomingActor, 0, Void >, public ActorCallback< ConnectionIncomingActor, 1, Void >, public ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >, public ActorCallback< ConnectionIncomingActor, 3, Void >, public FastAllocated<ConnectionIncomingActor>, public ConnectionIncomingActorState<ConnectionIncomingActor> {
															#line 6185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ConnectionIncomingActor>::operator new;
	using FastAllocated<ConnectionIncomingActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConnectionIncomingActor, 0, Void >;
friend struct ActorCallback< ConnectionIncomingActor, 1, Void >;
friend struct ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >;
friend struct ActorCallback< ConnectionIncomingActor, 3, Void >;
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ConnectionIncomingActor(TransportData* const& self,Reference<IConnection> const& conn) 
															#line 6199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   ConnectionIncomingActorState<ConnectionIncomingActor>(self, conn)
	{
		fdb_probe_actor_enter("connectionIncoming", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("connectionIncoming");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("connectionIncoming", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConnectionIncomingActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConnectionIncomingActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> connectionIncoming( TransportData* const& self, Reference<IConnection> const& conn ) {
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionIncomingActor(self, conn));
															#line 6228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

															#line 6233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via listen()
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class ListenActor>
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ListenActorState {
															#line 6240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ListenActorState(TransportData* const& self,NetworkAddress const& listenAddr) 
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   listenAddr(listenAddr),
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   incoming(),
															#line 1503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   listener(INetworkConnections::net()->listen(listenAddr))
															#line 6253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("listen", reinterpret_cast<unsigned long>(this));

	}
	~ListenActorState() 
	{
		fdb_probe_actor_destroy("listen", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (!g_network->isSimulated() && self->localAddresses.getAddressList().address.port == 0)
															#line 6268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent(SevInfo, "UpdatingListenAddress") .detail("AssignedListenAddress", listener->getListenAddress().toString());
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				self->localAddresses.setNetworkAddress(listener->getListenAddress());
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				setTraceLocalAddress(listener->getListenAddress());
															#line 6276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			connectionCount = 0;
															#line 6280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			try {
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				;
															#line 6284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
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
		this->~ListenActorState();
		static_cast<ListenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevError, "ListenError").error(e);
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 6316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

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
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_0 = listener->accept();
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ListenActor*>(this)->actor_wait_state = 1;
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListenActor, 0, Reference<IConnection> >*>(static_cast<ListenActor*>(this)));
															#line 6344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Reference<IConnection> const& conn,int loopDepth) 
	{
															#line 1514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (conn)
															#line 6353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionFrom", conn->getDebugID()) .suppressFor(1.0) .detail("FromAddress", conn->getPeerAddress()) .detail("ListenAddress", listenAddr.toString());
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			incoming.add(connectionIncoming(self, conn));
															#line 6359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		connectionCount++;
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (connectionCount % (FLOW_KNOBS->ACCEPT_BATCH_SIZE) == 0)
															#line 6365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::AcceptSocket);
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ListenActor*>(this)->actor_wait_state = 2;
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListenActor, 1, Void >*>(static_cast<ListenActor*>(this)));
															#line 6376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(Reference<IConnection> && conn,int loopDepth) 
	{
															#line 1514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (conn)
															#line 6390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionFrom", conn->getDebugID()) .suppressFor(1.0) .detail("FromAddress", conn->getPeerAddress()) .detail("ListenAddress", listenAddr.toString());
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			incoming.add(connectionIncoming(self, conn));
															#line 6396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		connectionCount++;
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (connectionCount % (FLOW_KNOBS->ACCEPT_BATCH_SIZE) == 0)
															#line 6402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::AcceptSocket);
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ListenActor*>(this)->actor_wait_state = 2;
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListenActor, 1, Void >*>(static_cast<ListenActor*>(this)));
															#line 6413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IConnection> const& conn,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(conn, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IConnection> && conn,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(conn), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListenActor*>(this)->actor_wait_state > 0) static_cast<ListenActor*>(this)->actor_wait_state = 0;
		static_cast<ListenActor*>(this)->ActorCallback< ListenActor, 0, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< ListenActor, 0, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListenActor, 0, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListenActor, 0, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
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
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ListenActor*>(this)->actor_wait_state > 0) static_cast<ListenActor*>(this)->actor_wait_state = 0;
		static_cast<ListenActor*>(this)->ActorCallback< ListenActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ListenActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ListenActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ListenActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress listenAddr;
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ActorCollectionNoErrors incoming;
															#line 1503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<IListener> listener;
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	uint64_t connectionCount;
															#line 6577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via listen()
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class ListenActor final : public Actor<Void>, public ActorCallback< ListenActor, 0, Reference<IConnection> >, public ActorCallback< ListenActor, 1, Void >, public FastAllocated<ListenActor>, public ListenActorState<ListenActor> {
															#line 6582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ListenActor>::operator new;
	using FastAllocated<ListenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListenActor, 0, Reference<IConnection> >;
friend struct ActorCallback< ListenActor, 1, Void >;
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	ListenActor(TransportData* const& self,NetworkAddress const& listenAddr) 
															#line 6594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   ListenActorState<ListenActor>(self, listenAddr)
	{
		fdb_probe_actor_enter("listen", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listen");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listen", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListenActor, 0, Reference<IConnection> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ListenActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> listen( TransportData* const& self, NetworkAddress const& listenAddr ) {
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ListenActor(self, listenAddr));
															#line 6623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

Reference<Peer> TransportData::getPeer(NetworkAddress const& address) {
	auto peer = peers.find(address);
	if (peer != peers.end()) {
		return peer->second;
	}
	return Reference<Peer>();
}

Reference<Peer> TransportData::getOrOpenPeer(NetworkAddress const& address, bool startConnectionKeeper) {
	auto peer = getPeer(address);
	if (!peer) {
		peer = makeReference<Peer>(this, address);
		if (startConnectionKeeper && !isLocalAddress(address)) {
			peer->connect = connectionKeeper(peer);
		}
		peers[address] = peer;
		if (address.isPublic()) {
			orderedAddresses.insert(address);
		}
	}

	return peer;
}

bool TransportData::isLocalAddress(const NetworkAddress& address) const {
	return address == localAddresses.getAddressList().address ||
	       (localAddresses.getAddressList().secondaryAddress.present() &&
	        address == localAddresses.getAddressList().secondaryAddress.get());
}

void TransportData::applyPublicKeySet(StringRef jwkSetString) {
	auto jwks = JsonWebKeySet::parse(jwkSetString, {});
	if (!jwks.present())
		throw pkey_decode_error();
	const auto& keySet = jwks.get().keys;
	publicKeys.clear();
	int numPrivateKeys = 0;
	for (auto [keyName, key] : keySet) {
		// ignore private keys
		if (key.isPublic()) {
			publicKeys[keyName] = key.getPublic();
		} else {
			numPrivateKeys++;
		}
	}
	TraceEvent(SevInfo, "AuthzPublicKeySetApply"_audit).detail("NumPublicKeys", publicKeys.size());
	if (numPrivateKeys > 0) {
		TraceEvent(SevWarnAlways, "AuthzPublicKeySetContainsPrivateKeys").detail("NumPrivateKeys", numPrivateKeys);
	}
}

															#line 6679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via multiVersionCleanupWorker()
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class MultiVersionCleanupWorkerActor>
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class MultiVersionCleanupWorkerActorState {
															#line 6686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	MultiVersionCleanupWorkerActorState(TransportData* const& self) 
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : self(self)
															#line 6693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this));

	}
	~MultiVersionCleanupWorkerActorState() 
	{
		fdb_probe_actor_destroy("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 6708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~MultiVersionCleanupWorkerActorState();
		static_cast<MultiVersionCleanupWorkerActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_CLEANUP_DELAY);
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<MultiVersionCleanupWorkerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MultiVersionCleanupWorkerActor*>(this)->actor_wait_state = 1;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >*>(static_cast<MultiVersionCleanupWorkerActor*>(this)));
															#line 6745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		bool foundIncompatible = false;
															#line 1587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->incompatiblePeers.begin();it != self->incompatiblePeers.end();) {
															#line 1588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (self->multiVersionConnections.count(it->second.first))
															#line 6758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it = self->incompatiblePeers.erase(it);
															#line 6762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (now() - it->second.second > FLOW_KNOBS->INCOMPATIBLE_PEER_DELAY_BEFORE_LOGGING)
															#line 6768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					foundIncompatible = true;
															#line 6772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->multiVersionConnections.begin();it != self->multiVersionConnections.end();) {
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (it->second < now())
															#line 6783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it = self->multiVersionConnections.erase(it);
															#line 6787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (foundIncompatible)
															#line 6798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->incompatiblePeersChanged.trigger();
															#line 6802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		bool foundIncompatible = false;
															#line 1587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->incompatiblePeers.begin();it != self->incompatiblePeers.end();) {
															#line 1588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (self->multiVersionConnections.count(it->second.first))
															#line 6816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it = self->incompatiblePeers.erase(it);
															#line 6820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				if (now() - it->second.second > FLOW_KNOBS->INCOMPATIBLE_PEER_DELAY_BEFORE_LOGGING)
															#line 6826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
					foundIncompatible = true;
															#line 6830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->multiVersionConnections.begin();it != self->multiVersionConnections.end();) {
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (it->second < now())
															#line 6841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it = self->multiVersionConnections.erase(it);
															#line 6845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (foundIncompatible)
															#line 6856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			self->incompatiblePeersChanged.trigger();
															#line 6860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MultiVersionCleanupWorkerActor*>(this)->actor_wait_state > 0) static_cast<MultiVersionCleanupWorkerActor*>(this)->actor_wait_state = 0;
		static_cast<MultiVersionCleanupWorkerActor*>(this)->ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 6931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via multiVersionCleanupWorker()
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class MultiVersionCleanupWorkerActor final : public Actor<Void>, public ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >, public FastAllocated<MultiVersionCleanupWorkerActor>, public MultiVersionCleanupWorkerActorState<MultiVersionCleanupWorkerActor> {
															#line 6936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<MultiVersionCleanupWorkerActor>::operator new;
	using FastAllocated<MultiVersionCleanupWorkerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >;
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	MultiVersionCleanupWorkerActor(TransportData* const& self) 
															#line 6947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   MultiVersionCleanupWorkerActorState<MultiVersionCleanupWorkerActor>(self)
	{
		fdb_probe_actor_enter("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("multiVersionCleanupWorker");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("multiVersionCleanupWorker", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> multiVersionCleanupWorker( TransportData* const& self ) {
															#line 1583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new MultiVersionCleanupWorkerActor(self));
															#line 6975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

FlowTransport::FlowTransport(uint64_t transportId, int maxWellKnownEndpoints, IPAllowList const* allowList)
  : self(new TransportData(transportId, maxWellKnownEndpoints, allowList)) {
	self->multiVersionCleanup = multiVersionCleanupWorker(self);
	if (g_network->isSimulated()) {
		for (auto const& p : g_simulator->authKeys) {
			self->publicKeys.emplace(p.first, p.second.toPublic());
		}
	}
}

FlowTransport::~FlowTransport() {
	delete self;
}

void FlowTransport::initMetrics() {
	self->initMetrics();
}

NetworkAddressList FlowTransport::getLocalAddresses() const {
	return self->localAddresses.getAddressList();
}

NetworkAddress FlowTransport::getLocalAddress() const {
	return self->localAddresses.getAddressList().address;
}

Standalone<StringRef> FlowTransport::getLocalAddressAsString() const {
	return self->localAddresses.getLocalAddressAsString();
}

void FlowTransport::setLocalAddress(NetworkAddress const& address) {
	auto newAddress = self->localAddresses.getAddressList();
	newAddress.address = address;
	self->localAddresses.setAddressList(newAddress);
}

const std::unordered_map<NetworkAddress, Reference<Peer>>& FlowTransport::getAllPeers() const {
	return self->peers;
}

std::map<NetworkAddress, std::pair<uint64_t, double>>* FlowTransport::getIncompatiblePeers() {
	for (auto it = self->incompatiblePeers.begin(); it != self->incompatiblePeers.end();) {
		if (self->multiVersionConnections.count(it->second.first)) {
			it = self->incompatiblePeers.erase(it);
		} else {
			it++;
		}
	}
	return &self->incompatiblePeers;
}

Future<Void> FlowTransport::onIncompatibleChanged() {
	return self->incompatiblePeersChanged.onTrigger();
}

Future<Void> FlowTransport::bind(NetworkAddress publicAddress, NetworkAddress listenAddress) {
	ASSERT(publicAddress.isPublic());
	if (self->localAddresses.getAddressList().address == NetworkAddress()) {
		self->localAddresses.setNetworkAddress(publicAddress);
	} else {
		auto addrList = self->localAddresses.getAddressList();
		addrList.secondaryAddress = publicAddress;
		self->localAddresses.setAddressList(addrList);
	}
	// reformatLocalAddress()
	TraceEvent("Binding").detail("PublicAddress", publicAddress).detail("ListenAddress", listenAddress);

	Future<Void> listenF = listen(self, listenAddress);
	self->listeners.push_back(listenF);
	return listenF;
}

Endpoint FlowTransport::loadedEndpoint(const UID& token) {
	return Endpoint(g_currentDeliveryPeerAddress, token);
}

Future<Void> FlowTransport::loadedDisconnect() {
	return g_currentDeliveryPeerDisconnect;
}

void FlowTransport::addPeerReference(const Endpoint& endpoint, bool isStream) {
	if (!isStream || !endpoint.getPrimaryAddress().isValid() || !endpoint.getPrimaryAddress().isPublic())
		return;

	Reference<Peer> peer = self->getOrOpenPeer(endpoint.getPrimaryAddress());
	if (peer->peerReferences == -1) {
		peer->peerReferences = 1;
	} else {
		peer->peerReferences++;
	}
}

void FlowTransport::removePeerReference(const Endpoint& endpoint, bool isStream) {
	if (!isStream || !endpoint.getPrimaryAddress().isValid() || !endpoint.getPrimaryAddress().isPublic())
		return;
	Reference<Peer> peer = self->getPeer(endpoint.getPrimaryAddress());
	if (peer) {
		peer->peerReferences--;
		if (peer->peerReferences < 0) {
			TraceEvent(SevError, "InvalidPeerReferences")
			    .detail("References", peer->peerReferences)
			    .detail("Address", endpoint.getPrimaryAddress())
			    .detail("Token", endpoint.token);
		}
		if (peer->peerReferences == 0 && peer->reliable.empty() && peer->unsent.empty() &&
		    peer->outstandingReplies == 0 &&
		    peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_UNREFERENCED_CLOSE_DELAY) {
			peer->resetPing.trigger();
		}
	}
}

void FlowTransport::addEndpoint(Endpoint& endpoint, NetworkMessageReceiver* receiver, TaskPriority taskID) {
	endpoint.token = deterministicRandom()->randomUniqueID();
	if (receiver->isStream()) {
		endpoint.addresses = self->localAddresses.getAddressList();
		endpoint.token = UID(endpoint.token.first() | TOKEN_STREAM_FLAG, endpoint.token.second());
	} else {
		endpoint.addresses = NetworkAddressList();
		endpoint.token = UID(endpoint.token.first() & ~TOKEN_STREAM_FLAG, endpoint.token.second());
	}
	self->endpoints.insert(receiver, endpoint.token, taskID);
}

void FlowTransport::addEndpoints(std::vector<std::pair<FlowReceiver*, TaskPriority>> const& streams) {
	self->endpoints.insert(self->localAddresses.getAddressList(), streams);
}

void FlowTransport::removeEndpoint(const Endpoint& endpoint, NetworkMessageReceiver* receiver) {
	self->endpoints.remove(endpoint.token, receiver);
}

void FlowTransport::addWellKnownEndpoint(Endpoint& endpoint, NetworkMessageReceiver* receiver, TaskPriority taskID) {
	endpoint.addresses = self->localAddresses.getAddressList();
	ASSERT(receiver->isStream());
	self->endpoints.insertWellKnown(receiver, endpoint.token, taskID);
}

static void sendLocal(TransportData* self, ISerializeSource const& what, const Endpoint& destination) {
	CODE_PROBE(true, "\"Loopback\" delivery");
	// SOMEDAY: Would it be better to avoid (de)serialization by doing this check in flow?

	Standalone<StringRef> copy;
	ObjectWriter wr(AssumeVersion(g_network->protocolVersion()));
	what.serializeObjectWriter(wr);
	copy = wr.toStringRef();
#if VALGRIND
	VALGRIND_CHECK_MEM_IS_DEFINED(copy.begin(), copy.size());
#endif

	ASSERT(copy.size() > 0);
	TaskPriority priority = self->endpoints.getPriority(destination.token);
	if (priority != TaskPriority::UnknownEndpoint || (destination.token.first() & TOKEN_STREAM_FLAG) != 0) {
		deliver(self,
		        destination,
		        priority,
		        ArenaReader(copy.arena(), copy, AssumeVersion(currentProtocolVersion())),
		        NetworkAddress(),
		        true,
		        InReadSocket::False,
		        Never());
	}
}

static ReliablePacket* sendPacket(TransportData* self,
                                  Reference<Peer> peer,
                                  ISerializeSource const& what,
                                  const Endpoint& destination,
                                  bool reliable) {
	const bool checksumEnabled = !destination.getPrimaryAddress().isTLS();
	++self->countPacketsGenerated;

	// If there isn't an open connection, a public address, or the peer isn't compatible, we can't send
	if (!peer || (peer->outgoingConnectionIdle && !destination.getPrimaryAddress().isPublic()) ||
	    (!peer->compatible && destination.token != Endpoint::wellKnownToken(WLTOKEN_PING_PACKET))) {
		CODE_PROBE(true, "Can't send to private address without a compatible open connection");
		return nullptr;
	}

	bool firstUnsent = peer->unsent.empty();

	PacketBuffer* pb = peer->unsent.getWriteBuffer();
	ReliablePacket* rp = reliable ? new ReliablePacket : 0;

	int prevBytesWritten = pb->bytes_written;
	PacketBuffer* checksumPb = pb;

	PacketWriter wr(pb,
	                rp,
	                AssumeVersion(g_network->protocolVersion())); // SOMEDAY: Can we downgrade to talk to older peers?

	// Reserve some space for packet length and checksum, write them after serializing data
	SplitBuffer packetInfoBuffer;
	uint32_t len;

	// This is technically abstraction breaking but avoids XXH3_createState() and XXH3_freeState() which are just
	// malloc/free
	XXH3_state_t checksumState;
	// Checksum will be calculated with buffer API if contiguous, else using stream API.  Mode is tracked here.
	bool checksumStream = false;
	XXH64_hash_t checksum;

	int packetInfoSize = PACKET_LEN_WIDTH;
	if (checksumEnabled) {
		packetInfoSize += sizeof(checksum);
	}

	wr.writeAhead(packetInfoSize, &packetInfoBuffer);
	wr << destination.token;
	what.serializePacketWriter(wr);
	pb = wr.finish();
	len = wr.size() - packetInfoSize;

	if (checksumEnabled) {
		// Find the correct place to start calculating checksum
		uint32_t checksumUnprocessedLength = len;
		prevBytesWritten += packetInfoSize;
		if (prevBytesWritten >= checksumPb->bytes_written) {
			prevBytesWritten -= checksumPb->bytes_written;
			checksumPb = checksumPb->nextPacketBuffer();
		}

		// Checksum calculation
		while (checksumUnprocessedLength > 0) {
			uint32_t processLength =
			    std::min(checksumUnprocessedLength, (uint32_t)(checksumPb->bytes_written - prevBytesWritten));

			// If not in checksum stream mode yet
			if (!checksumStream) {
				// If there is nothing left to process then calculate checksum directly
				if (processLength == checksumUnprocessedLength) {
					checksum = XXH3_64bits(checksumPb->data() + prevBytesWritten, processLength);
				} else {
					// Otherwise, initialize checksum state and switch to stream mode
					if (XXH3_64bits_reset(&checksumState) != XXH_OK) {
						throw internal_error();
					}
					checksumStream = true;
				}
			}

			// If in checksum stream mode, update the checksum state
			if (checksumStream) {
				if (XXH3_64bits_update(&checksumState, checksumPb->data() + prevBytesWritten, processLength) !=
				    XXH_OK) {
					throw internal_error();
				}
			}

			checksumUnprocessedLength -= processLength;
			checksumPb = checksumPb->nextPacketBuffer();
			prevBytesWritten = 0;
		}

		// If in checksum stream mode, get the final checksum
		if (checksumStream) {
			checksum = XXH3_64bits_digest(&checksumState);
		}
	}

	// Write packet length and checksum into packet buffer
	packetInfoBuffer.write(&len, sizeof(len));
	if (checksumEnabled) {
		packetInfoBuffer.write(&checksum, sizeof(checksum), sizeof(len));
	}

	if (len > FLOW_KNOBS->PACKET_LIMIT) {
		TraceEvent(SevError, "PacketLimitExceeded")
		    .detail("ToPeer", destination.getPrimaryAddress())
		    .detail("Length", (int)len);
		// throw platform_error();  // FIXME: How to recover from this situation?
	} else if (len > FLOW_KNOBS->PACKET_WARNING) {
		TraceEvent(SevWarn, "LargePacketSent")
		    .suppressFor(1.0)
		    .detail("ToPeer", destination.getPrimaryAddress())
		    .detail("Length", (int)len)
		    .detail("Token", destination.token)
		    .backtrace();
	}

#if VALGRIND
	SendBuffer* checkbuf = pb;
	while (checkbuf) {
		int size = checkbuf->bytes_written;
		const uint8_t* data = checkbuf->data();
		VALGRIND_CHECK_MEM_IS_DEFINED(data, size);
		checkbuf = checkbuf->next;
	}
#endif

	peer->send(pb, rp, firstUnsent);
	if (destination.token != Endpoint::wellKnownToken(WLTOKEN_PING_PACKET)) {
		peer->lastDataPacketSentTime = now();
	}
	return rp;
}

ReliablePacket* FlowTransport::sendReliable(ISerializeSource const& what, const Endpoint& destination) {
	if (self->isLocalAddress(destination.getPrimaryAddress())) {
		sendLocal(self, what, destination);
		return nullptr;
	}
	Reference<Peer> peer = self->getOrOpenPeer(destination.getPrimaryAddress());
	return sendPacket(self, peer, what, destination, true);
}

void FlowTransport::cancelReliable(ReliablePacket* p) {
	if (p)
		p->remove();
	// SOMEDAY: Call reliable.compact() if a lot of memory is wasted in PacketBuffers by formerly reliable packets mixed
	// with a few reliable ones.  Don't forget to delref the new PacketBuffers since they are unsent.
}

Reference<Peer> FlowTransport::sendUnreliable(ISerializeSource const& what,
                                              const Endpoint& destination,
                                              bool openConnection) {
	if (self->isLocalAddress(destination.getPrimaryAddress())) {
		sendLocal(self, what, destination);
		return Reference<Peer>();
	}
	Reference<Peer> peer;
	if (openConnection) {
		peer = self->getOrOpenPeer(destination.getPrimaryAddress());
	} else {
		peer = self->getPeer(destination.getPrimaryAddress());
	}

	sendPacket(self, peer, what, destination, false);
	return peer;
}

Reference<AsyncVar<bool>> FlowTransport::getDegraded() {
	return self->degraded;
}

// Returns the protocol version of the peer at the specified address. The result is returned as an AsyncVar that
// can be used to monitor for changes of a peer's protocol. The protocol version will be unset in the event that
// there is no connection established to the peer.
//
// Note that this function does not establish a connection to the peer. In order to obtain a peer's protocol
// version, some other mechanism should be used to connect to that peer.
Optional<Reference<AsyncVar<Optional<ProtocolVersion>> const>> FlowTransport::getPeerProtocolAsyncVar(
    NetworkAddress addr) {
	auto itr = self->peers.find(addr);
	if (itr != self->peers.end()) {
		return itr->second->protocolVersion;
	} else {
		return Optional<Reference<AsyncVar<Optional<ProtocolVersion>> const>>();
	}
}

void FlowTransport::resetConnection(NetworkAddress address) {
	auto peer = self->getPeer(address);
	if (peer) {
		peer->resetConnection.trigger();
	}
}

bool FlowTransport::incompatibleOutgoingConnectionsPresent() {
	return self->numIncompatibleConnections > 0;
}

void FlowTransport::createInstance(bool isClient,
                                   uint64_t transportId,
                                   int maxWellKnownEndpoints,
                                   IPAllowList const* allowList) {
	TokenCache::createInstance();
	g_network->setGlobal(INetwork::enFlowTransport,
	                     (flowGlobalType) new FlowTransport(transportId, maxWellKnownEndpoints, allowList));
	g_network->setGlobal(INetwork::enNetworkAddressFunc, (flowGlobalType)&FlowTransport::getGlobalLocalAddress);
	g_network->setGlobal(INetwork::enNetworkAddressesFunc, (flowGlobalType)&FlowTransport::getGlobalLocalAddresses);
	g_network->setGlobal(INetwork::enFailureMonitor, (flowGlobalType) new SimpleFailureMonitor());
	g_network->setGlobal(INetwork::enClientFailureMonitor, isClient ? (flowGlobalType)1 : nullptr);
}

HealthMonitor* FlowTransport::healthMonitor() {
	return &self->healthMonitor;
}

Optional<PublicKey> FlowTransport::getPublicKeyByName(StringRef name) const {
	auto iter = self->publicKeys.find(name);
	if (iter != self->publicKeys.end()) {
		return iter->second;
	}
	return {};
}

NetworkAddress FlowTransport::currentDeliveryPeerAddress() const {
	return g_currentDeliveryPeerAddress.address;
}

bool FlowTransport::currentDeliveryPeerIsTrusted() const {
	return g_currentDeliverPeerAddressTrusted;
}

void FlowTransport::addPublicKey(StringRef name, PublicKey key) {
	self->publicKeys[name] = key;
}

void FlowTransport::removePublicKey(StringRef name) {
	self->publicKeys.erase(name);
}

void FlowTransport::removeAllPublicKeys() {
	self->publicKeys.clear();
}

void FlowTransport::loadPublicKeyFile(const std::string& filePath) {
	if (!fileExists(filePath)) {
		throw file_not_found();
	}
	int64_t const len = fileSize(filePath);
	if (len <= 0) {
		TraceEvent(SevWarn, "AuthzPublicKeySetEmpty").detail("Path", filePath);
	} else if (len > FLOW_KNOBS->PUBLIC_KEY_FILE_MAX_SIZE) {
		throw file_too_large();
	} else {
		auto json = readFileBytes(filePath, len);
		self->applyPublicKeySet(StringRef(json));
	}
}

															#line 7402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via watchPublicKeyJwksFile()
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
template <class WatchPublicKeyJwksFileActor>
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class WatchPublicKeyJwksFileActorState {
															#line 7409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	WatchPublicKeyJwksFileActorState(std::string const& filePath,TransportData* const& self) 
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		 : filePath(filePath),
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   self(self),
															#line 2035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   fileChanged(),
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   fileWatch(),
															#line 2037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		   errorCount(0)
															#line 7424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
	{
		fdb_probe_actor_create("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this));

	}
	~WatchPublicKeyJwksFileActorState() 
	{
		fdb_probe_actor_destroy("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 7439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
		this->~WatchPublicKeyJwksFileActorState();
		static_cast<WatchPublicKeyJwksFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		const int& intervalSeconds = FLOW_KNOBS->PUBLIC_KEY_FILE_REFRESH_INTERVAL_SECONDS;
															#line 2046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		fileWatch = watchFileForChanges(filePath, &fileChanged, &intervalSeconds, "AuthzPublicKeySetRefreshStatError");
															#line 2047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 7466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

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
															#line 2041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (IAsyncFileSystem::filesystem())
															#line 7482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 2043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(1.0);
															#line 2043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 1;
															#line 2043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >*>(static_cast<WatchPublicKeyJwksFileActor*>(this)));
															#line 7495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
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
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state > 0) static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPublicKeyJwksFileActor*>(this)->ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
		try {
															#line 2049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_1 = fileChanged.onTrigger();
															#line 2049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 2;
															#line 2049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >*>(static_cast<WatchPublicKeyJwksFileActor*>(this)));
															#line 7607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 2064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 7629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 2065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 7633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 2068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			errorCount++;
															#line 2069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevWarn, "AuthzPublicKeySetRefreshError"_audit).error(e).detail("ErrorCount", errorCount);
															#line 7639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_2 = IAsyncFileSystem::filesystem()->open( filePath, IAsyncFile::OPEN_READONLY | IAsyncFile::OPEN_UNCACHED, 0);
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 3;
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >*>(static_cast<WatchPublicKeyJwksFileActor*>(this)));
															#line 7661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_2 = IAsyncFileSystem::filesystem()->open( filePath, IAsyncFile::OPEN_READONLY | IAsyncFile::OPEN_UNCACHED, 0);
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1loopBody1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 3;
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >*>(static_cast<WatchPublicKeyJwksFileActor*>(this)));
															#line 7677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state > 0) static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPublicKeyJwksFileActor*>(this)->ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1loopBody1cont3(int loopDepth) 
	{
															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<int64_t> __when_expr_3 = file->size();
															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1loopBody1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 4;
															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >*>(static_cast<WatchPublicKeyJwksFileActor*>(this)));
															#line 7756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2when1(Reference<IAsyncFile> const& __file,int loopDepth) 
	{
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		file = __file;
															#line 7765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2when1(Reference<IAsyncFile> && __file,int loopDepth) 
	{
		file = std::move(__file);
		loopDepth = a_body1cont1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state > 0) static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPublicKeyJwksFileActor*>(this)->ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1loopBody1cont4(int loopDepth) 
	{
															#line 2053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		json = std::string(filesize, '\0');
															#line 2054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (filesize > FLOW_KNOBS->PUBLIC_KEY_FILE_MAX_SIZE)
															#line 7834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 2055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			return a_body1cont1loopBody1Catch1(file_too_large(), loopDepth);
															#line 7838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 2056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (filesize <= 0)
															#line 7842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 2057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevWarn, "AuthzPublicKeySetEmpty").suppressFor(60);
															#line 7846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
			return a_body1cont1loopHead1(loopDepth); // continue
		}
															#line 2060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = success(file->read(&json[0], filesize, 0));
															#line 2060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state < 0) return a_body1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont1loopBody1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 5;
															#line 2060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >*>(static_cast<WatchPublicKeyJwksFileActor*>(this)));
															#line 7858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3when1(int64_t const& __filesize,int loopDepth) 
	{
															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		filesize = __filesize;
															#line 7867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3when1(int64_t && __filesize,int loopDepth) 
	{
		filesize = std::move(__filesize);
		loopDepth = a_body1cont1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state > 0) static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPublicKeyJwksFileActor*>(this)->ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 2061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->applyPublicKeySet(StringRef(json));
															#line 2062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		errorCount = 0;
															#line 7936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 2061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		self->applyPublicKeySet(StringRef(json));
															#line 2062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
		errorCount = 0;
															#line 7947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state > 0) static_cast<WatchPublicKeyJwksFileActor*>(this)->actor_wait_state = 0;
		static_cast<WatchPublicKeyJwksFileActor*>(this)->ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont1loopBody1cont9(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	std::string filePath;
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 2035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	AsyncTrigger fileChanged;
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> fileWatch;
															#line 2037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	unsigned errorCount;
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	Reference<IAsyncFile> file;
															#line 2052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	int64_t filesize;
															#line 2053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	std::string json;
															#line 8044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via watchPublicKeyJwksFile()
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
class WatchPublicKeyJwksFileActor final : public Actor<Void>, public ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >, public ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >, public ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >, public ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >, public ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >, public FastAllocated<WatchPublicKeyJwksFileActor>, public WatchPublicKeyJwksFileActorState<WatchPublicKeyJwksFileActor> {
															#line 8049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<WatchPublicKeyJwksFileActor>::operator new;
	using FastAllocated<WatchPublicKeyJwksFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >;
friend struct ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >;
friend struct ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >;
friend struct ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >;
friend struct ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >;
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	WatchPublicKeyJwksFileActor(std::string const& filePath,TransportData* const& self) 
															#line 8064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<Void>(),
		   WatchPublicKeyJwksFileActorState<WatchPublicKeyJwksFileActor>(filePath, self)
	{
		fdb_probe_actor_enter("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("watchPublicKeyJwksFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("watchPublicKeyJwksFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchPublicKeyJwksFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WatchPublicKeyJwksFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WatchPublicKeyJwksFileActor, 2, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WatchPublicKeyJwksFileActor, 3, int64_t >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WatchPublicKeyJwksFileActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> watchPublicKeyJwksFile( std::string const& filePath, TransportData* const& self ) {
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new WatchPublicKeyJwksFileActor(filePath, self));
															#line 8096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 2073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/FlowTransport.actor.cpp"

void FlowTransport::watchPublicKeyFile(const std::string& publicKeyFilePath) {
	self->publicKeyFileWatch = watchPublicKeyJwksFile(publicKeyFilePath, self);
}
