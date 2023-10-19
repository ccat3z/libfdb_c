#define POST_ACTOR_COMPILER 1
#line 1 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
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

#include "flow/Tracing.h"
#include "flow/UnitTest.h"
#include "flow/Knobs.h"
#include "flow/network.h"
#include <functional>
#include <iomanip>
#include <memory>

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
	void trace(OTELSpan const& span) override {}
};

struct LogfileTracer : ITracer {
	TracerType type() const override { return TracerType::LOG_FILE; }
	void trace(Span const& span) override {
		TraceEvent te(SevInfo, "TracingSpan", span.context);
		te.detail("Location", span.location.name)
		    .detail("Begin", format("%.6f", span.begin))
		    .detail("End", format("%.6f", span.end));
		if (span.parents.size() == 1) {
			te.detail("Parent", *span.parents.begin());
		} else {
			for (auto parent : span.parents) {
				TraceEvent(SevInfo, "TracingSpanAddParent", span.context).detail("AddParent", parent);
			}
		}
		for (const auto& [key, value] : span.tags) {
			TraceEvent(SevInfo, "TracingSpanTag", span.context).detail("Key", key).detail("Value", value);
		}
	}
	void trace(OTELSpan const& span) override {
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

struct TraceRequest {
	std::unique_ptr<uint8_t[]> buffer;
	// Amount of data in buffer (bytes).
	std::size_t data_size;
	// Size of buffer (bytes).
	std::size_t buffer_size;

	void write_byte(uint8_t byte) { write_bytes(&byte, 1); }

	void write_bytes(const uint8_t* buf, std::size_t n) {
		resize(n);
		std::copy(buf, buf + n, buffer.get() + data_size);
		data_size += n;
	}

	void resize(std::size_t n) {
		if (data_size + n <= buffer_size) {
			return;
		}

		std::size_t size = buffer_size;
		while (size < data_size + n) {
			size *= 2;
		}

		TraceEvent(SevInfo, "TracingSpanResizedBuffer").detail("OldSize", buffer_size).detail("NewSize", size);
		auto new_buffer = std::make_unique<uint8_t[]>(size);
		std::copy(buffer.get(), buffer.get() + data_size, new_buffer.get());
		buffer = std::move(new_buffer);
		buffer_size = size;
	}

	void reset() { data_size = 0; }
};

// A server listening for UDP trace messages, run only in simulation.
															#line 135 "/src/flow/Tracing.actor.g.cpp"
// This generated class is to be used only via simulationStartServer()
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class SimulationStartServerActor>
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class SimulationStartServerActorState {
															#line 141 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	SimulationStartServerActorState() 
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	{
															#line 147 "/src/flow/Tracing.actor.g.cpp"

	}
	~SimulationStartServerActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 136 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			TraceEvent(SevInfo, "UDPServerStarted") .detail("Address", "127.0.0.1") .detail("Port", FLOW_KNOBS->TRACING_UDP_LISTENER_PORT);
															#line 139 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			localAddress = NetworkAddress::parse("127.0.0.1:" + std::to_string(FLOW_KNOBS->TRACING_UDP_LISTENER_PORT));
															#line 141 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			StrictFuture<Reference<IUDPSocket>> __when_expr_0 = INetworkConnections::net()->createUDPSocket(localAddress);
															#line 141 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (static_cast<SimulationStartServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 165 "/src/flow/Tracing.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SimulationStartServerActor*>(this)->actor_wait_state = 1;
															#line 141 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*>(static_cast<SimulationStartServerActor*>(this)));
															#line 170 "/src/flow/Tracing.actor.g.cpp"
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
															#line 142 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		serverSocket->bind(localAddress);
															#line 144 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		packetString = makeString(IUDPSocket::MAX_PACKET_SIZE);
															#line 145 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		packet = mutateString(packetString);
															#line 147 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		;
															#line 199 "/src/flow/Tracing.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IUDPSocket> const& __serverSocket,int loopDepth) 
	{
															#line 141 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		serverSocket = __serverSocket;
															#line 208 "/src/flow/Tracing.actor.g.cpp"
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
	void a_callback_fire(ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*,Reference<IUDPSocket> && value) 
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
	void a_callback_error(ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >*,Error err) 
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
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 148 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		StrictFuture<int> __when_expr_1 = serverSocket->receive(packet, packet + IUDPSocket::MAX_PACKET_SIZE);
															#line 148 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		if (static_cast<SimulationStartServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 278 "/src/flow/Tracing.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SimulationStartServerActor*>(this)->actor_wait_state = 2;
															#line 148 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SimulationStartServerActor, 1, int >*>(static_cast<SimulationStartServerActor*>(this)));
															#line 283 "/src/flow/Tracing.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int const& size,int loopDepth) 
	{
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		auto message = packetString.substr(0, size);
															#line 155 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		ASSERT(message[0] == (4 | 0b10010000) || (5 | 0b10010000));
															#line 294 "/src/flow/Tracing.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int && size,int loopDepth) 
	{
															#line 149 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		auto message = packetString.substr(0, size);
															#line 155 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		ASSERT(message[0] == (4 | 0b10010000) || (5 | 0b10010000));
															#line 305 "/src/flow/Tracing.actor.g.cpp"
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
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< SimulationStartServerActor, 1, int >*,int && value) 
	{
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< SimulationStartServerActor, 1, int >*,Error err) 
	{
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
															#line 139 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	NetworkAddress localAddress;
															#line 141 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	Reference<IUDPSocket> serverSocket;
															#line 144 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	Standalone<StringRef> packetString;
															#line 145 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	uint8_t* packet;
															#line 375 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via simulationStartServer()
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class SimulationStartServerActor final : public Actor<Void>, public ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >, public ActorCallback< SimulationStartServerActor, 1, int >, public FastAllocated<SimulationStartServerActor>, public SimulationStartServerActorState<SimulationStartServerActor> {
															#line 380 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<SimulationStartServerActor>::operator new;
	using FastAllocated<SimulationStartServerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SimulationStartServerActor, 0, Reference<IUDPSocket> >;
friend struct ActorCallback< SimulationStartServerActor, 1, int >;
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	SimulationStartServerActor() 
															#line 392 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   SimulationStartServerActorState<SimulationStartServerActor>()
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("simulationStartServer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
[[nodiscard]] Future<Void> simulationStartServer(  ) {
															#line 133 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new SimulationStartServerActor());
															#line 418 "/src/flow/Tracing.actor.g.cpp"
}

#line 158 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"

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
	void serialize_span(const Span& span, TraceRequest& request) {
		// If you change the serialization format here, make sure to update the
		// fluentd filter to be able to correctly parse the updated format! See
		// the msgpack specification for more info on the bit patterns used
		// here.
		uint8_t size = 8;
		if (span.parents.size() == 0)
			--size;
		request.write_byte(size | 0b10010000); // write as array

		serialize_string(g_network->getLocalAddress().toString(), request); // ip:port

		serialize_value(span.context.first(), request, 0xcf); // trace id
		serialize_value(span.context.second(), request, 0xcf); // token (span id)

		serialize_value(span.begin, request, 0xcb); // start time
		serialize_value(span.end - span.begin, request, 0xcb); // duration

		serialize_string(span.location.name.toString(), request);

		serialize_map(span.tags, request);

		serialize_vector(span.parents, request);
	}

	void serialize_span(const OTELSpan& span, TraceRequest& request) {
		uint16_t size = 14;
		request.write_byte(size | 0b10010000); // write as array
		serialize_value(span.context.traceID.first(), request, 0xcf); // trace id
		serialize_value(span.context.traceID.second(), request, 0xcf); // trace id
		serialize_value(span.context.spanID, request, 0xcf); // spanid
		// parent value
		serialize_value(span.parentContext.traceID.first(), request, 0xcf); // trace id
		serialize_value(span.parentContext.traceID.second(), request, 0xcf); // trace id
		serialize_value(span.parentContext.spanID, request, 0xcf); // spanId
		// Payload
		serialize_string(span.location.name.toString(), request);
		serialize_value(span.begin, request, 0xcb); // start time
		serialize_value(span.end, request, 0xcb); // end
		// Kind
		serialize_value(span.kind, request, 0xcc);
		// Status
		serialize_value(span.status, request, 0xcc);
		// Links
		serialize_vector(span.links, request);
		// Events
		serialize_vector(span.events, request);
		// Attributes
		serialize_map(span.attributes, request);
	}

private:
	// Writes the given value in big-endian format to the request. Sets the
	// first byte to msgpack_type.
	template <typename T>
	inline void serialize_value(const T& val, TraceRequest& request, uint8_t msgpack_type) {
		request.write_byte(msgpack_type);

		const uint8_t* p = reinterpret_cast<const uint8_t*>(std::addressof(val));
		for (size_t i = 0; i < sizeof(T); ++i) {
			request.write_byte(p[sizeof(T) - i - 1]);
		}
	}

	// Writes the given string to the request as a sequence of bytes. Inserts a
	// format byte at the beginning of the string according to the its length,
	// as specified by the msgpack specification.
	inline void serialize_string(const uint8_t* c, int length, TraceRequest& request) {
		if (length <= 31) {
			// A size 0 string is ok. We still need to write a byte
			// identifiying the item as a string, but can set the size to 0.
			request.write_byte(static_cast<uint8_t>(length) | 0b10100000);
		} else if (length <= 255) {
			request.write_byte(0xd9);
			request.write_byte(static_cast<uint8_t>(length));
		} else if (length <= 65535) {
			request.write_byte(0xda);
			request.write_byte(reinterpret_cast<const uint8_t*>(&length)[1]);
			request.write_byte(reinterpret_cast<const uint8_t*>(&length)[0]);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeString")
			    .detail("Failed to MessagePack encode very large string", length);
			ASSERT_WE_THINK(false);
		}

		request.write_bytes(c, length);
	}

	inline void serialize_string(const std::string& str, TraceRequest& request) {
		serialize_string(reinterpret_cast<const uint8_t*>(str.data()), str.size(), request);
	}

	// Writes the given vector of SpanIDs to the request. If the vector is
	// empty, the request is not modified.
	inline void serialize_vector(const SmallVectorRef<SpanID>& vec, TraceRequest& request) {
		int size = vec.size();
		if (size == 0) {
			return;
		}
		if (size <= 15) {
			request.write_byte(static_cast<uint8_t>(size) | 0b10010000);
		} else if (size <= 65535) {
			request.write_byte(0xdc);
			request.write_byte(reinterpret_cast<const uint8_t*>(&size)[1]);
			request.write_byte(reinterpret_cast<const uint8_t*>(&size)[0]);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector")
			    .detail("Failed to MessagePack encode very large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& parentContext : vec) {
			serialize_value(parentContext.second(), request, 0xcf);
		}
	}

	// Writes the given vector of linked SpanContext's to the request. If the vector is
	// empty, the request is not modified.
	inline void serialize_vector(const SmallVectorRef<SpanContext>& vec, TraceRequest& request) {
		int size = vec.size();
		if (size <= 15) {
			request.write_byte(static_cast<uint8_t>(size) | 0b10010000);
		} else if (size <= 65535) {
			request.write_byte(0xdc);
			request.write_byte(reinterpret_cast<const uint8_t*>(&size)[1]);
			request.write_byte(reinterpret_cast<const uint8_t*>(&size)[0]);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector").detail("Failed to MessagePack encode large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& link : vec) {
			serialize_value(link.traceID.first(), request, 0xcf); // trace id
			serialize_value(link.traceID.second(), request, 0xcf); // trace id
			serialize_value(link.spanID, request, 0xcf); // spanid
		}
	}

	// Writes the given vector of linked SpanContext's to the request. If the vector is
	// empty, the request is not modified.
	inline void serialize_vector(const SmallVectorRef<OTELEventRef>& vec, TraceRequest& request) {
		int size = vec.size();
		if (size <= 15) {
			request.write_byte(static_cast<uint8_t>(size) | 0b10010000);
		} else if (size <= 65535) {
			request.write_byte(0xdc);
			request.write_byte(reinterpret_cast<const uint8_t*>(&size)[1]);
			request.write_byte(reinterpret_cast<const uint8_t*>(&size)[0]);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector").detail("Failed to MessagePack encode large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& event : vec) {
			serialize_string(event.name.toString(), request); // event name
			serialize_value(event.time, request, 0xcb); // event time
			serialize_vector(event.attributes, request);
		}
	}

	inline void serialize_vector(const SmallVectorRef<KeyValueRef>& vals, TraceRequest& request) {
		int size = vals.size();
		if (size <= 15) {
			// N.B. We're actually writing this out as a fixmap here in messagepack format!
			// fixmap	1000xxxx	0x80 - 0x8f
			request.write_byte(static_cast<uint8_t>(size) | 0b10000000);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeVector").detail("Failed to MessagePack encode large vector", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& kv : vals) {
			serialize_string(kv.key.toString(), request);
			serialize_string(kv.value.toString(), request);
		}
	}

	template <class Map>
	inline void serialize_map(const Map& map, TraceRequest& request) {
		int size = map.size();

		if (size <= 15) {
			request.write_byte(static_cast<uint8_t>(size) | 0b10000000);
		} else {
			TraceEvent(SevWarn, "TracingSpanSerializeMap").detail("Failed to MessagePack encode large map", size);
			ASSERT_WE_THINK(false);
		}

		for (const auto& [key, value] : map) {
			serialize_string(key.begin(), key.size(), request);
			serialize_string(value.begin(), value.size(), request);
		}
	}
};

#ifndef WIN32
															#line 645 "/src/flow/Tracing.actor.g.cpp"
// This generated class is to be used only via fastTraceLogger()
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class FastTraceLoggerActor>
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FastTraceLoggerActorState {
															#line 651 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FastTraceLoggerActorState(int* const& unreadyMessages,int* const& failedMessages,int* const& totalMessages,bool* const& sendError) 
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		 : unreadyMessages(unreadyMessages),
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		   failedMessages(failedMessages),
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		   totalMessages(totalMessages),
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		   sendError(sendError),
															#line 382 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		   sendErrorReset(false)
															#line 666 "/src/flow/Tracing.actor.g.cpp"
	{

	}
	~FastTraceLoggerActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 384 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			;
															#line 679 "/src/flow/Tracing.actor.g.cpp"
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
															#line 385 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		TraceEvent("TracingSpanStats") .detail("UnreadyMessages", *unreadyMessages) .detail("FailedMessages", *failedMessages) .detail("TotalMessages", *totalMessages) .detail("SendError", *sendError);
															#line 391 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		if (sendErrorReset)
															#line 711 "/src/flow/Tracing.actor.g.cpp"
		{
															#line 392 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			sendErrorReset = false;
															#line 393 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			*sendError = false;
															#line 717 "/src/flow/Tracing.actor.g.cpp"
		}
		else
		{
															#line 394 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (*sendError)
															#line 723 "/src/flow/Tracing.actor.g.cpp"
			{
															#line 395 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
				sendErrorReset = true;
															#line 727 "/src/flow/Tracing.actor.g.cpp"
			}
		}
															#line 398 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(kQueueSizeLogInterval);
															#line 398 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		if (static_cast<FastTraceLoggerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 734 "/src/flow/Tracing.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<FastTraceLoggerActor*>(this)->actor_wait_state = 1;
															#line 398 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FastTraceLoggerActor, 0, Void >*>(static_cast<FastTraceLoggerActor*>(this)));
															#line 739 "/src/flow/Tracing.actor.g.cpp"
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
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_fire(ActorCallback< FastTraceLoggerActor, 0, Void >*,Void && value) 
	{
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}

	}
	void a_callback_error(ActorCallback< FastTraceLoggerActor, 0, Void >*,Error err) 
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
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	int* unreadyMessages;
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	int* failedMessages;
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	int* totalMessages;
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	bool* sendError;
															#line 382 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	bool sendErrorReset;
															#line 823 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via fastTraceLogger()
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FastTraceLoggerActor final : public Actor<Void>, public ActorCallback< FastTraceLoggerActor, 0, Void >, public FastAllocated<FastTraceLoggerActor>, public FastTraceLoggerActorState<FastTraceLoggerActor> {
															#line 828 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<FastTraceLoggerActor>::operator new;
	using FastAllocated<FastTraceLoggerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FastTraceLoggerActor, 0, Void >;
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FastTraceLoggerActor(int* const& unreadyMessages,int* const& failedMessages,int* const& totalMessages,bool* const& sendError) 
															#line 839 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FastTraceLoggerActorState<FastTraceLoggerActor>(unreadyMessages, failedMessages, totalMessages, sendError)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("fastTraceLogger");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
[[nodiscard]] Future<Void> fastTraceLogger( int* const& unreadyMessages, int* const& failedMessages, int* const& totalMessages, bool* const& sendError ) {
															#line 381 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new FastTraceLoggerActor(unreadyMessages, failedMessages, totalMessages, sendError));
															#line 864 "/src/flow/Tracing.actor.g.cpp"
}

#line 401 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"

struct FastUDPTracer : public UDPTracer {
	FastUDPTracer()
	  : unready_socket_messages_(0), failed_messages_(0), total_messages_(0), socket_fd_(-1), send_error_(false) {
		request_ = TraceRequest{ .buffer = std::make_unique<uint8_t[]>(kTraceBufferSize),
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

	void trace(OTELSpan const& span) override {
		prepare(span.location.name.size());
		serialize_span(span, request_);
		write();
	}

	void trace(Span const& span) override {
		prepare(span.location.name.size());
		serialize_span(span, request_);
		write();
	}

private:
	TraceRequest request_;

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
	if (begin > 0.0 && context.second() > 0) {
		end = g_network->now();
		g_tracer->trace(*this);
	}
	arena = std::move(o.arena);
	context = o.context;
	begin = o.begin;
	end = o.end;
	location = o.location;
	parents = std::move(o.parents);
	o.begin = 0;
	return *this;
}

Span::~Span() {
	if (begin > 0.0 && context.second() > 0) {
		end = g_network->now();
		g_tracer->trace(*this);
	}
}

OTELSpan& OTELSpan::operator=(OTELSpan&& o) {
	if (begin > 0.0 && o.context.isSampled() > 0) {
		end = g_network->now();
		g_tracer->trace(*this);
	}
	arena = std::move(o.arena);
	context = o.context;
	parentContext = o.parentContext;
	begin = o.begin;
	end = o.end;
	location = o.location;
	links = std::move(o.links);
	events = std::move(o.events);
	status = o.status;
	kind = o.kind;
	o.context = SpanContext();
	o.parentContext = SpanContext();
	o.kind = SpanKind::INTERNAL;
	o.begin = 0.0;
	o.end = 0.0;
	o.status = SpanStatus::UNSET;
	return *this;
}

OTELSpan::~OTELSpan() {
	if (begin > 0.0 && context.isSampled()) {
		end = g_network->now();
		g_tracer->trace(*this);
	}
}

															#line 1035 "/src/flow/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase568()
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class FlowTestCase568Actor>
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase568ActorState {
															#line 1042 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase568ActorState(UnitTestParameters const& params) 
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		 : params(params)
															#line 1049 "/src/flow/Tracing.actor.g.cpp"
	{

	}
	~FlowTestCase568ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 570 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan notSampled("foo"_loc);
															#line 571 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(!notSampled.context.isSampled());
															#line 574 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan sampled("foo"_loc, []() { return 1.0; });
															#line 575 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(sampled.context.isSampled());
															#line 578 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan childTraceIDMatchesParent( "foo"_loc, []() { return 1.0; }, SpanContext(UID(100, 101), 200, TraceFlags::sampled));
															#line 580 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(childTraceIDMatchesParent.context.traceID.first() == childTraceIDMatchesParent.parentContext.traceID.first());
															#line 582 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(childTraceIDMatchesParent.context.traceID.second() == childTraceIDMatchesParent.parentContext.traceID.second());
															#line 587 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan parentNotSampled( "foo"_loc, []() { return 1.0; }, SpanContext(UID(1, 1), 1, TraceFlags::unsampled));
															#line 589 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(!parentNotSampled.context.isSampled());
															#line 594 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan noParent( "foo"_loc, []() { return 1.0; }, SpanContext(UID(0, 0), 0, TraceFlags::unsampled));
															#line 596 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(noParent.context.isSampled());
															#line 597 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase568Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase568ActorState(); static_cast<FlowTestCase568Actor*>(this)->destroy(); return 0; }
															#line 1084 "/src/flow/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase568Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase568ActorState();
			static_cast<FlowTestCase568Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase568ActorState();
		static_cast<FlowTestCase568Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1108 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase568()
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase568Actor final : public Actor<Void>, public FastAllocated<FlowTestCase568Actor>, public FlowTestCase568ActorState<FlowTestCase568Actor> {
															#line 1113 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase568Actor>::operator new;
	using FastAllocated<FlowTestCase568Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase568Actor(UnitTestParameters const& params) 
															#line 1123 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase568ActorState<FlowTestCase568Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase568");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
static Future<Void> flowTestCase568( UnitTestParameters const& params ) {
															#line 568 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase568Actor(params));
															#line 1148 "/src/flow/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase568, "/flow/Tracing/CreateOTELSpan")

#line 598 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"


															#line 1155 "/src/flow/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase600()
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class FlowTestCase600Actor>
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase600ActorState {
															#line 1162 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase600ActorState(UnitTestParameters const& params) 
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		 : params(params)
															#line 1169 "/src/flow/Tracing.actor.g.cpp"
	{

	}
	~FlowTestCase600ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 602 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span1("span_with_event"_loc);
															#line 603 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto arena = span1.arena;
															#line 604 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			SmallVectorRef<KeyValueRef> attrs;
															#line 605 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			attrs.push_back(arena, KeyValueRef("foo"_sr, "bar"_sr));
															#line 606 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span1.addEvent(LiteralStringRef("read_version"), 1.0, attrs);
															#line 607 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.events[0].name.toString() == "read_version");
															#line 608 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.events[0].time == 1.0);
															#line 609 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.events[0].attributes.begin()->key.toString() == "foo");
															#line 610 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.events[0].attributes.begin()->value.toString() == "bar");
															#line 613 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span2("span_with_event"_loc);
															#line 614 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span2.addEvent(StringRef(span2.arena, LiteralStringRef("commit_succeed")), 1234567.100);
															#line 615 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.events[0].name.toString() == "commit_succeed");
															#line 616 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.events[0].time == 1234567.100);
															#line 617 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.events[0].attributes.size() == 0);
															#line 620 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span3("span_with_event"_loc);
															#line 621 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto s3Arena = span3.arena;
															#line 622 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			SmallVectorRef<KeyValueRef> s3Attrs;
															#line 623 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			s3Attrs.push_back(s3Arena, KeyValueRef("xyz"_sr, "123"_sr));
															#line 624 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span3.addEvent("commit_fail"_sr, 1234567.100, s3Attrs).addEvent("commit_succeed"_sr, 1111.001, s3Attrs);
															#line 625 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[0].name.toString() == "commit_fail");
															#line 626 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[0].time == 1234567.100);
															#line 627 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[0].attributes.size() == 1);
															#line 628 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[0].attributes.begin()->key.toString() == "xyz");
															#line 629 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[0].attributes.begin()->value.toString() == "123");
															#line 630 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[1].name.toString() == "commit_succeed");
															#line 631 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[1].time == 1111.001);
															#line 632 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[1].attributes.size() == 1);
															#line 633 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[1].attributes.begin()->key.toString() == "xyz");
															#line 634 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.events[1].attributes.begin()->value.toString() == "123");
															#line 635 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase600Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase600ActorState(); static_cast<FlowTestCase600Actor*>(this)->destroy(); return 0; }
															#line 1240 "/src/flow/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase600Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase600ActorState();
			static_cast<FlowTestCase600Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase600ActorState();
		static_cast<FlowTestCase600Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1264 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase600()
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase600Actor final : public Actor<Void>, public FastAllocated<FlowTestCase600Actor>, public FlowTestCase600ActorState<FlowTestCase600Actor> {
															#line 1269 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase600Actor>::operator new;
	using FastAllocated<FlowTestCase600Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase600Actor(UnitTestParameters const& params) 
															#line 1279 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase600ActorState<FlowTestCase600Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase600");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
static Future<Void> flowTestCase600( UnitTestParameters const& params ) {
															#line 600 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase600Actor(params));
															#line 1304 "/src/flow/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase600, "/flow/Tracing/AddEvents")

#line 636 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"


															#line 1311 "/src/flow/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase638()
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class FlowTestCase638Actor>
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase638ActorState {
															#line 1318 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase638ActorState(UnitTestParameters const& params) 
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		 : params(params)
															#line 1325 "/src/flow/Tracing.actor.g.cpp"
	{

	}
	~FlowTestCase638ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 639 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span1("span_with_attrs"_loc);
															#line 640 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto arena = span1.arena;
															#line 641 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span1.addAttribute(StringRef(arena, LiteralStringRef("foo")), StringRef(arena, LiteralStringRef("bar")));
															#line 642 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span1.addAttribute(StringRef(arena, LiteralStringRef("operation")), StringRef(arena, LiteralStringRef("grv")));
															#line 643 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT_EQ(span1.attributes.size(), 3);
															#line 644 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.attributes[1] == KeyValueRef("foo"_sr, "bar"_sr));
															#line 645 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.attributes[2] == KeyValueRef("operation"_sr, "grv"_sr));
															#line 647 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span3("span_with_attrs"_loc);
															#line 648 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto s3Arena = span3.arena;
															#line 649 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span3.addAttribute(StringRef(s3Arena, LiteralStringRef("a")), StringRef(s3Arena, LiteralStringRef("1"))) .addAttribute(StringRef(s3Arena, LiteralStringRef("b")), LiteralStringRef("2")) .addAttribute(StringRef(s3Arena, LiteralStringRef("c")), LiteralStringRef("3"));
															#line 653 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT_EQ(span3.attributes.size(), 4);
															#line 654 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.attributes[1] == KeyValueRef("a"_sr, "1"_sr));
															#line 655 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.attributes[2] == KeyValueRef("b"_sr, "2"_sr));
															#line 656 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span3.attributes[3] == KeyValueRef("c"_sr, "3"_sr));
															#line 657 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase638Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase638ActorState(); static_cast<FlowTestCase638Actor*>(this)->destroy(); return 0; }
															#line 1366 "/src/flow/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase638Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase638ActorState();
			static_cast<FlowTestCase638Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase638ActorState();
		static_cast<FlowTestCase638Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1390 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase638()
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase638Actor final : public Actor<Void>, public FastAllocated<FlowTestCase638Actor>, public FlowTestCase638ActorState<FlowTestCase638Actor> {
															#line 1395 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase638Actor>::operator new;
	using FastAllocated<FlowTestCase638Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase638Actor(UnitTestParameters const& params) 
															#line 1405 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase638ActorState<FlowTestCase638Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase638");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
static Future<Void> flowTestCase638( UnitTestParameters const& params ) {
															#line 638 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase638Actor(params));
															#line 1430 "/src/flow/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase638, "/flow/Tracing/AddAttributes")

#line 658 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"


															#line 1437 "/src/flow/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase660()
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class FlowTestCase660Actor>
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase660ActorState {
															#line 1444 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase660ActorState(UnitTestParameters const& params) 
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		 : params(params)
															#line 1451 "/src/flow/Tracing.actor.g.cpp"
	{

	}
	~FlowTestCase660ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 661 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span1("span_with_links"_loc);
															#line 662 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span1.addLink(SpanContext(UID(100, 101), 200, TraceFlags::sampled));
															#line 663 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span1.addLink(SpanContext(UID(200, 201), 300, TraceFlags::unsampled)) .addLink(SpanContext(UID(300, 301), 400, TraceFlags::sampled));
															#line 666 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[0].traceID == UID(100, 101));
															#line 667 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[0].spanID == 200);
															#line 668 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[0].m_Flags == TraceFlags::sampled);
															#line 669 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[1].traceID == UID(200, 201));
															#line 670 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[1].spanID == 300);
															#line 671 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[1].m_Flags == TraceFlags::unsampled);
															#line 672 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[2].traceID == UID(300, 301));
															#line 673 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[2].spanID == 400);
															#line 674 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span1.links[2].m_Flags == TraceFlags::sampled);
															#line 676 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span2("span_with_links"_loc);
															#line 677 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto link1 = SpanContext(UID(1, 1), 1, TraceFlags::sampled);
															#line 678 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto link2 = SpanContext(UID(2, 2), 2, TraceFlags::sampled);
															#line 679 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto link3 = SpanContext(UID(3, 3), 3, TraceFlags::sampled);
															#line 680 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span2.addLinks({ link1, link2 }).addLinks({ link3 });
															#line 681 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[0].traceID == UID(1, 1));
															#line 682 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[0].spanID == 1);
															#line 683 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[0].m_Flags == TraceFlags::sampled);
															#line 684 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[1].traceID == UID(2, 2));
															#line 685 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[1].spanID == 2);
															#line 686 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[1].m_Flags == TraceFlags::sampled);
															#line 687 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[2].traceID == UID(3, 3));
															#line 688 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[2].spanID == 3);
															#line 689 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(span2.links[2].m_Flags == TraceFlags::sampled);
															#line 690 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase660Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase660ActorState(); static_cast<FlowTestCase660Actor*>(this)->destroy(); return 0; }
															#line 1516 "/src/flow/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase660Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase660ActorState();
			static_cast<FlowTestCase660Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase660ActorState();
		static_cast<FlowTestCase660Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1540 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase660()
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase660Actor final : public Actor<Void>, public FastAllocated<FlowTestCase660Actor>, public FlowTestCase660ActorState<FlowTestCase660Actor> {
															#line 1545 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase660Actor>::operator new;
	using FastAllocated<FlowTestCase660Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase660Actor(UnitTestParameters const& params) 
															#line 1555 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase660ActorState<FlowTestCase660Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase660");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
static Future<Void> flowTestCase660( UnitTestParameters const& params ) {
															#line 660 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase660Actor(params));
															#line 1580 "/src/flow/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase660, "/flow/Tracing/AddLinks")

#line 691 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"


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
															#line 1637 "/src/flow/Tracing.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase743()
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
template <class FlowTestCase743Actor>
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase743ActorState {
															#line 1644 "/src/flow/Tracing.actor.g.cpp"
public:
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase743ActorState(UnitTestParameters const& params) 
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
		 : params(params)
															#line 1651 "/src/flow/Tracing.actor.g.cpp"
	{

	}
	~FlowTestCase743ActorState() 
	{

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 744 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span1("encoded_span"_loc);
															#line 745 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto request = TraceRequest{ .buffer = std::make_unique<uint8_t[]>(kTraceBufferSize), .data_size = 0, .buffer_size = kTraceBufferSize };
															#line 748 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto tracer = FastUDPTracer();
															#line 749 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			tracer.serialize_span(span1, request);
															#line 750 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto data = request.buffer.get();
															#line 751 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011110);
															#line 752 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			request.reset();
															#line 756 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span2("encoded_span"_loc, SpanContext(UID(100, 101), 1, TraceFlags::sampled), SpanContext(UID(200, 201), 2, TraceFlags::sampled));
															#line 759 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			tracer.serialize_span(span2, request);
															#line 760 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			data = request.buffer.get();
															#line 761 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011110);
															#line 763 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[1] == 0xcf);
															#line 764 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[2]) == 100);
															#line 765 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[10] == 0xcf);
															#line 766 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[11]) == 101);
															#line 767 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[19] == 0xcf);
															#line 770 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[28] == 0xcf);
															#line 771 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[29]) == 100);
															#line 772 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[37] == 0xcf);
															#line 773 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[38]) == 101);
															#line 774 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[46] == 0xcf);
															#line 775 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[47]) == 1);
															#line 777 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(readMPString(&data[55]) == "encoded_span");
															#line 779 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[68] == 0xcb);
															#line 780 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[77] == 0xcb);
															#line 782 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[86] == 0xcc);
															#line 783 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[87] == static_cast<uint8_t>(SpanKind::SERVER));
															#line 785 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[88] == 0xcc);
															#line 786 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[89] == static_cast<uint8_t>(SpanStatus::OK));
															#line 788 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[90] == 0b10010001);
															#line 789 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[91] == 0xcf);
															#line 790 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[92]) == 200);
															#line 791 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[100] == 0xcf);
															#line 792 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[101]) == 201);
															#line 793 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[109] == 0xcf);
															#line 794 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[110]) == 2);
															#line 796 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[118] == 0b10010000);
															#line 798 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[119] == 0b10000001);
															#line 799 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[120] == 0b10100111);
															#line 801 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			request.reset();
															#line 804 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			OTELSpan span3("encoded_span_3"_loc);
															#line 805 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto s3Arena = span3.arena;
															#line 806 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			SmallVectorRef<KeyValueRef> attrs;
															#line 807 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			attrs.push_back(s3Arena, KeyValueRef("foo"_sr, "bar"_sr));
															#line 808 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span3.addAttribute("operation"_sr, "grv"_sr) .addLink(SpanContext(UID(300, 301), 400, TraceFlags::sampled)) .addEvent(StringRef(s3Arena, LiteralStringRef("event1")), 100.101, attrs);
															#line 811 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			tracer.serialize_span(span3, request);
															#line 812 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			data = request.buffer.get();
															#line 813 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011110);
															#line 816 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(readMPString(&data[55]) == "encoded_span_3");
															#line 818 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[70] == 0xcb);
															#line 819 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[79] == 0xcb);
															#line 821 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[88] == 0xcc);
															#line 822 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[89] == static_cast<uint8_t>(SpanKind::SERVER));
															#line 824 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[90] == 0xcc);
															#line 825 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[91] == static_cast<uint8_t>(SpanStatus::OK));
															#line 827 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[92] == 0b10010001);
															#line 828 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[93] == 0xcf);
															#line 829 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[94]) == 300);
															#line 830 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[102] == 0xcf);
															#line 831 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[103]) == 301);
															#line 832 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[111] == 0xcf);
															#line 833 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapUint64BE(&data[112]) == 400);
															#line 835 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[120] == 0b10010001);
															#line 836 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(readMPString(&data[121]) == "event1");
															#line 837 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[128] == 0xcb);
															#line 838 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(swapDoubleBE(&data[129]) == 100.101);
															#line 840 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[137] == 0b10000001);
															#line 841 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(readMPString(&data[138]) == "foo");
															#line 842 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(readMPString(&data[142]) == "bar");
															#line 844 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[146] == 0b10000010);
															#line 846 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			std::unordered_map<std::string, std::string> attributes;
															#line 847 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto index = 147;
															#line 849 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto firstKey = readMPString(&data[index]);
															#line 850 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			index += firstKey.length() + 1;
															#line 851 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto firstValue = readMPString(&data[index]);
															#line 852 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			index += firstValue.length() + 1;
															#line 853 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			attributes[firstKey] = firstValue;
															#line 855 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto secondKey = readMPString(&data[index]);
															#line 856 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			index += secondKey.length() + 1;
															#line 857 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto secondValue = readMPString(&data[index]);
															#line 858 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			attributes[secondKey] = secondValue;
															#line 860 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(attributes.find("address") != attributes.end());
															#line 861 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(attributes["operation"] == "grv");
															#line 863 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			request.reset();
															#line 866 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			const char* longString = "yGUtj42gSKfdqib3f0Ri4OVhD7eWyTbKsH/g9+x4UWyXry7NIBFIapPV9f1qdTRl" "2jXcZI8Ua/Gp8k9EBn7peaEN1uj4w9kf4FQ2Lalu0VrA4oquQoaKYr+wPsLBak9i" "uyZDF9sX/HW4pVvQhPQdXQWME5E7m58XFMpZ3H8HNXuytWInEuh97SRLlI0RhrvG" "ixNpYtYlvghsLCrEdZMMGnS2gXgGufIdg1xKJd30fUbZLHcYIC4DTnL5RBpkbQCR" "SGKKUrpIb/7zePhBDi+gzUzyAcbQ2zUbFWI1KNi3zQk58uUG6wWJZkw+GCs7Cc3V" "OUxOljwCJkC4QTgdsbbFhxUC+rtoHV5xAqoTQwR0FXnWigUjP7NtdL6huJUr3qRv" "40c4yUI1a4+P5vJa";
															#line 873 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto span4 = OTELSpan();
															#line 874 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			auto location = Location();
															#line 875 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			location.name = StringRef(span4.arena, longString);
															#line 876 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			span4.location = location;
															#line 877 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			tracer.serialize_span(span4, request);
															#line 878 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			data = request.buffer.get();
															#line 879 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[0] == 0b10011110);
															#line 882 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(data[55] == 0xda);
															#line 883 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			ASSERT(readMPString(&data[55]) == longString);
															#line 884 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
			if (!static_cast<FlowTestCase743Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase743ActorState(); static_cast<FlowTestCase743Actor*>(this)->destroy(); return 0; }
															#line 1852 "/src/flow/Tracing.actor.g.cpp"
			new (&static_cast<FlowTestCase743Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase743ActorState();
			static_cast<FlowTestCase743Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase743ActorState();
		static_cast<FlowTestCase743Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	UnitTestParameters params;
															#line 1876 "/src/flow/Tracing.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase743()
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
class FlowTestCase743Actor final : public Actor<Void>, public FastAllocated<FlowTestCase743Actor>, public FlowTestCase743ActorState<FlowTestCase743Actor> {
															#line 1881 "/src/flow/Tracing.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase743Actor>::operator new;
	using FastAllocated<FlowTestCase743Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	FlowTestCase743Actor(UnitTestParameters const& params) 
															#line 1891 "/src/flow/Tracing.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase743ActorState<FlowTestCase743Actor>(params)
	{
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase743");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();

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
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
static Future<Void> flowTestCase743( UnitTestParameters const& params ) {
															#line 743 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"
	return Future<Void>(new FlowTestCase743Actor(params));
															#line 1916 "/src/flow/Tracing.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase743, "/flow/Tracing/FastUDPMessagePackEncoding")

#line 885 "/tmp/fdb_c/foundationdb_ep/flow/Tracing.actor.cpp"

#endif
