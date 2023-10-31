#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
/*
 * Tracing.actor.cpp
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

#include "flow/Msgpack.h"
#include "fdbclient/Tracing.h"
#include "flow/IRandom.h"
#include "flow/UnitTest.h"
#include "flow/Knobs.h"
#include "flow/IConnection.h"
#include "fdbclient/IKnobCollection.h"
#include "flow/network.h"
#include <functional>
#include <iomanip>
#include <memory>
#include "flow/IUDPSocket.h"

#include "flow/actorcompiler.h" // has to be last include

#ifdef NO_INTELLISENSE
namespace {
#endif

// Initial size of buffer used to store serialized traces. Buffer will be
// resized when necessary.
constexpr int kTraceBufferSize = 1024;

// The time interval between each report of the tracer queue size (seconds).
constexpr float kQueueSizeLogInterval = 5.0;

struct NoopTracer : ITracer {
	TracerType type() const override { return TracerType::DISABLED; }
	void trace(Span const& span) override {}
};

struct LogfileTracer : ITracer {
	TracerType type() const override { return TracerType::LOG_FILE; }
	void trace(Span const& span) override {
		TraceEvent te(SevInfo, "TracingSpan", span.context.traceID);
		te.detail("SpanID", span.context.spanID)
		    .detail("Location", span.location.name)
		    .detail("Begin", format("%.6f", span.begin))
		    .detail("End", format("%.6f", span.end))
		    .detail("Kind", span.kind)
		    .detail("Status", span.status)
		    .detail("ParentSpanID", span.parentContext.spanID);

		for (const auto& link : span.links) {
			TraceEvent(SevInfo, "TracingSpanLink", span.context.traceID)
			    .detail("TraceID", link.traceID)
			    .detail("SpanID", link.spanID);
		}
		for (const auto& [key, value] : span.attributes) {
			TraceEvent(SevInfo, "TracingSpanTag", span.context.traceID).detail("Key", key).detail("Value", value);
		}
		for (const auto& event : span.events) {
			TraceEvent(SevInfo, "TracingSpanEvent", span.context.traceID)
			    .detail("Name", event.name)
			    .detail("Time", event.time);
			for (const auto& [key, value] : event.attributes) {
				TraceEvent(SevInfo, "TracingSpanEventAttribute", span.context.traceID)
				    .detail("Key", key)
				    .detail("Value", value);
			}
		}
	}
};

// A server listening for UDP trace messages, run only in simulation.
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
// This generated class is to be used only via simulationStartServer()
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class SimulationStartServerActor>
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class SimulationStartServerActorState {
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	SimulationStartServerActorState() 
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		fdb_probe_actor_create("simulationStartServer", reinterpret_cast<unsigned long>(this));

	}
	~SimulationStartServerActorState() 
	{
		fdb_probe_actor_destroy("simulationStartServer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			TraceEvent(SevInfo, "UDPServerStarted") .detail("Address", "127.0.0.1") .detail("Port", FLOW_KNOBS->TRACING_UDP_LISTENER_PORT);
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			localAddress = NetworkAddress::parse("127.0.0.1:" + std::to_string(FLOW_KNOBS->TRACING_UDP_LISTENER_PORT));
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			StrictFuture<Reference<IUDPSocket>> __when_expr_0 = INetworkConnections::net()->createUDPSocket(localAddress);
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (static_cast<SimulationStartServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SimulationStartServerActor*>(this)->actor_wait_state = 1;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*>(static_cast<SimulationStartServerActor*>(this)));
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
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
		this->~SimulationStartServerActorState();
		static_cast<SimulationStartServerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		serverSocket->bind(localAddress);
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		packetString = makeString(IUDPSocket::MAX_PACKET_SIZE);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		packet = mutateString(packetString);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IUDPSocket> const& __serverSocket,int loopDepth) 
	{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		serverSocket = __serverSocket;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IUDPSocket> && __serverSocket,int loopDepth) 
	{
		serverSocket = std::move(__serverSocket);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SimulationStartServerActor*>(this)->actor_wait_state > 0) static_cast<SimulationStartServerActor*>(this)->actor_wait_state = 0;
		static_cast<SimulationStartServerActor*>(this)->ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >::remove();

	}
	void a_callback_fire(ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*,Reference<IUDPSocket> const& value) 
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*,Reference<IUDPSocket> && value) 
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*,Error err) 
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		StrictFuture<int> __when_expr_1 = serverSocket->receive(packet, packet + IUDPSocket::MAX_PACKET_SIZE);
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		if (static_cast<SimulationStartServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SimulationStartServerActor*>(this)->actor_wait_state = 2;
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SimulationStartServerActor, 1, int >*>(static_cast<SimulationStartServerActor*>(this)));
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int const& size,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		auto message = packetString.substr(0, size);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		ASSERT(message[0] == (4 | 0b10010000) || (5 | 0b10010000));
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int && size,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		auto message = packetString.substr(0, size);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		ASSERT(message[0] == (4 | 0b10010000) || (5 | 0b10010000));
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(int const& size,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(size, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(int && size,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(size), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SimulationStartServerActor*>(this)->actor_wait_state > 0) static_cast<SimulationStartServerActor*>(this)->actor_wait_state = 0;
		static_cast<SimulationStartServerActor*>(this)->ActorCallback< SimulationStartServerActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< SimulationStartServerActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SimulationStartServerActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SimulationStartServerActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	NetworkAddress localAddress;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	Reference<IUDPSocket> serverSocket;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	Standalone<StringRef> packetString;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	uint8_t* packet;
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via simulationStartServer()
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class SimulationStartServerActor final : public Actor<Void>, public ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >, public ActorCallback< SimulationStartServerActor, 1, int >, public FastAllocated<SimulationStartServerActor>, public SimulationStartServerActorState<SimulationStartServerActor> {
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<SimulationStartServerActor>::operator new;
	using FastAllocated<SimulationStartServerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >;
friend struct ActorCallback< SimulationStartServerActor, 1, int >;
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	SimulationStartServerActor() 
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   SimulationStartServerActorState<SimulationStartServerActor>()
	{
		fdb_probe_actor_enter("simulationStartServer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("simulationStartServer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("simulationStartServer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SimulationStartServerActor, 1, int >*)0, actor_cancelled()); break;
		}

	}
};
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
[[nodiscard]] Future<Void> simulationStartServer(  ) {
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new SimulationStartServerActor());
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}

#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"

/*
// Runs on an interval, printing debug information and performing other
// connection tasks.
ACTOR Future<Void> traceLog(int* pendingMessages, bool* sendError) {
    state bool sendErrorReset = false;

    loop {
        TraceEvent("TracingSpanQueueSize").detail("PendingMessages", *pendingMessages);

        // Wait at least one full loop before attempting to send messages
        // again.
        if (sendErrorReset) {
            sendErrorReset = false;
            *sendError = false;
        } else if (*sendError) {
            sendErrorReset = true;
        }

        wait(delay(kQueueSizeLogInterval));
    }
}
*/

