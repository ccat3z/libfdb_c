#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
/*
 * HTTP.actor.cpp
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

#include "fdbrpc/HTTP.h"
#include "fdbrpc/simulator.h"

#include "flow/IRandom.h"
#include "flow/Net2Packet.h"
#include "openssl/md5.h"
#include "libb64/encode.h"
#include "flow/Knobs.h"
#include <cctype>
#include "flow/IConnection.h"
#include <unordered_map>

#include "flow/actorcompiler.h" // has to be last include

namespace HTTP {

// AWS V4 headers require this encoding for its signature calculation
std::string awsV4URIEncode(const std::string& s, bool encodeSlash) {
	std::string o;
	o.reserve(s.size() * 3);
	char buf[4];
	for (auto c : s) {
		if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
			o.append(&c, 1);
		else if (c == '/')
			o.append(encodeSlash ? "%2F" : "/");
		else {
			sprintf(buf, "%%%.02X", c);
			o.append(buf);
		}
	}
	return o;
}

std::string urlEncode(const std::string& s) {
	std::string o;
	o.reserve(s.size() * 3);
	char buf[4];
	for (auto c : s)
		if (std::isalnum(c) || c == '?' || c == '/' || c == '-' || c == '_' || c == '.' || c == ',' || c == ':')
			o.append(&c, 1);
		else {
			sprintf(buf, "%%%.02X", c);
			o.append(buf);
		}
	return o;
}

template <typename T>
std::string ResponseBase<T>::getCodeDescription() {
	if (code == HTTP_STATUS_CODE_OK) {
		return "OK";
	} else if (code == HTTP_STATUS_CODE_CREATED) {
		return "Created";
	} else if (code == HTTP_STATUS_CODE_ACCEPTED) {
		return "Accepted";
	} else if (code == HTTP_STATUS_CODE_NO_CONTENT) {
		return "No Content";
	} else if (code == HTTP_STATUS_CODE_UNAUTHORIZED) {
		return "Unauthorized";
	} else if (code == HTTP_STATUS_CODE_NOT_ACCEPTABLE) {
		return "Not Acceptable";
	} else if (code == HTTP_STATUS_CODE_TIMEOUT) {
		return "Timeout";
	} else if (code == HTTP_STATUS_CODE_TOO_MANY_REQUESTS) {
		return "Too Many Requests";
	} else if (code == HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR) {
		return "Internal Server Error";
	} else if (code == HTTP_STATUS_CODE_BAD_GATEWAY) {
		return "Bad Gateway";
	} else if (code == HTTP_STATUS_CODE_SERVICE_UNAVAILABLE) {
		return "Service Unavailable";
	} else if (code == HTTP_STATUS_CODE_GATEWAY_TIMEOUT) {
		return "Gateway Timeout";
	} else {
		throw internal_error();
	}
}

std::string computeMD5Sum(std::string content) {
	MD5_CTX sum;
	::MD5_Init(&sum);
	::MD5_Update(&sum, content.data(), content.size());
	std::string sumBytes;
	sumBytes.resize(16);
	::MD5_Final((unsigned char*)sumBytes.data(), &sum);
	std::string sumStr = base64::encoder::from_string(sumBytes);
	sumStr.resize(sumStr.size() - 1);
	return sumStr;
}

bool verifyMD5(HTTPData<std::string>* data, bool fail_if_header_missing, Optional<std::string> content_sum) {
	auto i = data->headers.find("Content-MD5");
	if (i != data->headers.end()) {
		// If a content sum is not provided, calculate one from the response content
		if (!content_sum.present()) {
			content_sum = computeMD5Sum(data->content);
		}
		return i->second == content_sum.get();
	}
	return !fail_if_header_missing;
}

std::string IncomingResponse::toString() const {
	std::string r = fmt::format("Response Code: {0}\n", code);
	r += fmt::format("Response ContentLen: {0}\n", data.contentLen);
	for (auto h : data.headers)
		r += fmt::format("Reponse Header: {0}: {1}\n", h.first, h.second);
	r.append("-- RESPONSE CONTENT--\n");
	r.append(data.content);
	r.append("\n--------\n");
	return r;
}

void writeHeaders(HTTP::Headers const& headers, PacketWriter& writer) {
	for (auto h : headers) {
		writer.serializeBytes(h.first);
		writer.serializeBytes(": "_sr);
		writer.serializeBytes(h.second);
		writer.serializeBytes("\r\n"_sr);
	}
	writer.serializeBytes("\r\n"_sr);
}

PacketBuffer* writeRequestHeader(Reference<OutgoingRequest> req, PacketBuffer* dest) {
	PacketWriter writer(dest, nullptr, Unversioned());
	writer.serializeBytes(req->verb);
	writer.serializeBytes(" ", 1);
	writer.serializeBytes(req->resource);
	writer.serializeBytes(" HTTP/1.1\r\n"_sr);

	writeHeaders(req->data.headers, writer);

	return writer.finish();
}

PacketBuffer* writeResponseHeader(Reference<OutgoingResponse> response, PacketBuffer* dest) {
	PacketWriter writer(dest, nullptr, Unversioned());
	writer.serializeBytes("HTTP/1.1 "_sr);
	writer.serializeBytes(std::to_string(response->code));
	writer.serializeBytes(" ", 1);
	writer.serializeBytes(response->getCodeDescription());
	writer.serializeBytes("\r\n"_sr);

	writeHeaders(response->data.headers, writer);

	return writer.finish();
}

															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via writeResponse()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class WriteResponseActor>
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class WriteResponseActorState {
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	WriteResponseActorState(Reference<IConnection> const& conn,Reference<OutgoingResponse> const& response) 
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   response(response)
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("writeResponse", reinterpret_cast<unsigned long>(this));

	}
	~WriteResponseActorState() 
	{
		fdb_probe_actor_destroy("writeResponse", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			ASSERT(response.isValid());
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			response->data.headers["Content-Length"] = std::to_string(response->data.contentLen);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			PacketBuffer* pFirst = PacketBuffer::create();
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			PacketBuffer* pLast = writeResponseHeader(response, pFirst);
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			response->data.content->prependWriteBuffer(pFirst, pLast);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~WriteResponseActorState();
		static_cast<WriteResponseActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int trySend = FLOW_KNOBS->HTTP_SEND_SIZE;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if ((!g_network->isSimulated() || !g_simulator->speedUpSimulation) && BUGGIFY_WITH_PROB(0.01))
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			trySend = deterministicRandom()->randomInt(1, 10);
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(response->data.content->getUnsent(), trySend);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		response->data.content->sent(len);
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (response->data.content->empty())
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<WriteResponseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteResponseActorState(); static_cast<WriteResponseActor*>(this)->destroy(); return 0; }
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<WriteResponseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WriteResponseActorState();
			static_cast<WriteResponseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = conn->onWritable();
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<WriteResponseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WriteResponseActor*>(this)->actor_wait_state = 1;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteResponseActor, 0, Void >*>(static_cast<WriteResponseActor*>(this)));
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = yield(TaskPriority::WriteSocket);
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<WriteResponseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteResponseActor*>(this)->actor_wait_state = 2;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteResponseActor, 1, Void >*>(static_cast<WriteResponseActor*>(this)));
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = yield(TaskPriority::WriteSocket);
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<WriteResponseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteResponseActor*>(this)->actor_wait_state = 2;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteResponseActor, 1, Void >*>(static_cast<WriteResponseActor*>(this)));
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<WriteResponseActor*>(this)->actor_wait_state > 0) static_cast<WriteResponseActor*>(this)->actor_wait_state = 0;
		static_cast<WriteResponseActor*>(this)->ActorCallback< WriteResponseActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteResponseActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteResponseActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteResponseActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

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
		if (static_cast<WriteResponseActor*>(this)->actor_wait_state > 0) static_cast<WriteResponseActor*>(this)->actor_wait_state = 0;
		static_cast<WriteResponseActor*>(this)->ActorCallback< WriteResponseActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteResponseActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteResponseActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteResponseActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<OutgoingResponse> response;
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via writeResponse()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class WriteResponseActor final : public Actor<Void>, public ActorCallback< WriteResponseActor, 0, Void >, public ActorCallback< WriteResponseActor, 1, Void >, public FastAllocated<WriteResponseActor>, public WriteResponseActorState<WriteResponseActor> {
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<WriteResponseActor>::operator new;
	using FastAllocated<WriteResponseActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteResponseActor, 0, Void >;
friend struct ActorCallback< WriteResponseActor, 1, Void >;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	WriteResponseActor(Reference<IConnection> const& conn,Reference<OutgoingResponse> const& response) 
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   WriteResponseActorState<WriteResponseActor>(conn, response)
	{
		fdb_probe_actor_enter("writeResponse", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("writeResponse");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("writeResponse", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteResponseActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteResponseActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> writeResponse( Reference<IConnection> const& conn, Reference<OutgoingResponse> const& response ) {
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new WriteResponseActor(conn, response));
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Read at least 1 bytes from conn and up to maxlen in a single read, append read data into *buf
// Returns the number of bytes read.
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_into_string()
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_into_stringActor>
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_into_stringActorState {
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_into_stringActorState(Reference<IConnection> const& conn,std::string* const& buf,int const& maxlen) 
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   maxlen(maxlen)
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("read_into_string", reinterpret_cast<unsigned long>(this));

	}
	~Read_into_stringActorState() 
	{
		fdb_probe_actor_destroy("read_into_string", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~Read_into_stringActorState();
		static_cast<Read_into_stringActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int originalSize = buf->size();
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		buf->resize(originalSize + maxlen);
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		uint8_t* wptr = (uint8_t*)buf->data() + originalSize;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->read(wptr, wptr + maxlen);
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		buf->resize(originalSize + len);
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (len > 0)
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_into_stringActor*>(this)->SAV<int>::futures) { (void)(len); this->~Read_into_stringActorState(); static_cast<Read_into_stringActor*>(this)->destroy(); return 0; }
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_into_stringActor*>(this)->SAV< int >::value()) int(len);
			this->~Read_into_stringActorState();
			static_cast<Read_into_stringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = conn->onReadable();
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 1;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 0, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 2;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 1, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 2;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 1, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state > 0) static_cast<Read_into_stringActor*>(this)->actor_wait_state = 0;
		static_cast<Read_into_stringActor*>(this)->ActorCallback< Read_into_stringActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_into_stringActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_into_stringActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_into_stringActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state > 0) static_cast<Read_into_stringActor*>(this)->actor_wait_state = 0;
		static_cast<Read_into_stringActor*>(this)->ActorCallback< Read_into_stringActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_into_stringActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Read_into_stringActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Read_into_stringActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int maxlen;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_into_string()
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_into_stringActor final : public Actor<int>, public ActorCallback< Read_into_stringActor, 0, Void >, public ActorCallback< Read_into_stringActor, 1, Void >, public FastAllocated<Read_into_stringActor>, public Read_into_stringActorState<Read_into_stringActor> {
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_into_stringActor>::operator new;
	using FastAllocated<Read_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_into_stringActor, 0, Void >;
friend struct ActorCallback< Read_into_stringActor, 1, Void >;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_into_stringActor(Reference<IConnection> const& conn,std::string* const& buf,int const& maxlen) 
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<int>(),
		   Read_into_stringActorState<Read_into_stringActor>(conn, buf, maxlen)
	{
		fdb_probe_actor_enter("read_into_string", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_into_string");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_into_string", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_into_stringActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Read_into_stringActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<int> read_into_string( Reference<IConnection> const& conn, std::string* const& buf, int const& maxlen ) {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<int>(new Read_into_stringActor(conn, buf, maxlen));
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Returns the position of delim within buf, relative to pos.  If delim is not found, continues to read from conn until
// either it is found or the connection ends, at which point connection_failed is thrown and buf contains
// everything that was read up to that point.
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_delimited_into_string()
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_delimited_into_stringActor>
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_delimited_into_stringActorState {
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_delimited_into_stringActorState(Reference<IConnection> const& conn,const char* const& delim,std::string* const& buf,size_t const& pos) 
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   delim(delim),
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   sPos(pos),
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   lookBack(strlen(delim) - 1)
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("read_delimited_into_string", reinterpret_cast<unsigned long>(this));

	}
	~Read_delimited_into_stringActorState() 
	{
		fdb_probe_actor_destroy("read_delimited_into_string", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			ASSERT(lookBack >= 0);
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~Read_delimited_into_stringActorState();
		static_cast<Read_delimited_into_stringActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		size_t endPos = buf->find(delim, sPos);
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (endPos != std::string::npos)
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_delimited_into_stringActor*>(this)->SAV<size_t>::futures) { (void)(endPos - pos); this->~Read_delimited_into_stringActorState(); static_cast<Read_delimited_into_stringActor*>(this)->destroy(); return 0; }
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_delimited_into_stringActor*>(this)->SAV< size_t >::value()) size_t(endPos - pos);
			this->~Read_delimited_into_stringActorState();
			static_cast<Read_delimited_into_stringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (buf->size() >= lookBack)
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			sPos = buf->size() - lookBack;
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = success(read_into_string(conn, buf, FLOW_KNOBS->HTTP_READ_SIZE));
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state = 1;
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_delimited_into_stringActor, 0, Void >*>(static_cast<Read_delimited_into_stringActor*>(this)));
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		if (static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state > 0) static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state = 0;
		static_cast<Read_delimited_into_stringActor*>(this)->ActorCallback< Read_delimited_into_stringActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_delimited_into_stringActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_delimited_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_delimited_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_delimited_into_stringActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_delimited_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_delimited_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_delimited_into_stringActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_delimited_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_delimited_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	const char* delim;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t sPos;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int lookBack;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_delimited_into_string()
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_delimited_into_stringActor final : public Actor<size_t>, public ActorCallback< Read_delimited_into_stringActor, 0, Void >, public FastAllocated<Read_delimited_into_stringActor>, public Read_delimited_into_stringActorState<Read_delimited_into_stringActor> {
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_delimited_into_stringActor>::operator new;
	using FastAllocated<Read_delimited_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<size_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_delimited_into_stringActor, 0, Void >;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_delimited_into_stringActor(Reference<IConnection> const& conn,const char* const& delim,std::string* const& buf,size_t const& pos) 
															#line 1038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<size_t>(),
		   Read_delimited_into_stringActorState<Read_delimited_into_stringActor>(conn, delim, buf, pos)
	{
		fdb_probe_actor_enter("read_delimited_into_string", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_delimited_into_string");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_delimited_into_string", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_delimited_into_stringActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<size_t> read_delimited_into_string( Reference<IConnection> const& conn, const char* const& delim, std::string* const& buf, size_t const& pos ) {
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<size_t>(new Read_delimited_into_stringActor(conn, delim, buf, pos));
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Reads from conn (as needed) until there are at least len bytes starting at pos in buf
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_fixed_into_string()
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_fixed_into_stringActor>
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_fixed_into_stringActorState {
															#line 1077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_fixed_into_stringActorState(Reference<IConnection> const& conn,int const& len,std::string* const& buf,size_t const& pos) 
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   len(len),
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   stop_size(pos + len)
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("read_fixed_into_string", reinterpret_cast<unsigned long>(this));

	}
	~Read_fixed_into_stringActorState() 
	{
		fdb_probe_actor_destroy("read_fixed_into_string", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~Read_fixed_into_stringActorState();
		static_cast<Read_fixed_into_stringActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_fixed_into_stringActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_fixed_into_stringActorState(); static_cast<Read_fixed_into_stringActor*>(this)->destroy(); return 0; }
															#line 1130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_fixed_into_stringActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_fixed_into_stringActorState();
		static_cast<Read_fixed_into_stringActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!(buf->size() < stop_size))
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = success(read_into_string(conn, buf, FLOW_KNOBS->HTTP_READ_SIZE));
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state = 1;
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_fixed_into_stringActor, 0, Void >*>(static_cast<Read_fixed_into_stringActor*>(this)));
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		if (static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state > 0) static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state = 0;
		static_cast<Read_fixed_into_stringActor*>(this)->ActorCallback< Read_fixed_into_stringActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_fixed_into_stringActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_fixed_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_fixed_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_fixed_into_stringActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_fixed_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_fixed_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_fixed_into_stringActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_fixed_into_string", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_fixed_into_string", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int len;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int stop_size;
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_fixed_into_string()
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_fixed_into_stringActor final : public Actor<Void>, public ActorCallback< Read_fixed_into_stringActor, 0, Void >, public FastAllocated<Read_fixed_into_stringActor>, public Read_fixed_into_stringActorState<Read_fixed_into_stringActor> {
															#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_fixed_into_stringActor>::operator new;
	using FastAllocated<Read_fixed_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_fixed_into_stringActor, 0, Void >;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_fixed_into_stringActor(Reference<IConnection> const& conn,int const& len,std::string* const& buf,size_t const& pos) 
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   Read_fixed_into_stringActorState<Read_fixed_into_stringActor>(conn, len, buf, pos)
	{
		fdb_probe_actor_enter("read_fixed_into_string", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_fixed_into_string");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_fixed_into_string", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_fixed_into_stringActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_fixed_into_string( Reference<IConnection> const& conn, int const& len, std::string* const& buf, size_t const& pos ) {
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_fixed_into_stringActor(conn, len, buf, pos));
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_response_headers()
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_http_response_headersActor>
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_response_headersActorState {
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_response_headersActorState(Reference<IConnection> const& conn,Headers* const& headers,std::string* const& buf,size_t* const& pos) 
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   headers(headers),
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos)
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("read_http_response_headers", reinterpret_cast<unsigned long>(this));

	}
	~Read_http_response_headersActorState() 
	{
		fdb_probe_actor_destroy("read_http_response_headers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~Read_http_response_headersActorState();
		static_cast<Read_http_response_headersActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", buf, *pos);
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_response_headersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_http_response_headersActor*>(this)->actor_wait_state = 1;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_response_headersActor, 0, size_t >*>(static_cast<Read_http_response_headersActor*>(this)));
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(size_t const& lineLen,int loopDepth) 
	{
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen == 0)
															#line 1393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += 2;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_response_headersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_response_headersActorState(); static_cast<Read_http_response_headersActor*>(this)->destroy(); return 0; }
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_response_headersActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_response_headersActorState();
			static_cast<Read_http_response_headersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int nameEnd = -1, valueStart = -1, valueEnd = -1;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = -1;
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string name, value;
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^:]%n:%*[ \t]%n", &nameEnd, &valueStart) >= 0 && valueStart > 0)
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			name = std::string(buf->substr(*pos, nameEnd));
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += valueStart;
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^\r]%n%*1[\r]%*1[\n]%n", &valueEnd, &len) >= 0 && len > 0)
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			value = std::string(buf->substr(*pos, valueEnd));
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += len;
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (sscanf(buf->c_str() + *pos, "%*1[\r]%*1[\n]%n", &len) >= 0 && len > 0)
															#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				*pos += len;
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
			else
			{
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		(*headers)[name] = value;
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen == 0)
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += 2;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_response_headersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_response_headersActorState(); static_cast<Read_http_response_headersActor*>(this)->destroy(); return 0; }
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_response_headersActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_response_headersActorState();
			static_cast<Read_http_response_headersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int nameEnd = -1, valueStart = -1, valueEnd = -1;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = -1;
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string name, value;
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^:]%n:%*[ \t]%n", &nameEnd, &valueStart) >= 0 && valueStart > 0)
															#line 1485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			name = std::string(buf->substr(*pos, nameEnd));
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += valueStart;
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^\r]%n%*1[\r]%*1[\n]%n", &valueEnd, &len) >= 0 && len > 0)
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			value = std::string(buf->substr(*pos, valueEnd));
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += len;
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (sscanf(buf->c_str() + *pos, "%*1[\r]%*1[\n]%n", &len) >= 0 && len > 0)
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				*pos += len;
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
			else
			{
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		(*headers)[name] = value;
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Read_http_response_headersActor*>(this)->actor_wait_state > 0) static_cast<Read_http_response_headersActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_response_headersActor*>(this)->ActorCallback< Read_http_response_headersActor, 0, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_response_headersActor, 0, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("read_http_response_headers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response_headers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_http_response_headersActor, 0, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("read_http_response_headers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response_headers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_http_response_headersActor, 0, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response_headers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response_headers", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Headers* headers;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t* pos;
															#line 1604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_response_headers()
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_response_headersActor final : public Actor<Void>, public ActorCallback< Read_http_response_headersActor, 0, size_t >, public FastAllocated<Read_http_response_headersActor>, public Read_http_response_headersActorState<Read_http_response_headersActor> {
															#line 1609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_http_response_headersActor>::operator new;
	using FastAllocated<Read_http_response_headersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_http_response_headersActor, 0, size_t >;
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_response_headersActor(Reference<IConnection> const& conn,Headers* const& headers,std::string* const& buf,size_t* const& pos) 
															#line 1620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   Read_http_response_headersActorState<Read_http_response_headersActor>(conn, headers, buf, pos)
	{
		fdb_probe_actor_enter("read_http_response_headers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_http_response_headers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_http_response_headers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_http_response_headersActor, 0, size_t >*)0, actor_cancelled()); break;
		}

	}
};
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_response_headers( Reference<IConnection> const& conn, Headers* const& headers, std::string* const& buf, size_t* const& pos ) {
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_response_headersActor(conn, headers, buf, pos));
															#line 1647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// FIXME: should this throw a different error for http requests? Or should we rename http_bad_response to
// http_bad_<something>?
															#line 1654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via readHTTPData()
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class ReadHTTPDataActor>
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class ReadHTTPDataActorState {
															#line 1660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	ReadHTTPDataActorState(HTTPData<std::string>* const& r,Reference<IConnection> const& conn,std::string* const& buf,size_t* const& pos,bool const& content_optional,bool const& skipCheckMD5) 
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : r(r),
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   conn(conn),
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   content_optional(content_optional),
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   skipCheckMD5(skipCheckMD5)
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("readHTTPData", reinterpret_cast<unsigned long>(this));

	}
	~ReadHTTPDataActorState() 
	{
		fdb_probe_actor_destroy("readHTTPData", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->headers.clear();
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_0 = read_http_response_headers(conn, &r->headers, buf, pos);
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 1;
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 0, Void >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 1701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~ReadHTTPDataActorState();
		static_cast<ReadHTTPDataActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		auto i = r->headers.find("Content-Length");
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = strtoll(i->second.c_str(), NULL, 10);
															#line 1730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = -1;
															#line 1736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		transferEncoding = std::string();
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		i = r->headers.find("Transfer-Encoding");
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			transferEncoding = i->second;
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.clear();
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (content_optional && *pos == buf->size())
															#line 1754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<ReadHTTPDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadHTTPDataActorState(); static_cast<ReadHTTPDataActor*>(this)->destroy(); return 0; }
															#line 1758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<ReadHTTPDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadHTTPDataActorState();
			static_cast<ReadHTTPDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->contentLen >= 0)
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->content = buf->substr(*pos);
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos = 0;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_1 = read_fixed_into_string(conn, r->contentLen, &r->content, *pos);
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 2;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 1, Void >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (transferEncoding == "chunked")
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				r->content = buf->substr(*pos);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				*pos = 0;
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				;
															#line 1796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				loopDepth = a_body1cont1loopHead1(loopDepth);
			}
			else
			{
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		auto i = r->headers.find("Content-Length");
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = strtoll(i->second.c_str(), NULL, 10);
															#line 1819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = -1;
															#line 1825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		transferEncoding = std::string();
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		i = r->headers.find("Transfer-Encoding");
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			transferEncoding = i->second;
															#line 1837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.clear();
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (content_optional && *pos == buf->size())
															#line 1843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<ReadHTTPDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadHTTPDataActorState(); static_cast<ReadHTTPDataActor*>(this)->destroy(); return 0; }
															#line 1847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<ReadHTTPDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadHTTPDataActorState();
			static_cast<ReadHTTPDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->contentLen >= 0)
															#line 1855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->content = buf->substr(*pos);
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos = 0;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_1 = read_fixed_into_string(conn, r->contentLen, &r->content, *pos);
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 2;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 1, Void >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (transferEncoding == "chunked")
															#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				r->content = buf->substr(*pos);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				*pos = 0;
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				;
															#line 1885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				loopDepth = a_body1cont1loopHead1(loopDepth);
			}
			else
			{
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
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
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state > 0) static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 0;
		static_cast<ReadHTTPDataActor*>(this)->ActorCallback< ReadHTTPDataActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadHTTPDataActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() > 0)
															#line 1965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (skipCheckMD5)
															#line 1969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (!static_cast<ReadHTTPDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadHTTPDataActorState(); static_cast<ReadHTTPDataActor*>(this)->destroy(); return 0; }
															#line 1973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				new (&static_cast<ReadHTTPDataActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~ReadHTTPDataActorState();
				static_cast<ReadHTTPDataActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!HTTP::verifyMD5(r, false))
															#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ReadHTTPDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadHTTPDataActorState(); static_cast<ReadHTTPDataActor*>(this)->destroy(); return 0; }
															#line 1990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<ReadHTTPDataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadHTTPDataActorState();
		static_cast<ReadHTTPDataActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() != r->contentLen)
															#line 2002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() != r->contentLen)
															#line 2016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state > 0) static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 0;
		static_cast<ReadHTTPDataActor*>(this)->ActorCallback< ReadHTTPDataActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadHTTPDataActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont10(int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont11(int loopDepth) 
	{
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->contentLen = *pos;
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_5 = read_http_response_headers(conn, &r->headers, &r->content, pos);
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont11when1(__when_expr_5.get(), loopDepth); };
		static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 6;
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 5, Void >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 2108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
		{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_2 = read_delimited_into_string(conn, "\r\n", &r->content, *pos);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 3;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 2, size_t >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 2132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont11(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
		{
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_4 = read_delimited_into_string(conn, "\r\n", &r->content, *pos);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
			static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 5;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 4, size_t >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 2163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(size_t const& lineLen,int loopDepth) 
	{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		chunkLen = strtol(r->content.substr(*pos, lineLen).c_str(), nullptr, 16);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(*pos, lineLen + 2);
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (chunkLen == 0)
															#line 2177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_3 = read_fixed_into_string(conn, chunkLen, &r->content, *pos);
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 4;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 3, Void >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 2190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(size_t && lineLen,int loopDepth) 
	{
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		chunkLen = strtol(r->content.substr(*pos, lineLen).c_str(), nullptr, 16);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(*pos, lineLen + 2);
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (chunkLen == 0)
															#line 2203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_3 = read_fixed_into_string(conn, chunkLen, &r->content, *pos);
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 4;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadHTTPDataActor, 3, Void >*>(static_cast<ReadHTTPDataActor*>(this)));
															#line 2216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state > 0) static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 0;
		static_cast<ReadHTTPDataActor*>(this)->ActorCallback< ReadHTTPDataActor, 2, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 2, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 2, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadHTTPDataActor, 2, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		*pos += chunkLen;
															#line 2288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		*pos += chunkLen;
															#line 2297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state > 0) static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 0;
		static_cast<ReadHTTPDataActor*>(this)->ActorCallback< ReadHTTPDataActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ReadHTTPDataActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont1loopBody1cont6(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont7(size_t const& lineLen,int loopDepth) 
	{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen != 0)
															#line 2375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 2379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(*pos, 2);
															#line 2383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont7(size_t && lineLen,int loopDepth) 
	{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen != 0)
															#line 2392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 2396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(*pos, 2);
															#line 2400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont7(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont7(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state > 0) static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 0;
		static_cast<ReadHTTPDataActor*>(this)->ActorCallback< ReadHTTPDataActor, 4, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 4, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 4, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ReadHTTPDataActor, 4, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont11cont1(Void const& _,int loopDepth) 
	{
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (*pos != r->content.size())
															#line 2472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(r->contentLen);
															#line 2480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont11cont1(Void && _,int loopDepth) 
	{
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (*pos != r->content.size())
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(r->contentLen);
															#line 2497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont11when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont11cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont11when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont11cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ReadHTTPDataActor*>(this)->actor_wait_state > 0) static_cast<ReadHTTPDataActor*>(this)->actor_wait_state = 0;
		static_cast<ReadHTTPDataActor*>(this)->ActorCallback< ReadHTTPDataActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont11when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ReadHTTPDataActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont11when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ReadHTTPDataActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	HTTPData<std::string>* r;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t* pos;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	bool content_optional;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	bool skipCheckMD5;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string transferEncoding;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int chunkLen;
															#line 2581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via readHTTPData()
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class ReadHTTPDataActor final : public Actor<Void>, public ActorCallback< ReadHTTPDataActor, 0, Void >, public ActorCallback< ReadHTTPDataActor, 1, Void >, public ActorCallback< ReadHTTPDataActor, 2, size_t >, public ActorCallback< ReadHTTPDataActor, 3, Void >, public ActorCallback< ReadHTTPDataActor, 4, size_t >, public ActorCallback< ReadHTTPDataActor, 5, Void >, public FastAllocated<ReadHTTPDataActor>, public ReadHTTPDataActorState<ReadHTTPDataActor> {
															#line 2586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<ReadHTTPDataActor>::operator new;
	using FastAllocated<ReadHTTPDataActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadHTTPDataActor, 0, Void >;
friend struct ActorCallback< ReadHTTPDataActor, 1, Void >;
friend struct ActorCallback< ReadHTTPDataActor, 2, size_t >;
friend struct ActorCallback< ReadHTTPDataActor, 3, Void >;
friend struct ActorCallback< ReadHTTPDataActor, 4, size_t >;
friend struct ActorCallback< ReadHTTPDataActor, 5, Void >;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	ReadHTTPDataActor(HTTPData<std::string>* const& r,Reference<IConnection> const& conn,std::string* const& buf,size_t* const& pos,bool const& content_optional,bool const& skipCheckMD5) 
															#line 2602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   ReadHTTPDataActorState<ReadHTTPDataActor>(r, conn, buf, pos, content_optional, skipCheckMD5)
	{
		fdb_probe_actor_enter("readHTTPData", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readHTTPData");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readHTTPData", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadHTTPDataActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadHTTPDataActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadHTTPDataActor, 2, size_t >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ReadHTTPDataActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ReadHTTPDataActor, 4, size_t >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ReadHTTPDataActor, 5, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> readHTTPData( HTTPData<std::string>* const& r, Reference<IConnection> const& conn, std::string* const& buf, size_t* const& pos, bool const& content_optional, bool const& skipCheckMD5 ) {
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new ReadHTTPDataActor(r, conn, buf, pos, content_optional, skipCheckMD5));
															#line 2634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Reads an HTTP request from a network connection
// If the connection fails while being read the exception will emitted
// If the response is not parsable or complete in some way, http_bad_response will be thrown
															#line 2642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_request()
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_http_requestActor>
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_requestActorState {
															#line 2648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_requestActorState(Reference<HTTP::IncomingRequest> const& r,Reference<IConnection> const& conn) 
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : r(r),
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   conn(conn),
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(),
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(0)
															#line 2661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("read_http_request", reinterpret_cast<unsigned long>(this));

	}
	~Read_http_requestActorState() 
	{
		fdb_probe_actor_destroy("read_http_request", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", &buf, pos);
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_requestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Read_http_requestActor*>(this)->actor_wait_state = 1;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_requestActor, 0, size_t >*>(static_cast<Read_http_requestActor*>(this)));
															#line 2683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~Read_http_requestActorState();
		static_cast<Read_http_requestActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(size_t const& lineLen,int loopDepth) 
	{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string requestLine = buf.substr(0, lineLen);
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::stringstream ss(requestLine);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		ss >> r->verb;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss.fail())
															#line 2712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		ss >> r->resource;
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss.fail())
															#line 2722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string httpVersion;
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		ss >> httpVersion;
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss.fail())
															#line 2734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss && !ss.eof())
															#line 2742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		float version;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sscanf(httpVersion.c_str(), "HTTP/%f", &version);
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (version < 1.1)
															#line 2754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = readHTTPData(&r->data, conn, &buf, &pos, false, false);
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_requestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_requestActor*>(this)->actor_wait_state = 2;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_requestActor, 1, Void >*>(static_cast<Read_http_requestActor*>(this)));
															#line 2771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string requestLine = buf.substr(0, lineLen);
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::stringstream ss(requestLine);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		ss >> r->verb;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss.fail())
															#line 2786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		ss >> r->resource;
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss.fail())
															#line 2796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string httpVersion;
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		ss >> httpVersion;
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss.fail())
															#line 2808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (ss && !ss.eof())
															#line 2816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		float version;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sscanf(httpVersion.c_str(), "HTTP/%f", &version);
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (version < 1.1)
															#line 2828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = readHTTPData(&r->data, conn, &buf, &pos, false, false);
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_requestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_requestActor*>(this)->actor_wait_state = 2;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_requestActor, 1, Void >*>(static_cast<Read_http_requestActor*>(this)));
															#line 2845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Read_http_requestActor*>(this)->actor_wait_state > 0) static_cast<Read_http_requestActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_requestActor*>(this)->ActorCallback< Read_http_requestActor, 0, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_requestActor, 0, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_http_requestActor, 0, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_http_requestActor, 0, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_http_requestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_requestActorState(); static_cast<Read_http_requestActor*>(this)->destroy(); return 0; }
															#line 2917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_http_requestActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_http_requestActorState();
		static_cast<Read_http_requestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_http_requestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_requestActorState(); static_cast<Read_http_requestActor*>(this)->destroy(); return 0; }
															#line 2929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_http_requestActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_http_requestActorState();
		static_cast<Read_http_requestActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Read_http_requestActor*>(this)->actor_wait_state > 0) static_cast<Read_http_requestActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_requestActor*>(this)->ActorCallback< Read_http_requestActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_requestActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Read_http_requestActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Read_http_requestActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::IncomingRequest> r;
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string buf;
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 3008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_request()
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_requestActor final : public Actor<Void>, public ActorCallback< Read_http_requestActor, 0, size_t >, public ActorCallback< Read_http_requestActor, 1, Void >, public FastAllocated<Read_http_requestActor>, public Read_http_requestActorState<Read_http_requestActor> {
															#line 3013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_http_requestActor>::operator new;
	using FastAllocated<Read_http_requestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_http_requestActor, 0, size_t >;
friend struct ActorCallback< Read_http_requestActor, 1, Void >;
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_requestActor(Reference<HTTP::IncomingRequest> const& r,Reference<IConnection> const& conn) 
															#line 3025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   Read_http_requestActorState<Read_http_requestActor>(r, conn)
	{
		fdb_probe_actor_enter("read_http_request", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_http_request");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_http_request", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_http_requestActor, 0, size_t >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Read_http_requestActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_request( Reference<HTTP::IncomingRequest> const& r, Reference<IConnection> const& conn ) {
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_requestActor(r, conn));
															#line 3053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

Future<Void> HTTP::IncomingRequest::read(Reference<IConnection> conn, bool header_only) {
	return read_http_request(Reference<HTTP::IncomingRequest>::addRef(this), conn);
}

Future<Void> HTTP::OutgoingResponse::write(Reference<IConnection> conn) {
	return writeResponse(conn, Reference<HTTP::OutgoingResponse>::addRef(this));
}

void HTTP::OutgoingResponse::reset() {
	data.headers = HTTP::Headers();
	data.content->discardAll();
	data.contentLen = 0;
}

// Reads an HTTP response from a network connection
// If the connection fails while being read the exception will emitted
// If the response is not parsable or complete in some way, http_bad_response will be thrown
															#line 3075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_response()
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_http_responseActor>
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_responseActorState {
															#line 3081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_responseActorState(Reference<HTTP::IncomingResponse> const& r,Reference<IConnection> const& conn,bool const& header_only) 
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : r(r),
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   conn(conn),
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   header_only(header_only),
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(),
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(0)
															#line 3096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("read_http_response", reinterpret_cast<unsigned long>(this));

	}
	~Read_http_responseActorState() 
	{
		fdb_probe_actor_destroy("read_http_response", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", &buf, pos);
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 1;
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 0, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 3118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~Read_http_responseActorState();
		static_cast<Read_http_responseActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(size_t const& lineLen,int loopDepth) 
	{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int reachedEnd = -1;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sscanf(buf.c_str() + pos, "HTTP/%f %d%n", &r->version, &r->code, &reachedEnd);
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (reachedEnd < 0)
															#line 3145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 3149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		bool skipCheckMD5 = r->code == 206 && FLOW_KNOBS->HTTP_RESPONSE_SKIP_VERIFY_CHECKSUM_FOR_PARTIAL_CONTENT;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = readHTTPData(&r->data, conn, &buf, &pos, header_only, skipCheckMD5);
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 2;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 1, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 3164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int reachedEnd = -1;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sscanf(buf.c_str() + pos, "HTTP/%f %d%n", &r->version, &r->code, &reachedEnd);
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (reachedEnd < 0)
															#line 3177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 3181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		bool skipCheckMD5 = r->code == 206 && FLOW_KNOBS->HTTP_RESPONSE_SKIP_VERIFY_CHECKSUM_FOR_PARTIAL_CONTENT;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = readHTTPData(&r->data, conn, &buf, &pos, header_only, skipCheckMD5);
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 2;
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 1, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 3196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 0, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 0, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 0, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 0, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 3268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_http_responseActorState();
		static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 3280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_http_responseActorState();
		static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::IncomingResponse> r;
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	bool header_only;
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string buf;
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 3361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_response()
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_responseActor final : public Actor<Void>, public ActorCallback< Read_http_responseActor, 0, size_t >, public ActorCallback< Read_http_responseActor, 1, Void >, public FastAllocated<Read_http_responseActor>, public Read_http_responseActorState<Read_http_responseActor> {
															#line 3366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_http_responseActor>::operator new;
	using FastAllocated<Read_http_responseActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_http_responseActor, 0, size_t >;
friend struct ActorCallback< Read_http_responseActor, 1, Void >;
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_responseActor(Reference<HTTP::IncomingResponse> const& r,Reference<IConnection> const& conn,bool const& header_only) 
															#line 3378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   Read_http_responseActorState<Read_http_responseActor>(r, conn, header_only)
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_http_response");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_http_responseActor, 0, size_t >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Read_http_responseActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_response( Reference<HTTP::IncomingResponse> const& r, Reference<IConnection> const& conn, bool const& header_only ) {
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_responseActor(r, conn, header_only));
															#line 3406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

Future<Void> HTTP::IncomingResponse::read(Reference<IConnection> conn, bool header_only) {
	return read_http_response(Reference<HTTP::IncomingResponse>::addRef(this), conn, header_only);
}

// Do a request, get a Response.
// Request content is provided as UnsentPacketQueue in req, which will be depleted as bytes are sent but the queue
// itself must live for the life of this actor and be destroyed by the caller
// TODO:  pSent is very hackish, do something better.
															#line 3419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via doRequestActor()
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class DoRequestActorActor>
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class DoRequestActorActorState {
															#line 3425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	DoRequestActorActorState(Reference<IConnection> const& conn,Reference<OutgoingRequest> const& request,Reference<IRateControl> const& sendRate,int64_t* const& pSent,Reference<IRateControl> const& recvRate) 
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   request(request),
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   sendRate(sendRate),
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pSent(pSent),
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   recvRate(recvRate),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   event(SevDebug, "HTTPRequest"),
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   requestIDHeader(FLOW_KNOBS->HTTP_REQUEST_ID_HEADER),
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   earlyResponse(false),
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   total_sent(0),
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   send_start()
															#line 3450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("doRequestActor", reinterpret_cast<unsigned long>(this));

	}
	~DoRequestActorActorState() 
	{
		fdb_probe_actor_destroy("doRequestActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("DebugID", conn->getDebugID());
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RemoteAddress", conn->getPeerAddress());
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("Verb", request->verb);
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("Resource", request->resource);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestContentLen", request->data.contentLen);
															#line 3473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			try {
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				requestID = std::string();
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (!requestIDHeader.empty())
															#line 3479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = deterministicRandom()->randomUniqueID().toString();
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(20, "-");
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(16, "-");
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(12, "-");
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(8, "-");
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					request->data.headers[requestIDHeader] = requestID;
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					event.detail("RequestIDSent", requestID);
															#line 3495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				}
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				request->data.headers["Content-Length"] = std::to_string(request->data.contentLen);
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				PacketBuffer* pFirst = PacketBuffer::create();
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				PacketBuffer* pLast = writeRequestHeader(request, pFirst);
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				request->data.content->prependWriteBuffer(pFirst, pLast);
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 1)
															#line 3507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					fmt::print("[{}] HTTP starting {} {} ContentLen:{}\n", conn->getDebugID().toString(), request->verb, request->resource, request->data.contentLen);
															#line 3511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				}
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 3515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					for( auto h : request->data.headers ) {
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
						fmt::print("Request Header: {}: {}\n", h.first, h.second);
															#line 3521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
					}
				}
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				r = Reference<HTTP::IncomingResponse>(new HTTP::IncomingResponse());
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				responseReading = r->read(conn, request->isHeaderOnlyResponse());
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				send_start = timer();
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				;
															#line 3532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~DoRequestActorActorState();
		static_cast<DoRequestActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			double elapsed = timer() - send_start;
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0 && e.code() != error_code_http_bad_request_id)
															#line 3564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				fmt::print("[{}] HTTP *ERROR*={} early={}, time={}s {} {} contentLen={} [{} out]\n", conn->getDebugID().toString(), e.name(), earlyResponse, elapsed, request->verb, request->resource, request->data.contentLen, total_sent);
															#line 3568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.errorUnsuppressed(e);
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 3574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_3 = responseReading;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<DoRequestActorActor*>(this)->actor_wait_state = 4;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoRequestActorActor, 3, Void >*>(static_cast<DoRequestActorActor*>(this)));
															#line 3595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (responseReading.isReady())
															#line 3611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			conn->close();
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->data.headers["Connection"] = "close";
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			earlyResponse = true;
															#line 3619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		trySend = FLOW_KNOBS->HTTP_SEND_SIZE;
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if ((!g_network->isSimulated() || !g_simulator->speedUpSimulation) && BUGGIFY_WITH_PROB(0.01))
															#line 3626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			trySend = deterministicRandom()->randomInt(1, 10);
															#line 3630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = sendRate->getAllowance(trySend);
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DoRequestActorActor*>(this)->actor_wait_state = 1;
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRequestActorActor, 0, Void >*>(static_cast<DoRequestActorActor*>(this)));
															#line 3641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(request->data.content->getUnsent(), trySend);
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pSent != nullptr)
															#line 3665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pSent += len;
															#line 3669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sendRate->returnUnused(trySend - len);
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		total_sent += len;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		request->data.content->sent(len);
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (request->data.content->empty())
															#line 3679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestActorActor*>(this)->actor_wait_state = 2;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestActorActor, 1, Void >*>(static_cast<DoRequestActorActor*>(this)));
															#line 3692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(request->data.content->getUnsent(), trySend);
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pSent != nullptr)
															#line 3703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pSent += len;
															#line 3707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sendRate->returnUnused(trySend - len);
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		total_sent += len;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		request->data.content->sent(len);
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (request->data.content->empty())
															#line 3717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestActorActor*>(this)->actor_wait_state = 2;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestActorActor, 1, Void >*>(static_cast<DoRequestActorActor*>(this)));
															#line 3730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActorActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActorActor*>(this)->ActorCallback< DoRequestActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoRequestActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestActorActor*>(this)->actor_wait_state = 3;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestActorActor, 2, Void >*>(static_cast<DoRequestActorActor*>(this)));
															#line 3809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestActorActor*>(this)->actor_wait_state = 3;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestActorActor, 2, Void >*>(static_cast<DoRequestActorActor*>(this)));
															#line 3825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActorActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActorActor*>(this)->ActorCallback< DoRequestActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoRequestActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActorActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActorActor*>(this)->ActorCallback< DoRequestActorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DoRequestActorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		double elapsed = timer() - send_start;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseCode", r->code);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseContentLen", r->data.contentLen);
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("Elapsed", elapsed);
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		Optional<Error> err;
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!requestIDHeader.empty())
															#line 3982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			std::string responseID;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			auto iid = r->data.headers.find(requestIDHeader);
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (iid != r->data.headers.end())
															#line 3990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				responseID = iid->second;
															#line 3994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestIDReceived", responseID);
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			bool serverError = r->code >= 500 && r->code < 600;
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (requestID != responseID && (!serverError || !responseID.empty()))
															#line 4002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				err = http_bad_request_id();
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				TraceEvent(SevError, "HTTPRequestFailedIDMismatch") .error(err.get()) .detail("DebugID", conn->getDebugID()) .detail("RemoteAddress", conn->getPeerAddress()) .detail("Verb", request->verb) .detail("Resource", request->resource) .detail("RequestContentLen", request->data.contentLen) .detail("ResponseCode", r->code) .detail("ResponseContentLen", r->data.contentLen) .detail("RequestIDSent", requestID) .detail("RequestIDReceived", responseID);
															#line 4008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0)
															#line 4013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{0}] HTTP {1}code={2} early={3}, time={4} {5} {6} contentLen={7} [{8} out, response content " "len {9}]\n", conn->getDebugID().toString(), (err.present() ? format("*ERROR*=%s ", err.get().name()).c_str() : ""), r->code, earlyResponse, elapsed, request->verb, request->resource, request->data.contentLen, total_sent, r->data.contentLen);
															#line 4017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 4021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{}] HTTP RESPONSE:  {} {}\n{}\n", conn->getDebugID().toString(), request->verb, request->resource, r->toString());
															#line 4025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 4029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch2(err.get(), loopDepth);
															#line 4033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<DoRequestActorActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(r); this->~DoRequestActorActorState(); static_cast<DoRequestActorActor*>(this)->destroy(); return 0; }
															#line 4037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<DoRequestActorActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(std::move(r)); // state_var_RVO
		this->~DoRequestActorActorState();
		static_cast<DoRequestActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		double elapsed = timer() - send_start;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseCode", r->code);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseContentLen", r->data.contentLen);
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("Elapsed", elapsed);
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		Optional<Error> err;
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!requestIDHeader.empty())
															#line 4059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			std::string responseID;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			auto iid = r->data.headers.find(requestIDHeader);
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (iid != r->data.headers.end())
															#line 4067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				responseID = iid->second;
															#line 4071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestIDReceived", responseID);
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			bool serverError = r->code >= 500 && r->code < 600;
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (requestID != responseID && (!serverError || !responseID.empty()))
															#line 4079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				err = http_bad_request_id();
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				TraceEvent(SevError, "HTTPRequestFailedIDMismatch") .error(err.get()) .detail("DebugID", conn->getDebugID()) .detail("RemoteAddress", conn->getPeerAddress()) .detail("Verb", request->verb) .detail("Resource", request->resource) .detail("RequestContentLen", request->data.contentLen) .detail("ResponseCode", r->code) .detail("ResponseContentLen", r->data.contentLen) .detail("RequestIDSent", requestID) .detail("RequestIDReceived", responseID);
															#line 4085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0)
															#line 4090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{0}] HTTP {1}code={2} early={3}, time={4} {5} {6} contentLen={7} [{8} out, response content " "len {9}]\n", conn->getDebugID().toString(), (err.present() ? format("*ERROR*=%s ", err.get().name()).c_str() : ""), r->code, earlyResponse, elapsed, request->verb, request->resource, request->data.contentLen, total_sent, r->data.contentLen);
															#line 4094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 4098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{}] HTTP RESPONSE:  {} {}\n{}\n", conn->getDebugID().toString(), request->verb, request->resource, r->toString());
															#line 4102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 4106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch2(err.get(), loopDepth);
															#line 4110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<DoRequestActorActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(r); this->~DoRequestActorActorState(); static_cast<DoRequestActorActor*>(this)->destroy(); return 0; }
															#line 4114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<DoRequestActorActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(std::move(r)); // state_var_RVO
		this->~DoRequestActorActorState();
		static_cast<DoRequestActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DoRequestActorActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActorActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActorActor*>(this)->ActorCallback< DoRequestActorActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DoRequestActorActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DoRequestActorActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<OutgoingRequest> request;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> sendRate;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int64_t* pSent;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> recvRate;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	TraceEvent event;
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string requestIDHeader;
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	bool earlyResponse;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int total_sent;
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	double send_start;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string requestID;
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::IncomingResponse> r;
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Future<Void> responseReading;
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int trySend;
															#line 4213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via doRequestActor()
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class DoRequestActorActor final : public Actor<Reference<HTTP::IncomingResponse>>, public ActorCallback< DoRequestActorActor, 0, Void >, public ActorCallback< DoRequestActorActor, 1, Void >, public ActorCallback< DoRequestActorActor, 2, Void >, public ActorCallback< DoRequestActorActor, 3, Void >, public FastAllocated<DoRequestActorActor>, public DoRequestActorActorState<DoRequestActorActor> {
															#line 4218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<DoRequestActorActor>::operator new;
	using FastAllocated<DoRequestActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::IncomingResponse>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoRequestActorActor, 0, Void >;
friend struct ActorCallback< DoRequestActorActor, 1, Void >;
friend struct ActorCallback< DoRequestActorActor, 2, Void >;
friend struct ActorCallback< DoRequestActorActor, 3, Void >;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	DoRequestActorActor(Reference<IConnection> const& conn,Reference<OutgoingRequest> const& request,Reference<IRateControl> const& sendRate,int64_t* const& pSent,Reference<IRateControl> const& recvRate) 
															#line 4232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Reference<HTTP::IncomingResponse>>(),
		   DoRequestActorActorState<DoRequestActorActor>(conn, request, sendRate, pSent, recvRate)
	{
		fdb_probe_actor_enter("doRequestActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doRequestActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doRequestActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoRequestActorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoRequestActorActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DoRequestActorActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DoRequestActorActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::IncomingResponse>> doRequestActor( Reference<IConnection> const& conn, Reference<OutgoingRequest> const& request, Reference<IRateControl> const& sendRate, int64_t* const& pSent, Reference<IRateControl> const& recvRate ) {
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Reference<HTTP::IncomingResponse>>(new DoRequestActorActor(conn, request, sendRate, pSent, recvRate));
															#line 4262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// IDE build didn't like the actor conversion i guess
Future<Reference<IncomingResponse>> doRequest(Reference<IConnection> conn,
                                              Reference<OutgoingRequest> request,
                                              Reference<IRateControl> sendRate,
                                              int64_t* pSent,
                                              Reference<IRateControl> recvRate) {
	return doRequestActor(conn, request, sendRate, pSent, recvRate);
}

															#line 4276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via sendProxyConnectRequest()
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class SendProxyConnectRequestActor>
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class SendProxyConnectRequestActorState {
															#line 4282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	SendProxyConnectRequestActorState(Reference<IConnection> const& conn,std::string const& remoteHost,std::string const& remoteService) 
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   remoteHost(remoteHost),
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   remoteService(remoteService),
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   requestTimeout(60),
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   maxTries(FLOW_KNOBS->RESTCLIENT_CONNECT_TRIES),
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   thisTry(1),
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   nextRetryDelay(2.0),
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   sendReceiveRate(makeReference<Unlimited>()),
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   bytes_sent(0),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   req(makeReference<HTTP::OutgoingRequest>())
															#line 4307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this));

	}
	~SendProxyConnectRequestActorState() 
	{
		fdb_probe_actor_destroy("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->verb = HTTP_VERB_CONNECT;
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->resource = remoteHost + ":" + remoteService;
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->data.content = nullptr;
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->data.contentLen = 0;
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->data.headers["Host"] = req->resource;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->data.headers["Accept"] = "application/xml";
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			req->data.headers["Proxy-Connection"] = "Keep-Alive";
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 4336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~SendProxyConnectRequestActorState();
		static_cast<SendProxyConnectRequestActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		err = Optional<Error>();
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r = Reference<HTTP::IncomingResponse>();
															#line 4368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		try {
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			Future<Reference<HTTP::IncomingResponse>> f = HTTP::doRequest(conn, req, sendReceiveRate, &bytes_sent, sendReceiveRate);
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Reference<HTTP::IncomingResponse>> __when_expr_0 = timeoutError(f, requestTimeout);
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 1;
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >*>(static_cast<SendProxyConnectRequestActor*>(this)));
															#line 4381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!err.present() && r->code == 200)
															#line 4396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<SendProxyConnectRequestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendProxyConnectRequestActorState(); static_cast<SendProxyConnectRequestActor*>(this)->destroy(); return 0; }
															#line 4400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<SendProxyConnectRequestActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~SendProxyConnectRequestActorState();
			static_cast<SendProxyConnectRequestActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		bool retryable = err.present() || r->code == 500 || r->code == 502 || r->code == 503 || r->code == 429;
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		retryable = retryable && (thisTry < maxTries);
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		TraceEvent event(SevWarn, retryable ? "ProxyConnectCommandFailedRetryable" : "ProxyConnectCommandFailed");
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 4414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.errorUnsuppressed(err.get());
															#line 4418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.suppressFor(60);
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!err.present())
															#line 4424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("ResponseCode", r->code);
															#line 4428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ThisTry", thisTry);
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!r || r->code != 429)
															#line 4434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			++thisTry;
															#line 4438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		double delay = nextRetryDelay;
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		nextRetryDelay = std::min(nextRetryDelay * 2, 60.0);
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (retryable)
															#line 4446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r)
															#line 4450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				auto iRetryAfter = r->data.headers.find("Retry-After");
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (iRetryAfter != r->data.headers.end())
															#line 4456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					event.detail("RetryAfterHeader", iRetryAfter->second);
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					char* pEnd;
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					double retryAfter = strtod(iRetryAfter->second.c_str(), &pEnd);
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					if (*pEnd)
															#line 4466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
					{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
						retryAfter = 300;
															#line 4470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
					}
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					delay = std::max(delay, retryAfter);
															#line 4474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				}
			}
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RetryDelay", delay);
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(delay);
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 2;
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SendProxyConnectRequestActor, 1, Void >*>(static_cast<SendProxyConnectRequestActor*>(this)));
															#line 4488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r && r->code == 406)
															#line 4495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_not_accepted(), std::max(0, loopDepth - 1));
															#line 4499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r && r->code == 401)
															#line 4503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_auth_failed(), std::max(0, loopDepth - 1));
															#line 4507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 1));
															#line 4511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 4521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 4525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			err = e;
															#line 4529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Reference<HTTP::IncomingResponse> const& _r,int loopDepth) 
	{
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r = _r;
															#line 4544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Reference<HTTP::IncomingResponse> && _r,int loopDepth) 
	{
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r = _r;
															#line 4553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<HTTP::IncomingResponse> const& _r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_r, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<HTTP::IncomingResponse> && _r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_r), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state > 0) static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 0;
		static_cast<SendProxyConnectRequestActor*>(this)->ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >::remove();

	}
	void a_callback_fire(ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> const& value) 
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> && value) 
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >*,Error err) 
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
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
	int a_body1loopBody1cont5(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state > 0) static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 0;
		static_cast<SendProxyConnectRequestActor*>(this)->ActorCallback< SendProxyConnectRequestActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendProxyConnectRequestActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SendProxyConnectRequestActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SendProxyConnectRequestActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteHost;
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteService;
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int requestTimeout;
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int maxTries;
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int thisTry;
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	double nextRetryDelay;
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int64_t bytes_sent;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::OutgoingRequest> req;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Optional<Error> err;
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::IncomingResponse> r;
															#line 4739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via sendProxyConnectRequest()
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class SendProxyConnectRequestActor final : public Actor<Void>, public ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >, public ActorCallback< SendProxyConnectRequestActor, 1, Void >, public FastAllocated<SendProxyConnectRequestActor>, public SendProxyConnectRequestActorState<SendProxyConnectRequestActor> {
															#line 4744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<SendProxyConnectRequestActor>::operator new;
	using FastAllocated<SendProxyConnectRequestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >;
friend struct ActorCallback< SendProxyConnectRequestActor, 1, Void >;
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	SendProxyConnectRequestActor(Reference<IConnection> const& conn,std::string const& remoteHost,std::string const& remoteService) 
															#line 4756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Void>(),
		   SendProxyConnectRequestActorState<SendProxyConnectRequestActor>(conn, remoteHost, remoteService)
	{
		fdb_probe_actor_enter("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendProxyConnectRequest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendProxyConnectRequest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendProxyConnectRequestActor, 0, Reference<HTTP::IncomingResponse> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SendProxyConnectRequestActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> sendProxyConnectRequest( Reference<IConnection> const& conn, std::string const& remoteHost, std::string const& remoteService ) {
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new SendProxyConnectRequestActor(conn, remoteHost, remoteService));
															#line 4784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

															#line 4789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via proxyConnectImpl()
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class ProxyConnectImplActor>
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class ProxyConnectImplActorState {
															#line 4795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	ProxyConnectImplActorState(std::string const& remoteHost,std::string const& remoteService,std::string const& proxyHost,std::string const& proxyService) 
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : remoteHost(remoteHost),
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   remoteService(remoteService),
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   proxyHost(proxyHost),
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   proxyService(proxyService)
															#line 4808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("proxyConnectImpl", reinterpret_cast<unsigned long>(this));

	}
	~ProxyConnectImplActorState() 
	{
		fdb_probe_actor_destroy("proxyConnectImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<NetworkAddress> __when_expr_0 = map(INetworkConnections::net()->resolveTCPEndpoint(remoteHost, remoteService), [=](std::vector<NetworkAddress> const& addresses) -> NetworkAddress { NetworkAddress addr = addresses[deterministicRandom()->randomInt(0, addresses.size())]; addr.fromHostname = true; addr.flags = NetworkAddress::FLAG_TLS; return addr; });
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 1;
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
		this->~ProxyConnectImplActorState();
		static_cast<ProxyConnectImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_1 = INetworkConnections::net()->connect(proxyHost, proxyService);
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 2;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(NetworkAddress const& __remoteEndpoint,int loopDepth) 
	{
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		remoteEndpoint = __remoteEndpoint;
															#line 4869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(NetworkAddress && __remoteEndpoint,int loopDepth) 
	{
		remoteEndpoint = std::move(__remoteEndpoint);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state > 0) static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 0;
		static_cast<ProxyConnectImplActor*>(this)->ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >::remove();

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*,NetworkAddress const& value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*,NetworkAddress && value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*,Error err) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = sendProxyConnectRequest(connection, remoteHost, remoteService);
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 3;
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 2, Void >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IConnection> const& __connection,int loopDepth) 
	{
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		connection = __connection;
															#line 4952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IConnection> && __connection,int loopDepth) 
	{
		connection = std::move(__connection);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state > 0) static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 0;
		static_cast<ProxyConnectImplActor*>(this)->ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		boost::asio::ip::tcp::socket socket = std::move(connection->getSocket());
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(remoteEndpoint, &socket);
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 4;
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 5028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		boost::asio::ip::tcp::socket socket = std::move(connection->getSocket());
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(remoteEndpoint, &socket);
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 4;
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 5046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state > 0) static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 0;
		static_cast<ProxyConnectImplActor*>(this)->ActorCallback< ProxyConnectImplActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ProxyConnectImplActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Reference<IConnection> const& remoteConnection,int loopDepth) 
	{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ProxyConnectImplActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(remoteConnection); this->~ProxyConnectImplActorState(); static_cast<ProxyConnectImplActor*>(this)->destroy(); return 0; }
															#line 5118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<ProxyConnectImplActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(remoteConnection);
		this->~ProxyConnectImplActorState();
		static_cast<ProxyConnectImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Reference<IConnection> && remoteConnection,int loopDepth) 
	{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ProxyConnectImplActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(remoteConnection); this->~ProxyConnectImplActorState(); static_cast<ProxyConnectImplActor*>(this)->destroy(); return 0; }
															#line 5130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<ProxyConnectImplActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(remoteConnection);
		this->~ProxyConnectImplActorState();
		static_cast<ProxyConnectImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Reference<IConnection> const& remoteConnection,int loopDepth) 
	{
		loopDepth = a_body1cont4(remoteConnection, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Reference<IConnection> && remoteConnection,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(remoteConnection), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state > 0) static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 0;
		static_cast<ProxyConnectImplActor*>(this)->ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >::remove();

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*,Reference<IConnection> const& value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*,Reference<IConnection> && value) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*,Error err) 
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteHost;
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteService;
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string proxyHost;
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string proxyService;
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	NetworkAddress remoteEndpoint;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> connection;
															#line 5213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via proxyConnectImpl()
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class ProxyConnectImplActor final : public Actor<Reference<IConnection>>, public ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >, public ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >, public ActorCallback< ProxyConnectImplActor, 2, Void >, public ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >, public FastAllocated<ProxyConnectImplActor>, public ProxyConnectImplActorState<ProxyConnectImplActor> {
															#line 5218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<ProxyConnectImplActor>::operator new;
	using FastAllocated<ProxyConnectImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IConnection>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >;
friend struct ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >;
friend struct ActorCallback< ProxyConnectImplActor, 2, Void >;
friend struct ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >;
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	ProxyConnectImplActor(std::string const& remoteHost,std::string const& remoteService,std::string const& proxyHost,std::string const& proxyService) 
															#line 5232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Reference<IConnection>>(),
		   ProxyConnectImplActorState<ProxyConnectImplActor>(remoteHost, remoteService, proxyHost, proxyService)
	{
		fdb_probe_actor_enter("proxyConnectImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("proxyConnectImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("proxyConnectImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ProxyConnectImplActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Reference<IConnection>> proxyConnectImpl( std::string const& remoteHost, std::string const& remoteService, std::string const& proxyHost, std::string const& proxyService ) {
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Reference<IConnection>>(new ProxyConnectImplActor(remoteHost, remoteService, proxyHost, proxyService));
															#line 5262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

Future<Reference<IConnection>> proxyConnect(const std::string& remoteHost,
                                            const std::string& remoteService,
                                            const std::string& proxyHost,
                                            const std::string& proxyService) {
	return proxyConnectImpl(remoteHost, remoteService, proxyHost, proxyService);
}

} // namespace HTTP
