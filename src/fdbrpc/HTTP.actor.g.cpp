#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
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

#include <cctype>

#include "fmt/format.h"
#include "fdbclient/Knobs.h"
#include "openssl/md5.h"
#include "fdbrpc/libb64/encode.h"
#include "flow/Knobs.h"

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

bool Response::verifyMD5(bool fail_if_header_missing, Optional<std::string> content_sum) {
	auto i = headers.find("Content-MD5");
	if (i != headers.end()) {
		// If a content sum is not provided, calculate one from the response content
		if (!content_sum.present()) {
			MD5_CTX sum;
			::MD5_Init(&sum);
			::MD5_Update(&sum, content.data(), content.size());
			std::string sumBytes;
			sumBytes.resize(16);
			::MD5_Final((unsigned char*)sumBytes.data(), &sum);
			std::string sumStr = base64::encoder::from_string(sumBytes);
			sumStr.resize(sumStr.size() - 1);
			content_sum = sumStr;
		}
		return i->second == content_sum.get();
	}
	return !fail_if_header_missing;
}

std::string Response::toString() {
	std::string r = format("Response Code: %d\n", code);
	r += format("Response ContentLen: %lld\n", contentLen);
	for (auto h : headers)
		r += format("Reponse Header: %s: %s\n", h.first.c_str(), h.second.c_str());
	r.append("-- RESPONSE CONTENT--\n");
	r.append(content);
	r.append("\n--------\n");
	return r;
}

PacketBuffer* writeRequestHeader(std::string const& verb,
                                 std::string const& resource,
                                 HTTP::Headers const& headers,
                                 PacketBuffer* dest) {
	PacketWriter writer(dest, nullptr, Unversioned());
	writer.serializeBytes(verb);
	writer.serializeBytes(" ", 1);
	writer.serializeBytes(resource);
	writer.serializeBytes(LiteralStringRef(" HTTP/1.1\r\n"));
	for (auto h : headers) {
		writer.serializeBytes(h.first);
		writer.serializeBytes(LiteralStringRef(": "));
		writer.serializeBytes(h.second);
		writer.serializeBytes(LiteralStringRef("\r\n"));
	}
	writer.serializeBytes(LiteralStringRef("\r\n"));
	return writer.finish();
}