struct UDPTracer : public ITracer {
	// Serializes span fields as an array into the supplied TraceRequest
	// buffer.
	void serialize_span(const Span& span, MsgpackBuffer& buf) {
		uint16_t size = 12;
		buf.write_byte(size | 0b10010000); // write as array
		serialize_value(span.context.traceID.first(), buf, 0xcf); // trace id
		serialize_value(span.context.traceID.second(), buf, 0xcf); // trace id
		serialize_value(span.context.spanID, buf, 0xcf); // spanid
		// parent span id
		serialize_value(span.parentContext.spanID, buf, 0xcf); // spanId
		// Payload
		serialize_string(span.location.name.toString(), buf);
		serialize_value(span.begin, buf, 0xcb); // start time
		serialize_value(span.end, buf, 0xcb); // end
		// Kind
		serialize_value(span.kind, buf, 0xcc);
		// Status
		serialize_value(span.status, buf, 0xcc);
		// Links
		serialize_vector(span.links, buf);
		// Events
		serialize_vector(span.events, buf);
		// Attributes
		serialize_map(span.attributes, buf);
	}

private:
	// Writes the given vector of linked SpanContext's to the request. If the vector is
	// empty, the request is not modified.
	inline void serialize_vector(const SmallVectorRef<SpanContext>& vec, MsgpackBuffer& buf) {
		int size = vec.size();
		if (size <= 15) {
			buf.write_byte(static_cast<uint8_t>(size) | 0b10010000);
		} else if (size <= 65535) {
			buf.write_byte(0xdc);
			buf.write_byte(reinterpret_cast<const uint8_t*>(&size)[1]);
			buf.write_byte(reinterpret_cast<const uint8_t*>(&size)[0]);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector").detail("Failed to MessagePack encode large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& link : vec) {
			serialize_value(link.traceID.first(), buf, 0xcf); // trace id
			serialize_value(link.traceID.second(), buf, 0xcf); // trace id
			serialize_value(link.spanID, buf, 0xcf); // spanid
		}
	}

