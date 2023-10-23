#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
/*
 * AsyncFileEIO.actor.h
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

#if defined(__unixish__)

#define Net2AsyncFile AsyncFileEIO

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FLOW_ASYNCFILEEIO_ACTOR_G_H)
#define FLOW_ASYNCFILEEIO_ACTOR_G_H
#include "fdbrpc/AsyncFileEIO.actor.g.h"
#elif !defined(FLOW_ASYNCFILEEIO_ACTOR_H)
#define FLOW_ASYNCFILEEIO_ACTOR_H

#include <fcntl.h>
#include <sys/stat.h>

#include "fdbrpc/libeio/eio.h"
#include "flow/flow.h"
#include "flow/ThreadHelper.actor.h"
#include "fdbrpc/IAsyncFile.h"
#include "flow/TDMetric.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

class AsyncFileEIO : public IAsyncFile, public ReferenceCounted<AsyncFileEIO> {

public:
	static void init() {
		eio_set_max_parallel(FLOW_KNOBS->EIO_MAX_PARALLELISM);
		if (eio_init(&eio_want_poll, nullptr)) {
			TraceEvent("EioInitError").detail("ErrorNo", errno);
			throw platform_error();
		}
	}

	static void stop() { eio_set_max_parallel(0); }

	static bool should_poll() { return want_poll; }

	static bool lock_fd(int fd) {
		// Acquire a "write" lock for the entire file
		struct flock lockDesc;
		lockDesc.l_type = F_WRLCK;
		lockDesc.l_whence = SEEK_SET;
		lockDesc.l_start = 0;
		lockDesc.l_len =
		    0; // "Specifying 0 for l_len has the special meaning: lock all bytes starting at the location specified by
		       // l_whence and l_start through to the end of file, no matter how large the file grows."
		lockDesc.l_pid = 0;
		if (fcntl(fd, F_SETLK, &lockDesc) == -1) {
			return false;
		}
		return true;
	}

																#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via open()
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class OpenActor>
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class OpenActorState {
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	OpenActorState(std::string const& filename,int const& flags,int const& mode,void* const& ignore) 
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : filename(filename),
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   flags(flags),
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   mode(mode),
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   ignore(ignore)
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("open", reinterpret_cast<unsigned long>(this));

	}
	~OpenActorState() 
	{
		fdb_probe_actor_destroy("open", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			std::string open_filename = filename;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				ASSERT((flags & OPEN_CREATE) && (flags & OPEN_READWRITE) && !(flags & OPEN_EXCLUSIVE));
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				open_filename = filename + ".part";
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			}
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			p = Promise<Void>();
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			r = eio_open(open_filename.c_str(), openFlags(flags), mode, 0, eio_callback, &p);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			try {
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<OpenActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OpenActor*>(this)->actor_wait_state = 1;
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OpenActor, 0, Void >*>(static_cast<OpenActor*>(this)));
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result < 0)
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			errno = r->errorno;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			bool notFound = errno == ENOENT;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			Error e = notFound ? file_not_found() : io_error();
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			TraceEvent(notFound ? SevWarn : SevWarnAlways, "FileOpenError") .error(e) .GetLastError() .detail("File", filename) .detail("Flags", flags) .detail("Mode", mode);
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		TraceEvent("AsyncFileOpened") .suppressFor(1.0) .detail("Filename", filename) .detail("Fd", r->result) .detail("Flags", flags);
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if ((flags & OPEN_LOCK) && !lock_fd(r->result))
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			TraceEvent(SevError, "UnableToLockFile").detail("Filename", filename).GetLastError();
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(io_error(), loopDepth);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<OpenActor*>(this)->SAV<Reference<IAsyncFile>>::futures) { (void)(Reference<IAsyncFile>(new AsyncFileEIO(r->result, flags, filename))); this->~OpenActorState(); static_cast<OpenActor*>(this)->destroy(); return 0; }
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<OpenActor*>(this)->SAV< Reference<IAsyncFile> >::value()) Reference<IAsyncFile>(Reference<IAsyncFile>(new AsyncFileEIO(r->result, flags, filename)));
		this->~OpenActorState();
		static_cast<OpenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OpenActor*>(this)->actor_wait_state > 0) static_cast<OpenActor*>(this)->actor_wait_state = 0;
		static_cast<OpenActor*>(this)->ActorCallback< OpenActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OpenActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OpenActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OpenActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), 0);

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
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string filename;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int flags;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int mode;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	void* ignore;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via open()
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class OpenActor final : public Actor<Reference<IAsyncFile>>, public ActorCallback< OpenActor, 0, Void >, public FastAllocated<OpenActor>, public OpenActorState<OpenActor> {
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<OpenActor>::operator new;
	using FastAllocated<OpenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<IAsyncFile>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OpenActor, 0, Void >;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	OpenActor(std::string const& filename,int const& flags,int const& mode,void* const& ignore) 
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Reference<IAsyncFile>>(),
		   OpenActorState<OpenActor>(filename, flags, mode, ignore)
	{
		fdb_probe_actor_enter("open", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("open");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("open", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OpenActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( std::string const& filename, int const& flags, int const& mode, void* const& ignore ) {
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Reference<IAsyncFile>>(new OpenActor(filename, flags, mode, ignore));
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	static Future<Void> deleteFile(std::string filename, bool mustBeDurable) {
		::deleteFile(filename);
		if (mustBeDurable) {
			TEST(true); // deleteFile and fsync parent dir
			return async_fsync_parent(filename);
		} else
			return Void();
	}

																#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via renameFile()
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class RenameFileActor>
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class RenameFileActorState {
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	RenameFileActorState(std::string const& from,std::string const& to) 
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : from(from),
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   to(to),
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_rename(from.c_str(), to.c_str(), 0, eio_callback, &p))
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("renameFile", reinterpret_cast<unsigned long>(this));

	}
	~RenameFileActorState() 
	{
		fdb_probe_actor_destroy("renameFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<RenameFileActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<RenameFileActor*>(this)->actor_wait_state = 1;
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RenameFileActor, 0, Void >*>(static_cast<RenameFileActor*>(this)));
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~RenameFileActorState();
		static_cast<RenameFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		try {
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			result = r->result;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (result == -1)
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			{
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				TraceEvent(SevError, "FileRenameError").detail("Errno", r->errorno);
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				return a_body1cont1Catch1(internal_error(), loopDepth);
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			}
			else
			{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<RenameFileActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
				static_cast<RenameFileActor*>(this)->actor_wait_state = 2;
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameFileActor, 1, Void >*>(static_cast<RenameFileActor*>(this)));
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				loopDepth = 0;
			}
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<RenameFileActor*>(this)->actor_wait_state > 0) static_cast<RenameFileActor*>(this)->actor_wait_state = 0;
		static_cast<RenameFileActor*>(this)->ActorCallback< RenameFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RenameFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RenameFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			_e = e;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<Void> __when_expr_2 = delay(0, taskID);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (static_cast<RenameFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RenameFileActor*>(this)->actor_wait_state = 3;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RenameFileActor, 2, Void >*>(static_cast<RenameFileActor*>(this)));
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<RenameFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameFileActorState(); static_cast<RenameFileActor*>(this)->destroy(); return 0; }
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<RenameFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameFileActorState();
		static_cast<RenameFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<RenameFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameFileActorState(); static_cast<RenameFileActor*>(this)->destroy(); return 0; }
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<RenameFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameFileActorState();
		static_cast<RenameFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<RenameFileActor*>(this)->actor_wait_state > 0) static_cast<RenameFileActor*>(this)->actor_wait_state = 0;
		static_cast<RenameFileActor*>(this)->ActorCallback< RenameFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RenameFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RenameFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		return a_body1Catch1(_e, loopDepth);
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		return a_body1Catch1(_e, loopDepth);
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<RenameFileActor*>(this)->actor_wait_state > 0) static_cast<RenameFileActor*>(this)->actor_wait_state = 0;
		static_cast<RenameFileActor*>(this)->ActorCallback< RenameFileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameFileActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RenameFileActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RenameFileActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string from;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string to;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int result;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Error _e;
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via renameFile()
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class RenameFileActor final : public Actor<Void>, public ActorCallback< RenameFileActor, 0, Void >, public ActorCallback< RenameFileActor, 1, Void >, public ActorCallback< RenameFileActor, 2, Void >, public FastAllocated<RenameFileActor>, public RenameFileActorState<RenameFileActor> {
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<RenameFileActor>::operator new;
	using FastAllocated<RenameFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RenameFileActor, 0, Void >;
friend struct ActorCallback< RenameFileActor, 1, Void >;
friend struct ActorCallback< RenameFileActor, 2, Void >;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	RenameFileActor(std::string const& from,std::string const& to) 
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Void>(),
		   RenameFileActorState<RenameFileActor>(from, to)
	{
		fdb_probe_actor_enter("renameFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("renameFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("renameFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RenameFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RenameFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RenameFileActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Void> renameFile( std::string const& from, std::string const& to ) {
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Void>(new RenameFileActor(from, to));
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via lastWriteTime()
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class LastWriteTimeActor>
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class LastWriteTimeActorState {
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	LastWriteTimeActorState(std::string const& filename) 
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : filename(filename)
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("lastWriteTime", reinterpret_cast<unsigned long>(this));

	}
	~LastWriteTimeActorState() 
	{
		fdb_probe_actor_destroy("lastWriteTime", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<EIO_STRUCT_STAT> __when_expr_0 = stat_impl(filename);
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (static_cast<LastWriteTimeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LastWriteTimeActor*>(this)->actor_wait_state = 1;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >*>(static_cast<LastWriteTimeActor*>(this)));
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~LastWriteTimeActorState();
		static_cast<LastWriteTimeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(EIO_STRUCT_STAT const& statdata,int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<LastWriteTimeActor*>(this)->SAV<std::time_t>::futures) { (void)(statdata.st_mtime); this->~LastWriteTimeActorState(); static_cast<LastWriteTimeActor*>(this)->destroy(); return 0; }
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<LastWriteTimeActor*>(this)->SAV< std::time_t >::value()) std::time_t(statdata.st_mtime);
		this->~LastWriteTimeActorState();
		static_cast<LastWriteTimeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(EIO_STRUCT_STAT && statdata,int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<LastWriteTimeActor*>(this)->SAV<std::time_t>::futures) { (void)(statdata.st_mtime); this->~LastWriteTimeActorState(); static_cast<LastWriteTimeActor*>(this)->destroy(); return 0; }
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<LastWriteTimeActor*>(this)->SAV< std::time_t >::value()) std::time_t(statdata.st_mtime);
		this->~LastWriteTimeActorState();
		static_cast<LastWriteTimeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(EIO_STRUCT_STAT const& statdata,int loopDepth) 
	{
		loopDepth = a_body1cont1(statdata, loopDepth);

		return loopDepth;
	}
	int a_body1when1(EIO_STRUCT_STAT && statdata,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(statdata), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<LastWriteTimeActor*>(this)->actor_wait_state > 0) static_cast<LastWriteTimeActor*>(this)->actor_wait_state = 0;
		static_cast<LastWriteTimeActor*>(this)->ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >::remove();

	}
	void a_callback_fire(ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >*,EIO_STRUCT_STAT const& value) 
	{
		fdb_probe_actor_enter("lastWriteTime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastWriteTime", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >*,EIO_STRUCT_STAT && value) 
	{
		fdb_probe_actor_enter("lastWriteTime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastWriteTime", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >*,Error err) 
	{
		fdb_probe_actor_enter("lastWriteTime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastWriteTime", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string filename;
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via lastWriteTime()
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class LastWriteTimeActor final : public Actor<std::time_t>, public ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >, public FastAllocated<LastWriteTimeActor>, public LastWriteTimeActorState<LastWriteTimeActor> {
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<LastWriteTimeActor>::operator new;
	using FastAllocated<LastWriteTimeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::time_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	LastWriteTimeActor(std::string const& filename) 
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<std::time_t>(),
		   LastWriteTimeActorState<LastWriteTimeActor>(filename)
	{
		fdb_probe_actor_enter("lastWriteTime", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lastWriteTime");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lastWriteTime", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LastWriteTimeActor, 0, EIO_STRUCT_STAT >*)0, actor_cancelled()); break;
		}

	}
};
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<std::time_t> lastWriteTime( std::string const& filename ) {
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<std::time_t>(new LastWriteTimeActor(filename));
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

	void addref() override { ReferenceCounted<AsyncFileEIO>::addref(); }
	void delref() override { ReferenceCounted<AsyncFileEIO>::delref(); }

	int64_t debugFD() const override { return fd; }

	Future<int> read(void* data, int length, int64_t offset) override {
		++countFileLogicalReads;
		++countLogicalReads;
		return read_impl(fd, data, length, offset);
	}
	Future<Void> write(void const* data, int length, int64_t offset) override // Copies data synchronously
	{
		++countFileLogicalWrites;
		++countLogicalWrites;
		// Standalone<StringRef> copy = StringRef((const uint8_t*)data, length);
		return write_impl(fd, err, StringRef((const uint8_t*)data, length), offset);
	}
	Future<Void> truncate(int64_t size) override {
		++countFileLogicalWrites;
		++countLogicalWrites;
		return truncate_impl(fd, err, size);
	}
	Future<Void> sync() override {
		++countFileLogicalWrites;
		++countLogicalWrites;
		auto fsync = sync_impl(fd, err);

		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE) {
			flags &= ~OPEN_ATOMIC_WRITE_AND_CREATE;

			return waitAndAtomicRename(fsync, filename + ".part", filename);
		}

		return fsync;
	}
	Future<int64_t> size() const override {
		++countFileLogicalReads;
		++countLogicalReads;
		return size_impl(fd);
	}
	std::string getFilename() const override { return filename; }

																#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via async_fsync_parent()
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Async_fsync_parentActor>
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Async_fsync_parentActorState {
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Async_fsync_parentActorState(std::string const& filename) 
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : filename(filename)
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("async_fsync_parent", reinterpret_cast<unsigned long>(this));

	}
	~Async_fsync_parentActorState() 
	{
		fdb_probe_actor_destroy("async_fsync_parent", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			std::string folder = parentDirectory(filename);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			TraceEvent("FSyncParentDir").detail("Folder", folder).detail("File", filename);
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			folderFD = ::open(folder.c_str(), O_DIRECTORY | O_CLOEXEC, 0);
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (folderFD < 0)
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			{
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				return a_body1Catch1(io_error(), loopDepth);
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			}
			try {
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = async_fsync(folderFD);
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Async_fsync_parentActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Async_fsync_parentActor*>(this)->actor_wait_state = 1;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Async_fsync_parentActor, 0, Void >*>(static_cast<Async_fsync_parentActor*>(this)));
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Async_fsync_parentActorState();
		static_cast<Async_fsync_parentActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		close(folderFD);
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Async_fsync_parentActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Async_fsync_parentActorState(); static_cast<Async_fsync_parentActor*>(this)->destroy(); return 0; }
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Async_fsync_parentActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Async_fsync_parentActorState();
		static_cast<Async_fsync_parentActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			close(folderFD);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 1175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<Async_fsync_parentActor*>(this)->actor_wait_state > 0) static_cast<Async_fsync_parentActor*>(this)->actor_wait_state = 0;
		static_cast<Async_fsync_parentActor*>(this)->ActorCallback< Async_fsync_parentActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Async_fsync_parentActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("async_fsync_parent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("async_fsync_parent", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Async_fsync_parentActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("async_fsync_parent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("async_fsync_parent", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Async_fsync_parentActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("async_fsync_parent", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("async_fsync_parent", reinterpret_cast<unsigned long>(this), 0);

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
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string filename;
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int folderFD;
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via async_fsync_parent()
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Async_fsync_parentActor final : public Actor<Void>, public ActorCallback< Async_fsync_parentActor, 0, Void >, public FastAllocated<Async_fsync_parentActor>, public Async_fsync_parentActorState<Async_fsync_parentActor> {
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Async_fsync_parentActor>::operator new;
	using FastAllocated<Async_fsync_parentActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Async_fsync_parentActor, 0, Void >;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Async_fsync_parentActor(std::string const& filename) 
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Void>(),
		   Async_fsync_parentActorState<Async_fsync_parentActor>(filename)
	{
		fdb_probe_actor_enter("async_fsync_parent", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("async_fsync_parent");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("async_fsync_parent", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Async_fsync_parentActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Void> async_fsync_parent( std::string const& filename ) {
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Void>(new Async_fsync_parentActor(filename));
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

	static Future<Void> async_fdatasync(int fd) {
		// Used by AsyncFileKAIO, since kernel AIO doesn't really implement fdatasync yet
		return sync_impl(fd, makeReference<ErrorInfo>());
	}
	static Future<Void> async_fsync(int fd) {
		// Used by AsyncFileKAIO, since kernel AIO doesn't really implement fsync yet
		return sync_impl(fd, makeReference<ErrorInfo>(), true);
	}
																#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via waitAndAtomicRename()
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class WaitAndAtomicRenameActor>
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class WaitAndAtomicRenameActorState {
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	WaitAndAtomicRenameActorState(Future<Void> const& fsync,std::string const& part_filename,std::string const& final_filename) 
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fsync(fsync),
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   part_filename(part_filename),
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   final_filename(final_filename)
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("waitAndAtomicRename", reinterpret_cast<unsigned long>(this));

	}
	~WaitAndAtomicRenameActorState() 
	{
		fdb_probe_actor_destroy("waitAndAtomicRename", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<Void> __when_expr_0 = fsync;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state = 1;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitAndAtomicRenameActor, 0, Void >*>(static_cast<WaitAndAtomicRenameActor*>(this)));
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~WaitAndAtomicRenameActorState();
		static_cast<WaitAndAtomicRenameActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (rename(part_filename.c_str(), final_filename.c_str()))
															#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			TraceEvent("AsyncFileEIORenameError").detail("Filename", final_filename).GetLastError();
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(io_error(), loopDepth);
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = async_fsync_parent(final_filename);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state = 2;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitAndAtomicRenameActor, 1, Void >*>(static_cast<WaitAndAtomicRenameActor*>(this)));
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (rename(part_filename.c_str(), final_filename.c_str()))
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			TraceEvent("AsyncFileEIORenameError").detail("Filename", final_filename).GetLastError();
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(io_error(), loopDepth);
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = async_fsync_parent(final_filename);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state = 2;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitAndAtomicRenameActor, 1, Void >*>(static_cast<WaitAndAtomicRenameActor*>(this)));
															#line 1438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state > 0) static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndAtomicRenameActor*>(this)->ActorCallback< WaitAndAtomicRenameActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAndAtomicRenameActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitAndAtomicRenameActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitAndAtomicRenameActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<WaitAndAtomicRenameActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndAtomicRenameActorState(); static_cast<WaitAndAtomicRenameActor*>(this)->destroy(); return 0; }
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<WaitAndAtomicRenameActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndAtomicRenameActorState();
		static_cast<WaitAndAtomicRenameActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<WaitAndAtomicRenameActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitAndAtomicRenameActorState(); static_cast<WaitAndAtomicRenameActor*>(this)->destroy(); return 0; }
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<WaitAndAtomicRenameActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WaitAndAtomicRenameActorState();
		static_cast<WaitAndAtomicRenameActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state > 0) static_cast<WaitAndAtomicRenameActor*>(this)->actor_wait_state = 0;
		static_cast<WaitAndAtomicRenameActor*>(this)->ActorCallback< WaitAndAtomicRenameActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitAndAtomicRenameActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitAndAtomicRenameActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitAndAtomicRenameActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Future<Void> fsync;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string part_filename;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string final_filename;
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via waitAndAtomicRename()
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class WaitAndAtomicRenameActor final : public Actor<Void>, public ActorCallback< WaitAndAtomicRenameActor, 0, Void >, public ActorCallback< WaitAndAtomicRenameActor, 1, Void >, public FastAllocated<WaitAndAtomicRenameActor>, public WaitAndAtomicRenameActorState<WaitAndAtomicRenameActor> {
															#line 1604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<WaitAndAtomicRenameActor>::operator new;
	using FastAllocated<WaitAndAtomicRenameActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitAndAtomicRenameActor, 0, Void >;
friend struct ActorCallback< WaitAndAtomicRenameActor, 1, Void >;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	WaitAndAtomicRenameActor(Future<Void> const& fsync,std::string const& part_filename,std::string const& final_filename) 
															#line 1616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Void>(),
		   WaitAndAtomicRenameActorState<WaitAndAtomicRenameActor>(fsync, part_filename, final_filename)
	{
		fdb_probe_actor_enter("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitAndAtomicRename");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitAndAtomicRename", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitAndAtomicRenameActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitAndAtomicRenameActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Void> waitAndAtomicRename( Future<Void> const& fsync, std::string const& part_filename, std::string const& final_filename ) {
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Void>(new WaitAndAtomicRenameActor(fsync, part_filename, final_filename));
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

	// Run the given function on the EIO thread pool and return its result
	template <class R>
	static Future<R> dispatch(std::function<R()> const& func) {
		return dispatch_impl(func);
	}

	~AsyncFileEIO() override { close_impl(fd); }

private:
	struct ErrorInfo : ReferenceCounted<ErrorInfo>, FastAllocated<ErrorInfo> {
		Error err;
		void set(const Error& e) {
			if (err.code() == invalid_error_code)
				err = e;
		}
		void report() {
			if (err.code() != invalid_error_code)
				throw err;
		}
	};

	template <class R>
	struct Dispatch {
		std::function<R()> func;
		ErrorOr<R> result;
		Promise<Void> done;
		explicit Dispatch(std::function<R()> const& func) : func(func) {}
	};

	int fd, flags;
	Reference<ErrorInfo> err;
	std::string filename;
	mutable Int64MetricHandle countFileLogicalWrites;
	mutable Int64MetricHandle countFileLogicalReads;

	mutable Int64MetricHandle countLogicalWrites;
	mutable Int64MetricHandle countLogicalReads;

	AsyncFileEIO(int fd, int flags, std::string const& filename)
	  : fd(fd), flags(flags), err(new ErrorInfo), filename(filename) {
		if (!g_network->isSimulated()) {
			countFileLogicalWrites.init(LiteralStringRef("AsyncFile.CountFileLogicalWrites"), filename);
			countFileLogicalReads.init(LiteralStringRef("AsyncFile.CountFileLogicalReads"), filename);

			countLogicalWrites.init(LiteralStringRef("AsyncFile.CountLogicalWrites"));
			countLogicalReads.init(LiteralStringRef("AsyncFile.CountLogicalReads"));
		}
	}

	static int openFlags(int flags) {
		int oflags = O_CLOEXEC;
		ASSERT(bool(flags & OPEN_READONLY) != bool(flags & OPEN_READWRITE)); // readonly xor readwrite
		if (flags & OPEN_EXCLUSIVE)
			oflags |= O_EXCL;
		if (flags & OPEN_CREATE)
			oflags |= O_CREAT;
		if (flags & OPEN_READONLY)
			oflags |= O_RDONLY;
		if (flags & OPEN_READWRITE)
			oflags |= O_RDWR;
		if (flags & OPEN_ATOMIC_WRITE_AND_CREATE)
			oflags |= O_TRUNC;
#if defined(__linux__)
		if (flags & OPEN_UNBUFFERED && FLOW_KNOBS->EIO_USE_ODIRECT)
			oflags |= O_DIRECT;
#endif
		return oflags;
	}

	static void error(const char* context,
	                  int fd,
	                  eio_req* r,
	                  Reference<ErrorInfo> const& err = Reference<ErrorInfo>()) {
		Error e = io_error();
		errno = r->errorno;
		TraceEvent(context).error(e).detail("Fd", fd).detail("Result", r->result).GetLastError();
		if (err)
			err->set(e);
		else
			throw e;
	}

																#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via close_impl()
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Close_implActor>
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Close_implActorState {
															#line 1737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Close_implActorState(int const& fd) 
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fd(fd),
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_close(fd, 0, eio_callback, &p))
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("close_impl", reinterpret_cast<unsigned long>(this));

	}
	~Close_implActorState() 
	{
		fdb_probe_actor_destroy("close_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			static_cast<Close_implActor*>(this)->actor_wait_state = 1;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Close_implActor, 0, Void >*>(static_cast<Close_implActor*>(this)));
															#line 1769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		delete static_cast<Close_implActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result)
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("CloseError", fd, r);
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		TraceEvent("AsyncFileClosed").suppressFor(1.0).detail("Fd", fd);
															#line 1799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result)
															#line 1808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("CloseError", fd, r);
															#line 1812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		TraceEvent("AsyncFileClosed").suppressFor(1.0).detail("Fd", fd);
															#line 1816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

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
		if (static_cast<Close_implActor*>(this)->actor_wait_state > 0) static_cast<Close_implActor*>(this)->actor_wait_state = 0;
		static_cast<Close_implActor*>(this)->ActorCallback< Close_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Close_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("close_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("close_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Close_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("close_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("close_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Close_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("close_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("close_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		delete static_cast<Close_implActor*>(this);
															#line 1888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		return 0;

		return loopDepth;
	}
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int fd;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via close_impl()
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Close_implActor final : public Actor<void>, public ActorCallback< Close_implActor, 0, Void >, public FastAllocated<Close_implActor>, public Close_implActorState<Close_implActor> {
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Close_implActor>::operator new;
	using FastAllocated<Close_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< Close_implActor, 0, Void >;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Close_implActor(int const& fd) 
															#line 1915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<void>(),
		   Close_implActorState<Close_implActor>(fd)
	{
		fdb_probe_actor_enter("close_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("close_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("close_impl", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
static void close_impl( int const& fd ) {
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	new Close_implActor(fd);
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 1938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via read_impl()
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Read_implActor>
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Read_implActorState {
															#line 1944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Read_implActorState(int const& fd,void* const& data,int const& length,int64_t const& offset) 
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fd(fd),
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   data(data),
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   length(length),
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   offset(offset),
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_read(fd, data, length, offset, 0, eio_callback, &p))
															#line 1963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("read_impl", reinterpret_cast<unsigned long>(this));

	}
	~Read_implActorState() 
	{
		fdb_probe_actor_destroy("read_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Read_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Read_implActor*>(this)->actor_wait_state = 1;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Read_implActor, 0, Void >*>(static_cast<Read_implActor*>(this)));
															#line 1986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		try {
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			result = r->result;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (result == -1)
															#line 2018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				error("ReadError", fd, r);
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				return a_body1cont1Catch1(internal_error(), loopDepth);
															#line 2024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			}
			else
			{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Read_implActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
				static_cast<Read_implActor*>(this)->actor_wait_state = 2;
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Read_implActor, 1, Void >*>(static_cast<Read_implActor*>(this)));
															#line 2037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				loopDepth = 0;
			}
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 2058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Read_implActor*>(this)->actor_wait_state > 0) static_cast<Read_implActor*>(this)->actor_wait_state = 0;
		static_cast<Read_implActor*>(this)->ActorCallback< Read_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Read_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Read_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont1Catch1(const Error& _e,int loopDepth=0) 
	{
		try {
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			e = _e;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<Void> __when_expr_2 = delay(0, taskID);
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (static_cast<Read_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<Read_implActor*>(this)->actor_wait_state = 3;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Read_implActor, 2, Void >*>(static_cast<Read_implActor*>(this)));
															#line 2170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Read_implActor*>(this)->SAV<int>::futures) { (void)(result); this->~Read_implActorState(); static_cast<Read_implActor*>(this)->destroy(); return 0; }
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Read_implActor*>(this)->SAV< int >::value()) int(std::move(result)); // state_var_RVO
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Read_implActor*>(this)->SAV<int>::futures) { (void)(result); this->~Read_implActorState(); static_cast<Read_implActor*>(this)->destroy(); return 0; }
															#line 2197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Read_implActor*>(this)->SAV< int >::value()) int(std::move(result)); // state_var_RVO
		this->~Read_implActorState();
		static_cast<Read_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Read_implActor*>(this)->actor_wait_state > 0) static_cast<Read_implActor*>(this)->actor_wait_state = 0;
		static_cast<Read_implActor*>(this)->ActorCallback< Read_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Read_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Read_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 2272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 2280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<Read_implActor*>(this)->actor_wait_state > 0) static_cast<Read_implActor*>(this)->actor_wait_state = 0;
		static_cast<Read_implActor*>(this)->ActorCallback< Read_implActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Read_implActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< Read_implActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< Read_implActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int fd;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	void* data;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int length;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int64_t offset;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int result;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Error e;
															#line 2365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via read_impl()
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Read_implActor final : public Actor<int>, public ActorCallback< Read_implActor, 0, Void >, public ActorCallback< Read_implActor, 1, Void >, public ActorCallback< Read_implActor, 2, Void >, public FastAllocated<Read_implActor>, public Read_implActorState<Read_implActor> {
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Read_implActor>::operator new;
	using FastAllocated<Read_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Read_implActor, 0, Void >;
friend struct ActorCallback< Read_implActor, 1, Void >;
friend struct ActorCallback< Read_implActor, 2, Void >;
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Read_implActor(int const& fd,void* const& data,int const& length,int64_t const& offset) 
															#line 2383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<int>(),
		   Read_implActorState<Read_implActor>(fd, data, length, offset)
	{
		fdb_probe_actor_enter("read_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Read_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Read_implActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< Read_implActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<int> read_impl( int const& fd, void* const& data, int const& length, int64_t const& offset ) {
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<int>(new Read_implActor(fd, data, length, offset));
															#line 2412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 2417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via write_impl()
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Write_implActor>
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Write_implActorState {
															#line 2423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Write_implActorState(int const& fd,Reference<ErrorInfo> const& err,StringRef const& data,int64_t const& offset) 
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fd(fd),
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   err(err),
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   data(data),
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   offset(offset),
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_write(fd, (void*)data.begin(), data.size(), offset, 0, eio_callback, &p))
															#line 2442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
			try {
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Write_implActor*>(this)->actor_wait_state = 1;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 0, Void >*>(static_cast<Write_implActor*>(this)));
															#line 2465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result != data.size())
															#line 2494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("WriteError", fd, r, err);
															#line 2498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<Write_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Write_implActor*>(this)->actor_wait_state = 2;
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Write_implActor, 1, Void >*>(static_cast<Write_implActor*>(this)));
															#line 2509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 2523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Write_implActor*>(this)->actor_wait_state > 0) static_cast<Write_implActor*>(this)->actor_wait_state = 0;
		static_cast<Write_implActor*>(this)->ActorCallback< Write_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Write_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Write_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 2625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Write_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Write_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Write_implActorState(); static_cast<Write_implActor*>(this)->destroy(); return 0; }
															#line 2637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Write_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Write_implActorState();
		static_cast<Write_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Write_implActor*>(this)->actor_wait_state > 0) static_cast<Write_implActor*>(this)->actor_wait_state = 0;
		static_cast<Write_implActor*>(this)->ActorCallback< Write_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Write_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Write_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Write_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int fd;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Reference<ErrorInfo> err;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	StringRef data;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int64_t offset;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 2722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via write_impl()
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Write_implActor final : public Actor<Void>, public ActorCallback< Write_implActor, 0, Void >, public ActorCallback< Write_implActor, 1, Void >, public FastAllocated<Write_implActor>, public Write_implActorState<Write_implActor> {
															#line 2727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Write_implActor>::operator new;
	using FastAllocated<Write_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Write_implActor, 0, Void >;
friend struct ActorCallback< Write_implActor, 1, Void >;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Write_implActor(int const& fd,Reference<ErrorInfo> const& err,StringRef const& data,int64_t const& offset) 
															#line 2739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Void>(),
		   Write_implActorState<Write_implActor>(fd, err, data, offset)
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
		case 2: this->a_callback_error((ActorCallback< Write_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Void> write_impl( int const& fd, Reference<ErrorInfo> const& err, StringRef const& data, int64_t const& offset ) {
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Void>(new Write_implActor(fd, err, data, offset));
															#line 2767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 2772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via truncate_impl()
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Truncate_implActor>
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Truncate_implActorState {
															#line 2778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Truncate_implActorState(int const& fd,Reference<ErrorInfo> const& err,int64_t const& size) 
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fd(fd),
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   err(err),
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   size(size),
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_ftruncate(fd, size, 0, eio_callback, &p))
															#line 2795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	~Truncate_implActorState() 
	{
		fdb_probe_actor_destroy("truncate_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 2813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Truncate_implActor*>(this)->actor_wait_state = 1;
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 0, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result)
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("TruncateError", fd, r, err);
															#line 2851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Truncate_implActor*>(this)->actor_wait_state = 2;
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Truncate_implActor, 1, Void >*>(static_cast<Truncate_implActor*>(this)));
															#line 2862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 2876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state > 0) static_cast<Truncate_implActor*>(this)->actor_wait_state = 0;
		static_cast<Truncate_implActor*>(this)->ActorCallback< Truncate_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Truncate_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Truncate_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActorState(); static_cast<Truncate_implActor*>(this)->destroy(); return 0; }
															#line 2978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Truncate_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Truncate_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Truncate_implActorState(); static_cast<Truncate_implActor*>(this)->destroy(); return 0; }
															#line 2990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Truncate_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Truncate_implActorState();
		static_cast<Truncate_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Truncate_implActor*>(this)->actor_wait_state > 0) static_cast<Truncate_implActor*>(this)->actor_wait_state = 0;
		static_cast<Truncate_implActor*>(this)->ActorCallback< Truncate_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Truncate_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Truncate_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int fd;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Reference<ErrorInfo> err;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int64_t size;
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 3073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via truncate_impl()
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Truncate_implActor final : public Actor<Void>, public ActorCallback< Truncate_implActor, 0, Void >, public ActorCallback< Truncate_implActor, 1, Void >, public FastAllocated<Truncate_implActor>, public Truncate_implActorState<Truncate_implActor> {
															#line 3078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Truncate_implActor>::operator new;
	using FastAllocated<Truncate_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Truncate_implActor, 0, Void >;
friend struct ActorCallback< Truncate_implActor, 1, Void >;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Truncate_implActor(int const& fd,Reference<ErrorInfo> const& err,int64_t const& size) 
															#line 3090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Void>(),
		   Truncate_implActorState<Truncate_implActor>(fd, err, size)
	{
		fdb_probe_actor_enter("truncate_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("truncate_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("truncate_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Truncate_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Truncate_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Void> truncate_impl( int const& fd, Reference<ErrorInfo> const& err, int64_t const& size ) {
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Void>(new Truncate_implActor(fd, err, size));
															#line 3118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

	static eio_req* start_fsync(int fd, Promise<Void>& p, bool sync_metadata) {
#ifdef __APPLE__
		// Neither fsync() nor fdatasync() do the right thing on OS X!
		eio_req* req = (eio_req*)calloc(1, sizeof *req);
		req->type = EIO_CUSTOM;
		req->pri = 0;
		req->finish = eio_callback;
		req->data = &p;
		req->destroy = free_req;
		req->int1 = fd;
		req->feed = apple_fsync;
		eio_submit(req);
		return req;
#else
		if (sync_metadata)
			return eio_fsync(fd, 0, eio_callback, &p);
		else
			return eio_fdatasync(fd, 0, eio_callback, &p);
#endif
	}

																#line 3144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via sync_impl()
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Sync_implActor>
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Sync_implActorState {
															#line 3150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Sync_implActorState(int const& fd,Reference<ErrorInfo> const& err,bool const& sync_metadata = false) 
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fd(fd),
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   err(err),
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   sync_metadata(sync_metadata),
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(start_fsync(fd, p, sync_metadata))
															#line 3167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("sync_impl", reinterpret_cast<unsigned long>(this));

	}
	~Sync_implActorState() 
	{
		fdb_probe_actor_destroy("sync_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Sync_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Sync_implActor*>(this)->actor_wait_state = 1;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Sync_implActor, 0, Void >*>(static_cast<Sync_implActor*>(this)));
															#line 3190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Sync_implActorState();
		static_cast<Sync_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
		try {
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			err->report();
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (r->result)
															#line 3222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			{
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				error("SyncError", fd, r);
															#line 3226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			}
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (static_cast<Sync_implActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 3232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<Sync_implActor*>(this)->actor_wait_state = 2;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Sync_implActor, 1, Void >*>(static_cast<Sync_implActor*>(this)));
															#line 3237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 3257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Sync_implActor*>(this)->actor_wait_state > 0) static_cast<Sync_implActor*>(this)->actor_wait_state = 0;
		static_cast<Sync_implActor*>(this)->ActorCallback< Sync_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Sync_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont1Catch1(const Error& _e,int loopDepth=0) 
	{
		try {
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			e = _e;
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			StrictFuture<Void> __when_expr_2 = delay(0, taskID);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			if (static_cast<Sync_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<Sync_implActor*>(this)->actor_wait_state = 3;
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Sync_implActor, 2, Void >*>(static_cast<Sync_implActor*>(this)));
															#line 3369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Sync_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Sync_implActorState(); static_cast<Sync_implActor*>(this)->destroy(); return 0; }
															#line 3384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Sync_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Sync_implActorState();
		static_cast<Sync_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Sync_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Sync_implActorState(); static_cast<Sync_implActor*>(this)->destroy(); return 0; }
															#line 3396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Sync_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Sync_implActorState();
		static_cast<Sync_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<Sync_implActor*>(this)->actor_wait_state > 0) static_cast<Sync_implActor*>(this)->actor_wait_state = 0;
		static_cast<Sync_implActor*>(this)->ActorCallback< Sync_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Sync_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 3471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		return a_body1Catch1(e, loopDepth);
															#line 3479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<Sync_implActor*>(this)->actor_wait_state > 0) static_cast<Sync_implActor*>(this)->actor_wait_state = 0;
		static_cast<Sync_implActor*>(this)->ActorCallback< Sync_implActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< Sync_implActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< Sync_implActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int fd;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Reference<ErrorInfo> err;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	bool sync_metadata;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Error e;
															#line 3560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via sync_impl()
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Sync_implActor final : public Actor<Void>, public ActorCallback< Sync_implActor, 0, Void >, public ActorCallback< Sync_implActor, 1, Void >, public ActorCallback< Sync_implActor, 2, Void >, public FastAllocated<Sync_implActor>, public Sync_implActorState<Sync_implActor> {
															#line 3565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Sync_implActor>::operator new;
	using FastAllocated<Sync_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Sync_implActor, 0, Void >;
friend struct ActorCallback< Sync_implActor, 1, Void >;
friend struct ActorCallback< Sync_implActor, 2, Void >;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Sync_implActor(int const& fd,Reference<ErrorInfo> const& err,bool const& sync_metadata = false) 
															#line 3578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<Void>(),
		   Sync_implActorState<Sync_implActor>(fd, err, sync_metadata)
	{
		fdb_probe_actor_enter("sync_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sync_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sync_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Sync_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Sync_implActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< Sync_implActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<Void> sync_impl( int const& fd, Reference<ErrorInfo> const& err, bool const& sync_metadata = false ) {
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<Void>(new Sync_implActor(fd, err, sync_metadata));
															#line 3607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 3612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via size_impl()
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Size_implActor>
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Size_implActorState {
															#line 3618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Size_implActorState(int const& fd) 
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : fd(fd),
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_fstat(fd, 0, eio_callback, &p))
															#line 3631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("size_impl", reinterpret_cast<unsigned long>(this));

	}
	~Size_implActorState() 
	{
		fdb_probe_actor_destroy("size_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Size_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 3649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Size_implActor*>(this)->actor_wait_state = 1;
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Size_implActor, 0, Void >*>(static_cast<Size_implActor*>(this)));
															#line 3654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result)
															#line 3683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("FStatError", fd, r);
															#line 3687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		EIO_STRUCT_STAT* statdata = (EIO_STRUCT_STAT*)r->ptr2;
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!statdata)
															#line 3693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("FStatBufferError", fd, r);
															#line 3697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		size = statdata->st_size;
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<Size_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Size_implActor*>(this)->actor_wait_state = 2;
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Size_implActor, 1, Void >*>(static_cast<Size_implActor*>(this)));
															#line 3710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 3724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Size_implActor*>(this)->actor_wait_state > 0) static_cast<Size_implActor*>(this)->actor_wait_state = 0;
		static_cast<Size_implActor*>(this)->ActorCallback< Size_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Size_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Size_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Size_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Size_implActor*>(this)->SAV<int64_t>::futures) { (void)(size); this->~Size_implActorState(); static_cast<Size_implActor*>(this)->destroy(); return 0; }
															#line 3826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Size_implActor*>(this)->SAV< int64_t >::value()) int64_t(std::move(size)); // state_var_RVO
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Size_implActor*>(this)->SAV<int64_t>::futures) { (void)(size); this->~Size_implActorState(); static_cast<Size_implActor*>(this)->destroy(); return 0; }
															#line 3838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Size_implActor*>(this)->SAV< int64_t >::value()) int64_t(std::move(size)); // state_var_RVO
		this->~Size_implActorState();
		static_cast<Size_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Size_implActor*>(this)->actor_wait_state > 0) static_cast<Size_implActor*>(this)->actor_wait_state = 0;
		static_cast<Size_implActor*>(this)->ActorCallback< Size_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Size_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Size_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Size_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int fd;
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	int64_t size;
															#line 3919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via size_impl()
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Size_implActor final : public Actor<int64_t>, public ActorCallback< Size_implActor, 0, Void >, public ActorCallback< Size_implActor, 1, Void >, public FastAllocated<Size_implActor>, public Size_implActorState<Size_implActor> {
															#line 3924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Size_implActor>::operator new;
	using FastAllocated<Size_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Size_implActor, 0, Void >;
friend struct ActorCallback< Size_implActor, 1, Void >;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Size_implActor(int const& fd) 
															#line 3936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<int64_t>(),
		   Size_implActorState<Size_implActor>(fd)
	{
		fdb_probe_actor_enter("size_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("size_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("size_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Size_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Size_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<int64_t> size_impl( int const& fd ) {
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<int64_t>(new Size_implActor(fd));
															#line 3964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 3969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via stat_impl()
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Stat_implActor>
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Stat_implActorState {
															#line 3975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Stat_implActorState(std::string const& filename) 
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : filename(filename),
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   p(),
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   statdata(),
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_stat(filename.c_str(), 0, eio_callback, &p))
															#line 3990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("stat_impl", reinterpret_cast<unsigned long>(this));

	}
	~Stat_implActorState() 
	{
		fdb_probe_actor_destroy("stat_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = p.getFuture();
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Stat_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Stat_implActor*>(this)->actor_wait_state = 1;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Stat_implActor, 0, Void >*>(static_cast<Stat_implActor*>(this)));
															#line 4013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Stat_implActorState();
		static_cast<Stat_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (r->result)
															#line 4042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("StatError", 0, r);
															#line 4046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!r->ptr2)
															#line 4050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			error("StatBufferError", 0, r);
															#line 4054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		statdata = *EIO_STAT_BUF(r);
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<Stat_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Stat_implActor*>(this)->actor_wait_state = 2;
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Stat_implActor, 1, Void >*>(static_cast<Stat_implActor*>(this)));
															#line 4067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 4081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Stat_implActor*>(this)->actor_wait_state > 0) static_cast<Stat_implActor*>(this)->actor_wait_state = 0;
		static_cast<Stat_implActor*>(this)->ActorCallback< Stat_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Stat_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Stat_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Stat_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Stat_implActor*>(this)->SAV<EIO_STRUCT_STAT>::futures) { (void)(statdata); this->~Stat_implActorState(); static_cast<Stat_implActor*>(this)->destroy(); return 0; }
															#line 4183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Stat_implActor*>(this)->SAV< EIO_STRUCT_STAT >::value()) EIO_STRUCT_STAT(std::move(statdata)); // state_var_RVO
		this->~Stat_implActorState();
		static_cast<Stat_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Stat_implActor*>(this)->SAV<EIO_STRUCT_STAT>::futures) { (void)(statdata); this->~Stat_implActorState(); static_cast<Stat_implActor*>(this)->destroy(); return 0; }
															#line 4195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Stat_implActor*>(this)->SAV< EIO_STRUCT_STAT >::value()) EIO_STRUCT_STAT(std::move(statdata)); // state_var_RVO
		this->~Stat_implActorState();
		static_cast<Stat_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Stat_implActor*>(this)->actor_wait_state > 0) static_cast<Stat_implActor*>(this)->actor_wait_state = 0;
		static_cast<Stat_implActor*>(this)->ActorCallback< Stat_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Stat_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Stat_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Stat_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::string filename;
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Promise<Void> p;
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	EIO_STRUCT_STAT statdata;
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 4276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via stat_impl()
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Stat_implActor final : public Actor<EIO_STRUCT_STAT>, public ActorCallback< Stat_implActor, 0, Void >, public ActorCallback< Stat_implActor, 1, Void >, public FastAllocated<Stat_implActor>, public Stat_implActorState<Stat_implActor> {
															#line 4281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Stat_implActor>::operator new;
	using FastAllocated<Stat_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<EIO_STRUCT_STAT>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Stat_implActor, 0, Void >;
friend struct ActorCallback< Stat_implActor, 1, Void >;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Stat_implActor(std::string const& filename) 
															#line 4293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<EIO_STRUCT_STAT>(),
		   Stat_implActorState<Stat_implActor>(filename)
	{
		fdb_probe_actor_enter("stat_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("stat_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("stat_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Stat_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Stat_implActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<EIO_STRUCT_STAT> stat_impl( std::string const& filename ) {
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<EIO_STRUCT_STAT>(new Stat_implActor(filename));
															#line 4321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

																#line 4326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via dispatch_impl()
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class R, class Dispatch_implActor>
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Dispatch_implActorState {
															#line 4332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Dispatch_implActorState(std::function<R()> const& func) 
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		 : func(func),
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   data(func),
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   taskID(g_network->getCurrentTask()),
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		   r(eio_custom( [](eio_req* req) { auto data = reinterpret_cast<Dispatch<R>*>(req->data); try { data->result = data->func(); req->result = 0; } catch (Error& e) { data->result = e; req->result = -1; } catch (...) { data->result = unknown_error(); req->result = -1; } }, 0, [](eio_req* req) { if (EIO_CANCELLED(req)) return 0; auto data = reinterpret_cast<Dispatch<R>*>(req->data); Promise<Void> p = std::move(data->done); p.send(Void()); return 0; }, &data))
															#line 4345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
	{
		fdb_probe_actor_create("dispatch_impl", reinterpret_cast<unsigned long>(this));

	}
	~Dispatch_implActorState() 
	{
		fdb_probe_actor_destroy("dispatch_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				StrictFuture<Void> __when_expr_0 = data.done.getFuture();
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				if (static_cast<Dispatch_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<Dispatch_implActor*>(this)->actor_wait_state = 1;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Dispatch_implActor, 0, Void >*>(static_cast<Dispatch_implActor*>(this)));
															#line 4368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		this->~Dispatch_implActorState();
		static_cast<Dispatch_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(0, taskID);
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (static_cast<Dispatch_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Dispatch_implActor*>(this)->actor_wait_state = 2;
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Dispatch_implActor, 1, Void >*>(static_cast<Dispatch_implActor*>(this)));
															#line 4404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			g_network->setCurrentTask(taskID);
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			eio_cancel(r);
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(__current_error, loopDepth);
															#line 4418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<Dispatch_implActor*>(this)->actor_wait_state > 0) static_cast<Dispatch_implActor*>(this)->actor_wait_state = 0;
		static_cast<Dispatch_implActor*>(this)->ActorCallback< Dispatch_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Dispatch_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Dispatch_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Dispatch_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (data.result.isError())
															#line 4520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(data.result.getError(), loopDepth);
															#line 4524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Dispatch_implActor*>(this)->SAV<R>::futures) { (void)(data.result.get()); this->~Dispatch_implActorState(); static_cast<Dispatch_implActor*>(this)->destroy(); return 0; }
															#line 4528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Dispatch_implActor*>(this)->SAV< R >::value()) R(data.result.get());
		this->~Dispatch_implActorState();
		static_cast<Dispatch_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (data.result.isError())
															#line 4540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			return a_body1Catch1(data.result.getError(), loopDepth);
															#line 4544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		}
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!static_cast<Dispatch_implActor*>(this)->SAV<R>::futures) { (void)(data.result.get()); this->~Dispatch_implActorState(); static_cast<Dispatch_implActor*>(this)->destroy(); return 0; }
															#line 4548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		new (&static_cast<Dispatch_implActor*>(this)->SAV< R >::value()) R(data.result.get());
		this->~Dispatch_implActorState();
		static_cast<Dispatch_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<Dispatch_implActor*>(this)->actor_wait_state > 0) static_cast<Dispatch_implActor*>(this)->actor_wait_state = 0;
		static_cast<Dispatch_implActor*>(this)->ActorCallback< Dispatch_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Dispatch_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Dispatch_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Dispatch_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	std::function<R()> func;
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Dispatch<R> data;
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	TaskPriority taskID;
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	eio_req* r;
															#line 4627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
};
// This generated class is to be used only via dispatch_impl()
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class R>
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Dispatch_implActor final : public Actor<R>, public ActorCallback< Dispatch_implActor<R>, 0, Void >, public ActorCallback< Dispatch_implActor<R>, 1, Void >, public FastAllocated<Dispatch_implActor<R>>, public Dispatch_implActorState<R, Dispatch_implActor<R>> {
															#line 4634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Dispatch_implActor<R>>::operator new;
	using FastAllocated<Dispatch_implActor<R>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<R>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Dispatch_implActor<R>, 0, Void >;
friend struct ActorCallback< Dispatch_implActor<R>, 1, Void >;
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Dispatch_implActor(std::function<R()> const& func) 
															#line 4646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<R>(),
		   Dispatch_implActorState<R, Dispatch_implActor<R>>(func)
	{
		fdb_probe_actor_enter("dispatch_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("dispatch_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("dispatch_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Dispatch_implActor<R>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Dispatch_implActor<R>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class R>
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
[[nodiscard]] static Future<R> dispatch_impl( std::function<R()> const& func ) {
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	return Future<R>(new Dispatch_implActor<R>(func));
															#line 4676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

	static std::atomic<int32_t> want_poll;

																#line 4683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
// This generated class is to be used only via poll_eio()
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
template <class Poll_eioActor>
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Poll_eioActorState {
															#line 4689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Poll_eioActorState() 
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	{
															#line 4695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		fdb_probe_actor_create("poll_eio", reinterpret_cast<unsigned long>(this));

	}
	~Poll_eioActorState() 
	{
		fdb_probe_actor_destroy("poll_eio", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
			;
															#line 4709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		delete static_cast<Poll_eioActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		want_poll = 0;
															#line 4731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

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
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (!(eio_poll() == -1))
															#line 4747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		StrictFuture<Void> __when_expr_0 = yield();
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 4755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		static_cast<Poll_eioActor*>(this)->actor_wait_state = 1;
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Poll_eioActor, 0, Void >*>(static_cast<Poll_eioActor*>(this)));
															#line 4759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
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
		if (static_cast<Poll_eioActor*>(this)->actor_wait_state > 0) static_cast<Poll_eioActor*>(this)->actor_wait_state = 0;
		static_cast<Poll_eioActor*>(this)->ActorCallback< Poll_eioActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Poll_eioActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("poll_eio", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("poll_eio", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Poll_eioActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("poll_eio", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("poll_eio", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Poll_eioActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("poll_eio", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("poll_eio", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
		delete static_cast<Poll_eioActor*>(this);
															#line 4856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		return 0;

		return loopDepth;
	}
};
// This generated class is to be used only via poll_eio()
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
class Poll_eioActor final : public Actor<void>, public ActorCallback< Poll_eioActor, 0, Void >, public FastAllocated<Poll_eioActor>, public Poll_eioActorState<Poll_eioActor> {
															#line 4865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
public:
	using FastAllocated<Poll_eioActor>::operator new;
	using FastAllocated<Poll_eioActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< Poll_eioActor, 0, Void >;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	Poll_eioActor() 
															#line 4876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
		 : Actor<void>(),
		   Poll_eioActorState<Poll_eioActor>()
	{
		fdb_probe_actor_enter("poll_eio", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("poll_eio");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("poll_eio", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
static void poll_eio(  ) {
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"
	new Poll_eioActor();
															#line 4894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.g.h"
}

#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/AsyncFileEIO.actor.h"

	static void eio_want_poll() {
		want_poll = 1;
		// SOMEDAY: nullptr for deferred error, no analysis of correctness (itp)
		onMainThreadVoid([]() { poll_eio(); }, TaskPriority::PollEIO);
	}

	static int eio_callback(eio_req* req) {
		if (EIO_CANCELLED(req))
			return 0;
		Promise<Void> p = std::move(*(Promise<Void>*)req->data);
		p.send(Void());
		return 0;
	}

#ifdef __APPLE__
	static void apple_fsync(eio_req* req) { req->result = fcntl(req->int1, F_FULLFSYNC, 0); }
	static void free_req(eio_req* req) { free(req); }
#endif
};

#ifdef FILESYSTEM_IMPL
std::atomic<int32_t> AsyncFileEIO::want_poll = 0;
#endif

#include "flow/unactorcompiler.h"
#endif
#endif
