#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
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
#include "flow/network.h"

#include <cstdint>
#include <unordered_map>
#if VALGRIND
#include <memcheck.h>
#endif

#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/FailureMonitor.h"
#include "fdbrpc/HealthMonitor.h"
#include "fdbrpc/genericactors.actor.h"
#include "fdbrpc/simulator.h"
#include "flow/ActorCollection.h"
#include "flow/Error.h"
#include "flow/flow.h"
#include "flow/Net2Packet.h"
#include "flow/TDMetric.actor.h"
#include "flow/ObjectSerializer.h"
#include "flow/ProtocolVersion.h"
#include "flow/UnitTest.h"
#define XXH_INLINE_ALL
#include "flow/xxhash.h"
#include "flow/actorcompiler.h" // This must be the last #include.

static NetworkAddressList g_currentDeliveryPeerAddress = NetworkAddressList();
static Future<Void> g_currentDeliveryPeerDisconnect;

constexpr int PACKET_LEN_WIDTH = sizeof(uint32_t);
const uint64_t TOKEN_STREAM_FLAG = 1;

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
};

class TransportData {
public:
	TransportData(uint64_t transportId, int maxWellKnownEndpoints);

	~TransportData();

	void initMetrics() {
		bytesSent.init(LiteralStringRef("Net2.BytesSent"));
		countPacketsReceived.init(LiteralStringRef("Net2.CountPacketsReceived"));
		countPacketsGenerated.init(LiteralStringRef("Net2.CountPacketsGenerated"));
		countConnEstablished.init(LiteralStringRef("Net2.CountConnEstablished"));
		countConnClosedWithError.init(LiteralStringRef("Net2.CountConnClosedWithError"));
		countConnClosedWithoutError.init(LiteralStringRef("Net2.CountConnClosedWithoutError"));
	}

	Reference<struct Peer> getPeer(NetworkAddress const& address);
	Reference<struct Peer> getOrOpenPeer(NetworkAddress const& address, bool startConnectionKeeper = true);

	// Returns true if given network address 'address' is one of the address we are listening on.
	bool isLocalAddress(const NetworkAddress& address) const;

	NetworkAddressList localAddresses;
	std::vector<Future<Void>> listeners;
	std::unordered_map<NetworkAddress, Reference<struct Peer>> peers;
	std::unordered_map<NetworkAddress, std::pair<double, double>> closedPeers;
	HealthMonitor healthMonitor;
	std::set<NetworkAddress> orderedAddresses;
	Reference<AsyncVar<bool>> degraded;
	bool warnAlwaysForLargePacket;

	EndpointMap endpoints;
	EndpointNotFoundReceiver endpointNotFoundReceiver{ endpoints };
	PingReceiver pingReceiver{ endpoints };

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