	// Writes the given vector of linked SpanEventRef's to the request. If the vector is
	// empty, the request is not modified.
	inline void serialize_vector(const SmallVectorRef<SpanEventRef>& vec, MsgpackBuffer& buf) {
		int size = vec.size();
		if (size <= 15) {
			buf.write_byte(static_cast<uint8_t>(size) | 0b10010000);
		} else if (size <= 65535) {
			buf.write_byte(0xdc);
			buf.write_byte(reinterpret_cast<const uint8_t*>(&size)[1]);
			buf.write_byte(reinterpret_cast<const uint8_t*>(&size)[0]);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector").detail("Failed to MessagePack encode large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& event : vec) {
			serialize_string(event.name.toString(), buf); // event name
			serialize_value(event.time, buf, 0xcb); // event time
			serialize_vector(event.attributes, buf);
		}
	}

	inline void serialize_vector(const SmallVectorRef<KeyValueRef>& vals, MsgpackBuffer& buf) {
		int size = vals.size();
		if (size <= 15) {
			// N.B. We're actually writing this out as a fixmap here in messagepack format!
			// fixmap	1000xxxx	0x80 - 0x8f
			buf.write_byte(static_cast<uint8_t>(size) | 0b10000000);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector").detail("Failed to MessagePack encode large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& kv : vals) {
			serialize_string(kv.key.toString(), buf);
			serialize_string(kv.value.toString(), buf);
		}
	}
};