// Read at least 1 bytes from conn and up to maxlen in a single read, append read data into *buf
// Returns the number of bytes read.
															#line 121 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_into_string()
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class Read_into_stringActor>
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_into_stringActorState {
															#line 127 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_into_stringActorState(Reference<IConnection> const& conn,std::string* const& buf,int const& maxlen) 
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   maxlen(maxlen)
															#line 138 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 120 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			;
															#line 153 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 122 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int originalSize = buf->size();
															#line 125 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		buf->resize(originalSize + maxlen);
															#line 126 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		uint8_t* wptr = (uint8_t*)buf->data() + originalSize;
															#line 127 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int len = conn->read(wptr, wptr + maxlen);
															#line 128 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		buf->resize(originalSize + len);
															#line 131 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (len > 0)
															#line 193 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 132 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_into_stringActor*>(this)->SAV<int>::futures) { (void)(len); this->~Read_into_stringActorState(); static_cast<Read_into_stringActor*>(this)->destroy(); return 0; }
															#line 197 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_into_stringActor*>(this)->SAV< int >::value()) int(len);
			this->~Read_into_stringActorState();
			static_cast<Read_into_stringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 135 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = conn->onReadable();
															#line 135 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 207 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 1;
															#line 135 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 0, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 212 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 136 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 136 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 223 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 2;
															#line 136 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 1, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 228 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 136 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 136 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 239 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 2;
															#line 136 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 1, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 244 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int maxlen;
															#line 393 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_into_string()
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_into_stringActor final : public Actor<int>, public ActorCallback< Read_into_stringActor, 0, Void >, public ActorCallback< Read_into_stringActor, 1, Void >, public FastAllocated<Read_into_stringActor>, public Read_into_stringActorState<Read_into_stringActor> {
															#line 398 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_into_stringActor>::operator new;
	using FastAllocated<Read_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_into_stringActor, 0, Void >;
friend struct ActorCallback< Read_into_stringActor, 1, Void >;
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_into_stringActor(Reference<IConnection> const& conn,std::string* const& buf,int const& maxlen) 
															#line 410 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<int> read_into_string( Reference<IConnection> const& conn, std::string* const& buf, int const& maxlen ) {
															#line 119 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<int>(new Read_into_stringActor(conn, buf, maxlen));
															#line 438 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 139 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

// Returns the position of delim within buf, relative to pos.  If delim is not found, continues to read from conn until
// either it is found or the connection ends, at which point connection_failed is thrown and buf contains
// everything that was read up to that point.
															#line 446 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_delimited_into_string()
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class Read_delimited_into_stringActor>
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_delimited_into_stringActorState {
															#line 452 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_delimited_into_stringActorState(Reference<IConnection> const& conn,const char* const& delim,std::string* const& buf,size_t const& pos) 
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   delim(delim),
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 147 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   sPos(pos),
															#line 148 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   lookBack(strlen(delim) - 1)
															#line 469 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 149 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			ASSERT(lookBack >= 0);
															#line 151 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			;
															#line 486 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 152 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		size_t endPos = buf->find(delim, sPos);
															#line 153 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (endPos != std::string::npos)
															#line 518 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 154 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_delimited_into_stringActor*>(this)->SAV<size_t>::futures) { (void)(endPos - pos); this->~Read_delimited_into_stringActorState(); static_cast<Read_delimited_into_stringActor*>(this)->destroy(); return 0; }
															#line 522 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_delimited_into_stringActor*>(this)->SAV< size_t >::value()) size_t(endPos - pos);
			this->~Read_delimited_into_stringActorState();
			static_cast<Read_delimited_into_stringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 156 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (sPos >= lookBack)
															#line 530 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 157 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			sPos -= lookBack;
															#line 534 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 158 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = success(read_into_string(conn, buf, FLOW_KNOBS->HTTP_READ_SIZE));
															#line 158 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 540 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state = 1;
															#line 158 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_delimited_into_stringActor, 0, Void >*>(static_cast<Read_delimited_into_stringActor*>(this)));
															#line 545 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	const char* delim;
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 147 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	size_t sPos;
															#line 148 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int lookBack;
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_delimited_into_string()
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_delimited_into_stringActor final : public Actor<size_t>, public ActorCallback< Read_delimited_into_stringActor, 0, Void >, public FastAllocated<Read_delimited_into_stringActor>, public Read_delimited_into_stringActorState<Read_delimited_into_stringActor> {
															#line 642 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_delimited_into_stringActor>::operator new;
	using FastAllocated<Read_delimited_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<size_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_delimited_into_stringActor, 0, Void >;
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_delimited_into_stringActor(Reference<IConnection> const& conn,const char* const& delim,std::string* const& buf,size_t const& pos) 
															#line 653 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<size_t> read_delimited_into_string( Reference<IConnection> const& conn, const char* const& delim, std::string* const& buf, size_t const& pos ) {
															#line 143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<size_t>(new Read_delimited_into_stringActor(conn, delim, buf, pos));
															#line 680 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 161 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

// Reads from conn (as needed) until there are at least len bytes starting at pos in buf
															#line 686 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_fixed_into_string()
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class Read_fixed_into_stringActor>
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_fixed_into_stringActorState {
															#line 692 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_fixed_into_stringActorState(Reference<IConnection> const& conn,int const& len,std::string* const& buf,size_t const& pos) 
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   len(len),
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 164 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   stop_size(pos + len)
															#line 707 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 165 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			;
															#line 722 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 167 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_fixed_into_stringActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_fixed_into_stringActorState(); static_cast<Read_fixed_into_stringActor*>(this)->destroy(); return 0; }
															#line 745 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 165 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!(buf->size() < stop_size))
															#line 764 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 166 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = success(read_into_string(conn, buf, FLOW_KNOBS->HTTP_READ_SIZE));
															#line 166 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 772 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state = 1;
															#line 166 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_fixed_into_stringActor, 0, Void >*>(static_cast<Read_fixed_into_stringActor*>(this)));
															#line 777 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int len;
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 164 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int stop_size;
															#line 880 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_fixed_into_string()
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_fixed_into_stringActor final : public Actor<Void>, public ActorCallback< Read_fixed_into_stringActor, 0, Void >, public FastAllocated<Read_fixed_into_stringActor>, public Read_fixed_into_stringActorState<Read_fixed_into_stringActor> {
															#line 885 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_fixed_into_stringActor>::operator new;
	using FastAllocated<Read_fixed_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_fixed_into_stringActor, 0, Void >;
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_fixed_into_stringActor(Reference<IConnection> const& conn,int const& len,std::string* const& buf,size_t const& pos) 
															#line 896 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_fixed_into_string( Reference<IConnection> const& conn, int const& len, std::string* const& buf, size_t const& pos ) {
															#line 163 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_fixed_into_stringActor(conn, len, buf, pos));
															#line 923 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 169 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

															#line 928 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_response_headers()
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class Read_http_response_headersActor>
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_http_response_headersActorState {
															#line 934 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_http_response_headersActorState(Reference<IConnection> const& conn,Headers* const& headers,std::string* const& buf,size_t* const& pos) 
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   headers(headers),
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   pos(pos)
															#line 947 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 174 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			;
															#line 962 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 176 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", buf, *pos);
															#line 176 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_response_headersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 994 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_http_response_headersActor*>(this)->actor_wait_state = 1;
															#line 176 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_response_headersActor, 0, size_t >*>(static_cast<Read_http_response_headersActor*>(this)));
															#line 999 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(size_t const& lineLen,int loopDepth) 
	{
															#line 179 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (lineLen == 0)
															#line 1008 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 181 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pos += 2;
															#line 182 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_response_headersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_response_headersActorState(); static_cast<Read_http_response_headersActor*>(this)->destroy(); return 0; }
															#line 1014 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_response_headersActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_response_headersActorState();
			static_cast<Read_http_response_headersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 185 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int nameEnd = -1, valueStart = -1, valueEnd = -1;
															#line 186 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int len = -1;
															#line 188 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		std::string name, value;
															#line 199 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^:]%n:%*[ \t]%n", &nameEnd, &valueStart) >= 0 && valueStart > 0)
															#line 1028 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 201 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			name = std::string(buf->substr(*pos, nameEnd));
															#line 202 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pos += valueStart;
															#line 1034 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 205 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1040 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 208 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^\r]%n%*1[\r]%*1[\n]%n", &valueEnd, &len) >= 0 && len > 0)
															#line 1044 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 210 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			value = std::string(buf->substr(*pos, valueEnd));
															#line 211 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pos += len;
															#line 1050 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 212 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (sscanf(buf->c_str() + *pos, "%*1[\r]%*1[\n]%n", &len) >= 0 && len > 0)
															#line 1056 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 214 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				*pos += len;
															#line 1060 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
			else
			{
															#line 217 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1066 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 220 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		(*headers)[name] = value;
															#line 1071 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 179 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (lineLen == 0)
															#line 1080 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 181 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pos += 2;
															#line 182 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_response_headersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_response_headersActorState(); static_cast<Read_http_response_headersActor*>(this)->destroy(); return 0; }
															#line 1086 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_response_headersActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_response_headersActorState();
			static_cast<Read_http_response_headersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 185 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int nameEnd = -1, valueStart = -1, valueEnd = -1;
															#line 186 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int len = -1;
															#line 188 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		std::string name, value;
															#line 199 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^:]%n:%*[ \t]%n", &nameEnd, &valueStart) >= 0 && valueStart > 0)
															#line 1100 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 201 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			name = std::string(buf->substr(*pos, nameEnd));
															#line 202 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pos += valueStart;
															#line 1106 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 205 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1112 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 208 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^\r]%n%*1[\r]%*1[\n]%n", &valueEnd, &len) >= 0 && len > 0)
															#line 1116 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 210 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			value = std::string(buf->substr(*pos, valueEnd));
															#line 211 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pos += len;
															#line 1122 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 212 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (sscanf(buf->c_str() + *pos, "%*1[\r]%*1[\n]%n", &len) >= 0 && len > 0)
															#line 1128 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 214 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				*pos += len;
															#line 1132 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
			else
			{
															#line 217 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1138 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 220 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		(*headers)[name] = value;
															#line 1143 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Headers* headers;
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	size_t* pos;
															#line 1219 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_response_headers()
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_http_response_headersActor final : public Actor<Void>, public ActorCallback< Read_http_response_headersActor, 0, size_t >, public FastAllocated<Read_http_response_headersActor>, public Read_http_response_headersActorState<Read_http_response_headersActor> {
															#line 1224 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_http_response_headersActor>::operator new;
	using FastAllocated<Read_http_response_headersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_http_response_headersActor, 0, size_t >;
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_http_response_headersActor(Reference<IConnection> const& conn,Headers* const& headers,std::string* const& buf,size_t* const& pos) 
															#line 1235 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_response_headers( Reference<IConnection> const& conn, Headers* const& headers, std::string* const& buf, size_t* const& pos ) {
															#line 170 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_response_headersActor(conn, headers, buf, pos));
															#line 1262 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 223 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

// Reads an HTTP response from a network connection
// If the connection fails while being read the exception will emitted
// If the response is not parsable or complete in some way, http_bad_response will be thrown
															#line 1270 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_response()
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class Read_http_responseActor>
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_http_responseActorState {
															#line 1276 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_http_responseActorState(Reference<HTTP::Response> const& r,Reference<IConnection> const& conn,bool const& header_only) 
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : r(r),
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   conn(conn),
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   header_only(header_only),
															#line 228 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   buf(),
															#line 229 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   pos(0)
															#line 1291 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 232 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", &buf, pos);
															#line 232 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1308 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 1;
															#line 232 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 0, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1313 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 234 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int reachedEnd = -1;
															#line 235 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		sscanf(buf.c_str() + pos, "HTTP/%f %d%n", &r->version, &r->code, &reachedEnd);
															#line 236 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (reachedEnd < 0)
															#line 1340 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 237 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1344 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 240 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 243 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->headers.clear();
															#line 245 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = read_http_response_headers(conn, &r->headers, &buf, &pos);
															#line 245 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 2;
															#line 245 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 1, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1359 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 234 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int reachedEnd = -1;
															#line 235 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		sscanf(buf.c_str() + pos, "HTTP/%f %d%n", &r->version, &r->code, &reachedEnd);
															#line 236 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (reachedEnd < 0)
															#line 1372 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 237 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1376 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 240 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 243 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->headers.clear();
															#line 245 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = read_http_response_headers(conn, &r->headers, &buf, &pos);
															#line 245 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1386 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 2;
															#line 245 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 1, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1391 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 247 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		auto i = r->headers.find("Content-Length");
															#line 248 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1465 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 249 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->contentLen = strtoll(i->second.c_str(), NULL, 10);
															#line 1469 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 251 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->contentLen = -1;
															#line 1475 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 253 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		transferEncoding = std::string();
															#line 254 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		i = r->headers.find("Transfer-Encoding");
															#line 255 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1483 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 256 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			transferEncoding = i->second;
															#line 1487 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 258 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.clear();
															#line 262 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (header_only && pos == buf.size())
															#line 1493 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 263 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1497 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_responseActorState();
			static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 267 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (r->contentLen >= 0)
															#line 1505 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 269 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->content = buf.substr(pos);
															#line 270 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			pos = 0;
															#line 273 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_2 = read_fixed_into_string(conn, r->contentLen, &r->content, pos);
															#line 273 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1515 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 3;
															#line 273 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 2, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1520 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 278 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (transferEncoding == "chunked")
															#line 1527 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 282 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				r->content = buf.substr(pos);
															#line 283 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				pos = 0;
															#line 285 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				;
															#line 1535 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				loopDepth = a_body1cont2loopHead1(loopDepth);
			}
			else
			{
															#line 329 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1542 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 247 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		auto i = r->headers.find("Content-Length");
															#line 248 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1554 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 249 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->contentLen = strtoll(i->second.c_str(), NULL, 10);
															#line 1558 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 251 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->contentLen = -1;
															#line 1564 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 253 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		transferEncoding = std::string();
															#line 254 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		i = r->headers.find("Transfer-Encoding");
															#line 255 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1572 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 256 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			transferEncoding = i->second;
															#line 1576 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 258 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.clear();
															#line 262 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (header_only && pos == buf.size())
															#line 1582 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 263 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1586 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_responseActorState();
			static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 267 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (r->contentLen >= 0)
															#line 1594 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 269 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->content = buf.substr(pos);
															#line 270 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			pos = 0;
															#line 273 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_2 = read_fixed_into_string(conn, r->contentLen, &r->content, pos);
															#line 273 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1604 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 3;
															#line 273 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 2, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1609 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 278 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (transferEncoding == "chunked")
															#line 1616 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 282 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				r->content = buf.substr(pos);
															#line 283 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				pos = 0;
															#line 285 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				;
															#line 1624 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				loopDepth = a_body1cont2loopHead1(loopDepth);
			}
			else
			{
															#line 329 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1631 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}

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
	int a_body1cont4(int loopDepth) 
	{
															#line 333 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() > 0)
															#line 1704 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 334 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (r->code == 206 && CLIENT_KNOBS->HTTP_RESPONSE_SKIP_VERIFY_CHECKSUM_FOR_PARTIAL_CONTENT)
															#line 1708 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 335 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1712 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~Read_http_responseActorState();
				static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 338 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!r->verifyMD5(false))
															#line 1720 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 339 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1724 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 343 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1729 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_http_responseActorState();
		static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 276 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() != r->contentLen)
															#line 1741 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 277 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1745 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 276 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() != r->contentLen)
															#line 1755 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 277 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1759 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont12(int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont13(int loopDepth) 
	{
															#line 316 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->contentLen = pos;
															#line 319 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_6 = read_http_response_headers(conn, &r->headers, &r->content, &pos);
															#line 319 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1842 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont13when1(__when_expr_6.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 7;
															#line 319 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 6, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1847 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1(int loopDepth) 
	{
		{
															#line 288 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_3 = read_delimited_into_string(conn, "\r\n", &r->content, pos);
															#line 288 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1866 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 4;
															#line 288 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 3, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1871 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2break1(int loopDepth) 
	{
		try {
			return a_body1cont13(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(int loopDepth) 
	{
		{
															#line 306 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_5 = read_delimited_into_string(conn, "\r\n", &r->content, pos);
															#line 306 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1897 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont1when1(__when_expr_5.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 6;
															#line 306 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 5, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1902 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2(size_t const& lineLen,int loopDepth) 
	{
															#line 289 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		chunkLen = strtol(r->content.substr(pos, lineLen).c_str(), nullptr, 16);
															#line 293 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, lineLen + 2);
															#line 296 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (chunkLen == 0)
															#line 1916 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_4 = read_fixed_into_string(conn, chunkLen, &r->content, pos);
															#line 300 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1924 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 5;
															#line 300 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 4, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1929 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2(size_t && lineLen,int loopDepth) 
	{
															#line 289 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		chunkLen = strtol(r->content.substr(pos, lineLen).c_str(), nullptr, 16);
															#line 293 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, lineLen + 2);
															#line 296 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (chunkLen == 0)
															#line 1942 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_4 = read_fixed_into_string(conn, chunkLen, &r->content, pos);
															#line 300 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1950 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 5;
															#line 300 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 4, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1955 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont2(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont2(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 3, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 3, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 3, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 3, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont2loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 301 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		pos += chunkLen;
															#line 2027 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 301 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		pos += chunkLen;
															#line 2036 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont2loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont2loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont2loopBody1cont6(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont7(size_t const& lineLen,int loopDepth) 
	{
															#line 307 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (lineLen != 0)
															#line 2114 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 308 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 2118 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 311 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, 2);
															#line 2122 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont7(size_t && lineLen,int loopDepth) 
	{
															#line 307 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (lineLen != 0)
															#line 2131 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 308 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 2135 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 311 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, 2);
															#line 2139 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1when1(size_t const& lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont7(lineLen, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1when1(size_t && lineLen,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont7(std::move(lineLen), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 5, size_t >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 5, size_t >*,size_t const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont2loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 5, size_t >*,size_t && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont2loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 5, size_t >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont13cont1(Void const& _,int loopDepth) 
	{
															#line 322 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (pos != r->content.size())
															#line 2211 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 323 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2215 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 326 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.erase(r->contentLen);
															#line 2219 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont13cont1(Void && _,int loopDepth) 
	{
															#line 322 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (pos != r->content.size())
															#line 2228 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 323 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2232 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 326 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r->content.erase(r->contentLen);
															#line 2236 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont13when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont13cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont13when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont13cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state > 0) static_cast<Read_http_responseActor*>(this)->actor_wait_state = 0;
		static_cast<Read_http_responseActor*>(this)->ActorCallback< Read_http_responseActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont13when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< Read_http_responseActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont13when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< Read_http_responseActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_http_response", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_http_response", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::Response> r;
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	bool header_only;
															#line 228 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string buf;
															#line 229 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 253 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string transferEncoding;
															#line 289 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int chunkLen;
															#line 2318 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_response()
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class Read_http_responseActor final : public Actor<Void>, public ActorCallback< Read_http_responseActor, 0, size_t >, public ActorCallback< Read_http_responseActor, 1, Void >, public ActorCallback< Read_http_responseActor, 2, Void >, public ActorCallback< Read_http_responseActor, 3, size_t >, public ActorCallback< Read_http_responseActor, 4, Void >, public ActorCallback< Read_http_responseActor, 5, size_t >, public ActorCallback< Read_http_responseActor, 6, Void >, public FastAllocated<Read_http_responseActor>, public Read_http_responseActorState<Read_http_responseActor> {
															#line 2323 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_http_responseActor>::operator new;
	using FastAllocated<Read_http_responseActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_http_responseActor, 0, size_t >;
friend struct ActorCallback< Read_http_responseActor, 1, Void >;
friend struct ActorCallback< Read_http_responseActor, 2, Void >;
friend struct ActorCallback< Read_http_responseActor, 3, size_t >;
friend struct ActorCallback< Read_http_responseActor, 4, Void >;
friend struct ActorCallback< Read_http_responseActor, 5, size_t >;
friend struct ActorCallback< Read_http_responseActor, 6, Void >;
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Read_http_responseActor(Reference<HTTP::Response> const& r,Reference<IConnection> const& conn,bool const& header_only) 
															#line 2340 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
		case 3: this->a_callback_error((ActorCallback< Read_http_responseActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< Read_http_responseActor, 3, size_t >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< Read_http_responseActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< Read_http_responseActor, 5, size_t >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< Read_http_responseActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_response( Reference<HTTP::Response> const& r, Reference<IConnection> const& conn, bool const& header_only ) {
															#line 227 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_responseActor(r, conn, header_only));
															#line 2373 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 345 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

Future<Void> HTTP::Response::read(Reference<IConnection> conn, bool header_only) {
	return read_http_response(Reference<HTTP::Response>::addRef(this), conn, header_only);
}

// Do a request, get a Response.
// Request content is provided as UnsentPacketQueue *pContent which will be depleted as bytes are sent but the queue
// itself must live for the life of this actor and be destroyed by the caller
// TODO:  pSent is very hackish, do something better.
															#line 2386 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via doRequest()
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class DoRequestActor>
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class DoRequestActorState {
															#line 2392 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	DoRequestActorState(Reference<IConnection> const& conn,std::string const& verb,std::string const& resource,HTTP::Headers const& headers,UnsentPacketQueue* const& pContent,int const& contentLen,Reference<IRateControl> const& sendRate,int64_t* const& pSent,Reference<IRateControl> const& recvRate,std::string const& requestIDHeader) 
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   verb(verb),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   resource(resource),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   headers(headers),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   pContent(pContent),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   contentLen(contentLen),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   sendRate(sendRate),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   pSent(pSent),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   recvRate(recvRate),
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   requestIDHeader(requestIDHeader),
															#line 364 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   event(SevDebug, "HTTPRequest"),
															#line 366 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   empty()
															#line 2421 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
	{
		fdb_probe_actor_create("doRequest", reinterpret_cast<unsigned long>(this));

	}
	~DoRequestActorState() 
	{
		fdb_probe_actor_destroy("doRequest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 367 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (pContent == nullptr)
															#line 2436 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 368 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				pContent = &empty;
															#line 2440 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 372 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (requestIDHeader.empty())
															#line 2444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 373 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				requestIDHeader = FLOW_KNOBS->HTTP_REQUEST_ID_HEADER;
															#line 2448 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 376 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			earlyResponse = false;
															#line 377 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			total_sent = 0;
															#line 378 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			send_start = double();
															#line 380 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("DebugID", conn->getDebugID());
															#line 381 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("RemoteAddress", conn->getPeerAddress());
															#line 382 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("Verb", verb);
															#line 383 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("Resource", resource);
															#line 384 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestContentLen", contentLen);
															#line 2466 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			try {
															#line 387 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				requestID = std::string();
															#line 388 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				if (!requestIDHeader.empty())
															#line 2472 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 389 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					requestID = deterministicRandom()->randomUniqueID().toString();
															#line 390 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(20, "-");
															#line 391 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(16, "-");
															#line 392 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(12, "-");
															#line 393 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(8, "-");
															#line 395 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					headers[requestIDHeader] = requestID;
															#line 396 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					event.detail("RequestIDSent", requestID);
															#line 2488 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				}
															#line 400 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				PacketBuffer* pFirst = PacketBuffer::create();
															#line 401 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				PacketBuffer* pLast = writeRequestHeader(verb, resource, headers, pFirst);
															#line 403 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				pContent->prependWriteBuffer(pFirst, pLast);
															#line 405 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 1)
															#line 2498 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 406 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					printf("[%s] HTTP starting %s %s ContentLen:%d\n", conn->getDebugID().toString().c_str(), verb.c_str(), resource.c_str(), contentLen);
															#line 2502 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				}
															#line 411 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 2506 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 412 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					for( auto h : headers ) {
															#line 413 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
						printf("Request Header: %s: %s\n", h.first.c_str(), h.second.c_str());
															#line 2512 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
					}
				}
															#line 416 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				r = Reference<HTTP::Response>(new HTTP::Response());
															#line 417 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				responseReading = r->read(conn, verb == "HEAD" || verb == "DELETE" || verb == "CONNECT");
															#line 419 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				send_start = timer();
															#line 421 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				;
															#line 2523 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
		this->~DoRequestActorState();
		static_cast<DoRequestActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 514 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			double elapsed = timer() - send_start;
															#line 516 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0 && e.code() != error_code_http_bad_request_id)
															#line 2555 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 517 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				printf("[%s] HTTP *ERROR*=%s early=%d, time=%fs %s %s contentLen=%d [%d out]\n", conn->getDebugID().toString().c_str(), e.name(), earlyResponse, elapsed, verb.c_str(), resource.c_str(), contentLen, total_sent);
															#line 2559 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 527 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.errorUnsuppressed(e);
															#line 528 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 2565 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(int loopDepth) 
	{
															#line 447 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_3 = responseReading;
															#line 447 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2581 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 4;
															#line 447 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 3, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2586 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 425 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (responseReading.isReady())
															#line 2602 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 426 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			conn->close();
															#line 427 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			r->headers["Connection"] = "close";
															#line 428 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			earlyResponse = true;
															#line 2610 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 432 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		trySend = FLOW_KNOBS->HTTP_SEND_SIZE;
															#line 433 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = sendRate->getAllowance(trySend);
															#line 433 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2619 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 1;
															#line 433 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 0, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2624 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont4(loopDepth);
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
															#line 434 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(pContent->getUnsent(), trySend);
															#line 435 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (pSent != nullptr)
															#line 2648 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 436 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pSent += len;
															#line 2652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 437 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		sendRate->returnUnused(trySend - len);
															#line 438 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		total_sent += len;
															#line 439 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		pContent->sent(len);
															#line 440 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (pContent->empty())
															#line 2662 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 443 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 443 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2670 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 2;
															#line 443 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 1, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2675 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 434 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(pContent->getUnsent(), trySend);
															#line 435 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (pSent != nullptr)
															#line 2686 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 436 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			*pSent += len;
															#line 2690 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 437 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		sendRate->returnUnused(trySend - len);
															#line 438 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		total_sent += len;
															#line 439 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		pContent->sent(len);
															#line 440 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (pContent->empty())
															#line 2700 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 443 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 443 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2708 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 2;
															#line 443 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 1, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2713 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
		if (static_cast<DoRequestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActor*>(this)->ActorCallback< DoRequestActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoRequestActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2787 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 3;
															#line 444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 2, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2792 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2803 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 3;
															#line 444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 2, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2808 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
		if (static_cast<DoRequestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActor*>(this)->ActorCallback< DoRequestActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoRequestActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DoRequestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActor*>(this)->ActorCallback< DoRequestActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DoRequestActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 448 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		double elapsed = timer() - send_start;
															#line 450 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseCode", r->code);
															#line 451 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseContentLen", r->contentLen);
															#line 452 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("Elapsed", elapsed);
															#line 454 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		Optional<Error> err;
															#line 455 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!requestIDHeader.empty())
															#line 2965 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 456 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			std::string responseID;
															#line 457 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			auto iid = r->headers.find(requestIDHeader);
															#line 458 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (iid != r->headers.end())
															#line 2973 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 459 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				responseID = iid->second;
															#line 2977 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 461 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestIDReceived", responseID);
															#line 465 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			bool serverError = r->code >= 500 && r->code < 600;
															#line 469 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (requestID != responseID && (!serverError || !responseID.empty()))
															#line 2985 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 470 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				err = http_bad_request_id();
															#line 472 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				TraceEvent(SevError, "HTTPRequestFailedIDMismatch") .error(err.get()) .detail("DebugID", conn->getDebugID()) .detail("RemoteAddress", conn->getPeerAddress()) .detail("Verb", verb) .detail("Resource", resource) .detail("RequestContentLen", contentLen) .detail("ResponseCode", r->code) .detail("ResponseContentLen", r->contentLen) .detail("RequestIDSent", requestID) .detail("RequestIDReceived", responseID);
															#line 2991 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 486 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0)
															#line 2996 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 487 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{0}] HTTP {1}code={2} early={3}, time={4} {5} {6} contentLen={7} [{8} out, response content " "len {9}]\n", conn->getDebugID().toString(), (err.present() ? format("*ERROR*=%s ", err.get().name()).c_str() : ""), r->code, earlyResponse, elapsed, verb, resource, contentLen, total_sent, r->contentLen);
															#line 3000 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 500 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 3004 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 501 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			printf("[%s] HTTP RESPONSE:  %s %s\n%s\n", conn->getDebugID().toString().c_str(), verb.c_str(), resource.c_str(), r->toString().c_str());
															#line 3008 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 508 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 3012 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 509 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch2(err.get(), loopDepth);
															#line 3016 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 512 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<DoRequestActor*>(this)->SAV<Reference<HTTP::Response>>::futures) { (void)(r); this->~DoRequestActorState(); static_cast<DoRequestActor*>(this)->destroy(); return 0; }
															#line 3020 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<DoRequestActor*>(this)->SAV< Reference<HTTP::Response> >::value()) Reference<HTTP::Response>(std::move(r)); // state_var_RVO
		this->~DoRequestActorState();
		static_cast<DoRequestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 448 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		double elapsed = timer() - send_start;
															#line 450 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseCode", r->code);
															#line 451 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseContentLen", r->contentLen);
															#line 452 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("Elapsed", elapsed);
															#line 454 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		Optional<Error> err;
															#line 455 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!requestIDHeader.empty())
															#line 3042 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 456 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			std::string responseID;
															#line 457 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			auto iid = r->headers.find(requestIDHeader);
															#line 458 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (iid != r->headers.end())
															#line 3050 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 459 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				responseID = iid->second;
															#line 3054 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 461 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestIDReceived", responseID);
															#line 465 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			bool serverError = r->code >= 500 && r->code < 600;
															#line 469 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (requestID != responseID && (!serverError || !responseID.empty()))
															#line 3062 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 470 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				err = http_bad_request_id();
															#line 472 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				TraceEvent(SevError, "HTTPRequestFailedIDMismatch") .error(err.get()) .detail("DebugID", conn->getDebugID()) .detail("RemoteAddress", conn->getPeerAddress()) .detail("Verb", verb) .detail("Resource", resource) .detail("RequestContentLen", contentLen) .detail("ResponseCode", r->code) .detail("ResponseContentLen", r->contentLen) .detail("RequestIDSent", requestID) .detail("RequestIDReceived", responseID);
															#line 3068 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 486 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0)
															#line 3073 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 487 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{0}] HTTP {1}code={2} early={3}, time={4} {5} {6} contentLen={7} [{8} out, response content " "len {9}]\n", conn->getDebugID().toString(), (err.present() ? format("*ERROR*=%s ", err.get().name()).c_str() : ""), r->code, earlyResponse, elapsed, verb, resource, contentLen, total_sent, r->contentLen);
															#line 3077 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 500 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 3081 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 501 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			printf("[%s] HTTP RESPONSE:  %s %s\n%s\n", conn->getDebugID().toString().c_str(), verb.c_str(), resource.c_str(), r->toString().c_str());
															#line 3085 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 508 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 3089 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 509 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch2(err.get(), loopDepth);
															#line 3093 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 512 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<DoRequestActor*>(this)->SAV<Reference<HTTP::Response>>::futures) { (void)(r); this->~DoRequestActorState(); static_cast<DoRequestActor*>(this)->destroy(); return 0; }
															#line 3097 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<DoRequestActor*>(this)->SAV< Reference<HTTP::Response> >::value()) Reference<HTTP::Response>(std::move(r)); // state_var_RVO
		this->~DoRequestActorState();
		static_cast<DoRequestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DoRequestActor*>(this)->actor_wait_state > 0) static_cast<DoRequestActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequestActor*>(this)->ActorCallback< DoRequestActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DoRequestActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DoRequestActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string verb;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string resource;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	HTTP::Headers headers;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	UnsentPacketQueue* pContent;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int contentLen;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> sendRate;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int64_t* pSent;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> recvRate;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string requestIDHeader;
															#line 364 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	TraceEvent event;
															#line 366 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	UnsentPacketQueue empty;
															#line 376 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	bool earlyResponse;
															#line 377 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int total_sent;
															#line 378 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	double send_start;
															#line 387 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string requestID;
															#line 416 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::Response> r;
															#line 417 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Future<Void> responseReading;
															#line 432 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int trySend;
															#line 3206 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via doRequest()
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class DoRequestActor final : public Actor<Reference<HTTP::Response>>, public ActorCallback< DoRequestActor, 0, Void >, public ActorCallback< DoRequestActor, 1, Void >, public ActorCallback< DoRequestActor, 2, Void >, public ActorCallback< DoRequestActor, 3, Void >, public FastAllocated<DoRequestActor>, public DoRequestActorState<DoRequestActor> {
															#line 3211 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<DoRequestActor>::operator new;
	using FastAllocated<DoRequestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::Response>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoRequestActor, 0, Void >;
friend struct ActorCallback< DoRequestActor, 1, Void >;
friend struct ActorCallback< DoRequestActor, 2, Void >;
friend struct ActorCallback< DoRequestActor, 3, Void >;
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	DoRequestActor(Reference<IConnection> const& conn,std::string const& verb,std::string const& resource,HTTP::Headers const& headers,UnsentPacketQueue* const& pContent,int const& contentLen,Reference<IRateControl> const& sendRate,int64_t* const& pSent,Reference<IRateControl> const& recvRate,std::string const& requestIDHeader) 
															#line 3225 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		 : Actor<Reference<HTTP::Response>>(),
		   DoRequestActorState<DoRequestActor>(conn, verb, resource, headers, pContent, contentLen, sendRate, pSent, recvRate, requestIDHeader)
	{
		fdb_probe_actor_enter("doRequest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doRequest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doRequest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoRequestActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoRequestActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DoRequestActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DoRequestActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::Response>> doRequest( Reference<IConnection> const& conn, std::string const& verb, std::string const& resource, HTTP::Headers const& headers, UnsentPacketQueue* const& pContent, int const& contentLen, Reference<IRateControl> const& sendRate, int64_t* const& pSent, Reference<IRateControl> const& recvRate, std::string const& requestIDHeader ) {
															#line 354 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<Reference<HTTP::Response>>(new DoRequestActor(conn, verb, resource, headers, pContent, contentLen, sendRate, pSent, recvRate, requestIDHeader));
															#line 3255 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 531 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

															#line 3260 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via sendProxyConnectRequest()
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class SendProxyConnectRequestActor>
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class SendProxyConnectRequestActorState {
															#line 3266 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	SendProxyConnectRequestActorState(Reference<IConnection> const& conn,std::string const& remoteHost,std::string const& remoteService) 
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   remoteHost(remoteHost),
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   remoteService(remoteService),
															#line 535 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   headers()
															#line 3279 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 536 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			headers["Host"] = remoteHost + ":" + remoteService;
															#line 537 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			headers["Accept"] = "application/xml";
															#line 538 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			headers["Proxy-Connection"] = "Keep-Alive";
															#line 539 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			requestTimeout = 60;
															#line 540 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			maxTries = FLOW_KNOBS->HTTP_CONNECT_TRIES;
															#line 541 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			thisTry = 1;
															#line 542 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			nextRetryDelay = 2.0;
															#line 543 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			sendReceiveRate = makeReference<Unlimited>();
															#line 544 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			bytes_sent = 0;
															#line 546 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			;
															#line 3312 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 547 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		err = Optional<Error>();
															#line 548 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r = Reference<Response>();
															#line 3344 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		try {
															#line 551 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Reference<Response>> __when_expr_0 = timeoutError(doRequest(conn, "CONNECT", remoteHost + ":" + remoteService, headers, nullptr, 0, sendReceiveRate, &bytes_sent, sendReceiveRate), requestTimeout);
															#line 551 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3350 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 1;
															#line 551 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >*>(static_cast<SendProxyConnectRequestActor*>(this)));
															#line 3355 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 570 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!err.present() && r->code == 200)
															#line 3370 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 571 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<SendProxyConnectRequestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendProxyConnectRequestActorState(); static_cast<SendProxyConnectRequestActor*>(this)->destroy(); return 0; }
															#line 3374 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<SendProxyConnectRequestActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~SendProxyConnectRequestActorState();
			static_cast<SendProxyConnectRequestActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 575 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		bool retryable = err.present() || r->code == 500 || r->code == 502 || r->code == 503 || r->code == 429;
															#line 578 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		retryable = retryable && (thisTry < maxTries);
															#line 580 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		TraceEvent event(SevWarn, retryable ? "ProxyConnectCommandFailedRetryable" : "ProxyConnectCommandFailed");
															#line 583 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 3388 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 584 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.errorUnsuppressed(err.get());
															#line 3392 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 586 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.suppressFor(60);
															#line 587 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!err.present())
															#line 3398 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 588 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("ResponseCode", r->code);
															#line 3402 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 591 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		event.detail("ThisTry", thisTry);
															#line 595 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!r || r->code != 429)
															#line 3408 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 596 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			++thisTry;
															#line 3412 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 599 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		double delay = nextRetryDelay;
															#line 601 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		nextRetryDelay = std::min(nextRetryDelay * 2, 60.0);
															#line 603 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (retryable)
															#line 3420 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 605 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (r)
															#line 3424 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 606 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				auto iRetryAfter = r->headers.find("Retry-After");
															#line 607 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				if (iRetryAfter != r->headers.end())
															#line 3430 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 608 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					event.detail("RetryAfterHeader", iRetryAfter->second);
															#line 609 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					char* pEnd;
															#line 610 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					double retryAfter = strtod(iRetryAfter->second.c_str(), &pEnd);
															#line 611 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					if (*pEnd)
															#line 3440 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
					{
															#line 613 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
						retryAfter = 300;
															#line 3444 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
					}
															#line 615 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
					delay = std::max(delay, retryAfter);
															#line 3448 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
				}
			}
															#line 620 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			event.detail("RetryDelay", delay);
															#line 621 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(delay);
															#line 621 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3457 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 2;
															#line 621 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SendProxyConnectRequestActor, 1, Void >*>(static_cast<SendProxyConnectRequestActor*>(this)));
															#line 3462 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 626 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (r && r->code == 406)
															#line 3469 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 627 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_not_accepted(), std::max(0, loopDepth - 1));
															#line 3473 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 629 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (r && r->code == 401)
															#line 3477 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 630 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_auth_failed(), std::max(0, loopDepth - 1));
															#line 3481 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 632 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 1));
															#line 3485 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 563 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 3495 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 564 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 3499 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 565 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			err = e;
															#line 3503 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Reference<Response> const& _r,int loopDepth) 
	{
															#line 561 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r = _r;
															#line 3518 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Reference<Response> && _r,int loopDepth) 
	{
															#line 561 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		r = _r;
															#line 3527 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<Response> const& _r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_r, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<Response> && _r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_r), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state > 0) static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 0;
		static_cast<SendProxyConnectRequestActor*>(this)->ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >::remove();

	}
	void a_callback_fire(ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >*,Reference<Response> const& value) 
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
	void a_callback_fire(ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >*,Reference<Response> && value) 
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
	void a_callback_error(ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >*,Error err) 
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
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string remoteHost;
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string remoteService;
															#line 535 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Headers headers;
															#line 539 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int requestTimeout;
															#line 540 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int maxTries;
															#line 541 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int thisTry;
															#line 542 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	double nextRetryDelay;
															#line 543 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 544 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	int64_t bytes_sent;
															#line 547 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Optional<Error> err;
															#line 548 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<Response> r;
															#line 3713 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via sendProxyConnectRequest()
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class SendProxyConnectRequestActor final : public Actor<Void>, public ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >, public ActorCallback< SendProxyConnectRequestActor, 1, Void >, public FastAllocated<SendProxyConnectRequestActor>, public SendProxyConnectRequestActorState<SendProxyConnectRequestActor> {
															#line 3718 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<SendProxyConnectRequestActor>::operator new;
	using FastAllocated<SendProxyConnectRequestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >;
friend struct ActorCallback< SendProxyConnectRequestActor, 1, Void >;
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	SendProxyConnectRequestActor(Reference<IConnection> const& conn,std::string const& remoteHost,std::string const& remoteService) 
															#line 3730 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
		case 1: this->a_callback_error((ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SendProxyConnectRequestActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> sendProxyConnectRequest( Reference<IConnection> const& conn, std::string const& remoteHost, std::string const& remoteService ) {
															#line 532 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new SendProxyConnectRequestActor(conn, remoteHost, remoteService));
															#line 3758 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 636 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

															#line 3763 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via proxyConnectImpl()
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
template <class ProxyConnectImplActor>
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class ProxyConnectImplActorState {
															#line 3769 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	ProxyConnectImplActorState(std::string const& remoteHost,std::string const& remoteService,std::string const& proxyHost,std::string const& proxyService) 
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		 : remoteHost(remoteHost),
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   remoteService(remoteService),
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   proxyHost(proxyHost),
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		   proxyService(proxyService)
															#line 3782 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 641 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			StrictFuture<NetworkAddress> __when_expr_0 = map(INetworkConnections::net()->resolveTCPEndpoint(remoteHost, remoteService), [=](std::vector<NetworkAddress> const& addresses) -> NetworkAddress { NetworkAddress addr = addresses[deterministicRandom()->randomInt(0, addresses.size())]; addr.fromHostname = true; addr.flags = NetworkAddress::FLAG_TLS; return addr; });
															#line 641 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3799 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 1;
															#line 641 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 3804 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 649 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_1 = INetworkConnections::net()->connect(proxyHost, proxyService);
															#line 649 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3829 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 2;
															#line 649 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 3834 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(NetworkAddress const& __remoteEndpoint,int loopDepth) 
	{
															#line 641 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		remoteEndpoint = __remoteEndpoint;
															#line 3843 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 650 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = sendProxyConnectRequest(connection, remoteHost, remoteService);
															#line 650 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3912 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 3;
															#line 650 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 2, Void >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 3917 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IConnection> const& __connection,int loopDepth) 
	{
															#line 649 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		connection = __connection;
															#line 3926 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 651 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		boost::asio::ip::tcp::socket socket = std::move(connection->getSocket());
															#line 652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(remoteEndpoint, &socket);
															#line 652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3997 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 4;
															#line 652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4002 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 651 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		boost::asio::ip::tcp::socket socket = std::move(connection->getSocket());
															#line 652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(remoteEndpoint, &socket);
															#line 652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4015 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 4;
															#line 652 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4020 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 653 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ProxyConnectImplActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(remoteConnection); this->~ProxyConnectImplActorState(); static_cast<ProxyConnectImplActor*>(this)->destroy(); return 0; }
															#line 4092 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<ProxyConnectImplActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(remoteConnection);
		this->~ProxyConnectImplActorState();
		static_cast<ProxyConnectImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Reference<IConnection> && remoteConnection,int loopDepth) 
	{
															#line 653 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ProxyConnectImplActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(remoteConnection); this->~ProxyConnectImplActorState(); static_cast<ProxyConnectImplActor*>(this)->destroy(); return 0; }
															#line 4104 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string remoteHost;
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string remoteService;
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string proxyHost;
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	std::string proxyService;
															#line 641 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	NetworkAddress remoteEndpoint;
															#line 649 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> connection;
															#line 4187 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via proxyConnectImpl()
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
class ProxyConnectImplActor final : public Actor<Reference<IConnection>>, public ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >, public ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >, public ActorCallback< ProxyConnectImplActor, 2, Void >, public ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >, public FastAllocated<ProxyConnectImplActor>, public ProxyConnectImplActorState<ProxyConnectImplActor> {
															#line 4192 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	ProxyConnectImplActor(std::string const& remoteHost,std::string const& remoteService,std::string const& proxyHost,std::string const& proxyService) 
															#line 4206 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
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
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Reference<IConnection>> proxyConnectImpl( std::string const& remoteHost, std::string const& remoteService, std::string const& proxyHost, std::string const& proxyService ) {
															#line 637 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"
	return Future<Reference<IConnection>>(new ProxyConnectImplActor(remoteHost, remoteService, proxyHost, proxyService));
															#line 4236 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.g.cpp"
}

#line 655 "/usr/src/libfdb_c/fdbrpc/HTTP.actor.cpp"

Future<Reference<IConnection>> proxyConnect(const std::string& remoteHost,
                                            const std::string& remoteService,
                                            const std::string& proxyHost,
                                            const std::string& proxyService) {
	return proxyConnectImpl(remoteHost, remoteService, proxyHost, proxyService);
}

} // namespace HTTP
