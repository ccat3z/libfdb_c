#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
/*
 * AsyncFileS3BlobStore.actor.h
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

#pragma once

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FDBRPC_ASYNCFILEBLOBSTORE_ACTOR_G_H)
#define FDBRPC_ASYNCFILEBLOBSTORE_ACTOR_G_H
#include "fdbclient/AsyncFileS3BlobStore.actor.g.h"
#elif !defined(FDBRPC_ASYNCFILES3BLOBSTORE_ACTOR_H)
#define FDBRPC_ASYNCFILES3BLOBSTORE_ACTOR_H

#include <sstream>
#include <time.h>

#include "fdbrpc/IAsyncFile.h"
#include "flow/serialize.h"
#include "flow/Net2Packet.h"
#include "fdbrpc/IRateControl.h"
#include "fdbclient/S3BlobStore.h"
#include "openssl/md5.h"
#include "fdbrpc/libb64/encode.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
namespace {
// This generated class is to be used only via joinErrorGroup()
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <typename T, class JoinErrorGroupActor>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class JoinErrorGroupActorState {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	JoinErrorGroupActorState(Future<T> const& f,Promise<Void> const& p) 
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		 : f(f),
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		   p(p)
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
	{
		fdb_probe_actor_create("joinErrorGroup", reinterpret_cast<unsigned long>(this));

	}
	~JoinErrorGroupActorState() 
	{
		fdb_probe_actor_destroy("joinErrorGroup", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				StrictFuture<Void> __when_expr_0 = success(f) || p.getFuture();
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				if (static_cast<JoinErrorGroupActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<JoinErrorGroupActor*>(this)->actor_wait_state = 1;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< JoinErrorGroupActor, 0, Void >*>(static_cast<JoinErrorGroupActor*>(this)));
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
		this->~JoinErrorGroupActorState();
		static_cast<JoinErrorGroupActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			if (p.canBeSet())
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
			{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				p.sendError(e);
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
			}
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<JoinErrorGroupActor*>(this)->SAV<T>::futures) { (void)(f.get()); this->~JoinErrorGroupActorState(); static_cast<JoinErrorGroupActor*>(this)->destroy(); return 0; }
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<JoinErrorGroupActor*>(this)->SAV< T >::value()) T(f.get());
		this->~JoinErrorGroupActorState();
		static_cast<JoinErrorGroupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<JoinErrorGroupActor*>(this)->SAV<T>::futures) { (void)(f.get()); this->~JoinErrorGroupActorState(); static_cast<JoinErrorGroupActor*>(this)->destroy(); return 0; }
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<JoinErrorGroupActor*>(this)->SAV< T >::value()) T(f.get());
		this->~JoinErrorGroupActorState();
		static_cast<JoinErrorGroupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<JoinErrorGroupActor*>(this)->actor_wait_state > 0) static_cast<JoinErrorGroupActor*>(this)->actor_wait_state = 0;
		static_cast<JoinErrorGroupActor*>(this)->ActorCallback< JoinErrorGroupActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< JoinErrorGroupActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("joinErrorGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("joinErrorGroup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< JoinErrorGroupActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("joinErrorGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("joinErrorGroup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< JoinErrorGroupActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("joinErrorGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("joinErrorGroup", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Future<T> f;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Promise<Void> p;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
};
// This generated class is to be used only via joinErrorGroup()
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <typename T>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class JoinErrorGroupActor final : public Actor<T>, public ActorCallback< JoinErrorGroupActor<T>, 0, Void >, public FastAllocated<JoinErrorGroupActor<T>>, public JoinErrorGroupActorState<T, JoinErrorGroupActor<T>> {
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
	using FastAllocated<JoinErrorGroupActor<T>>::operator new;
	using FastAllocated<JoinErrorGroupActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< JoinErrorGroupActor<T>, 0, Void >;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	JoinErrorGroupActor(Future<T> const& f,Promise<Void> const& p) 
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		 : Actor<T>(),
		   JoinErrorGroupActorState<T, JoinErrorGroupActor<T>>(f, p)
	{
		fdb_probe_actor_enter("joinErrorGroup", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("joinErrorGroup");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("joinErrorGroup", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< JoinErrorGroupActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <typename T>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
[[nodiscard]] static Future<T> joinErrorGroup( Future<T> const& f, Promise<Void> const& p ) {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	return Future<T>(new JoinErrorGroupActor<T>(f, p));
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
}

#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
// This class represents a write-only file that lives in an S3-style blob store.  It writes using the REST API,
// using multi-part upload and beginning to transfer each part as soon as it is large enough.
// All write operations file operations must be sequential and contiguous.
// Limits on part sizes, upload speed, and concurrent uploads are taken from the S3BlobStoreEndpoint being used.
class AsyncFileS3BlobStoreWrite final : public IAsyncFile, public ReferenceCounted<AsyncFileS3BlobStoreWrite> {
public:
	void addref() override { ReferenceCounted<AsyncFileS3BlobStoreWrite>::addref(); }
	void delref() override { ReferenceCounted<AsyncFileS3BlobStoreWrite>::delref(); }

	struct Part : ReferenceCounted<Part> {
		Part(int n, int minSize)
		  : number(n), writer(content.getWriteBuffer(minSize), nullptr, Unversioned()), length(0) {
			etag = std::string();
			::MD5_Init(&content_md5_buf);
		}
		virtual ~Part() { etag.cancel(); }
		Future<std::string> etag;
		int number;
		UnsentPacketQueue content;
		std::string md5string;
		PacketWriter writer;
		int length;
		void write(const uint8_t* buf, int len) {
			writer.serializeBytes(buf, len);
			::MD5_Update(&content_md5_buf, buf, len);
			length += len;
		}
		// MD5 sum can only be finalized once, further calls will do nothing so new writes will be reflected in the sum.
		void finalizeMD5() {
			if (md5string.empty()) {
				std::string sumBytes;
				sumBytes.resize(16);
				::MD5_Final((unsigned char*)sumBytes.data(), &content_md5_buf);
				md5string = base64::encoder::from_string(sumBytes);
				md5string.resize(md5string.size() - 1);
			}
		}

	private:
		MD5_CTX content_md5_buf;
	};

	Future<int> read(void* data, int length, int64_t offset) override { throw file_not_readable(); }

																#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
// This generated class is to be used only via write_impl()
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <class Write_implActor>
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class Write_implActorState {
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Write_implActorState(Reference<AsyncFileS3BlobStoreWrite> const& f,const uint8_t* const& data,int const& length) 
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		 : f(f),
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		   data(data),
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		   length(length),
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		   p(f->m_parts.back().getPtr())
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
	{
		fdb_probe_actor_create("write_impl", reinterpret_cast<unsigned long>(this));

	}
	~Write_implActorState() 
	{
		fdb_probe_actor_destroy("write_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p->write((const uint8_t*)data, length);
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<Write_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!(p->length + length >= f->m_bstore->knobs.multipart_min_part_size))
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		int finishlen = f->m_bstore->knobs.multipart_min_part_size - p->length;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p->write((const uint8_t*)data, finishlen);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		length -= finishlen;
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		data = (const uint8_t*)data + finishlen;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		StrictFuture<Void> __when_expr_0 = f->endCurrentPart(f.getPtr(), true);
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<Write_implActor*>(this)->actor_wait_state = 1;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 0, Void >*>(static_cast<Write_implActor*>(this)));
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p = f->m_parts.back().getPtr();
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p = f->m_parts.back().getPtr();
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
		if (static_cast<Write_implActor*>(this)->actor_wait_state > 0) static_cast<Write_implActor*>(this)->actor_wait_state = 0;
		static_cast<Write_implActor*>(this)->ActorCallback< Write_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Write_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Reference<AsyncFileS3BlobStoreWrite> f;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	const uint8_t* data;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	int length;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Part* p;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
};
// This generated class is to be used only via write_impl()
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class Write_implActor final : public Actor<Void>, public ActorCallback< Write_implActor, 0, Void >, public FastAllocated<Write_implActor>, public Write_implActorState<Write_implActor> {
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
	using FastAllocated<Write_implActor>::operator new;
	using FastAllocated<Write_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Write_implActor, 0, Void >;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Write_implActor(Reference<AsyncFileS3BlobStoreWrite> const& f,const uint8_t* const& data,int const& length) 
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		 : Actor<Void>(),
		   Write_implActorState<Write_implActor>(f, data, length)
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("write_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Write_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
[[nodiscard]] static Future<Void> write_impl( Reference<AsyncFileS3BlobStoreWrite> const& f, const uint8_t* const& data, int const& length ) {
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	return Future<Void>(new Write_implActor(f, data, length));
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
}

#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"

	Future<Void> write(void const* data, int length, int64_t offset) override {
		if (offset != m_cursor)
			throw non_sequential_op();
		m_cursor += length;

		return m_error.getFuture() ||
		       write_impl(Reference<AsyncFileS3BlobStoreWrite>::addRef(this), (const uint8_t*)data, length);
	}

	Future<Void> truncate(int64_t size) override {
		if (size != m_cursor)
			return non_sequential_op();
		return Void();
	}

																#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
// This generated class is to be used only via doPartUpload()
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <class DoPartUploadActor>
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class DoPartUploadActorState {
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	DoPartUploadActorState(AsyncFileS3BlobStoreWrite* const& f,Part* const& p) 
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		 : f(f),
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		   p(p)
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
	{
		fdb_probe_actor_create("doPartUpload", reinterpret_cast<unsigned long>(this));

	}
	~DoPartUploadActorState() 
	{
		fdb_probe_actor_destroy("doPartUpload", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			p->finalizeMD5();
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			StrictFuture<std::string> __when_expr_0 = f->getUploadID();
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			if (static_cast<DoPartUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoPartUploadActor*>(this)->actor_wait_state = 1;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoPartUploadActor, 0, std::string >*>(static_cast<DoPartUploadActor*>(this)));
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
		this->~DoPartUploadActorState();
		static_cast<DoPartUploadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::string const& upload_id,int loopDepth) 
	{
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		StrictFuture<std::string> __when_expr_1 = f->m_bstore->uploadPart( f->m_bucket, f->m_object, upload_id, p->number, &p->content, p->length, p->md5string);
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (static_cast<DoPartUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoPartUploadActor*>(this)->actor_wait_state = 2;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoPartUploadActor, 1, std::string >*>(static_cast<DoPartUploadActor*>(this)));
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::string && upload_id,int loopDepth) 
	{
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		StrictFuture<std::string> __when_expr_1 = f->m_bstore->uploadPart( f->m_bucket, f->m_object, upload_id, p->number, &p->content, p->length, p->md5string);
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (static_cast<DoPartUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoPartUploadActor*>(this)->actor_wait_state = 2;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoPartUploadActor, 1, std::string >*>(static_cast<DoPartUploadActor*>(this)));
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::string const& upload_id,int loopDepth) 
	{
		loopDepth = a_body1cont1(upload_id, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::string && upload_id,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(upload_id), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoPartUploadActor*>(this)->actor_wait_state > 0) static_cast<DoPartUploadActor*>(this)->actor_wait_state = 0;
		static_cast<DoPartUploadActor*>(this)->ActorCallback< DoPartUploadActor, 0, std::string >::remove();

	}
	void a_callback_fire(ActorCallback< DoPartUploadActor, 0, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoPartUploadActor, 0, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoPartUploadActor, 0, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(std::string const& etag,int loopDepth) 
	{
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<DoPartUploadActor*>(this)->SAV<std::string>::futures) { (void)(etag); this->~DoPartUploadActorState(); static_cast<DoPartUploadActor*>(this)->destroy(); return 0; }
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<DoPartUploadActor*>(this)->SAV< std::string >::value()) std::string(etag);
		this->~DoPartUploadActorState();
		static_cast<DoPartUploadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::string && etag,int loopDepth) 
	{
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<DoPartUploadActor*>(this)->SAV<std::string>::futures) { (void)(etag); this->~DoPartUploadActorState(); static_cast<DoPartUploadActor*>(this)->destroy(); return 0; }
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<DoPartUploadActor*>(this)->SAV< std::string >::value()) std::string(etag);
		this->~DoPartUploadActorState();
		static_cast<DoPartUploadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(std::string const& etag,int loopDepth) 
	{
		loopDepth = a_body1cont2(etag, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::string && etag,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(etag), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoPartUploadActor*>(this)->actor_wait_state > 0) static_cast<DoPartUploadActor*>(this)->actor_wait_state = 0;
		static_cast<DoPartUploadActor*>(this)->ActorCallback< DoPartUploadActor, 1, std::string >::remove();

	}
	void a_callback_fire(ActorCallback< DoPartUploadActor, 1, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoPartUploadActor, 1, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoPartUploadActor, 1, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	AsyncFileS3BlobStoreWrite* f;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	Part* p;
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
};
// This generated class is to be used only via doPartUpload()
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class DoPartUploadActor final : public Actor<std::string>, public ActorCallback< DoPartUploadActor, 0, std::string >, public ActorCallback< DoPartUploadActor, 1, std::string >, public FastAllocated<DoPartUploadActor>, public DoPartUploadActorState<DoPartUploadActor> {
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
	using FastAllocated<DoPartUploadActor>::operator new;
	using FastAllocated<DoPartUploadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::string>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoPartUploadActor, 0, std::string >;
friend struct ActorCallback< DoPartUploadActor, 1, std::string >;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	DoPartUploadActor(AsyncFileS3BlobStoreWrite* const& f,Part* const& p) 
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		 : Actor<std::string>(),
		   DoPartUploadActorState<DoPartUploadActor>(f, p)
	{
		fdb_probe_actor_enter("doPartUpload", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doPartUpload");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doPartUpload", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoPartUploadActor, 0, std::string >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoPartUploadActor, 1, std::string >*)0, actor_cancelled()); break;
		}

	}
};
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
[[nodiscard]] static Future<std::string> doPartUpload( AsyncFileS3BlobStoreWrite* const& f, Part* const& p ) {
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	return Future<std::string>(new DoPartUploadActor(f, p));
															#line 877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
}

#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"

																#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
// This generated class is to be used only via doFinishUpload()
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <class DoFinishUploadActor>
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class DoFinishUploadActorState {
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	DoFinishUploadActorState(AsyncFileS3BlobStoreWrite* const& f) 
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		 : f(f)
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
	{
		fdb_probe_actor_create("doFinishUpload", reinterpret_cast<unsigned long>(this));

	}
	~DoFinishUploadActorState() 
	{
		fdb_probe_actor_destroy("doFinishUpload", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			if (f->m_parts.size() == 1)
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
			{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				Reference<Part> part = f->m_parts.back();
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				part->finalizeMD5();
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				StrictFuture<Void> __when_expr_0 = f->m_bstore->writeEntireFileFromBuffer( f->m_bucket, f->m_object, &part->content, part->length, part->md5string);
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 1;
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoFinishUploadActor, 0, Void >*>(static_cast<DoFinishUploadActor*>(this)));
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
				loopDepth = 0;
			}
			else
			{
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
		this->~DoFinishUploadActorState();
		static_cast<DoFinishUploadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		StrictFuture<Void> __when_expr_1 = f->endCurrentPart(f);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 2;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoFinishUploadActor, 1, Void >*>(static_cast<DoFinishUploadActor*>(this)));
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<DoFinishUploadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoFinishUploadActorState(); static_cast<DoFinishUploadActor*>(this)->destroy(); return 0; }
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<DoFinishUploadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoFinishUploadActorState();
		static_cast<DoFinishUploadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<DoFinishUploadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoFinishUploadActorState(); static_cast<DoFinishUploadActor*>(this)->destroy(); return 0; }
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<DoFinishUploadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoFinishUploadActorState();
		static_cast<DoFinishUploadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state > 0) static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 0;
		static_cast<DoFinishUploadActor*>(this)->ActorCallback< DoFinishUploadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoFinishUploadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		partSet = S3BlobStoreEndpoint::MultiPartSetT();
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p = std::vector<Reference<Part>>::iterator();
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p = f->m_parts.begin();
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = a_body1cont4loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		partSet = S3BlobStoreEndpoint::MultiPartSetT();
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p = std::vector<Reference<Part>>::iterator();
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		p = f->m_parts.begin();
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = a_body1cont4loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state > 0) static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 0;
		static_cast<DoFinishUploadActor*>(this)->ActorCallback< DoFinishUploadActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoFinishUploadActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		StrictFuture<Void> __when_expr_3 = f->m_bstore->finishMultiPartUpload(f->m_bucket, f->m_object, f->m_upload_id.get(), partSet);
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 4;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DoFinishUploadActor, 3, Void >*>(static_cast<DoFinishUploadActor*>(this)));
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont4loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont4loopBody1(int loopDepth) 
	{
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!(p != f->m_parts.end()))
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		{
			return a_body1cont4break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		StrictFuture<std::string> __when_expr_2 = (*p)->etag;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont4loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 3;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoFinishUploadActor, 2, std::string >*>(static_cast<DoFinishUploadActor*>(this)));
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4break1(int loopDepth) 
	{
		try {
			return a_body1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4loopBody1cont1(std::string const& tag,int loopDepth) 
	{
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if ((*p)->length > 0)
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		{
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			partSet[(*p)->number] = tag;
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		}
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		++p;
															#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (loopDepth == 0) return a_body1cont4loopHead1(0);

		return loopDepth;
	}
	int a_body1cont4loopBody1cont1(std::string && tag,int loopDepth) 
	{
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if ((*p)->length > 0)
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		{
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			partSet[(*p)->number] = tag;
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		}
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		++p;
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		if (loopDepth == 0) return a_body1cont4loopHead1(0);

		return loopDepth;
	}
	int a_body1cont4loopBody1when1(std::string const& tag,int loopDepth) 
	{
		loopDepth = a_body1cont4loopBody1cont1(tag, loopDepth);

		return loopDepth;
	}
	int a_body1cont4loopBody1when1(std::string && tag,int loopDepth) 
	{
		loopDepth = a_body1cont4loopBody1cont1(std::move(tag), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state > 0) static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 0;
		static_cast<DoFinishUploadActor*>(this)->ActorCallback< DoFinishUploadActor, 2, std::string >::remove();

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 2, std::string >*,std::string const& value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 2, std::string >*,std::string && value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DoFinishUploadActor, 2, std::string >*,Error err) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<DoFinishUploadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoFinishUploadActorState(); static_cast<DoFinishUploadActor*>(this)->destroy(); return 0; }
															#line 1300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<DoFinishUploadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoFinishUploadActorState();
		static_cast<DoFinishUploadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<DoFinishUploadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DoFinishUploadActorState(); static_cast<DoFinishUploadActor*>(this)->destroy(); return 0; }
															#line 1312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<DoFinishUploadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DoFinishUploadActorState();
		static_cast<DoFinishUploadActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<DoFinishUploadActor*>(this)->actor_wait_state > 0) static_cast<DoFinishUploadActor*>(this)->actor_wait_state = 0;
		static_cast<DoFinishUploadActor*>(this)->ActorCallback< DoFinishUploadActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DoFinishUploadActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DoFinishUploadActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	AsyncFileS3BlobStoreWrite* f;
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	S3BlobStoreEndpoint::MultiPartSetT partSet;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	std::vector<Reference<Part>>::iterator p;
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
};
// This generated class is to be used only via doFinishUpload()
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class DoFinishUploadActor final : public Actor<Void>, public ActorCallback< DoFinishUploadActor, 0, Void >, public ActorCallback< DoFinishUploadActor, 1, Void >, public ActorCallback< DoFinishUploadActor, 2, std::string >, public ActorCallback< DoFinishUploadActor, 3, Void >, public FastAllocated<DoFinishUploadActor>, public DoFinishUploadActorState<DoFinishUploadActor> {
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
	using FastAllocated<DoFinishUploadActor>::operator new;
	using FastAllocated<DoFinishUploadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoFinishUploadActor, 0, Void >;
friend struct ActorCallback< DoFinishUploadActor, 1, Void >;
friend struct ActorCallback< DoFinishUploadActor, 2, std::string >;
friend struct ActorCallback< DoFinishUploadActor, 3, Void >;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	DoFinishUploadActor(AsyncFileS3BlobStoreWrite* const& f) 
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		 : Actor<Void>(),
		   DoFinishUploadActorState<DoFinishUploadActor>(f)
	{
		fdb_probe_actor_enter("doFinishUpload", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doFinishUpload");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doFinishUpload", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoFinishUploadActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoFinishUploadActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DoFinishUploadActor, 2, std::string >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DoFinishUploadActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
[[nodiscard]] static Future<Void> doFinishUpload( AsyncFileS3BlobStoreWrite* const& f ) {
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	return Future<Void>(new DoFinishUploadActor(f));
															#line 1438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
}

#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"

	// Ready once all data has been sent AND acknowledged from the remote side
	Future<Void> sync() override {
		// Only initiate the finish operation once, and also prevent further writing.
		if (!m_finished.isValid()) {
			m_finished = doFinishUpload(this);
			m_cursor = -1; // Cause future write attempts to fail
		}

		return m_finished;
	}

	//
	// Flush can't really do what the caller would "want" for a blob store file.  The caller would probably notionally
	// want all bytes written to be at least in transit to the blob store, but that is not very feasible.  The blob
	// store has a minimum size requirement for all but the final part, and parts must be sent with a header that
	// specifies their size.  So in the case of a write buffer that does not meet the part minimum size the part could
	// be sent but then if there is any more data written then that part needs to be sent again in its entirety.  So a
	// client that calls flush often could generate far more blob store write traffic than they intend to.
	Future<Void> flush() override { return Void(); }

	Future<int64_t> size() const override { return m_cursor; }

	Future<Void> readZeroCopy(void** data, int* length, int64_t offset) override {
		TraceEvent(SevError, "ReadZeroCopyNotSupported").detail("FileType", "S3BlobStoreWrite");
		return platform_error();
	}
	void releaseZeroCopy(void* data, int length, int64_t offset) override {}

	int64_t debugFD() const override { return -1; }

	~AsyncFileS3BlobStoreWrite() override {
		m_upload_id.cancel();
		m_finished.cancel();
		m_parts.clear(); // Contains futures
	}

	std::string getFilename() const override { return m_object; }

private:
	Reference<S3BlobStoreEndpoint> m_bstore;
	std::string m_bucket;
	std::string m_object;

	int64_t m_cursor;

	Future<std::string> m_upload_id;
	Future<Void> m_finished;
	std::vector<Reference<Part>> m_parts;
	Promise<Void> m_error;
	FlowLock m_concurrentUploads;

	// End the current part and start uploading it, but also wait for a part to finish if too many are in transit.
																#line 1495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
// This generated class is to be used only via endCurrentPart()
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
template <class EndCurrentPartActor>
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class EndCurrentPartActorState {
															#line 1501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	EndCurrentPartActorState(AsyncFileS3BlobStoreWrite* const& f,bool const& startNew = false) 
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		 : f(f),
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		   startNew(startNew)
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
	{
		fdb_probe_actor_create("endCurrentPart", reinterpret_cast<unsigned long>(this));

	}
	~EndCurrentPartActorState() 
	{
		fdb_probe_actor_destroy("endCurrentPart", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			if (f->m_parts.back()->length == 0)
															#line 1525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
			{
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
				if (!static_cast<EndCurrentPartActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EndCurrentPartActorState(); static_cast<EndCurrentPartActor*>(this)->destroy(); return 0; }
															#line 1529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
				new (&static_cast<EndCurrentPartActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~EndCurrentPartActorState();
				static_cast<EndCurrentPartActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			StrictFuture<Void> __when_expr_0 = f->m_concurrentUploads.take();
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			if (static_cast<EndCurrentPartActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<EndCurrentPartActor*>(this)->actor_wait_state = 1;
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< EndCurrentPartActor, 0, Void >*>(static_cast<EndCurrentPartActor*>(this)));
															#line 1544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
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
		this->~EndCurrentPartActorState();
		static_cast<EndCurrentPartActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		auto releaser = std::make_shared<FlowLock::Releaser>(f->m_concurrentUploads, 1);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		f->m_parts.back()->etag = holdWhile(std::move(releaser), joinErrorGroup(doPartUpload(f, f->m_parts.back().getPtr()), f->m_error));
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (startNew)
															#line 1571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		{
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			f->m_parts.push_back( Reference<Part>(new Part(f->m_parts.size() + 1, f->m_bstore->knobs.multipart_min_part_size)));
															#line 1575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		}
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<EndCurrentPartActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EndCurrentPartActorState(); static_cast<EndCurrentPartActor*>(this)->destroy(); return 0; }
															#line 1579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<EndCurrentPartActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~EndCurrentPartActorState();
		static_cast<EndCurrentPartActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		auto releaser = std::make_shared<FlowLock::Releaser>(f->m_concurrentUploads, 1);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		f->m_parts.back()->etag = holdWhile(std::move(releaser), joinErrorGroup(doPartUpload(f, f->m_parts.back().getPtr()), f->m_error));
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (startNew)
															#line 1595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		{
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
			f->m_parts.push_back( Reference<Part>(new Part(f->m_parts.size() + 1, f->m_bstore->knobs.multipart_min_part_size)));
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		}
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
		if (!static_cast<EndCurrentPartActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~EndCurrentPartActorState(); static_cast<EndCurrentPartActor*>(this)->destroy(); return 0; }
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		new (&static_cast<EndCurrentPartActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~EndCurrentPartActorState();
		static_cast<EndCurrentPartActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<EndCurrentPartActor*>(this)->actor_wait_state > 0) static_cast<EndCurrentPartActor*>(this)->actor_wait_state = 0;
		static_cast<EndCurrentPartActor*>(this)->ActorCallback< EndCurrentPartActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< EndCurrentPartActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("endCurrentPart", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endCurrentPart", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< EndCurrentPartActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("endCurrentPart", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endCurrentPart", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< EndCurrentPartActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("endCurrentPart", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("endCurrentPart", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	AsyncFileS3BlobStoreWrite* f;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	bool startNew;
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
};
// This generated class is to be used only via endCurrentPart()
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
class EndCurrentPartActor final : public Actor<Void>, public ActorCallback< EndCurrentPartActor, 0, Void >, public FastAllocated<EndCurrentPartActor>, public EndCurrentPartActorState<EndCurrentPartActor> {
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
public:
	using FastAllocated<EndCurrentPartActor>::operator new;
	using FastAllocated<EndCurrentPartActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< EndCurrentPartActor, 0, Void >;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	EndCurrentPartActor(AsyncFileS3BlobStoreWrite* const& f,bool const& startNew = false) 
															#line 1694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
		 : Actor<Void>(),
		   EndCurrentPartActorState<EndCurrentPartActor>(f, startNew)
	{
		fdb_probe_actor_enter("endCurrentPart", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("endCurrentPart");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("endCurrentPart", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< EndCurrentPartActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
[[nodiscard]] static Future<Void> endCurrentPart( AsyncFileS3BlobStoreWrite* const& f, bool const& startNew = false ) {
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"
	return Future<Void>(new EndCurrentPartActor(f, startNew));
															#line 1721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.g.h"
}

#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AsyncFileS3BlobStore.actor.h"

	Future<std::string> getUploadID() {
		if (!m_upload_id.isValid())
			m_upload_id = m_bstore->beginMultiPartUpload(m_bucket, m_object);
		return m_upload_id;
	}

public:
	AsyncFileS3BlobStoreWrite(Reference<S3BlobStoreEndpoint> bstore, std::string bucket, std::string object)
	  : m_bstore(bstore), m_bucket(bucket), m_object(object), m_cursor(0),
	    m_concurrentUploads(bstore->knobs.concurrent_writes_per_file) {

		// Add first part
		m_parts.push_back(makeReference<Part>(1, m_bstore->knobs.multipart_min_part_size));
	}
};

// This class represents a read-only file that lives in an S3-style blob store.  It reads using the REST API.
class AsyncFileS3BlobStoreRead final : public IAsyncFile, public ReferenceCounted<AsyncFileS3BlobStoreRead> {
public:
	void addref() override { ReferenceCounted<AsyncFileS3BlobStoreRead>::addref(); }
	void delref() override { ReferenceCounted<AsyncFileS3BlobStoreRead>::delref(); }

	Future<int> read(void* data, int length, int64_t offset) override;

	Future<Void> write(void const* data, int length, int64_t offset) override { throw file_not_writable(); }
	Future<Void> truncate(int64_t size) override { throw file_not_writable(); }

	Future<Void> sync() override { return Void(); }
	Future<Void> flush() override { return Void(); }

	Future<int64_t> size() const override;

	Future<Void> readZeroCopy(void** data, int* length, int64_t offset) override {
		TraceEvent(SevError, "ReadZeroCopyNotSupported").detail("FileType", "S3BlobStoreRead");
		return platform_error();
	}
	void releaseZeroCopy(void* data, int length, int64_t offset) override {}

	int64_t debugFD() const override { return -1; }

	std::string getFilename() const override { return m_object; }

	~AsyncFileS3BlobStoreRead() override {}

	Reference<S3BlobStoreEndpoint> m_bstore;
	std::string m_bucket;
	std::string m_object;
	mutable Future<int64_t> m_size;

	AsyncFileS3BlobStoreRead(Reference<S3BlobStoreEndpoint> bstore, std::string bucket, std::string object)
	  : m_bstore(bstore), m_bucket(bucket), m_object(object) {}
};

#include "flow/unactorcompiler.h"
#endif