#ifndef WIN32
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
// This generated class is to be used only via fastTraceLogger()
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class FastTraceLoggerActor>
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FastTraceLoggerActorState {
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FastTraceLoggerActorState(int* const& unreadyMessages,int* const& failedMessages,int* const& totalMessages,bool* const& sendError) 
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		 : unreadyMessages(unreadyMessages),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		   failedMessages(failedMessages),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		   totalMessages(totalMessages),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		   sendError(sendError),
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		   sendErrorReset(false)
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
	{
		fdb_probe_actor_create("fastTraceLogger", reinterpret_cast<unsigned long>(this));

	}
	~FastTraceLoggerActorState() 
	{
		fdb_probe_actor_destroy("fastTraceLogger", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
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
		this->~FastTraceLoggerActorState();
		static_cast<FastTraceLoggerActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		TraceEvent("TracingSpanStats") .detail("UnreadyMessages", *unreadyMessages) .detail("FailedMessages", *failedMessages) .detail("TotalMessages", *totalMessages) .detail("SendError", *sendError);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		if (sendErrorReset)
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			sendErrorReset = false;
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			*sendError = false;
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		}
		else
		{
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (*sendError)
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
				sendErrorReset = true;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			}
		}
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(kQueueSizeLogInterval);
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		if (static_cast<FastTraceLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FastTraceLoggerActor*>(this)->actor_wait_state = 1;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FastTraceLoggerActor, 0, Void >*>(static_cast<FastTraceLoggerActor*>(this)));
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<FastTraceLoggerActor*>(this)->actor_wait_state > 0) static_cast<FastTraceLoggerActor*>(this)->actor_wait_state = 0;
		static_cast<FastTraceLoggerActor*>(this)->ActorCallback< FastTraceLoggerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FastTraceLoggerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("fastTraceLogger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fastTraceLogger", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FastTraceLoggerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("fastTraceLogger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fastTraceLogger", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FastTraceLoggerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("fastTraceLogger", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("fastTraceLogger", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	int* unreadyMessages;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	int* failedMessages;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	int* totalMessages;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	bool* sendError;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	bool sendErrorReset;
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via fastTraceLogger()
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FastTraceLoggerActor final : public Actor<Void>, public ActorCallback< FastTraceLoggerActor, 0, Void >, public FastAllocated<FastTraceLoggerActor>, public FastTraceLoggerActorState<FastTraceLoggerActor> {
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<FastTraceLoggerActor>::operator new;
	using FastAllocated<FastTraceLoggerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FastTraceLoggerActor, 0, Void >;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FastTraceLoggerActor(int* const& unreadyMessages,int* const& failedMessages,int* const& totalMessages,bool* const& sendError) 
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FastTraceLoggerActorState<FastTraceLoggerActor>(unreadyMessages, failedMessages, totalMessages, sendError)
	{
		fdb_probe_actor_enter("fastTraceLogger", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fastTraceLogger");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("fastTraceLogger", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FastTraceLoggerActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
[[nodiscard]] Future<Void> fastTraceLogger( int* const& unreadyMessages, int* const& failedMessages, int* const& totalMessages, bool* const& sendError ) {
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new FastTraceLoggerActor(unreadyMessages, failedMessages, totalMessages, sendError));
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}

#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"

struct FastUDPTracer : public UDPTracer {
	FastUDPTracer()
	  : unready_socket_messages_(0), failed_messages_(0), total_messages_(0), socket_fd_(-1), send_error_(false) {
		request_ = MsgpackBuffer{ .buffer = std::make_unique<uint8_t[]>(kTraceBufferSize),
			                      .data_size = 0,
			                      .buffer_size = kTraceBufferSize };
	}

	TracerType type() const override { return TracerType::NETWORK_LOSSY; }

	void prepare(int size) {
		static std::once_flag once;
		std::call_once(once, [&]() {
			log_actor_ = fastTraceLogger(&unready_socket_messages_, &failed_messages_, &total_messages_, &send_error_);
			std::string destAddr = FLOW_KNOBS->TRACING_UDP_LISTENER_ADDR;
			if (g_network->isSimulated()) {
				udp_server_actor_ = simulationStartServer();
				// Force loopback when in simulation mode
				destAddr = "127.0.0.1";
			}
			NetworkAddress destAddress =
			    NetworkAddress::parse(destAddr + ":" + std::to_string(FLOW_KNOBS->TRACING_UDP_LISTENER_PORT));

			socket_ = INetworkConnections::net()->createUDPSocket(destAddress);
		});

		if (size == 0) {
			return;
		}

		++total_messages_;
		if (!socket_.isReady()) {
			++unready_socket_messages_;
			return;
		} else if (socket_fd_ == -1) {
			socket_fd_ = socket_.get()->native_handle();
		}

		if (send_error_) {
			return;
		}
	}

	void write() {
		int bytesSent = send(socket_fd_, request_.buffer.get(), request_.data_size, MSG_DONTWAIT);
		if (bytesSent == -1) {
			// Will forgo checking errno here, and assume all error messages
			// should be treated the same.
			++failed_messages_;
			send_error_ = true;
		}
		request_.reset();
	}

	void trace(Span const& span) override {
		prepare(span.location.name.size());
		serialize_span(span, request_);
		write();
	}

private:
	MsgpackBuffer request_;

	int unready_socket_messages_;
	int failed_messages_;
	int total_messages_;

	int socket_fd_;
	bool send_error_;

	Future<Reference<IUDPSocket>> socket_;
	Future<Void> log_actor_;
	Future<Void> udp_server_actor_;
};
#endif

ITracer* g_tracer = new NoopTracer();

#ifdef NO_INTELLISENSE
} // namespace
#endif

void openTracer(TracerType type) {
	if (g_tracer->type() == type) {
		return;
	}
	delete g_tracer;
	switch (type) {
	case TracerType::DISABLED:
		g_tracer = new NoopTracer{};
		break;
	case TracerType::LOG_FILE:
		g_tracer = new LogfileTracer{};
		break;
	case TracerType::NETWORK_LOSSY:
#ifndef WIN32
		g_tracer = new FastUDPTracer{};
#endif
		break;
	case TracerType::SIM_END:
		ASSERT(false);
		break;
	}
}

ITracer::~ITracer() {}

Span& Span::operator=(Span&& o) {
	if (begin > 0.0 && context.isSampled()) {
		end = g_network->now();
		g_tracer->trace(*this);
	}
	arena = std::move(o.arena);
	// All memory referenced in *Ref fields of Span is now (potentially)
	// invalid, and o no longer has ownership of any memory referenced by *Ref
	// fields of o. We must ensure that o no longer references any memory it no
	// longer owns, and that *this no longer references any memory it no longer
	// owns. Not every field references arena memory, but this std::exchange
	// pattern provides a nice template for getting this right in a concise way
	// should we add more fields to Span.

	attributes = std::exchange(o.attributes, decltype(o.attributes)());
	begin = std::exchange(o.begin, decltype(o.begin)());
	context = std::exchange(o.context, decltype(o.context)());
	end = std::exchange(o.end, decltype(o.end)());
	events = std::exchange(o.events, decltype(o.events)());
	kind = std::exchange(o.kind, decltype(o.kind)());
	links = std::exchange(o.links, decltype(o.links)());
	location = std::exchange(o.location, decltype(o.location)());
	parentContext = std::exchange(o.parentContext, decltype(o.parentContext)());
	status = std::exchange(o.status, decltype(o.status)());

	return *this;
}

Span::~Span() {
	if (begin > 0.0 && context.isSampled()) {
		end = g_network->now();
		g_tracer->trace(*this);
	}
}

															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase389()
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class FlowTestCase389Actor>
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase389ActorState {
															#line 889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase389ActorState(UnitTestParameters const& params) 
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		 : params(params)
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase389", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase389ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase389", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span notSampled("foo"_loc);
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(!notSampled.context.isSampled());
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span childTraceIDMatchesParent("foo"_loc, SpanContext(UID(100, 101), 200, TraceFlags::sampled));
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(childTraceIDMatchesParent.context.traceID.first() == childTraceIDMatchesParent.parentContext.traceID.first());
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(childTraceIDMatchesParent.context.traceID.second() == childTraceIDMatchesParent.parentContext.traceID.second());
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span parentNotSampled("foo"_loc, SpanContext(UID(1, 1), 1, TraceFlags::unsampled));
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(!parentNotSampled.context.isSampled());
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase389Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase389ActorState(); static_cast<FlowTestCase389Actor*>(this)->destroy(); return 0; }
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase389Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase389ActorState();
			static_cast<FlowTestCase389Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase389ActorState();
		static_cast<FlowTestCase389Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase389()
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase389Actor final : public Actor<Void>, public FastAllocated<FlowTestCase389Actor>, public FlowTestCase389ActorState<FlowTestCase389Actor> {
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase389Actor>::operator new;
	using FastAllocated<FlowTestCase389Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase389Actor(UnitTestParameters const& params) 
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase389ActorState<FlowTestCase389Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase389", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase389");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase389", reinterpret_cast<unsigned long>(this), -1);

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
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
static Future<Void> flowTestCase389( UnitTestParameters const& params ) {
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase389Actor(params));
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase389, "/flow/Tracing/CreateOTELSpan")

#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"


															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase409()
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class FlowTestCase409Actor>
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase409ActorState {
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase409ActorState(UnitTestParameters const& params) 
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		 : params(params)
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
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
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span1("span_with_event"_loc);
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto arena = span1.arena;
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			SmallVectorRef<KeyValueRef> attrs;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			attrs.push_back(arena, KeyValueRef("foo"_sr, "bar"_sr));
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span1.addEvent("read_version"_sr, 1.0, attrs);
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.events[0].name.toString() == "read_version");
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.events[0].time == 1.0);
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.events[0].attributes.begin()->key.toString() == "foo");
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.events[0].attributes.begin()->value.toString() == "bar");
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span2("span_with_event"_loc);
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span2.addEvent(StringRef(span2.arena, "commit_succeed"_sr), 1234567.100);
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.events[0].name.toString() == "commit_succeed");
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.events[0].time == 1234567.100);
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.events[0].attributes.size() == 0);
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span3("span_with_event"_loc);
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto s3Arena = span3.arena;
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			SmallVectorRef<KeyValueRef> s3Attrs;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			s3Attrs.push_back(s3Arena, KeyValueRef("xyz"_sr, "123"_sr));
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span3.addEvent("commit_fail"_sr, 1234567.100, s3Attrs).addEvent("commit_succeed"_sr, 1111.001, s3Attrs);
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[0].name.toString() == "commit_fail");
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[0].time == 1234567.100);
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[0].attributes.size() == 1);
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[0].attributes.begin()->key.toString() == "xyz");
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[0].attributes.begin()->value.toString() == "123");
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[1].name.toString() == "commit_succeed");
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[1].time == 1111.001);
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[1].attributes.size() == 1);
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[1].attributes.begin()->key.toString() == "xyz");
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span3.events[1].attributes.begin()->value.toString() == "123");
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase409Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase409ActorState(); static_cast<FlowTestCase409Actor*>(this)->destroy(); return 0; }
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase409Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase409ActorState();
			static_cast<FlowTestCase409Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase409ActorState();
		static_cast<FlowTestCase409Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase409()
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase409Actor final : public Actor<Void>, public FastAllocated<FlowTestCase409Actor>, public FlowTestCase409ActorState<FlowTestCase409Actor> {
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase409Actor>::operator new;
	using FastAllocated<FlowTestCase409Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase409Actor(UnitTestParameters const& params) 
															#line 1124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
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
		}

	}
};
}
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
static Future<Void> flowTestCase409( UnitTestParameters const& params ) {
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase409Actor(params));
															#line 1151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase409, "/flow/Tracing/AddEvents")