	Future<Void> multiVersionCleanup;
	Future<Void> pingLogger;
};

															#line 288 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via pingLatencyLogger()
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class PingLatencyLoggerActor>
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class PingLatencyLoggerActorState {
															#line 295 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	PingLatencyLoggerActorState(TransportData* const& self) 
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 287 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   lastAddress(NetworkAddress())
															#line 304 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 288 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 319 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 289 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (self->orderedAddresses.size())
															#line 349 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 290 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			auto it = self->orderedAddresses.upper_bound(lastAddress);
															#line 291 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (it == self->orderedAddresses.end())
															#line 355 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 292 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it = self->orderedAddresses.begin();
															#line 359 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 294 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastAddress = *it;
															#line 295 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			auto peer = self->getPeer(lastAddress);
															#line 296 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!peer)
															#line 367 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 297 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent(SevWarnAlways, "MissingNetworkAddress").suppressFor(10.0).detail("PeerAddr", lastAddress);
															#line 371 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 299 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (peer->lastLoggedTime <= 0.0)
															#line 375 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 300 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedTime = peer->lastConnectTime;
															#line 379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 303 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (peer && (peer->pingLatencies.getPopulationSize() >= 10 || peer->connectFailedCount > 0 || peer->timeoutCount > 0))
															#line 383 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 305 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("PingLatency") .detail("Elapsed", now() - peer->lastLoggedTime) .detail("PeerAddr", lastAddress) .detail("MinLatency", peer->pingLatencies.min()) .detail("MaxLatency", peer->pingLatencies.max()) .detail("MeanLatency", peer->pingLatencies.mean()) .detail("MedianLatency", peer->pingLatencies.median()) .detail("P90Latency", peer->pingLatencies.percentile(0.90)) .detail("Count", peer->pingLatencies.getPopulationSize()) .detail("BytesReceived", peer->bytesReceived - peer->lastLoggedBytesReceived) .detail("BytesSent", peer->bytesSent - peer->lastLoggedBytesSent) .detail("TimeoutCount", peer->timeoutCount) .detail("ConnectOutgoingCount", peer->connectOutgoingCount) .detail("ConnectIncomingCount", peer->connectIncomingCount) .detail("ConnectFailedCount", peer->connectFailedCount) .detail("ConnectMinLatency", peer->connectLatencies.min()) .detail("ConnectMaxLatency", peer->connectLatencies.max()) .detail("ConnectMeanLatency", peer->connectLatencies.mean()) .detail("ConnectMedianLatency", peer->connectLatencies.median()) .detail("ConnectP90Latency", peer->connectLatencies.percentile(0.90));
															#line 325 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedTime = now();
															#line 326 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->connectOutgoingCount = 0;
															#line 327 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->connectIncomingCount = 0;
															#line 328 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->connectFailedCount = 0;
															#line 329 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->pingLatencies.clear();
															#line 330 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->connectLatencies.clear();
															#line 331 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedBytesReceived = peer->bytesReceived;
															#line 332 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->lastLoggedBytesSent = peer->bytesSent;
															#line 333 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->timeoutCount = 0;
															#line 334 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->PING_LOGGING_INTERVAL);
															#line 334 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 409 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 1;
															#line 334 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PingLatencyLoggerActor, 0, Void >*>(static_cast<PingLatencyLoggerActor*>(this)));
															#line 414 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 335 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (it == self->orderedAddresses.begin())
															#line 421 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					StrictFuture<Void> __when_expr_1 = delay(FLOW_KNOBS->PING_LOGGING_INTERVAL);
															#line 336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 427 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
					static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 2;
															#line 336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PingLatencyLoggerActor, 1, Void >*>(static_cast<PingLatencyLoggerActor*>(this)));
															#line 432 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 339 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_2 = delay(FLOW_KNOBS->PING_LOGGING_INTERVAL);
															#line 339 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 447 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when3(__when_expr_2.get(), loopDepth); };
			static_cast<PingLatencyLoggerActor*>(this)->actor_wait_state = 3;
															#line 339 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PingLatencyLoggerActor, 2, Void >*>(static_cast<PingLatencyLoggerActor*>(this)));
															#line 452 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 287 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress lastAddress;
															#line 705 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via pingLatencyLogger()
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class PingLatencyLoggerActor final : public Actor<Void>, public ActorCallback< PingLatencyLoggerActor, 0, Void >, public ActorCallback< PingLatencyLoggerActor, 1, Void >, public ActorCallback< PingLatencyLoggerActor, 2, Void >, public FastAllocated<PingLatencyLoggerActor>, public PingLatencyLoggerActorState<PingLatencyLoggerActor> {
															#line 710 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	PingLatencyLoggerActor(TransportData* const& self) 
															#line 723 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> pingLatencyLogger( TransportData* const& self ) {
															#line 286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new PingLatencyLoggerActor(self));
															#line 753 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 343 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

TransportData::TransportData(uint64_t transportId, int maxWellKnownEndpoints)
  : warnAlwaysForLargePacket(true), endpoints(maxWellKnownEndpoints), endpointNotFoundReceiver(endpoints),
    pingReceiver(endpoints), numIncompatibleConnections(0), lastIncompatibleMessage(0), transportId(transportId) {
	degraded = makeReference<AsyncVar<bool>>(false);
	pingLogger = pingLatencyLogger(this);
}

#define CONNECT_PACKET_V0 0x0FDB00A444020001LL
#define CONNECT_PACKET_V0_SIZE 14

#pragma pack(push, 1)
struct ConnectPacket {
	// The value does not include the size of `connectPacketLength` itself,
	// but only the other fields of this structure.
	uint32_t connectPacketLength;
	ProtocolVersion protocolVersion; // Expect currentProtocolVersion

	uint16_t canonicalRemotePort; // Port number to reconnect to the originating process
	uint64_t connectionId; // Multi-version clients will use the same Id for both connections, other connections will
	                       // set this to zero. Added at protocol Version 0x0FDB00A444020001.

	// IP Address to reconnect to the originating process. Only one of these must be populated.
	uint32_t canonicalRemoteIp4;

	enum ConnectPacketFlags { FLAG_IPV6 = 1 };
	uint16_t flags;
	uint8_t canonicalRemoteIp6[16];

	ConnectPacket() { memset((void*)this, 0, sizeof(*this)); }

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
			TraceEvent("SerializationFailed").detail("PacketLength", connectPacketLength).backtrace();
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

															#line 835 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
[[nodiscard]] static Future<Void> connectionReader( TransportData* const& transport, Reference<IConnection> const& conn, Reference<struct Peer> const& peer, Promise<Reference<struct Peer>> const& onConnected );

#line 425 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

static void sendLocal(TransportData* self, ISerializeSource const& what, const Endpoint& destination);
static ReliablePacket* sendPacket(TransportData* self,
                                  Reference<Peer> peer,
                                  ISerializeSource const& what,
                                  const Endpoint& destination,
                                  bool reliable);

															#line 847 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionMonitor()
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionMonitorActor>
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionMonitorActorState {
															#line 854 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionMonitorActorState(Reference<Peer> const& peer) 
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : peer(peer),
															#line 434 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   remotePingEndpoint({ peer->destination }, Endpoint::wellKnownToken(WLTOKEN_PING_PACKET))
															#line 863 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 435 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 878 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 436 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!FlowTransport::isClient() && !peer->destination.isPublic() && peer->compatible)
															#line 908 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 440 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastRefreshed = now();
															#line 441 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastBytesReceived = peer->bytesReceived;
															#line 442 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 916 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 459 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 459 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 932 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 2;
															#line 459 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 1, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 443 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME, TaskPriority::ReadSocket);
															#line 443 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 961 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 1;
															#line 443 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 0, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 966 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 444 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (lastBytesReceived < peer->bytesReceived)
															#line 988 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 445 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastRefreshed = now();
															#line 446 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastBytesReceived = peer->bytesReceived;
															#line 994 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
		else
		{
															#line 447 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (lastRefreshed < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_INCOMING_IDLE_MULTIPLIER)
															#line 1000 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
				return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
			}
		}
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 444 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (lastBytesReceived < peer->bytesReceived)
															#line 1013 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 445 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastRefreshed = now();
															#line 446 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			lastBytesReceived = peer->bytesReceived;
															#line 1019 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
		else
		{
															#line 447 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (lastRefreshed < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT * FLOW_KNOBS->CONNECTION_MONITOR_INCOMING_IDLE_MULTIPLIER)
															#line 1025 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 461 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (peer->reliable.empty() && peer->unsent.empty() && peer->outstandingReplies == 0)
															#line 1101 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 462 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (peer->peerReferences == 0 && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_UNREFERENCED_CLOSE_DELAY))
															#line 1105 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 465 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(connection_unreferenced(), std::max(0, loopDepth - 1));
															#line 1109 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 466 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (FlowTransport::isClient() && peer->compatible && peer->destination.isPublic() && (peer->lastConnectTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT) && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT))
															#line 1115 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 470 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					return a_body1Catch1(connection_idle(), std::max(0, loopDepth - 1));
															#line 1119 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
		}
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME, TaskPriority::ReadSocket);
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1127 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 3;
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 2, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1132 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 461 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (peer->reliable.empty() && peer->unsent.empty() && peer->outstandingReplies == 0)
															#line 1141 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 462 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (peer->peerReferences == 0 && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_UNREFERENCED_CLOSE_DELAY))
															#line 1145 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 465 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(connection_unreferenced(), std::max(0, loopDepth - 1));
															#line 1149 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 466 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (FlowTransport::isClient() && peer->compatible && peer->destination.isPublic() && (peer->lastConnectTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT) && (peer->lastDataPacketSentTime < now() - FLOW_KNOBS->CONNECTION_MONITOR_IDLE_TIMEOUT))
															#line 1155 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 470 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					return a_body1Catch1(connection_idle(), std::max(0, loopDepth - 1));
															#line 1159 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
		}
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_LOOP_TIME, TaskPriority::ReadSocket);
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1167 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 3;
															#line 474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 2, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 477 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		pingRequest = PingRequest();
															#line 478 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		FlowTransport::transport().sendUnreliable(SerializeSource<PingRequest>(pingRequest), remotePingEndpoint, true);
															#line 479 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 480 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		timeouts = 0;
															#line 481 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		startTime = now();
															#line 482 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1254 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 477 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		pingRequest = PingRequest();
															#line 478 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		FlowTransport::transport().sendUnreliable(SerializeSource<PingRequest>(pingRequest), remotePingEndpoint, true);
															#line 479 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 480 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		timeouts = 0;
															#line 481 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		startTime = now();
															#line 482 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1273 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 484 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 483 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionMonitorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1360 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont4loopBody1when1(__when_expr_3.get(), loopDepth); };
															#line 502 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = pingRequest.reply.getFuture();
															#line 1364 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont4loopBody1when2(__when_expr_4.get(), loopDepth); };
															#line 508 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_5 = peer->resetPing.onTrigger();
															#line 1368 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont4loopBody1when3(__when_expr_5.get(), loopDepth); };
		static_cast<ConnectionMonitorActor*>(this)->actor_wait_state = 4;
															#line 484 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 3, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 502 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 4, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 508 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ConnectionMonitorActor, 5, Void >*>(static_cast<ConnectionMonitorActor*>(this)));
															#line 1377 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 485 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		peer->timeoutCount++;
															#line 486 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (startingBytes == peer->bytesReceived)
															#line 1407 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 487 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (peer->destination.isPublic())
															#line 1411 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 488 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->pingLatencies.addSample(now() - startTime);
															#line 1415 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 490 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionTimeout").suppressFor(1.0).detail("WithAddr", peer->destination);
															#line 491 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 2));
															#line 1421 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 493 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (timeouts > 1)
															#line 1425 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 494 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevWarnAlways, "ConnectionSlowPing") .suppressFor(1.0) .detail("WithAddr", peer->destination) .detail("Timeouts", timeouts);
															#line 1429 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 499 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 500 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		timeouts++;
															#line 1435 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when1(Void && _,int loopDepth) 
	{
															#line 485 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		peer->timeoutCount++;
															#line 486 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (startingBytes == peer->bytesReceived)
															#line 1446 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 487 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (peer->destination.isPublic())
															#line 1450 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 488 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->pingLatencies.addSample(now() - startTime);
															#line 1454 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 490 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionTimeout").suppressFor(1.0).detail("WithAddr", peer->destination);
															#line 491 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 2));
															#line 1460 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 493 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (timeouts > 1)
															#line 1464 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 494 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevWarnAlways, "ConnectionSlowPing") .suppressFor(1.0) .detail("WithAddr", peer->destination) .detail("Timeouts", timeouts);
															#line 1468 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 499 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		startingBytes = peer->bytesReceived;
															#line 500 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		timeouts++;
															#line 1474 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (peer->destination.isPublic())
															#line 1483 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 504 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			peer->pingLatencies.addSample(now() - startTime);
															#line 1487 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
		return a_body1loopBody1cont4break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4loopBody1when2(Void && _,int loopDepth) 
	{
															#line 503 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (peer->destination.isPublic())
															#line 1497 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 504 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			peer->pingLatencies.addSample(now() - startTime);
															#line 1501 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> peer;
															#line 434 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Endpoint remotePingEndpoint;
															#line 440 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	double lastRefreshed;
															#line 441 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int64_t lastBytesReceived;
															#line 477 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	PingRequest pingRequest;
															#line 479 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int64_t startingBytes;
															#line 480 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int timeouts;
															#line 481 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	double startTime;
															#line 1678 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionMonitor()
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionMonitorActor final : public Actor<Void>, public ActorCallback< ConnectionMonitorActor, 0, Void >, public ActorCallback< ConnectionMonitorActor, 1, Void >, public ActorCallback< ConnectionMonitorActor, 2, Void >, public ActorCallback< ConnectionMonitorActor, 3, Void >, public ActorCallback< ConnectionMonitorActor, 4, Void >, public ActorCallback< ConnectionMonitorActor, 5, Void >, public FastAllocated<ConnectionMonitorActor>, public ConnectionMonitorActorState<ConnectionMonitorActor> {
															#line 1683 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionMonitorActor(Reference<Peer> const& peer) 
															#line 1699 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> connectionMonitor( Reference<Peer> const& peer ) {
															#line 433 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionMonitorActor(peer));
															#line 1730 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 515 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

															#line 1735 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionWriter()
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionWriterActor>
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionWriterActorState {
															#line 1742 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionWriterActorState(Reference<Peer> const& self,Reference<IConnection> const& conn) 
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn),
															#line 517 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   lastWriteTime(now())
															#line 1753 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 518 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 1768 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 520 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delayJittered( std::max<double>(FLOW_KNOBS->MIN_COALESCE_DELAY, FLOW_KNOBS->MAX_COALESCE_DELAY - (now() - lastWriteTime)), TaskPriority::WriteSocket);
															#line 520 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1800 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 1;
															#line 520 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 0, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 1805 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 527 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1814 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 527 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1823 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 548 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 1895 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 528 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		lastWriteTime = now();
															#line 530 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		int sent = conn->write(self->unsent.getUnsent(), FLOW_KNOBS->MAX_PACKET_SEND_BYTES);
															#line 531 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (sent)
															#line 1915 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 532 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->bytesSent += sent;
															#line 533 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->transport->bytesSent += sent;
															#line 534 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->unsent.sent(sent);
															#line 1923 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 537 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (self->unsent.empty())
															#line 1927 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 541 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TEST(true);
															#line 543 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 543 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 2;
															#line 543 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 1, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 1942 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 544 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 544 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1966 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 3;
															#line 544 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 2, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 1971 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 544 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 544 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1982 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 3;
															#line 544 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 2, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 1987 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 548 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!(self->unsent.empty()))
															#line 2147 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 549 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = self->dataToSend.onTrigger();
															#line 549 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionWriterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2155 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionWriterActor*>(this)->actor_wait_state = 4;
															#line 549 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionWriterActor, 3, Void >*>(static_cast<ConnectionWriterActor*>(this)));
															#line 2160 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> self;
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 517 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	double lastWriteTime;
															#line 2259 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionWriter()
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionWriterActor final : public Actor<Void>, public ActorCallback< ConnectionWriterActor, 0, Void >, public ActorCallback< ConnectionWriterActor, 1, Void >, public ActorCallback< ConnectionWriterActor, 2, Void >, public ActorCallback< ConnectionWriterActor, 3, Void >, public FastAllocated<ConnectionWriterActor>, public ConnectionWriterActorState<ConnectionWriterActor> {
															#line 2264 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionWriterActor(Reference<Peer> const& self,Reference<IConnection> const& conn) 
															#line 2278 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> connectionWriter( Reference<Peer> const& self, Reference<IConnection> const& conn ) {
															#line 516 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionWriterActor(self, conn));
															#line 2309 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 552 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

															#line 2314 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via delayedHealthUpdate()
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class DelayedHealthUpdateActor>
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class DelayedHealthUpdateActorState {
															#line 2321 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	DelayedHealthUpdateActorState(NetworkAddress const& address,bool* const& tooManyConnectionsClosed) 
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : address(address),
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   tooManyConnectionsClosed(tooManyConnectionsClosed),
															#line 554 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   start(now())
															#line 2332 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 555 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 2347 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 572 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<DelayedHealthUpdateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DelayedHealthUpdateActorState(); static_cast<DelayedHealthUpdateActor*>(this)->destroy(); return 0; }
															#line 2370 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 556 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (FLOW_KNOBS->HEALTH_MONITOR_MARK_FAILED_UNSTABLE_CONNECTIONS && FlowTransport::transport().healthMonitor()->tooManyConnectionsClosed(address) && address.isPublic())
															#line 2389 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 558 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delayJittered(FLOW_KNOBS->MAX_RECONNECTION_TIME * 2.0);
															#line 558 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<DelayedHealthUpdateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2395 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DelayedHealthUpdateActor*>(this)->actor_wait_state = 1;
															#line 558 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DelayedHealthUpdateActor, 0, Void >*>(static_cast<DelayedHealthUpdateActor*>(this)));
															#line 2400 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 560 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (*tooManyConnectionsClosed)
															#line 2407 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 561 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("TooManyConnectionsClosedMarkAvailable") .detail("Dest", address) .detail("StartTime", start) .detail("TimeElapsed", now() - start) .detail("ClosedCount", FlowTransport::transport().healthMonitor()->closedConnectionsCount(address));
															#line 566 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				*tooManyConnectionsClosed = false;
															#line 2413 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 568 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().setStatus(address, FailureStatus(false));
															#line 2417 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress address;
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool* tooManyConnectionsClosed;
															#line 554 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	double start;
															#line 2523 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via delayedHealthUpdate()
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class DelayedHealthUpdateActor final : public Actor<Void>, public ActorCallback< DelayedHealthUpdateActor, 0, Void >, public FastAllocated<DelayedHealthUpdateActor>, public DelayedHealthUpdateActorState<DelayedHealthUpdateActor> {
															#line 2528 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<DelayedHealthUpdateActor>::operator new;
	using FastAllocated<DelayedHealthUpdateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DelayedHealthUpdateActor, 0, Void >;
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	DelayedHealthUpdateActor(NetworkAddress const& address,bool* const& tooManyConnectionsClosed) 
															#line 2539 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> delayedHealthUpdate( NetworkAddress const& address, bool* const& tooManyConnectionsClosed ) {
															#line 553 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new DelayedHealthUpdateActor(address, tooManyConnectionsClosed));
															#line 2567 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 574 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

															#line 2572 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionKeeper()
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionKeeperActor>
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionKeeperActorState {
															#line 2579 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionKeeperActorState(Reference<Peer> const& self,Reference<IConnection> const& conn = Reference<IConnection>(),Future<Void> const& reader = Void()) 
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn),
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   reader(reader)
															#line 2590 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 578 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevDebug, "ConnectionKeeper", conn ? conn->getDebugID() : UID()) .detail("PeerAddr", self->destination) .detail("ConnSet", (bool)conn);
															#line 581 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			ASSERT_WE_THINK(FlowTransport::transport().getLocalAddress() != self->destination);
															#line 583 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = Future<Void>();
															#line 584 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			firstConnFailedTime = Optional<double>();
															#line 585 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			retryConnect = false;
															#line 586 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			tooManyConnectionsClosed = false;
															#line 588 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 2617 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 590 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = Future<Void>();
															#line 592 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!conn)
															#line 2650 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 593 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->outgoingConnectionIdle = true;
															#line 595 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				;
															#line 2656 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = a_body1loopBody1loopHead1(loopDepth);
			}
			else
			{
															#line 673 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->outgoingConnectionIdle = false;
															#line 674 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->lastConnectTime = now();
															#line 2665 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 699 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF.cancel();
															#line 700 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (now() - self->lastConnectTime > FLOW_KNOBS->RECONNECTION_RESET_TIME)
															#line 2690 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 701 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->reconnectionDelay = FLOW_KNOBS->INITIAL_RECONNECTION_TIME;
															#line 2694 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 703 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->reconnectionDelay = std::min(FLOW_KNOBS->MAX_RECONNECTION_TIME, self->reconnectionDelay * FLOW_KNOBS->RECONNECTION_TIME_GROWTH_RATE);
															#line 2700 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 707 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (firstConnFailedTime.present())
															#line 2704 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 708 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (now() - firstConnFailedTime.get() > FLOW_KNOBS->PEER_UNAVAILABLE_FOR_LONG_TIME_TIMEOUT)
															#line 2708 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 709 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent(SevWarnAlways, "PeerUnavailableForLongTime", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 712 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					firstConnFailedTime = now() - FLOW_KNOBS->PEER_UNAVAILABLE_FOR_LONG_TIME_TIMEOUT / 2.0;
															#line 2714 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
			else
			{
															#line 715 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				firstConnFailedTime = now();
															#line 2721 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 720 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			retryConnect = true;
															#line 721 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_connection_failed)
															#line 2727 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 722 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (!self->destination.isPublic())
															#line 2731 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 724 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(true));
															#line 2735 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
				else
				{
															#line 725 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (now() - firstConnFailedTime.get() > FLOW_KNOBS->FAILURE_DETECTION_DELAY)
															#line 2741 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 726 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(true));
															#line 2745 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
			}
															#line 730 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->discardUnreliablePackets();
															#line 731 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			reader = Future<Void>();
															#line 732 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			bool ok = e.code() == error_code_connection_failed || e.code() == error_code_actor_cancelled || e.code() == error_code_connection_unreferenced || e.code() == error_code_connection_idle || (g_network->isSimulated() && e.code() == error_code_checksum_failed);
															#line 736 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (self->compatible)
															#line 2757 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 737 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent(ok ? SevInfo : SevWarnAlways, "ConnectionClosed", conn ? conn->getDebugID() : UID()) .errorUnsuppressed(e) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 2761 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 742 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent( ok ? SevInfo : SevWarnAlways, "IncompatibleConnectionClosed", conn ? conn->getDebugID() : UID()) .errorUnsuppressed(e) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 749 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->incompatibleProtocolVersionNewer = false;
															#line 2769 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 752 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (self->destination.isPublic() && IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() && !FlowTransport::isClient())
															#line 2773 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 755 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				auto& it = self->transport->closedPeers[self->destination];
															#line 756 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (now() - it.second > FLOW_KNOBS->TOO_MANY_CONNECTIONS_CLOSED_RESET_DELAY)
															#line 2779 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 757 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					it.first = now();
															#line 2783 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
				else
				{
															#line 758 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (now() - it.first > FLOW_KNOBS->TOO_MANY_CONNECTIONS_CLOSED_TIMEOUT)
															#line 2789 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 759 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						TraceEvent(SevWarnAlways, "TooManyConnectionsClosed", conn ? conn->getDebugID() : UID()) .suppressFor(5.0) .detail("PeerAddr", self->destination);
															#line 762 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						self->transport->degraded->set(true);
															#line 2795 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
															#line 764 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it.second = now();
															#line 2800 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 767 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (conn)
															#line 2804 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 768 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (self->destination.isPublic() && e.code() == error_code_connection_failed)
															#line 2808 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 769 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					FlowTransport::transport().healthMonitor()->reportPeerClosed(self->destination);
															#line 770 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (FLOW_KNOBS->HEALTH_MONITOR_MARK_FAILED_UNSTABLE_CONNECTIONS && FlowTransport::transport().healthMonitor()->tooManyConnectionsClosed(self->destination) && self->destination.isPublic())
															#line 2814 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 773 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						TraceEvent("TooManyConnectionsClosedMarkFailed") .detail("Dest", self->destination) .detail( "ClosedCount", FlowTransport::transport().healthMonitor()->closedConnectionsCount(self->destination));
															#line 778 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						tooManyConnectionsClosed = true;
															#line 779 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(true));
															#line 2822 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
															#line 783 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				conn->close();
															#line 784 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				conn = Reference<IConnection>();
															#line 789 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (e.code() != error_code_incompatible_protocol_version)
															#line 2831 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 790 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					self->protocolVersion->set(Optional<ProtocolVersion>());
															#line 2835 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
			}
															#line 795 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().notifyDisconnect(self->destination);
															#line 796 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			Promise<Void> disconnect = self->disconnect;
															#line 797 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->disconnect = Promise<Void>();
															#line 798 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			disconnect.send(Void());
															#line 800 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 2848 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 801 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 2852 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 804 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (self->peerReferences <= 0 && self->reliable.empty() && self->unsent.empty() && self->outstandingReplies == 0)
															#line 2856 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 806 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("PeerDestroy").errorUnsuppressed(e).suppressFor(1.0).detail("PeerAddr", self->destination);
															#line 807 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->connect.cancel();
															#line 808 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->transport->peers.erase(self->destination);
															#line 809 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->transport->orderedAddresses.erase(self->destination);
															#line 810 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (!static_cast<ConnectionKeeperActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionKeeperActorState(); static_cast<ConnectionKeeperActor*>(this)->destroy(); return 0; }
															#line 2868 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 677 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		firstConnFailedTime.reset();
															#line 2888 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		try {
															#line 679 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->transport->countConnEstablished++;
															#line 680 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!delayedHealthUpdateF.isValid())
															#line 2894 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 681 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				delayedHealthUpdateF = delayedHealthUpdate(self->destination, &tooManyConnectionsClosed);
															#line 2898 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 682 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_8 = connectionWriter(self, conn) || reader || connectionMonitor(self) || self->resetConnection.onTrigger();
															#line 682 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2Catch1(actor_cancelled(), loopDepth);
															#line 2904 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1loopBody1cont2Catch1(__when_expr_8.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_8.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 6;
															#line 682 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 8, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 2909 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 612 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		ASSERT(self->destination.isPublic());
															#line 613 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		self->outgoingConnectionIdle = false;
															#line 614 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = delayJittered(std::max(0.0, self->lastConnectTime + self->reconnectionDelay - now()));
															#line 614 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2930 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 2;
															#line 614 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 2, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 2935 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 595 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!(self->unsent.empty()))
															#line 2951 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 597 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		Future<Void> retryConnectF = Never();
															#line 598 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (retryConnect)
															#line 2959 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 599 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			retryConnectF = IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() ? delay(FLOW_KNOBS->FAILURE_DETECTION_DELAY) : delay(FLOW_KNOBS->SERVER_REQUEST_INTERVAL);
															#line 2963 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 605 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->dataToSend.onTrigger();
															#line 604 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2969 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 606 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = retryConnectF;
															#line 2973 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 1;
															#line 605 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 0, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 606 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 1, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 2980 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 617 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		self->lastConnectTime = now();
															#line 619 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectingTo", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination) .detail("PeerReferences", self->peerReferences) .detail("FailureStatus", IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() ? "OK" : "FAILED");
															#line 626 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		++self->connectOutgoingCount;
															#line 3133 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		try {
															#line 629 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(self->destination);
															#line 628 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3139 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont4Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
															#line 657 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_4 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3143 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1cont4Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont4when2(__when_expr_4.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 3;
															#line 629 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 657 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 4, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3150 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 617 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		self->lastConnectTime = now();
															#line 619 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectingTo", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination) .detail("PeerReferences", self->peerReferences) .detail("FailureStatus", IFailureMonitor::failureMonitor().getState(self->destination).isAvailable() ? "OK" : "FAILED");
															#line 626 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		++self->connectOutgoingCount;
															#line 3169 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		try {
															#line 629 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(self->destination);
															#line 628 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3175 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont4Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_3.get(), loopDepth); };
															#line 657 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_4 = delay(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 3179 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1cont4Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont4when2(__when_expr_4.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 3;
															#line 629 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 657 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 4, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3186 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 662 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			++self->connectFailedCount;
															#line 663 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() != error_code_connection_failed)
															#line 3273 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 664 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				return a_body1loopBody1Catch1(e, loopDepth);
															#line 3277 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 666 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionTimedOut", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 670 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1loopBody1Catch1(e, loopDepth);
															#line 3283 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 631 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		conn = _conn;
															#line 632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_5 = conn->connectHandshake();
															#line 632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3307 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1cont4Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont4when1when1(__when_expr_5.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 4;
															#line 632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 5, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3312 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Reference<IConnection> && _conn,int loopDepth) 
	{
															#line 631 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		conn = _conn;
															#line 632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_5 = conn->connectHandshake();
															#line 632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3325 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1cont4Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont4when1when1(__when_expr_5.get(), loopDepth); };
		static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 4;
															#line 632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 5, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3330 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4when2(Void const& _,int loopDepth) 
	{
															#line 658 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont4Catch1(connection_failed(), loopDepth);
															#line 3339 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont4when2(Void && _,int loopDepth) 
	{
															#line 658 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont4Catch1(connection_failed(), loopDepth);
															#line 3347 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"

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
															#line 633 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		self->connectLatencies.addSample(now() - self->lastConnectTime);
															#line 634 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (FlowTransport::isClient())
															#line 3364 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 635 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(false));
															#line 3368 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 637 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (self->unsent.empty())
															#line 3372 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 638 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = delayedHealthUpdate(self->destination, &tooManyConnectionsClosed);
															#line 641 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delayedHealthUpdateF;
															#line 640 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3380 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1cont4Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when1(__when_expr_6.get(), loopDepth); };
															#line 647 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_7 = self->dataToSend.onTrigger();
															#line 3384 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1cont4Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when2(__when_expr_7.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 5;
															#line 641 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 6, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 647 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 7, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3391 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 633 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		self->connectLatencies.addSample(now() - self->lastConnectTime);
															#line 634 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (FlowTransport::isClient())
															#line 3407 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 635 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			IFailureMonitor::failureMonitor().setStatus(self->destination, FailureStatus(false));
															#line 3411 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 637 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (self->unsent.empty())
															#line 3415 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 638 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			delayedHealthUpdateF = delayedHealthUpdate(self->destination, &tooManyConnectionsClosed);
															#line 641 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_6 = delayedHealthUpdateF;
															#line 640 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ConnectionKeeperActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont4Catch1(actor_cancelled(), loopDepth);
															#line 3423 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1cont4Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when1(__when_expr_6.get(), loopDepth); };
															#line 647 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_7 = self->dataToSend.onTrigger();
															#line 3427 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1cont4Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont4when1cont1when2(__when_expr_7.get(), loopDepth); };
			static_cast<ConnectionKeeperActor*>(this)->actor_wait_state = 5;
															#line 641 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 6, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 647 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ConnectionKeeperActor, 7, Void >*>(static_cast<ConnectionKeeperActor*>(this)));
															#line 3434 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 651 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectionExchangingConnectPacket", conn->getDebugID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 654 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		self->prependConnectPacket();
															#line 655 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		reader = connectionReader(self->transport, conn, self, Promise<Reference<Peer>>());
															#line 3515 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 642 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		conn->close();
															#line 643 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		conn = Reference<IConnection>();
															#line 644 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		retryConnect = false;
															#line 3534 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont4when1cont1when1(Void && _,int loopDepth) 
	{
															#line 642 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		conn->close();
															#line 643 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		conn = Reference<IConnection>();
															#line 644 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		retryConnect = false;
															#line 3547 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 689 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() == error_code_connection_failed || e.code() == error_code_actor_cancelled || e.code() == error_code_connection_unreferenced || (g_network->isSimulated() && e.code() == error_code_checksum_failed))
															#line 3769 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 692 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->transport->countConnClosedWithoutError++;
															#line 3773 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 694 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->transport->countConnClosedWithError++;
															#line 3779 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 696 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1loopBody1Catch1(e, loopDepth);
															#line 3783 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 684 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectionReset", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 687 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont2Catch1(connection_failed(), loopDepth);
															#line 3799 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 684 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TraceEvent("ConnectionReset", conn ? conn->getDebugID() : UID()) .suppressFor(1.0) .detail("PeerAddr", self->destination);
															#line 687 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		return a_body1loopBody1cont2Catch1(connection_failed(), loopDepth);
															#line 3809 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"

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
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> self;
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> reader;
															#line 583 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> delayedHealthUpdateF;
															#line 584 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Optional<double> firstConnFailedTime;
															#line 585 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int retryConnect;
															#line 586 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool tooManyConnectionsClosed;
															#line 3890 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionKeeper()
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionKeeperActor final : public Actor<Void>, public ActorCallback< ConnectionKeeperActor, 0, Void >, public ActorCallback< ConnectionKeeperActor, 1, Void >, public ActorCallback< ConnectionKeeperActor, 2, Void >, public ActorCallback< ConnectionKeeperActor, 3, Reference<IConnection> >, public ActorCallback< ConnectionKeeperActor, 5, Void >, public ActorCallback< ConnectionKeeperActor, 6, Void >, public ActorCallback< ConnectionKeeperActor, 7, Void >, public ActorCallback< ConnectionKeeperActor, 4, Void >, public ActorCallback< ConnectionKeeperActor, 8, Void >, public FastAllocated<ConnectionKeeperActor>, public ConnectionKeeperActorState<ConnectionKeeperActor> {
															#line 3895 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionKeeperActor(Reference<Peer> const& self,Reference<IConnection> const& conn = Reference<IConnection>(),Future<Void> const& reader = Void()) 
															#line 3914 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] Future<Void> connectionKeeper( Reference<Peer> const& self, Reference<IConnection> const& conn = Reference<IConnection>(), Future<Void> const& reader = Void() ) {
															#line 575 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionKeeperActor(self, conn, reader));
															#line 3947 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 815 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

Peer::Peer(TransportData* transport, NetworkAddress const& destination)
  : transport(transport), destination(destination), compatible(true), outgoingConnectionIdle(true),
    lastConnectTime(0.0), reconnectionDelay(FLOW_KNOBS->INITIAL_RECONNECTION_TIME), peerReferences(-1),
    incompatibleProtocolVersionNewer(false), bytesReceived(0), bytesSent(0), lastDataPacketSentTime(now()),
    outstandingReplies(0), pingLatencies(destination.isPublic() ? FLOW_KNOBS->PING_SAMPLE_AMOUNT : 1),
    lastLoggedTime(0.0), lastLoggedBytesReceived(0), lastLoggedBytesSent(0), timeoutCount(0),
    protocolVersion(Reference<AsyncVar<Optional<ProtocolVersion>>>(new AsyncVar<Optional<ProtocolVersion>>())),
    connectOutgoingCount(0), connectIncomingCount(0), connectFailedCount(0),
    connectLatencies(destination.isPublic() ? FLOW_KNOBS->NETWORK_CONNECT_SAMPLE_AMOUNT : 1) {
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
	if (transport->localAddresses.address.isTLS() == destination.isTLS()) {
		pkt.canonicalRemotePort = transport->localAddresses.address.port;
		pkt.setCanonicalRemoteIp(transport->localAddresses.address.ip);
	} else if (transport->localAddresses.secondaryAddress.present()) {
		pkt.canonicalRemotePort = transport->localAddresses.secondaryAddress.get().port;
		pkt.setCanonicalRemoteIp(transport->localAddresses.secondaryAddress.get().ip);
	} else {
		// a "mixed" TLS/non-TLS connection is like a client/server connection - there's no way to reverse it
		pkt.canonicalRemotePort = 0;
		pkt.setCanonicalRemoteIp(IPAddress(0));
	}

	pkt.connectPacketLength = sizeof(pkt) - sizeof(pkt.connectPacketLength);
	pkt.protocolVersion = g_network->protocolVersion();
	pkt.protocolVersion.addObjectSerializerFlag();
	pkt.connectionId = transport->transportId;

	PacketBuffer* pb_first = PacketBuffer::create();
	PacketWriter wr(pb_first, nullptr, Unversioned());
	pkt.serialize(wr);
	unsent.prependWriteBuffer(pb_first, wr.finish());
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
	NetworkAddress compatibleAddr = transport->localAddresses.address;
	if (transport->localAddresses.secondaryAddress.present() &&
	    transport->localAddresses.secondaryAddress.get().isTLS() == destination.isTLS()) {
		compatibleAddr = transport->localAddresses.secondaryAddress.get();
	}

	if (!destination.isPublic() || outgoingConnectionIdle || destination > compatibleAddr ||
	    (lastConnectTime > 1.0 && now() - lastConnectTime > FLOW_KNOBS->ALWAYS_ACCEPT_DELAY)) {
		// Keep the new connection
		TraceEvent("IncomingConnection", conn->getDebugID())
		    .suppressFor(1.0)
		    .detail("FromAddr", conn->getPeerAddress())
		    .detail("CanonicalAddr", destination)
		    .detail("IsPublic", destination.isPublic());

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
															#line 4073 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via deliver()
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class DeliverActor>
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class DeliverActorState {
															#line 4080 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	DeliverActorState(TransportData* const& self,Endpoint const& destination,TaskPriority const& priority,ArenaReader const& reader,bool const& inReadSocket,Future<Void> const& disconnect) 
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   destination(destination),
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   priority(priority),
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   reader(reader),
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   inReadSocket(inReadSocket),
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   disconnect(disconnect)
															#line 4097 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 948 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (priority < TaskPriority::ReadSocket || !inReadSocket)
															#line 4112 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 949 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				StrictFuture<Void> __when_expr_0 = orderedDelay(0, priority);
															#line 949 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 4118 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				static_cast<DeliverActor*>(this)->actor_wait_state = 1;
															#line 949 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeliverActor, 0, Void >*>(static_cast<DeliverActor*>(this)));
															#line 4122 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 951 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				g_network->setCurrentTask(priority);
															#line 4129 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 954 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		auto receiver = self->endpoints.get(destination.token);
															#line 955 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (receiver)
															#line 4154 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 956 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!checkCompatible(receiver->peerCompatibilityPolicy(), reader.protocolVersion()))
															#line 4158 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 957 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				delete static_cast<DeliverActor*>(this);
															#line 4162 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				return 0;
			}
			try {
															#line 960 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerAddress = destination.addresses;
															#line 961 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerDisconnect = disconnect;
															#line 962 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				StringRef data = reader.arenaReadAll();
															#line 963 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(data.size() > 8);
															#line 964 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				ArenaObjectReader objReader(reader.arena(), reader.arenaReadAll(), AssumeVersion(reader.protocolVersion()));
															#line 965 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				receiver->receive(objReader);
															#line 966 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerAddress = { NetworkAddress() };
															#line 967 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				g_currentDeliveryPeerDisconnect = Future<Void>();
															#line 4182 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 980 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (destination.token.first() & TOKEN_STREAM_FLAG)
															#line 4195 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 982 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (destination.token.first() != -1)
															#line 4199 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 983 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (self->isLocalAddress(destination.getPrimaryAddress()))
															#line 4203 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 984 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						sendLocal(self, SerializeSource<UID>(destination.token), Endpoint::wellKnown(destination.addresses, WLTOKEN_ENDPOINT_NOT_FOUND));
															#line 4207 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
					else
					{
															#line 988 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						Reference<Peer> peer = self->getOrOpenPeer(destination.getPrimaryAddress());
															#line 989 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						sendPacket(self, peer, SerializeSource<UID>(destination.token), Endpoint::wellKnown(destination.addresses, WLTOKEN_ENDPOINT_NOT_FOUND), false);
															#line 4215 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
		loopDepth = a_body1cont14(loopDepth);

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
															#line 969 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			g_currentDeliveryPeerAddress = { NetworkAddress() };
															#line 970 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			g_currentDeliveryPeerDisconnect = Future<Void>();
															#line 971 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevError, "ReceiverError") .error(e) .detail("Token", destination.token.toString()) .detail("Peer", destination.getPrimaryAddress());
															#line 975 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!FlowTransport::isClient())
															#line 4322 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 976 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				flushAndExit(FDB_EXIT_ERROR);
															#line 4326 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 978 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 4330 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
	int a_body1cont14(int loopDepth) 
	{
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		delete static_cast<DeliverActor*>(this);
															#line 4357 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Endpoint destination;
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TaskPriority priority;
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ArenaReader reader;
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool inReadSocket;
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> disconnect;
															#line 4374 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via deliver()
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class DeliverActor final : public Actor<void>, public ActorCallback< DeliverActor, 0, Void >, public FastAllocated<DeliverActor>, public DeliverActorState<DeliverActor> {
															#line 4379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<DeliverActor>::operator new;
	using FastAllocated<DeliverActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< DeliverActor, 0, Void >;
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	DeliverActor(TransportData* const& self,Endpoint const& destination,TaskPriority const& priority,ArenaReader const& reader,bool const& inReadSocket,Future<Void> const& disconnect) 
															#line 4390 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		 : Actor<void>(),
		   DeliverActorState<DeliverActor>(self, destination, priority, reader, inReadSocket, disconnect)
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
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
static void deliver( TransportData* const& self, Endpoint const& destination, TaskPriority const& priority, ArenaReader const& reader, bool const& inReadSocket, Future<Void> const& disconnect ) {
															#line 937 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	new DeliverActor(self, destination, priority, reader, inReadSocket, disconnect);
															#line 4409 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 998 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

static void scanPackets(TransportData* transport,
                        uint8_t*& unprocessed_begin,
                        const uint8_t* e,
                        Arena& arena,
                        NetworkAddress const& peerAddress,
                        ProtocolVersion peerProtocolVersion,
                        Future<Void> disconnect,
                        bool isStableConnection) {
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
		ASSERT(packetLen >= sizeof(UID));

		if (checksumEnabled) {
			bool isBuggifyEnabled = false;
			if (g_network->isSimulated() && !isStableConnection &&
			    g_network->now() - g_simulator.lastConnectionFailure > g_simulator.connectionFailuresDisableDuration &&
			    BUGGIFY_WITH_PROB(0.0001)) {
				g_simulator.lastConnectionFailure = g_network->now();
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
			TraceEvent(transport->warnAlwaysForLargePacket ? SevWarnAlways : SevWarn, "LargePacketReceived")
			    .suppressFor(1.0)
			    .detail("FromPeer", peerAddress.toString())
			    .detail("Length", (int)packetLen)
			    .detail("Token", token);

			if (g_network->isSimulated())
				transport->warnAlwaysForLargePacket = false;
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
			deliver(transport, Endpoint({ peerAddress }, token), priority, std::move(reader), true, disconnect);
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
															#line 4566 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionReader()
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionReaderActor>
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionReaderActorState {
															#line 4573 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionReaderActorState(TransportData* const& transport,Reference<IConnection> const& conn,Reference<Peer> const& peer,Promise<Reference<Peer>> const& onConnected) 
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : transport(transport),
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn),
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   peer(peer),
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   onConnected(onConnected),
															#line 1156 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   arena(),
															#line 1157 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   unprocessed_begin(nullptr),
															#line 1158 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   unprocessed_end(nullptr),
															#line 1159 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   buffer_end(nullptr),
															#line 1160 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   expectConnectPacket(true),
															#line 1161 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   compatible(false),
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   incompatiblePeerCounted(false),
															#line 1163 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   incompatibleProtocolVersionNewer(false),
															#line 1164 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   peerAddress(),
															#line 1165 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   peerProtocolVersion()
															#line 4606 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1167 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			peerAddress = conn->getPeerAddress();
															#line 1168 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!peer)
															#line 4623 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1169 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(!peerAddress.isPublic());
															#line 4627 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			try {
															#line 1172 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				;
															#line 4632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1340 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (incompatiblePeerCounted)
															#line 4662 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1341 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				ASSERT(peer && peer->transport->numIncompatibleConnections > 0);
															#line 1342 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->transport->numIncompatibleConnections--;
															#line 4668 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1344 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 4672 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1173 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 4693 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 1335 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = conn->onReadable();
															#line 1335 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4704 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 4;
															#line 1335 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 3, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 4709 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1174 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		readAllBytes = buffer_end - unprocessed_end;
															#line 1175 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (readAllBytes < FLOW_KNOBS->MIN_PACKET_BUFFER_FREE_BYTES)
															#line 4727 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1176 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			Arena newArena;
															#line 1177 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			const int unproc_len = unprocessed_end - unprocessed_begin;
															#line 1178 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			const int len = getNewBufferSize(unprocessed_begin, unprocessed_end, peerAddress, peerProtocolVersion);
															#line 1180 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			uint8_t* const newBuffer = new (newArena) uint8_t[len];
															#line 1181 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (unproc_len > 0)
															#line 4739 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1182 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				memcpy(newBuffer, unprocessed_begin, unproc_len);
															#line 4743 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1184 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			arena = newArena;
															#line 1185 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			unprocessed_begin = newBuffer;
															#line 1186 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			unprocessed_end = newBuffer + unproc_len;
															#line 1187 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			buffer_end = newBuffer + len;
															#line 1188 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			readAllBytes = buffer_end - unprocessed_end;
															#line 4755 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1191 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		totalReadBytes = 0;
															#line 1192 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		;
															#line 4761 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1203 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (peer)
															#line 4783 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1204 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			peer->bytesReceived += totalReadBytes;
															#line 4787 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1206 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (totalReadBytes == 0)
															#line 4791 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1208 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		readWillBlock = totalReadBytes != readAllBytes;
															#line 1210 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (expectConnectPacket && unprocessed_end - unprocessed_begin >= CONNECT_PACKET_V0_SIZE)
															#line 4799 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1213 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			int32_t connectPacketSize = ((ConnectPacket*)unprocessed_begin)->totalPacketSize();
															#line 1214 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (unprocessed_end - unprocessed_begin >= connectPacketSize)
															#line 4805 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1215 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				auto protocolVersion = ((ConnectPacket*)unprocessed_begin)->protocolVersion;
															#line 1216 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				BinaryReader pktReader(unprocessed_begin, connectPacketSize, AssumeVersion(protocolVersion));
															#line 1217 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				ConnectPacket pkt;
															#line 1218 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				serializer(pktReader, pkt);
															#line 1220 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				uint64_t connectionId = pkt.connectionId;
															#line 1221 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (!pkt.protocolVersion.hasObjectSerializerFlag() || !pkt.protocolVersion.isCompatible(g_network->protocolVersion()))
															#line 4819 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1223 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					incompatibleProtocolVersionNewer = pkt.protocolVersion > g_network->protocolVersion();
															#line 1224 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					NetworkAddress addr = pkt.canonicalRemotePort ? NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort) : conn->getPeerAddress();
															#line 1227 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (connectionId != 1)
															#line 4827 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1228 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						addr.port = 0;
															#line 4831 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1230 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (!transport->multiVersionConnections.count(connectionId))
															#line 4835 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1231 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						if (now() - transport->lastIncompatibleMessage > FLOW_KNOBS->CONNECTION_REJECTED_MESSAGE_DELAY)
															#line 4839 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1233 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
							TraceEvent(SevWarn, "ConnectionRejected", conn->getDebugID()) .detail("Reason", "IncompatibleProtocolVersion") .detail("LocalVersion", g_network->protocolVersion()) .detail("RejectedVersion", pkt.protocolVersion) .detail("Peer", pkt.canonicalRemotePort ? NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort) : conn->getPeerAddress()) .detail("ConnectionId", connectionId);
															#line 1242 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
							transport->lastIncompatibleMessage = now();
															#line 4845 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						}
															#line 1244 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						if (!transport->incompatiblePeers.count(addr))
															#line 4849 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1245 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
							transport->incompatiblePeers[addr] = std::make_pair(connectionId, now());
															#line 4853 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						}
					}
					else
					{
															#line 1247 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						if (connectionId > 1)
															#line 4860 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1248 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
							transport->multiVersionConnections[connectionId] = now() + FLOW_KNOBS->CONNECTION_ID_TIMEOUT;
															#line 4864 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						}
					}
															#line 1251 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					compatible = false;
															#line 1252 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (!protocolVersion.hasInexpensiveMultiVersionClient())
															#line 4871 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1253 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						if (peer)
															#line 4875 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						{
															#line 1254 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
							peer->protocolVersion->set(protocolVersion);
															#line 4879 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
						}
															#line 1259 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						return a_body1Catch2(incompatible_protocol_version(), std::max(0, loopDepth - 2));
															#line 4883 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
				}
				else
				{
															#line 1262 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					compatible = true;
															#line 1263 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent("ConnectionEstablished", conn->getDebugID()) .suppressFor(1.0) .detail("Peer", conn->getPeerAddress()) .detail("ConnectionId", connectionId);
															#line 4892 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1269 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (connectionId > 1)
															#line 4896 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1270 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					transport->multiVersionConnections[connectionId] = now() + FLOW_KNOBS->CONNECTION_ID_TIMEOUT;
															#line 4900 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1273 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				unprocessed_begin += connectPacketSize;
															#line 1274 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				expectConnectPacket = false;
															#line 1276 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (peer)
															#line 4908 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1277 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peerProtocolVersion = protocolVersion;
															#line 1279 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					TraceEvent("ConnectedOutgoing") .suppressFor(1.0) .detail("PeerAddr", NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort));
															#line 1282 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peer->compatible = compatible;
															#line 1283 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peer->incompatibleProtocolVersionNewer = incompatibleProtocolVersionNewer;
															#line 1284 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (!compatible)
															#line 4920 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1285 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						peer->transport->numIncompatibleConnections++;
															#line 1286 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						incompatiblePeerCounted = true;
															#line 4926 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1288 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					ASSERT(pkt.canonicalRemotePort == peerAddress.port);
															#line 1289 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					onConnected.send(peer);
															#line 4932 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					loopDepth = a_body1loopBody1loopBody1cont8(loopDepth);
				}
				else
				{
															#line 1291 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peerProtocolVersion = protocolVersion;
															#line 1292 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (pkt.canonicalRemotePort)
															#line 4941 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1293 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						peerAddress = NetworkAddress(pkt.canonicalRemoteIp(), pkt.canonicalRemotePort, true, peerAddress.isTLS(), NetworkAddressFromHostname(peerAddress.fromHostname));
															#line 4945 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1299 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peer = transport->getOrOpenPeer(peerAddress, false);
															#line 1300 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peer->compatible = compatible;
															#line 1301 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					peer->incompatibleProtocolVersionNewer = incompatibleProtocolVersionNewer;
															#line 1302 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (!compatible)
															#line 4955 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					{
															#line 1303 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						peer->transport->numIncompatibleConnections++;
															#line 1304 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
						incompatiblePeerCounted = true;
															#line 4961 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					}
															#line 1306 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					onConnected.send(peer);
															#line 1307 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					StrictFuture<Void> __when_expr_1 = delay(0);
															#line 1307 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 4969 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
					static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 2;
															#line 1307 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 1, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 4974 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1193 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		const int len = std::min<int>(buffer_end - unprocessed_end, FLOW_KNOBS->MAX_PACKET_SEND_BYTES);
															#line 1194 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (len == 0)
															#line 5003 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		readBytes = conn->read(unprocessed_end, unprocessed_end + len);
															#line 1197 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (readBytes == 0)
															#line 5011 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1199 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = yield(TaskPriority::ReadSocket);
															#line 1199 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 3));
															#line 5019 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 3)); else return a_body1loopBody1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 1;
															#line 1199 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 0, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5024 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1200 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		totalReadBytes += readBytes;
															#line 1201 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		unprocessed_end += readBytes;
															#line 5048 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1200 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		totalReadBytes += readBytes;
															#line 1201 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		unprocessed_end += readBytes;
															#line 5059 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1313 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!expectConnectPacket)
															#line 5131 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1314 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (compatible || peerProtocolVersion.hasStableInterfaces())
															#line 5135 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1315 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				scanPackets(transport, unprocessed_begin, unprocessed_end, arena, peerAddress, peerProtocolVersion, peer->disconnect.getFuture(), g_network->isSimulated() && conn->isStableConnection());
															#line 5139 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1324 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				unprocessed_begin = unprocessed_end;
															#line 1325 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				peer->resetPing.trigger();
															#line 5147 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1329 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (readWillBlock)
															#line 5152 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1332 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::ReadSocket);
															#line 1332 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 5160 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 3;
															#line 1332 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 2, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5165 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1309 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		peer->protocolVersion->set(peerProtocolVersion);
															#line 5180 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(0, TaskPriority::ReadSocket);
															#line 1336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5341 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 5;
															#line 1336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 4, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5346 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_4 = delay(0, TaskPriority::ReadSocket);
															#line 1336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionReaderActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5357 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<ConnectionReaderActor*>(this)->actor_wait_state = 5;
															#line 1336 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ConnectionReaderActor, 4, Void >*>(static_cast<ConnectionReaderActor*>(this)));
															#line 5362 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TransportData* transport;
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<Peer> peer;
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Promise<Reference<Peer>> onConnected;
															#line 1156 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Arena arena;
															#line 1157 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	uint8_t* unprocessed_begin;
															#line 1158 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	uint8_t* unprocessed_end;
															#line 1159 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	uint8_t* buffer_end;
															#line 1160 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool expectConnectPacket;
															#line 1161 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool compatible;
															#line 1162 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool incompatiblePeerCounted;
															#line 1163 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool incompatibleProtocolVersionNewer;
															#line 1164 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress peerAddress;
															#line 1165 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ProtocolVersion peerProtocolVersion;
															#line 1174 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int readAllBytes;
															#line 1191 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int totalReadBytes;
															#line 1196 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	int readBytes;
															#line 1208 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	bool readWillBlock;
															#line 5541 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionReader()
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionReaderActor final : public Actor<Void>, public ActorCallback< ConnectionReaderActor, 0, Void >, public ActorCallback< ConnectionReaderActor, 1, Void >, public ActorCallback< ConnectionReaderActor, 2, Void >, public ActorCallback< ConnectionReaderActor, 3, Void >, public ActorCallback< ConnectionReaderActor, 4, Void >, public FastAllocated<ConnectionReaderActor>, public ConnectionReaderActorState<ConnectionReaderActor> {
															#line 5546 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionReaderActor(TransportData* const& transport,Reference<IConnection> const& conn,Reference<Peer> const& peer,Promise<Reference<Peer>> const& onConnected) 
															#line 5561 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> connectionReader( TransportData* const& transport, Reference<IConnection> const& conn, Reference<Peer> const& peer, Promise<Reference<Peer>> const& onConnected ) {
															#line 1151 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionReaderActor(transport, conn, peer, onConnected));
															#line 5593 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1347 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

															#line 5598 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via connectionIncoming()
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class ConnectionIncomingActor>
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionIncomingActorState {
															#line 5605 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionIncomingActorState(TransportData* const& self,Reference<IConnection> const& conn) 
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   conn(conn)
															#line 5614 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1350 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				StrictFuture<Void> __when_expr_0 = conn->acceptHandshake();
															#line 1350 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5632 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 1;
															#line 1350 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 0, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 5637 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1368 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (e.code() != error_code_actor_cancelled)
															#line 5667 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1369 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent("IncomingConnectionError", conn->getDebugID()) .errorUnsuppressed(e) .suppressFor(1.0) .detail("FromAddress", conn->getPeerAddress());
															#line 5671 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1374 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			conn->close();
															#line 1375 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5677 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1351 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		onConnected = Promise<Reference<Peer>>();
															#line 1352 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		reader = connectionReader(self, conn, Reference<Peer>(), onConnected);
															#line 1354 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reader;
															#line 1353 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5701 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 1358 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<Peer>> __when_expr_2 = onConnected.getFuture();
															#line 5705 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont2when2(__when_expr_2.get(), loopDepth); };
															#line 1361 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 5709 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont2when3(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 2;
															#line 1354 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 1, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1358 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1361 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 3, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 5718 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1351 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		onConnected = Promise<Reference<Peer>>();
															#line 1352 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		reader = connectionReader(self, conn, Reference<Peer>(), onConnected);
															#line 1354 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_1 = reader;
															#line 1353 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ConnectionIncomingActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5733 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
															#line 1358 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<Peer>> __when_expr_2 = onConnected.getFuture();
															#line 5737 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont2when2(__when_expr_2.get(), loopDepth); };
															#line 1361 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delayJittered(FLOW_KNOBS->CONNECTION_MONITOR_TIMEOUT);
															#line 5741 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont2when3(__when_expr_3.get(), loopDepth); };
		static_cast<ConnectionIncomingActor*>(this)->actor_wait_state = 2;
															#line 1354 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 1, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1358 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 1361 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ConnectionIncomingActor, 3, Void >*>(static_cast<ConnectionIncomingActor*>(this)));
															#line 5750 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1366 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5822 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
															#line 1355 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		ASSERT(false);
															#line 1356 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5836 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
															#line 1355 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		ASSERT(false);
															#line 1356 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (!static_cast<ConnectionIncomingActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConnectionIncomingActorState(); static_cast<ConnectionIncomingActor*>(this)->destroy(); return 0; }
															#line 5850 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		new (&static_cast<ConnectionIncomingActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConnectionIncomingActorState();
		static_cast<ConnectionIncomingActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when2(Reference<Peer> const& p,int loopDepth) 
	{
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		p->onIncomingConnection(p, conn, reader);
															#line 5862 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(Reference<Peer> && p,int loopDepth) 
	{
															#line 1359 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		p->onIncomingConnection(p, conn, reader);
															#line 5871 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when3(Void const& _,int loopDepth) 
	{
															#line 1362 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TEST(true);
															#line 1363 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		return a_body1Catch2(timed_out(), loopDepth);
															#line 5882 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2when3(Void && _,int loopDepth) 
	{
															#line 1362 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		TEST(true);
															#line 1363 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		return a_body1Catch2(timed_out(), loopDepth);
															#line 5892 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"

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
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<IConnection> conn;
															#line 1351 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Promise<Reference<Peer>> onConnected;
															#line 1352 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Future<Void> reader;
															#line 6047 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via connectionIncoming()
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ConnectionIncomingActor final : public Actor<Void>, public ActorCallback< ConnectionIncomingActor, 0, Void >, public ActorCallback< ConnectionIncomingActor, 1, Void >, public ActorCallback< ConnectionIncomingActor, 2, Reference<Peer> >, public ActorCallback< ConnectionIncomingActor, 3, Void >, public FastAllocated<ConnectionIncomingActor>, public ConnectionIncomingActorState<ConnectionIncomingActor> {
															#line 6052 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ConnectionIncomingActor(TransportData* const& self,Reference<IConnection> const& conn) 
															#line 6066 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> connectionIncoming( TransportData* const& self, Reference<IConnection> const& conn ) {
															#line 1348 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ConnectionIncomingActor(self, conn));
															#line 6095 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1378 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

															#line 6100 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via listen()
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class ListenActor>
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ListenActorState {
															#line 6107 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ListenActorState(TransportData* const& self,NetworkAddress const& listenAddr) 
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self),
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   listenAddr(listenAddr),
															#line 1380 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   incoming(),
															#line 1382 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		   listener(INetworkConnections::net()->listen(listenAddr))
															#line 6120 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1383 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (!g_network->isSimulated() && self->localAddresses.address.port == 0)
															#line 6135 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1384 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				TraceEvent(SevInfo, "UpdatingListenAddress") .detail("AssignedListenAddress", listener->getListenAddress().toString());
															#line 1386 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				self->localAddresses.address = listener->getListenAddress();
															#line 6141 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
															#line 1388 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			connectionCount = 0;
															#line 6145 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			try {
															#line 1390 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				;
															#line 6149 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1405 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent(SevError, "ListenError").error(e);
															#line 1406 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 6181 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1391 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_0 = listener->accept();
															#line 1391 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6204 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ListenActor*>(this)->actor_wait_state = 1;
															#line 1391 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListenActor, 0, Reference<IConnection> >*>(static_cast<ListenActor*>(this)));
															#line 6209 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Reference<IConnection> const& conn,int loopDepth) 
	{
															#line 1392 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (conn)
															#line 6218 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1393 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionFrom", conn->getDebugID()) .suppressFor(1.0) .detail("FromAddress", conn->getPeerAddress()) .detail("ListenAddress", listenAddr.toString());
															#line 1397 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			incoming.add(connectionIncoming(self, conn));
															#line 6224 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1399 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		connectionCount++;
															#line 1400 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (connectionCount % (FLOW_KNOBS->ACCEPT_BATCH_SIZE) == 0)
															#line 6230 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1401 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::AcceptSocket);
															#line 1401 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6236 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ListenActor*>(this)->actor_wait_state = 2;
															#line 1401 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListenActor, 1, Void >*>(static_cast<ListenActor*>(this)));
															#line 6241 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1392 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (conn)
															#line 6255 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1393 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			TraceEvent("ConnectionFrom", conn->getDebugID()) .suppressFor(1.0) .detail("FromAddress", conn->getPeerAddress()) .detail("ListenAddress", listenAddr.toString());
															#line 1397 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			incoming.add(connectionIncoming(self, conn));
															#line 6261 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
															#line 1399 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		connectionCount++;
															#line 1400 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (connectionCount % (FLOW_KNOBS->ACCEPT_BATCH_SIZE) == 0)
															#line 6267 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1401 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::AcceptSocket);
															#line 1401 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (static_cast<ListenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6273 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ListenActor*>(this)->actor_wait_state = 2;
															#line 1401 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListenActor, 1, Void >*>(static_cast<ListenActor*>(this)));
															#line 6278 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	NetworkAddress listenAddr;
															#line 1380 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ActorCollectionNoErrors incoming;
															#line 1382 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	Reference<IListener> listener;
															#line 1388 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	uint64_t connectionCount;
															#line 6442 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via listen()
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class ListenActor final : public Actor<Void>, public ActorCallback< ListenActor, 0, Reference<IConnection> >, public ActorCallback< ListenActor, 1, Void >, public FastAllocated<ListenActor>, public ListenActorState<ListenActor> {
															#line 6447 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<ListenActor>::operator new;
	using FastAllocated<ListenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListenActor, 0, Reference<IConnection> >;
friend struct ActorCallback< ListenActor, 1, Void >;
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	ListenActor(TransportData* const& self,NetworkAddress const& listenAddr) 
															#line 6459 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> listen( TransportData* const& self, NetworkAddress const& listenAddr ) {
															#line 1379 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new ListenActor(self, listenAddr));
															#line 6488 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1409 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

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
	return address == localAddresses.address ||
	       (localAddresses.secondaryAddress.present() && address == localAddresses.secondaryAddress.get());
}

															#line 6522 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
namespace {
// This generated class is to be used only via multiVersionCleanupWorker()
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
template <class MultiVersionCleanupWorkerActor>
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class MultiVersionCleanupWorkerActorState {
															#line 6529 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	MultiVersionCleanupWorkerActorState(TransportData* const& self) 
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		 : self(self)
															#line 6536 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1440 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			;
															#line 6551 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1441 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(FLOW_KNOBS->CONNECTION_CLEANUP_DELAY);
															#line 1441 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (static_cast<MultiVersionCleanupWorkerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6583 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MultiVersionCleanupWorkerActor*>(this)->actor_wait_state = 1;
															#line 1441 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >*>(static_cast<MultiVersionCleanupWorkerActor*>(this)));
															#line 6588 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1442 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		bool foundIncompatible = false;
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->incompatiblePeers.begin();it != self->incompatiblePeers.end();) {
															#line 1444 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (self->multiVersionConnections.count(it->second.first))
															#line 6601 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1445 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it = self->incompatiblePeers.erase(it);
															#line 6605 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1447 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (now() - it->second.second > FLOW_KNOBS->INCOMPATIBLE_PEER_DELAY_BEFORE_LOGGING)
															#line 6611 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1448 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					foundIncompatible = true;
															#line 6615 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1450 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6619 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1454 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->multiVersionConnections.begin();it != self->multiVersionConnections.end();) {
															#line 1455 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (it->second < now())
															#line 6626 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1456 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it = self->multiVersionConnections.erase(it);
															#line 6630 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1458 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6636 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1462 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (foundIncompatible)
															#line 6641 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1463 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->incompatiblePeersChanged.trigger();
															#line 6645 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1442 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		bool foundIncompatible = false;
															#line 1443 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->incompatiblePeers.begin();it != self->incompatiblePeers.end();) {
															#line 1444 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (self->multiVersionConnections.count(it->second.first))
															#line 6659 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1445 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it = self->incompatiblePeers.erase(it);
															#line 6663 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1447 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				if (now() - it->second.second > FLOW_KNOBS->INCOMPATIBLE_PEER_DELAY_BEFORE_LOGGING)
															#line 6669 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				{
															#line 1448 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
					foundIncompatible = true;
															#line 6673 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
				}
															#line 1450 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6677 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1454 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		for(auto it = self->multiVersionConnections.begin();it != self->multiVersionConnections.end();) {
															#line 1455 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			if (it->second < now())
															#line 6684 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			{
															#line 1456 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it = self->multiVersionConnections.erase(it);
															#line 6688 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
			else
			{
															#line 1458 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
				it++;
															#line 6694 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
			}
		}
															#line 1462 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
		if (foundIncompatible)
															#line 6699 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
		{
															#line 1463 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
			self->incompatiblePeersChanged.trigger();
															#line 6703 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	TransportData* self;
															#line 6774 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
};
// This generated class is to be used only via multiVersionCleanupWorker()
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
class MultiVersionCleanupWorkerActor final : public Actor<Void>, public ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >, public FastAllocated<MultiVersionCleanupWorkerActor>, public MultiVersionCleanupWorkerActorState<MultiVersionCleanupWorkerActor> {
															#line 6779 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
public:
	using FastAllocated<MultiVersionCleanupWorkerActor>::operator new;
	using FastAllocated<MultiVersionCleanupWorkerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MultiVersionCleanupWorkerActor, 0, Void >;
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	MultiVersionCleanupWorkerActor(TransportData* const& self) 
															#line 6790 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
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
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
[[nodiscard]] static Future<Void> multiVersionCleanupWorker( TransportData* const& self ) {
															#line 1439 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"
	return Future<Void>(new MultiVersionCleanupWorkerActor(self));
															#line 6818 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.g.cpp"
}

#line 1467 "/usr/src/libfdb_c/fdbrpc/FlowTransport.actor.cpp"

FlowTransport::FlowTransport(uint64_t transportId, int maxWellKnownEndpoints)
  : self(new TransportData(transportId, maxWellKnownEndpoints)) {
	self->multiVersionCleanup = multiVersionCleanupWorker(self);
}

FlowTransport::~FlowTransport() {
	delete self;
}

void FlowTransport::initMetrics() {
	self->initMetrics();
}

NetworkAddressList FlowTransport::getLocalAddresses() const {
	return self->localAddresses;
}

NetworkAddress FlowTransport::getLocalAddress() const {
	return self->localAddresses.address;
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
	if (self->localAddresses.address == NetworkAddress()) {
		self->localAddresses.address = publicAddress;
	} else {
		self->localAddresses.secondaryAddress = publicAddress;
	}
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
		endpoint.addresses = self->localAddresses;
		endpoint.token = UID(endpoint.token.first() | TOKEN_STREAM_FLAG, endpoint.token.second());
	} else {
		endpoint.addresses = NetworkAddressList();
		endpoint.token = UID(endpoint.token.first() & ~TOKEN_STREAM_FLAG, endpoint.token.second());
	}
	self->endpoints.insert(receiver, endpoint.token, taskID);
}

void FlowTransport::addEndpoints(std::vector<std::pair<FlowReceiver*, TaskPriority>> const& streams) {
	self->endpoints.insert(self->localAddresses, streams);
}

void FlowTransport::removeEndpoint(const Endpoint& endpoint, NetworkMessageReceiver* receiver) {
	self->endpoints.remove(endpoint.token, receiver);
}

void FlowTransport::addWellKnownEndpoint(Endpoint& endpoint, NetworkMessageReceiver* receiver, TaskPriority taskID) {
	endpoint.addresses = self->localAddresses;
	ASSERT(receiver->isStream());
	self->endpoints.insertWellKnown(receiver, endpoint.token, taskID);
}

static void sendLocal(TransportData* self, ISerializeSource const& what, const Endpoint& destination) {
	TEST(true); // "Loopback" delivery
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
		        ArenaReader(copy.arena(), copy, AssumeVersion(currentProtocolVersion)),
		        false,
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
	    (peer->incompatibleProtocolVersionNewer &&
	     destination.token != Endpoint::wellKnownToken(WLTOKEN_PING_PACKET))) {
		TEST(true); // Can't send to private address without a compatible open connection
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
		TraceEvent(self->warnAlwaysForLargePacket ? SevWarnAlways : SevWarn, "LargePacketSent")
		    .suppressFor(1.0)
		    .detail("ToPeer", destination.getPrimaryAddress())
		    .detail("Length", (int)len)
		    .detail("Token", destination.token)
		    .backtrace();

		if (g_network->isSimulated())
			self->warnAlwaysForLargePacket = false;
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

void FlowTransport::createInstance(bool isClient, uint64_t transportId, int maxWellKnownEndpoints) {
	g_network->setGlobal(INetwork::enFlowTransport,
	                     (flowGlobalType) new FlowTransport(transportId, maxWellKnownEndpoints));
	g_network->setGlobal(INetwork::enNetworkAddressFunc, (flowGlobalType)&FlowTransport::getGlobalLocalAddress);
	g_network->setGlobal(INetwork::enNetworkAddressesFunc, (flowGlobalType)&FlowTransport::getGlobalLocalAddresses);
	g_network->setGlobal(INetwork::enFailureMonitor, (flowGlobalType) new SimpleFailureMonitor());
	g_network->setGlobal(INetwork::enClientFailureMonitor, isClient ? (flowGlobalType)1 : nullptr);
}

HealthMonitor* FlowTransport::healthMonitor() {
	return &self->healthMonitor;
}
