#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
/*
 * WatchFile.actor.h
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

// When actually compiled (NO_INTELLISENSE), include the generated
// version of this file.  In intellisense use the source version.
#if defined(NO_INTELLISENSE) && !defined(FLOW_WATCH_FILE_ACTOR_G_H)
#define FLOW_WATCH_FILE_ACTOR_G_H
#include "flow/WatchFile.actor.g.h"
#elif !defined(FLOW_WATCH_FILE_ACTOR_H)
#define FLOW_WATCH_FILE_ACTOR_H

#include <ctime>
#include <string>
#include "flow/IAsyncFile.h"
#include "flow/genericactors.actor.h"
#include "flow/actorcompiler.h"

															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
namespace {
// This generated class is to be used only via watchFileForChanges()
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
template <class WatchFileForChangesActor>
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
class WatchFileForChangesActorState {
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
public:
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	WatchFileForChangesActorState(std::string const& filename,AsyncTrigger* const& fileChanged,const int* const& intervalSeconds,const char* const& errorType) 
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		 : filename(filename),
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		   fileChanged(fileChanged),
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		   intervalSeconds(intervalSeconds),
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		   errorType(errorType)
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
	{
		fdb_probe_actor_create("watchFileForChanges", reinterpret_cast<unsigned long>(this));

	}
	~WatchFileForChangesActorState() 
	{
		fdb_probe_actor_destroy("watchFileForChanges", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			if (filename == "")
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
			{
															#line 42 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
				this->~WatchFileForChangesActorState();
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
				static_cast<WatchFileForChangesActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			firstRun = true;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			statError = false;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			lastModTime = 0;
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
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
		this->~WatchFileForChangesActorState();
		static_cast<WatchFileForChangesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			StrictFuture<std::time_t> __when_expr_0 = IAsyncFileSystem::filesystem()->lastWriteTime(filename);
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 1;
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchFileForChangesActor, 0, std::time_t >*>(static_cast<WatchFileForChangesActor*>(this)));
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
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
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		StrictFuture<Void> __when_expr_1 = delay(*intervalSeconds);
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 2;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchFileForChangesActor, 1, Void >*>(static_cast<WatchFileForChangesActor*>(this)));
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			if (e.code() == error_code_io_error)
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
			{
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
				TraceEvent(SevWarnAlways, errorType).detail("File", filename);
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
				statError = true;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
			}
			else
			{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
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
	int a_body1loopBody1cont2(std::time_t const& modtime,int loopDepth) 
	{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		if (firstRun)
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			lastModTime = modtime;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			firstRun = false;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		if (lastModTime != modtime || statError)
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		{
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			lastModTime = modtime;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			statError = false;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			fileChanged->trigger();
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::time_t && modtime,int loopDepth) 
	{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		if (firstRun)
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			lastModTime = modtime;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			firstRun = false;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
		if (lastModTime != modtime || statError)
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		{
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			lastModTime = modtime;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			statError = false;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
			fileChanged->trigger();
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::time_t const& modtime,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(modtime, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::time_t && modtime,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(modtime), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state > 0) static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 0;
		static_cast<WatchFileForChangesActor*>(this)->ActorCallback< WatchFileForChangesActor, 0, std::time_t >::remove();

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 0, std::time_t >*,std::time_t const& value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 0, std::time_t >*,std::time_t && value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchFileForChangesActor, 0, std::time_t >*,Error err) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(int loopDepth) 
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
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WatchFileForChangesActor*>(this)->actor_wait_state > 0) static_cast<WatchFileForChangesActor*>(this)->actor_wait_state = 0;
		static_cast<WatchFileForChangesActor*>(this)->ActorCallback< WatchFileForChangesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WatchFileForChangesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WatchFileForChangesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	std::string filename;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	AsyncTrigger* fileChanged;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	const int* intervalSeconds;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	const char* errorType;
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	bool firstRun;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	bool statError;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	std::time_t lastModTime;
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
};
// This generated class is to be used only via watchFileForChanges()
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
class WatchFileForChangesActor final : public Actor<Void>, public ActorCallback< WatchFileForChangesActor, 0, std::time_t >, public ActorCallback< WatchFileForChangesActor, 1, Void >, public FastAllocated<WatchFileForChangesActor>, public WatchFileForChangesActorState<WatchFileForChangesActor> {
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
public:
	using FastAllocated<WatchFileForChangesActor>::operator new;
	using FastAllocated<WatchFileForChangesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchFileForChangesActor, 0, std::time_t >;
friend struct ActorCallback< WatchFileForChangesActor, 1, Void >;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	WatchFileForChangesActor(std::string const& filename,AsyncTrigger* const& fileChanged,const int* const& intervalSeconds,const char* const& errorType) 
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
		 : Actor<Void>(),
		   WatchFileForChangesActorState<WatchFileForChangesActor>(filename, fileChanged, intervalSeconds, errorType)
	{
		fdb_probe_actor_enter("watchFileForChanges", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("watchFileForChanges");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("watchFileForChanges", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchFileForChangesActor, 0, std::time_t >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WatchFileForChangesActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
[[nodiscard]] static Future<Void> watchFileForChanges( std::string const& filename, AsyncTrigger* const& fileChanged, const int* const& intervalSeconds, const char* const& errorType ) {
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"
	return Future<Void>(new WatchFileForChangesActor(filename, fileChanged, intervalSeconds, errorType));
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.g.h"
}

#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/flow/include/flow/WatchFile.actor.h"

#include "flow/unactorcompiler.h"

#endif // FLOW_WATCH_FILE_ACTOR_H