#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"


															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase447()
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class FlowTestCase447Actor>
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase447ActorState {
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase447ActorState(UnitTestParameters const& params) 
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		 : params(params)
															#line 1172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase447", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase447ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase447", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span1("span_with_attrs"_loc, SpanContext(deterministicRandom()->randomUniqueID(), deterministicRandom()->randomUInt64(), TraceFlags::sampled));
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto arena = span1.arena;
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span1.addAttribute(StringRef(arena, "foo"_sr), StringRef(arena, "bar"_sr));
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span1.addAttribute(StringRef(arena, "operation"_sr), StringRef(arena, "grv"_sr));
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT_EQ(span1.attributes.size(), 3);
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.attributes[1] == KeyValueRef("foo"_sr, "bar"_sr));
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.attributes[2] == KeyValueRef("operation"_sr, "grv"_sr));
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span2("span_with_attrs"_loc, SpanContext(deterministicRandom()->randomUniqueID(), deterministicRandom()->randomUInt64(), TraceFlags::sampled));
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto s2Arena = span2.arena;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span2.addAttribute(StringRef(s2Arena, "a"_sr), StringRef(s2Arena, "1"_sr)) .addAttribute(StringRef(s2Arena, "b"_sr), "2"_sr) .addAttribute(StringRef(s2Arena, "c"_sr), "3"_sr);
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT_EQ(span2.attributes.size(), 4);
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.attributes[1] == KeyValueRef("a"_sr, "1"_sr));
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.attributes[2] == KeyValueRef("b"_sr, "2"_sr));
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.attributes[3] == KeyValueRef("c"_sr, "3"_sr));
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase447Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase447ActorState(); static_cast<FlowTestCase447Actor*>(this)->destroy(); return 0; }
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase447Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase447ActorState();
			static_cast<FlowTestCase447Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase447ActorState();
		static_cast<FlowTestCase447Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase447()
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase447Actor final : public Actor<Void>, public FastAllocated<FlowTestCase447Actor>, public FlowTestCase447ActorState<FlowTestCase447Actor> {
															#line 1244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase447Actor>::operator new;
	using FastAllocated<FlowTestCase447Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase447Actor(UnitTestParameters const& params) 
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase447ActorState<FlowTestCase447Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase447", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase447");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase447", reinterpret_cast<unsigned long>(this), -1);

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
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
static Future<Void> flowTestCase447( UnitTestParameters const& params ) {
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase447Actor(params));
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase447, "/flow/Tracing/AddAttributes")

