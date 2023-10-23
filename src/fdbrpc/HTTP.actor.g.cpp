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
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_into_string()
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_into_stringActor>
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_into_stringActorState {
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_into_stringActorState(Reference<IConnection> const& conn,std::string* const& buf,int const& maxlen) 
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   maxlen(maxlen)
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int originalSize = buf->size();
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		buf->resize(originalSize + maxlen);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		uint8_t* wptr = (uint8_t*)buf->data() + originalSize;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->read(wptr, wptr + maxlen);
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		buf->resize(originalSize + len);
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (len > 0)
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_into_stringActor*>(this)->SAV<int>::futures) { (void)(len); this->~Read_into_stringActorState(); static_cast<Read_into_stringActor*>(this)->destroy(); return 0; }
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_into_stringActor*>(this)->SAV< int >::value()) int(len);
			this->~Read_into_stringActorState();
			static_cast<Read_into_stringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = conn->onReadable();
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 1;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 0, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 2;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 1, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(0, TaskPriority::ReadSocket);
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_into_stringActor*>(this)->actor_wait_state = 2;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_into_stringActor, 1, Void >*>(static_cast<Read_into_stringActor*>(this)));
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int maxlen;
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_into_string()
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_into_stringActor final : public Actor<int>, public ActorCallback< Read_into_stringActor, 0, Void >, public ActorCallback< Read_into_stringActor, 1, Void >, public FastAllocated<Read_into_stringActor>, public Read_into_stringActorState<Read_into_stringActor> {
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_into_stringActor>::operator new;
	using FastAllocated<Read_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_into_stringActor, 0, Void >;
friend struct ActorCallback< Read_into_stringActor, 1, Void >;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_into_stringActor(Reference<IConnection> const& conn,std::string* const& buf,int const& maxlen) 
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<int> read_into_string( Reference<IConnection> const& conn, std::string* const& buf, int const& maxlen ) {
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<int>(new Read_into_stringActor(conn, buf, maxlen));
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Returns the position of delim within buf, relative to pos.  If delim is not found, continues to read from conn until
// either it is found or the connection ends, at which point connection_failed is thrown and buf contains
// everything that was read up to that point.
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_delimited_into_string()
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_delimited_into_stringActor>
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_delimited_into_stringActorState {
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_delimited_into_stringActorState(Reference<IConnection> const& conn,const char* const& delim,std::string* const& buf,size_t const& pos) 
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   delim(delim),
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   sPos(pos),
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   lookBack(strlen(delim) - 1)
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			ASSERT(lookBack >= 0);
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		size_t endPos = buf->find(delim, sPos);
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (endPos != std::string::npos)
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_delimited_into_stringActor*>(this)->SAV<size_t>::futures) { (void)(endPos - pos); this->~Read_delimited_into_stringActorState(); static_cast<Read_delimited_into_stringActor*>(this)->destroy(); return 0; }
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_delimited_into_stringActor*>(this)->SAV< size_t >::value()) size_t(endPos - pos);
			this->~Read_delimited_into_stringActorState();
			static_cast<Read_delimited_into_stringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sPos >= lookBack)
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			sPos -= lookBack;
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = success(read_into_string(conn, buf, FLOW_KNOBS->HTTP_READ_SIZE));
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_delimited_into_stringActor*>(this)->actor_wait_state = 1;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_delimited_into_stringActor, 0, Void >*>(static_cast<Read_delimited_into_stringActor*>(this)));
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	const char* delim;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t sPos;
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int lookBack;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_delimited_into_string()
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_delimited_into_stringActor final : public Actor<size_t>, public ActorCallback< Read_delimited_into_stringActor, 0, Void >, public FastAllocated<Read_delimited_into_stringActor>, public Read_delimited_into_stringActorState<Read_delimited_into_stringActor> {
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_delimited_into_stringActor>::operator new;
	using FastAllocated<Read_delimited_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<size_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_delimited_into_stringActor, 0, Void >;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_delimited_into_stringActor(Reference<IConnection> const& conn,const char* const& delim,std::string* const& buf,size_t const& pos) 
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<size_t> read_delimited_into_string( Reference<IConnection> const& conn, const char* const& delim, std::string* const& buf, size_t const& pos ) {
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<size_t>(new Read_delimited_into_stringActor(conn, delim, buf, pos));
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Reads from conn (as needed) until there are at least len bytes starting at pos in buf
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_fixed_into_string()
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_fixed_into_stringActor>
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_fixed_into_stringActorState {
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_fixed_into_stringActorState(Reference<IConnection> const& conn,int const& len,std::string* const& buf,size_t const& pos) 
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   len(len),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos),
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   stop_size(pos + len)
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_fixed_into_stringActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_fixed_into_stringActorState(); static_cast<Read_fixed_into_stringActor*>(this)->destroy(); return 0; }
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!(buf->size() < stop_size))
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = success(read_into_string(conn, buf, FLOW_KNOBS->HTTP_READ_SIZE));
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_fixed_into_stringActor*>(this)->actor_wait_state = 1;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_fixed_into_stringActor, 0, Void >*>(static_cast<Read_fixed_into_stringActor*>(this)));
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int len;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int stop_size;
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_fixed_into_string()
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_fixed_into_stringActor final : public Actor<Void>, public ActorCallback< Read_fixed_into_stringActor, 0, Void >, public FastAllocated<Read_fixed_into_stringActor>, public Read_fixed_into_stringActorState<Read_fixed_into_stringActor> {
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_fixed_into_stringActor>::operator new;
	using FastAllocated<Read_fixed_into_stringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_fixed_into_stringActor, 0, Void >;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_fixed_into_stringActor(Reference<IConnection> const& conn,int const& len,std::string* const& buf,size_t const& pos) 
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_fixed_into_string( Reference<IConnection> const& conn, int const& len, std::string* const& buf, size_t const& pos ) {
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_fixed_into_stringActor(conn, len, buf, pos));
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_response_headers()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_http_response_headersActor>
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_response_headersActorState {
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_response_headersActorState(Reference<IConnection> const& conn,Headers* const& headers,std::string* const& buf,size_t* const& pos) 
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   headers(headers),
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(buf),
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(pos)
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", buf, *pos);
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_response_headersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Read_http_response_headersActor*>(this)->actor_wait_state = 1;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_response_headersActor, 0, size_t >*>(static_cast<Read_http_response_headersActor*>(this)));
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(size_t const& lineLen,int loopDepth) 
	{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen == 0)
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += 2;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_response_headersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_response_headersActorState(); static_cast<Read_http_response_headersActor*>(this)->destroy(); return 0; }
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_response_headersActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_response_headersActorState();
			static_cast<Read_http_response_headersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int nameEnd = -1, valueStart = -1, valueEnd = -1;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = -1;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string name, value;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^:]%n:%*[ \t]%n", &nameEnd, &valueStart) >= 0 && valueStart > 0)
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			name = std::string(buf->substr(*pos, nameEnd));
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += valueStart;
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^\r]%n%*1[\r]%*1[\n]%n", &valueEnd, &len) >= 0 && len > 0)
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			value = std::string(buf->substr(*pos, valueEnd));
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += len;
															#line 1050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (sscanf(buf->c_str() + *pos, "%*1[\r]%*1[\n]%n", &len) >= 0 && len > 0)
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				*pos += len;
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
			else
			{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		(*headers)[name] = value;
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen == 0)
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += 2;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_response_headersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_response_headersActorState(); static_cast<Read_http_response_headersActor*>(this)->destroy(); return 0; }
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_response_headersActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_response_headersActorState();
			static_cast<Read_http_response_headersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int nameEnd = -1, valueStart = -1, valueEnd = -1;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = -1;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		std::string name, value;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^:]%n:%*[ \t]%n", &nameEnd, &valueStart) >= 0 && valueStart > 0)
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			name = std::string(buf->substr(*pos, nameEnd));
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += valueStart;
															#line 1106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (sscanf(buf->c_str() + *pos, "%*[^\r]%n%*1[\r]%*1[\n]%n", &valueEnd, &len) >= 0 && len > 0)
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			value = std::string(buf->substr(*pos, valueEnd));
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pos += len;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (sscanf(buf->c_str() + *pos, "%*1[\r]%*1[\n]%n", &len) >= 0 && len > 0)
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				*pos += len;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
			else
			{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		(*headers)[name] = value;
															#line 1143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Headers* headers;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string* buf;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t* pos;
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_response_headers()
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_response_headersActor final : public Actor<Void>, public ActorCallback< Read_http_response_headersActor, 0, size_t >, public FastAllocated<Read_http_response_headersActor>, public Read_http_response_headersActorState<Read_http_response_headersActor> {
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<Read_http_response_headersActor>::operator new;
	using FastAllocated<Read_http_response_headersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_http_response_headersActor, 0, size_t >;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_response_headersActor(Reference<IConnection> const& conn,Headers* const& headers,std::string* const& buf,size_t* const& pos) 
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_response_headers( Reference<IConnection> const& conn, Headers* const& headers, std::string* const& buf, size_t* const& pos ) {
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_response_headersActor(conn, headers, buf, pos));
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

// Reads an HTTP response from a network connection
// If the connection fails while being read the exception will emitted
// If the response is not parsable or complete in some way, http_bad_response will be thrown
															#line 1270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via read_http_response()
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class Read_http_responseActor>
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_responseActorState {
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_responseActorState(Reference<HTTP::Response> const& r,Reference<IConnection> const& conn,bool const& header_only) 
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : r(r),
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   conn(conn),
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   header_only(header_only),
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   buf(),
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pos(0)
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_0 = read_delimited_into_string(conn, "\r\n", &buf, pos);
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 1;
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 0, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int reachedEnd = -1;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sscanf(buf.c_str() + pos, "HTTP/%f %d%n", &r->version, &r->code, &reachedEnd);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (reachedEnd < 0)
															#line 1340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->headers.clear();
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = read_http_response_headers(conn, &r->headers, &buf, &pos);
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 2;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 1, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(size_t && lineLen,int loopDepth) 
	{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int reachedEnd = -1;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sscanf(buf.c_str() + pos, "HTTP/%f %d%n", &r->version, &r->code, &reachedEnd);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (reachedEnd < 0)
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += lineLen + 2;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->headers.clear();
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = read_http_response_headers(conn, &r->headers, &buf, &pos);
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 2;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 1, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		auto i = r->headers.find("Content-Length");
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = strtoll(i->second.c_str(), NULL, 10);
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = -1;
															#line 1475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		transferEncoding = std::string();
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		i = r->headers.find("Transfer-Encoding");
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			transferEncoding = i->second;
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.clear();
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (header_only && pos == buf.size())
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_responseActorState();
			static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->contentLen >= 0)
															#line 1505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->content = buf.substr(pos);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			pos = 0;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_2 = read_fixed_into_string(conn, r->contentLen, &r->content, pos);
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 3;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 2, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (transferEncoding == "chunked")
															#line 1527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				r->content = buf.substr(pos);
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				pos = 0;
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				;
															#line 1535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				loopDepth = a_body1cont2loopHead1(loopDepth);
			}
			else
			{
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		auto i = r->headers.find("Content-Length");
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = strtoll(i->second.c_str(), NULL, 10);
															#line 1558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		else
		{
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->contentLen = -1;
															#line 1564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		transferEncoding = std::string();
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		i = r->headers.find("Transfer-Encoding");
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (i != r->headers.end())
															#line 1572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			transferEncoding = i->second;
															#line 1576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.clear();
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (header_only && pos == buf.size())
															#line 1582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~Read_http_responseActorState();
			static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->contentLen >= 0)
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->content = buf.substr(pos);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			pos = 0;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_2 = read_fixed_into_string(conn, r->contentLen, &r->content, pos);
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 3;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 2, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (transferEncoding == "chunked")
															#line 1616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				r->content = buf.substr(pos);
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				pos = 0;
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				;
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				loopDepth = a_body1cont2loopHead1(loopDepth);
			}
			else
			{
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() > 0)
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r->code == 206 && CLIENT_KNOBS->HTTP_RESPONSE_SKIP_VERIFY_CHECKSUM_FOR_PARTIAL_CONTENT)
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~Read_http_responseActorState();
				static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!r->verifyMD5(false))
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<Read_http_responseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Read_http_responseActorState(); static_cast<Read_http_responseActor*>(this)->destroy(); return 0; }
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<Read_http_responseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Read_http_responseActorState();
		static_cast<Read_http_responseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() != r->contentLen)
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (r->content.size() != r->contentLen)
															#line 1755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->contentLen = pos;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_6 = read_http_response_headers(conn, &r->headers, &r->content, &pos);
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont13when1(__when_expr_6.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 7;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 6, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_3 = read_delimited_into_string(conn, "\r\n", &r->content, pos);
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 4;
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 3, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<size_t> __when_expr_5 = read_delimited_into_string(conn, "\r\n", &r->content, pos);
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont1when1(__when_expr_5.get(), loopDepth); };
			static_cast<Read_http_responseActor*>(this)->actor_wait_state = 6;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 5, size_t >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2(size_t const& lineLen,int loopDepth) 
	{
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		chunkLen = strtol(r->content.substr(pos, lineLen).c_str(), nullptr, 16);
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, lineLen + 2);
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (chunkLen == 0)
															#line 1916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_4 = read_fixed_into_string(conn, chunkLen, &r->content, pos);
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 5;
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 4, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2loopBody1cont2(size_t && lineLen,int loopDepth) 
	{
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		chunkLen = strtol(r->content.substr(pos, lineLen).c_str(), nullptr, 16);
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, lineLen + 2);
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (chunkLen == 0)
															#line 1942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_4 = read_fixed_into_string(conn, chunkLen, &r->content, pos);
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<Read_http_responseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
		static_cast<Read_http_responseActor*>(this)->actor_wait_state = 5;
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< Read_http_responseActor, 4, Void >*>(static_cast<Read_http_responseActor*>(this)));
															#line 1955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += chunkLen;
															#line 2027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pos += chunkLen;
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen != 0)
															#line 2114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 2118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, 2);
															#line 2122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont7(size_t && lineLen,int loopDepth) 
	{
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (lineLen != 0)
															#line 2131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), std::max(0, loopDepth - 1));
															#line 2135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(pos, 2);
															#line 2139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pos != r->content.size())
															#line 2211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(r->contentLen);
															#line 2219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont13cont1(Void && _,int loopDepth) 
	{
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pos != r->content.size())
															#line 2228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(http_bad_response(), loopDepth);
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r->content.erase(r->contentLen);
															#line 2236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::Response> r;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	bool header_only;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string buf;
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	size_t pos;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string transferEncoding;
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int chunkLen;
															#line 2318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via read_http_response()
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class Read_http_responseActor final : public Actor<Void>, public ActorCallback< Read_http_responseActor, 0, size_t >, public ActorCallback< Read_http_responseActor, 1, Void >, public ActorCallback< Read_http_responseActor, 2, Void >, public ActorCallback< Read_http_responseActor, 3, size_t >, public ActorCallback< Read_http_responseActor, 4, Void >, public ActorCallback< Read_http_responseActor, 5, size_t >, public ActorCallback< Read_http_responseActor, 6, Void >, public FastAllocated<Read_http_responseActor>, public Read_http_responseActorState<Read_http_responseActor> {
															#line 2323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Read_http_responseActor(Reference<HTTP::Response> const& r,Reference<IConnection> const& conn,bool const& header_only) 
															#line 2340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> read_http_response( Reference<HTTP::Response> const& r, Reference<IConnection> const& conn, bool const& header_only ) {
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new Read_http_responseActor(r, conn, header_only));
															#line 2373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

Future<Void> HTTP::Response::read(Reference<IConnection> conn, bool header_only) {
	return read_http_response(Reference<HTTP::Response>::addRef(this), conn, header_only);
}

// Do a request, get a Response.
// Request content is provided as UnsentPacketQueue *pContent which will be depleted as bytes are sent but the queue
// itself must live for the life of this actor and be destroyed by the caller
// TODO:  pSent is very hackish, do something better.
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via doRequest()
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class DoRequestActor>
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class DoRequestActorState {
															#line 2392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	DoRequestActorState(Reference<IConnection> const& conn,std::string const& verb,std::string const& resource,HTTP::Headers const& headers,UnsentPacketQueue* const& pContent,int const& contentLen,Reference<IRateControl> const& sendRate,int64_t* const& pSent,Reference<IRateControl> const& recvRate,std::string const& requestIDHeader) 
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   verb(verb),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   resource(resource),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   headers(headers),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pContent(pContent),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   contentLen(contentLen),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   sendRate(sendRate),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   pSent(pSent),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   recvRate(recvRate),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   requestIDHeader(requestIDHeader),
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   event(SevDebug, "HTTPRequest"),
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   empty()
															#line 2421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (pContent == nullptr)
															#line 2436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				pContent = &empty;
															#line 2440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (requestIDHeader.empty())
															#line 2444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				requestIDHeader = FLOW_KNOBS->HTTP_REQUEST_ID_HEADER;
															#line 2448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			earlyResponse = false;
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			total_sent = 0;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			send_start = double();
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("DebugID", conn->getDebugID());
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RemoteAddress", conn->getPeerAddress());
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("Verb", verb);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("Resource", resource);
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestContentLen", contentLen);
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			try {
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				requestID = std::string();
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (!requestIDHeader.empty())
															#line 2472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = deterministicRandom()->randomUniqueID().toString();
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(20, "-");
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(16, "-");
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(12, "-");
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					requestID = requestID.insert(8, "-");
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					headers[requestIDHeader] = requestID;
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					event.detail("RequestIDSent", requestID);
															#line 2488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				}
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				PacketBuffer* pFirst = PacketBuffer::create();
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				PacketBuffer* pLast = writeRequestHeader(verb, resource, headers, pFirst);
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				pContent->prependWriteBuffer(pFirst, pLast);
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 1)
															#line 2498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					printf("[%s] HTTP starting %s %s ContentLen:%d\n", conn->getDebugID().toString().c_str(), verb.c_str(), resource.c_str(), contentLen);
															#line 2502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				}
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 2506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					for( auto h : headers ) {
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
						printf("Request Header: %s: %s\n", h.first.c_str(), h.second.c_str());
															#line 2512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
					}
				}
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				r = Reference<HTTP::Response>(new HTTP::Response());
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				responseReading = r->read(conn, verb == "HEAD" || verb == "DELETE" || verb == "CONNECT");
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				send_start = timer();
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				;
															#line 2523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			double elapsed = timer() - send_start;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0 && e.code() != error_code_http_bad_request_id)
															#line 2555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				printf("[%s] HTTP *ERROR*=%s early=%d, time=%fs %s %s contentLen=%d [%d out]\n", conn->getDebugID().toString().c_str(), e.name(), earlyResponse, elapsed, verb.c_str(), resource.c_str(), contentLen, total_sent);
															#line 2559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.errorUnsuppressed(e);
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 2565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_3 = responseReading;
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 4;
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 3, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (responseReading.isReady())
															#line 2602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			conn->close();
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			r->headers["Connection"] = "close";
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			earlyResponse = true;
															#line 2610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		trySend = FLOW_KNOBS->HTTP_SEND_SIZE;
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_0 = sendRate->getAllowance(trySend);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 1;
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 0, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(pContent->getUnsent(), trySend);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pSent != nullptr)
															#line 2648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pSent += len;
															#line 2652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sendRate->returnUnused(trySend - len);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		total_sent += len;
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pContent->sent(len);
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pContent->empty())
															#line 2662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 2;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 1, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		int len = conn->write(pContent->getUnsent(), trySend);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pSent != nullptr)
															#line 2686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			*pSent += len;
															#line 2690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		sendRate->returnUnused(trySend - len);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		total_sent += len;
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		pContent->sent(len);
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (pContent->empty())
															#line 2700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_1 = conn->onWritable();
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 2;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 1, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 3;
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 2, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = yield(TaskPriority::WriteSocket);
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<DoRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoRequestActor*>(this)->actor_wait_state = 3;
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequestActor, 2, Void >*>(static_cast<DoRequestActor*>(this)));
															#line 2808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		double elapsed = timer() - send_start;
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseCode", r->code);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseContentLen", r->contentLen);
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("Elapsed", elapsed);
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		Optional<Error> err;
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!requestIDHeader.empty())
															#line 2965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			std::string responseID;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			auto iid = r->headers.find(requestIDHeader);
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (iid != r->headers.end())
															#line 2973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				responseID = iid->second;
															#line 2977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestIDReceived", responseID);
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			bool serverError = r->code >= 500 && r->code < 600;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (requestID != responseID && (!serverError || !responseID.empty()))
															#line 2985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				err = http_bad_request_id();
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				TraceEvent(SevError, "HTTPRequestFailedIDMismatch") .error(err.get()) .detail("DebugID", conn->getDebugID()) .detail("RemoteAddress", conn->getPeerAddress()) .detail("Verb", verb) .detail("Resource", resource) .detail("RequestContentLen", contentLen) .detail("ResponseCode", r->code) .detail("ResponseContentLen", r->contentLen) .detail("RequestIDSent", requestID) .detail("RequestIDReceived", responseID);
															#line 2991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0)
															#line 2996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{0}] HTTP {1}code={2} early={3}, time={4} {5} {6} contentLen={7} [{8} out, response content " "len {9}]\n", conn->getDebugID().toString(), (err.present() ? format("*ERROR*=%s ", err.get().name()).c_str() : ""), r->code, earlyResponse, elapsed, verb, resource, contentLen, total_sent, r->contentLen);
															#line 3000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 3004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			printf("[%s] HTTP RESPONSE:  %s %s\n%s\n", conn->getDebugID().toString().c_str(), verb.c_str(), resource.c_str(), r->toString().c_str());
															#line 3008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 3012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch2(err.get(), loopDepth);
															#line 3016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<DoRequestActor*>(this)->SAV<Reference<HTTP::Response>>::futures) { (void)(r); this->~DoRequestActorState(); static_cast<DoRequestActor*>(this)->destroy(); return 0; }
															#line 3020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<DoRequestActor*>(this)->SAV< Reference<HTTP::Response> >::value()) Reference<HTTP::Response>(std::move(r)); // state_var_RVO
		this->~DoRequestActorState();
		static_cast<DoRequestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		double elapsed = timer() - send_start;
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseCode", r->code);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ResponseContentLen", r->contentLen);
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("Elapsed", elapsed);
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		Optional<Error> err;
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!requestIDHeader.empty())
															#line 3042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			std::string responseID;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			auto iid = r->headers.find(requestIDHeader);
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (iid != r->headers.end())
															#line 3050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				responseID = iid->second;
															#line 3054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RequestIDReceived", responseID);
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			bool serverError = r->code >= 500 && r->code < 600;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (requestID != responseID && (!serverError || !responseID.empty()))
															#line 3062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				err = http_bad_request_id();
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				TraceEvent(SevError, "HTTPRequestFailedIDMismatch") .error(err.get()) .detail("DebugID", conn->getDebugID()) .detail("RemoteAddress", conn->getPeerAddress()) .detail("Verb", verb) .detail("Resource", resource) .detail("RequestContentLen", contentLen) .detail("ResponseCode", r->code) .detail("ResponseContentLen", r->contentLen) .detail("RequestIDSent", requestID) .detail("RequestIDReceived", responseID);
															#line 3068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
		}
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 0)
															#line 3073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			fmt::print("[{0}] HTTP {1}code={2} early={3}, time={4} {5} {6} contentLen={7} [{8} out, response content " "len {9}]\n", conn->getDebugID().toString(), (err.present() ? format("*ERROR*=%s ", err.get().name()).c_str() : ""), r->code, earlyResponse, elapsed, verb, resource, contentLen, total_sent, r->contentLen);
															#line 3077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (FLOW_KNOBS->HTTP_VERBOSE_LEVEL > 2)
															#line 3081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			printf("[%s] HTTP RESPONSE:  %s %s\n%s\n", conn->getDebugID().toString().c_str(), verb.c_str(), resource.c_str(), r->toString().c_str());
															#line 3085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch2(err.get(), loopDepth);
															#line 3093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<DoRequestActor*>(this)->SAV<Reference<HTTP::Response>>::futures) { (void)(r); this->~DoRequestActorState(); static_cast<DoRequestActor*>(this)->destroy(); return 0; }
															#line 3097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string verb;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string resource;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	HTTP::Headers headers;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	UnsentPacketQueue* pContent;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int contentLen;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> sendRate;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int64_t* pSent;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> recvRate;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string requestIDHeader;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	TraceEvent event;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	UnsentPacketQueue empty;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	bool earlyResponse;
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int total_sent;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	double send_start;
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string requestID;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<HTTP::Response> r;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Future<Void> responseReading;
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int trySend;
															#line 3206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via doRequest()
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class DoRequestActor final : public Actor<Reference<HTTP::Response>>, public ActorCallback< DoRequestActor, 0, Void >, public ActorCallback< DoRequestActor, 1, Void >, public ActorCallback< DoRequestActor, 2, Void >, public ActorCallback< DoRequestActor, 3, Void >, public FastAllocated<DoRequestActor>, public DoRequestActorState<DoRequestActor> {
															#line 3211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	DoRequestActor(Reference<IConnection> const& conn,std::string const& verb,std::string const& resource,HTTP::Headers const& headers,UnsentPacketQueue* const& pContent,int const& contentLen,Reference<IRateControl> const& sendRate,int64_t* const& pSent,Reference<IRateControl> const& recvRate,std::string const& requestIDHeader) 
															#line 3225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::Response>> doRequest( Reference<IConnection> const& conn, std::string const& verb, std::string const& resource, HTTP::Headers const& headers, UnsentPacketQueue* const& pContent, int const& contentLen, Reference<IRateControl> const& sendRate, int64_t* const& pSent, Reference<IRateControl> const& recvRate, std::string const& requestIDHeader ) {
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Reference<HTTP::Response>>(new DoRequestActor(conn, verb, resource, headers, pContent, contentLen, sendRate, pSent, recvRate, requestIDHeader));
															#line 3255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

															#line 3260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via sendProxyConnectRequest()
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class SendProxyConnectRequestActor>
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class SendProxyConnectRequestActorState {
															#line 3266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	SendProxyConnectRequestActorState(Reference<IConnection> const& conn,std::string const& remoteHost,std::string const& remoteService) 
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : conn(conn),
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   remoteHost(remoteHost),
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   remoteService(remoteService),
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   headers()
															#line 3279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			headers["Host"] = remoteHost + ":" + remoteService;
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			headers["Accept"] = "application/xml";
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			headers["Proxy-Connection"] = "Keep-Alive";
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			requestTimeout = 60;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			maxTries = FLOW_KNOBS->HTTP_CONNECT_TRIES;
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			thisTry = 1;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			nextRetryDelay = 2.0;
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			sendReceiveRate = makeReference<Unlimited>();
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			bytes_sent = 0;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			;
															#line 3312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		err = Optional<Error>();
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r = Reference<Response>();
															#line 3344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		try {
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Reference<Response>> __when_expr_0 = timeoutError(doRequest(conn, "CONNECT", remoteHost + ":" + remoteService, headers, nullptr, 0, sendReceiveRate, &bytes_sent, sendReceiveRate), requestTimeout);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 1;
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >*>(static_cast<SendProxyConnectRequestActor*>(this)));
															#line 3355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!err.present() && r->code == 200)
															#line 3370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (!static_cast<SendProxyConnectRequestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendProxyConnectRequestActorState(); static_cast<SendProxyConnectRequestActor*>(this)->destroy(); return 0; }
															#line 3374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			new (&static_cast<SendProxyConnectRequestActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~SendProxyConnectRequestActorState();
			static_cast<SendProxyConnectRequestActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		bool retryable = err.present() || r->code == 500 || r->code == 502 || r->code == 503 || r->code == 429;
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		retryable = retryable && (thisTry < maxTries);
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		TraceEvent event(SevWarn, retryable ? "ProxyConnectCommandFailedRetryable" : "ProxyConnectCommandFailed");
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (err.present())
															#line 3388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.errorUnsuppressed(err.get());
															#line 3392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.suppressFor(60);
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!err.present())
															#line 3398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("ResponseCode", r->code);
															#line 3402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		event.detail("ThisTry", thisTry);
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!r || r->code != 429)
															#line 3408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			++thisTry;
															#line 3412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		double delay = nextRetryDelay;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		nextRetryDelay = std::min(nextRetryDelay * 2, 60.0);
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (retryable)
															#line 3420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		{
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r)
															#line 3424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				auto iRetryAfter = r->headers.find("Retry-After");
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				if (iRetryAfter != r->headers.end())
															#line 3430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				{
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					event.detail("RetryAfterHeader", iRetryAfter->second);
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					char* pEnd;
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					double retryAfter = strtod(iRetryAfter->second.c_str(), &pEnd);
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					if (*pEnd)
															#line 3440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
					{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
						retryAfter = 300;
															#line 3444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
					}
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
					delay = std::max(delay, retryAfter);
															#line 3448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
				}
			}
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			event.detail("RetryDelay", delay);
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delay(delay);
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SendProxyConnectRequestActor*>(this)->actor_wait_state = 2;
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SendProxyConnectRequestActor, 1, Void >*>(static_cast<SendProxyConnectRequestActor*>(this)));
															#line 3462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r && r->code == 406)
															#line 3469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_not_accepted(), std::max(0, loopDepth - 1));
															#line 3473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (r && r->code == 401)
															#line 3477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(http_auth_failed(), std::max(0, loopDepth - 1));
															#line 3481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 1));
															#line 3485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 3495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			{
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 3499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			}
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			err = e;
															#line 3503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r = _r;
															#line 3518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Reference<Response> && _r,int loopDepth) 
	{
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		r = _r;
															#line 3527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> conn;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteHost;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteService;
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Headers headers;
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int requestTimeout;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int maxTries;
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int thisTry;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	double nextRetryDelay;
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	int64_t bytes_sent;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Optional<Error> err;
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<Response> r;
															#line 3713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via sendProxyConnectRequest()
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class SendProxyConnectRequestActor final : public Actor<Void>, public ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >, public ActorCallback< SendProxyConnectRequestActor, 1, Void >, public FastAllocated<SendProxyConnectRequestActor>, public SendProxyConnectRequestActorState<SendProxyConnectRequestActor> {
															#line 3718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
	using FastAllocated<SendProxyConnectRequestActor>::operator new;
	using FastAllocated<SendProxyConnectRequestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendProxyConnectRequestActor, 0, Reference<Response> >;
friend struct ActorCallback< SendProxyConnectRequestActor, 1, Void >;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	SendProxyConnectRequestActor(Reference<IConnection> const& conn,std::string const& remoteHost,std::string const& remoteService) 
															#line 3730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Void> sendProxyConnectRequest( Reference<IConnection> const& conn, std::string const& remoteHost, std::string const& remoteService ) {
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Void>(new SendProxyConnectRequestActor(conn, remoteHost, remoteService));
															#line 3758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

															#line 3763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
// This generated class is to be used only via proxyConnectImpl()
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
template <class ProxyConnectImplActor>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class ProxyConnectImplActorState {
															#line 3769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
public:
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	ProxyConnectImplActorState(std::string const& remoteHost,std::string const& remoteService,std::string const& proxyHost,std::string const& proxyService) 
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		 : remoteHost(remoteHost),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   remoteService(remoteService),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   proxyHost(proxyHost),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		   proxyService(proxyService)
															#line 3782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			StrictFuture<NetworkAddress> __when_expr_0 = map(INetworkConnections::net()->resolveTCPEndpoint(remoteHost, remoteService), [=](std::vector<NetworkAddress> const& addresses) -> NetworkAddress { NetworkAddress addr = addresses[deterministicRandom()->randomInt(0, addresses.size())]; addr.fromHostname = true; addr.flags = NetworkAddress::FLAG_TLS; return addr; });
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 1;
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 3804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_1 = INetworkConnections::net()->connect(proxyHost, proxyService);
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 2;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 3834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(NetworkAddress const& __remoteEndpoint,int loopDepth) 
	{
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		remoteEndpoint = __remoteEndpoint;
															#line 3843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Void> __when_expr_2 = sendProxyConnectRequest(connection, remoteHost, remoteService);
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 3;
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 2, Void >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 3917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IConnection> const& __connection,int loopDepth) 
	{
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		connection = __connection;
															#line 3926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		boost::asio::ip::tcp::socket socket = std::move(connection->getSocket());
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(remoteEndpoint, &socket);
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 4;
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		boost::asio::ip::tcp::socket socket = std::move(connection->getSocket());
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		StrictFuture<Reference<IConnection>> __when_expr_3 = INetworkConnections::net()->connect(remoteEndpoint, &socket);
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (static_cast<ProxyConnectImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<ProxyConnectImplActor*>(this)->actor_wait_state = 4;
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >*>(static_cast<ProxyConnectImplActor*>(this)));
															#line 4020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ProxyConnectImplActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(remoteConnection); this->~ProxyConnectImplActorState(); static_cast<ProxyConnectImplActor*>(this)->destroy(); return 0; }
															#line 4092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
		new (&static_cast<ProxyConnectImplActor*>(this)->SAV< Reference<IConnection> >::value()) Reference<IConnection>(remoteConnection);
		this->~ProxyConnectImplActorState();
		static_cast<ProxyConnectImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Reference<IConnection> && remoteConnection,int loopDepth) 
	{
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
		if (!static_cast<ProxyConnectImplActor*>(this)->SAV<Reference<IConnection>>::futures) { (void)(remoteConnection); this->~ProxyConnectImplActorState(); static_cast<ProxyConnectImplActor*>(this)->destroy(); return 0; }
															#line 4104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteHost;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string remoteService;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string proxyHost;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	std::string proxyService;
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	NetworkAddress remoteEndpoint;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	Reference<IConnection> connection;
															#line 4187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
};
// This generated class is to be used only via proxyConnectImpl()
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
class ProxyConnectImplActor final : public Actor<Reference<IConnection>>, public ActorCallback< ProxyConnectImplActor, 0, NetworkAddress >, public ActorCallback< ProxyConnectImplActor, 1, Reference<IConnection> >, public ActorCallback< ProxyConnectImplActor, 2, Void >, public ActorCallback< ProxyConnectImplActor, 3, Reference<IConnection> >, public FastAllocated<ProxyConnectImplActor>, public ProxyConnectImplActorState<ProxyConnectImplActor> {
															#line 4192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	ProxyConnectImplActor(std::string const& remoteHost,std::string const& remoteService,std::string const& proxyHost,std::string const& proxyService) 
															#line 4206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
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
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
[[nodiscard]] Future<Reference<IConnection>> proxyConnectImpl( std::string const& remoteHost, std::string const& remoteService, std::string const& proxyHost, std::string const& proxyService ) {
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"
	return Future<Reference<IConnection>>(new ProxyConnectImplActor(remoteHost, remoteService, proxyHost, proxyService));
															#line 4236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.g.cpp"
}

#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HTTP.actor.cpp"

Future<Reference<IConnection>> proxyConnect(const std::string& remoteHost,
                                            const std::string& remoteService,
                                            const std::string& proxyHost,
                                            const std::string& proxyService) {
	return proxyConnectImpl(remoteHost, remoteService, proxyHost, proxyService);
}

} // namespace HTTP