#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"


															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase475()
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class FlowTestCase475Actor>
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase475ActorState {
															#line 1295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase475ActorState(UnitTestParameters const& params) 
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		 : params(params)
															#line 1302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase475", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase475ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase475", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span1("span_with_links"_loc);
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(!span1.context.isSampled());
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(!span1.context.isValid());
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span1.addLink(SpanContext(UID(100, 101), 200, TraceFlags::sampled));
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span1.addLink(SpanContext(UID(200, 201), 300, TraceFlags::unsampled)) .addLink(SpanContext(UID(300, 301), 400, TraceFlags::sampled));
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.context.isSampled());
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.context.isValid());
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[0].traceID == UID(100, 101));
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[0].spanID == 200);
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[0].m_Flags == TraceFlags::sampled);
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[1].traceID == UID(200, 201));
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[1].spanID == 300);
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[1].m_Flags == TraceFlags::unsampled);
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[2].traceID == UID(300, 301));
															#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[2].spanID == 400);
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span1.links[2].m_Flags == TraceFlags::sampled);
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span2("span_with_links"_loc);
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(!span2.context.isSampled());
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(!span2.context.isValid());
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto link1 = SpanContext(UID(1, 1), 1, TraceFlags::sampled);
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto link2 = SpanContext(UID(2, 2), 2, TraceFlags::sampled);
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto link3 = SpanContext(UID(3, 3), 3, TraceFlags::sampled);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span2.addLinks({ link1, link2 }).addLinks({ link3 });
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.context.isSampled());
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.context.isValid());
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[0].traceID == UID(1, 1));
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[0].spanID == 1);
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[0].m_Flags == TraceFlags::sampled);
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[1].traceID == UID(2, 2));
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[1].spanID == 2);
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[1].m_Flags == TraceFlags::sampled);
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[2].traceID == UID(3, 3));
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[2].spanID == 3);
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(span2.links[2].m_Flags == TraceFlags::sampled);
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase475Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase475ActorState(); static_cast<FlowTestCase475Actor*>(this)->destroy(); return 0; }
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase475Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase475ActorState();
			static_cast<FlowTestCase475Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase475ActorState();
		static_cast<FlowTestCase475Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase475()
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase475Actor final : public Actor<Void>, public FastAllocated<FlowTestCase475Actor>, public FlowTestCase475ActorState<FlowTestCase475Actor> {
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase475Actor>::operator new;
	using FastAllocated<FlowTestCase475Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase475Actor(UnitTestParameters const& params) 
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase475ActorState<FlowTestCase475Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase475", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase475");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase475", reinterpret_cast<unsigned long>(this), -1);

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
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
static Future<Void> flowTestCase475( UnitTestParameters const& params ) {
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase475Actor(params));
															#line 1451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase475, "/flow/Tracing/AddLinks")

#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"


uint16_t swapUint16BE(uint8_t* index) {
	uint16_t value;
	memcpy(&value, index, sizeof(value));
	return fromBigEndian16(value);
}

uint64_t swapUint64BE(uint8_t* index) {
	uint64_t value;
	memcpy(&value, index, sizeof(value));
	return fromBigEndian64(value);
}

double swapDoubleBE(uint8_t* index) {
	double value;
	memcpy(&value, index, sizeof(value));
	char* const p = reinterpret_cast<char*>(&value);
	for (size_t i = 0; i < sizeof(double) / 2; ++i)
		std::swap(p[i], p[sizeof(double) - i - 1]);
	return value;
}

std::string readMPString(uint8_t* index, int len) {
	uint8_t data[len + 1];
	std::copy(index, index + len, data);
	data[len] = '\0';
	return reinterpret_cast<char*>(data);
}

std::string readMPString(uint8_t* index) {
	auto len = 0;
	switch (*index) {
	case 0xda:
		index++; // read the size in the next 2 bytes
		len = swapUint16BE(index);
		index += 2; // move index past the size bytes
		break;
	default:
		// We & out the bits here that contain the length the initial 3 higher order bits are
		// to signify this is a string of len <= 31 chars.
		len = static_cast<uint8_t>(*index & 0b00011111);
		index++;
	}
	uint8_t data[len + 1];
	std::copy(index, index + len, data);
	data[len] = '\0';
	return reinterpret_cast<char*>(data);
}

// Windows doesn't like lack of header and declaration of constructor for FastUDPTracer
#ifndef WIN32
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase569()
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
template <class FlowTestCase569Actor>
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase569ActorState {
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase569ActorState(UnitTestParameters const& params) 
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
		 : params(params)
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase569", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase569ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase569", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span1("encoded_span"_loc);
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto request = MsgpackBuffer{ .buffer = std::make_unique<uint8_t[]>(kTraceBufferSize), .data_size = 0, .buffer_size = kTraceBufferSize };
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto tracer = FastUDPTracer();
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			tracer.serialize_span(span1, request);
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto data = request.buffer.get();
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011100);
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			request.reset();
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span2("encoded_span"_loc, SpanContext(UID(100, 101), 1, TraceFlags::sampled), { SpanContext(UID(200, 201), 2, TraceFlags::sampled) });
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			tracer.serialize_span(span2, request);
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			data = request.buffer.get();
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011100);
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[1] == 0xcf);
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[2]) == 100);
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[10] == 0xcf);
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[11]) == 101);
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[19] == 0xcf);
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[28] == 0xcf);
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[29]) == 1);
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(readMPString(&data[37]) == "encoded_span");
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[50] == 0xcb);
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[59] == 0xcb);
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[68] == 0xcc);
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[69] == static_cast<uint8_t>(SpanKind::SERVER));
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[70] == 0xcc);
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[71] == static_cast<uint8_t>(SpanStatus::OK));
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[72] == 0b10010001);
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[73] == 0xcf);
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[74]) == 200);
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[82] == 0xcf);
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[83]) == 201);
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[91] == 0xcf);
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[92]) == 2);
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[100] == 0b10010000);
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[101] == 0b10000001);
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[102] == 0b10100111);
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			request.reset();
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span3("encoded_span_3"_loc, SpanContext());
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto s3Arena = span3.arena;
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			SmallVectorRef<KeyValueRef> attrs;
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			attrs.push_back(s3Arena, KeyValueRef("foo"_sr, "bar"_sr));
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span3.addAttribute("operation"_sr, "grv"_sr) .addLink(SpanContext(UID(300, 301), 400, TraceFlags::sampled)) .addEvent(StringRef(s3Arena, "event1"_sr), 100.101, attrs);
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			tracer.serialize_span(span3, request);
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			data = request.buffer.get();
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011100);
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(readMPString(&data[37]) == "encoded_span_3");
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[52] == 0xcb);
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[61] == 0xcb);
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[70] == 0xcc);
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[71] == static_cast<uint8_t>(SpanKind::SERVER));
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[72] == 0xcc);
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[73] == static_cast<uint8_t>(SpanStatus::OK));
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[74] == 0b10010001);
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[75] == 0xcf);
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[76]) == 300);
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[84] == 0xcf);
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[85]) == 301);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[93] == 0xcf);
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[94]) == 400);
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[102] == 0b10010001);
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(readMPString(&data[103]) == "event1");
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[110] == 0xcb);
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(swapDoubleBE(&data[111]) == 100.101);
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[119] == 0b10000001);
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(readMPString(&data[120]) == "foo");
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(readMPString(&data[124]) == "bar");
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[128] == 0b10000010);
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			std::unordered_map<std::string, std::string> attributes;
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto index = 129;
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto firstKey = readMPString(&data[index]);
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			index += firstKey.length() + 1;
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto firstValue = readMPString(&data[index]);
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			index += firstValue.length() + 1;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			attributes[firstKey] = firstValue;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto secondKey = readMPString(&data[index]);
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			index += secondKey.length() + 1;
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto secondValue = readMPString(&data[index]);
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			attributes[secondKey] = secondValue;
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(attributes.find("address") != attributes.end());
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(attributes["operation"] == "grv");
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			request.reset();
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			const char* longString = "yGUtj42gSKfdqib3f0Ri4OVhD7eWyTbKsH/g9+x4UWyXry7NIBFIapPV9f1qdTRl" "2jXcZI8Ua/Gp8k9EBn7peaEN1uj4w9kf4FQ2Lalu0VrA4oquQoaKYr+wPsLBak9i" "uyZDF9sX/HW4pVvQhPQdXQWME5E7m58XFMpZ3H8HNXuytWInEuh97SRLlI0RhrvG" "ixNpYtYlvghsLCrEdZMMGnS2gXgGufIdg1xKJd30fUbZLHcYIC4DTnL5RBpkbQCR" "SGKKUrpIb/7zePhBDi+gzUzyAcbQ2zUbFWI1KNi3zQk58uUG6wWJZkw+GCs7Cc3V" "OUxOljwCJkC4QTgdsbbFhxUC+rtoHV5xAqoTQwR0FXnWigUjP7NtdL6huJUr3qRv" "40c4yUI1a4+P5vJa";
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			Span span4;
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			auto location = Location();
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			location.name = StringRef(span4.arena, longString);
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			span4.location = location;
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			tracer.serialize_span(span4, request);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			data = request.buffer.get();
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011100);
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(data[37] == 0xda);
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			ASSERT(readMPString(&data[37]) == longString);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase569Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase569ActorState(); static_cast<FlowTestCase569Actor*>(this)->destroy(); return 0; }
															#line 1717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase569Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase569ActorState();
			static_cast<FlowTestCase569Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase569ActorState();
		static_cast<FlowTestCase569Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase569()
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
class FlowTestCase569Actor final : public Actor<Void>, public FastAllocated<FlowTestCase569Actor>, public FlowTestCase569ActorState<FlowTestCase569Actor> {
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase569Actor>::operator new;
	using FastAllocated<FlowTestCase569Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	FlowTestCase569Actor(UnitTestParameters const& params) 
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase569ActorState<FlowTestCase569Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase569", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase569");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase569", reinterpret_cast<unsigned long>(this), -1);

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
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
static Future<Void> flowTestCase569( UnitTestParameters const& params ) {
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase569Actor(params));
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase569, "/flow/Tracing/FastUDPMessagePackEncoding")

#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/Tracing.actor.cpp"

#endif
